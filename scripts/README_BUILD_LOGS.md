# Build logs

Run `scripts\build_windows_release.bat` from Windows.

The script writes timestamped logs into `logs\`:

- `Build_YYYY-MM-DD_HH-mm-ss.log` — full combined build log
- `Environment_YYYY-MM-DD_HH-mm-ss.log` — Windows, PATH, CMake, Git, MSBuild diagnostics
- `Configure_YYYY-MM-DD_HH-mm-ss.log` — CMake configure output
- `Compile_YYYY-MM-DD_HH-mm-ss.log` — compiler/MSBuild output
- `Packaging_YYYY-MM-DD_HH-mm-ss.log` — release copy/ZIP output
- `Installer_YYYY-MM-DD_HH-mm-ss.log` — Inno Setup output, if installed
- `Errors_YYYY-MM-DD_HH-mm-ss.log` — short error summary plus failing step output

If the build fails, send the newest `Build_*.log` and `Errors_*.log` for debugging.
