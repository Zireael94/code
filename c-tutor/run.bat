@echo off
setlocal
cd /d "%~dp0"
if not exist "c_tutor.exe" (
    echo c_tutor.exe not found. Building first...
    call build.bat
    if errorlevel 1 exit /b 1
)
.\c_tutor.exe
endlocal
