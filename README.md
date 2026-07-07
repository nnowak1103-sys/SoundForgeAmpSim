# SonicForge Pro Amp Sim

Native C++/JUCE professional amp-sim starter project with Standalone and VST3 targets. This is the correct architecture for a no-Python end-user product.

## What is included
- Standalone desktop app target
- VST3 plugin target
- Eight amp-style voices: US Rectifier, British JCM, Bogner Modern, SLO Lead, 5150 Block, Blackface Clean, AC Chime, Orange Doom
- Real-time controls: input, gain, bass, mid, treble, tone, presence, resonance, master, output volume, gate, cab mix, room, bypass
- Modular DSP folders for amp engine, cab processing, gate, effects, presets, UI
- GitHub Actions Windows release workflow
- Inno Setup installer script
- End-user VST3 install helper

## Important build truth
End users do not need Python, CMake, Visual Studio, or JUCE. Developers/build machines do need a native Windows toolchain to produce the final `.exe`, `.vst3`, portable ZIP, and installer.

## Windows build
Open PowerShell or Command Prompt in this folder:

```bat
scripts\build_windows_release.bat
```

Outputs are placed in `release/`.

## GitHub Actions build
Push this folder to a GitHub repository and run **Windows Release Build**. Download the generated artifacts from the workflow run.

## Legal note on amp names
The project uses descriptive amp-style labels. For commercial release, use original product names and avoid implying endorsement by Mesa/Boogie, Marshall, Bogner, Soldano, Peavey, Fender, Vox, Orange, or any other brand.
