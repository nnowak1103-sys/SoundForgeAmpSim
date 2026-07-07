@echo off
setlocal
set VSTDIR=%LOCALAPPDATA%\Programs\Common\VST3
if not exist "%VSTDIR%" mkdir "%VSTDIR%"
if exist "SonicForge Pro Amp Sim.vst3" xcopy /E /I /Y "SonicForge Pro Amp Sim.vst3" "%VSTDIR%\SonicForge Pro Amp Sim.vst3"
echo Installed VST3 to %VSTDIR%
endlocal
pause
