// TR3EquipmentPage.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXEquipmentPage.h"
#include "TRXTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char *pValueMinusOne   = "-1";
static const char *pValue10K1       = "10001";
static const char *pValue10K2       = "10002";
static const char *pValue10K3       = "10003";
static const char *pSome1K          = "1000";
static const char *pValue201        = "201";
static const char *pValue202        = "202";
static const char *pValue203        = "203";
static const char *pMany32K         = "0x7fff";

static const int Many32K            = 0x7fff;

//
static const    char *MessageTitle = "Tombraider Standard Editions";

//
/////////////////////////////////////////////////////////////////////////////
// CTRXEquipmentPage property page
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTRXEquipmentPage, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXEquipmentPage::CTRXEquipmentPage() : CTRXPropertyPage(CTRXEquipmentPage::IDD)
{
    //{{AFX_DATA_INIT(CTRXEquipmentPage)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXEquipmentPage::~CTRXEquipmentPage()
{
    CTRSaveGame *pGame = CTRSaveGame::I(FALSE);
    if ( pGame != NULL )
    {
        delete pGame;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTRXEquipmentPage)
    DDX_Control(pDX, IDC_GOD, m_God);
    DDX_Control(pDX, IDC_LIFE, m_Life);
    DDX_Control(pDX, IDC_BUT_FLARES, m_But_Flares);
    DDX_Control(pDX, IDC_BUT_SMEDI, m_But_Small_MediPak);
    DDX_Control(pDX, IDC_BUT_LMEDI, m_But_Large_MediPak);
    DDX_Control(pDX, IDC_UNFINITE_AIR, m_Unfinite_Air);
    DDX_Control(pDX, IDC_STATUS, m_Status);
    DDX_Control(pDX, IDC_SMALLMEDIPAK, m_Small_Medipak);
    DDX_Control(pDX, IDC_LARGEMEDIPAK, m_Large_Medipak);
    DDX_Control(pDX, IDC_HEALTH, m_GunAmmos);
    DDX_Control(pDX, IDC_FLARES, m_Flares);
    DDX_Control(pDX, IDC_AIR, m_Air);
    DDX_Control(pDX, IDC_LASER, m_Laser);
    DDX_Control(pDX, IDC_BINOCULAR, m_Binocular);
    DDX_Control(pDX, IDC_FULL, m_Full_Kits);
    DDX_Control(pDX, IDC_TRNG_GUNS, m_TRNG_Guns);
    DDX_Control(pDX, IDC_TRNG_GODMODE, m_TRNG_GodMode);
    DDX_Control(pDX, IDC_TRNG_DOORS, m_TRNG_Doors);
    DDX_Control(pDX, IDC_TRNG_ENEMIES, m_TRNG_Enemies);
    DDX_Control(pDX, IDC_TRNG_GODRESET, m_TRNG_GodReset);
    //}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXEquipmentPage, CTRXPropertyPage)
    //{{AFX_MSG_MAP(CTRXEquipmentPage)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_UNFINITE_AIR, OnUnfiniteAir)
    ON_BN_CLICKED(IDC_BUT_SMEDI, OnButSmedi)
    ON_BN_CLICKED(IDC_BUT_LMEDI, OnButLmedi)
    ON_BN_CLICKED(IDC_BUT_FLARES, OnButFlares)
    ON_BN_CLICKED(IDC_GOD, OnGod)
    ON_BN_CLICKED(IDC_LASER, &CTRXEquipmentPage::OnBnClickedLaser)
    ON_BN_CLICKED(IDC_BINOCULAR, &CTRXEquipmentPage::OnBnClickedBinocular)
    ON_EN_CHANGE(IDC_LIFE, &CTRXEquipmentPage::OnChangeEdit)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_FULL, &CTRXEquipmentPage::OnBnClickedFull)
    ON_BN_CLICKED(IDC_TRNG_GUNS, &CTRXEquipmentPage::OnBnClickedTrngGuns)
    ON_BN_CLICKED(IDC_TRNG_GODMODE, &CTRXEquipmentPage::OnBnClickedTrngGodMode)
    ON_BN_CLICKED(IDC_TRNG_DOORS, &CTRXEquipmentPage::OnBnClickedTrngDoors)
    ON_BN_CLICKED(IDC_TRNG_ENEMIES, &CTRXEquipmentPage::OnBnClickedTrngEnemies)
    ON_BN_CLICKED(IDC_TRNG_GODRESET, &CTRXEquipmentPage::OnBnClickedTrngGodreset)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXEquipmentPage message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnHelp()
{
    //
    CTRXHelpDialog dlg;
    dlg.DoModal();

}

//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::DisplayValues()
{
    BOOL bModified = IsGUIModified();

    char    szString [ 32 ];
    int     iX;
    int     iVal;

    if ( CTRSaveGame::I() != NULL )
    {
        m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus () );
    }

    EnableForVersion ();

    if ( CTRSaveGame::IsValid() )
    {
        /*
         *      Get gun state.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        /*
         *      Get current values for ammos.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetGunAmmos ( ) );
        m_GunAmmos.SetWindowText ( szString );

        iVal = CTRSaveGame::I()->GetRealHealth ( );
        sprintf_s ( szString, sizeof(szString), "%d", iVal );
        m_Life.SetWindowText ( szString );

        if ( iVal == -1 )
        {
            // m_Life.EnableWindow ( 0 );
            // m_God.EnableWindow ( 0 );
        }
        else
        {
            // m_Life.EnableWindow ( 1 );
            // m_God.EnableWindow ( 1 );
            if ( iVal == Many32K )
            {
                m_God.SetCheck ( TRUE );
            }
            else
            {
                m_God.SetCheck ( FALSE );
            }
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetSmallMedipak ( iX ) );
        m_Small_Medipak.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetLargeMedipak ( iX ) );
        m_Large_Medipak.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetFlares ( iX ) );
        m_Flares.SetWindowText ( szString );

        /*
         *      Display air.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAir () );
        m_Air.SetWindowText ( szString );

        m_Unfinite_Air.SetCheck ( FALSE );
        if ( CTRSaveGame::I()->GetAir () == Many32K )
        {
            m_Unfinite_Air.SetCheck ( TRUE );
        }

        //
        m_Laser.SetCheck ( FALSE );
        if ( CTRSaveGame::I()->GetLaser ( 0 ) != 0 )
        {
            m_Laser.SetCheck ( TRUE );
        }

        //
        m_Binocular.SetCheck ( FALSE );
        if ( CTRSaveGame::I()->GetBinocular ( 0 ) != 0 )
        {
            m_Binocular.SetCheck ( TRUE );
        }
    }

    SetGUIModified ( bModified );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEquipmentPage::OnSetActive()
{
    //
    BOOL bModified = IsGUIModified();
    if ( CTRSaveGame::IsValid( ) )
    {
        bModified = CTRSaveGame::IsBufferModified();
    }

    DisplayValues ();

    SetGUIModified ( bModified );

    return CTRXPropertyPage::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::UpdateBuffer()
{
    int                     iX;

    if ( CTRSaveGame::IsValid() )
    {
        /*
         *      Get gun state.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        /*
         *      Get current values for ammos.
         */
        CTRSaveGame::I()->SetSmallMedipak ( GetValue ( m_Small_Medipak ), iX );

        CTRSaveGame::I()->SetLargeMedipak ( GetValue ( m_Large_Medipak ), iX );

        CTRSaveGame::I()->SetFlares ( GetValue ( m_Flares ), iX );

        /*
         *      Set air.
         */
        CTRSaveGame::I()->SetAir ( GetValue ( m_Air ) );

        /*
         *      Set Health.
         */
        CTRSaveGame::I()->SetGunAmmos ( GetValue ( m_GunAmmos ) );

        /*
         *      Set Life.
         */
        CTRSaveGame::I()->SetRealHealth ( GetValue ( m_Life ) );

        //  Binocular
        CTRSaveGame::I()->SetBinocular ( GetValue ( m_Binocular ) );

        //  Laser
        CTRSaveGame::I()->SetLaser ( GetValue ( m_Laser ) );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEquipmentPage::OnKillActive()
{
    //
    UpdateBuffer();

    BOOL bModified = IsGUIModified();

    return CTRXPropertyPage::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXEquipmentPage::EnableForVersion()
{
    BOOL bModified = IsGUIModified();

    int iVersion =  CTRXPropertyPage::EnableForVersion();

    //      m_Air.EnableWindow ( bEnable2 );
    if ( iVersion != 0 )
    {
        m_Flares.EnableWindow ( iVersion >= GAME_TR20 );
        m_But_Flares.EnableWindow ( iVersion >= GAME_TR20 );

        m_Laser.EnableWindow ( iVersion >= GAME_TR40 );
        m_Binocular.EnableWindow ( iVersion >= GAME_TR40 );

        m_Life.EnableWindow ( iVersion <= GAME_TR30 || iVersion >= GAME_TR30 || iVersion == GAME_TR50 );
        m_GunAmmos.EnableWindow ( iVersion <= GAME_TR30 );
        m_Unfinite_Air.EnableWindow ( iVersion <= GAME_TR30 || iVersion == GAME_TR40 || iVersion == GAME_TR45 || iVersion == GAME_TR49 || iVersion == GAME_TR50 );
        m_Air.EnableWindow ( iVersion <= GAME_TR30 || iVersion == GAME_TR40 || iVersion == GAME_TR45 || iVersion == GAME_TR49 || iVersion == GAME_TR50 );
        m_God.EnableWindow ( iVersion <= GAME_TR30 );

        m_TRNG_Guns.EnableWindow ( iVersion == GAME_TR49 );
        m_TRNG_GodMode.EnableWindow ( iVersion == GAME_TR49 );
        m_TRNG_GodReset.EnableWindow ( iVersion == GAME_TR49 );
        m_TRNG_Doors.EnableWindow ( iVersion == GAME_TR49 );
        m_TRNG_Enemies.EnableWindow ( iVersion == GAME_TR49 );

        //
        if ( ! CTRSaveGame::I()->EnableGuns( TRUE ) )
        {
            m_TRNG_Guns.SetWindowText ( "Enable TRNG Guns" );
        }
        else
        {
            m_TRNG_Guns.SetWindowText ( "TRNG Guns Enabled" );
            m_TRNG_Guns.EnableWindow ( FALSE );
        }

        //
        if ( ! CTRSaveGame::I()->TRNGGodMode( TRUE ) )
        {
            m_TRNG_GodMode.SetWindowText ( "Enable TRNG God Mode" );
        }
        else
        {
            m_TRNG_GodMode.SetWindowText ( "Disable TRNG God Mode" );
        }

        //
        if ( ! CTRSaveGame::I()->TRNGOpenDoors( TRUE ) )
        {
            m_TRNG_Doors.SetWindowText ( "Enable TRNG Open Doors" );
        }
        else
        {
            m_TRNG_Doors.SetWindowText ( "Disable TRNG Open Doors" );
        }

        //
        if ( ! CTRSaveGame::I()->TRNGKillEnemies( TRUE ) )
        {
            m_TRNG_Enemies.SetWindowText ( "Enable TRNG Kill Enemies" );
        }
        else
        {
            m_TRNG_Enemies.SetWindowText ( "Disable TRNG Kill Enemies" );
        }
    }
    else
    {
        m_Flares.EnableWindow ( FALSE );
        m_But_Flares.EnableWindow ( FALSE );

        m_Laser.EnableWindow ( FALSE );
        m_Binocular.EnableWindow ( FALSE );

        m_Life.EnableWindow ( FALSE );
        m_GunAmmos.EnableWindow ( FALSE );
        m_Unfinite_Air.EnableWindow ( FALSE );
        m_Air.EnableWindow ( FALSE );
        m_God.EnableWindow ( FALSE );
        m_TRNG_Guns.EnableWindow ( FALSE );
        m_TRNG_GodMode.EnableWindow ( FALSE );
        m_TRNG_GodReset.EnableWindow ( FALSE );
        m_TRNG_Doors.EnableWindow ( FALSE );
        m_TRNG_Enemies.EnableWindow ( FALSE );
    }

    if ( CTRSaveGame::I() != NULL && CTRSaveGame::I()->Valid ( ) )
    {
        m_But_Small_MediPak.EnableWindow ( TRUE );
        m_But_Large_MediPak.EnableWindow ( TRUE );
        m_Small_Medipak.EnableWindow ( TRUE );
        m_Large_Medipak.EnableWindow ( TRUE );
    }
    else
    {
        m_But_Small_MediPak.EnableWindow ( FALSE );
        m_But_Large_MediPak.EnableWindow ( FALSE );
        m_Small_Medipak.EnableWindow ( FALSE );
        m_Large_Medipak.EnableWindow ( FALSE );
    }

    SetGUIModified ( bModified, "Equipment Version" );

    return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnUnfiniteAir()
{
    SetGUIModified ( TRUE, "Equipment UnfiniteAir" );

    char        szString [ 32 ];
    //
    if ( m_Unfinite_Air.GetCheck () )
    {
        sprintf_s ( szString, sizeof(szString), "%d", Many32K );
        m_Air.SetWindowText ( szString );
    }
    else
    {
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAir () );
        m_Air.SetWindowText ( szString );
    }   
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnButSmedi()
{
    //
    SetGUIModified ( TRUE, "Equipment Smedi" );
    // m_But_Small_MediPak.SetCheck ( FALSE );
    if ( CTRSaveGame::I() != NULL && CTRSaveGame::GetVersion () >= 40 )
    {
        m_Small_Medipak.SetWindowText ( pValue10K1 );
    }
    else
    {
        m_Small_Medipak.SetWindowText ( pValue201 );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnButLmedi()
{
    //
    SetGUIModified ( TRUE, "Equipment Lmedi" );

    // m_But_Large_MediPak.SetCheck ( FALSE );
    if ( CTRSaveGame::I() != NULL && CTRSaveGame::GetVersion () >= 40 )
    {
        m_Large_Medipak.SetWindowText ( pValue10K2 );
    }
    else
    {
        m_Large_Medipak.SetWindowText ( pValue202 );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnButFlares()
{
    //
    int     iX;

    SetGUIModified ( TRUE );

    if ( CTRSaveGame::IsValid() )
    {
        /*
         *      Get gun state.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        if ( CTRSaveGame::GetVersion () >= 40 )
        {
            m_Flares.SetWindowText ( pValue10K3 );
        }
        else
        {
            m_Flares.SetWindowText ( pValue203 );
        }

    }
    // m_But_Flares.SetCheck ( FALSE );
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnGod()
{
    SetGUIModified ( TRUE );

    char                    szString [ 32 ];

    //
    if ( m_God.GetCheck () )
    {
        sprintf_s ( szString, sizeof(szString), "%d", Many32K );
        m_Life.SetWindowText ( szString );
    }
    else
    {
        m_Life.SetWindowText ( pSome1K );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedLaser()
{
    SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedBinocular()
{
    SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEquipmentPage::OnInitDialog()
{
    CTRXPropertyPage::OnInitDialog();

    //
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_Status, "Status" );
        m_ToolTip.AddTool( &m_TRNG_Guns, "Enable TRNG Guns when disabled" );
        m_ToolTip.AddTool( &m_TRNG_GodMode, "Enable TRNG God Mode\nThis could be unreversible\nAnd you may be god forever" );
        m_ToolTip.AddTool( &m_TRNG_GodReset, "Remove TRNG SNG_IMMORTAL_LARA,\nSNG_REMOVE_IMMORTAL_LARA and\nSNG_INFINITE_AIR" );
        m_ToolTip.AddTool( &m_TRNG_Doors, "Enable TRNG Open All Doors\nMay not work on all versions" );
        m_ToolTip.AddTool( &m_TRNG_Enemies, "Enable TRNG Kill All Enemies\nMay not work on all versions" );
        m_ToolTip.Activate(TRUE);
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEquipmentPage::OnApply()
{
    WriteWhenOnApply("Equipment");

    return CTRXPropertyPage::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnOK()
{
    //
    INT_PTR response = AskToSave("Equipment");
    DisplayValues ( );

    CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnCancel()
{
    //
    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        INT_PTR response = AskToSave("Equipment");
    }

    CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnChangeEdit()
{
    SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedFull()
{
    SetGUIModified ( TRUE );

    if ( CTRSaveGame::I() != NULL )
    {
        if ( CTRSaveGame::GetVersion () >= 40 )
        {
            m_Small_Medipak.SetWindowText ( pValue10K1 );
            m_Large_Medipak.SetWindowText ( pValue10K2 );
            m_Flares.SetWindowText ( pValue10K3 );
        }
        else
        {
            m_Small_Medipak.SetWindowText ( pValue201 );
            m_Large_Medipak.SetWindowText ( pValue202 );
            m_Flares.SetWindowText ( pValue203 );
        }

        if ( m_iHitCountForAll > 0 )
        {
            m_Laser.SetCheck ( TRUE );
            m_Binocular.SetCheck ( TRUE );
        }

        m_iHitCountForAll++;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedTrngGuns()
{
    //
    SetGUIModified ( TRUE, "Equipment TRNG Guns" );
    CTRSaveGame::I()->EnableGuns( FALSE, TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedTrngGodMode()
{
    SetGUIModified ( TRUE, "Equipment TRNG God Mode" );
    if ( ! CTRSaveGame::I()->TRNGGodMode( TRUE ) )
    {
        CTRSaveGame::I()->TRNGGodMode( FALSE, TRUE );
        m_TRNG_GodMode.SetWindowText ( "Disable TRNG God Mode" );
    }
    else
    {
        CTRSaveGame::I()->TRNGGodMode( FALSE, FALSE );
        m_TRNG_GodMode.SetWindowText ( "Enable TRNG God Mode" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedTrngDoors()
{
    SetGUIModified ( TRUE, "Equipment TRNG Doors" );
    if ( ! CTRSaveGame::I()->TRNGOpenDoors( TRUE ) )
    {
        CTRSaveGame::I()->TRNGOpenDoors( FALSE, TRUE );
        m_TRNG_Doors.SetWindowText ( "Disable TRNG Open Doors" );
    }
    else
    {
        CTRSaveGame::I()->TRNGOpenDoors( FALSE, FALSE );
        m_TRNG_Doors.SetWindowText ( "Enable TRNG Open Doors" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedTrngEnemies()
{
    SetGUIModified ( TRUE, "Equipment TRNG Enemies" );
    if ( ! CTRSaveGame::I()->TRNGKillEnemies( TRUE ) )
    {
        CTRSaveGame::I()->TRNGKillEnemies( FALSE, TRUE );
        m_TRNG_Enemies.SetWindowText ( "Disable TRNG Kill Enemies" );
    }
    else
    {
        CTRSaveGame::I()->TRNGKillEnemies( FALSE, FALSE );
        m_TRNG_Enemies.SetWindowText ( "Enable TRNG Kill Enemies" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnBnClickedTrngGodreset()
{
    //
    CTRSaveGame::I()->TRNGGodMode( FALSE, FALSE, TRUE );
    m_TRNG_GodMode.SetWindowText ( "Enable TRNG God Mode" );
}
