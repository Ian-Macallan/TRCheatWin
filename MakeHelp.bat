@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TR3CHEATWIN.HPJ. >"hlp\TR3CHEATWIN.hm"
echo. >>"hlp\TR3CHEATWIN.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\TR3CHEATWIN.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\TR3CHEATWIN.hm"
echo. >>"hlp\TR3CHEATWIN.hm"
echo // Prompts (IDP_*) >>"hlp\TR3CHEATWIN.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\TR3CHEATWIN.hm"
echo. >>"hlp\TR3CHEATWIN.hm"
echo // Resources (IDR_*) >>"hlp\TR3CHEATWIN.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\TR3CHEATWIN.hm"
echo. >>"hlp\TR3CHEATWIN.hm"
echo // Dialogs (IDD_*) >>"hlp\TR3CHEATWIN.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\TR3CHEATWIN.hm"
echo. >>"hlp\TR3CHEATWIN.hm"
echo // Frame Controls (IDW_*) >>"hlp\TR3CHEATWIN.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\TR3CHEATWIN.hm"
REM -- Make help for Project TR3CHEATWIN


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\TR3CHEATWIN.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\TR3CHEATWIN.hlp" goto :Error
if not exist "hlp\TR3CHEATWIN.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\TR3CHEATWIN.hlp" Debug
if exist Debug\nul copy "hlp\TR3CHEATWIN.cnt" Debug
if exist Release\nul copy "hlp\TR3CHEATWIN.hlp" Release
if exist Release\nul copy "hlp\TR3CHEATWIN.cnt" Release
echo.
goto :done

:Error
echo hlp\TR3CHEATWIN.hpj(1) : error: Problem encountered creating help file

:done
echo.
