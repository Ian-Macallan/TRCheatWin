// TRXRemastered.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"

#include "TR9SaveGame.h"


#include "TRXRemastered.h"
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
#include "TRXMessageBox.h"

#include "TRXGunPage.h"         // Added by ClassView
#include "TRXInfoPage.h"        // Added by ClassView
#include "TRXItems.h"           // Added by ClassView
#include "TRXItemsTR4.h"        // Added by ClassView
#include "TRXAmmosPage.h"       // Added by ClassView
#include "TRXEquipmentPage.h"   // Added by ClassView
#include "TRXPropertySheet.h"

#include "TRXColors.h"
#include "TRXGlobal.h"

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
#define MAX_PAK             200
#define MAX_AMMOS           6000
#define MAX_AMMOS_MAX       (6*6000)

//
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static  STRUCTLOCATION  LocationPathname [ LEN_LOCATION ];

//
static const    char *MessageTitle = "Tombraider 123 Remastered";

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static  int             iSortAscending      = 1;
static  int             iSortColumn         = -1;
static  const   int     maxData             = 2 * NB_TR_BLOCKS * NB_OF_SLOTS;
typedef struct  structData
{
    int     tombraider;
    int     level;
    int     save;
    int     block;
    int     slot;
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

#define SET_BLOCK_CHECK(wnd,msk)                            \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pBlock->gunmask&##msk,TRUE );            \
    }                                                       \
    else                                                    \
    {                                                       \
        SetValue(##wnd,0,FALSE);                            \
    }

#define SET_CHECK_DISABLE(wnd,msk)                          \
    if ( pBlock != NULL )                                   \
    {                                                       \
        SetValue(##wnd,pBlock->gunmask&##msk,FALSE,TRUE);       \
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
IMPLEMENT_DYNAMIC(CTRXRemastered, CTRXPropertyPage123)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemastered::CTRXRemastered() : CTRXPropertyPage123(CTRXRemastered::IDD)
{
    CTR9SaveGame::I();

    m_pListCtrl         = NULL;
    m_Line              = -1;

    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        ZeroMemory ( LocationPathname [ i ].szPathname, sizeof(LocationPathname [ i ].szPathname) );
    }

    //
    LoadLocation ( LocationPathname, PROFILE_TRX_LOC_COUNT, PROFILE_TRX_LOCATION );

    //
    m_bRoomCreated  = false;

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemastered::~CTRXRemastered()
{
    CTR9SaveGame *pGame = CTR9SaveGame::I(FALSE);
    if ( pGame != NULL )
    {
        delete pGame;
    }

    SaveLocation ( LocationPathname, PROFILE_TRX_LOC_COUNT, PROFILE_TRX_LOCATION );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnHelp()
{
    int tombraider          = -1;
    int level               = -1;
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        tombraider              = pInfoData->tombraider;
        level                   = pInfoData->level;
    }


    //
    CTRXHelpDialog dlg;

    //
    dlg.m_Tombraider    = tombraider;
    dlg.m_LevelNumber   = level;

    //
    if ( CTR9SaveGame::I(NULL) != NULL )
    {
        strcpy_s ( dlg.m_szTitle, sizeof(dlg.m_szTitle), CTR9SaveGame::I(NULL)->GetLevelName(tombraider,level) );
    }

    //
    strcpy_s ( dlg.m_szLevelName, sizeof(dlg.m_szLevelName), dlg.m_szTitle );

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            dlg.m_ID_Resource = IDR_TR1CHEATS;
            break;
        }
        case GAME_TRR2:
        {
            dlg.m_ID_Resource = IDR_TR2CHEATS;
            break;
        }
        case GAME_TRR3:
        {
            dlg.m_ID_Resource = IDR_TR3CHEATS;
            break;
        }
    }
    dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage123::DoDataExchange(pDX);
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
    DDX_Control(pDX, IDC_OBJ_1, m_Object_1);
    DDX_Control(pDX, IDC_OBJ_2, m_Object_2);
    DDX_Control(pDX, IDC_OBJ_3, m_Object_3);
    DDX_Control(pDX, IDC_OBJ_4, m_Object_4);
    DDX_Control(pDX, IDC_KEY_1, m_Key_1);
    DDX_Control(pDX, IDC_KEY_2, m_Key_2);
    DDX_Control(pDX, IDC_KEY_3, m_Key_3);
    DDX_Control(pDX, IDC_KEY_4, m_Key_4);
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
    DDX_Control(pDX, IDC_ZERO, m_Zero);
    DDX_Control(pDX, IDC_ONE, m_One);
    DDX_Control(pDX, IDC_FOUR, m_Four);
    DDX_Control(pDX, IDC_REAL_HEALTH, m_Real_Health);
    DDX_Control(pDX, IDC_STRONG, m_Invincible);
    DDX_Control(pDX, IDC_MAP_1, m_Map_1);
    DDX_Control(pDX, IDC_MAP_2, m_Map_2);
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
    DDX_Control(pDX, IDC_TRE_1, m_Tre_1);
    DDX_Control(pDX, IDC_TRE_2, m_Tre_2);
    DDX_Control(pDX, IDC_TRE_3, m_Tre_3);
    DDX_Control(pDX, IDC_TRE_4, m_Tre_4);
    DDX_Control(pDX, IDC_UZI_CLIP, m_Uzi_Clip);
    DDX_Control(pDX, IDC_M16_CLIP, m_M16_Clip);
    DDX_Control(pDX, IDC_GRENADE_CLIP, m_Grenade_Clip);
    DDX_Control(pDX, IDC_RECURSE, m_Recurse);
    DDX_Control(pDX, IDC_POSITION, m_Position);
    DDX_Control(pDX, IDC_SHOW_MAP, m_Show_Map);
    DDX_Control(pDX, IDC_FRAME_POSITION, m_Frame_Position);
    DDX_Control(pDX, IDC_SHELL, m_Shell);
    DDX_Control(pDX, IDC_POS_LABEL, m_Position_Label);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXRemastered, CTRXPropertyPage123)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_LOAD, &CTRXRemastered::OnBnClickedLoad)
    ON_BN_CLICKED(IDC_WRITE, &CTRXRemastered::OnBnClickedWrite)
    ON_NOTIFY(NM_CLICK, IDC_LISTCTRL, OnClickListctrl)
    ON_BN_CLICKED(IDC_REFRESH, &CTRXRemastered::OnBnClickedRefresh)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, &CTRXRemastered::OnItemchangedListctrl)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CTRXRemastered::OnItemclickListctrl)
    ON_BN_CLICKED(IDC_MAX, &CTRXRemastered::OnBnClickedMax)
    ON_BN_CLICKED(IDC_MAX_ONE, &CTRXRemastered::OnBnClickedMaxOne)
    ON_BN_CLICKED(IDC_AR_FULL, &CTRXRemastered::OnBnClickedArFull)
    ON_BN_CLICKED(IDC_AR_INFINITE, &CTRXRemastered::OnBnClickedArInfinite)
    ON_BN_CLICKED(IDC_LIST_ALL, &CTRXRemastered::OnBnClickedListAll)
    ON_BN_CLICKED(IDC_UPDATE, &CTRXRemastered::OnBnClickedUpdate)
    ON_CBN_SELCHANGE(IDC_COMBO, &CTRXRemastered::OnSelchangeCombo)
    ON_BN_CLICKED(IDC_BURNING, &CTRXRemastered::OnBnClickedBurning)
    ON_BN_CLICKED(IDC_REMOVE_LOC, &CTRXRemastered::OnBnClickedRemoveLoc)
    ON_BN_CLICKED(IDC_ZERO, &CTRXRemastered::OnBnClickedZero)
    ON_BN_CLICKED(IDC_ONE, &CTRXRemastered::OnBnClickedOne)
    ON_BN_CLICKED(IDC_FOUR, &CTRXRemastered::OnBnClickedFour)
    ON_BN_CLICKED(IDC_STRONG, &CTRXRemastered::OnBnClickedStrong)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
    ON_BN_CLICKED(IDC_SET, &CTRXRemastered::OnBnClickedSet)
    ON_BN_CLICKED(IDC_SHOW, &CTRXRemastered::OnBnClickedShow)
    ON_BN_CLICKED(IDC_KILL_WILLARD, &CTRXRemastered::OnBnClickedKillWillard)
    ON_NOTIFY(NM_RCLICK, IDC_LISTCTRL, &CTRXRemastered::OnRclickListctrl)
    ON_COMMAND(ID_MENULIST_DELETE, &CTRXRemastered::OnMenulistDelete)
    ON_COMMAND(ID_MENULIST_EXPORT, &CTRXRemastered::OnMenulistExport)
    ON_COMMAND(ID_MENULIST_IMPORT, &CTRXRemastered::OnMenulistImport)
    ON_COMMAND(ID_MENULIST_INFOS, &CTRXRemastered::OnMenulistInfos)
    ON_COMMAND(ID_MENULIST_POSITION, &CTRXRemastered::OnMenulistPosition)
    ON_COMMAND(ID_MENULIST_RESETTO, &CTRXRemastered::OnMenulistresettoplus)
    ON_COMMAND(ID_MENULIST_RESETTOSTART, &CTRXRemastered::OnMenulistresettostart)
    ON_COMMAND(ID_MENULIST_RENUMBER, &CTRXRemastered::OnMenulistRenumber)

    ON_EN_CHANGE(IDC_AMAGNUM, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AUZI, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_ARIOTGUN, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AM16, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AGRENADE, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AROCKET, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AHARPOON, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_1, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_2, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_3, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_OBJ_4, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_1, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_2, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_3, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_KEY_4, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_MAP_1, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_MAP_2, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_1, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_2, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_3, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_TRE_4, &CTRXRemastered::OnChangeEdit)

    ON_EN_CHANGE(IDC_SAVE, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_HEALTH, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_LARGEPAK, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_SMALLPAK, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_FLARE, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_AIR_REM, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_STATE, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_REAL_HEALTH, &CTRXRemastered::OnChangeEdit)

    ON_EN_CHANGE(IDC_PICKUP, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_CRYSTAL, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_KILL, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_COL_CRYSTAL, &CTRXRemastered::OnChangeEdit)
    ON_EN_CHANGE(IDC_SECRETS, &CTRXRemastered::OnChangeEdit)

    ON_BN_CLICKED(IDC_MAGNUM, &CTRXRemastered::OnClickedMagnum)
    ON_BN_CLICKED(IDC_UZI, &CTRXRemastered::OnClickedUzi)
    ON_BN_CLICKED(IDC_RIOTGUN, &CTRXRemastered::OnClickedRiotgun)
    ON_BN_CLICKED(IDC_M16, &CTRXRemastered::OnClickedM16)
    ON_BN_CLICKED(IDC_GRENADE, &CTRXRemastered::OnClickedGrenade)
    ON_BN_CLICKED(IDC_HARPOON, &CTRXRemastered::OnClickedHarpoon)
    ON_BN_CLICKED(IDC_GUN, &CTRXRemastered::OnClickedGun)
    ON_BN_CLICKED(IDC_ROCKET, &CTRXRemastered::OnClickedRocket)
    ON_BN_CLICKED(IDC_TR1_PLUS, &CTRXRemastered::OnClickedTr1Plus)
    ON_BN_CLICKED(IDC_TR2_PLUS, &CTRXRemastered::OnClickedTr2Plus)
    ON_BN_CLICKED(IDC_TR3_PLUS, &CTRXRemastered::OnBnClickedTr3Plus)
    ON_BN_CLICKED(IDC_GAME_PLUS, &CTRXRemastered::OnBnClickedGamePlus)
    ON_BN_CLICKED(IDC_RESET, &CTRXRemastered::OnBnClickedReset)
    ON_BN_CLICKED(IDC_RESETGAME, &CTRXRemastered::OnBnClickedResetgame)
    ON_BN_CLICKED(IDC_JOURNAL, &CTRXRemastered::OnBnClickedJournal)
    ON_WM_CTLCOLOR()
    ON_CBN_SELENDOK(IDC_COMBO, &CTRXRemastered::OnSelendokCombo)
    ON_BN_CLICKED(IDC_POSITION, &CTRXRemastered::OnBnClickedPosition)
    ON_BN_CLICKED(IDC_SHOW_MAP, &CTRXRemastered::OnBnClickedShowMap)
    ON_BN_CLICKED(IDC_RECURSE, &CTRXRemastered::OnBnClickedRecurse)
    ON_BN_CLICKED(IDC_SHELL, &CTRXRemastered::OnBnClickedShell)
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
void CTRXRemastered::OnBnClickedLoad()
{
    //
    char    szDirectory [ MAX_PATH ];
    char    szFilename [ MAX_PATH ];

    //
    if ( m_Update.GetCheck() )
    {
        BOOL bRecurse = m_Recurse.GetCheck();
        UpdateBuffer(bRecurse);
    }

    //
    if ( CTR9SaveGame::IsBufferModified() )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR123 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            ZeroMemory ( szFilename, sizeof ( szFilename ) );
            m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
            CTR9SaveGame::WriteFile ( szFilename );
        }
        else if ( response == IDCANCEL )
        {
            return;
        }
        else
        {
            CTR9SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    //
    static char BASED_CODE szFilter[] = "Savegame|savegame.dat|Savegames|savegame.*|All Files (*.*)|*.*||";

    ZeroMemory ( szDirectory, sizeof(szDirectory) );
    ZeroMemory ( szFilename, sizeof(szFilename) );

    m_Write.EnableWindow ( FALSE );
    m_Max.EnableWindow ( FALSE );

    CString lastRead  = theApp.GetProfileString( PROFILE_SETTING, PROFILE_TRX_LAST_OPENED, szFilename );
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
void CTRXRemastered::OnBnClickedWrite()
{
    //
    if ( m_Update.GetCheck() )
    {
        BOOL bRecurse = m_Recurse.GetCheck();
        UpdateBuffer(bRecurse);
    }

    WriteFile();

    DisplayOne ( m_Line );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::WriteFile()
{
    theApp.BeginWaitCursor();

    //
    if ( m_Update.GetCheck() )
    {
        BOOL bRecurse = m_Recurse.GetCheck();
        UpdateBuffer(bRecurse);
    }

    char    szFilename [ MAX_PATH ];
    ZeroMemory ( szFilename, sizeof ( szFilename ) );
    m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );

    BOOL bWritten = CTR9SaveGame::WriteFile ( szFilename );
    if ( bWritten )
    {
        m_Status.SetWindowText ( "File successfully written" );
    }
    else
    {
        CTRXMessageBox::MessageBox ( "Failed : File has not been written", MessageTitle, MB_OK|MB_ICONERROR );
        m_Status.SetWindowText ( "Failed : File has not been written." );
    }

    theApp.EndWaitCursor();

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered::OnInitDialog()
{
    CTRXPropertyPage123::OnInitDialog();

    //
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_ListCtrl, LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool( &m_Object_1, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Object_2, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Object_3, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Object_4, LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool( &m_Key_1, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Key_2, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Key_3, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Key_4, LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool( &m_Map_1, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Map_2, LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool( &m_Tre_1, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Tre_2, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Tre_3, LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool( &m_Tre_4, LPSTR_TEXTCALLBACK );

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
        m_ToolTip.AddTool( &m_Recurse, "Update all structures pointing to the same address");

        //m_ToolTip.SetDelayTime(TTDT_AUTOPOP, 3000);
        //m_ToolTip.SetDelayTime(TTDT_RESHOW, 1000);
        m_ToolTip.Activate(TRUE);
    }

    //
    int RecurseWrite = theApp.GetProfileInt ( PROFILE_SETTING, PROFILE_RECURSE_WRITE, 1 );
    m_Recurse.SetCheck ( RecurseWrite );

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
BOOL CTRXRemastered::OnSetActive()
{
    //
    BOOL bModified = IsGUIModified();
    if ( CTR9SaveGame::I() != NULL )
    {
        bModified = CTR9SaveGame::IsBufferModified();
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
        strcat_s ( szDefault, sizeof(szDefault), "\\AppData\\Roaming\\TRX\\" );
        strcat_s ( szDefault, sizeof(szDefault), CTRXGlobal::m_szTRXNumber );
        strcat_s ( szDefault, sizeof(szDefault), "\\savegame.dat" );
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
            lastRead = theApp.GetProfileString( PROFILE_SETTING, PROFILE_TRX_LAST_OPENED, szFilename );
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
    m_Status.SetWindowText ( "Tombraider Remastered 1, Unfinished Busines, 2, Golden Mask, 3 and Lost Artifact" );

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
    return CTRXPropertyPage123::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::SetComboSelection ( const char *pFilename )
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
void CTRXRemastered::SetComboSelection ( )
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
BOOL CTRXRemastered::OnKillActive()
{
    //
    BOOL bRecurse = m_Recurse.GetCheck();
    UpdateBuffer(bRecurse);

    BOOL bModified = IsGUIModified();

    return CTRXPropertyPage123::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::SetValue ( CButton &button, int check, BOOL enable, BOOL bShowAnyWay )
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
void CTRXRemastered::SetValue ( CEdit &edit, int value, BOOL enable, BOOL bShowAnyWay )
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
void CTRXRemastered::SetBlockObjectOnDisplay(int tombraider, int block, int value)
{
    //  Value -1 mean the real value
    if ( value == -1 )
    {
        SetValue ( m_Object_1, CTR9SaveGame::I()->GetBlockObject ( tombraider, block, 0 ), TRUE );
        SetValue ( m_Object_2, CTR9SaveGame::I()->GetBlockObject ( tombraider, block, 1 ), TRUE );
        SetValue ( m_Object_3, CTR9SaveGame::I()->GetBlockObject ( tombraider, block, 2 ), TRUE );
        SetValue ( m_Object_4, CTR9SaveGame::I()->GetBlockObject ( tombraider, block, 3 ), TRUE );

        SetValue ( m_Key_1, CTR9SaveGame::I()->GetKey ( tombraider, block, 0 ), TRUE );
        SetValue ( m_Key_2, CTR9SaveGame::I()->GetKey ( tombraider, block, 1 ), TRUE );
        SetValue ( m_Key_3, CTR9SaveGame::I()->GetKey ( tombraider, block, 2 ), TRUE );
        SetValue ( m_Key_4, CTR9SaveGame::I()->GetKey ( tombraider, block, 3 ), TRUE );

        SetValue ( m_Map_1, CTR9SaveGame::I()->GetMap ( tombraider, block, 0 ), TRUE );
        SetValue ( m_Map_2, CTR9SaveGame::I()->GetMap ( tombraider, block, 1 ), TRUE );
    }
    //  -2 Mean Incremental
    else if ( value == -2 )
    {
        SetValue ( m_Object_1, 2, TRUE );
        SetValue ( m_Object_2, 3, TRUE );
        SetValue ( m_Object_3, 4, TRUE );
        SetValue ( m_Object_4, 5, TRUE );

        SetValue ( m_Key_1, 6, TRUE );
        SetValue ( m_Key_2, 7, TRUE );
        SetValue ( m_Key_3, 8, TRUE );
        SetValue ( m_Key_4, 9, TRUE );

        SetValue ( m_Map_1, 10, TRUE );
        SetValue ( m_Map_2, 11, TRUE );
    }
    else
    {
        SetValue ( m_Object_1, value, TRUE );
        SetValue ( m_Object_2, value, TRUE );
        SetValue ( m_Object_3, value, TRUE );
        SetValue ( m_Object_4, value, TRUE );

        SetValue ( m_Key_1, value, TRUE );
        SetValue ( m_Key_2, value, TRUE );
        SetValue ( m_Key_3, value, TRUE );
        SetValue ( m_Key_4, value, TRUE );

        SetValue ( m_Map_1, value, TRUE );
        SetValue ( m_Map_2, value, TRUE );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::Enable ( int tombraider, int level )
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
void CTRXRemastered::DisplayOne ( int line )
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
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        //
        const char *pLevelName = CTR9SaveGame::GetLevelName ( tombraider, CTR9SaveGame::I()->GetBlockLevelNumber (  tombraider, block ) );
        m_Status.SetWindowText ( pLevelName );

        //
        //  Display Label
        const char *pLabel  =   "";
        CWnd *pWnd          = NULL;
        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_OBJ_1 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 0, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 1, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "Objects" );
            }
        }

        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_OBJ_3 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 2, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 3, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "" );
            }
        }

        //
        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_KEY_1 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 4, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 5, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "Key" );
            }
        }

        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_KEY_3 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 6, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 7, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "" );
            }
        }

        //
        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_MAP_1 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 8, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 9, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "Map" );
            }
        }

        //
        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_TRE_1 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 10, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 11, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "Treasure" );
            }
        }

        pWnd                = dynamic_cast<CWnd *>(GetDlgItem ( IDC_LBL_TRE_3 ));
        pLabel = GetLabelForObject ( tombraider, level - 1, 12, true );
        if ( strlen(pLabel) > 0 )
        {
            pWnd->SetWindowText ( pLabel );
        }
        else
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, 13, true );
            if ( strlen(pLabel) > 0 )
            {
                pWnd->SetWindowText ( pLabel );
            }
            else
            {
                pWnd->SetWindowText ( "" );
            }
        }

        //
        CWnd *WndArray [] =
        {
            &m_Object_1, &m_Object_2, &m_Object_3, &m_Object_4,
            &m_Key_1, &m_Key_2, &m_Key_3, &m_Key_4,
            &m_Map_1, &m_Map_2,
            &m_Tre_1, &m_Tre_2, &m_Tre_3, &m_Tre_4,
        };

        for ( int iX = 0; iX < sizeof(WndArray)/sizeof(CWnd *); iX++ )
        {
            pLabel = GetLabelForObject ( tombraider, level - 1, iX );
            if ( pLabel != NULL && strlen (pLabel) > 0 )
            {
                WndArray[iX]->SetFont ( m_pBoldFont );
            }
            else
            {
                WndArray[iX]->SetFont ( m_pItalicFont );
            }
        }

        //
        void *pBlockEntry   = NULL;
        void *pGunEntry     = NULL;
        if ( slot >= 0 )
        {
            pBlockEntry     = CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
            pGunEntry       = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );
        }
        else
        {
            pBlockEntry     = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
            pGunEntry       = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block);
        }

        //
        m_TR1_Plus.SetCheck ( CTR9SaveGame::I()->GetTRPlus ( GAME_TRR1 ) );
        m_TR2_Plus.SetCheck ( CTR9SaveGame::I()->GetTRPlus ( GAME_TRR2 ) );
        m_TR3_Plus.SetCheck ( CTR9SaveGame::I()->GetTRPlus ( GAME_TRR3 ) );

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
            sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetSaveNumber ( tombraider, block ) );
            m_Save_No.SetWindowTextA ( szText );
            m_Save_No.EnableWindow ( TRUE );

            //
            ShowRoom();

            //
            switch ( tombraider )
            {
                case GAME_TRR1:
                {
                    TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlockEntry;
                    GUN_TR1 *pGun       = ( GUN_TR1 *) pGunEntry;

                    SET_BLOCK_VALUE(m_AGuns,guns);
                    SET_GUN_VALUE(m_XGuns,guns);

                    sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetRealHealth ( tombraider, block ) );
                    m_Real_Health.SetWindowTextA (szText );
                    if ( CTR9SaveGame::I()->GetRealHealthAddress ( tombraider, block ) == NULL )
                    {
                        m_Real_Health.EnableWindow ( FALSE );
                        m_Invincible.EnableWindow ( FALSE );
                    }
                    else
                    {
                        m_Real_Health.EnableWindow ( TRUE );
                        m_Invincible.EnableWindow ( TRUE );
                    }

                    SET_BLOCK_VALUE(m_LargePak,largeMedipak);
                    SET_BLOCK_VALUE(m_SmallPak,smallMedipak);
                    SET_BLOCK_VALUE(m_Flare,scion);
                    SET_BLOCK_HIDE(m_Col_Crystal);

                    SET_BLOCK_VALUE(m_AMagnum,magnum);
                    SET_GUN_VALUE(m_XMagnum,magnum);

                    SET_BLOCK_VALUE(m_AUzi,uzi);
                    SET_GUN_VALUE(m_XUzi,uzi);
                    SET_BLOCK_UNITS(m_Uzi_Clip,uzi,UZI_CLIP);

                    SET_BLOCK_VALUE(m_ARiotgun,riotgun/6);
                    SET_GUN_VALUE(m_XRiotgun,riotgun/6);

                    SET_BLOCK_HIDE(m_AGrenade);
                    SET_GUN_HIDE(m_XGrenade);
                    SET_GUN_HIDE(m_Grenade_Clip);

                    SET_BLOCK_HIDE(m_AHarpoon);
                    SET_GUN_HIDE(m_XHarpoon);

                    SET_BLOCK_HIDE(m_AM16);
                    SET_GUN_HIDE(m_XM16);
                    SET_BLOCK_HIDE(m_M16_Clip);

                    SET_BLOCK_HIDE(m_ARocket);
                    SET_GUN_HIDE(m_XRocket);

                    //
                    SET_BLOCK_CHECK ( m_Guns, TRR1_MASK_GUN );

                    SET_BLOCK_CHECK ( m_Magnum, TRR1_MASK_MAGNUM );
                    SET_BLOCK_CHECK ( m_Uzi, TRR1_MASK_UZI );
                    SET_BLOCK_CHECK ( m_Riotgun, TRR1_MASK_RIOT );

                    SET_BLOCK_HIDE ( m_M16 );
                    SET_BLOCK_HIDE ( m_Grenade );
                    SET_BLOCK_HIDE ( m_Harpoon );
                    SET_BLOCK_HIDE ( m_Rocket );

                    m_Zero.EnableWindow ( TRUE );
                    m_One.EnableWindow ( TRUE );
                    m_Four.EnableWindow ( TRUE );

                    SetBlockObjectOnDisplay ( tombraider, block, -1 );

                    //
                    SET_BLOCK_HIDE ( m_Tre_1 );
                    SET_BLOCK_HIDE ( m_Tre_2 );
                    SET_BLOCK_HIDE ( m_Tre_3 );
                    SET_BLOCK_HIDE ( m_Tre_4 );

                    m_Tre_1.EnableWindow ( FALSE );
                    m_Tre_2.EnableWindow ( FALSE );
                    m_Tre_3.EnableWindow ( FALSE );
                    m_Tre_4.EnableWindow ( FALSE );

                    //
                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                    m_Pickup.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockCrystal( tombraider, block ) );
                    m_Crystal.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockKills( tombraider, block ) );
                    m_Kill.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetPickupsForLevel ( tombraider, level - 1 ) );
                    m_Pickup_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetCrystalsForLevel ( tombraider, level - 1 ) );
                    m_Crystal_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetKillsForLevel ( tombraider, level - 1 ) );
                    m_Kill_Max.SetWindowText ( szText);

                    int acqsuired = CTR9SaveGame::I()->GetBlockSecretsAcquired( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", acqsuired );
                    m_Secrets_A.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetSecretsMaximum( tombraider, block ) );
                    m_Secrets_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockSecretsCurrent( tombraider, block ) );
                    m_Secrets_C.SetWindowText ( szText);

                    int completed =  CTR9SaveGame::I()->GetSecretsCompleted( tombraider, block );
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

                    m_Kill_Willard.EnableWindow ( CTR9SaveGame::I()->IsKillEnabled ( tombraider, block ) );

                    if ( pGun != NULL )
                    {
                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetAir ( tombraider, block ) );
                        m_Air.SetWindowText ( szText );
                        m_Air.EnableWindow ( TRUE );

                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetState ( tombraider, block ) );
                        m_State.SetWindowText ( szText );
                        m_State.EnableWindow ( TRUE );
                        m_Burning.EnableWindow ( TRUE );
                        m_Burning.SetCheck ( CTR9SaveGame::I()->GetState ( tombraider, block ) & STATE_123_BURNING );
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
                    m_Game_Plus.SetCheck ( CTR9SaveGame::I()->IsGamePlus(tombraider, block) );
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
                case GAME_TRR2:
                {
                    TABLE_TR2 *pBlock = ( TABLE_TR2 *) pBlockEntry;
                    GUN_TR2 *pGun = ( GUN_TR2 *) pGunEntry;

                    SET_BLOCK_VALUE(m_AGuns,guns);
                    SET_GUN_VALUE(m_XGuns,guns);

                    sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetRealHealth( tombraider, block ) );
                    m_Real_Health.SetWindowTextA (szText );
                    if ( CTR9SaveGame::I()->GetRealHealthAddress ( tombraider, block ) == NULL )
                    {
                        m_Real_Health.EnableWindow ( FALSE );
                        m_Invincible.EnableWindow ( FALSE );
                    }
                    else
                    {
                        m_Real_Health.EnableWindow ( TRUE );
                        m_Invincible.EnableWindow ( TRUE );
                    }

                    SET_BLOCK_VALUE(m_LargePak,largeMedipak);
                    SET_BLOCK_VALUE(m_SmallPak,smallMedipak);
                    SET_BLOCK_VALUE(m_Flare,flare);
                    SET_BLOCK_HIDE(m_Col_Crystal);

                    SET_BLOCK_VALUE(m_AMagnum,magnum);
                    SET_GUN_VALUE(m_XMagnum,magnum);

                    SET_BLOCK_VALUE(m_AUzi,uzi);
                    SET_GUN_VALUE(m_XUzi,uzi);
                    SET_BLOCK_UNITS(m_Uzi_Clip,uzi,UZI_CLIP);

                    SET_BLOCK_VALUE(m_ARiotgun,riotgun/6);
                    SET_GUN_VALUE(m_XRiotgun,riotgun/6);

                    SET_BLOCK_VALUE(m_AGrenade,grenade);
                    SET_GUN_VALUE(m_XGrenade,grenade);
                    SET_BLOCK_UNITS(m_Grenade_Clip,grenade,GRENADE_CLIP);

                    SET_BLOCK_VALUE(m_AHarpoon,harpoon);
                    SET_GUN_VALUE(m_XHarpoon,harpoon);

                    SET_BLOCK_VALUE(m_AM16,m16);
                    SET_GUN_VALUE(m_XM16,m16);
                    SET_BLOCK_UNITS(m_M16_Clip,m16,M16_CLIP);

                    SET_BLOCK_HIDE(m_ARocket);
                    SET_GUN_HIDE(m_XRocket);

                    //
                    SET_BLOCK_CHECK ( m_Guns, TRR2_MASK_GUN );

                    SET_BLOCK_CHECK ( m_Magnum, TRR2_MASK_MAGNUM );
                    SET_BLOCK_CHECK ( m_Uzi, TRR2_MASK_UZI );
                    SET_BLOCK_CHECK ( m_Riotgun, TRR2_MASK_RIOT );

                    SET_BLOCK_CHECK ( m_M16, TRR2_MASK_M16 );
                    SET_BLOCK_CHECK ( m_Grenade, TRR2_MASK_GRENADE );
                    SET_BLOCK_CHECK ( m_Harpoon, TRR2_MASK_HARPOON );

                    SET_BLOCK_HIDE ( m_Rocket );

                    //
                    m_Zero.EnableWindow ( TRUE );
                    m_One.EnableWindow ( TRUE );
                    m_Four.EnableWindow ( TRUE );

                    SetBlockObjectOnDisplay ( tombraider, block, -1 );

                    //
                    SET_BLOCK_HIDE ( m_Tre_1 );
                    SET_BLOCK_HIDE ( m_Tre_2 );
                    SET_BLOCK_HIDE ( m_Tre_3 );
                    SET_BLOCK_HIDE ( m_Tre_4 );

                    //
                    m_Tre_1.EnableWindow ( FALSE );
                    m_Tre_2.EnableWindow ( FALSE );
                    m_Tre_3.EnableWindow ( FALSE );
                    m_Tre_4.EnableWindow ( FALSE );

                    //
                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                    m_Pickup.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockCrystal( tombraider, block ) );
                    m_Crystal.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockKills( tombraider, block ) );
                    m_Kill.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetPickupsForLevel ( tombraider, level - 1 ) );
                    m_Pickup_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetCrystalsForLevel ( tombraider, level - 1 ) );
                    m_Crystal_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetKillsForLevel ( tombraider, level - 1 ) );
                    m_Kill_Max.SetWindowText ( szText);

                    int acqsuired = CTR9SaveGame::I()->GetBlockSecretsAcquired( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", acqsuired );
                    m_Secrets_A.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetSecretsMaximum( tombraider, block ) );
                    m_Secrets_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockSecretsCurrent( tombraider, block ) );
                    m_Secrets_C.SetWindowText ( szText);

                    int completed =  CTR9SaveGame::I()->GetSecretsCompleted( tombraider, block );
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
                    m_Kill_Willard.EnableWindow ( CTR9SaveGame::I()->IsKillEnabled ( tombraider, block ) );

                    if ( pGun != NULL )
                    {
                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetAir ( tombraider, block) );
                        m_Air.SetWindowText ( szText );
                        m_Air.EnableWindow ( TRUE );

                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetState ( tombraider, block) );
                        m_State.SetWindowText ( szText );
                        m_State.EnableWindow ( TRUE );
                        m_Burning.EnableWindow ( TRUE );
                        m_Burning.SetCheck ( CTR9SaveGame::I()->GetState ( tombraider, block ) & STATE_123_BURNING );
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
                    m_Game_Plus.SetCheck ( CTR9SaveGame::I()->IsGamePlus(tombraider, block) );
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
                case GAME_TRR3:
                {
                    TABLE_TR3   *pBlock = ( TABLE_TR3 *) pBlockEntry;
                    GUN_TR3 *pGun       = ( GUN_TR3 *) pGunEntry;

                    SET_BLOCK_VALUE(m_AGuns,guns);
                    SET_GUN_VALUE(m_XGuns,guns);

                    sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetRealHealth( tombraider, block ) );
                    m_Real_Health.SetWindowTextA (szText );
                    if ( CTR9SaveGame::I()->GetRealHealthAddress ( tombraider, block ) == NULL )
                    {
                        m_Real_Health.EnableWindow ( FALSE );
                        m_Invincible.EnableWindow ( FALSE );
                    }
                    else
                    {
                        m_Real_Health.EnableWindow ( TRUE );
                        m_Invincible.EnableWindow ( TRUE );
                    }

                    SET_BLOCK_VALUE(m_LargePak,largeMedipak);
                    SET_BLOCK_VALUE(m_SmallPak,smallMedipak);
                    SET_BLOCK_VALUE(m_Flare,flare);
                    SET_BLOCK_VALUE(m_Col_Crystal,colCrystals);

                    SET_BLOCK_VALUE(m_AMagnum,magnum);
                    SET_GUN_VALUE(m_XMagnum,magnum);

                    SET_BLOCK_VALUE(m_AUzi,uzi);
                    SET_GUN_VALUE(m_XUzi,uzi);
                    SET_BLOCK_UNITS(m_Uzi_Clip,uzi,UZI_CLIP);

                    SET_BLOCK_VALUE(m_ARiotgun,riotgun/6);
                    SET_GUN_VALUE(m_XRiotgun,riotgun/6);

                    SET_BLOCK_VALUE(m_AGrenade,grenade);
                    SET_GUN_VALUE(m_XGrenade,grenade);
                    SET_BLOCK_UNITS(m_Grenade_Clip,grenade,GRENADE_CLIP);

                    SET_BLOCK_VALUE(m_AHarpoon,harpoon);
                    SET_GUN_VALUE(m_XHarpoon,harpoon);

                    SET_BLOCK_VALUE(m_AM16,m16);
                    SET_GUN_VALUE(m_XM16,m16);
                    SET_BLOCK_UNITS(m_M16_Clip,m16,M16_CLIP);

                    SET_BLOCK_VALUE(m_ARocket,rocket);
                    SET_GUN_VALUE(m_XRocket,rocket);

                    //
                    SET_BLOCK_CHECK ( m_Guns, TRR3_MASK_GUN );

                    SET_BLOCK_CHECK ( m_Magnum, TRR3_MASK_MAGNUM );
                    SET_BLOCK_CHECK ( m_Uzi, TRR3_MASK_UZI );
                    SET_BLOCK_CHECK ( m_Riotgun, TRR3_MASK_RIOT );

                    SET_BLOCK_CHECK ( m_M16, TRR3_MASK_M16 );
                    SET_BLOCK_CHECK ( m_Grenade, TRR3_MASK_GRENADE );
                    SET_BLOCK_CHECK ( m_Harpoon, TRR3_MASK_HARPOON );
                    SET_BLOCK_CHECK ( m_Rocket, TRR3_MASK_ROCKET );

                    m_Zero.EnableWindow ( TRUE );
                    m_One.EnableWindow ( TRUE );
                    m_Four.EnableWindow ( TRUE );

                    SetBlockObjectOnDisplay ( tombraider, block, -1 );

                    //
                    m_Tre_1.EnableWindow ( TRUE );
                    m_Tre_2.EnableWindow ( TRUE );
                    m_Tre_3.EnableWindow ( TRUE );
                    m_Tre_4.EnableWindow ( TRUE );

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetTreasure ( tombraider, block, slot, 0 ) );
                    m_Tre_1.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetTreasure ( tombraider, block, slot, 1 ) );
                    m_Tre_2.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetTreasure ( tombraider, block, slot, 2 ) );
                    m_Tre_3.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetTreasure ( tombraider, block, slot, 3 ) );
                    m_Tre_4.SetWindowText ( szText);

                    //
                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                    m_Pickup.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockCrystal( tombraider, block ) );
                    m_Crystal.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockKills( tombraider, block ) );
                    m_Kill.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetPickupsForLevel ( tombraider, level - 1 ) );
                    m_Pickup_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetCrystalsForLevel ( tombraider, level - 1 ) );
                    m_Crystal_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetKillsForLevel ( tombraider, level - 1 ) );
                    m_Kill_Max.SetWindowText ( szText);

                    int acqsuired = CTR9SaveGame::I()->GetBlockSecretsAcquired( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", acqsuired );
                    m_Secrets_A.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetSecretsMaximum( tombraider, block ) );
                    m_Secrets_Max.SetWindowText ( szText);

                    sprintf_s ( szText, sizeof(szText),  "%d", CTR9SaveGame::I()->GetBlockSecretsCurrent( tombraider, block ) );
                    m_Secrets_C.SetWindowText ( szText);

                    int completed =  CTR9SaveGame::I()->GetSecretsCompleted( tombraider, block );
                    sprintf_s ( szText, sizeof(szText),  "%d", completed );
                    m_Secrets_E.SetWindowText ( szText);

                    m_Pickup.EnableWindow ( TRUE );
                    m_Crystal.EnableWindow ( TRUE );
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
                    //
                    m_Show.EnableWindow ( TRUE );

                    m_Kill_Willard.EnableWindow ( CTR9SaveGame::I()->IsKillEnabled ( tombraider, block ) );

                    if ( pGun != NULL )
                    {
                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetAir ( tombraider, block) );
                        m_Air.SetWindowText ( szText );
                        m_Air.EnableWindow ( TRUE );

                        sprintf_s ( szText, sizeof(szText), "%d", CTR9SaveGame::I()->GetState ( tombraider, block) );
                        m_State.SetWindowText ( szText );
                        m_State.EnableWindow ( TRUE );
                        m_Burning.EnableWindow ( TRUE );
                        m_Burning.SetCheck ( CTR9SaveGame::I()->GetState ( tombraider, block ) & STATE_123_BURNING );
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
                    m_Game_Plus.SetCheck ( CTR9SaveGame::I()->IsGamePlus(tombraider, block) );
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

    SetValue ( m_Object_1, 0, FALSE );
    SetValue ( m_Object_2, 0, FALSE );
    SetValue ( m_Object_3, 0, FALSE );
    SetValue ( m_Object_4, 0, FALSE );

    SetValue ( m_Key_1, 0, FALSE );
    SetValue ( m_Key_2, 0, FALSE );
    SetValue ( m_Key_3, 0, FALSE );
    SetValue ( m_Key_4, 0, FALSE );

    SetValue ( m_Map_1, 0, FALSE );
    SetValue ( m_Map_2, 0, FALSE );

    SetValue ( m_Air, 0, FALSE );
    SetValue ( m_State, 0, FALSE );

    m_Real_Health.EnableWindow ( FALSE );
    m_Invincible.EnableWindow ( FALSE );

    m_Burning.EnableWindow ( FALSE );
    m_Write.EnableWindow ( FALSE );
    m_Max.EnableWindow ( FALSE );
    m_ResetToPlus.EnableWindow ( FALSE );
    m_ResetGames.EnableWindow ( FALSE );

    m_Zero.EnableWindow ( FALSE );
    m_One.EnableWindow ( FALSE );
    m_Four.EnableWindow ( FALSE );

    m_Pickup.EnableWindow ( FALSE );
    m_Crystal.EnableWindow ( FALSE );
    m_Kill.EnableWindow ( FALSE );

    m_Pickup_Max.SetWindowText ( "" );
    m_Crystal_Max.SetWindowText ( "" );
    m_Kill_Max.SetWindowText ( "" );

    //
    m_Tre_1.EnableWindow ( FALSE );
    m_Tre_2.EnableWindow ( FALSE );
    m_Tre_3.EnableWindow ( FALSE );
    m_Tre_4.EnableWindow ( FALSE );

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
void CTRXRemastered::OnBnClickedRefresh()
{
    //
    char    szFilename [ MAX_PATH ];

    //
    if ( m_Update.GetCheck() )
    {
        BOOL bRecurse = m_Recurse.GetCheck();
        UpdateBuffer(bRecurse);
    }

    //
    if ( CTR9SaveGame::IsBufferModified() )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR123 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            ZeroMemory ( szFilename, sizeof ( szFilename ) );
            m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
            CTR9SaveGame::WriteFile ( szFilename );
        }
        else if ( response == IDCANCEL )
        {
            return;
        }
        else
        {
            CTR9SaveGame::Discard();
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
    if ( m_Line >= 0 && CTR9SaveGame::I() != NULL )
    {
        //
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        currentTombraider           = pInfoData->tombraider;
        currentLevel                = pInfoData->level;
        currentBlock                = pInfoData->block;
        currentSlot                 = pInfoData->slot;
    }

    DisplayList ( szFilename );

    if ( currentTombraider >= 1 && currentBlock > 0 && currentSlot > 0 )
    {
        for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
        {
            DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( i );
            STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
            if (    currentTombraider == pInfoData->tombraider &&
                    currentBlock == pInfoData->block &&
                    currentSlot == pInfoData->slot )
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
void CTRXRemastered::DisplayList ( const char *pFilename )
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
        BOOL bRead = CTR9SaveGame::I()->ReadFile ( pFilename );
        if ( bRead )
        {
            m_Status.SetWindowText ( "File sucessfuily read" );
            CTR9SaveGame::I()->Load ( );
            BOOL bWritten = theApp.WriteProfileString ( PROFILE_SETTING, PROFILE_TRX_LAST_OPENED, pFilename );

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
void CTRXRemastered::DisplayList ( )
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
void CTRXRemastered::DisplayListBrief ( )
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
    int slotFound   = -1;

    //
    int tombraider = GAME_TRR1;
    for ( int block = 0; block < NB_TR1_BLOCKS; block++ )
    {
        WORD save   = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR1" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 1");
            if ( level >= TR1G_START )
            {
                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Unfinished Business");
            }

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            void *pAddress1 = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block);
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

            sprintf_s  ( szString, sizeof(szString), "%02d", slotFound );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "1");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR1 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;
            tagData [ position ].slot           = slotFound;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );

            position++;
        }
    }

    //  TR2
    tombraider = GAME_TRR2;
    for ( int block = 0; block < NB_TR2_BLOCKS; block++ )
    {
        WORD save   = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR2" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 2");
            if ( level >= TR2G_START )
            {
                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Golden Mask");
            }

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );


            void *pAddress1 = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound);
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block);
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

            sprintf_s  ( szString, sizeof(szString), "%02d", slotFound );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "2");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR2 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;
            tagData [ position ].slot           = slotFound;

            ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
            STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
            DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

            m_ListCtrl.SetItemData ( position, dwData );

            position++;
        }
    }

    //  TR3
    tombraider = GAME_TRR3;
    for ( int block = 0; block < NB_TR3_BLOCKS; block++ )
    {
        WORD save   = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
        WORD level  = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
        if ( save != 0 && level != 0 )
        {
            m_ListCtrl.InsertItem ( position, "TR3" );

            m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 3");
            if ( level >= TR3G_START )
            {
                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Lost Artefact");
            }

            sprintf_s  ( szString, sizeof(szString), "%03d", save );
            m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d", level );
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

            strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
            if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
            {
                strcat_s ( szString, sizeof(szString), "+" );
            }
            m_ListCtrl.SetItemText ( position, COLR_LEVEL_NAME, szString );

            void *pAddress1 = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound);
            if ( pAddress1 )
            {
                sprintf_s  ( szString, sizeof(szString), "0x%07X", RelativeAddress ( pAddress1 ) );
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, szString );
            }
            else
            {
                m_ListCtrl.SetItemText ( position, COLR_ADDRESS_1, "" );
            }

            void *pAddress2 = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block);
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

            sprintf_s  ( szString, sizeof(szString), "%02d", slotFound );
            m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

            sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

            m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "3");

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

            sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

            m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

            m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

            sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
            m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

            //  TR3 + block
            tagData [ position ].tombraider     = tombraider;
            tagData [ position ].level          = level;
            tagData [ position ].save           = save;
            tagData [ position ].block          = block;
            tagData [ position ].slot           = slotFound;

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
void CTRXRemastered::DisplayListFull ( bool bShort )
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
    int tombraider = GAME_TRR1;
    for ( int block = 0; block < NB_TR1_BLOCKS; block++ )
    {
        for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
        {
            TABLE_TR1 *pAddress1    = (TABLE_TR1 *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
            void *pAddress2         = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );

            if ( bShort && pAddress2 == NULL )
            {
                continue;
            }

            WORD    save    = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
            WORD    level   = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
            if ( /* save != 0 && */ level != 0 && pAddress1 != NULL && ( pAddress1 ->gunmask & 1 ) == 1 )
            {
                m_ListCtrl.InsertItem ( position, "TR1" );

                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 1");
                if ( level >= TR1G_START )
                {
                    m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Unfinished Business");
                }

                sprintf_s  ( szString, sizeof(szString), "%03d", save );
                m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d", level );
                m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

                strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
                if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
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

                sprintf_s  ( szString, sizeof(szString), "%02d", slot );
                m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

                m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "1");

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

                m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

                m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

                sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

                //  TR1 + block
                tagData [ position ].tombraider     = tombraider;
                tagData [ position ].level          = level;
                tagData [ position ].save           = save;
                tagData [ position ].block          = block;
                tagData [ position ].slot           = slot;

                ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
                STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
                DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

                m_ListCtrl.SetItemData ( position, dwData );
                position++;
            }
        }
    }

    //
    tombraider = GAME_TRR2;
    for ( int block = 0; block < NB_TR2_BLOCKS; block++ )
    {
        for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
        {
            TABLE_TR2 *pAddress1 = (TABLE_TR2 *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
            void *pAddress2 = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );

            WORD    save    = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
            WORD    level   = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
            if ( /* save != 0 && */ level != 0 && pAddress1 != NULL && ( pAddress1 ->gunmask & 1 ) == 1 )
            {
                m_ListCtrl.InsertItem ( position, "TR2" );

                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 2");
                if ( level >= TR2G_START )
                {
                    m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Golden Mask");
                }

                sprintf_s  ( szString, sizeof(szString), "%03d", save );
                m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d", level );
                m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

                strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
                if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
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

                sprintf_s  ( szString, sizeof(szString), "%02d", slot );
                m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

                m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "2");

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

                m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

                m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

                sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

                //  TR1 + block
                tagData [ position ].tombraider     = tombraider;
                tagData [ position ].level          = level;
                tagData [ position ].save           = save;
                tagData [ position ].block          = block;
                tagData [ position ].slot           = slot;

                ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
                STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
                DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

                m_ListCtrl.SetItemData ( position, dwData );
                position++;
            }
        }
    }

    //
    tombraider = GAME_TRR3;
    for ( int block = 0; block < NB_TR3_BLOCKS; block++ )
    {
        for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
        {
            TABLE_TR3 *pAddress1 = (TABLE_TR3 *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
            void *pAddress2 = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );

            WORD    save    = CTR9SaveGame::I()->GetSaveNumber ( tombraider, block );
            WORD    level   = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
            if ( /* save != 0 && */ level != 0 && pAddress1 != NULL && ( pAddress1 ->gunmask & 1 ) == 1 )
            {
                m_ListCtrl.InsertItem ( position, "TR3" );

                m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "Tombraider 3");
                if ( level >= TR3G_START )
                {
                    m_ListCtrl.SetItemText ( position, COLR_SUB_GAME, "The Lost Artefact");
                }

                sprintf_s  ( szString, sizeof(szString), "%03d", save );
                m_ListCtrl.SetItemText ( position, COLR_SAVE_NO, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d", level );
                m_ListCtrl.SetItemText ( position, COLR_LEVEL_NO, szString);

                strcpy_s ( szString, sizeof(szString), CTR9SaveGame::GetLevelName ( tombraider, level ) );
                if (  CTR9SaveGame::I()->IsGamePlus ( tombraider, block  ) )
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

                sprintf_s  ( szString, sizeof(szString), "%02d", slot );
                m_ListCtrl.SetItemText ( position, COLR_SLOT, szString);

                sprintf_s  ( szString, sizeof(szString), "%02d",  CTR9SaveGame::I()->countTag ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_FOUND, szString);

                m_ListCtrl.SetItemText ( position, COLR_TOMBRAIDER, "3");

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockKills ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_KILLS, szString);

                sprintf_s  ( szString, sizeof(szString), "%d",  CTR9SaveGame::I()->GetBlockPickup ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_PICKUPS, szString);

                m_ListCtrl.SetItemText ( position, COLR_ELAPSED, CTR9SaveGame::I()->GetBlockElapsed ( tombraider, block ) );

                m_ListCtrl.SetItemText ( position, COLR_DISTANCE, CTR9SaveGame::I()->GetBlockDistance ( tombraider, block ) );

                sprintf_s  ( szString, sizeof(szString), "%02X",  CTR9SaveGame::I()->GetBlockIndicator ( tombraider, block ) );
                m_ListCtrl.SetItemText ( position, COLR_EXTRA, szString);

                //  TR1 + block
                tagData [ position ].tombraider     = tombraider;
                tagData [ position ].level          = level;
                tagData [ position ].save           = save;
                tagData [ position ].block          = block;
                tagData [ position ].slot           = slot;

                ZeroMemory ( tagData [ position ].szText, sizeof(tagData [ position ].szText) );
                STRUCTDATA  *pInfoData  = & ( tagData [ position ] );
                DWORD_PTR dwData        = (DWORD_PTR  ) ( pInfoData );

                m_ListCtrl.SetItemData ( position, dwData );
                position++;
            }
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
void CTRXRemastered::UpdateBlock(TABLE_TR1 *pBlock, bool bMax, int level )
{
    int tombraider = GAME_TRR1;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
            pBlock->guns            = MAX_HEALTH;

            pBlock->largeMedipak    = MAX_PAK;
            pBlock->smallMedipak    = MAX_PAK;

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, true );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, true, MAX_AMMOS, true );
        }
        else
        {
            pBlock->guns            = GetValue ( m_AGuns );

            pBlock->largeMedipak    = GetValue ( m_LargePak );
            pBlock->smallMedipak    = GetValue ( m_SmallPak );
            pBlock->scion           = GetValue ( m_Flare );

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, GetValue ( m_Guns ) );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );

        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBlock (int tombraider, int block, TABLE_TR1 *pBlock, const GUN_TR1 *pGun, bool bMax, int level, BOOL bRecurse )
{
    //
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );

        //
        if ( pGun != NULL && bRecurse )
        {
            //
            //  Update All Slot with the same data
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                if ( CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot ) == pGun )
                {
                    pBlock = (TABLE_TR1 *)CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
                    UpdateBlock ( pBlock, bMax, level );
                }
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateGun(int tombraider, int block, GUN_TR1 *pGun, TABLE_TR1 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
            pGun->guns      = MAX_HEALTH;

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, true, MAX_AMMOS, true );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  MAX_AIR );
            CTR9SaveGame::I()->SetState ( tombraider, block, CTR9SaveGame::I()->GetState ( tombraider, block ) &  STATE_123_SAFE );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  MAX_HEALTH );
        }
        else
        {
            pGun->guns      = GetValue ( m_AGuns );

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  GetValue ( m_Air ) );
            CTR9SaveGame::I()->SetState ( tombraider, block,  GetValue ( m_State ) );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  GetValue ( m_Real_Health ) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBuffer(int tombraider, int block, TABLE_TR1 *pBlock, GUN_TR1 *pGun, bool bMax, BOOL bRecurse)
{
    int level = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock != NULL )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level, bRecurse );
        
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
void CTRXRemastered::UpdateBlock(TABLE_TR2 *pBlock, bool bMax, int level )
{
    int tombraider = GAME_TRR2;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
            pBlock->guns            = MAX_HEALTH;

            pBlock->largeMedipak    = MAX_PAK;
            pBlock->smallMedipak    = MAX_PAK;
            pBlock->flare           = MAX_PAK;

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, true );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, true, MAX_AMMOS, true );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 1, true );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 2, true );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 3, true );
        }
        else
        {
            pBlock->guns            = GetValue ( m_AGuns );

            pBlock->largeMedipak    = GetValue ( m_LargePak );
            pBlock->smallMedipak    = GetValue ( m_SmallPak );
            pBlock->flare           = GetValue ( m_Flare );

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, GetValue ( m_Guns ) );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );

        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBlock(int tombraider, int block, TABLE_TR2 *pBlock, GUN_TR2 *pGun, bool bMax, int level, BOOL bRecurse)
{
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );

        //
        if ( pGun != NULL && bRecurse )
        {
            //
            //  Update All Slot with the same data
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                if ( CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot ) == pGun )
                {
                    pBlock = (TABLE_TR2 *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
                    UpdateBlock ( pBlock, bMax, level );
                }
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateGun(int tombraider, int block, GUN_TR2 *pGun, TABLE_TR2 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
            pGun->guns      = MAX_HEALTH;

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, true, MAX_AMMOS, true );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 1, true );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 2, true );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 3, true );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  MAX_AIR );
            CTR9SaveGame::I()->SetState ( tombraider, block,  CTR9SaveGame::I()->GetState ( tombraider, block ) &  STATE_123_SAFE );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  MAX_HEALTH );
        }
        else
        {
            pGun->guns      = GetValue ( m_AGuns );

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  GetValue ( m_Air ) );
            CTR9SaveGame::I()->SetState ( tombraider, block,  GetValue ( m_State ) );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  GetValue ( m_Real_Health ) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBuffer(int tombraider, int block, TABLE_TR2 *pBlock, GUN_TR2 *pGun, bool bMax, BOOL bRecurse)
{
    //  Home Sweet Home : 18
    int level = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level, bRecurse );

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
void CTRXRemastered::UpdateBlock (TABLE_TR3 *pBlock, bool bMax, int level)
{
    int tombraider = GAME_TRR3;

    if ( pBlock != NULL )
    {
        if ( bMax )
        {
            pBlock->guns            = MAX_HEALTH;

            pBlock->largeMedipak    = MAX_PAK;
            pBlock->smallMedipak    = MAX_PAK;
            pBlock->flare           = MAX_PAK;

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, true );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, true, MAX_AMMOS, true );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 1, true );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 2, true );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 3, true );
            CTR9SaveGame::I()->SetRocket( tombraider, pBlock, NULL, level, true, MAX_AMMOS + 4, true );
        }
        else
        {
            pBlock->guns            = GetValue ( m_AGuns );

            pBlock->largeMedipak    = GetValue ( m_LargePak );
            pBlock->smallMedipak    = GetValue ( m_SmallPak );
            pBlock->flare           = GetValue ( m_Flare );
            pBlock->colCrystals     = GetValue ( m_Col_Crystal );

            CTR9SaveGame::I()->SetPistols ( tombraider, pBlock, level, GetValue ( m_Guns ) );
            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, NULL, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, NULL, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, NULL, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, NULL, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, NULL, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, NULL, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );
            CTR9SaveGame::I()->SetRocket( tombraider, pBlock, NULL, level, GetValue ( m_Rocket ), GetValue ( m_ARocket ), false );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBlock(int tombraider, int block, TABLE_TR3 *pBlock, GUN_TR3 *pGun, bool bMax, int level, BOOL bRecurse)
{
    if ( pBlock != NULL )
    {
        UpdateBlock ( pBlock, bMax, level );

        //
        if ( pGun != NULL && bRecurse )
        {
            //
            //  Update All Slot with the same data
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                if ( CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot ) == pGun )
                {
                    pBlock = (TABLE_TR3 *)CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
                    UpdateBlock ( pBlock, bMax, level );
                }
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateGun(int tombraider, int block, GUN_TR3 *pGun, TABLE_TR3 *pBlock, bool bMax, int level)
{
    if ( pGun != NULL )
    {
        if ( bMax )
        {
            pGun->guns      = MAX_HEALTH;

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 2, true );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, true, MAX_AMMOS - 1, true );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, true, MAX_AMMOS, true );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 1, true );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 2, true );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 3, true );
            CTR9SaveGame::I()->SetRocket( tombraider, pBlock, pGun, level, true, MAX_AMMOS + 4, true );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  MAX_AIR );
            CTR9SaveGame::I()->SetState ( tombraider, block,  CTR9SaveGame::I()->GetState ( tombraider, block ) &  STATE_123_SAFE );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  MAX_HEALTH );
        }
        else
        {
            pGun->guns      = GetValue ( m_AGuns );

            CTR9SaveGame::I()->SetMagnum ( tombraider, pBlock, pGun, level, GetValue ( m_Magnum ), GetValue ( m_AMagnum ), false );
            CTR9SaveGame::I()->SetUzi ( tombraider, pBlock, pGun, level, GetValue ( m_Uzi ), GetValue ( m_AUzi ), false );
            CTR9SaveGame::I()->SetRiotgun( tombraider, pBlock, pGun, level, GetValue ( m_Riotgun ), GetValue ( m_ARiotgun ) * 6, false );
            CTR9SaveGame::I()->SetGrenade( tombraider, pBlock, pGun, level, GetValue ( m_Grenade ), GetValue ( m_AGrenade ), false );
            CTR9SaveGame::I()->SetHarpoon( tombraider, pBlock, pGun, level, GetValue ( m_Harpoon ), GetValue ( m_AHarpoon ), false );
            CTR9SaveGame::I()->SetM16( tombraider, pBlock, pGun, level, GetValue ( m_M16 ), GetValue ( m_AM16 ), false );
            CTR9SaveGame::I()->SetRocket( tombraider, pBlock, pGun, level, GetValue ( m_Rocket ), GetValue ( m_ARocket ), false );

            CTR9SaveGame::I()->SetAir ( tombraider, block,  GetValue ( m_Air ) );
            CTR9SaveGame::I()->SetState ( tombraider, block,  GetValue ( m_State ) );
            CTR9SaveGame::I()->SetRealHealth ( tombraider, block,  GetValue ( m_Real_Health ) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::UpdateBuffer(int tombraider, int block, TABLE_TR3 *pBlock, GUN_TR3 *pGun, bool bMax, BOOL bRecurse)
{
    int level = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );

    if ( pBlock != NULL )
    {
        UpdateBlock ( tombraider, block, pBlock, pGun, bMax, level, bRecurse );

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
void CTRXRemastered::UpdateBuffer(BOOL bRecurse)
{
    char szText [ MAX_PATH ];

    if ( CTR9SaveGame::I(FALSE) == NULL )
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
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        void *pBlockEntry   = NULL;
        void *pGunEntry     = NULL;

        //
        if ( slot >= 0 )
        {
            pBlockEntry     = CTR9SaveGame::I()->GetBlockAddress( tombraider, block, slot );
            pGunEntry       = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );
        }
        else
        {
            pBlockEntry     = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
            pGunEntry       = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block );
        }

        CTR9SaveGame::I()->SetTRPlus ( 1, m_TR1_Plus.GetCheck () );
        CTR9SaveGame::I()->SetTRPlus ( 2, m_TR2_Plus.GetCheck () );
        CTR9SaveGame::I()->SetTRPlus ( 3, m_TR3_Plus.GetCheck () );


        CTR9SaveGame::I()->SetSaveNumber ( tombraider, block, GetValue ( m_Save_No ) );
        sprintf_s ( szText, sizeof(szText), "%03d", GetValue ( m_Save_No ) );
        m_ListCtrl.SetItemText ( m_Line, COLR_SAVE_NO, szText);

        //
        if ( pBlockEntry != NULL /* && pGunEntry != NULL */ )
        {
            switch ( tombraider )
            {
                //
                case GAME_TRR1:
                {
                    if ( slot < 0 )
                    {
                        //  Update All Occurrences
                        for ( int index = 0; index < NB_OF_SLOTS; index++ )
                        {
                            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                            GUN_TR1 *pGun       = ( GUN_TR1 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                            UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                        }
                    }
                    else
                    {
                        TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlockEntry;
                        GUN_TR1 *pGun       = ( GUN_TR1 * ) pGunEntry;
                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }

                    //  Object and keys are for a block
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, GetValue ( m_Object_1 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, GetValue ( m_Object_2 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, GetValue ( m_Object_3 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, GetValue ( m_Object_4 ) );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, GetValue ( m_Key_1 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, GetValue ( m_Key_2 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, GetValue ( m_Key_3 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, GetValue ( m_Key_4 ) );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, GetValue ( m_Map_1 ) );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, GetValue ( m_Map_2 ) );

                    CTR9SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR9SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR9SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR9SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );

                    break;
                }

                //
                case GAME_TRR2:
                {
                    if ( slot < 0 )
                    {
                        for ( int index = 0; index < NB_OF_SLOTS; index++ )
                        {
                            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                            GUN_TR2 *pGun       = ( GUN_TR2 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                            UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                        }
                    }
                    else
                    {
                        TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlockEntry;
                        GUN_TR2 *pGun       = ( GUN_TR2 * ) pGunEntry;
                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }

                    //  Object and keys are for a block
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, GetValue ( m_Object_1 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, GetValue ( m_Object_2 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, GetValue ( m_Object_3 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, GetValue ( m_Object_4 ) );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, GetValue ( m_Key_1 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, GetValue ( m_Key_2 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, GetValue ( m_Key_3 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, GetValue ( m_Key_4 ) );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, GetValue ( m_Map_1 ) );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, GetValue ( m_Map_2 ) );

                    CTR9SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR9SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR9SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR9SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );

                    break;
                }

                //
                case GAME_TRR3:
                {

                    if ( slot < 0 )
                    {
                        for ( int index = 0; index < NB_OF_SLOTS; index++ )
                        {
                            TABLE_TR3   *pBlock = ( TABLE_TR3 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                            GUN_TR3 *pGun       = ( GUN_TR3 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                            UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                        }
                    }
                    else
                    {
                        TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlockEntry;
                        GUN_TR3 *pGun       = ( GUN_TR3 * ) pGunEntry;
                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }

                    //  Object and keys are for a block
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, GetValue ( m_Object_1 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, GetValue ( m_Object_2 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, GetValue ( m_Object_3 ) );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, GetValue ( m_Object_4 ) );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, GetValue ( m_Key_1 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, GetValue ( m_Key_2 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, GetValue ( m_Key_3 ) );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, GetValue ( m_Key_4 ) );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, GetValue ( m_Map_1 ) );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, GetValue ( m_Map_2 ) );

                    CTR9SaveGame::I()->SetTreasure ( tombraider, block, slot, 0, GetValue ( m_Tre_1 ) );
                    CTR9SaveGame::I()->SetTreasure ( tombraider, block, slot, 1, GetValue ( m_Tre_2 ) );
                    CTR9SaveGame::I()->SetTreasure ( tombraider, block, slot, 2, GetValue ( m_Tre_3 ) );
                    CTR9SaveGame::I()->SetTreasure ( tombraider, block, slot, 3, GetValue ( m_Tre_4 ) );

                    CTR9SaveGame::I()->SetBlockPickup ( tombraider, block, GetValue ( m_Pickup ) );
                    CTR9SaveGame::I()->SetBlockKills ( tombraider, block, GetValue ( m_Kill ) );
                    CTR9SaveGame::I()->SetBlockCrystal ( tombraider, block, GetValue ( m_Crystal ) );

                    CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block, GetValue ( m_Secrets_A ) );

                    CTR9SaveGame::I()->SetGamePlus ( tombraider, block, m_Game_Plus.GetCheck() );

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
int CTRXRemastered::GetValue ( CEdit &edit )
{
    //
    int result = CTRXPropertyPage123::GetValue ( edit );

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
BOOL CTRXRemastered::GetValue ( CButton &button )
{
    return CTRXPropertyPage123::GetValue ( button );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickListctrl(NMHDR* pNMHDR, LRESULT* pResult)
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
                BOOL bRecurse = m_Recurse.GetCheck();
                UpdateBuffer(bRecurse);
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
void CTRXRemastered::OnItemchangedListctrl(NMHDR *pNMHDR, LRESULT *pResult)
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
                BOOL bRecurse = m_Recurse.GetCheck();
                UpdateBuffer(bRecurse);
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
void CTRXRemastered::SortItems ( int col )
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
void CTRXRemastered::OnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
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
void CTRXRemastered::ResetToGame ( int line )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;
    int slot                = pInfoData->slot;
    int slotFound           = -1;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    if ( slot >= 0 )
    {
        CTR9SaveGame::I()->SetLikeGame ( tombraider, block, slot );
    }
    else
    {
        pBlockEntry     = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
        CTR9SaveGame::I()->SetLikeGame ( tombraider, block, slotFound );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::ResetToPlus ( int line )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;
    int slot                = pInfoData->slot;
    int slotFound           = -1;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    if ( slot >= 0 )
    {
        CTR9SaveGame::I()->SetLikePlus ( tombraider, block, slot );
    }
    else
    {
        pBlockEntry     = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
        CTR9SaveGame::I()->SetLikePlus ( tombraider, block, slotFound );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::MaxOne ( int line, BOOL bRecurse )
{
    //
    DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( line );
    STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
    int tombraider          = pInfoData->tombraider;
    int level               = pInfoData->level;
    int block               = pInfoData->block;
    int slot                = pInfoData->slot;
    int slotFound           = -1;

    void *pBlockEntry   = NULL;
    void *pGunEntry     = NULL;

    //
    bool bMax = true;

    //
    if ( slot >= 0 )
    {
        pBlockEntry     = CTR9SaveGame::I()->GetBlockAddress( tombraider, block, slot );
        pGunEntry       = CTR9SaveGame::I()->GetGunAddress ( tombraider, block, slot );
    }
    else
    {
        pBlockEntry     = CTR9SaveGame::I()->SearchBlockEntry ( tombraider, block, &slotFound );
        pGunEntry       = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block );
    }


    if ( pBlockEntry != NULL /* && pGunEntry != NULL */ )
    {
        switch ( tombraider )
        {
            //
            case GAME_TRR1:
            {
                if ( slot < 0 )
                {
                    for ( int index = 0; index < NB_OF_SLOTS; index++ )
                    {
                        TABLE_TR1 *pBlock   = ( TABLE_TR1 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                        GUN_TR1 *pGun       = ( GUN_TR1 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }
                }
                else
                {
                    TABLE_TR1 *pBlock   = ( TABLE_TR1 *)pBlockEntry;
                    GUN_TR1 *pGun       = ( GUN_TR1 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                }

                if ( false )
                {
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, 1 );
                }

                break;
            }

            //
            case GAME_TRR2:
            {
                if ( slot < 0 )
                {
                    for ( int index = 0; index < NB_OF_SLOTS; index++ )
                    {
                        TABLE_TR2 *pBlock   = ( TABLE_TR2 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                        GUN_TR2 *pGun       = ( GUN_TR2 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }
                }
                else
                {
                    TABLE_TR2 *pBlock   = ( TABLE_TR2 *)pBlockEntry;
                    GUN_TR2 *pGun       = ( GUN_TR2 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                }

                if ( false )
                {
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, 1 );
                }

                break;
            }

            //
            case GAME_TRR3:
            {
                if ( slot < 0 )
                {
                    for ( int index = 0; index < NB_OF_SLOTS; index++ )
                    {
                        TABLE_TR3   *pBlock = ( TABLE_TR3 *) CTR9SaveGame::GetBlockSlot ( tombraider, block, index );
                        GUN_TR3 *pGun       = ( GUN_TR3 * ) CTR9SaveGame::GetBlockGuns ( tombraider, block, index );

                        UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                    }
                }
                else
                {
                    TABLE_TR3 *pBlock   = ( TABLE_TR3 *)pBlockEntry;
                    GUN_TR3 *pGun       = ( GUN_TR3 * ) pGunEntry;
                    UpdateBuffer ( tombraider, block, pBlock, pGun, bMax, bRecurse );
                }

                if ( false )
                {
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetKey ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 1, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 2, 1 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, 3, 1 );

                    CTR9SaveGame::I()->SetMap ( tombraider, block, 0, 1 );
                    CTR9SaveGame::I()->SetMap ( tombraider, block, 1, 1 );
                }

                break;
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedMax()
{
    //  For All Lines
    SetGUIModified( TRUE );

    for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
    {
        MaxOne ( i, true );
    }

    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedMaxOne()
{
    if ( m_Line >= 0 && m_Line < m_ListCtrl.GetItemCount() )
    {
        MaxOne ( m_Line, true );
        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedArFull()
{
    char szString [ 32 ];
    sprintf_s ( szString, sizeof(szString), "%d", MAX_AIR );
    m_Air.SetWindowText ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedArInfinite()
{
    char szString [ 32 ];
    sprintf_s ( szString, sizeof(szString), "%d", 0x7fff );
    m_Air.SetWindowText ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedListAll()
{
    //
    DisplayList ( );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedUpdate()
{
    if ( m_Update.GetCheck() )
    {
        BOOL bRecurse = m_Recurse.GetCheck();
        UpdateBuffer(bRecurse);
    }
    DisplayOne ( m_Line );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnSelchangeCombo()
{
    static char szFilename [ MAX_PATH ];

    if ( m_SetManualCombo )
    {
        //
        if ( CTR9SaveGame::I() != NULL )
        {
            if ( m_Update.GetCheck() )
            {
                BOOL bRecurse = m_Recurse.GetCheck();
                UpdateBuffer(bRecurse);
            }

            if ( CTR9SaveGame::IsBufferModified() )
            {
                INT_PTR response = CTRXDifferences::MessageBoxR123 ( "Do you want to save your changes ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
                if ( response == IDYES )
                {
                    DisplayOne ( m_Line );
                    ZeroMemory ( szFilename, sizeof ( szFilename ) );
                    m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
                    CTR9SaveGame::WriteFile ( szFilename );
                }
                else if ( response == IDCANCEL )
                {
                    SetComboSelection ( );
                    return;
                }
                else
                {
                    CTR9SaveGame::Discard();
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
void CTRXRemastered::OnBnClickedBurning()
{
    SetGUIModified( TRUE );

    //
    char szState [ 32 ];
    ZeroMemory ( szState, sizeof(szState) );

    int iState = GetValue ( m_State );

    if ( m_Burning.GetCheck() )
    {
        iState = iState | STATE_123_BURNING;
    }
    else
    {
        iState = iState & STATE_123_SAFE;
    }
    sprintf_s ( szState, sizeof(szState), "0x%x", iState );
    m_State.SetWindowText ( szState );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedRemoveLoc()
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
void CTRXRemastered::OnBnClickedZero()
{
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        SetBlockObjectOnDisplay ( tombraider, block, 0 );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedOne()
{
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        SetBlockObjectOnDisplay ( tombraider, block, 1 );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedFour()
{
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

#ifdef _DEBUG
        for ( tombraider = GAME_TRR1; tombraider <= GAME_TRR3; tombraider++ )
        {
            for ( block = 0; block < NB_TR_BLOCKS; block++ )
            {
                for ( int i = 0; i < 2; i++ )
                {
                    CTR9SaveGame::I()->SetMap ( tombraider, block, i, i + 10 );
                }

                for ( int i = 0; i < 4; i++ )
                {
                    CTR9SaveGame::I()->SetBlockObject ( tombraider, block, i, i + 2 );
                    CTR9SaveGame::I()->SetKey ( tombraider, block, i, i + 6 );
                }
            }
        }

        DisplayOne ( m_Line );
#else
        SetBlockObjectOnDisplay ( tombraider, block, 4 );
#endif
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedStrong()
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
const char *CTRXRemastered::GetLabelForObject ( int tombraider, int levelIndex, int iObject, bool bDouble )
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
        case GAME_TR15 :
        {
            levelIndex  = levelIndex + TR1G_START;
            break;
        }
        case GAME_TR25 :
        {
            levelIndex  = levelIndex + TR2G_START;
            break;
        }
        case GAME_TR35 :
        {
            levelIndex  = levelIndex + TR3G_START;
            break;
        }
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        case GAME_TR10:
        case GAME_TR15:
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
        case GAME_TRR2:
        case GAME_TR20:
        case GAME_TR25:
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
        case GAME_TRR3:
        case GAME_TR30:
        case GAME_TR35:
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
BOOL CTRXRemastered::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
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
                int slot                = pInfoData->slot;

                //
                if ( hitInfo.iSubItem == COLR_ADDRESS_1 )
                {
                    sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                        "%d %d:%d:%d\r\n", hitInfo.iItem, tombraider, block, slot );
                    void *pAddress = NULL;
                    if ( slot >= 0 )
                    {
                        pAddress = ( void *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, slot );
                    }
                    else
                    {
                        pAddress = ( void *) CTR9SaveGame::I()->GetBlockAddress ( tombraider, block, 0 );
                    }
                    sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                " - Address : 0x%08lX\r\n", RelativeAddress ( pAddress ) );

                    //
                    pAddress = ( void *) CTR9SaveGame::I()->GetBlockSecretsAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - A : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = ( void *) CTR9SaveGame::I()->GetBlockSecretsCurrentAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - C : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = ( void *) CTR9SaveGame::I()->GetSecretsCompletedAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - E : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                }
                else if ( hitInfo.iSubItem == COLR_ADDRESS_2 )
                {
                    void *pAddress = NULL;
                    pAddress = ( void *) CTR9SaveGame::I()->GetRealHealthAddress ( tombraider, block );
                    if ( pAddress )
                    {
                        sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText),
                                    " - RH : 0x%08lX\r\n", RelativeAddress ( pAddress ) );
                    }
                    pAddress = CTR9SaveGame::I()->SearchGunEntry ( tombraider, block);
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
                    DWORD *pDistance = CTR9SaveGame::I()->GetBlockDistanceAddress ( tombraider, block);
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
        int slot                = pInfoData->slot;

        //
        int levelIndex          = level - 1;

        //
        CWnd *WndArray [] =
        {
            &m_Object_1, &m_Object_2, &m_Object_3, &m_Object_4,
            &m_Key_1, &m_Key_2, &m_Key_3, &m_Key_4,
            &m_Map_1, &m_Map_2,
            &m_Tre_1, &m_Tre_2, &m_Tre_3, &m_Tre_4,
        };

        //
        GetCursorPos ( &ptAction );
        ScreenToClient ( &ptAction );
        for ( int iX = 0; iX < sizeof(WndArray)/sizeof(CWnd *); iX++ )
        {
            CWnd *pWnd      = WndArray[iX];

            RECT rect;
            pWnd->GetWindowRect ( &rect );
            ScreenToClient ( &rect );
            if ( ptAction.x >= rect.left && ptAction.x <= rect.right && ptAction.y >= rect.top && ptAction.y <= rect.bottom )
            {
                const char *pLabel = GetLabelForObject ( tombraider, levelIndex, iX );

                //
                if ( strlen ( pLabel ) > 0 )
                {
                    strcpy_s ( szText, sizeof(szText), pLabel );
                    pText->lpszText = szText;
                }
                else if ( iX < 4 )
                {
                    sprintf_s ( szText, sizeof(szText), "Object #%d", iX + 1 );
                    pText->lpszText = szText;
                }
                else if ( iX < 8 )
                {
                    sprintf_s ( szText, sizeof(szText), "Key #%d", iX - 4 + 1 );
                    pText->lpszText = szText;
                }
                else if ( iX < 10 )
                {
                    sprintf_s ( szText, sizeof(szText), "Map #%d", iX - 8 + 1 );
                    pText->lpszText = szText;
                }
                else
                {
                    sprintf_s ( szText, sizeof(szText), "Treasure #%d", iX - 10 + 1 );
                    pText->lpszText = szText;
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedSet()
{
    if ( m_Line >= 0 )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        if ( CTR9SaveGame::I() )
        {
            SetGUIModified( TRUE );

            switch ( tombraider )
            {
                //
                case GAME_TRR1:
                {
                    int acquired    = CTR9SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR9SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR9SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
                    break;
                }
                //
                case GAME_TRR2:
                {
                    int acquired    = CTR9SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR9SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR9SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
                    break;
                }
                //
                case GAME_TRR3:
                {
                    int acquired    = CTR9SaveGame::I()->GetBlockSecretsAcquired ( tombraider, block );
                    int completed   = CTR9SaveGame::I()->GetSecretsCompleted ( tombraider, block );
                    if ( acquired == completed )
                    {
                        // CTR9SaveGame::I()->SetBlockSecretsAcquired ( tombraider, block );
                    }
                    CTR9SaveGame::I()->SetBlockSecretsAcquiredAll ( tombraider, block, -1 );
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
void CTRXRemastered::OnBnClickedShow()
{
    if ( m_Line >= 0 && CTR9SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

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
DWORD CTRXRemastered::RelativeAddress ( const void *pAddress )
{
    if ( CTR9SaveGame::I() != NULL && CTR9SaveGame::I()->getBufferAddress() != NULL )
    {
        return CTRXTools::RelativeAddress ( pAddress, CTR9SaveGame::I()->getBufferAddress() );
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedKillWillard()
{
    if ( m_Line >= 0 && CTR9SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        if ( tombraider == GAME_TRR1 && level == 15 )
        {
            CTR9SaveGame::I()->KillTR1Boss ( tombraider, block );
        }
        else if ( tombraider == GAME_TRR3 && level == 19 )
        {
            CTR9SaveGame::I()->KillWillard ( tombraider, block );
        }

        SetGUIModified( TRUE );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::ShowInfo()
{
    if ( m_Line >= 0 && CTR9SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        CTRXRemasteredInfos dlg;
        dlg.m_iTombraider   = tombraider;
        dlg.m_iBlock        = block;
        dlg.m_iSlot         = slot;
        dlg.m_iLevel        = level;

        dlg.DoModal();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::ShowPosition()
{
    if ( m_Line >= 0 && CTR9SaveGame::I() )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;
        CTRXPosition    dlg;
        dlg.m_iTombraider   = tombraider;
        dlg.m_iBlock        = block;
        dlg.m_iSlot         = slot;
        dlg.m_iLevel        = level;
        dlg.m_123           = TRUE;

        dlg.DoModal();

        //  SHow Room in case of updated
        ShowRoom();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::ShowMap()
{
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        //
        CTRXMapAreas dlg;
        dlg.SetTombraiderLevelIndex ( tombraider, level - 1 );
        dlg.SetClickable ( FALSE );

        //
        TR9_POSITION *position = CTR9SaveGame::I()->GetPositionAddress(tombraider, block);
        if ( position != NULL )
        {
            int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( tombraider, block );
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
void CTRXRemastered::ShowRoom()
{
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        //
        TR9_POSITION *position = CTR9SaveGame::I(FALSE)->GetPositionAddress(tombraider, block);
        if ( position != NULL )
        {
            int levelNumber     = CTR9SaveGame::I(FALSE)->GetBlockLevelNumber ( tombraider, block );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( tombraider, levelIndex, position->wRoom );
            if ( pArea != NULL )
            {
                TR_CUR_POSITION     currentPosition;
                currentPosition.x               = position->dwWestToEast;
                currentPosition.z               = position->dwSouthToNorth;
                currentPosition.orientation     = position->wOrientation;
                m_Room.SetAreaAndPosition ( pArea, &currentPosition );

                m_Position_Label.SetWindowText( CTR9SaveGame::I()->getIndicatorLabel() );

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
                point = m_Room.ComputeRoomPoint ( position->dwWestToEast, position->dwSouthToNorth );
                double dfOrientation = CTRXTools::ConvertOrientationFromWORD ( position->wOrientation );
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
void CTRXRemastered::Renumber()
{
    if ( CTR9SaveGame::I(FALSE) != NULL )
    {
        CTR9SaveGame::I(FALSE)->ReNumber ( );
        DisplayList();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::DeleteOneGame()
{
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        UINT_PTR iResponse = CTRXMessageBox::MessageBox ( "Are You sure to want to delete this line ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( iResponse == IDYES )
        {
            CTR9SaveGame::I(FALSE)->Delete ( tombraider, block );
            DisplayList();
        }

    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnRclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
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
void CTRXRemastered::OnMenulistDelete()
{
    DeleteOneGame();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistExport()
{
    //
    static char BASED_CODE szFilter1[] = "Exports|savegame.1.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter2[] = "Exports|savegame.2.*.trx|All Files (*.*)|*.*||";
    static char BASED_CODE szFilter3[] = "Exports|savegame.3.*.trx|All Files (*.*)|*.*||";

    static char szDefault1 [ MAX_PATH ];
    static char szDefault2 [ MAX_PATH ];
    static char szDefault3 [ MAX_PATH ];

    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        const char *pFilter     = szFilter1;
        const char *pDefault    = szDefault1;

        sprintf_s ( szDefault1, "savegame.1.%02d.trx", CTR9SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );
        sprintf_s ( szDefault2, "savegame.2.%02d.trx", CTR9SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );
        sprintf_s ( szDefault3, "savegame.3.%02d.trx", CTR9SaveGame::I()->GetBlockLevelNumber( tombraider, block ) );

        switch ( tombraider )
        {
            case GAME_TRR1 :
            {
                pFilter     = szFilter1;
                pDefault    = szDefault1;
                break;
            }
            case GAME_TRR2 :
            {
                pFilter     = szFilter2;
                pDefault    = szDefault2;
                break;
            }
            case GAME_TRR3 :
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
            BOOL bResult = CTR9SaveGame::I()->Export ( dlgFileDialog.GetPathName(), tombraider, block );
            if ( ! bResult )
            {
                CTRXMessageBox::MessageBox ( "Export fails", MessageTitle, MB_OK|MB_ICONERROR );
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistImport()
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

    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;
        int slotFound           = -1;

        switch ( tombraider )
        {
            case GAME_TRR1 :
            {
                pFilter     = szFilter1;
                pDefault    = pDefault1;
                break;
            }
            case GAME_TRR2 :
            {
                pFilter     = szFilter2;
                pDefault    = pDefault2;
                break;
            }
            case GAME_TRR3 :
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
            BOOL bResult = CTR9SaveGame::I()->Import ( dlgFileDialog.GetPathName(), tombraider, block );
            if ( bResult )
            {
                DisplayList();
            }
            else
            {
                CTRXMessageBox::MessageBox ( "Import fails", MessageTitle, MB_OK|MB_ICONERROR );
            }

        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistInfos()
{
    ShowInfo();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistPosition()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistRenumber()
{
    Renumber();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistresettostart()
{
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;

        CTR9SaveGame::I()->SetLikeGame ( tombraider, block, slot );

        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnMenulistresettoplus()
{
    if ( m_Line >= 0 && CTR9SaveGame::I(FALSE) != NULL )
    {
        DWORD_PTR   dwData      = m_ListCtrl.GetItemData ( m_Line );
        STRUCTDATA  *pInfoData  = (STRUCTDATA *) dwData;
        int tombraider          = pInfoData->tombraider;
        int level               = pInfoData->level;
        int block               = pInfoData->block;
        int slot                = pInfoData->slot;

        CTR9SaveGame::I(FALSE)->SetLikePlus ( tombraider, block, slot );

        DisplayOne ( m_Line );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnChangeEdit()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedMagnum()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedUzi()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedRiotgun()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedM16()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedGrenade()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedHarpoon()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedGun()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedRocket()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnOK()
{
    //
    BOOL bRecurse = m_Recurse.GetCheck();
    UpdateBuffer(bRecurse);

    if ( CTR9SaveGame::I(FALSE) != NULL && ( IsGUIModified () || CTR9SaveGame::IsBufferModified() ) )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR123 ( "Do You want to save your changes ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            WriteFile();
        }
        else
        {
            CTR9SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    CTRXPropertyPage123::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemastered::OnApply()
{
    //
    BOOL bRecurse = m_Recurse.GetCheck();
    UpdateBuffer(bRecurse);

    if ( CTR9SaveGame::I(FALSE) != NULL && ( IsGUIModified() || CTR9SaveGame::IsBufferModified() ) )
    {
        WriteFile();
    }

    return CTRXPropertyPage123::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnCancel()
{
    BOOL bRecurse = m_Recurse.GetCheck();
    UpdateBuffer(bRecurse);

    if ( CTR9SaveGame::I( FALSE) != NULL && ( IsGUIModified () || CTR9SaveGame::IsBufferModified() ) )
    {
        INT_PTR response = CTRXDifferences::MessageBoxR123 ( "Do You want to save your changes ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
        if ( response == IDYES )
        {
            DisplayOne ( m_Line );
            WriteFile();
        }
        else
        {
            CTR9SaveGame::Discard();
            DisplayOne ( m_Line );
        }
    }

    CTRXPropertyPage123::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedTr1Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnClickedTr2Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedTr3Plus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedGamePlus()
{
    SetGUIModified( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedReset()
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
void CTRXRemastered::OnBnClickedResetgame()
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
void CTRXRemastered::OnBnClickedJournal()
{
    if ( CTR9SaveGame::I(FALSE) != NULL )
    {
        if ( m_Update.GetCheck() )
        {
            BOOL bRecurse = m_Recurse.GetCheck();
            UpdateBuffer(bRecurse);
        }

        //
        CTRXDifferences     dlg;
        dlg.m_bRemastered123   = TRUE;
        dlg.DoModal();

        DisplayOne ( m_Line );

        SetGUIModified ( CTR9SaveGame::IsBufferModified() );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnSelendokCombo()
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedPosition()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedShowMap()
{
    ShowMap();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::SingleClick()
{
    ShowPosition();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::SetThemeChanged ( bool bDarkTheme )
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

    CTRXPropertyPage123::SetThemeChanged ( bDarkTheme );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::DoDropFiles(const char *pFilename)
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
void CTRXRemastered::OnBnClickedRecurse()
{
    if ( m_Recurse.GetCheck() )
    {
        theApp.WriteProfileInt ( PROFILE_SETTING, PROFILE_RECURSE_WRITE, 1 );
    }
    else
    {
        theApp.WriteProfileInt ( PROFILE_SETTING, PROFILE_RECURSE_WRITE, 0 );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemastered::OnBnClickedShell()
{
    static char     szDirectory [ MAX_PATH ];
    m_Filename.GetWindowText ( szDirectory, sizeof ( szDirectory ) - 1 );
    theApp.RemoveFilename ( szDirectory );
    HINSTANCE hInst = ShellExecute ( NULL, "open", szDirectory, "", "", SW_SHOWDEFAULT );
}
