#!/bin/sh

# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

# Script to run the Astyle formatter on the Mbed OS code base.
# Run before submitting code changes!

git diff --name-only --diff-filter=d origin/master \
  | ( grep '.*\.\(c\|cpp\|h\|hpp\)$' || true ) \
  | ( grep -v -f .codecheckignore || true ) \
  | while read file; do astyle -n --options=.astylerc "${file}"; done