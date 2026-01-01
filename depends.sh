#!/bin/bash

# Generate dependency rules for Makefile from source files

SRC_DIR=$1
OUTPUT_FILE=$2

# Initialize output file
echo "# Automatically generated dependencies file." > "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Resolve source files
find "$SRC_DIR" \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -type f | \
while read -r file; do
    # Get relative path
    rel_path="${file#$SRC_DIR/}"

    target="\$(SRC)/$rel_path"
    
    # Get dependencies
    deps=""
    while IFS= read -r line; do
        inc=$(echo "$line" | sed -n 's/^[[:space:]]*#[[:space:]]*include[[:space:]]*"\([^"]*\)".*$/\1/p')
        if [ -n "$inc" ]; then
            if [[ "$inc" == */* ]]; then
                dep_path="\$(SRC)/$inc"
            else
                dir_part=$(dirname "$rel_path")
                if [ "$dir_part" = "." ]; then
                    dep_path="\$(SRC)/$inc"
                else
                    dep_path="\$(SRC)/$dir_part/$inc"
                fi
            fi
            deps+=" $dep_path"
        fi
    done < <(grep '^[[:space:]]*#[[:space:]]*include[[:space:]]*"' "$file")
    
    # Write dependency rule to output file
    echo "$target:$deps" >> "$OUTPUT_FILE"
done

echo "depends.sh: Dependencies file generated: $OUTPUT_FILE"