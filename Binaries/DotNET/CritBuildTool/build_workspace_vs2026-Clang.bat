@echo off
echo Generating Visual Studio 2026 Project Files...
call bin\Debug\net8.0\CritBuildTool.exe vs2026 clang
popd
pause