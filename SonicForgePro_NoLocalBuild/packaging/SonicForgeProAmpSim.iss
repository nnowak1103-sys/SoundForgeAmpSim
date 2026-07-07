#define MyAppName "SonicForge Pro Amp Sim"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "SonicForge Audio"
[Setup]
AppId={{7D771B02-13F0-4AB6-8C68-53AA3D5E0E31}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\SonicForge Pro Amp Sim
DefaultGroupName={#MyAppName}
OutputDir=..\release
OutputBaseFilename=SonicForgeProAmpSim-Windows-Setup
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64
[Files]
Source: "..\release\SonicForge Pro Amp Sim.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\release\SonicForge Pro Amp Sim.vst3\*"; DestDir: "{commoncf}\VST3\SonicForge Pro Amp Sim.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\SonicForge Pro Amp Sim.exe"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\SonicForge Pro Amp Sim.exe"; Tasks: desktopicon
[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"
