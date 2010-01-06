@echo off
echo fost-base
@pushd %0\..

..\bjam %*

@IF ERRORLEVEL 1 (
    @echo fost-base %* BUILD FAIL
    @popd
    @copy
) ELSE (
    @popd
)
