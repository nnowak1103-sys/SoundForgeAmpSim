@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0.."
if not exist logs mkdir logs
for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyy-MM-dd_HH-mm-ss"') do set TS=%%i
set MAINLOG=logs\Build_%TS%.log
set ERRLOG=logs\Errors_%TS%.log

echo SoundForge Amp Sim Windows Build > "%MAINLOG%"
echo Started %DATE% %TIME% >> "%MAINLOG%"

echo ---- Environment diagnostics ----
where cmake >> "%MAINLOG%" 2>> "%ERRLOG%" || goto missing_cmake
cmake --version >> "%MAINLOG%" 2>> "%ERRLOG%"
where git >> "%MAINLOG%" 2>> "%ERRLOG%"

echo ---- Configure ----
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 >> "%MAINLOG%" 2>> "%ERRLOG%" || goto fail

echo ---- Build Release ----
cmake --build build --config Release --parallel >> "%MAINLOG%" 2>> "%ERRLOG%" || goto fail

echo BUILD SUCCESS >> "%MAINLOG%"
echo Build complete. See %MAINLOG%
pause
exit /b 0

:missing_cmake
echo ERROR: Missing required tool: cmake >> "%ERRLOG%"
echo ERROR: Missing required tool: cmake
echo Use GitHub Actions for a no-local-tools build, or install CMake for local builds.
echo Logs: %MAINLOG% and %ERRLOG%
pause
exit /b 1

:fail
echo BUILD FAILED >> "%MAINLOG%"
echo Build failed. See %MAINLOG% and %ERRLOG%
pause
exit /b 1
