# Build project on Windows with Clang-cl.
#
# The script assumes that it will be called from inside the project directory.
#
# Usage: .ci\build-project-with-clang-cl.ps1 [build-directory-name]
# - build-directory-name: Optional name of build directory. Default: build.
#
# Example 1: .ci\build-project-with-clang-cl.ps1
# Example 2: .ci\build-project-with-clang-cl.ps1 build-clang

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

# Visual Studio installation directory
$VS_DIR = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath

# init development environment
Push-Location .
& "$VS_DIR\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64
Pop-Location

# Clang-cl
$CLANG = "$Env:VCINSTALLDIR/Tools/Llvm/x64/bin/clang.exe".Replace("\" ,"/")

Write-Host "---- build-project-with-clang-cl.ps1 ----"
Write-Host "BUILD_DIR: $BUILD_DIR"
Write-Host "VS_DIR: $VS_DIR"
Write-Host "CLANG: $CLANG"
Write-Host "-----------------------------------------"

New-Item -Name "$BUILD_DIR" -ItemType Directory
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="$CLANG" -DCMAKE_CXX_COMPILER="$CLANG" -DBUILD_TEST:BOOL=ON -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" -j --config Debug

