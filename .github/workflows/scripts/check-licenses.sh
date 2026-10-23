#!/bin/bash -e

# Figure out main branch name to compare against
if [ -z "$GITHUB_BASE_REF" ]; then
  main_branch="main"
else
  main_branch="$GITHUB_BASE_REF"
fi

# Get all changed files vs main branch
changed_files=$(git diff --name-only --diff-filter=d "origin/$main_branch")

# Check licenses
mbed-license-checker $changed_files