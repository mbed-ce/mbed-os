#!/bin/bash -ex

# Create and activate venv
python3 -m venv .venv
source .venv/bin/activate
pip install json5 tabulate

git config --global --add safe.directory "$GITHUB_WORKSPACE"

# What branch/commit should we be comparing against?
if [ -z "${GITHUB_BASE_REF}" ]; then
  compare_ref="HEAD~1" # previous commit of main
else
  compare_ref="origin/${GITHUB_BASE_REF}" # base branch of PR
fi

# Run pinvalidate on each changed file
git diff --name-only --diff-filter=d "$compare_ref" \
  | ( grep '.*[\\|\/]PinNames.h$' || true ) \
  | while read file; do python ./hal/tests/pinvalidate/pinvalidate.py -vvvfp "${file}"; done

git diff --exit-code --diff-filter=d --color