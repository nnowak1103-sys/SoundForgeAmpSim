# SonicForge Pro - Build without installing CMake, Visual Studio, Python, or JUCE

Your error appears because you ran the **local developer build script**. Local builds require CMake and Visual Studio.

This package is set up so you do **not** need local tools. Build it using GitHub Actions instead.

## Do this

1. Create a new GitHub repository.
2. Upload the entire contents of this project folder to that repository.
3. Open the repository on GitHub.
4. Go to **Actions**.
5. Select **Windows Release Build**.
6. Click **Run workflow**.
7. When it finishes, open the workflow run.
8. Download the artifact named **SonicForgeProAmpSim-Windows**.

That artifact contains the Windows release output.

## Do not do this

Do not run:

```bat
scripts\build_windows_release_LOCAL_REQUIRES_CMAKE.bat
```

unless you intentionally want to compile locally and have installed CMake + Visual Studio Build Tools.

## What changed in this package

- `scripts\build_windows_release.bat` now only displays no-local-tools instructions.
- The original local build script has been renamed to `scripts\build_windows_release_LOCAL_REQUIRES_CMAKE.bat`.
- The GitHub Actions workflow is located at `.github\workflows\windows-release.yml`.
