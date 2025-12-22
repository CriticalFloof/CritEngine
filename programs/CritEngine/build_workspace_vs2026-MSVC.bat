@echo off
echo Generating Visual Studio 2026 Project Files...
call ..\..\external\premake\premake5.exe vs2026 msc-v145
popd
pause