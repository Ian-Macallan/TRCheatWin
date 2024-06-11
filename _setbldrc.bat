@echo off

cd %~dp0
%~d0

call SetBldRCW.cmd %*

exit /b

setbld -f TR3CHEATWIN.RC -a "TombRaiders Cheat" -u %1

rem Generic Settings
set PRODUCT=

if "%PRODUCT%."=="." for %%1 in (*.rc) do set PRODUCT=%%~n1
if "%PRODUCT%."=="." for %%1 in (*.sln) do set PRODUCT=%%~n1

for /d %%1 in (*Setup*) do if exist "%%~1\Product.wxs" %EXEPATH%\setbld -l -f "%%~1\Product.wxs" -a "%PRODUCT%" -u %1 -n
for /d %%1 in (*Setup*) do if exist "%%~1\Version.wxi" %EXEPATH%\setbld -l -f "%%~1\Version.wxi" -a "%PRODUCT%" -u %1 -n

set PRODUCT=

exit /b
