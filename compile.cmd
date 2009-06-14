@echo off

..\bjam toolset=msvc %*

call ..\boost-version.cmd
IF NOT ERRORLEVEL 1 (
    for /r %BUILD_DIRECTORY% %%f in (*.pdb) do xcopy /D /Y %%f ..\dist\bin
)

IF NOT ERRORLEVEL 1 (
    ..\dist\bin\ftest ..\dist\bin\fost-core-test-smoke.dll ..\dist\bin\fost-crypto-test-smoke.dll ..\dist\bin\fost-inet-test-smoke.dll
)
