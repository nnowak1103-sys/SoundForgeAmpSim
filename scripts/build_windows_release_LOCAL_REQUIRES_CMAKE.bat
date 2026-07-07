@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM ============================================================
REM SonicForge Pro Amp Sim - Windows Release Build
REM Creates timestamped logs so build errors are easy to diagnose.
REM ============================================================

cd /d "%~dp0\.."
set "ROOT=%CD%"
set "LOGDIR=%ROOT%\logs"
set "BUILDDIR=%ROOT%\build"
set "RELEASEDIR=%ROOT%\release"

if not exist "%LOGDIR%" mkdir "%LOGDIR%"
if not exist "%BUILDDIR%" mkdir "%BUILDDIR%"
if not exist "%RELEASEDIR%" mkdir "%RELEASEDIR%"

for /f %%i in ('powershell -NoProfile -ExecutionPolicy Bypass -Command "Get-Date -Format yyyy-MM-dd_HH-mm-ss"') do set "TS=%%i"
set "MAINLOG=%LOGDIR%\Build_!TS!.log"
set "ENVLOG=%LOGDIR%\Environment_!TS!.log"
set "CONFIGLOG=%LOGDIR%\Configure_!TS!.log"
set "COMPILELOG=%LOGDIR%\Compile_!TS!.log"
set "PACKAGELOG=%LOGDIR%\Packaging_!TS!.log"
set "ERRORLOG=%LOGDIR%\Errors_!TS!.log"

call :Banner "SonicForge Pro Amp Sim Release Build" "%MAINLOG%"
echo Project root: %ROOT%>> "%MAINLOG%"
echo Started: %DATE% %TIME%>> "%MAINLOG%"
echo.>> "%MAINLOG%"

echo.
echo ============================================================
echo SonicForge Pro Amp Sim - Release Build
echo ============================================================
echo Main log: %MAINLOG%
echo.

call :RunToLog "Environment diagnostics" "%ENVLOG%" powershell -NoProfile -ExecutionPolicy Bypass -Command "Write-Host 'Windows:'; cmd /c ver; Write-Host ''; Write-Host 'PowerShell:' $PSVersionTable.PSVersion; Write-Host ''; Write-Host 'PATH:'; $env:PATH; Write-Host ''; Write-Host 'CMake:'; if(Get-Command cmake -ErrorAction SilentlyContinue){ cmake --version } else { Write-Host 'NOT FOUND' }; Write-Host ''; Write-Host 'Git:'; if(Get-Command git -ErrorAction SilentlyContinue){ git --version } else { Write-Host 'NOT FOUND' }; Write-Host ''; Write-Host 'MSBuild:'; if(Get-Command msbuild -ErrorAction SilentlyContinue){ msbuild -version } else { Write-Host 'NOT FOUND IN PATH - Visual Studio generator may still work if CMake can find it.' }"

call :RequireTool "cmake" "Install CMake for local builds, OR use GitHub Actions for a no-local-tools build. See NO_LOCAL_TOOLS_BUILD.md."
if errorlevel 1 goto :Fail

call :RunToLog "Configure CMake" "%CONFIGLOG%" cmake -S "%ROOT%" -B "%BUILDDIR%" -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto :Fail

call :RunToLog "Build standalone and VST3" "%COMPILELOG%" cmake --build "%BUILDDIR%" --config Release --target SonicForgePro_VST3 SonicForgePro_Standalone -- /m
if errorlevel 1 goto :Fail

call :RunToLog "Collect portable release files" "%PACKAGELOG%" powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\scripts\collect_release.ps1"
if errorlevel 1 goto :Fail

if exist "%ROOT%\packaging\SonicForgeProAmpSim.iss" (
    where iscc.exe >nul 2>nul
    if not errorlevel 1 (
        call :RunToLog "Build Inno Setup installer" "%LOGDIR%\Installer_!TS!.log" iscc.exe "%ROOT%\packaging\SonicForgeProAmpSim.iss"
        if errorlevel 1 goto :Fail
    ) else (
        echo Inno Setup is not installed or iscc.exe is not in PATH. Skipping installer build.>> "%MAINLOG%"
        echo Installer skipped: iscc.exe not found. Portable ZIP can still be used.>> "%PACKAGELOG%"
    )
)

echo.>> "%MAINLOG%"
echo BUILD SUCCESSFUL>> "%MAINLOG%"
echo Finished: %DATE% %TIME%>> "%MAINLOG%"
echo.
echo ============================================================
echo BUILD SUCCESSFUL
echo ============================================================
echo Release folder: %RELEASEDIR%
echo Main log: %MAINLOG%
echo.
pause
exit /b 0

:RequireTool
where %~1 >nul 2>nul
if errorlevel 1 (
    echo Missing required tool: %~1> "%ERRORLOG%"
    echo Suggested fix: %~2>> "%ERRORLOG%"
    echo Missing required tool: %~1>> "%MAINLOG%"
    echo Suggested fix: %~2>> "%MAINLOG%"
    echo.
    echo ERROR: Missing required tool: %~1
    echo Suggested fix: %~2
    echo Details: %ERRORLOG%
    exit /b 1
)
exit /b 0

:RunToLog
set "STEP=%~1"
set "STEPLOG=%~2"
shift
shift

echo.
echo ---- %STEP% ----
echo ---- %STEP% ---->> "%MAINLOG%"
echo Started: %DATE% %TIME%> "%STEPLOG%"
echo Command: %*>> "%STEPLOG%"
echo.>> "%STEPLOG%"

%* >> "%STEPLOG%" 2>&1
set "RC=%ERRORLEVEL%"

type "%STEPLOG%" >> "%MAINLOG%"
echo.>> "%MAINLOG%"
echo Exit code: %RC%>> "%MAINLOG%"

if not "%RC%"=="0" (
    echo Step failed: %STEP%> "%ERRORLOG%"
    echo Exit code: %RC%>> "%ERRORLOG%"
    echo Step log: %STEPLOG%>> "%ERRORLOG%"
    echo Main log: %MAINLOG%>> "%ERRORLOG%"
    echo.>> "%ERRORLOG%"
    type "%STEPLOG%" >> "%ERRORLOG%"
    echo.
    echo ERROR: %STEP% failed with exit code %RC%.
    echo See: %STEPLOG%
    exit /b %RC%
)
exit /b 0

:Banner
set "TITLE=%~1"
set "FILE=%~2"
echo ============================================================ > "%FILE%"
echo %TITLE%>> "%FILE%"
echo ============================================================>> "%FILE%"
exit /b 0

:Fail
echo.>> "%MAINLOG%"
echo BUILD FAILED>> "%MAINLOG%"
echo Finished: %DATE% %TIME%>> "%MAINLOG%"
echo.
echo ============================================================
echo BUILD FAILED
echo ============================================================
echo Main log: %MAINLOG%
echo Error summary: %ERRORLOG%
echo.
pause
exit /b 1
