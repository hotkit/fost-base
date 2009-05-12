@echo off

..\bjam toolset=msvc %*

IF NOT ERRORLEVEL 1 (
    ..\dist\bin\ftest ..\dist\bin\fost-core-test-smoke.dll ..\dist\bin\fost-crypto-test-smoke.dll ..\dist\bin\fost-inet-test-smoke.dll  ..\dist\bin\fost-jsondb-test-smoke.dll ..\dist\bin\fost-schema-test-smoke.dll
)

IF NOT ERRORLEVEL 1 (
    ..\dist\bin\fost-schema-test-jsondb-file Cpp\fost-schema-test\jsondb-file.json
)

IF NOT ERRORLEVEL 1 (
    ..\dist\bin\fost-jsondb-test-file
)

