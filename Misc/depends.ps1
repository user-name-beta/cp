# This file is used to generate depends.d file for makefile.

param(
    [string]$src = "src",
    [string]$output = "build/depends.d",
    [string[]]$includeDirs = @('.', 'src')
)

# Write the head of depends.d file
@"
# Automatically generated dependencies file for Makefile.

"@ | Out-File $output -Encoding ASCII

# Supported source and header file extensions
$sourceExts = @('.cpp', '.c', '.cc', '.cxx')
$headerExts = @('.h', '.hpp', '.hh', '.hxx', '.h++', '.hxx')

# Process the #include statements in a source file to generate dependencies
function Process-Includes {
    param(
        [string]$filePath,
        [string]$relativePath
    )
    
    $includes = @()
    $lines = Get-Content $filePath -Raw
    $pattern = '(?m)^\s*#\s*include\s*(?:<([^<>]*)>|"([^"]*)").*$'
    $regexMatches = [regex]::Matches($lines, $pattern)
    
    foreach ($match in $regexMatches) {
        if ($match.Groups[1].Success) {
            # `#include <stdint.h>`
            $includeFile = $match.Groups[1].Value.Trim()
            foreach ($dir in $script:includeDirs) {
                $fullPath = Join-Path $dir $includeFile
                if (Test-Path $fullPath -PathType Leaf) {
                    $includes += $fullPath
                    break
                }
            }
        } elseif ($match.Groups[2].Success) {
            # `#include "cptypes.h"`
            $includeFile = $match.Groups[2].Value.Trim()
            $dir = Split-Path $filePath -Parent
            $fullPath = Join-Path $dir $includeFile
            if (Test-Path $fullPath -PathType Leaf) {
                $fullPath = $fullPath.replace("\", "/")
                $includes += $fullPath
            } else {
                foreach ($dir in $script:includeDirs) {
                    $fullPath = Join-Path $dir $includeFile
                    if (Test-Path $fullPath -PathType Leaf) {
                        $fullPath = $fullPath.replace("\", "/")
                        $includes += $fullPath
                    }
                }
            }
        }
    }
    return $includes
}

# Scan the directory and generate dependencies
function Scan-Directory {
    param(
        [string]$directory
    )
    
    # Get all files in the directory
    $allFiles = Get-ChildItem -Path $directory -File
    
    foreach ($file in $allFiles) {
        $ext = $file.Extension.ToLower()
        
        # Check if the file is a source or header file
        if (($sourceExts -contains $ext) -or ($headerExts -contains $ext)) {
            $script:fileCount++
            
            # Get relative path
            $relativePath = $file.FullName.Substring((Resolve-Path $src).Path.Length + 1)
            $relativePath = $relativePath.Replace('\', '/')
            
            # Get the current directory of the file
            $currentDir = Split-Path $relativePath -Parent
            if ([string]::IsNullOrEmpty($currentDir)) {
                $currentDir = "."
            }
            
            # Generate target name
            $target = "`$(SRC)/$relativePath"
            
            # Get includes
            $includes = Process-Includes $file.FullName $relativePath
            # Process includes and generate dependencies
            $uniqueDeps = @()
            foreach ($inc in $includes) {
                if ($uniqueDeps -notcontains $inc) {
                    $uniqueDeps += $inc
                }
            }
            
            # Write dependencies to depends.d file
            "$target`: $($uniqueDeps -join ' ')" | Out-File $output -Append -Encoding ASCII
        }
    }
    
    # Process subdirectory recursively
    $subDirs = Get-ChildItem -Path $directory -Directory
    foreach ($subDir in $subDirs) {
        Scan-Directory $subDir.FullName
    }
}

# Starting the scanning process
Scan-Directory $src

Write-Host "depends.ps1: Successfully generated dependencies file: $output"
