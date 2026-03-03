#!/usr/bin/env python3
"""
Set up .gitignore and push only source code to hklest/BaryonAsym (development branch).
ROOT files, binaries, backup files, and compiled objects are excluded.
"""

import os
import subprocess
import sys

# Fix CVMFS Python 2.7 PYTHONHOME conflict
if "PYTHONHOME" in os.environ:
    del os.environ["PYTHONHOME"]
    os.execv(sys.executable, [sys.executable] + sys.argv)

REPO_DIR = "/data/dust/group/h1/klesthen/BaryonAsym"

GITIGNORE_CONTENT = """
# ROOT files
*.root

# Editor backup files
*~

# Compiled ROOT / C++ artifacts
*.so
*.d
*.pcm
*.o
*.a

# Logs
*.log
log/

# FORTRAN output
fort.*

# Compiled binaries in subdirs
amd64_rhel70/
x86_64-centos7-gcc9-opt/
"""

COMMIT_MESSAGE = "Add analysis scripts, H1LambdaC source, and .gitignore"

def run(cmd, cwd=REPO_DIR):
    print(f"$ {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if result.stdout:
        print(result.stdout)
    if result.stderr:
        print(result.stderr, file=sys.stderr)
    if result.returncode != 0:
        print(f"ERROR: command failed with return code {result.returncode}")
        sys.exit(result.returncode)
    return result

if __name__ == "__main__":
    # Write .gitignore
    gitignore_path = os.path.join(REPO_DIR, ".gitignore")
    with open(gitignore_path, "w") as f:
        f.write(GITIGNORE_CONTENT.strip() + "\n")
    print(f"Wrote {gitignore_path}")

    # Stage everything - .gitignore will filter out the ROOT files etc.
    run(["git", "add", ".gitignore"])
    run(["git", "add", "."])

    # Show what's actually staged before committing
    run(["git", "status"])

    input("\nReview the staged files above. Press Enter to commit and push, or Ctrl+C to abort: ")

    run(["git", "commit", "-m", COMMIT_MESSAGE])
    run(["git", "push", "origin", "development"])
    print("\nDone! Source files pushed to origin/development.")
