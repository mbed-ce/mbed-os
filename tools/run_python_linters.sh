#!/bin/bash -e

#
# Copyright (c) 2025 Jamie Smith.
# SPDX-License-Identifier: Apache-2.0
#

# Script which runs Python formatters and linters
# This is executed by the GitHub Actions CI build but also can be run locally.

# Go to script directory
cd "$(dirname $0)"

# Activate Mbed OS virtualenv
if command -v mbedhtrun >/dev/null 2>&1; then
  echo "Mbed OS python environment appears to already be activated."
elif [ -e "../venv/Scripts/activate" ]; then
  source "../venv/Scripts/activate"
elif [ -e "../venv/bin/activate" ]; then
  source "../venv/bin/activate"
else
  echo "Failed to find Mbed OS virtualenv in ../venv and Python packages not installed to global environment."
  exit 1
fi

if ! command -v ruff >/dev/null 2>&1; then
  echo "Linters optional dependency of Mbed not installed. Please run 'mbed-os/venv/bin/pip install mbed-os/tools[linters]'."
fi

echo ">> Formatting with Ruff..."
ruff format