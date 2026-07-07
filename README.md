# SoundForge Amp Sim

Native C++/JUCE starter project for a standalone amp simulator and VST3 plugin.

## What this is

This repository is structured so GitHub Actions can build on Windows without requiring your personal PC to install CMake, Visual Studio, JUCE, or Python.

## Start here

1. Upload this repository contents to GitHub.
2. Confirm this file exists at the repository root:

```text
.github/workflows/windows-release.yml
```

3. Go to **Actions** in GitHub.
4. Run **Windows Release Build**.
5. Download the build artifacts from the completed run.

## Local build

Local builds are optional. If you build locally, you need CMake and Visual Studio Build Tools.

```bat
scripts\build_windows_release.bat
```

## Notes

This is a professional native project skeleton with working CI/build structure and starter DSP architecture. Final commercial amp-model quality requires iterative DSP development, testing, IR assets, UI art, and plugin validation.
