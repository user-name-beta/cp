#!/bin/bash

# Generate dependency rules for Makefile from source files

SRC_DIR="$1"
OUTPUT_FILE="$2"
shift 2
INCLUDE_DIRS=("$@")

# Initialize output file
echo "# Automatically generated dependencies file." > "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Resolve source files
find "$SRC_DIR" \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -type f | \
while read -r file; do

    # Get target name

    rel_path=${file#$SRC_DIR/}
    target="\$(SRC)/$rel_path"
    
    # Get dependencies
    deps=""
    while IFS= read -r line; do
        dep_path=""
        if echo "$line" | grep -q '^[[:space:]]*#[[:space:]]*include[[:space:]]*"[^"]*".*$'; then
            # `#include "cptypes.h"`
            include=$(echo "$line" | sed -n 's/^[[:space:]]*#[[:space:]]*include[[:space:]]*"\([^"]*\)".*$/\1/p')
            dir=$(dirname "$file")
            if [ -f "$dir/$include" ]; then
                dep_path="$dir/$include"
            else
                for dir in "${INCLUDE_DIRS[@]}"; do
                    if [ -f "$dir/$include" ]; then
                        dep_path="$dir/$include"
                        break
                    fi
                done
            fi
        elif echo "$line" | grep -q '^[[:space:]]*#[[:space:]]*include[[:space:]]*<[^<>]*>.*$'; then
            # `#include <stdlib.h>`
            include=$(echo "$line" | sed -n 's/^[[:space:]]*#[[:space:]]*include[[:space:]]*<\([^<>]*\)>.*$/\1/p')
            for dir in "${INCLUDE_DIRS[@]}"; do
                if [ -f "$dir/$include" ]; then
                    dep_path="$dir/$include"
                    break
                fi
            done
        fi
        dep_path=${dep_path//\\//}
        if [ -n "$dep_path" ]; then
            deps+=" $dep_path"
        fi
    done < <(grep '^[[:space:]]*#[[:space:]]*include[[:space:]]*["<]' "$file")
    
    # Write into file
    if [ -n "$deps" ]; then
        echo "$target:$deps" >> "$OUTPUT_FILE"
    fi
done

echo "depends.sh: Dependencies file generated: $OUTPUT_FILE"
