// TR3AmmosPage.cpp : implementation file
//
#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXAmmosPage.h"
#include "TRXTools.h"

#include "GunGrids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char *pValueMinusOne       = "-1";
static const char *pValue10K            = "10000";
static const char *pSome5K              = "5000";

//
static const    char *MessageTitle = "Tombraider Standard Editions";

//
/////////////////////////////////////////////////////////////////////////////
// CTRXAmmosPage property page
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTRXAmmosPage, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXAmmosPage::CTRXAmmosPage() : CTRXPropertyPage(CTRXAmmosPage::IDD)
{
    //{{AFX_DATA_INIT(CTRXAmmosPage)
    //}}AFX_DATA_INIT
    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXAmmosPage::~CTRXAmmosPage()
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
void CTRXAmmosPage::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTRXAmmosPage)
    DDX_Control(pDX, IDC_CGUNS, m_C_Guns);
    DDX_Control(pDX, IDC_CUZIS, m_C_Uzis);
    DDX_Control(pDX, IDC_CROCKETLAUNCHER, m_C_Rocket_Launcher);
    DDX_Control(pDX, IDC_CMP5, m_C_MP5);
    DDX_Control(pDX, IDC_CRIOTGUN, m_C_Riot_Gun);
    DDX_Control(pDX, IDC_CHARPOON, m_C_Harpoon);
    DDX_Control(pDX, IDC_CGRENADELAUNCHER, m_C_Grenade_Launcher);
    DDX_Control(pDX, IDC_CDESERTEAGLE, m_C_Desert_Eagle);
    DDX_Control(pDX, IDC_STATUS, m_Status);
    DDX_Control(pDX, IDC_UNLIMITED, m_Unlimited);

    DDX_Control(pDX, IDC_AGUNS, m_Ammos_Guns);
    DDX_Control(pDX, IDC_AUZIS, m_Ammos_Uzis);
    DDX_Control(pDX, IDC_AROCKETLAUNCHER, m_Ammos_Rocket_Launcher);
    DDX_Control(pDX, IDC_ARIOTGUN, m_Ammos_Riot_Gun);
    DDX_Control(pDX, IDC_ARIOTGUN2, m_Ammos_Riot_Gun2);
    DDX_Control(pDX, IDC_AMP5, m_Ammos_MP5);
    DDX_Control(pDX, IDC_AHARPOON, m_Ammos_Harpoon);
    DDX_Control(pDX, IDC_AHARPOON2, m_Ammos_Harpoon2);
    DDX_Control(pDX, IDC_AHARPOON3, m_Ammos_Harpoon3);
    DDX_Control(pDX, IDC_AGRENADELAUNCHER, m_Ammos_Grenade_Launcher);
    DDX_Control(pDX, IDC_AGRENADELAUNCHER2, m_Ammos_Grenade_Launcher2);
    DDX_Control(pDX, IDC_AGRENADELAUNCHER3, m_Ammos_Grenade_Launcher3);
    DDX_Control(pDX, IDC_ADESERTEAGLE, m_Ammos_Desert_Eagle);

    DDX_Control(pDX, IDC_BM_RIOTGUN, m_BM_RiotGun);
    DDX_Control(pDX, IDC_BM_DESERTEAGLE, m_BM_DesertEagle);
    DDX_Control(pDX, IDC_BM_UZIS, m_BM_Uzis);
    DDX_Control(pDX, IDC_BM_GRENADE, m_BM_Grenade);
    DDX_Control(pDX, IDC_BM_MP5, m_BM_MP5);
    DDX_Control(pDX, IDC_BM_ROCKET, m_BM_Rocket);
    DDX_Control(pDX, IDC_BM_ARROW, m_BM_Arrow);

    //
    DDX_Control(pDX, IDC_ZRIOTGUN, m_ZeroRiotGun);
    DDX_Control(pDX, IDC_ZMP5, m_ZeroMP5);
    DDX_Control(pDX, IDC_ZHARPOON, m_ZeroHarpoon);
    DDX_Control(pDX, IDC_ZDESERT, m_ZeroDesert);
    DDX_Control(pDX, IDC_ZGRENADE, m_ZeroGrenade);
    DDX_Control(pDX, IDC_ZUZI, m_ZeroUzi);
    DDX_Control(pDX, IDC_ZROCKET, m_ZeroRocket);
    DDX_Control(pDX, IDC_ZGUNS, m_ZeroGuns);
    DDX_Control(pDX, IDC_ALL_AMMOS, m_Full_Ammos);
    //}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXAmmosPage, CTRXPropertyPage)
    //{{AFX_MSG_MAP(CTRXAmmosPage)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_CGUNS, &OnCguns)
    ON_BN_CLICKED(IDC_CROCKETLAUNCHER, OnCrocketlauncher)
    ON_BN_CLICKED(IDC_CUZIS, OnCuzis)
    ON_BN_CLICKED(IDC_CMP5, OnCmp5)
    ON_BN_CLICKED(IDC_CDESERTEAGLE, OnCdeserteagle)
    ON_BN_CLICKED(IDC_CGRENADELAUNCHER, OnCgrenadelauncher)
    ON_BN_CLICKED(IDC_CRIOTGUN, OnCriotgun)
    ON_BN_CLICKED(IDC_CHARPOON, OnCharpoon)
    ON_BN_CLICKED(IDC_UNLIMITED, &CTRXAmmosPage::OnBnClickedUnlimited)

    ON_EN_CHANGE(IDC_AGUNS, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_ARIOTGUN, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_ARIOTGUN2, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AUZIS, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_ADESERTEAGLE, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AMP5, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AHARPOON, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AHARPOON2, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AHARPOON3, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AGRENADELAUNCHER, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AGRENADELAUNCHER2, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AGRENADELAUNCHER3, &CTRXAmmosPage::OnChangeEdit)
    ON_EN_CHANGE(IDC_AROCKETLAUNCHER, &CTRXAmmosPage::OnChangeEdit)

    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ZRIOTGUN, &CTRXAmmosPage::OnBnClickedZriotgun)
    ON_BN_CLICKED(IDC_ZMP5, &CTRXAmmosPage::OnBnClickedZmp5)
    ON_BN_CLICKED(IDC_ZHARPOON, &CTRXAmmosPage::OnBnClickedZharpoon)
    ON_BN_CLICKED(IDC_ZDESERT, &CTRXAmmosPage::OnBnClickedZdesert)
    ON_BN_CLICKED(IDC_ZGRENADE, &CTRXAmmosPage::OnBnClickedZgrenade)
    ON_BN_CLICKED(IDC_ZUZI, &CTRXAmmosPage::OnBnClickedZuzi)
    ON_BN_CLICKED(IDC_ZROCKET, &CTRXAmmosPage::OnBnClickedZrocket)
    ON_BN_CLICKED(IDC_ZGUNS, &CTRXAmmosPage::OnBnClickedZguns)
    ON_BN_CLICKED(IDC_ALL_AMMOS, &CTRXAmmosPage::OnBnClickedAllAmmos)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXAmmosPage message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnHelp()
{
    //
    CTRXHelpDialog dlg;
    dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::DisplayValues()
{
    BOOL bModified = IsGUIModified();

    char    szString [ 32 ];
    int     iX;

    if ( CTRSaveGame::I() != NULL )
    {
        m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus ( ) );
    }

    int iVersion = EnableForVersion ();

    //SetBitmap ( &m_BM_DesertEagle, 1, IDB_AMMOS_GRENADES );

    /*
     *      Get gun state.
     */
    if ( CTRSaveGame::IsValid() )
    {
        iX = CTRSaveGame::GetLevelIndex ();

        //
        //  Set Labels
        //
        //      Weapons Order
        //      iMaskCompass            : Weapon0
        //      iMaskPistol             : Weapon1
        //      iMaskDesertEagle        : Weapon2
        //      iMaskUzi                : Weapon3
        //      iMaskShotGun            : Weapon4
        //      iMaskMP5                : Weapon5
        //      iMaskRocket             : Weapon6
        //      iMaskGrenade            : Weapon7
        //      iMaskHarpoon            : Weapon8
        // m_C_Guns.SetWindowTextA( CTRSaveGame::I()->GetLabel1() );
        m_C_Desert_Eagle.SetWindowTextA( CTRSaveGame::I()->GetLabel2() );
        m_C_Uzis.SetWindowTextA( CTRSaveGame::I()->GetLabel3() );
        m_C_Riot_Gun.SetWindowTextA( CTRSaveGame::I()->GetLabel4() );
        m_C_MP5.SetWindowTextA( CTRSaveGame::I()->GetLabel5() );
        m_C_Rocket_Launcher.SetWindowTextA( CTRSaveGame::I()->GetLabel6() );
        m_C_Grenade_Launcher.SetWindowTextA( CTRSaveGame::I()->GetLabel7() );
        m_C_Harpoon.SetWindowTextA( CTRSaveGame::I()->GetLabel8() );

        SetBitmap ( &m_BM_DesertEagle, 2, CTRSaveGame::I()->GetAmmoBitmap2() );
        SetBitmap ( &m_BM_Uzis, 3, CTRSaveGame::I()->GetAmmoBitmap3() );
        SetBitmap ( &m_BM_RiotGun, 4, CTRSaveGame::I()->GetAmmoBitmap4() );
        SetBitmap ( &m_BM_MP5, 5, CTRSaveGame::I()->GetAmmoBitmap5() );
        SetBitmap ( &m_BM_Rocket, 6, CTRSaveGame::I()->GetAmmoBitmap6() );
        SetBitmap ( &m_BM_Grenade, 7, CTRSaveGame::I()->GetAmmoBitmap7() );
        SetBitmap ( &m_BM_Arrow, 8, CTRSaveGame::I()->GetAmmoBitmap8() );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos1 ( iX ) );
        m_Ammos_Guns.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos2 ( iX ) );
        m_Ammos_Desert_Eagle.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos4a ( iX ) );
        m_Ammos_Riot_Gun.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos4b ( iX ) );
        m_Ammos_Riot_Gun2.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos3 ( iX ) );
        m_Ammos_Uzis.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos7a ( iX ) );
        m_Ammos_Grenade_Launcher.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos7b ( iX ) );
        m_Ammos_Grenade_Launcher2.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos7c ( iX ) );
        m_Ammos_Grenade_Launcher3.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos6 ( iX ) );
        m_Ammos_Rocket_Launcher.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos8a ( iX ) );
        m_Ammos_Harpoon.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos8b ( iX ) );
        m_Ammos_Harpoon2.SetWindowText ( szString );

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos8c ( iX ) );
        m_Ammos_Harpoon3.SetWindowText ( szString );

        //
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetAmmos5 ( iX ) );
        m_Ammos_MP5.SetWindowText ( szString );

        /*
         *      Infinite ammos.
         */
        if ( CTRSaveGame::I()->GetUnlimitedAmmos () )
        {
            m_Unlimited.SetCheck ( TRUE );
        }
        else
        {
            m_Unlimited.SetCheck ( FALSE );
        }
    }

    SetGUIModified( bModified, "Ammos DisplayValues" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXAmmosPage::OnSetActive()
{
    //
    BOOL bModified = IsGUIModified();
    if ( CTRSaveGame::IsValid( ) )
    {
        bModified = CTRSaveGame::IsBufferModified();
    }

    DisplayValues ();
    
    SetGUIModified ( bModified, "Ammos OnSetActive" );

    return CTRXPropertyPage::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::UpdateBuffer()
{
    int                     iX;

    char                    szString [ 32 ];

    if ( CTRSaveGame::IsValid() )
    {

        /*
         *      Get gun state.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        /*
         *      Get current values for ammos.
         */
        m_Ammos_Guns.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos1 ( szString, iX );

        m_Ammos_Desert_Eagle.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos2 ( szString, iX );
        // CTRSaveGame::I()->SetAmmos2 ( szString );

        //
        m_Ammos_Uzis.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos3 ( szString, iX );
        // CTRSaveGame::I()->SetAmmos3 ( szString );

        //
        m_Ammos_Riot_Gun.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos4a ( szString, iX );
        // CTRSaveGame::I()->SetAmmos4a ( szString );

        m_Ammos_Riot_Gun2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos4b ( szString, iX );

        //
        m_Ammos_MP5.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos5 ( szString, iX );
        // CTRSaveGame::I()->SetAmmos5 ( szString );

        //
        m_Ammos_Rocket_Launcher.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos6 ( szString, iX );
        // CTRSaveGame::I()->SetAmmos6 ( szString );

        //
        m_Ammos_Grenade_Launcher.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos7a ( szString, iX );
        // CTRSaveGame::I()->SetAmmos7a ( szString );

        m_Ammos_Grenade_Launcher2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos7b ( szString, iX );
        // CTRSaveGame::I()->SetAmmos7a ( szString );

        m_Ammos_Grenade_Launcher3.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos7c ( szString, iX );
        // CTRSaveGame::I()->SetAmmos7a ( szString );

        //
        m_Ammos_Harpoon.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos8a ( szString, iX );
        // CTRSaveGame::I()->SetAmmos8a ( szString );

        m_Ammos_Harpoon2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos8b ( szString, iX );
        // CTRSaveGame::I()->SetAmmos8a ( szString );

        m_Ammos_Harpoon3.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetAmmos8c ( szString, iX );
        // CTRSaveGame::I()->SetAmmos8a ( szString );

        /*
         *      Infinite ammos.
         */
        CTRSaveGame::I()->SetUnlimitedAmmos ( 0x00 );
        if ( m_Unlimited.GetCheck () )
        {
            CTRSaveGame::I()->SetUnlimitedAmmos ( 0xff );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXAmmosPage::OnKillActive()
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
int CTRXAmmosPage::EnableForVersion()
{
    BOOL bModified = IsGUIModified();

    int iVersion = CTRXPropertyPage::EnableForVersion();

    if ( CTRSaveGame::I() != NULL && iVersion != 0 )
    {   
        m_Ammos_Guns.EnableWindow ( CTRSaveGame::I()->HasAmmos1 ( ) );
        m_C_Guns.EnableWindow ( CTRSaveGame::I()->HasAmmos1 ( ) );
        m_ZeroGuns.EnableWindow ( CTRSaveGame::I()->HasAmmos1 ( ) );

        m_Ammos_Desert_Eagle.EnableWindow ( CTRSaveGame::I()->HasAmmos2 ( ) );
        m_C_Desert_Eagle.EnableWindow ( CTRSaveGame::I()->HasAmmos2 ( ) );
        m_ZeroDesert.EnableWindow ( CTRSaveGame::I()->HasAmmos2 ( ) );

        m_Ammos_Uzis.EnableWindow ( CTRSaveGame::I()->HasAmmos3 ( ) );
        m_C_Uzis.EnableWindow ( CTRSaveGame::I()->HasAmmos3 ( ) );
        m_ZeroUzi.EnableWindow ( CTRSaveGame::I()->HasAmmos3 ( ) );

        m_Ammos_Riot_Gun.EnableWindow ( CTRSaveGame::I()->HasAmmos4 ( ) );
        m_Ammos_Riot_Gun2.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasAmmos4 ( ) );
        m_C_Riot_Gun.EnableWindow ( CTRSaveGame::I()->HasAmmos4 ( ) );
        m_ZeroRiotGun.EnableWindow ( CTRSaveGame::I()->HasAmmos4 ( ) );

        m_Ammos_MP5.EnableWindow ( CTRSaveGame::I()->HasAmmos5 ( ) );
        m_C_MP5.EnableWindow ( (iVersion >= 20 ) && CTRSaveGame::I()->HasAmmos5 ( ) );
        m_ZeroMP5.EnableWindow ( CTRSaveGame::I()->HasAmmos5 ( ) );

        m_Ammos_Rocket_Launcher.EnableWindow ( CTRSaveGame::I()->HasAmmos6 ( ) );
        m_C_Rocket_Launcher.EnableWindow ( ( iVersion >= 30 ) && CTRSaveGame::I()->HasAmmos6 ( ) );
        m_ZeroRocket.EnableWindow ( CTRSaveGame::I()->HasAmmos6 ( ) );

        m_Ammos_Grenade_Launcher.EnableWindow ( CTRSaveGame::I()->HasAmmos7 ( ) );
        m_Ammos_Grenade_Launcher2.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasAmmos7 ( ) );
        m_Ammos_Grenade_Launcher3.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasAmmos7 ( ) );
        m_C_Grenade_Launcher.EnableWindow ( (iVersion >= 20 ) && CTRSaveGame::I()->HasAmmos7 ( ) );
        m_ZeroGrenade.EnableWindow ( CTRSaveGame::I()->HasAmmos7 ( ) );

        m_Ammos_Harpoon.EnableWindow ( CTRSaveGame::I()->HasAmmos8 ( ) );
        m_Ammos_Harpoon2.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasAmmos8 ( ) );
        m_Ammos_Harpoon3.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasAmmos8 ( ) );
        m_C_Harpoon.EnableWindow ( (iVersion >= 20 ) && CTRSaveGame::I()->HasAmmos8 ( ) );
        m_ZeroHarpoon.EnableWindow ( CTRSaveGame::I()->HasAmmos8 ( ) );

        // m_Guns.EnableWindow ( CTRSaveGame::I()->HasAmmos1( ) );

        m_Unlimited.EnableWindow ( TRUE );
    }
    else
    {
        m_Ammos_Guns.EnableWindow ( FALSE );

        m_Ammos_Rocket_Launcher.EnableWindow ( FALSE );

        m_Ammos_Riot_Gun.EnableWindow ( FALSE );
        m_Ammos_Riot_Gun2.EnableWindow ( FALSE );

        m_Ammos_Harpoon.EnableWindow ( FALSE );
        m_Ammos_Harpoon2.EnableWindow ( FALSE );
        m_Ammos_Harpoon3.EnableWindow ( FALSE );

        m_Ammos_MP5.EnableWindow ( FALSE );

        m_Ammos_Grenade_Launcher.EnableWindow ( FALSE );
        m_Ammos_Grenade_Launcher2.EnableWindow ( FALSE );
        m_Ammos_Grenade_Launcher3.EnableWindow ( FALSE );


        m_Ammos_Desert_Eagle.EnableWindow ( FALSE );

        m_Ammos_Uzis.EnableWindow ( FALSE );

        // m_Guns.EnableWindow ( FALSE );
        m_C_Guns.EnableWindow ( FALSE );
        m_C_Harpoon.EnableWindow ( FALSE );
        m_C_MP5.EnableWindow ( FALSE );
        m_C_Desert_Eagle.EnableWindow ( FALSE );
        m_C_Uzis.EnableWindow ( FALSE );
        m_C_Riot_Gun.EnableWindow ( FALSE );
        m_C_Rocket_Launcher.EnableWindow ( FALSE );
        m_C_Grenade_Launcher.EnableWindow ( FALSE );

        m_Unlimited.EnableWindow ( FALSE );
    }

    //
    switch ( iVersion )
    {
        case 10 :
        {
            break;
        }
        case 15 :
        {
            break;
        }
        case 20 :
        {
            break;
        }
        case 30 :
        {
            break;
        }
    }

    SetGUIModified ( bModified );

    return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCguns()
{
    m_Ammos_Guns.SetWindowText (pValueMinusOne );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCrocketlauncher()
{
    //
    m_Ammos_Rocket_Launcher.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCuzis()
{
    //
    m_Ammos_Uzis.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCmp5()
{
    //
    m_C_MP5.SetCheck ( FALSE );
    m_Ammos_MP5.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCdeserteagle()
{
    //
    m_Ammos_Desert_Eagle.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCgrenadelauncher()
{
    //
    m_Ammos_Grenade_Launcher.SetWindowText ( pValue10K );
    m_Ammos_Grenade_Launcher2.SetWindowText ( pValue10K );
    m_Ammos_Grenade_Launcher3.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCriotgun()
{
    //
    m_Ammos_Riot_Gun.SetWindowText ( pSome5K );
    m_Ammos_Riot_Gun2.SetWindowText ( pSome5K);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCharpoon()
{
    //
    m_Ammos_Harpoon.SetWindowText ( pValue10K );
    m_Ammos_Harpoon2.SetWindowText ( pValue10K );
    m_Ammos_Harpoon3.SetWindowText ( pValue10K );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedUnlimited()
{
    //
    if ( CTRSaveGame::I() != NULL )
    {
        if ( CTRSaveGame::GetVersion() >= 40 )
        {
            SetGUIModified ( TRUE, "Ammos Unlimited" );

            m_Ammos_Uzis.SetWindowText ( pValueMinusOne );

            m_Ammos_MP5.SetWindowText ( pValueMinusOne );

            m_Ammos_Harpoon.SetWindowText ( pValueMinusOne );
            m_Ammos_Harpoon2.SetWindowText ( pValueMinusOne );
            m_Ammos_Harpoon3.SetWindowText ( pValueMinusOne );

            m_Ammos_Grenade_Launcher.SetWindowText ( pValueMinusOne );
            m_Ammos_Grenade_Launcher2.SetWindowText ( pValueMinusOne );
            m_Ammos_Grenade_Launcher3.SetWindowText ( pValueMinusOne );

            m_Ammos_Rocket_Launcher.SetWindowText ( pValueMinusOne );

            m_Ammos_Riot_Gun.SetWindowText ( pValueMinusOne );
            m_Ammos_Riot_Gun2.SetWindowText ( pValueMinusOne);

            m_Ammos_Desert_Eagle.SetWindowText ( pValueMinusOne );

            m_Ammos_Guns.SetWindowText ( pValueMinusOne );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXAmmosPage::OnInitDialog()
{
    CTRXPropertyPage::OnInitDialog();

    //
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_Status, ("Status"));
        m_ToolTip.Activate(TRUE);
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnChangeEdit()
{
    SetGUIModified ( TRUE, "Ammos ChangeEdit" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnCancel()
{
    //
    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        INT_PTR response = AskToSave("Ammos");
        DisplayValues ( );
    }

    CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnOK()
{
    //
    INT_PTR response = AskToSave("Ammos");

    CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXAmmosPage::OnApply()
{
    //
    WriteWhenOnApply("Ammos");

    return CTRXPropertyPage::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZriotgun()
{
    m_Ammos_Riot_Gun.SetWindowText ( "0" );
    m_Ammos_Riot_Gun2.SetWindowText ( "0");
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZmp5()
{
    m_Ammos_MP5.SetWindowText ( "0" );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZharpoon()
{
    m_Ammos_Harpoon.SetWindowText ( "0" );
    m_Ammos_Harpoon2.SetWindowText ( "0" );
    m_Ammos_Harpoon3.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZdesert()
{
    m_Ammos_Desert_Eagle.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZgrenade()
{
    m_Ammos_Grenade_Launcher.SetWindowText ( "0" );
    m_Ammos_Grenade_Launcher2.SetWindowText ( "0" );
    m_Ammos_Grenade_Launcher3.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZuzi()
{
    m_Ammos_Uzis.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZrocket()
{
    m_Ammos_Rocket_Launcher.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedZguns()
{
    m_Ammos_Guns.SetWindowText ( "0" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAmmosPage::OnBnClickedAllAmmos()
{
    //
    if ( CTRSaveGame::I() != NULL  )
    {
        if ( CTRSaveGame::GetVersion() != 0 )
        {
            SetGUIModified ( TRUE, "All Ammos" );

            if ( m_Ammos_Uzis.IsWindowEnabled() )
            {
                m_Ammos_Uzis.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_MP5.IsWindowEnabled() )
            {
                m_Ammos_MP5.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_Harpoon.IsWindowEnabled() )
            {
                m_Ammos_Harpoon.SetWindowText ( pValue10K );
                m_Ammos_Harpoon2.SetWindowText ( pValue10K );
                m_Ammos_Harpoon3.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_Grenade_Launcher.IsWindowEnabled() )
            {
                m_Ammos_Grenade_Launcher.SetWindowText ( pValue10K );
                m_Ammos_Grenade_Launcher2.SetWindowText ( pValue10K );
                m_Ammos_Grenade_Launcher3.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_Rocket_Launcher.IsWindowEnabled() )
            {
                m_Ammos_Rocket_Launcher.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_Riot_Gun.IsWindowEnabled() )
            {
                m_Ammos_Riot_Gun.SetWindowText ( pValue10K );
                m_Ammos_Riot_Gun2.SetWindowText ( pValue10K);
            }

            if ( m_Ammos_Desert_Eagle.IsWindowEnabled() )
            {
                m_Ammos_Desert_Eagle.SetWindowText ( pValue10K );
            }

            if ( m_Ammos_Guns.IsWindowEnabled() )
            {
                m_Ammos_Guns.SetWindowText ( pValueMinusOne );
            }
        }

        //
        if ( m_iHitCountForAll > 0 )
        {
        }

        m_iHitCountForAll++;
    }
}
