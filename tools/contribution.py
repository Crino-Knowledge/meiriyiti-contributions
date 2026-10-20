"""Shared submission format and validation, also used by the official importer."""

import argparse
from functools import lru_cache
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import subprocess
import tempfile
import zipfile

import yaml


PROBHUB_VERSION = "0.7.1"
DRAFT_ID = "P0001"
SLUG = re.compile(r"[a-z0-9]+(?:-[a-z0-9]+)*")
MAX_FILE = 64 * 1024 * 1024
MAX_TOTAL = 512 * 1024 * 1024
PROBLEM_ROOT_FILES = {"probhub.yaml", "problem.md", "data.zip"}
PROBLEM_DIRS = {"code", "data", "assets", "judge-fixtures"}


def read_json(path):
    return json.loads(Path(path).read_text(encoding="utf-8"))


def write_json(path, value):
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def workspace_config(pid, title):
    return {
        "schema_version": 1,
        "contest": {"title": title, "author": "每日一题投稿"},
        "typst": {"directory": "typst-statement/每日一题", "creation_timestamp": 0},
        "problems": [{"id": pid, "directory": pid}],
        "lint": {"forbidden_patterns": ["TODO", "FIXME", "114514", "待补充"]},
    }


def write_workspace(root, pid, title):
    path = Path(root) / ".probhub/workspace.yaml"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(yaml.safe_dump(workspace_config(pid, title), allow_unicode=True, sort_keys=False),
                    encoding="utf-8")


def safe_relative(raw):
    path = PurePosixPath(raw)
    if not raw or "\\" in raw or path.is_absolute() or any(
        part in ("", ".", "..") or any(ch in part for ch in ':*?"<>|')
        or any(ord(ch) < 32 for ch in part) or part.endswith((".", " "))
        or part.lower() in (".git", ".github")
        or re.fullmatch(r"(con|prn|aux|nul|com[1-9]|lpt[1-9])(?:\..*)?", part, re.I)
        for part in raw.split("/")
    ):
        raise ValueError(f"不安全的相对路径: {raw}")
    return path


def source_files(problem):
    """Allow only problem sources and data; never import caches or executable hooks."""
    root = Path(problem)
    if root.is_symlink() or not root.is_dir():
        raise ValueError(f"题目目录必须是普通目录: {root}")
    files, total, seen = [], 0, set()
    for path in sorted(root.rglob("*")):
        rel = path.relative_to(root)
        safe_relative(rel.as_posix())
        if path.is_symlink():
            raise ValueError(f"不允许符号链接: {rel}")
        if rel.parts[0] == ".probhub":
            continue
        if any(part.startswith(".") for part in rel.parts):
            raise ValueError(f"不允许隐藏文件或 Git 属性配置: {rel}")
        if rel.parts[0] not in PROBLEM_DIRS and rel.as_posix() not in PROBLEM_ROOT_FILES:
            raise ValueError(f"不属于题目规范源的文件: {rel}")
        if path.is_dir():
            continue
        if not path.is_file():
            raise ValueError(f"不允许特殊文件: {rel}")
        if rel.parts[0] == "data" and (len(rel.parts) != 3 or rel.parts[1] not in ("sample", "secret")
                                      or path.suffix not in (".in", ".ans")):
            raise ValueError(f"测试数据必须是 data/sample 或 data/secret 下的 .in/.ans: {rel}")
        if path.suffix.lower() in (".exe", ".dll", ".so", ".o", ".pyc", ".pdf"):
            raise ValueError(f"不允许编译产物: {rel}")
        folded = rel.as_posix().casefold()
        if folded in seen:
            raise ValueError(f"Windows 文件名冲突: {rel}")
        seen.add(folded)
        size = path.stat().st_size
        total += size
        if size > MAX_FILE or total > MAX_TOTAL:
            raise ValueError("题目文件超过大小限制")
        files.append(path)
    return files


def source_path(problem, raw, prefix=None):
    if not isinstance(raw, str):
        raise ValueError("规范源路径必须是字符串")
    rel = safe_relative(raw)
    if prefix and (not rel.parts or rel.parts[0] != prefix):
        raise ValueError(f"源码路径必须在 {prefix}/ 下: {raw}")
    path = Path(problem).joinpath(*rel.parts)
    if not path.is_file() or path.is_symlink():
        raise ValueError(f"规范源文件缺失或为链接: {raw}")
    return path


def validate_archive(problem):
    problem = Path(problem)
    files = {}
    for folder in ("sample", "secret"):
        paths = sorted((problem / "data" / folder).glob("*"))
        inputs = {p.stem for p in paths if p.suffix == ".in"}
        answers = {p.stem for p in paths if p.suffix == ".ans"}
        if not inputs or inputs != answers or any(p.suffix not in (".in", ".ans") for p in paths):
            raise ValueError(f"{folder} 数据缺失或不成对")
        if folder == "secret" and not 10 <= len(inputs) <= 20:
            raise ValueError("隐藏测试点数量必须为 10 ~ 20")
        for path in paths:
            if path.name.casefold() in {name.casefold() for name in files}:
                raise ValueError("sample/secret 平铺名称冲突")
            content = path.read_bytes()
            if b"\r" in content:
                raise ValueError(f"数据必须使用 LF: {path.name}")
            files[path.name] = content
    with zipfile.ZipFile(problem / "data.zip") as archive:
        entries = archive.infolist()
        if len(entries) != len(files) or {e.filename for e in entries} != set(files):
            raise ValueError("data.zip 必须完整平铺且无重复文件")
        for entry in entries:
            content = files[entry.filename]
            if entry.file_size != len(content) or archive.read(entry) != content:
                raise ValueError(f"data.zip 与规范源数据不一致: {entry.filename}")
    return files


def unfenced_lines(text):
    fence = None
    for line in text.splitlines():
        stripped = line.strip()
        match = re.match(r"^([~" + chr(96) + r"]{3,})", stripped)
        if match and len(set(match[1])) == 1:
            marks = match[1]
            if fence is None:
                fence = marks
            elif marks[0] == fence[0] and len(marks) >= len(fence) and stripped == marks:
                fence = None
            continue
        if fence is None:
            yield line


def validate_problem(problem, pid, solution, *, require_data=True):
    problem, solution = Path(problem), Path(solution)
    source_files(problem)
    cfg = yaml.safe_load((problem / "probhub.yaml").read_text(encoding="utf-8"))
    if not isinstance(cfg, dict) or cfg.get("schema_version") != 1 or cfg.get("id") != pid:
        raise ValueError("题目必须使用 Schema v1 且 ID 与目录一致")
    if cfg.get("difficulty") != 0:
        raise ValueError("投稿题目 difficulty 必须为 0")
    title = cfg.get("name")
    if not isinstance(title, str) or not title.strip() or "\n" in title:
        raise ValueError("题名必须是非空单行文本")
    if cfg.get("statement", {}).get("source") != "problem.md":
        raise ValueError("题面源必须是 problem.md")
    statement = (problem / "problem.md").read_text(encoding="utf-8")
    outside = "\n".join(unfenced_lines(statement))
    headings = re.findall(r"^# (.+)$", outside, re.M)
    if headings != [title]:
        raise ValueError("题面必须恰好一个 H1，且与 name 一致")
    for heading in ("题面描述", "输入描述", "输出描述", "样例", "提示"):
        if not re.search(r"^### " + heading + r"\s*$", outside, re.M):
            raise ValueError(f"题面缺少章节: {heading}")
    if re.search(r"^#{1,6}\s+.*样例.*(?:输入|输出)", outside, re.M):
        raise ValueError("样例输入/输出必须使用粗体标签，不能使用标题")
    judge = cfg.get("judge", {})
    if judge.get("type") not in ("standard", "custom"):
        raise ValueError("目前投稿发布链只支持 standard/custom")
    source_path(problem, judge.get("validator"), "code")
    if judge["type"] == "custom":
        checker = source_path(problem, judge.get("checker"), "code")
        if checker.suffix != ".cpp":
            raise ValueError("OJ Checker 必须为 C++")
        if not judge.get("qa"):
            raise ValueError("新 custom 题目必须配置 judge.qa")
    elif judge.get("checker") or judge.get("interactor"):
        raise ValueError("standard 题不能配置 Checker/Interactor")
    solutions = cfg.get("solutions", {})
    accepted = solutions.get("accepted", [])
    if len(accepted) < 2:
        raise ValueError("必须配置两份独立标程")
    for filename in ("code/std.cpp", "code/std2.cpp"):
        item = next((s for s in accepted if s.get("file") == filename), {})
        if item.get("expected", {}).get("status") != "AC" or item.get("expected", {}).get("all") is not True:
            raise ValueError(f"{filename} 必须声明全量 AC")
    second = next(s for s in accepted if s.get("file") == "code/std2.cpp")
    independence = second.get("independence", {})
    if independence.get("from") != "code/std.cpp" or independence.get("basis") not in (
        "algorithm", "key_implementation"
    ) or not independence.get("note"):
        raise ValueError("std2.cpp 必须声明独立性")
    if not solutions.get("wrong"):
        raise ValueError("必须配置错解矩阵")
    for group in ("accepted", "brute", "wrong"):
        for item in solutions.get(group, []):
            source_path(problem, item.get("file"), "code")
    if not cfg.get("stress") or cfg["stress"].get("brute") != "code/std2.cpp":
        raise ValueError("stress 必须以 std2.cpp 独立对拍")
    source_path(problem, cfg["stress"].get("generator"), "code")
    data = cfg.get("data", {})
    if data.get("sample_dir") != "data/sample" or data.get("secret_dir") != "data/secret":
        raise ValueError("投稿采用标准 data/sample 和 data/secret 目录")
    recipes = data.get("recipes", [])
    cases = [r.get("case") for r in recipes]
    actual = {p.stem for p in (problem / "data/secret").glob("*.in")}
    if len(cases) != len(set(cases)) or (set(cases) != actual if require_data else not actual <= set(cases)):
        raise ValueError("secret 数据与 recipes 必须一一对应")
    if not 10 <= len(cases) <= 20:
        raise ValueError("recipes 必须登记 10 ~ 20 个隐藏测试点")
    for recipe in recipes:
        if recipe.get("generator"):
            source_path(problem, recipe["generator"], "code")
    if require_data:
        validate_archive(problem)
    if solution.is_symlink() or solution.parent.is_symlink() or not solution.is_file():
        raise ValueError("题解缺失或为链接")
    content = solution.read_text(encoding="utf-8")
    for heading in ("## 1. 题意简述", "## 2. 解题思路", "## 3. 常见易错点",
                    "## 4. 参考代码与语法精讲", "### 4.1 C++ 参考代码", "### 4.2 Python 参考代码"):
        if heading not in content.splitlines():
            raise ValueError(f"题解缺少章节: {heading}")
    return cfg


def validate_submission(root, *, require_data=True):
    root = Path(root)
    if root.is_symlink() or not root.is_dir():
        raise ValueError("投稿工作区必须是普通目录")
    allowed = {DRAFT_ID, ".probhub", "contribution.json", "solution.md", ".gitignore", "typst-statement"}
    if any(p.name not in allowed or p.is_symlink() for p in root.iterdir()):
        raise ValueError("投稿工作区含不允许的顶层文件或符号链接")
    metadata = read_json(root / "contribution.json")
    if not isinstance(metadata, dict):
        raise ValueError("contribution.json 必须为 JSON 对象")
    if metadata.get("schema_version") != 1 or metadata.get("problem_id") != DRAFT_ID:
        raise ValueError("contribution.json 必须使用 Schema v1 与 P0001")
    if not isinstance(metadata.get("topic"), str) or not metadata["topic"].strip():
        raise ValueError("必须填写考点 topic")
    authors = metadata.get("authors")
    if not isinstance(authors, list) or not authors or any(
        not isinstance(a, dict) or not isinstance(a.get("name"), str) or not a["name"].strip()
        for a in authors
    ):
        raise ValueError("必须填写投稿作者")
    if metadata.get("permission") != "允许署名发布、修改并用于本项目教学与评测":
        raise ValueError("必须明确同意署名发布与修改授权")
    if not isinstance(metadata.get("source"), str) or not metadata["source"].strip():
        raise ValueError("必须填写原创或改编来源 source")
    work = yaml.safe_load((root / ".probhub/workspace.yaml").read_text(encoding="utf-8"))
    if not isinstance(work, dict) or work.get("schema_version") != 1 or work.get("problems") != [
        {"id": DRAFT_ID, "directory": DRAFT_ID}
    ]:
        raise ValueError("每份投稿工作区只能登记 P0001")
    cfg = validate_problem(root / DRAFT_ID, DRAFT_ID, root / "solution.md", require_data=require_data)
    return metadata, cfg


def copy_problem(source, target):
    target = Path(target)
    if target.exists():
        raise ValueError(f"目标题目已存在: {target}")
    for path in source_files(source):
        destination = target / path.relative_to(source)
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(path, destination)


def package_data(problem):
    problem = Path(problem)
    with zipfile.ZipFile(problem / "data.zip", "w", zipfile.ZIP_DEFLATED) as archive:
        seen = set()
        for folder in ("sample", "secret"):
            for path in sorted((problem / "data" / folder).glob("*")):
                if path.suffix not in (".in", ".ans") or path.name.casefold() in seen:
                    raise ValueError("数据文件无效或平铺名称冲突")
                seen.add(path.name.casefold())
                archive.write(path, path.name)


@lru_cache(maxsize=1)
def probhub_command():
    executable = shutil.which("probhub.cmd" if os.name == "nt" else "probhub")
    if not executable:
        raise ValueError("未安装 ProbHub，请先运行 npm install -g probhub@0.7.1")
    if os.name == "nt":
        entry = Path(executable).parent / "node_modules/probhub/bin/probhub.js"
        if not entry.is_file() or not shutil.which("node"):
            raise ValueError("无法定位 npm 全局安装的 ProbHub Node 入口")
        command = [shutil.which("node"), str(entry)]
    else:
        command = [executable]
    version = subprocess.run([*command, "--version"], check=True, capture_output=True, text=True,
                             encoding="utf-8", timeout=30).stdout.strip()
    if version != PROBHUB_VERSION:
        raise ValueError(f"投稿工具固定使用 ProbHub {PROBHUB_VERSION}，当前为 {version}")
    return tuple(command)


def run_probhub(root, *args):
    root = Path(root).resolve()
    print(f"[ProbHub] {' '.join(args)}", flush=True)
    command = list(probhub_command())
    command.extend(["--workspace", str(root), "--json", *args])
    result = subprocess.run(command, cwd=root, capture_output=True, text=True, encoding="utf-8",
                            timeout=900, check=False)
    if result.stderr:
        print(result.stderr)
    try:
        body = json.loads(result.stdout)
    except ValueError as exc:
        raise ValueError(f"ProbHub 返回非 JSON: {result.stdout[:2000]}") from exc
    if result.returncode or body.get("ok") is not True:
        detail = json.dumps(body, ensure_ascii=False)
        raise ValueError(f"ProbHub {args[0]} 未通过: {detail[:4000]}")
    return body


def core_statement(text):
    """Adapt the project's Markdown convention only in a disposable Core verification copy."""
    replacements = {
        "### 题面描述": "## 题目描述", "### 输入描述": "## 输入格式",
        "### 输出描述": "## 输出格式", "### 样例": "**样例**", "### 提示": "## 提示",
    }
    lines, fence = [], None
    for line in text.splitlines():
        stripped = line.strip()
        match = re.match(r"^([~" + chr(96) + r"]{3,})", stripped)
        if match:
            marks = match[1]
            if len(set(marks)) == 1:
                if fence is None:
                    fence = marks
                elif marks[0] == fence[0] and len(marks) >= len(fence) and stripped == marks:
                    fence = None
                lines.append(line)
                continue
        lines.append(replacements.get(stripped, line) if fence is None else line)
    return "\n".join(lines) + "\n"


def verify_problem(root, pid, solution, *, prepare=False):
    """Run only inside the CI container or an explicitly trusted local workspace."""
    problem = Path(root) / pid
    validate_problem(problem, pid, solution, require_data=not prepare)
    statement = problem / "problem.md"
    statement.write_text(core_statement(statement.read_text(encoding="utf-8")), encoding="utf-8")
    # Initial lint may report missing local Judge evidence; final lint must have no warnings.
    run_probhub(root, "lint", pid)
    sample = run_probhub(root, "sample-check", pid, "--no-cache")["problems"][pid]
    if sample.get("final", {}).get("code") != "sample_answers_match":
        raise ValueError("样例未精确复现")
    run_probhub(root, "gen", pid, "--apply")
    # Generated bytes must reproduce the submitted archive, rather than silently accepting new data.
    if not prepare:
        validate_archive(problem)
    judged = run_probhub(root, "judge", pid, "--no-cache")["problems"][pid]
    if judged.get("final", {}).get("code") != "all_expectations_met":
        raise ValueError("Judge 未满足全部期望")
    cfg = yaml.safe_load((Path(root) / pid / "probhub.yaml").read_text(encoding="utf-8"))
    if cfg["judge"]["type"] == "custom":
        qa = run_probhub(root, "judge-qa", pid, "--no-cache")["problems"][pid]
        if qa.get("status") != "passed":
            raise ValueError("Checker QA 未通过")
    stressed = run_probhub(root, "stress", pid, "--rounds", "100", "--seed", "12345")["problems"][pid]
    if stressed.get("status") != "passed" or stressed.get("counterexample") is not None \
            or stressed.get("rounds_completed") != 100:
        raise ValueError("Stress 未完成 100 轮独立对拍")
    if prepare:
        package_data(problem)
        validate_archive(problem)
    lint = run_probhub(root, "lint", pid)
    if lint.get("errors") or any(p.get("errors") or p.get("warnings") for p in lint["problems"]):
        details = [{"id": p["id"], "errors": p.get("errors"), "warnings": p.get("warnings")}
                   for p in lint["problems"]]
        raise ValueError(f"最终 lint 必须 errors=0, warnings=0: {json.dumps(details, ensure_ascii=False)}")


def verify_submission(root):
    metadata, cfg = validate_submission(root)
    with tempfile.TemporaryDirectory(prefix="daily-review-") as tmp:
        work = Path(tmp)
        copy_problem(Path(root) / DRAFT_ID, work / DRAFT_ID)
        shutil.copyfile(Path(root) / "solution.md", work / "solution.md")
        write_workspace(work, DRAFT_ID, cfg["name"])
        verify_problem(work, DRAFT_ID, work / "solution.md")
    return metadata


def prepare_submission(root):
    """Generate and validate in a snapshot, then return only data and its verified archive."""
    root = Path(root)
    _, cfg = validate_submission(root, require_data=False)
    problem = root / DRAFT_ID
    def identity():
        return [(p.relative_to(problem).as_posix(), hashlib.sha256(p.read_bytes()).hexdigest())
                for p in source_files(problem)]
    before = identity()
    with tempfile.TemporaryDirectory(prefix="daily-prepare-") as tmp:
        work = Path(tmp)
        copy_problem(problem, work / DRAFT_ID)
        shutil.copyfile(root / "solution.md", work / "solution.md")
        write_workspace(work, DRAFT_ID, cfg["name"])
        verify_problem(work, DRAFT_ID, work / "solution.md", prepare=True)
        if identity() != before:
            raise ValueError("验证期间题目规范源发生变化，未写回数据")
        shutil.copytree(work / DRAFT_ID / "data", problem / "data", dirs_exist_ok=True)
        shutil.copyfile(work / DRAFT_ID / "data.zip", problem / "data.zip")


def create_submission(destination, title, author, topic, judge="standard"):
    destination = Path(destination)
    if destination.exists():
        raise ValueError("投稿目录已存在，不覆盖")
    destination.mkdir(parents=True)
    write_workspace(destination, DRAFT_ID, title)
    workspace = yaml.safe_load((destination / ".probhub/workspace.yaml").read_text(encoding="utf-8"))
    workspace["problems"] = []
    (destination / ".probhub/workspace.yaml").write_text(
        yaml.safe_dump(workspace, allow_unicode=True, sort_keys=False), encoding="utf-8")
    run_probhub(destination, "new", DRAFT_ID, "--name", title, "--judge", judge)
    manifest = destination / DRAFT_ID / "probhub.yaml"
    cfg = yaml.safe_load(manifest.read_text(encoding="utf-8"))
    cfg["difficulty"] = 0
    cfg["domjudge"] = {"include_pdf": False}
    cfg["stress"] = {
        "generator": "code/inmaker.cpp", "args": ["random", "{seed}"],
        "brute": "code/std2.cpp", "rounds": 100, "time_limit": 1, "tool_timeout": 5,
    }
    cfg["data"]["recipes"].extend({
        "case": f"random{index:02d}", "generator": "code/inmaker.cpp", "args": ["random", str(index)]
    } for index in range(2, 10))
    manifest.write_text(yaml.safe_dump(cfg, allow_unicode=True, sort_keys=False), encoding="utf-8")
    statement_path = destination / DRAFT_ID / "problem.md"
    statement = statement_path.read_text(encoding="utf-8")
    for old, new in (("## 题目描述", "### 题面描述"), ("## 输入格式", "### 输入描述"),
                     ("## 输出格式", "### 输出描述")):
        statement = statement.replace(old, new)
    statement += "\n### 样例\n"
    for index, sample in enumerate(sorted((destination / DRAFT_ID / "data/sample").glob("*.in")), 1):
        statement += (
            f"\n**【样例 {index}】**\n\n**样例输入：**\n\n~~~text\n"
            + sample.read_text(encoding="utf-8").rstrip("\n")
            + "\n~~~\n\n**样例输出：**\n\n~~~text\n"
            + sample.with_suffix(".ans").read_text(encoding="utf-8").rstrip("\n") + "\n~~~\n"
        )
    statement += "\n### 提示\n\n请补充样例解释，并替换脚手架示例内容。\n"
    statement_path.write_text(statement, encoding="utf-8")
    write_json(destination / "contribution.json", {
        "schema_version": 1, "problem_id": DRAFT_ID, "topic": topic,
        "authors": [{"name": author}], "permission": "", "source": "",
    })
    (destination / "solution.md").write_text(
        f"# {title} 题解\n\n## 1. 题意简述\n\n## 2. 解题思路\n\n## 3. 常见易错点\n\n"
        "所有题目均不强求在最后一行输出后换行。\n\n## 4. 参考代码与语法精讲\n\n"
        "### 4.1 C++ 参考代码\n\n#### C++ 语法细节讲解\n\n"
        "### 4.2 Python 参考代码\n\n#### Python 语法细节讲解\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description="每日一题投稿：创建、静态检查、完整验证、平铺打包")
    sub = parser.add_subparsers(dest="command", required=True)
    new = sub.add_parser("new")
    new.add_argument("slug")
    new.add_argument("--title", required=True)
    new.add_argument("--author", required=True)
    new.add_argument("--topic", required=True)
    new.add_argument("--judge", choices=("standard", "custom"), default="standard")
    for command in ("check", "verify", "prepare", "pack"):
        item = sub.add_parser(command)
        item.add_argument("workspace", type=Path)
    args = parser.parse_args()
    if args.command == "new":
        if not SLUG.fullmatch(args.slug) or len(args.slug) > 64:
            raise ValueError("slug 必须是最多 64 字符的小写英文、数字及单连字符")
        create_submission(Path("submissions") / args.slug, args.title, args.author, args.topic, args.judge)
    elif args.command == "pack":
        package_data(args.workspace / DRAFT_ID)
        validate_submission(args.workspace)
    elif args.command == "check":
        validate_submission(args.workspace)
    elif args.command == "prepare":
        prepare_submission(args.workspace)
    else:
        verify_submission(args.workspace)
    print("通过")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, KeyError, TypeError, zipfile.BadZipFile, subprocess.SubprocessError) as error:
        raise SystemExit(str(error))
