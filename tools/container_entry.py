import shutil
import sys
import tempfile
from pathlib import Path

# Keep imports on the trusted read-only image, rather than the input or temporary workspace.
sys.path[:] = [p for p in sys.path if p not in ("", "/tmp")]
from contribution import verify_problem, verify_submission


def main():
    mode, pid = sys.argv[1:]
    if mode == "submission":
        verify_submission("/input")
    elif mode == "official":
        with tempfile.TemporaryDirectory(prefix="daily-official-") as tmp:
            shutil.copytree("/input", Path(tmp) / "workspace")
            root = Path(tmp) / "workspace"
            verify_problem(root, pid, root / "solution.md")
    elif mode == "smoke":
        from smoke import run
        run()
    else:
        raise SystemExit("unknown verification mode")
    print("完整验证通过")


if __name__ == "__main__":
    main()
