@echo off
setlocal EnableExtensions EnableDelayedExpansion

set ROOT=%~dp0..
cd /d "%ROOT%"

if not exist logs mkdir logs
for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyy-MM-dd_HH-mm-ss"') do set TS=%%i
set MAINLOG=%ROOT%\logs\Build_%TS%.log
set ERRLOG=%ROOT%\logs\Errors_%TS%.log
set ENVLOG=%ROOT%\logs\Environment_%TS%.log

call :log "SoundForge Amp Sim Windows Release Build"
call :log "Root: %ROOT%"
call :log "Main log: %MAINLOG%"

echo ---- Environment diagnostics ----
(
  echo Date: %DATE% %TIME%
  echo Computer: %COMPUTERNAME%
  echo User: %USERNAME%
  ver
  echo PATH=%PATH%
  where cmake
  cmake --version
  where git
  git --version
) > "%ENVLOG%" 2>&1

if errorlevel 1 (
  call :fail "Missing required tool: cmake or git. Install tools for local builds, or use GitHub Actions."
)

call :run "Configure CMake" cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
call :run "Build Release" cmake --build build --config Release --parallel

if not exist dist mkdir dist
xcopy /Y /E /I "build\SoundForgeAmpSim_artefacts\Release" "dist" >> "%MAINLOG%" 2>&1

call :log "BUILD SUCCESS"
echo Build completed. Logs are in logs\
pause
exit /b 0

:run
set STEP=%~1
shift
call :log "---- %STEP% ----"
%* >> "%MAINLOG%" 2>> "%ERRLOG%"
if errorlevel 1 call :fail "%STEP% failed"
exit /b 0

:log
echo %~1
echo %~1 >> "%MAINLOG%"
exit /b 0

:fail
echo ERROR: %~1
echo ERROR: %~1 >> "%MAINLOG%"
echo ERROR: %~1 >> "%ERRLOG%"
echo.
echo BUILD FAILED. Main log: %MAINLOG%
echo Error log: %ERRLOG%
pause
exit /b 1
