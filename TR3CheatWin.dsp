# Microsoft Developer Studio Project File - Name="TR3CHEATWIN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TR3CHEATWIN - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TR3CHEATWIN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TR3CHEATWIN.mak" CFG="TR3CHEATWIN - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TR3CHEATWIN - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TR3CHEATWIN - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TR3CHEATWIN - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TR3CHEATWIN - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TR3CHEATWIN - Win32 Release"
# Name "TR3CHEATWIN - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TR1SaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\TR2SaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3AmmosPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3CHEATWIN.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\TR3CHEATWIN.hpj

!IF  "$(CFG)" == "TR3CHEATWIN - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Release
InputPath=.\hlp\TR3CHEATWIN.hpj
InputName=TR3CHEATWIN

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "TR3CHEATWIN - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Debug
InputPath=.\hlp\TR3CHEATWIN.hpj
InputName=TR3CHEATWIN

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TR3CHEATWIN.rc
# End Source File
# Begin Source File

SOURCE=.\TR3CHEATWINDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3EquipmentPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3GunPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3InfoPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3Items.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3PropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TR3SaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\TR4SaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\TRSaveGame.cpp
# End Source File
# Begin Source File

SOURCE=.\TUBSaveGame.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "TR3CHEATWIN - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=TR3CHEATWIN
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "TR3CHEATWIN - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=TR3CHEATWIN
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TR1SaveGame.h
# End Source File
# Begin Source File

SOURCE=.\TR2SaveGame.h
# End Source File
# Begin Source File

SOURCE=.\TR3AmmosPage.h
# End Source File
# Begin Source File

SOURCE=.\TR3CHEATWIN.h
# End Source File
# Begin Source File

SOURCE=.\TR3CHEATWINDlg.h
# End Source File
# Begin Source File

SOURCE=.\TR3EquipmentPage.h
# End Source File
# Begin Source File

SOURCE=.\TR3GunPage.h
# End Source File
# Begin Source File

SOURCE=.\TR3InfoPage.h
# End Source File
# Begin Source File

SOURCE=.\TR3Items.h
# End Source File
# Begin Source File

SOURCE=.\TR3PropSheet.h
# End Source File
# Begin Source File

SOURCE=.\TR3SaveGame.h
# End Source File
# Begin Source File

SOURCE=.\TR4SaveGame.h
# End Source File
# Begin Source File

SOURCE=.\TRSaveGame.h
# End Source File
# Begin Source File

SOURCE=.\TUBSaveGame.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\Aire 51.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Aldwych.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Ammos Desert Eagle.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos Grenade Launcher.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos harpoon.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos MP5.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos Riot Gun.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos Rocket Launcher.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ammos Uzis.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\ANTARC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Antarctique.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Chrono.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Desert Eagle 3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Desert Eagle.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Flares 2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Flares.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Grenade Launcher 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Grenade Launcher 3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Grenade Launcher.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Guns.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Hallows.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Harpoon 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Harpoon 3.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Harpoon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\INDIA.bmp
# End Source File
# Begin Source File

SOURCE=".\res\K1 Aire Mise a Feu.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Crash Bishop.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Desert Pass.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Gange Portail.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Haute Carte A.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Jungle Indra 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Londres Salle du Conduit.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Lude Chaufferie.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Temple Ganesha.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K1 Village Cotier Clef Contrebandier.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K2 Aldwych Salomon.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K2 Crash Tuckerman.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K2 Desert Detonateur.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K2 Haute Carte B.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K2 Londres Cathedral.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K3 Aldwych Salomon.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K4 Aire Passe.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K4 Aldwych Maintenance.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\K4 Jungle Indra.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\La caverne Du Meteore.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\La Cite Perdue de Tinos.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\La Gorge de Mabudu.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\La Jungle.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\La Ville.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Large Medipak 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Large Medipak 3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Large Medipak.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Desert Du Nevada.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Gange.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Lieu du Crash.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Portail de Lude.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Temple de Puna.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Le Village Cotier.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Les Grottes de Kaliya.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Les Mines de RX.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Les Quais de La Tamise.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Les Ruines du Temple.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\LONDON.bmp
# End Source File
# Begin Source File

SOURCE=".\res\M1 Crash Carte du Marais.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\mp5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NEVADA.bmp
# End Source File
# Begin Source File

SOURCE=".\res\P1 Aire Cle Tour.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P1 Aldwych Penny.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P1 Haute Passe Bleu.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P1 Lude Embaumenent.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P1 Temple Cimeterre.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P1 Village Cotier - Pierre Du Serpent.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P2 Aire Disque Code.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P2 Aldwych Ticket.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P2 Haute Passe Jaune.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P3 Aire Disque Code.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P3 Aldwych Maillet.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P4 Aire Cle Hangar.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\P4 Aldwych Etoile.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Quartier Haute Securite.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Riot Gun 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Riot Gun.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Rocket Launcher 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Rocket Launcher.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Small Medipak 2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Small Medipak 3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Small Medipak.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\SOUTHPAC.bmp
# End Source File
# Begin Source File

SOURCE=".\res\T1 Pierre Infada.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\T3 Oeil D Isis.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\T4 Object 115.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\THEEND.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Title.bmp
# End Source File
# Begin Source File

SOURCE=".\res\TR1 The End.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TR1 The Start.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TR2 The End.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TR2 The Start.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\TR3CHEATWIN.ico
# End Source File
# Begin Source File

SOURCE=.\res\TR3CHEATWIN.rc2
# End Source File
# Begin Source File

SOURCE=".\res\TUB End.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TUB Start.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Uzis 2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Uzis.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxDlg.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\TR3CHEATWIN.cnt

!IF  "$(CFG)" == "TR3CHEATWIN - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\TR3CHEATWIN.cnt
InputName=TR3CHEATWIN

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "TR3CHEATWIN - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\TR3CHEATWIN.cnt
InputName=TR3CHEATWIN

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
