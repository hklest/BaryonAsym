#!/usr/bin/env python3
"""
Stage and push source files to hklest/BaryonAsym (development branch).
Excludes: *.root, *~, '#'* (emacs locks), fort.*, compiled dirs, logs,
          KaonTree/, LambdaTree/ (large directories).
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

# Editor backup/lock files
*~
\#*\#

# Compiled ROOT / C++ artifacts
*.so
*.d
*.pcm
*.o
*.a

# FORTRAN output
fort.*

# Logs and job output
*.log
*.err
log/

# Large binary files
dummyfile
dummyfile.root

# Steering files
*.steer

# Compiled binary dirs
amd64_rhel70/
x86_64-centos7-gcc9-opt/
x86_64-el9-gcc14-opt/

# Large data directories
H1LambdaC/KaonTree/
H1LambdaC/LambdaTree/
"""

COMMIT_MESSAGE = "Add analysis source files and scripts"

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

    # Untrack any previously committed files that should now be ignored
    run(["git", "rm", "-r", "--cached", "--ignore-unmatch",
         "*.root", "*~", "fort.*", "*.steer",
         "amd64_rhel70", "x86_64-centos7-gcc9-opt", "x86_64-el9-gcc14-opt",
         "log/", "KaonTree/", "LambdaTree/", "KaonTreeMC/", "LambdaTreeMC/", "LambdaTreeMC_PythiaBG/", "genMCTree/","KaonTreeMC_PythiaBG/"])

    run(["git", "add", "-A", "--", ".", ":!*.steer"])

    # Show what's staged before committing
    run(["git", "status"])

    input("\nReview staged files above. Press Enter to commit and push, or Ctrl+C to abort: ")

    run(["git", "commit", "-m", COMMIT_MESSAGE])
    run(["git", "push", "origin", "development"])
    print("\nDone! Pushed to origin/development.")
