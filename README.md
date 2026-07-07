# SoundForge Amp Sim - Engineering Milestone 1

This is a real native JUCE/CMake starter project for a guitar amp simulator.

## What it builds

- Standalone Windows app
- VST3 plugin
- JUCE audio device support
- Basic amp DSP with gain, tone stack, cab-style low pass, gate, and master volume
- Four starter amp models: Clean, British JCM, Recto Modern, Boutique Lead

## Recommended no-local-tools build

1. Upload the contents of this folder to your GitHub repository root.
2. Make sure this file exists in GitHub:

```text
.github/workflows/windows-release.yml
```

3. Open the **Actions** tab.
4. Run **Windows Release Build**.
5. Download the artifact named `SoundForgeAmpSim-Windows-M1`.

## Local Windows build

Requires CMake and Visual Studio 2022 with Desktop C++ workload.

```bat
scripts\build_windows_release.bat
```

## Roadmap

Milestone 2 will add an IR cabinet loader, better oversampled nonlinear stages, preset saving/loading UI, and proper installer packaging.
