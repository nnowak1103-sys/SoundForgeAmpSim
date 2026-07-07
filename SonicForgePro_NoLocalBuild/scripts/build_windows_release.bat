@echo off
setlocal

echo ============================================================
echo SonicForge Pro - No Local Tools Build
echo ============================================================
echo.
echo You are running the no-local-tools package.
echo.
echo This package is designed to be built by GitHub Actions, not on
 echo your PC. Do NOT run a local CMake build if you do not want to
 echo install CMake or Visual Studio.
echo.
echo What to do next:
echo 1. Upload this entire folder to a GitHub repository.
echo 2. Open the repository on GitHub.
echo 3. Go to Actions.
echo 4. Run "Windows Release Build".
echo 5. Download the artifact named "SonicForgeProAmpSim-Windows".
echo.
echo Full instructions are in:
echo NO_LOCAL_TOOLS_BUILD.md
echo.
echo The old local build script was renamed to:
echo scripts\build_windows_release_LOCAL_REQUIRES_CMAKE.bat
echo.
pause
exit /b 0
