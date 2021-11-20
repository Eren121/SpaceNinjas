#!/bin/bash
#
# Usage: echo "file list...." | ./sort-files
#
#
# Sort a file list in entry, removing extra spaces / newline
# Useful to have a clean SRC variable in CMakeLists.txt

tr ' ' '\n'