 
# Build project.
#
# The script assumes that it will be called from inside the project directory.
#
# Usage: .ci\build-project.ps1 [build-directory-name]
# - build-directory-name: Optional name of build directory. Default: build.
#
# Example 1: .ci\build-project.ps1
# Example 2: .ci\build-project.ps1 build-clang

$ErrorActionPreference = "Stop"

$BUILD_DIR = $args[0]

if ($null -eq $BUILD_DIR)
{
    $BUILD_DIR = "build"
}

# if CMake is not in PATH then we assume that it is located in its normal install directory
if (-not(Get-Command cmake -ErrorAction SilentlyContinue))
{
    New-Alias -Name cmake -Value "$Env:ProgramFiles\CMake\bin\cmake.exe"
}

Write-Host "---- build-project.ps1 ----"
Write-Host "BUILD_DIR: $BUILD_DIR"
Write-Host "---------------------------"

New-Item -Name "$BUILD_DIR" -ItemType Directory
cmake -DCMAKE_BUILD_TYPE=Debug -B "$BUILD_DIR" -DDST_TESTS:BOOL=ON
cmake --build "$BUILD_DIR" -j --config Debug
