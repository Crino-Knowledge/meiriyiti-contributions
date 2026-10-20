"""Exercise the real pinned Core with its own disposable standard/custom examples."""

from pathlib import Path
import tempfile
import yaml

from contribution import DRAFT_ID, create_submission, prepare_submission, read_json, verify_submission, write_json


def run():
    for judge in ("standard", "custom"):
        with tempfile.TemporaryDirectory(prefix=f"daily-smoke-{judge}-") as tmp:
            source = Path(tmp) / "submission"
            create_submission(source, "投稿工具链自检", "CI", "工具自检", judge)
            metadata = read_json(source / "contribution.json")
            metadata.update(permission="允许署名发布、修改并用于本项目教学与评测",
                            source="ProbHub 0.7.1 内置示例，仅用于工具链测试")
            write_json(source / "contribution.json", metadata)
            if judge == "custom":
                manifest = source / DRAFT_ID / "probhub.yaml"
                cfg = yaml.safe_load(manifest.read_text(encoding="utf-8"))
                fixture = source / DRAFT_ID / "judge-fixtures/checker"
                fixture.mkdir(parents=True)
                (fixture / "valid.out").write_text("3\n", encoding="utf-8")
                (fixture / "wrong.out").write_text("4\n", encoding="utf-8")
                cfg["judge"]["qa"] = {
                    "schema_version": 1,
                    "robustness": {
                        "baseline": "valid",
                        "probes": ["empty", "truncated", "extra-token", "oversized"],
                    },
                    "cases": [
                        {"id": "valid", "purpose": "accept valid output", "case": "sample/1",
                         "contestant_output": "judge-fixtures/checker/valid.out", "expected": {"status": "AC"}},
                        {"id": "wrong", "purpose": "reject wrong output", "case": "sample/1",
                         "contestant_output": "judge-fixtures/checker/wrong.out", "expected": {"status": "WA"}},
                    ],
                }
                manifest.write_text(yaml.safe_dump(cfg, allow_unicode=True, sort_keys=False), encoding="utf-8")
            prepare_submission(source)
            verify_submission(source)
            print(f"{judge}: prepare + verify 通过", flush=True)


if __name__ == "__main__":
    run()
