#!/usr/bin/env bash

set -e  # exit on error

for script in plot_ex*.py; do
    echo "Running $script..."
    python "$script"
done