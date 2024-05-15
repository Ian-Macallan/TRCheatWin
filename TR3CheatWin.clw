; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tr3cheatwin.h"
LastPage=0

ClassCount=10
Class1=CTR3AmmosPage
Class2=CTR3CHEATWINApp
Class3=CAboutDlg
Class4=CTR3EquipmentPage
Class5=CTR3GunPage
Class6=CTR3InfoPage
Class7=CTR3Items
Class8=CTR3PropSheet

ResourceCount=6
Resource1=IDD_GUN_PAGE (English (U.S.))
Resource2=IDD_INFO (English (U.S.))
Resource3=IDD_ABOUTBOX (English (U.S.))
Resource4=IDD_ITEMS (English (U.S.))
Resource5=IDD_EQUIPMENT (English (U.S.))
Class9=CTR1SaveGame
Class10=CTUBSaveGame
Resource6=IDD_AMMOS (English (U.S.))

[CLS:CTR3AmmosPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TR3AmmosPage.h
ImplementationFile=TR3AmmosPage.cpp
LastObject=CTR3AmmosPage
Filter=D
VirtualFilter=idWC

[CLS:CTR3CHEATWINApp]
Type=0
BaseClass=CWinApp
HeaderFile=TR3CHEATWIN.h
ImplementationFile=TR3CHEATWIN.cpp
LastObject=CTR3CHEATWINApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=TR3CHEATWINDlg.h
ImplementationFile=TR3CHEATWINDlg.cpp
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[CLS:CTR3EquipmentPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TR3EquipmentPage.h
ImplementationFile=TR3EquipmentPage.cpp
LastObject=CTR3EquipmentPage
Filter=D
VirtualFilter=idWC

[CLS:CTR3GunPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TR3GunPage.h
ImplementationFile=TR3GunPage.cpp
Filter=D
VirtualFilter=idWC
LastObject=CTR3GunPage

[CLS:CTR3InfoPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=TR3InfoPage.h
ImplementationFile=TR3InfoPage.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_DEF_ADDRESS

[CLS:CTR3Items]
Type=0
BaseClass=CPropertyPage
HeaderFile=TR3Items.h
ImplementationFile=TR3Items.cpp
LastObject=IDC_ITEM1

[CLS:CTR3PropSheet]
Type=0
BaseClass=CPropertySheet
HeaderFile=TR3PropSheet.h
ImplementationFile=TR3PropSheet.cpp
LastObject=CTR3PropSheet

[DLG:IDD_AMMOS]
Type=1
Class=CTR3AmmosPage

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_EQUIPMENT]
Type=1
Class=CTR3EquipmentPage

[DLG:IDD_GUN_PAGE]
Type=1
Class=CTR3GunPage

[DLG:IDD_INFO]
Type=1
Class=CTR3InfoPage

[DLG:IDD_ITEMS]
Type=1
Class=CTR3Items

[DLG:IDD_INFO (English (U.S.))]
Type=1
Class=CTR3InfoPage
ControlCount=17
Control1=IDC_DIRECTORY,edit,1350568064
Control2=IDC_FILENAME,edit,1350568064
Control3=IDC_SAVENUMBER,edit,1350568064
Control4=IDC_LEVELNUMBER,edit,1350568064
Control5=IDC_LEVELNAME,edit,1350568064
Control6=IDC_LOAD,button,1342242817
Control7=IDC_WRITE,button,1342242817
Control8=IDC_LEVEL_PICTURE,static,1350572046
Control9=IDC_STATUS,edit,1350568064
Control10=IDC_STATIC,static,1342308352
Control11=IDC_SECRETS,edit,1350631552
Control12=IDC_NBSECRETS,edit,1350568064
Control13=IDC_STATIC,static,1342308352
Control14=IDC_LARABURNING,button,1342242819
Control15=IDC_LEVEL_PLACE,static,1350572046
Control16=IDC_LISTCTRL,SysListView32,1350631429
Control17=IDC_DEF_ADDRESS,button,1476460547

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=9
Control1=IDC_VERSION,static,1350701056
Control2=IDC_COPYRIGHT,static,1350701056
Control3=IDOK,button,1342373889
Control4=IDC_WEB,static,1350701312
Control5=IDC_EMAIL,static,1350701312
Control6=IDC_STATIC,static,1350569998
Control7=IDC_DRACMAN,button,1342242816
Control8=IDC_MACALLAN,button,1342242816
Control9=IDC_STATIC,static,1350701056

[DLG:IDD_GUN_PAGE (English (U.S.))]
Type=1
Class=CTR3GunPage
ControlCount=19
Control1=IDC_GUNS,button,1342242819
Control2=IDC_HARPOON,button,1342242819
Control3=IDC_DESERTEAGLE,button,1342242819
Control4=IDC_UZI,button,1342242819
Control5=IDC_MP5,button,1342242819
Control6=IDC_ROCKETLAUNCHER,button,1342242819
Control7=IDC_GRENADELAUNCHER,button,1342251011
Control8=IDC_RIOTGUN,button,1342242819
Control9=IDC_STATIC,static,1350572046
Control10=IDC_STATIC,static,1350572046
Control11=IDC_STATIC,static,1350572046
Control12=IDC_STATIC,static,1350572046
Control13=IDC_STATIC,static,1350572046
Control14=IDC_STATIC,static,1350572046
Control15=IDC_STATIC,static,1350572046
Control16=IDC_STATIC,static,1350572046
Control17=IDC_STATIC,static,1350572046
Control18=IDC_STATUS,edit,1350568064
Control19=IDC_STATIC,static,1342308353

[DLG:IDD_EQUIPMENT (English (U.S.))]
Type=1
Class=CTR3EquipmentPage
ControlCount=22
Control1=IDC_LIFE,edit,1350631552
Control2=IDC_AIR,edit,1350631552
Control3=IDC_UNFINITE_AIR,button,1342242819
Control4=IDC_SMALLMEDIPAK,edit,1350631552
Control5=IDC_BUT_SMEDI,button,1342246915
Control6=IDC_LARGEMEDIPAK,edit,1350631552
Control7=IDC_BUT_LMEDI,button,1342246915
Control8=IDC_FLARES,edit,1350631552
Control9=IDC_BUT_FLARES,button,1342246915
Control10=IDC_STATIC,static,1350572046
Control11=IDC_STATIC,static,1350572046
Control12=IDC_STATIC,static,1350572046
Control13=IDC_STATUS,edit,1350568064
Control14=IDC_STATIC,static,1342308352
Control15=IDC_HEALTH,edit,1350568064
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_GOD,button,1342242819

[DLG:IDD_ITEMS (English (U.S.))]
Type=1
Class=?
ControlCount=69
Control1=IDC_ITEM1,button,1342242819
Control2=IDC_ITEM2,button,1342242819
Control3=IDC_NITEM1,edit,1350631552
Control4=IDC_NITEM2,edit,1350631552
Control5=IDC_ITEM3,button,1342242819
Control6=IDC_ITEM4,button,1342242819
Control7=IDC_NITEM3,edit,1350631552
Control8=IDC_NITEM4,edit,1350631552
Control9=IDC_MAP1,button,1342242819
Control10=IDC_NMAP1,edit,1350631552
Control11=IDC_MAP2,button,1342242819
Control12=IDC_NMAP2,edit,1350631552
Control13=IDC_KEY1,button,1342242819
Control14=IDC_KEY2,button,1342242819
Control15=IDC_KEY3,button,1342242819
Control16=IDC_KEY4,button,1342242819
Control17=IDC_NKEY1,edit,1350631552
Control18=IDC_NKEY2,edit,1350631552
Control19=IDC_NKEY3,edit,1350631552
Control20=IDC_NKEY4,edit,1350631552
Control21=IDC_TREASURE1,button,1342242819
Control22=IDC_TREASURE2,button,1342242819
Control23=IDC_TREASURE3,button,1342242819
Control24=IDC_TREASURE4,button,1342242819
Control25=IDC_NTREASURE1,edit,1350631552
Control26=IDC_NTREASURE2,edit,1350631552
Control27=IDC_NTREASURE3,edit,1350631552
Control28=IDC_NTREASURE4,edit,1350631552
Control29=IDC_STATUS,edit,1350568064
Control30=IDC_STATIC,static,1350572046
Control31=IDC_STATIC,static,1350572046
Control32=IDC_STATIC,static,1350572046
Control33=IDC_STATIC,static,1350572046
Control34=IDC_STATIC,static,1342177294
Control35=IDC_STATIC,button,1342177287
Control36=IDC_STATIC,button,1342177287
Control37=IDC_STATIC,static,1350572046
Control38=IDC_STATIC,static,1350572046
Control39=IDC_STATIC,static,1350572046
Control40=IDC_STATIC,static,1350572046
Control41=IDC_STATIC,static,1350572046
Control42=IDC_STATIC,static,1350572046
Control43=IDC_STATIC,static,1350572046
Control44=IDC_STATIC,static,1350572046
Control45=IDC_STATIC,static,1350572046
Control46=IDC_STATIC,static,1350572046
Control47=IDC_STATIC,static,1350572046
Control48=IDC_STATIC,static,1350572046
Control49=IDC_STATIC,static,1350572046
Control50=IDC_STATIC,static,1350572046
Control51=IDC_STATIC,static,1350572046
Control52=IDC_STATIC,static,1350572046
Control53=IDC_STATIC,static,1350572046
Control54=IDC_STATIC,static,1350572046
Control55=IDC_STATIC,static,1350572046
Control56=IDC_STATIC,static,1350572046
Control57=IDC_STATIC,static,1350572046
Control58=IDC_STATIC,static,1350572046
Control59=IDC_STATIC,static,1350572046
Control60=IDC_STATIC,static,1350572046
Control61=IDC_STATIC,static,1350572046
Control62=IDC_STATIC,static,1350572046
Control63=IDC_STATIC,static,1350572046
Control64=IDC_STATIC,static,1350572046
Control65=IDC_STATIC,static,1350572046
Control66=IDC_STATIC,static,1350572046
Control67=IDC_STATIC,static,1350572046
Control68=IDC_STATIC,button,1342177287
Control69=IDC_STATIC,button,1342177287

[DLG:IDD_AMMOS (English (U.S.))]
Type=1
Class=CTR3AmmosPage
ControlCount=23
Control1=IDC_ARIOTGUN,edit,1350631552
Control2=IDC_AGRENADELAUNCHER,edit,1350631552
Control3=IDC_AHARPOON,edit,1350631552
Control4=IDC_ADESERTEAGLE,edit,1350631552
Control5=IDC_AMP5,edit,1350631552
Control6=IDC_AUZIS,edit,1350631552
Control7=IDC_AROCKETLAUNCHER,edit,1350631552
Control8=IDC_UNLIMITED,button,1342242819
Control9=IDC_STATIC,static,1350572046
Control10=IDC_STATIC,static,1350572046
Control11=IDC_STATIC,static,1350572046
Control12=IDC_STATIC,static,1350572046
Control13=IDC_STATIC,static,1350572046
Control14=IDC_STATIC,static,1350572046
Control15=IDC_STATIC,static,1350572046
Control16=IDC_STATUS,edit,1350568064
Control17=IDC_CRIOTGUN,button,1342246915
Control18=IDC_CGRENADELAUNCHER,button,1342246915
Control19=IDC_CHARPOON,button,1342246915
Control20=IDC_CDESERTEAGLE,button,1342246915
Control21=IDC_CMP5,button,1342246915
Control22=IDC_CUZIS,button,1342246915
Control23=IDC_CROCKETLAUNCHER,button,1342246915

[CLS:CTR1SaveGame]
Type=0
HeaderFile=TR1SaveGame.h
ImplementationFile=TR1SaveGame.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CTR1SaveGame

