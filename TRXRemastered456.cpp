// TRXRemastered.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"

#include "TR8SaveGame.h"


#include "TRXRemastered456.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXTools.h"
#include "TRXSecretsDialog.h"
#include "TRXPosition.h"
#include "TRXDifferences.h"
#include "TRXRemasteredInfos.h"
#include "TRLabelItems.h"
#include "TRXMapAreas.h"
#include "TRXAllMaps.h"
#include "TRXMenuBase.h"

#include "TRXGunPage.h"         // Added by ClassView
#include "TRXInfoPage.h"        // Added by ClassView
#include "TRXItems.h"           // Added by ClassView
#include "TRXItemsTR4.h"        // Added by ClassView
#include "TRXAmmosPage.h"       // Added by ClassView
#include "TRXEquipmentPage.h"   // Added by ClassView
#include "TRXPropertySheet.h"

#include "TRXItemsTRR456.h"

#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXRemastered
//
/////////////////////////////////////////////////////////////////////////////
extern CTRXCHEATWINApp theApp;

#define COLR_GAME            0       // TR1, TR2, TR3
#define COLR_SUB_GAME        1       // GOLD
#define COLR_SAVE_NO         2
#define COLR_LEVEL_NO        3
#define COLR_LEVEL_NAME      4
#define COLR_ADDRESS_1       5
#define COLR_ADDRESS_2       6
#define COLR_BLOCK           7
#define COLR_SLOT            8
#define COLR_FOUND           9
#define COLR_TOMBRAIDER      10
#define COLR_KILLS           11
#define COLR_PICKUPS         12
#define COLR_ELAPSED         13
#define COLR_DISTANCE        14
#define COLR_EXTRA           15

#define MAX_AIR             1800
#define MAX_HEALTH          1000
#define MAX_GUN             0xffff
#define MAX_PAK             1000
#define MAX_AMMOS           6000
#define MAX_AMMOS_MAX       (6*6000)

#define STATE_SAFE          0x0a
#define STATE_BURNING       0x1a

//
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static  STRUCTLOCATION  LocationPathname [ LEN_LOCATION ];

//
static const    char *MessageTitle = "Tombraider 456 Remastered";

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static  int             iSortAscending      = 1;
static  int             iSortColumn         = -1;
static  const   int     maxData             = 3 * NB_SLOT_456;
typedef struct  structData
{
    int     tombraider;
    int     level;
    int     save;
    int     block;
    char    szText [ MAX_PATH ];
} STRUCTDATA;

static  STRUCTDATA tagData [ maxData ];

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
#define SET_BLOCK_VALUE(wnd,field)                          \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pBlock->##field,TRUE );              \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE );                           \
    }

#define SET_BLOCK_UNITS(wnd,field,unit)                     \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,(pBlock->##field)/unit,TRUE );       \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE );                           \
    }

#define SET_BLOCK_DISABLE(wnd,field)                        \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pBlock->##field,FALSE,TRUE);         \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE );                           \
    }

#define SET_BLOCK_HIDE(wnd)                                 \
        SetValue(##wnd,0,FALSE );       

#define SET_BLOCK_CHECK(wnd,field,msk)                      \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pGun->##field&##msk,TRUE );          \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE);                            \
    }

#define SET_CHECK_DISABLE(wnd,msk)                          \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pBlock->gunmask&##msk,FALSE,TRUE);   \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE);                            \
    }

#define SET_GUN_VALUE(wnd,field)                            \
    if ( pGun != NULL )                                     \
    {                                                       \
        SetValue(##wnd,pGun->##field,TRUE);                 \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE);                            \
    }

#define SET_GUN_HIDE(wnd)                                   \
        SetValue(##wnd,0,FALSE);        

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXRemastered456, CTRXPropertyPage456)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemastered456::CTRXRemastered456() : CTRXPropertyPage456(CTRXRemastered456::IDD)
{
    CTR8SaveGame::I();

    m_pListCtrl         = NULL;
    m_Line              = -1;

    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        ZeroMemory ( LocationPathname [ i ].szPathname, sizeof(LocationPathname [ i ].szPathname) );
    }

    //
    LoadLocation ( LocationPathname, PROFILE_TRX2_LOC_COUNT, PROFILE_TRX2_LOCATION );

    //
    m_bRoomCreated  = false;

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemastered456::~CTRXRemastered456()
{
    CTR8SaveGame *pGame = CTR8SaveGame::I(FALSE);
    if ( pGame != NULL )
    {
        delete pGame;
    }

    SaveLocation ( LocationPathname, PROFILE_TRX2_LOC_COUNT, PROFILE_TRX2_LOCATION );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnHelp()
{
    //
    CTRXHelpDialog dlg;
    dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage456::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTCTRL, m_ListCtrl);
    DDX_Control(pDX, IDC_LOAD, m_Load);
    DDX_Control(pDX, IDC_WRITE, m_Write);
    DDX_Control(pDX, IDC_FILENAME, m_Filename);
    DDX_Control(pDX, IDC_STATUS, m_Status);

    DDX_Control(pDX, IDC_MAGNUM, m_Magnum);
    DDX_Control(pDX, IDC_UZI, m_Uzi);
    DDX_Control(pDX, IDC_RIOTGUN, m_Riotgun);
    DDX_Control(pDX, IDC_M16, m_M16);
    DDX_Control(pDX, IDC_GRENADE, m_Grenade);
    DDX_Control(pDX, IDC_ROCKET, m_Rocket);
    DDX_Control(pDX, IDC_HARPOON, m_Harpoon);

    DDX_Control(pDX, IDC_AMAGNUM, m_AMagnum);
    DDX_Control(pDX, IDC_AUZI, m_AUzi);
    DDX_Control(pDX, IDC_ARIOTGUN, m_ARiotgun);
    DDX_Control(pDX, IDC_AM16, m_AM16);
    DDX_Control(pDX, IDC_AGRENADE, m_AGrenade);
    DDX_Control(pDX, IDC_AROCKET, m_ARocket);
    DDX_Control(pDX, IDC_AHARPOON, m_AHarpoon);

    DDX_Control(pDX, IDC_HEALTH, m_AGuns);
    DDX_Control(pDX, IDC_LARGEPAK, m_LargePak);
    DDX_Control(pDX, IDC_SMALLPAK, m_SmallPak);
    DDX_Control(pDX, IDC_FLARE, m_Flare);
    DDX_Control(pDX, IDC_REFRESH, m_Refresh);
    DDX_Control(pDX, IDC_XMAGNUM, m_XMagnum);
    DDX_Control(pDX, IDC_XUZI, m_XUzi);
    DDX_Control(pDX, IDC_XRIOTGUN, m_XRiotgun);
    DDX_Control(pDX, IDC_XM16, m_XM16);
    DDX_Control(pDX, IDC_XGRENADE, m_XGrenade);
    DDX_Control(pDX, IDC_XROCKET, m_XRocket);
    DDX_Control(pDX, IDC_XHARPOON, m_XHarpoon);
    DDX_Control(pDX, IDC_XHEALTH, m_XGuns);
    DDX_Control(pDX, IDC_DEFAULT, m_Default);
    DDX_Control(pDX, IDC_MAX, m_Max);
    DDX_Control(pDX, IDC_MAX_ONE, m_Max_One);
    DDX_Control(pDX, IDC_AIR_REM, m_Air);
    DDX_Control(pDX, IDC_GUN, m_Guns);
    DDX_Control(pDX, IDC_AR_FULL, m_Air_Full);
    DDX_Control(pDX, IDC_AR_INFINITE, m_Air_Infinite);
    DDX_Control(pDX, IDC_LIST_ALL, m_List_All);
    DDX_Control(pDX, IDC_UPDATE, m_Update);
    DDX_Control(pDX, IDC_COMBO, m_Combo);
    DDX_Control(pDX, IDC_STATE, m_State);
    DDX_Control(pDX, IDC_CHECK1, m_Burning);
    DDX_Control(pDX, IDC_REMOVE_LOC, m_Remove_Loc);
    DDX_Control(pDX, IDC_REAL_HEALTH, m_Real_Health);
    DDX_Control(pDX, IDC_STRONG, m_Invincible);
    DDX_Control(pDX, IDC_PICKUP, m_Pickup);
    DDX_Control(pDX, IDC_CRYSTAL, m_Crystal);
    DDX_Control(pDX, IDC_KILL, m_Kill);
    DDX_Control(pDX, IDC_SECRETS, m_Secrets_A);
    DDX_Control(pDX, IDC_SECRETS_1, m_Secrets_C);
    DDX_Control(pDX, IDC_SECRETS_2, m_Secrets_E);
    DDX_Control(pDX, IDC_SECRETS_MAX, m_Secrets_Max);
    DDX_Control(pDX, IDC_COL_CRYSTAL, m_Col_Crystal);
    DDX_Control(pDX, IDC_SET, m_Set);
    DDX_Control(pDX, IDC_SHOW, m_Show);
    DDX_Control(pDX, IDC_TR1_PLUS, m_TR1_Plus);
    DDX_Control(pDX, IDC_TR2_PLUS, m_TR2_Plus);
    DDX_Control(pDX, IDC_TR3_PLUS, m_TR3_Plus);
    DDX_Control(pDX, IDC_GAME_PLUS, m_Game_Plus);
    DDX_Control(pDX, IDC_KILL_WILLARD, m_Kill_Willard);
    DDX_Control(pDX, IDC_SAVE, m_Save_No);
    DDX_Control(pDX, IDC_PICKUP_MAX, m_Pickup_Max);
    DDX_Control(pDX, IDC_CRISTAL_MAX, m_Crystal_Max);
    DDX_Control(pDX, IDC_KILLS_MAX, m_Kill_Max);
    DDX_Control(pDX, IDC_RESET, m_ResetToPlus);
    DDX_Control(pDX, IDC_RESETGAME, m_ResetGames);
    DDX_Control(pDX, IDC_JOURNAL, m_Journal);
    DDX_Control(pDX, IDC_UZI_CLIP, m_Uzi_Clip);
    DDX_Control(pDX, IDC_M16_CLIP, m_M16_Clip);
    DDX_Control(pDX, IDC_GRENADE_CLIP, m_Grenade_Clip);
    DDX_Control(pDX, IDC_POSITION, m_Position);
    DDX_Control(pDX, IDC_SHOW_MAP, m_Show_Map);
    DDX_Control(pDX, IDC_FRAME_POSITION, m_Frame_Position);
    DDX_Control(pDX, IDC_SHELL, m_Shell);
    DDX_Control(pDX, IDC_POS_LABEL, m_Position_Label);
    DDX_Control(pDX, IDC_ITEMS, m_Items);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXRemastered456, CTRXPropertyPage456)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_LOAD, &CTRXRemastered456::OnBnClickedLoad)
    ON_BN_CLICKED(IDC_WRITE, &CTRXRemastered456::OnBnClickedWrite)
    ON_NOTIFY(NM_CLICK, IDC_LISTCTRL, OnClickListctrl)
    ON_BN_CLICKED(IDC_REFRESH, &CTRXRemastered456::OnBnClickedRefresh)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &CTRXRemastered456::OnItemchangedListctrl)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CTRXRemastered456::OnItemclickListctrl)
    ON_BN_CLICKED(IDC_MAX, &CTRXRemastered456::OnBnClickedMax)
    ON_BN_CLICKED(IDC_MAX_ONE, &CTRXRemastered456::OnBnClickedMaxOne)
    ON_BN_CLICKED(IDC_AR_FULL, &CTRXRemastered456::OnBnClickedArFull)
    ON_BN_CLICKED(IDC_AR_INFINITE, &CTRXRemastered456::OnBnClickedArInfinite)
    ON_BN_CLICKED(IDC_LIST_ALL, &CTRXRemastered456::OnBnClickedListAll)
    ON_BN_CLICKED(IDC_UPDATE, &CTRXRemastered456::OnBnClickedUpdate)
    ON_CBN_SELCHANGE(IDC_COMBO, &CTRXRemastered456::OnSelchangeCombo)
    ON_BN_CLICKED(IDC_BURNING, &CTRXRemastered456::OnBnClickedBurning)
    ON_BN_CLICKED(IDC_REMOVE_LOC, &CTRXRemastered456::OnBnClickedRemoveLoc)
    ON_BN_CLICKED(IDC_STRONG, &CTRXRemastered456::OnBnClickedStrong)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
    ON_BN_CLICKED(IDC_SET, &CTRXRemastered456::OnBnClickedSet)
    ON_BN_CLICKED(IDC_SHOW, &CTRXRemastered456::OnBnClickedShow)
    ON_BN_CLICKED(IDC_KILL_WILLARD, &CTRXRemastered456::OnBnClickedKillWillard)
    ON_NOTIFY(NM_RCLICK, IDC_LISTCTRL, &CTRXRemastered456::OnRclickListctrl)
    ON_COMMAND(ID_MENULIST_DELETE, &CTRXRemastered456::OnMenulistDelete)
    ON_COMMAND(ID_MENULIST_EXPORT, &CTRXRemastered456::OnMenulistExport)
    ON_COMMAND(ID_MENULIST_IMPORT, &CTRXRemastered456::OnMenulistImport)
    ON_COMMAND(ID_MENULIST_INFOS, &CTRXRemastered456::OnMenulistInfos)
    ON_COMMAND(ID_MENULIST_POSITION, &CTRXRemastered456::OnMenulistPosition)
    ON_COMMAND(ID_MENULIST_RESETTO, &CTRXRemastered456::OnMenulistresettoplus)
    ON_COMMAND(ID_MENULIST_RESETTOSTART, &CTRXRemastered456::OnMenulistresettostart)
    ON_COMMAND(ID_MENULIST_RENUMBER, &CTRXRemastered456::OnMenulistRenumber)

    ON_EN_CHANGE(IDC_AMAGNUM, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AUZI, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_ARIOTGUN, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AM16, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AGRENADE, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AROCKET, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AHARPOON, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_1, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_2, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_3, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_4, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_1, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_2, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_3, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_4, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_MAP_1, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_MAP_2, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_1, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_2, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_3, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_4, &CTRXRemastered456::OnChangeEdit)

    ON_EN_CHANGE(IDC_SAVE, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_HEALTH, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_LARGEPAK, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_SMALLPAK, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_FLARE, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_AIR_REM, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_STATE, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_REAL_HEALTH, &CTRXRemastered456::OnChangeEdit)

    ON_EN_CHANGE(IDC_PICKUP, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_CRYSTAL, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_KILL, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_COL_CRYSTAL, &CTRXRemastered456::OnChangeEdit)
    ON_EN_CHANGE(IDC_SECRETS, &CTRXRemastered456::OnChangeEdit)

    ON_BN_CLICKED(IDC_MAGNUM, &CTRXRemastered456::OnClickedMagnum)
    ON_BN_CLICKED(IDC_UZI, &CTRXRemastered456::OnClickedUzi)
    ON_BN_CLICKED(IDC_RIOTGUN, &CTRXRemastered456::OnClickedRiotgun)
    ON_BN_CLICKED(IDC_M16, &CTRXRemastered456::OnClickedM16)
    ON_BN_CLICKED(IDC_GRENADE, &CTRXRemastered456::OnClickedGrenade)
    ON_BN_CLICKED(IDC_HARPOON, &CTRXRemastered456::OnClickedHarpoon)
    ON_BN_CLICKED(IDC_GUN, &CTRXRemastered456::OnClickedGun)
    ON_BN_CLICKED(IDC_ROCKET, &CTRXRemastered456::OnClickedRocket)
    ON_BN_CLICKED(IDC_TR1_PLUS, &CTRXRemastered456::OnClickedTr1Plus)
    ON_BN_CLICKED(IDC_TR2_PLUS, &CTRXRemastered456::OnClickedTr2Plus)
    ON_BN_CLICKED(IDC_TR3_PLUS, &CTRXRemastered456::OnBnClickedTr3Plus)
    ON_BN_CLICKED(IDC_GAME_PLUS, &CTRXRemastered456::OnBnClickedGamePlus)
    ON_BN_CLICKED(IDC_RESET, &CTRXRemastered456::OnBnClickedReset)
    ON_BN_CLICKED(IDC_RESETGAME, &CTRXRemastered456::OnBnClickedResetgame)
    ON_BN_CLICKED(IDC_JOURNAL, &CTRXRemastered456::OnBnClickedJournal)
    ON_WM_CTLCOLOR()
    ON_CBN_SELENDOK(IDC_COMBO, &CTRXRemastered456::OnSelendokCombo)
    ON_BN_CLICKED(IDC_POSITION, &CTRXRemastered456::OnBnClickedPosition)
    ON_BN_CLICKED(IDC_SHOW_MAP, &CTRXRemastered456::OnBnClickedShowMap)
    ON_BN_CLICKED(IDC_SHELL, &CTRXRemastered456::OnBnClickedShell)
    ON_BN_CLICKED(IDC_ITEMS, &CTRXRemastered456::OnBnClickedItems)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Message Handler CTRXRemastered
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedLoad()
{
    //
    char    szDirectory [ MAX_PATH ];
    char    szFilename [ MAX_PATH ];

    //
    if ( m_Update.GetCheck() )
    {
        UpdateBuffer();
    }

    //
    if ( CTR8SaveGame::IsBufferModified() )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR456 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            ZeroMemory ( szFilename, sizeof ( szFilename ) );
            m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
            CTR8SaveGame::WriteFile ( szFilename );
        }
        else if ( response == IDCANCEL )
        {
            return;
        }
        else
        {
            CTR8SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    //
    static char BASED_CODE szFilter[] = "Savegame|savegame.dat|Savegames|savegame.*|All Files (*.*)|*.*||";

    ZeroMemory ( szDirectory, sizeof(szDirectory) );
    ZeroMemory ( szFilename, sizeof(szFilename) );

    m_Write.EnableWindow ( FALSE );
    m_Max.EnableWindow ( FALSE );

    CString lastRead  = theApp.GetProfileString( PROFILE_SETTING, PROFILE_TRX2_LAST_OPENED, szFilename );
    if ( lastRead != "" )
    {
        m_Filename.SetWindowText ( lastRead );

        strcpy_s ( szDirectory, sizeof(szDirectory), lastRead );
        theApp.RemoveFilename ( szDirectory );
    }

    CFileDialog dlgFileDialog ( TRUE, NULL, "savegame.dat", NULL, szFilter );
    dlgFileDialog.m_ofn.lpstrInitialDir = szDirectory;

    /*
     *      Starts a dialog box.
     */
    if ( dlgFileDialog.DoModal () == IDOK)
    {
        /*
         *  Set filename Text.
         */
        m_Filename.SetWindowText ( dlgFileDialog.GetPathName() );
        m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );

        DisplayList ( szFilename );

        /*
         *  Write Profile String.
         */
        //
        BOOL bAdded = AddLocation ( LocationPathname, szFilename );
        bAdded      = AddComboString ( &m_Combo, szFilename );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedWrite()
{
    //
    if ( m_Update.GetCheck() )
    {
        UpdateBuffer();
    }

    WriteFile();

    DisplayOne ( m_Line );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::WriteFile()
{
    theApp.BeginWaitCursor();

    //
    if ( m_Update.GetCheck() )
    {
        UpdateBuffer();
    }

    char    szFilename [ MAX_PATH ];
    ZeroMemory ( szFilename, sizeof ( szFilename ) );
    m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );

    BOOL bWritten = CTR8SaveGame::WriteFile ( szFilename );
    if ( bWritten )
    {
        m_Status.SetWindowText ( "File successfully written" );
    }
    else
    {
        MessageBox ( "Failed : File has not been written", MessageTitle, MB_OK|MB_ICONERROR );
        m_Status.SetWindowText ( "Failed : File has not been written." );
    }

    theApp.EndWaitCursor();

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::OnInitDialog()
{
    CTRXPropertyPage456::OnInitDialog();

    //
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_ListCtrl, LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool( &m_Flare, "Flares for TR2/TR3\r\nScions for TR1");

        m_ToolTip.AddTool( &m_Crystal, "Crystal");
        m_ToolTip.AddTool( &m_Kill, "Kills");
        m_ToolTip.AddTool( &m_Kill_Willard, "Kill Willard in TR3 Meteore Cavern\r\nOr Nearly Kill The Great Pyramid Boss\r\n");
        m_ToolTip.AddTool( &m_List_All, "List All Entries\r\nCan be useful if a game entry is not the correct one");
        m_ToolTip.AddTool( &m_Load, "Load a file");
        m_ToolTip.AddTool( &m_Max, "Set Max Ammos for all saves");
        m_ToolTip.AddTool( &m_Max_One, "Set Max Ammos for this save");
        m_ToolTip.AddTool( &m_Pickup, "Pickup");
        m_ToolTip.AddTool( &m_Refresh, "Reload a file");
        m_ToolTip.AddTool( &m_Remove_Loc, "Remove From Known Files");
        m_ToolTip.AddTool( &m_Secrets_A, "Secrets");
        m_ToolTip.AddTool( &m_Set, "Set Secrets for previous levels");
        m_ToolTip.AddTool( &m_Show, "Show Secrets for levels");
        m_ToolTip.AddTool( &m_Status, "Status");
        m_ToolTip.AddTool( &m_Update, "Update the modification in memory\r\nThen use Write to save the file");
        m_ToolTip.AddTool( &m_Write, "Use write to save modified file");
        m_ToolTip.AddTool( &m_ResetToPlus, "Reset to Start Plus conditions\r\nGet All guns\r\nReset Secrets, Pickups, Kill\r\n");
        m_ToolTip.AddTool( &m_ResetGames, "Reset to Start conditions\r\nReset Secrets, Pickups, Kill\r\n");

        //m_ToolTip.SetDelayTime(TTDT_AUTOPOP, 3000);
        //m_ToolTip.SetDelayTime(TTDT_RESHOW, 1000);
        m_ToolTip.Activate(TRUE);
    }

    //
    m_Update.SetCheck ( TRUE );

    if ( CTRXColors::m_iDarkTheme != 0 )
    {
        m_Load.SetIconResource ( IDI_LOAD_BLACK );
        m_Write.SetIconResource ( IDI_SAVE_BLACK );
        m_Refresh.SetIconResource ( IDI_REFRESH_BLACK );
    }
    else
    {
        m_Load.SetIconResource ( IDI_LOAD_WHITE );
        m_Write.SetIconResource ( IDI_SAVE_WHITE );
        m_Refresh.SetIconResource ( IDI_REFRESH_WHITE );
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
///////////////////////( //////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::OnSetActive()
{
    //
    BOOL bModified = IsGUIModified();
    if ( CTR8SaveGame::I() != NULL )
    {
        bModified = CTR8SaveGame::IsBufferModified();
    }

    //
    //
    if ( m_pListCtrl == NULL )
    {
        m_pListCtrl = &m_ListCtrl;

        m_ListCtrl.InsertColumn ( COLR_GAME, "Game", LVCFMT_LEFT, 48);
        m_ListCtrl.InsertColumn ( COLR_SUB_GAME, "Variant", LVCFMT_LEFT, 96);
        m_ListCtrl.InsertColumn ( COLR_SAVE_NO, "Save #", LVCFMT_RIGHT, 48);
        m_ListCtrl.InsertColumn ( COLR_LEVEL_NO, "Level #", LVCFMT_RIGHT, 48);
        m_ListCtrl.InsertColumn ( COLR_LEVEL_NAME, "Level Name", LVCFMT_LEFT, 150);
        m_ListCtrl.InsertColumn ( COLR_ADDRESS_1, "Address 1", LVCFMT_LEFT, 80);
        m_ListCtrl.InsertColumn ( COLR_ADDRESS_2, "Address 2", LVCFMT_LEFT, 80);
        m_ListCtrl.InsertColumn ( COLR_BLOCK, "Block", LVCFMT_RIGHT, 48);
        m_ListCtrl.InsertColumn ( COLR_SLOT, "Slot", LVCFMT_RIGHT, 32);
        m_ListCtrl.InsertColumn ( COLR_FOUND, "Found", LVCFMT_RIGHT, 44);
        m_ListCtrl.InsertColumn ( COLR_TOMBRAIDER, "TR", LVCFMT_RIGHT, 28);
        m_ListCtrl.InsertColumn ( COLR_KILLS, "K", LVCFMT_RIGHT, 32);
        m_ListCtrl.InsertColumn ( COLR_PICKUPS, "P", LVCFMT_RIGHT, 32);
        m_ListCtrl.InsertColumn ( COLR_ELAPSED, "Elapsed", LVCFMT_RIGHT, 60);
        m_ListCtrl.InsertColumn ( COLR_DISTANCE, "Distance", LVCFMT_RIGHT, 60);
        m_ListCtrl.InsertColumn ( COLR_EXTRA, "3", LVCFMT_RIGHT, 32);

        m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

        //  Set Default
        char szDefault [ MAX_PATH ];

        char szProfile [ MAX_PATH ];
        size_t iProfile = sizeof(szProfile);

        char szUsername [ MAX_PATH ];
        size_t iUsername = sizeof(szUsername);

        ZeroMemory ( szDefault, sizeof(szDefault) );
        ZeroMemory ( szProfile, sizeof(szProfile) );
        ZeroMemory ( szUsername, sizeof(szUsername) );

        getenv_s ( &iProfile, szProfile, sizeof(szProfile), "USERPROFILE");
        getenv_s ( &iUsername, szUsername, sizeof(szUsername), "USERNAME");


        if ( iProfile > 0 )
        {
            strcat_s ( szDefault, sizeof(szDefault), szProfile );
        }
        else if ( iUsername > 0 )
        {
            strcat_s ( szDefault, sizeof(szDefault), "C:\\Users\\" );
            strcat_s ( szDefault, sizeof(szDefault), szUsername );
        }
        else
        {
            strcat_s ( szDefault, sizeof(szDefault), "C:`\\Users\\yourname" );
        }
        strcat_s ( szDefault, sizeof(szDefault), "\\AppData\\Roaming\\TRX2\\72057598332895233\\savegame.dat" );
        m_Default.SetWindowText ( szDefault );

        //
        DisplayOne ( -1 );

        //  Add To Combo
        for ( int i = 0; i < LEN_LOCATION; i++ )
        {
            if ( strlen(LocationPathname [ i ].szPathname) > 0 )
            {
                AddComboString ( &m_Combo, LocationPathname [ i ].szPathname );
            }
        }

        //
        static char szFilename [ MAX_PATH ];
        ZeroMemory ( szFilename, sizeof(szFilename) );

        //
        CString lastRead  = "";
        if ( strlen(m_ParmPathname) > 0 )
        {
            lastRead = m_ParmPathname;
            ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
        }
        else
        {
            lastRead = theApp.GetProfileString( PROFILE_SETTING, PROFILE_TRX2_LAST_OPENED, szFilename );
        }

        //
        if ( lastRead != "" )
        {
            m_Filename.SetWindowText ( lastRead );
        }

        m_Filename.GetWindowText ( szFilename, sizeof(szFilename) );

        //  Show Current File
        SetComboSelection ( szFilename );

        //  Create Room
        m_Room.SetPointMode ( TRUE );
        m_Room.SetClickable ( TRUE, FALSE, FALSE );
        CTRXRoomPicture::CreateInstanceInside ( this, &m_Room, &m_Frame_Position, "", SS_NOTIFY );
        m_bRoomCreated = true;
        m_Room.ShowWindow ( SW_HIDE );
        m_Frame_Position.ShowWindow ( SW_HIDE );
    }

    //
    m_Status.SetWindowText ( "Tombraider Remastered 4, 5" );

    SetGUIModified ( bModified );

    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    if ( pos != NULL )
    {
        int nItem = m_ListCtrl.GetNextSelectedItem(pos);
        if ( nItem != -1 )
        {
            m_ListCtrl.EnsureVisible ( nItem, FALSE );
        }
    }

    //
    return CTRXPropertyPage456::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetComboSelection ( const char *pFilename )
{
    //  Show Current File
    for ( int i = 0; i < m_Combo.GetCount(); i++ )
    {
        CString selected;
        m_Combo.GetLBText ( i, selected );
        if ( _stricmp ( selected, pFilename ) == 0 )
        {
            m_SetManualCombo = false;
            m_Combo.SetCurSel ( i );
            m_SetManualCombo = true;
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetComboSelection ( )
{
    static char szFilename [ MAX_PATH ];
    ZeroMemory ( szFilename, sizeof(szFilename) );
    m_Filename.GetWindowText ( szFilename, sizeof(szFilename) );
    SetComboSelection ( szFilename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::OnKillActive()
{
    //
    UpdateBuffer();

    BOOL bModified = IsGUIModified();

    return CTRXPropertyPage456::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetValue ( CButton &button, int check, BOOL enable, BOOL bShowAnyWay )
{
    button.EnableWindow ( enable );
    if ( enable || bShowAnyWay )
    {
        button.SetCheck ( check );
    }
    else
    {
        button.SetCheck ( FALSE );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetValue ( CEdit &edit, int value, BOOL enable, BOOL bShowAnyWay )
{
    char szText [ 32 ];
    edit.EnableWindow ( enable );
    if ( enable || bShowAnyWay )
    {
        sprintf_s ( szText, sizeof(szText), "%d", value );
        edit.SetWindowTextA ( szText );
    }
    else
    {
        edit.SetWindowTextA ( "" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetBlockObjectOnDisplay(int tombraider, int block, int value)
{
    //  Value -1 mean the real value
    if ( value == -1 )
    {
    }
    //  -2 Mean Incremental
    else if ( value == -2 )
    {
    }
    else
    {
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::Enable ( int tombraider, int level )
{
    m_Guns.EnableWindow ( CTRXTools::IsEnablePistol ( tombraider, level ) );

    //
    m_Magnum.EnableWindow ( CTRXTools::IsEnableMagnum ( tombraider, level ) );
    m_Uzi.EnableWindow ( CTRXTools::IsEnableUzi ( tombraider, level ) );
    m_Riotgun.EnableWindow ( CTRXTools::IsEnableRiotgun ( tombraider, level ) );

    m_M16.EnableWindow ( CTRXTools::IsEnableM16 ( tombraider, level ) );
    m_Grenade.EnableWindow ( CTRXTools::IsEnableGrenade ( tombraider, level ) );
    m_Harpoon.EnableWindow ( CTRXTools::IsEnableHarpoon ( tombraider, level ) );

    m_Rocket.EnableWindow ( CTRXTools::IsEnableRocket ( tombraider, level ) );

    //
    m_AMagnum.EnableWindow ( CTRXTools::IsEnableMagnum ( tombraider, level ) );
    m_AUzi.EnableWindow ( CTRXTools::IsEnableUzi ( tombraider, level ) );
    m_ARiotgun.EnableWindow ( CTRXTools::IsEnableRiotgun ( tombraider, level ) );

    m_AM16.EnableWindow ( CTRXTools::IsEnableM16 ( tombraider, level ) );
    m_AGrenade.EnableWindow ( CTRXTools::IsEnableGrenade ( tombraider, level ) );
    m_AHarpoon.EnableWindow ( CTRXTools::IsEnableHarpoon ( tombraider, level ) );

    //
    m_ARocket.EnableWindow ( CTRXTools::IsEnableRocket ( tombraider, level ) );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DisplayOne ( int line )
{
    BOOL bModified = IsGUIModified();

    //
    static char szText [ MAX_PATH ];

    // check that one item gets the focus
    m_Line = line;

    if ( m_Line < 0 )
    {
        m_Line  = -1;
    }

    //
    if ( line >= 0 )
    {
        //
        ZeroMemory ( szText, sizeof(szText) );

        // DisplayOne (  pNMListView->iItem );
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slotFound           = -1;

        //
        const char *pLevelName = CTR8SaveGame::GetLevelName ( tombraider, CTR8SaveGame::I()->GetBlockLevelNumber (  tombraider, block ) );
        m_Status.SetWindowText ( pLevelName );

        //

        //
        void *pBlockEntry   = NULL;
        void *pGunEntry     = NULL;
        pBlockEntry     = CTR8SaveGame::I()->GetBlockAddress ( tombraider, block );
        pGunEntry       = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );

        //
        m_TR1_Plus.SetCheck ( CTR8SaveGame::I()->GetTRPlus ( 4 ) );
        m_TR2_Plus.SetCheck ( CTR8SaveGame::I()->GetTRPlus ( 5 ) );
        m_TR3_Plus.SetCheck ( CTR8SaveGame::I()->GetTRPlus ( 6 ) );

        //
        if ( pBlockEntry != NULL /* && pGunEntry != NULL */ )
        {
            if ( pGunEntry == NULL )
            {
                m_Status.SetWindowText (
                    "Updating ammos on this this row may not be good. "
                    "Only change pak, flares, keys/objects and guns. "
                    "you can get Max go to game and save again." );
            }

            //
            sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetSaveNumber ( tombraider, block ) );
            m_Save_No.SetWindowTextA ( szText );
            m_Save_No.EnableWindow ( TRUE );

            //
            ShowRoom();

            //
            switch ( tombraider )
            {
                case 4:
                {
                    TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlockEntry;
                    GUN_TR4 *pGun       = ( GUN_TR4 *) pGunEntry;

                    SET_BLOCK_VALUE(m_AGuns,m_iGunAmmos);
                    SET_GUN_VALUE(m_XGuns,m_gunPistol);

                    sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetRealHealth ( tombraider, block ) );
                    m_Real_Health.SetWindowTextA (szText );
                    if ( CTR8SaveGame::I()->GetRealHealthAddress ( tombraider, block ) == NULL )
                    {
                        m_Real_Health.EnableWindow ( FALSE );
                        m_Invincible.EnableWindow ( FALSE );
                    }
                    else
                    {
                        m_Real_Health.EnableWindow ( TRUE );
                        m_Invincible.EnableWindow ( TRUE );
                    }

                    SET_BLOCK_VALUE(m_LargePak,m_iLargeMedipak);
                    SET_BLOCK_VALUE(m_SmallPak,m_iSmallMedipak);
                    SET_BLOCK_VALUE(m_Flare,m_iFlares);

                    SET_BLOCK_HIDE(m_Col_Crystal);

                    SET_BLOCK_VALUE(m_AMagnum,m_iRevolverAmmos);
                    SET_GUN_VALUE(m_XMagnum,m_gunRevolver);

                    SET_BLOCK_VALUE(m_AUzi,m_iUziAmmos);
                    SET_GUN_VALUE(m_XUzi,m_gunUzis);
                    SET_BLOCK_UNITS(m_Uzi_Clip,m_iUziAmmos,UZI_CLIP);

                    SET_BLOCK_VALUE(m_ARiotgun,m_iShotGunAmmo1/6);
                    SET_GUN_VALUE(m_XRiotgun,m_gunRiotGun/6);

                    SET_BLOCK_VALUE(m_AGrenade,m_iGrenade1);
                    SET_GUN_VALUE(m_XGrenade,m_gunGrenadesLauncher);
                    SET_BLOCK_UNITS(m_Grenade_Clip,m_iGrenade1,GRENADE_CLIP);

                    SET_BLOCK_VALUE(m_AHarpoon,m_iCrossbow1);
                    SET_GUN_VALUE(m_XHarpoon,m_gunCrossBow);

                    SET_BLOCK_HIDE(m_AM16);
                    SET_GUN_HIDE(m_XM16);
                    SET_BLOCK_HIDE(m_M16_Clip);

                    SET_BLOCK_HIDE(m_ARocket);
                    SET_GUN_HIDE(m_XRocket);

                    //
                    if ( pGun != NULL )
                    {
                        SET_BLOCK_CHECK ( m_Guns, m_gunPistol, TRR4_GUN_SET );

                        SET_BLOCK_CHECK ( m_Magnum, m_gunRevolver, TRR4_GUN_SET );
                        SET_BLOCK_CHECK ( m_Uzi, m_gunUzis, TRR4_GUN_SET );
                        SET_BLOCK_CHECK ( m_Riotgun, m_gunRiotGun, TRR4_GUN_SET );
                        SET_BLOCK_CHECK ( m_Grenade, m_gunGrenadesLauncher, TRR4_GUN_SET );
                        SET_BLOCK_CHECK ( m_Harpoon, m_gunCrossBow, TRR4_GUN_SET );
                    }

                    // SET_BLOCK_HIDE ( m_Grenade );
                    // SET_BLOCK_HIDE ( m_Harpoon );
                    SET_BLOCK_HIDE ( m_M16 );
                    SET_BLOCK_HIDE ( m_Rocket );

                    SetBlockObjectOnDisplay ( tombraider, block, -1 );

                    //
                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                    m_Pickup.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockCrystal( tombraider, block ) );
                    m_Crystal.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockKills( tombraider, block ) );
                    m_Kill.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetPickupsForLevel ( tombraider, level - 1 ) );
                    m_Pickup_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetCrystalsForLevel ( tombraider, level - 1 ) );
                    m_Crystal_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetKillssForLevel ( tombraider, level - 1 ) );
                    m_Kill_Max.SetWindowText ( szText);

                    int acqsuired = CTR8SaveGame::I()->GetBlockSecretsAcquired( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", acqsuired );
                    m_Secrets_A.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetSecretsMaximum( tombraider, block ) );
                    m_Secrets_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockSecretsCurrent( tombraider, block ) );
                    m_Secrets_C.SetWindowText ( szText);

                    int completed =  CTR8SaveGame::I()->GetSecretsCompleted( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", completed );
                    m_Secrets_E.SetWindowText ( szText);

                    m_Pickup.EnableWindow ( TRUE );
                    m_Kill.EnableWindow ( TRUE );
                    m_Crystal.EnableWindow ( FALSE );
                    if ( acqsuired == completed )
                    {
                        m_Secrets_A.EnableWindow ( TRUE );
                    }
                    else
                    {
                        m_Secrets_A.EnableWindow ( FALSE );
                    }
                    m_Set.EnableWindow ( TRUE );

#ifdef _DEBUG
                    m_Secrets_A.EnableWindow ( TRUE );
#endif
                    m_Show.EnableWindow ( TRUE );

                    m_Kill_Willard.EnableWindow ( CTR8SaveGame::I()->IsKillEnabled ( tombraider, block ) );

                    if ( pGun != NULL )
                    {
                        sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetAir ( tombraider, block ) );
                        m_Air.SetWindowText ( szText );
                        m_Air.EnableWindow ( TRUE );

                        sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetState ( tombraider, block ) );
                        m_State.SetWindowText ( szText );
                        m_State.EnableWindow ( TRUE );
                        m_Burning.EnableWindow ( TRUE );
                        m_Burning.SetCheck ( CTR8SaveGame::I()->GetState ( tombraider, block ) == 0x1a );
                    }
                    else
                    {
                        m_Air.SetWindowText ( "" );
                        m_Air.EnableWindow ( FALSE );

                        m_State.SetWindowText ( "" );
                        m_State.EnableWindow ( FALSE );
                        m_Burning.EnableWindow ( FALSE );
                        m_Burning.SetCheck ( FALSE );
                    }

                    //
                    m_Game_Plus.SetCheck ( CTR8SaveGame::I()->IsGamePlus(tombraider, block) );
                    m_Game_Plus.EnableWindow ( TRUE );

                    //
                    m_Write.EnableWindow ( TRUE );
                    m_Max.EnableWindow ( TRUE );
                    m_ResetToPlus.EnableWindow ( TRUE );
                    m_ResetGames.EnableWindow ( TRUE );

                    Enable ( tombraider, level );

                    SetGUIModified ( bModified );

                    return;

                    break;
                }

                //
                case 5:
                {
                    TABLE_TR5 *pBlock = ( TABLE_TR5 *) pBlockEntry;
                    GUN_TR5 *pGun = ( GUN_TR5 *) pGunEntry;

                    SET_BLOCK_VALUE(m_AGuns,m_iGunAmmos);
                    SET_GUN_VALUE(m_XGuns,m_gunPistol);

                    sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetRealHealth( tombraider, block ) );
                    m_Real_Health.SetWindowTextA (szText );
                    if ( CTR8SaveGame::I()->GetRealHealthAddress ( tombraider, block ) == NULL )
                    {
                        m_Real_Health.EnableWindow ( FALSE );
                        m_Invincible.EnableWindow ( FALSE );
                    }
                    else
                    {
                        m_Real_Health.EnableWindow ( TRUE );
                        m_Invincible.EnableWindow ( TRUE );
                    }

                    SET_BLOCK_VALUE(m_LargePak,m_iLargeMedipak);
                    SET_BLOCK_VALUE(m_SmallPak,m_iSmallMedipak);
                    SET_BLOCK_VALUE(m_Flare,m_iFlares);
                    SET_BLOCK_HIDE(m_Col_Crystal);

                    SET_BLOCK_VALUE(m_AMagnum,m_iRevolverAmmos);
                    SET_GUN_VALUE(m_XMagnum,m_gunRevolver);

                    SET_BLOCK_VALUE(m_AUzi,m_iUziAmmos);
                    SET_GUN_VALUE(m_XUzi,m_gunUzis);
                    SET_BLOCK_UNITS(m_Uzi_Clip,m_iUziAmmos,UZI_CLIP);

                    SET_BLOCK_VALUE(m_ARiotgun,m_iShotGunAmmo1/6);
                    SET_GUN_VALUE(m_XRiotgun,m_gunRiotGun/6);

                    SET_BLOCK_VALUE(m_AM16,m_iHKAmmos);
                    SET_GUN_VALUE(m_XM16,m_gunHK);
                    SET_BLOCK_UNITS(m_M16_Clip,m_iHKAmmos,M16_CLIP);

                    //  Grenade Used as Grapling
                    SET_BLOCK_VALUE(m_AGrenade,m_iGrapplingAmmos);
                    SET_GUN_VALUE(m_XGrenade,m_gunGrappling);

                    SET_BLOCK_HIDE(m_ARocket);
                    SET_GUN_HIDE(m_XRocket);

                    SET_BLOCK_HIDE(m_AHarpoon);
                    SET_GUN_HIDE(m_XHarpoon);

                    //
                    //
                    if ( pGun != NULL )
                    {
                        SET_BLOCK_CHECK ( m_Guns, m_gunPistol, TRR5_GUN_SET );

                        SET_BLOCK_CHECK ( m_Uzi, m_gunUzis, TRR5_GUN_SET );
                        SET_BLOCK_CHECK ( m_Riotgun, m_gunRiotGun, TRR5_GUN_SET );
                        SET_BLOCK_CHECK ( m_Grenade, m_gunGrappling, TRR5_GUN_SET );
                        SET_BLOCK_CHECK ( m_M16, m_gunHK, TRR5_GUN_SET );
                        SET_BLOCK_CHECK ( m_Magnum, m_gunRevolver, TRR5_GUN_SET );
                    }
                     
                    SET_BLOCK_HIDE ( m_Rocket );
                    SET_BLOCK_HIDE ( m_Harpoon );

                    //
                    SetBlockObjectOnDisplay ( tombraider, block, -1 );

                    //
                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                    m_Pickup.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockCrystal( tombraider, block ) );
                    m_Crystal.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockKills( tombraider, block ) );
                    m_Kill.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetPickupsForLevel ( tombraider, level - 1 ) );
                    m_Pickup_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetCrystalsForLevel ( tombraider, level - 1 ) );
                    m_Crystal_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetKillssForLevel ( tombraider, level - 1 ) );
                    m_Kill_Max.SetWindowText ( szText);

                    int acqsuired = CTR8SaveGame::I()->GetBlockSecretsAcquired( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", acqsuired );
                    m_Secrets_A.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetSecretsMaximum( tombraider, block ) );
                    m_Secrets_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR8SaveGame::I()->GetBlockSecretsCurrent( tombraider, block ) );
                    m_Secrets_C.SetWindowText ( szText);

                    int completed =  CTR8SaveGame::I()->GetSecretsCompleted( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", completed );
                    m_Secrets_E.SetWindowText ( szText);

                    m_Pickup.EnableWindow ( TRUE );
                    m_Crystal.EnableWindow ( FALSE );
                    m_Kill.EnableWindow ( TRUE );
                    if ( acqsuired == completed )
                    {
                        m_Secrets_A.EnableWindow ( TRUE );
                    }
                    else
                    {
                        m_Secrets_A.EnableWindow ( FALSE );
                    }
                    m_Set.EnableWindow ( TRUE );

#ifdef _DEBUG
                    m_Secrets_A.EnableWindow ( TRUE );
#endif
                    m_Show.EnableWindow ( TRUE );
                    m_Kill_Willard.EnableWindow ( CTR8SaveGame::I()->IsKillEnabled ( tombraider, block ) );

                    if ( pGun != NULL )
                    {
                        sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetAir ( tombraider, block) );
                        m_Air.SetWindowText ( szText );
                        m_Air.EnableWindow ( TRUE );

                        sprintf_s ( szText, sizeof(szText), "%d", CTR8SaveGame::I()->GetState ( tombraider, block) );
                        m_State.SetWindowText ( szText );
                        m_State.EnableWindow ( TRUE );
                        m_Burning.EnableWindow ( TRUE );
                        m_Burning.SetCheck ( CTR8SaveGame::I()->GetState ( tombraider, block ) == 0x1a );
                    }
                    else
                    {
                        m_Air.SetWindowText ( "" );
                        m_Air.EnableWindow ( FALSE );

                        m_State.SetWindowText ( "" );
                        m_State.EnableWindow ( FALSE );
                        m_Burning.EnableWindow ( FALSE );
                        m_Burning.SetCheck ( FALSE );
                    }

                    //
                    m_Game_Plus.SetCheck ( CTR8SaveGame::I()->IsGamePlus(tombraider, block) );
                    m_Game_Plus.EnableWindow ( TRUE );

                    //
                    m_Write.EnableWindow ( TRUE );
                    m_Max.EnableWindow ( TRUE );
                    m_ResetToPlus.EnableWindow ( TRUE );
                    m_ResetGames.EnableWindow ( TRUE );

                    Enable ( tombraider, level );

                    SetGUIModified ( bModified );

                    return;

                    break;
                }

                //
                case 6:
                {
                    break;
                }
            }
        }
    }

    //
    m_Save_No.EnableWindow ( FALSE );

    SetValue ( m_AGuns, 0, FALSE );
    SetValue ( m_XGuns, 0, FALSE );

    SetValue ( m_LargePak, 0, FALSE );
    SetValue ( m_SmallPak, 0, FALSE );
    SetValue ( m_Flare, 0, FALSE );
    SetValue ( m_Col_Crystal, 0, FALSE );

    SetValue ( m_Guns, 0, FALSE );

    SetValue ( m_Magnum, 0, FALSE );
    SetValue ( m_AMagnum, 0, FALSE );
    SetValue ( m_XMagnum, 0, FALSE );

    SetValue ( m_Uzi, 0, FALSE );
    SetValue ( m_AUzi, 0, FALSE );
    SetValue ( m_XUzi, 0, FALSE );
    SetValue ( m_Uzi_Clip, 0, FALSE );

    SetValue ( m_Riotgun, 0, FALSE );
    SetValue ( m_ARiotgun, 0, FALSE );
    SetValue ( m_XRiotgun, 0, FALSE );

    SetValue ( m_Grenade, 0, FALSE );
    SetValue ( m_AGrenade, 0, FALSE );
    SetValue ( m_XGrenade, 0, FALSE );

    SetValue ( m_Harpoon, 0, FALSE );
    SetValue ( m_AHarpoon, 0, FALSE );
    SetValue ( m_XHarpoon, 0, FALSE );

    SetValue ( m_M16, 0, FALSE );
    SetValue ( m_AM16, 0, FALSE );
    SetValue ( m_XM16, 0, FALSE );
    SetValue ( m_M16_Clip, 0, FALSE );

    SetValue ( m_Rocket, 0, FALSE );
    SetValue ( m_ARocket, 0, FALSE );
    SetValue ( m_XRocket, 0, FALSE );

    SetValue ( m_Air, 0, FALSE );
    SetValue ( m_State, 0, FALSE );

    m_Real_Health.EnableWindow ( FALSE );
    m_Invincible.EnableWindow ( FALSE );

    m_Burning.EnableWindow ( FALSE );
    m_Write.EnableWindow ( FALSE );
    m_Max.EnableWindow ( FALSE );
    m_ResetToPlus.EnableWindow ( FALSE );
    m_ResetGames.EnableWindow ( FALSE );

    m_Pickup.EnableWindow ( FALSE );
    m_Crystal.EnableWindow ( FALSE );
    m_Kill.EnableWindow ( FALSE );

    m_Pickup_Max.SetWindowText ( "" );
    m_Crystal_Max.SetWindowText ( "" );
    m_Kill_Max.SetWindowText ( "" );

    //
    m_Secrets_A.EnableWindow ( FALSE );

    m_Set.EnableWindow ( FALSE );
    m_Show.EnableWindow ( FALSE );

    m_Game_Plus.EnableWindow ( FALSE );

    m_Kill_Willard.EnableWindow ( FALSE );

    SetGUIModified ( bModified );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedRefresh()
{
    //
    char    szFilename [ MAX_PATH ];

    //
    if ( m_Update.GetCheck() )
    {
        UpdateBuffer();
    }

    //
    if ( CTR8SaveGame::IsBufferModified() )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR456 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            ZeroMemory ( szFilename, sizeof ( szFilename ) );
            m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
            CTR8SaveGame::WriteFile ( szFilename );
        }
        else if ( response == IDCANCEL )
        {
            return;
        }
        else
        {
            CTR8SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    //
    ZeroMemory ( szFilename, sizeof ( szFilename ) );
    m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );

    //
    //
    //  To SHow Item Again
    int currentTombraider   = -1;
    int currentLevel        = -1;
    int currentBlock        = -1;
    int currentSlot         = -1;
    if ( m_Line >= 0 && CTR8SaveGame::I() != NULL )
    {
        //
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        currentTombraider           = pInfoData->tombraider;
        currentLevel                = pInfoData->level;
        currentBlock                = pInfoData->block;
    }

    DisplayList ( szFilename );

    if ( currentTombraider >= 1 && currentBlock > 0 && currentSlot > 0 )
    {
        for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
        {
            DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( i );
            STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
            if ( currentTombraider == pInfoData->tombraider && currentBlock == pInfoData->block )
            {
                m_ListCtrl.EnsureVisible ( i, FALSE );
                break;
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DisplayList ( const char *pFilename )
{
    //
    m_ListCtrl.DeleteAllItems();

    //
    ZeroMemory ( tagData, sizeof(tagData) );

    m_Line = -1;

    //
    if ( pFilename != NULL && strlen(pFilename) > 0 )
    {
        //
        m_Filename.SetWindowText ( pFilename );

        //  Retrieve Info
        BOOL bRead = CTR8SaveGame::I()->ReadFile ( pFilename );
        if ( bRead )
        {
            m_Status.SetWindowText ( "File sucessfuily read" );
            CTR8SaveGame::I()->Load ( );
            BOOL bWritten = theApp.WriteProfileString ( PROFILE_SETTING, PROFILE_TRX2_LAST_OPENED, pFilename );

        }
        else
        {
            m_Status.SetWindowText ( "File read fails" );
            m_Filename.SetWindowText ( "" );

            //
            RemoveLocation ( LocationPathname, pFilename );

            //
            m_SetManualCombo = false;

            //
            RemoveFromCombo ( &m_Combo, pFilename );

            //
            m_SetManualCombo = true;
        }

        //
        DisplayList ( );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DisplayList ( )
{
    //
    if ( m_List_All.GetCheck() )
    {
        DisplayListFull ( false );
    }
    else
    {
        DisplayListBrief();
    }

    //
    if ( iSortColumn == -1 )
    {
        iSortAscending  = 1;
        iSortColumn     = COLR_GAME;
    }

    SortItems ( iSortColumn );

    //
    if ( m_ListCtrl.GetItemCount() > 0 )
    {
        m_Line  = 0;
        DisplayOne ( m_Line );
    }

    SetGUIModified ( FALSE );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DisplayListBrief ( )
{
    BOOL bModified = IsGUIModified();

    //
    char szString [ MAX_PATH ];

    //
    DisplayOne ( -1 );

    //
    m_SetManualList = false;

    //
    m_ListCtrl.DeleteAllItems();

    //
    ZeroMemory ( tagData, sizeof(tagData) );

    m_Line = -1;

    //
    //  TR1
    int position    = 0;

    //
    int tombraider = 4;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        WORD save   = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR4" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 4");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            void *pAddress1 = CTR8SaveGame::I()->SearchBlockEntry ( tombraider, block );
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR8SaveGame::I()->SearchGunEntry ( tombraider, block);
            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }

            //
            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "1");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR1 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );

            position++;
        }
    }

    //  TR5
    tombraider = 5;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        WORD save   = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR5" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 5");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );


            void *pAddress1 = CTR8SaveGame::I()->SearchBlockEntry ( tombraider, block );
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR8SaveGame::I()->SearchGunEntry ( tombraider, block);
            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }

            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "2");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR2 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );

            position++;
        }
    }

    //  TR6
    tombraider = 6;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        WORD save   = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR6" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 6");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            void *pAddress1 = CTR8SaveGame::I()->SearchBlockEntry ( tombraider, block );
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR8SaveGame::I()->SearchGunEntry ( tombraider, block);
            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }


            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "3");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR3 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );

            position++;
        }
    }

    m_SetManualList = true;
    SetGUIModified ( bModified );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DisplayListFull ( bool bShort )
{
    //
    BOOL bModified = IsGUIModified();

    //
    char szString [ MAX_PATH ];

    //
    DisplayOne ( -1 );


    //
    m_SetManualList = false;

    //
    m_ListCtrl.DeleteAllItems();

    //
    ZeroMemory ( tagData, sizeof(tagData) );

    //
    m_Line              = -1;

    //
    //  TR1
    int position = 0;

    //
    int tombraider = 4;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        TABLE_TR4 *pAddress1    = (TABLE_TR4 *) CTR8SaveGame::I()->GetBlockAddress ( tombraider, block );
        void *pAddress2         = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );

        if ( bShort && pAddress2 == NULL )
        {
            continue;
        }

        WORD    save    = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD    level   = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( /* save != 0 && */ level != 0 && pAddress1 != NULL /* && ( pAddress1 ->gunmask & 1 ) == 1 **/ )
        {
            m_ListCtrl.InsertItem ( position, "TR4" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 4");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }
            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "1");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR1 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );
            position++;
        }
    }

    //
    tombraider = 5;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        TABLE_TR5 *pAddress1 = (TABLE_TR5 *) CTR8SaveGame::I()->GetBlockAddress ( tombraider, block );
        void *pAddress2 = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );

        WORD    save    = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD    level   = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( /* save != 0 && */ level != 0 && pAddress1 != NULL /* && ( pAddress1 ->gunmask & 1 ) == 1 */ )
        {
            m_ListCtrl.InsertItem ( position, "TR5" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 5");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }
            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "2");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR1 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );
            position++;
        }
    }

    //
    tombraider = 6;
    for ( int block = 0; block < NB_SLOT_456; block++ )
    {
        TABLE_TR6 *pAddress1 = (TABLE_TR6 *) CTR8SaveGame::I()->GetBlockAddress ( tombraider, block );
        void *pAddress2 = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );

        WORD    save    = CTR8SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD    level   = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( /* save != 0 && */ level != 0 && pAddress1 != NULL /* && ( pAddress1 ->gunmask & 1 ) == 1 */ )
        {
            m_ListCtrl.InsertItem ( position, "TR6" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 6");

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR8SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR8SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            if ( pAddress2 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress2 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_2, "" );
            }

            sprintf_s  ( szString, sizeof(szString), "%02d", block);
            m_ListCtrl.SetItemText ( position, COLR_BLOCK, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", block );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR8SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "3");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR8SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR8SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR8SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR8SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR1 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );
            position++;
        }
    }

    //
    m_SetManualList = true;

    SetGUIModified ( bModified );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock(TABLE_TR4 *pBlock, bool bMax, int level )
{
    int tombraider = 4;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
            pBlock->m_iGunAmmos         = MAX_GUN;

            pBlock->m_iLargeMedipak     = MAX_PAK;
            pBlock->m_iSmallMedipak     = MAX_PAK;
            pBlock->m_iFlares           = MAX_PAK;

            CTR8SaveGame::I()->SetPistols ( tombraider, pBlock, NULL, level, true );
            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 2, true );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 1, true );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, true, MAX_AMMOS, true );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 1, true );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 2, true );
        }
        else
        {
            pBlock->m_iGunAmmos         = GetValue ( m_AGuns );

            pBlock->m_iLargeMedipak     = GetValue ( m_LargePak );
            pBlock->m_iSmallMedipak     = GetValue ( m_SmallPak );
            pBlock->m_iFlares           = GetValue ( m_Flare );

            CTR8SaveGame::I()->SetPistols ( tombraider, pBlock, NULL, level, GetValue ( m_Guns ) );
            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
         }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock (int tombraider, int block, TABLE_TR4 *pBlock, const GUN_TR4 *pGun, bool bMax, int level )
{
    //
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateGun(int tombraider, int block, GUN_TR4 *pGun, TABLE_TR4 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 2, true );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 1, true );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, true, MAX_AMMOS, true );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 1, true );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 2, true );

            CTR8SaveGame::I()->SetAir ( tombraider, block,  MAX_AIR );
            CTR8SaveGame::I()->SetState ( tombraider, block,  STATE_SAFE );
            CTR8SaveGame::I()->SetRealHealth ( tombraider, block,  MAX_HEALTH );
        }
        else
        {
            // pGun->m_gunPistol   = GetValue ( m_AGuns );
            // pGun->m_gunPistol      = GetValue ( m_AGuns );

            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );

            CTR8SaveGame::I()->SetAir ( tombraider, block,  GetValue ( m_Air ) );
            CTR8SaveGame::I()->SetState ( tombraider, block,  GetValue ( m_State ) );
            CTR8SaveGame::I()->SetRealHealth ( tombraider, block,  GetValue ( m_Real_Health ) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBuffer(int tombraider, int block, TABLE_TR4 *pBlock, GUN_TR4 *pGun, bool bMax)
{
    int level = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock != NULL )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level );
        
        if ( pGun )
        {
            UpdateGun ( tombraider, block, pGun, pBlock,  bMax, level );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock(TABLE_TR5 *pBlock, bool bMax, int level )
{
    int tombraider = 5;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
            pBlock->m_iGunAmmos         = MAX_GUN;

            pBlock->m_iLargeMedipak     = MAX_PAK;
            pBlock->m_iSmallMedipak     = MAX_PAK;
            pBlock->m_iFlares           = MAX_PAK;

            CTR8SaveGame::I()->SetPistols ( tombraider, pBlock, NULL, level, true );
            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 2, true );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 1, true );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, true, MAX_AMMOS, true );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 1, true );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 2, true );
            CTR8SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 3, true );
        }
        else
        {
            pBlock->m_iGunAmmos         = GetValue ( m_AGuns );

            pBlock->m_iLargeMedipak     = GetValue ( m_LargePak );
            pBlock->m_iSmallMedipak     = GetValue ( m_SmallPak );
            pBlock->m_iFlares           = GetValue ( m_Flare );

            CTR8SaveGame::I()->SetPistols ( tombraider, pBlock, NULL, level, GetValue ( m_Guns ) );
            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR8SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );

        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock(int tombraider, int block, TABLE_TR5 *pBlock, GUN_TR5 *pGun, bool bMax, int level)
{
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateGun(int tombraider, int block, GUN_TR5 *pGun, TABLE_TR5 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
            // pGun->m_gunPistol      = MAX_GUN;

            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 2, true );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 1, true );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, true, MAX_AMMOS, true );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 1, true );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 2, true );
            CTR8SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 3, true );

            CTR8SaveGame::I()->SetAir ( tombraider, block,  MAX_AIR );
            CTR8SaveGame::I()->SetState ( tombraider, block,  STATE_SAFE );
            CTR8SaveGame::I()->SetRealHealth ( tombraider, block,  MAX_HEALTH );
        }
        else
        {
            // pGun->m_gunPistol      = GetValue ( m_AGuns );

            CTR8SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR8SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR8SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR8SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR8SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR8SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );

            CTR8SaveGame::I()->SetAir ( tombraider, block,  GetValue ( m_Air ) );
            CTR8SaveGame::I()->SetState ( tombraider, block,  GetValue ( m_State ) );
            CTR8SaveGame::I()->SetRealHealth ( tombraider, block,  GetValue ( m_Real_Health ) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBuffer(int tombraider, int block, TABLE_TR5 *pBlock, GUN_TR5 *pGun, bool bMax )
{
    //  Home Sweet Home : 18
    int level = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level );

        if ( pGun )
        {
            UpdateGun ( tombraider, block, pGun, pBlock, bMax, level );
        }
    }
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock (TABLE_TR6 *pBlock, bool bMax, int level)
{
    int tombraider = 6;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
        }
        else
        {
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBlock(int tombraider, int block, TABLE_TR6 *pBlock, GUN_TR6 *pGun, bool bMax, int level )
{
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateGun(int tombraider, int block, GUN_TR6 *pGun, TABLE_TR6 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
        }
        else
        {
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBuffer(int tombraider, int block, TABLE_TR6 *pBlock, GUN_TR6 *pGun, bool bMax )
{
    int level = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock != NULL )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level );

        if ( pGun )
        {
            UpdateGun ( tombraider, block, pGun, pBlock, bMax, level );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::UpdateBuffer( )
{
    char szText [ MAX_PATH ];

    if ( CTR8SaveGame::I(FALSE) == NULL )
    {
        return;
    }

    //
    bool bMax = false;

    //
    if ( m_Line >= 0 )
    {
        //
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        void *pBlockEntry   = NULL;
        void *pGunEntry     = NULL;

        //
        pBlockEntry     = CTR8SaveGame::I()->GetBlockAddress( tombraider, block );
        pGunEntry       = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );

        CTR8SaveGame::I()->SetTRPlus ( 1, m_TR1_Plus.GetCheck () );
        CTR8SaveGame::I()->SetTRPlus ( 2, m_TR2_Plus.GetCheck () );
        CTR8SaveGame::I()->SetTRPlus ( 3, m_TR3_Plus.GetCheck () );


        CTR8SaveGame::I()->SetSaveNumber ( tombraider, block, GetValue ( m_Save_No ) );
        sprintf_s ( szText, sizeof(szText), "%03d", GetValue ( m_Save_No ) );
        m_ListCtrl.SetItemText ( m_Line, COLR_SAVE_NO, szText);

        //
        if ( pBlockEntry != NULL /* && pGunEntry != NULL */ )
        {
            switch ( tombraider )
            {
                //
                case 4:
                {
                    //  Update All Occurrences
                    TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlockEntry;
                    GUN_TR4 *pGun       = ( GUN_TR4 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                    //  Object and keys are for a block

                    CTR8SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR8SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR8SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR8SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );

                    break;
                }

                //
                case 5:
                {
                    TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlockEntry;
                    GUN_TR5 *pGun       = ( GUN_TR5 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                    //  Object and keys are for a block
                    CTR8SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR8SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR8SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR8SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );

                    break;
                }

                //
                case 6:
                {
#if 0
                    TABLE_TR6 *pBlock   = ( TABLE_TR6 *) pBlockEntry;
                    GUN_TR6 *pGun       = ( GUN_TR6 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                    //  Object and keys are for a block
                    CTR8SaveGame::I()->SetBlockObject ( tombraider, block, 0, GetValue ( m_Object_1 ) );
                    CTR8SaveGame::I()->SetBlockObject ( tombraider, block, 1, GetValue ( m_Object_2 ) );
                    CTR8SaveGame::I()->SetBlockObject ( tombraider, block, 2, GetValue ( m_Object_3 ) );
                    CTR8SaveGame::I()->SetBlockObject ( tombraider, block, 3, GetValue ( m_Object_4 ) );

                    CTR8SaveGame::I()->SetKey ( tombraider, block, 0, GetValue ( m_Key_1 ) );
                    CTR8SaveGame::I()->SetKey ( tombraider, block, 1, GetValue ( m_Key_2 ) );
                    CTR8SaveGame::I()->SetKey ( tombraider, block, 2, GetValue ( m_Key_3 ) );
                    CTR8SaveGame::I()->SetKey ( tombraider, block, 3, GetValue ( m_Key_4 ) );

                    CTR8SaveGame::I()->SetMap ( tombraider, block, 0, GetValue ( m_Map_1 ) );
                    CTR8SaveGame::I()->SetMap ( tombraider, block, 1, GetValue ( m_Map_2 ) );

                    CTR8SaveGame::I()->SetTreasure ( tombraider, block, 0, GetValue ( m_Tre_1 ) );
                    CTR8SaveGame::I()->SetTreasure ( tombraider, block, 1, GetValue ( m_Tre_2 ) );
                    CTR8SaveGame::I()->SetTreasure ( tombraider, block, 2, GetValue ( m_Tre_3 ) );
                    CTR8SaveGame::I()->SetTreasure ( tombraider, block, 3, GetValue ( m_Tre_4 ) );

                    CTR8SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR8SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR8SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR8SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );
#endif
                    break;
                }
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXRemastered456::GetValue ( CEdit &edit )
{
    //
    int result = CTRXPropertyPage456::GetValue ( edit );

    //
    if ( edit == m_AGuns )
    {
        if ( result < 0 )
        {
            result = MAX_HEALTH;
        }
    }
    else if ( edit == m_Air )
    {
        if ( result < 0 )
        {
            result = MAX_AIR;
        }
    }
    else if ( edit == m_Real_Health )
    {
        if ( result < 0 )
        {
            result = MAX_HEALTH;
        }
    }
    else
    {
        if ( result > MAX_AMMOS_MAX )
        {
            result = MAX_AMMOS_MAX;
        }
    }

    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::GetValue ( CButton &button )
{
    return CTRXPropertyPage456::GetValue ( button );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickListctrl(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    //  Unselected
    if (    ( pNMListView->uNewState & LVIS_SELECTED ) == 0 &&
            ( pNMListView->uOldState & LVIS_SELECTED ) == LVIS_SELECTED )
    {
        if ( m_SetManualList && IsGUIModified() )
        {
            if ( m_Update.GetCheck() )
            {
                UpdateBuffer();
            }
        }

        m_Line = -1;

        *pResult = TRUE;
        return;
    }

    //  Newly Selected
    if (    ( pNMListView->uOldState & LVIS_SELECTED ) == 0 &&
            ( pNMListView->uNewState & LVIS_SELECTED ) == LVIS_SELECTED )
    {
        if ( m_SetManualList && pNMListView->iItem >= 0 )
        {
            m_Line = pNMListView->iItem;

            DisplayOne ( pNMListView->iItem );
        }
    }

    *pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  LVIS_SELECTED
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnItemchangedListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // Unselected
    if (    ( pNMListView->uNewState & LVIS_SELECTED ) == 0 &&
            ( pNMListView->uOldState & LVIS_SELECTED ) == LVIS_SELECTED )
    {
        if ( m_SetManualList && IsGUIModified() )
        {
            if ( m_Update.GetCheck() )
            {
                UpdateBuffer();
            }
        }

        m_Line = -1;

        *pResult = TRUE;
        return;
    }

    //  Newly Selected
    if (    ( pNMListView->uOldState & LVIS_SELECTED ) == 0 &&
            ( pNMListView->uNewState & LVIS_SELECTED ) == LVIS_SELECTED )
    {
        if ( m_SetManualList && pNMListView->iItem >= 0 )
        {
            m_Line = pNMListView->iItem;

            DisplayOne ( pNMListView->iItem );
        }
    }

    *pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int CALLBACK SortFuncRemastered(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
  {
    STRUCTDATA  *pData1 = (STRUCTDATA*)lParam1;
    STRUCTDATA  *pData2 = (STRUCTDATA*)lParam2;
    DWORD   dwData      = *( DWORD *)lParamSort;

    if ( pData1 != NULL && pData2 != NULL )
    {
        return iSortAscending * _stricmp ( pData1->szText, pData2->szText );
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SortItems ( int col )
{
    m_SetManualList = false;

    char    szString [ MAX_PATH ];

    for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        STRUCTDATA  *pData = (STRUCTDATA *) m_ListCtrl.GetItemData ( i );

        ZeroMemory ( szString, sizeof(szString) );
        m_ListCtrl.GetItemText( i, col, szString, sizeof(szString) );

        ZeroMemory ( pData->szText, sizeof( pData->szText) );
        strcpy_s ( pData->szText, sizeof( pData->szText), szString );

        ZeroMemory ( szString, sizeof(szString) );
        m_ListCtrl.GetItemText( i, COLR_GAME, szString, sizeof(szString) );
        strcat_s ( pData->szText, sizeof( pData->szText), szString );

        ZeroMemory ( szString, sizeof(szString) );
        m_ListCtrl.GetItemText( i, COLR_LEVEL_NO, szString, sizeof(szString) );
        strcat_s ( pData->szText, sizeof( pData->szText), szString );

        ZeroMemory ( szString, sizeof(szString) );
        m_ListCtrl.GetItemText( i, COLR_SLOT, szString, sizeof(szString) );
        strcat_s ( pData->szText, sizeof( pData->szText), szString );
    }

    DWORD       dwData  = col;
    DWORD_PTR   pdwData = ( DWORD_PTR ) &dwData;
    m_ListCtrl.SortItems(SortFuncRemastered, pdwData );

    m_SetManualList = true;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    //
    NMLISTVIEW *pLV = (NMLISTVIEW *) pNMHDR;
    
    if ( pLV->iItem == iSortColumn )
    {
        iSortAscending *= -1;
    }
    else
    {
        iSortAscending  = 1;
    }

    SortItems ( pLV->iItem );
    iSortColumn = pLV->iItem;
    
    

    *pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ResetToGame ( int line )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    CTR8SaveGame::I()->SetLikeGame ( tombraider, block );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ResetToPlus ( int line )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    CTR8SaveGame::I()->SetLikePlus ( tombraider, block );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::MaxOne ( int line )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    bool bMax = true;

    //
    pBlockEntry     = CTR8SaveGame::I()->GetBlockAddress( tombraider, block );
    pGunEntry       = CTR8SaveGame::I()->GetGunAddress ( tombraider, block );


    if ( pBlockEntry != NULL /* && pGunEntry != NULL */ )
    {
        switch ( tombraider )
        {
            //
            case 4:
            {
                TABLE_TR4 *pBlock   = ( TABLE_TR4 *)pBlockEntry;
                GUN_TR4 *pGun       = ( GUN_TR4 * ) pGunEntry;
                UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                break;
            }

            //
            case 5:
            {
                TABLE_TR5 *pBlock   = ( TABLE_TR5 *)pBlockEntry;
                GUN_TR5 *pGun       = ( GUN_TR5 * ) pGunEntry;
                UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                break;
            }

            //
            case 3:
            {
                TABLE_TR6 *pBlock   = ( TABLE_TR6 *)pBlockEntry;
                GUN_TR6 *pGun       = ( GUN_TR6 * ) pGunEntry;
                UpdateBuffer ( tombraider, block, pBlock, pGun, bMax );

                break;
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedMax()
{
    //  For All Lines
    SetGUIModified( TRUE );

    for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        MaxOne ( i );
    }

    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedMaxOne()
{
    if ( m_Line >= 0 && m_Line < m_ListCtrl.GetItemCount() )
    {
        MaxOne ( m_Line );
        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedArFull()
{
    char szString [ 32 ];
    sprintf_s ( szString, sizeof(szString), "%d", MAX_AIR );
    m_Air.SetWindowText ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedArInfinite()
{
    char szString [ 32 ];
    sprintf_s ( szString, sizeof(szString), "%d", 0x7fff );
    m_Air.SetWindowText ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedListAll()
{
    //
    DisplayList ( );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedUpdate()
{
    if ( m_Update.GetCheck() )
    {
        UpdateBuffer();
    }
    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnSelchangeCombo()
{
    static char szFilename [ MAX_PATH ];

    if ( m_SetManualCombo )
    {
        //
        if ( CTR8SaveGame::I() != NULL )
        {
            if ( m_Update.GetCheck() )
            {
                UpdateBuffer();
            }

            if ( CTR8SaveGame::IsBufferModified() )
            {
                INT_PTR response = CTRXDifferences::MessageBoxR456 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
                if ( response == IDYES )
                {
                    DisplayOne ( m_Line );
                    ZeroMemory ( szFilename, sizeof ( szFilename ) );
                    m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
                    CTR8SaveGame::WriteFile ( szFilename );
                }
                else if ( response == IDCANCEL )
                {
                    SetComboSelection ( );
                    return;
                }
                else
                {
                    CTR8SaveGame::Discard();
                    DisplayOne ( m_Line );
                }
            }
        }

        //
        int iCurSel = m_Combo.GetCurSel();
        if ( iCurSel >= 0 && iCurSel < m_Combo.GetCount() )
        {
            // m_Directory.Get
            char szFilename [ MAX_PATH ];
            m_Combo.GetLBText( iCurSel, szFilename );

            //
            DisplayList ( szFilename );
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedBurning()
{
    SetGUIModified( TRUE );

    if ( m_Burning.GetCheck() )
    {
        m_State.SetWindowText ( "0x1a" );
    }
    else
    {
        m_State.SetWindowText ( "0x0a" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedRemoveLoc()
{
    //
    int iCurSel = m_Combo.GetCurSel();
    if ( iCurSel >= 0 && iCurSel < m_Combo.GetCount() )
    {
        // m_Directory.Get
        char szFilename [ MAX_PATH ];
        m_Combo.GetLBText( iCurSel, szFilename );
        BOOL bRemoved = RemoveLocation ( LocationPathname, szFilename );

        //
        RemoveFromCombo ( &m_Combo, iCurSel );

        //
        iCurSel = m_Combo.GetCurSel();
        if ( iCurSel >= 0 )
        {
            m_Combo.SetCurSel ( iCurSel );
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedStrong()
{
    m_Real_Health.SetWindowTextA ( "0x7FFF" );
}

//
/////////////////////////////////////////////////////////////////////////////
//  iObject 0-3     Objects
//  iObject 4-7     Keys
//  iObject 8-9     Maps
//  iObhect 10-13   Treasures
/////////////////////////////////////////////////////////////////////////////
const char *CTRXRemastered456::GetLabelForObject ( int tombraider, int levelIndex, int iObject, bool bDouble )
{
    //
    static char szLabel [ MAX_PATH ] = "";
    ZeroMemory ( szLabel, sizeof(szLabel) );

    FourLabels  *pFourTable = NULL;
    TwoLabels   *pTwoTable  = NULL;
    int         index       = -1;
    int         count       = -1;

    switch ( tombraider )
    {
        case 15 :
        {
            levelIndex  = levelIndex + TR1G_START;
            break;
        }
        case 25 :
        {
            levelIndex  = levelIndex + TR2G_START;
            break;
        }
        case 35 :
        {
            levelIndex  = levelIndex + TR3G_START;
            break;
        }
    }

    //
    switch ( tombraider )
    {
        //
        case 1:
        case 10:
        case 15:
        {
            switch ( iObject )
            {
                case 0:
                case 1:
                case 2:
                case 3:
                {
                    pFourTable  = TR1_Object_Label;
                    index       = iObject;
                    count       = sizeof(TR1_Object_Label)/sizeof(FourLabels);
                    break;
                }
                case 4:
                case 5:
                case 6:
                case 7:
                {
                    pFourTable  = TR1_Key_Label;
                    index       = iObject - 4;
                    count       = sizeof(TR1_Key_Label)/sizeof(FourLabels);
                    break;
                }
                case 8:
                case 9:
                {
                    pTwoTable   = TR1_Map_Label;
                    index       = iObject - 8;
                    count       = sizeof(TR1_Map_Label)/sizeof(TwoLabels);
                    break;
                }
            }

            break;
        }

        //
        case 2:
        case 20:
        case 25:
        {
            switch ( iObject )
            {
                case 0:
                case 1:
                case 2:
                case 3:
                {
                    pFourTable  = TR2_Object_Label;
                    index       = iObject;
                    count       = sizeof(TR2_Object_Label)/sizeof(FourLabels);
                    break;
                }
                case 4:
                case 5:
                case 6:
                case 7:
                {
                    pFourTable  = TR2_Key_Label;
                    index       = iObject - 4;
                    count       = sizeof(TR2_Key_Label)/sizeof(FourLabels);
                    break;
                }
                case 8:
                case 9:
                {
                    pTwoTable   = TR3_Map_Label;
                    index       = iObject - 8;
                    count       = sizeof(TR3_Map_Label)/sizeof(TwoLabels);
                    break;
                }
            }

            break;
        }

        //
        case 3:
        case 30:
        case 35:
        {
            switch ( iObject )
            {
                case 0:
                case 1:
                case 2:
                case 3:
                {
                    pFourTable  = TR3_Object_Label;
                    index       = iObject;
                    count       = sizeof(TR3_Object_Label)/sizeof(FourLabels);
                    break;
                }
                case 4:
                case 5:
                case 6:
                case 7:
                {
                    pFourTable  = TR3_Key_Label;
                    index       = iObject - 4;
                    count       = sizeof(TR3_Key_Label)/sizeof(FourLabels);
                    break;
                }
                case 8:
                case 9:
                {
                    pTwoTable   = TR3_Map_Label;
                    index       = iObject - 8;
                    count       = sizeof(TR3_Map_Label)/sizeof(TwoLabels);
                    break;
                }
                case 10:
                case 11:
                case 12:
                case 13:
                {
                    pFourTable  = TR3_Treasure_Label;
                    index       = iObject - 10;
                    count       = sizeof(TR3_Treasure_Label)/sizeof(FourLabels);
                    break;
                }
            }

            break;
        }
    }

    //
    char *pTempText1 = "";
    char *pTempText2 = "";
    if ( index >= 0 && count >= 0 && levelIndex < count )
    {
        if ( pFourTable != NULL )
        {
            switch ( index )
            {
                case 0 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pFourTable [ levelIndex ].label1;
                    }
                    else
                    {
                        pTempText1  = pFourTable [ levelIndex ].label1;
                        pTempText2  = pFourTable [ levelIndex ].label2;
                    }
                    break;
                }
                case 1 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pFourTable [ levelIndex ].label2;
                    }
                    else
                    {
                        pTempText1  = pFourTable [ levelIndex ].label1;
                        pTempText2  = pFourTable [ levelIndex ].label2;
                    }
                    break;
                }
                case 2 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pFourTable [ levelIndex ].label3;
                    }
                    else
                    {
                        pTempText1  = pFourTable [ levelIndex ].label3;
                        pTempText2  = pFourTable [ levelIndex ].label4;
                    }
                    break;
                }
                case 3 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pFourTable [ levelIndex ].label4;
                    }
                    else
                    {
                        pTempText1  = pFourTable [ levelIndex ].label3;
                        pTempText2  = pFourTable [ levelIndex ].label4;
                    }
                    break;
                }
            }
        }
        else if ( pTwoTable != NULL )
        {
            switch ( index )
            {
                case 0 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pTwoTable [ levelIndex ].label1;
                    }
                    else
                    {
                        pTempText1  = pTwoTable [ levelIndex ].label1;
                        pTempText2  = pTwoTable [ levelIndex ].label2;
                    }
                    break;
                }
                case 1 :
                {
                    if ( ! bDouble )
                    {
                        pTempText1  = pTwoTable [ levelIndex ].label2;
                    }
                    else
                    {
                        pTempText1  = pTwoTable [ levelIndex ].label1;
                        pTempText2  = pTwoTable [ levelIndex ].label2;
                    }
                    break;
                }
            }
        }
    }

    //
    if ( ! bDouble )
    {
        if ( strlen ( pTempText1 ) > 0 )
        {
            strcpy_s ( szLabel, sizeof(szLabel), pTempText1 );
        }
    }
    else
    {
        if ( strlen ( pTempText1 ) > 0 )
        {
            strcpy_s ( szLabel, sizeof(szLabel), pTempText1 );
        }

        if ( strlen ( pTempText2 ) > 0 )
        {
            strcat_s ( szLabel, sizeof(szLabel), "/" );
            strcat_s ( szLabel, sizeof(szLabel), pTempText2 );
        }
    }
    return szLabel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
    //
    if ( ! m_bToolTip )
    {
        return FALSE;
    }

    TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMH;

    //
    static char szText [ MAX_PATH ] = "";
    ZeroMemory ( szText, sizeof(szText) );

    //
    POINT           ptAction;
    GetCursorPos ( &ptAction );
    m_ListCtrl.ScreenToClient ( &ptAction );

    //  
    int iHit = m_ListCtrl.HitTest ( ptAction );
    if ( iHit != -1 )
    {
        LVHITTESTINFO hitInfo;
        memset ( &hitInfo, 0, sizeof ( hitInfo ) );
        hitInfo.pt =    ptAction;

        //  iiTem and iSubItem from hitInfo
        int iSub = m_ListCtrl.SubItemHitTest ( &hitInfo );
        if ( iSub != -1 )
        {
            if ( hitInfo.iItem >= 0 && hitInfo.iSubItem >= 0 )
            {
                DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( hitInfo.iItem );

                STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
                int tombraider          = pInfoData->tombraider;
                int level               = pInfoData->level;
                int block               = pInfoData->block;

                //
                if ( hitInfo.iSubItem == COLR_ADDRESS_1 )
                {
                    sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                        "%d %d:%d:%d\r\n", hitInfo.iItem, tombraider, block );
                    void *pAddress = NULL;
                    pAddress = ( void *) CTR8SaveGame::I()->GetBlockAddress ( tombraider, block );
                    sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                " - Address : 0x%08lX\r\n", RelativeAddress ( pAddress ) );

                    //
                    pAddress = ( void *) CTR8SaveGame::I()->GetBlockSecretsAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - A : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = ( void *) CTR8SaveGame::I()->GetBlockSecretsCurrentAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - C : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = ( void *) CTR8SaveGame::I()->GetSecretsCompletedAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - E : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                }
                else if ( hitInfo.iSubItem == COLR_ADDRESS_2 )
                {
                    void *pAddress = NULL;
                    pAddress = ( void *) CTR8SaveGame::I()->GetRealHealthAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - RH : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = CTR8SaveGame::I()->SearchGunEntry ( tombraider, block);
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - G : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                }
                //
                else if ( hitInfo.iSubItem == COLR_DISTANCE )
                {
                    strcpy_s ( szText, sizeof(szText), "Distance Travelled\nThe value may differs from Statistics.\r\n" );
                    DWORD *pDistance = CTR8SaveGame::I()->GetBlockDistanceAddress ( tombraider, block);
                    if ( pDistance )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "0x%08lX\r\n", *pDistance  );
                    }
                }
                else
                {
                    m_ListCtrl.GetItemText ( hitInfo.iItem, hitInfo.iSubItem, szText, sizeof(szText) );
                }

                pText->lpszText = szText;
                return TRUE;
            }
        }

        return FALSE;
    }

    //
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        //
        int levelIndex          = level - 1;

        //
        GetCursorPos ( &ptAction );
        ScreenToClient ( &ptAction );
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedSet()
{
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        if ( CTR8SaveGame::I() )
        {
            SetGUIModified( TRUE );

            switch ( tombraider )
            {
                case 4:
                {
                    int acquired    = CTR8SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR8SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR8SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
                    break;
                }
                case 5:
                {
                    int acquired    = CTR8SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR8SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR8SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
                    break;
                }
                case 6:
                {
                    int acquired    = CTR8SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR8SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR8SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR8SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
                    break;
                }
            }
        }

        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedShow()
{
    if ( m_Line >= 0 && CTR8SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        CTRXSecretsDialog   dlg;
        dlg.m_iTombraider   = tombraider;
        dlg.m_iBlock        = block;
        dlg.m_iLevel        = level;

        dlg.DoModal();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTRXRemastered456::RelativeAddress ( const void *pAddress )
{
    if ( CTR8SaveGame::I() != NULL && CTR8SaveGame::I()->getBufferAddress() != NULL )
    {
        return CTRXTools::RelativeAddress ( pAddress, CTR8SaveGame::I()->getBufferAddress() );
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedKillWillard()
{
    if ( m_Line >= 0 && CTR8SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        if ( tombraider == 1 && level == 15 )
        {
            CTR8SaveGame::I()->KillTR1Boss ( tombraider, block );
        }
        else if ( tombraider == 3 && level == 19 )
        {
            CTR8SaveGame::I()->KillWillard ( tombraider, block );
        }

        SetGUIModified( TRUE );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ShowInfo()
{
    if ( m_Line >= 0 && CTR8SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        CTRXRemasteredInfos dlg;
        dlg.m_iTombraider   = tombraider;
        dlg.m_iBlock        = block;
        dlg.m_iSlot         = -1;
        dlg.m_iLevel        = level;

        dlg.DoModal();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ShowPosition()
{
    if ( m_Line >= 0 && CTR8SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        CTRXPosition    dlg;
        dlg.m_iTombraider   = tombraider;
        dlg.m_iBlock        = block;
        dlg.m_iSlot         = -1;
        dlg.m_iLevel        = level;
        dlg.m_456           = TRUE;

        dlg.DoModal();

        //  SHow Room in case of updated
        ShowRoom();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ShowMap()
{
    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        //
        CTRXMapAreas dlg;
        dlg.SetTombraiderLevelIndex ( tombraider, level - 1 );
        dlg.SetClickable ( FALSE );

        //
        const TR_POSITION *position = CTR8SaveGame::I()->GetPosition(tombraider, block);
        if ( position != NULL )
        {
            int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( tombraider, levelIndex, position->wRoom );
            if ( pArea )
            {
                TR_CUR_POSITION currentPosition;
                ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                currentPosition.x               = position->dwWestToEast;
                currentPosition.y               = position->dwVertical;
                currentPosition.z               = position->dwSouthToNorth;
                currentPosition.orientation     = CTRXTools::ConvertOrientationFromWORD ( position->wOrientation );
                dlg.SetCurrentArea ( pArea, &currentPosition );
            }
        }

        dlg.SetClickable ( FALSE );

        INT_PTR area = dlg.DoModal();

    }
    else
    {
        CTRXAllMaps dlg;
        dlg.DoModal();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::ShowRoom()
{
    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        //
        const TR_POSITION *position = CTR8SaveGame::I(FALSE)->GetPosition(tombraider, block);
        if ( position != NULL )
        {
            int levelNumber     = CTR8SaveGame::I(FALSE)->GetBlockLevelNumber ( tombraider, block );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( tombraider, levelIndex, position->wRoom );
            if ( pArea != NULL )
            {
                TR_CUR_POSITION     currentPosition;
                currentPosition.x               = position->dwWestToEast;
                currentPosition.z               = position->dwSouthToNorth;
                currentPosition.orientation     = position->wOrientation;
                m_Room.SetAreaAndPosition ( pArea, &currentPosition );

                m_Position_Label.SetWindowText( CTR8SaveGame::I()->getIndicatorLabel() );

                //
                RECT frameRect;
                m_Frame_Position.GetClientRect ( &frameRect );

                //
                int width   = pArea->zSectors * TR_SECTOR_SIZE;
                int height  = pArea->xSectors * TR_SECTOR_SIZE;

                //
                double wRatio   = (double) width / (double) ( frameRect.right - frameRect.left );
                double hRatio   = (double) height / (double) ( frameRect.bottom - frameRect.top );
                double ratio    = wRatio;
                if ( ratio < hRatio )
                {
                    ratio = hRatio;
                }

                width   = (int) ( ( double) width / ratio );
                height  = (int) ( ( double) height / ratio );

                //
                //  Compute Rect
                RECT roomRect;
                m_Room.GetWindowRect ( &roomRect );
                ScreenToClient ( &roomRect );
                InvalidateRect ( &roomRect );
                roomRect.bottom     = roomRect.top + height;
                roomRect.right      = roomRect.left + width;
                m_Room.MoveWindow ( &roomRect, TRUE );

                //
                CPoint point;
                point = m_Room.ComputeRoomPoint ( (DWORD) position->dwWestToEast, (DWORD)position->dwSouthToNorth );
                double dfOrientation = CTRXTools::ConvertOrientationFromWORD ( ( WORD ) position->wOrientation );
                m_Room.SetRoomPoint ( point, &dfOrientation );

                //
                InvalidateRect ( &roomRect );

                m_Room.ShowWindow ( SW_NORMAL );

                return;
            }
        }
    }

    m_Room.ShowWindow ( SW_HIDE );

    m_Position_Label.SetWindowText( "" );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::Renumber()
{
    if ( CTR8SaveGame::I(FALSE) != NULL )
    {
        CTR8SaveGame::I(FALSE)->ReNumber ( );
        DisplayList();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DeleteOneGame()
{
    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        int iResponse = MessageBox ( "Are You sure to want to delete this line ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( iResponse == IDYES )
        {
            CTR8SaveGame::I(FALSE)->Delete ( tombraider, block );
            DisplayList();
        }

    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnRclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // Add Handler Here

    if ( pNMItemActivate && pNMItemActivate->iItem >= 0 )
    {
        //
        m_Line      = pNMItemActivate->iItem;

        //
        POINT point = pNMItemActivate->ptAction;
        m_ListCtrl.ClientToScreen ( &point );

        //
        CTRXMenuBase        menu;
        menu.LoadMenu ( IDR_MENU_LIST );
        m_pContextMenu = menu.GetSubMenu ( 0 );
        m_pContextMenu->TrackPopupMenu ( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this );
    }

    //
    *pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistDelete()
{
    DeleteOneGame();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistExport()
{
    //
    static char BASED_CODE szFilter1[] = "Exports|savegame.1.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter2[] = "Exports|savegame.2.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter3[] = "Exports|savegame.3.*.trx|All Files (*.*)|*.*||";

    static char szDefault1 [ MAX_PATH ];
    static char szDefault2 [ MAX_PATH ];
    static char szDefault3 [ MAX_PATH ];

    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        const char *pFilter     = szFilter1;
        const char *pDefault    = szDefault1;

        sprintf_s ( szDefault1, "savegame.1.%02d.trx", CTR8SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );
        sprintf_s ( szDefault2, "savegame.2.%02d.trx", CTR8SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );
        sprintf_s ( szDefault3, "savegame.3.%02d.trx", CTR8SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );

        switch ( tombraider )
        {
            case 4 :
            {
                pFilter     = szFilter1;
                pDefault    = szDefault1;
                break;
            }
            case 5 :
            {
                pFilter     = szFilter2;
                pDefault    = szDefault2;
                break;
            }
            case 6 :
            {
                pFilter     = szFilter3;
                pDefault    = szDefault3;
                break;
            }
            default :
            {
                return;
                break;
            }
        }

        CFileDialog dlgFileDialog ( FALSE, NULL, pDefault, NULL, pFilter );
        // dlgFileDialog.m_ofn.lpstrInitialDir = szDirectory;

        //      Starts a dialog box.
        if ( dlgFileDialog.DoModal () == IDOK)
        {
            BOOL bResult = CTR8SaveGame::I()->Export ( dlgFileDialog.GetPathName(), tombraider, block );
            if ( ! bResult )
            {
                MessageBox ( "Export fails", MessageTitle, MB_OK|MB_ICONERROR );
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistImport()
{
    //
    static char BASED_CODE szFilter1[] = "Exports|savegame.1.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter2[] = "Exports|savegame.2.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter3[] = "Exports|savegame.3.*.trx|All Files (*.*)|*.*||";

    const char *pDefault1   = "savegame.1.*.trx";
    const char *pDefault2   = "savegame.2.*.trx";
    const char *pDefault3   = "savegame.3.*.trx";

    const char *pFilter     = szFilter1;
    const char *pDefault    = pDefault1;

    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        switch ( tombraider )
        {
            case 4 :
            {
                pFilter     = szFilter1;
                pDefault    = pDefault1;
                break;
            }
            case 5 :
            {
                pFilter     = szFilter2;
                pDefault    = pDefault2;
                break;
            }
            case 6 :
            {
                pFilter     = szFilter3;
                pDefault    = pDefault3;
                break;
            }
            default :
            {
                return;
                break;
            }
        }

        CFileDialog dlgFileDialog ( TRUE, NULL, pDefault, NULL, pFilter );
        // dlgFileDialog.m_ofn.lpstrInitialDir = szDirectory;

        //      Starts a dialog box.
        if ( dlgFileDialog.DoModal () == IDOK)
        {
            BOOL bResult = CTR8SaveGame::I()->Import ( dlgFileDialog.GetPathName(), tombraider, block );
            if ( bResult )
            {
                DisplayList();
            }
            else
            {
                MessageBox ( "Import fails", MessageTitle, MB_OK|MB_ICONERROR );
            }

        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistInfos()
{
    ShowInfo();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistPosition()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistRenumber()
{
    Renumber();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistresettostart()
{
    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        CTR8SaveGame::I()->SetLikeGame ( tombraider, block );

        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnMenulistresettoplus()
{
    if ( m_Line >= 0 && CTR8SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;

        CTR8SaveGame::I(FALSE)->SetLikePlus ( tombraider, block );

        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnChangeEdit()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedMagnum()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedUzi()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedRiotgun()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedM16()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedGrenade()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedHarpoon()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedGun()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedRocket()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnOK()
{
    //
    UpdateBuffer();

    if ( CTR8SaveGame::I(FALSE) != NULL && ( IsGUIModified () || CTR8SaveGame::IsBufferModified() ) )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR456 ( "Do You want to save your changes ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            WriteFile();
        }
        else
        {
            CTR8SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    CTRXPropertyPage456::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered456::OnApply()
{
    //
    UpdateBuffer();

    if ( CTR8SaveGame::I(FALSE) != NULL && ( IsGUIModified() || CTR8SaveGame::IsBufferModified() ) )
    {
        WriteFile();
    }

    return CTRXPropertyPage456::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnCancel()
{
    UpdateBuffer();

    if ( CTR8SaveGame::I( FALSE) != NULL && ( IsGUIModified () || CTR8SaveGame::IsBufferModified() ) )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR456 ( "Do You want to save your changes ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            WriteFile();
        }
        else
        {
            CTR8SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    CTRXPropertyPage456::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedTr1Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnClickedTr2Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedTr3Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedGamePlus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedReset()
{
    SetGUIModified( TRUE );

    for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        ResetToPlus ( i );
    }

    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedResetgame()
{
    SetGUIModified( TRUE );

    for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        ResetToGame ( i );
    }

    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedJournal()
{
    if ( CTR8SaveGame::I(FALSE) != NULL )
    {
        if ( m_Update.GetCheck() )
        {
            UpdateBuffer();
        }

        //
        CTRXDifferences     dlg;
        dlg.m_bRemastered456   = TRUE;
        dlg.DoModal();

        DisplayOne ( m_Line );

        SetGUIModified ( CTR8SaveGame::IsBufferModified() );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnSelendokCombo()
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedPosition()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedShowMap()
{
    ShowMap();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SingleClick()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::SetThemeChanged ( bool bDarkTheme )
{
    if ( m_bInitDone )
    {
        //
        if ( CTRXColors::m_iDarkTheme != 0 )
        {
            m_Load.SetIconResource ( IDI_LOAD_BLACK );
            m_Write.SetIconResource ( IDI_SAVE_BLACK );
            m_Refresh.SetIconResource ( IDI_REFRESH_BLACK );
        }
        else
        {
            m_Load.SetIconResource ( IDI_LOAD_WHITE );
            m_Write.SetIconResource ( IDI_SAVE_WHITE );
            m_Refresh.SetIconResource ( IDI_REFRESH_WHITE );
        }
    }

    CTRXPropertyPage456::SetThemeChanged ( bDarkTheme );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::DoDropFiles(const char *pFilename)
{
    //
    m_Filename.SetWindowText ( pFilename );

    DisplayList ( pFilename );

    //
    BOOL bAdded = AddLocation ( LocationPathname, pFilename );
    bAdded      = AddComboString ( &m_Combo, pFilename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedShell()
{
    static char     szDirectory [ MAX_PATH ];
    m_Filename.GetWindowText ( szDirectory, sizeof ( szDirectory ) - 1 );
    theApp.RemoveFilename ( szDirectory );
    HINSTANCE hInst = ShellExecute ( NULL, "open", szDirectory, "", "", SW_SHOWDEFAULT );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered456::OnBnClickedItems()
{
    //
    //
    //
    //  To SHow Item Again
    int currentTombraider   = -1;
    int currentLevel        = -1;
    int currentBlock        = -1;
    int currentSlot         = -1;
    if ( m_Line >= 0 && CTR8SaveGame::I() != NULL )
    {
        //
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        currentTombraider           = pInfoData->tombraider;
        currentLevel                = pInfoData->level;
        currentBlock                = pInfoData->block;

        if ( currentTombraider >= 4 && currentTombraider <= 5 && currentBlock >= 0 && currentLevel >= 1 )
        {
            CTRXItemsTRR456 dlg;
            dlg.m_iTombraider       = currentTombraider;
            dlg.m_iBlock            = currentBlock;
            dlg.m_iLevel            = currentLevel;
            dlg.DoModal();
        }
    }

}
