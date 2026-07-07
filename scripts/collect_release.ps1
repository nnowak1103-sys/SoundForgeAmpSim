$ErrorActionPreference = "Stop"
$root = Resolve-Path "$PSScriptRoot\.."
$release = Join-Path $root "release"
New-Item -ItemType Directory -Force -Path $release | Out-Null
$standalone = Get-ChildItem "$root\build" -Recurse -Filter "SonicForge Pro Amp Sim.exe" | Select-Object -First 1
$vst3 = Get-ChildItem "$root\build" -Recurse -Directory -Filter "SonicForge Pro Amp Sim.vst3" | Select-Object -First 1
if ($standalone) { Copy-Item $standalone.FullName $release -Force }
if ($vst3) { Copy-Item $vst3.FullName (Join-Path $release $vst3.Name) -Recurse -Force }
Copy-Item "$root\README.md" $release -Force
Compress-Archive -Path "$release\*" -DestinationPath "$release\SonicForgeProAmpSim-Portable-Windows.zip" -Force
Write-Host "Release files collected in $release"
