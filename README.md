# SoundForge Amp Sim — Engineering Milestones 2 + 3

Native JUCE/C++ standalone + VST3 amp simulator starter project.

## What changed in this package

- Added Milestone 2 amp voices: Marshall JCM, Mesa Rectifier, Bogner Ecstasy, Soldano SLO, 5150/6505, Orange Rocker, Vox/Fender Glass, Studio Clean.
- Added cabinet selector: Open 1x12, Vintage 2x12, Modern 4x12, Dark 4x12, Bright Mix.
- Added Milestone 3 effects: Tube Screamer-style front end, compressor, chorus, delay, reverb, wet/dry mix.
- Reworked UI with a polished dark/gold amplifier-panel design, custom rotary knobs, better spacing, and a refined visual hierarchy.
- Includes `.github/workflows/windows-release.yml` at repo root for GitHub Actions Windows builds.

## GitHub build

Upload the **contents** of this folder to the root of your GitHub repository. Confirm this file exists:

```text
.github/workflows/windows-release.yml
```

Then go to **Actions → Windows Release Build → Run workflow**.

The build artifact will be named:

```text
SoundForgeAmpSim-Windows-M2M3
```

## Local Windows build

```bat
scripts\build_windows_release.bat
```

Local builds require CMake + Visual Studio Build Tools. GitHub Actions does not require you to install those locally.

## Current status

This is a real buildable JUCE/CMake engineering starter, not a finished commercial amp simulator. The DSP models are algorithmic approximations intended as a foundation for further work. Commercial-grade accuracy would require deeper circuit modeling, measured cabinets/IRs, anti-aliased nonlinear stages, profiling, and listening tests.
