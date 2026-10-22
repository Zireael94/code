@echo off
setlocal
cd /d "%~dp0"
echo ============================================================
echo   C Language Ladder Tutor - Build
echo ============================================================
where gcc >nul 2>nul
if errorlevel 1 (
    echo [ERROR] gcc was not found. Install MinGW-w64 and add its bin folder to PATH.
    echo         Check with: gcc --version
    exit /b 1
)
echo [1/2] Compiling 12 chapters...
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -Iinclude src\*.c -o c_tutor.exe -lm -static
if errorlevel 1 (
    echo [ERROR] Build failed. Read the gcc output above and fix the first error.
    exit /b 1
)
echo [2/2] Build succeeded: c_tutor.exe
echo Run with: run.bat  or  c_tutor.exe
echo Help:     c_tutor.exe --help
endlocal
