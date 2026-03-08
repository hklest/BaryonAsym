#!/usr/bin/env python3
"""
Overwrite local repository contents with a remote branch snapshot.

This script is intentionally destructive:
  1) fetches from origin
  2) checks out the target branch
  3) hard-resets to origin/<branch>
  4) removes untracked files/dirs (git clean -fdx)

Default target branch: development
Default repo path: /data/dust/group/h1/klesthen/BaryonAsym
"""

import argparse
import os
import subprocess
import sys

# Fix CVMFS Python 2.7 PYTHONHOME conflict
if "PYTHONHOME" in os.environ:
    del os.environ["PYTHONHOME"]
    os.execv(sys.executable, [sys.executable] + sys.argv)

DEFAULT_REPO_DIR = "/data/dust/group/h1/klesthen/BaryonAsym"
DEFAULT_REMOTE = "origin"
DEFAULT_BRANCH = "development"


def run(cmd, cwd):
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


def parse_args():
    parser = argparse.ArgumentParser(
        description="Overwrite local repo with remote branch contents"
    )
    parser.add_argument(
        "--repo-dir",
        default=DEFAULT_REPO_DIR,
        help=f"Path to local git repo (default: {DEFAULT_REPO_DIR})",
    )
    parser.add_argument(
        "--remote",
        default=DEFAULT_REMOTE,
        help=f"Remote name (default: {DEFAULT_REMOTE})",
    )
    parser.add_argument(
        "--branch",
        default=DEFAULT_BRANCH,
        help=f"Target branch (default: {DEFAULT_BRANCH})",
    )
    parser.add_argument(
        "--yes",
        action="store_true",
        help="Skip interactive confirmation",
    )
    return parser.parse_args()


def ensure_repo(repo_dir):
    if not os.path.isdir(repo_dir):
        print(f"ERROR: repo directory does not exist: {repo_dir}")
        sys.exit(2)

    git_dir = os.path.join(repo_dir, ".git")
    if not os.path.isdir(git_dir):
        print(f"ERROR: not a git repository (missing .git): {repo_dir}")
        sys.exit(2)


def main():
    args = parse_args()
    repo_dir = os.path.abspath(args.repo_dir)
    remote = args.remote
    branch = args.branch

    ensure_repo(repo_dir)

    print("\nThis will OVERWRITE local files with remote state:\n")
    print(f"  repo   : {repo_dir}")
    print(f"  remote : {remote}")
    print(f"  branch : {branch}")
    print("\nActions:")
    print("  - git fetch <remote>")
    print("  - git checkout <branch>")
    print("  - git reset --hard <remote>/<branch>")
    print("  - git clean -fdx")

    if not args.yes:
        answer = input("\nType 'overwrite' to continue: ").strip().lower()
        if answer != "overwrite":
            print("Aborted.")
            return

    run(["git", "status", "--short"], cwd=repo_dir)
    run(["git", "fetch", remote], cwd=repo_dir)
    run(["git", "checkout", branch], cwd=repo_dir)
    run(["git", "reset", "--hard", f"{remote}/{branch}"], cwd=repo_dir)
    run(["git", "clean", "-fdx"], cwd=repo_dir)

    print("\nDone! Local repository now matches remote branch exactly.")


if __name__ == "__main__":
    main()
