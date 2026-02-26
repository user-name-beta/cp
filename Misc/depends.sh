#!/bin/bash

# Generate dependency rules for Makefile from source files

SRC_DIR="$1"
BUILD_DIR="$2"

# Remove the trailing slash

SRC_DIR="${SRC_DIR%/}"
BUILD_DIR="${BUILD_DIR%/}"

shift 2

INCLUDE_DIRS=()

while true; do
    if [ "$1" = "--" ]; then
        shift
        break
    fi
    INCLUDE_DIRS+=("$1")
    shift
done

FILES=("$@")

OUTPUT_FILE="$BUILD_DIR/depends.d"
DEP_DIR="$BUILD_DIR/depends"

mkdir -p "$DEP_DIR"

# Initialize output file
echo "# Automatically generated dependencies file." > "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Resolve source files

for file in "${FILES[@]}"; do
    echo "$file"
    file_dir=$(dirname "$file")
    dir=${file_dir/#"$SRC_DIR"/"$DEP_DIR"}
    mkdir -p "$dir"
    OUT_FILE="$dir/$(basename "$file").d"
    echo "include $OUT_FILE" >> "$OUTPUT_FILE"

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
    echo "$target:$deps" > "$OUT_FILE"
done

echo "depends.sh: Dependencies file generated: $OUTPUT_FILE"
