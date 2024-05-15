// TR3AmmosPage.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXGunPage.h"

#include "GunGrids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
static const	char *MessageTitle = "Tombraider Standard Editions";

//
/////////////////////////////////////////////////////////////////////////////
// CTRXGunPage property page
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTRXGunPage, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXGunPage::CTRXGunPage() : CTRXPropertyPage(CTRXGunPage::IDD)
{
	//{{AFX_DATA_INIT(CTRXGunPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXGunPage::~CTRXGunPage()
{
	CTRSaveGame *pGame = CTRSaveGame::I(FALSE);
	if ( pGame != NULL )
	{
		delete pGame;
	}
}

void CTRXGunPage::DoDataExchange(CDataExchange* pDX)
{
	CTRXPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTRXGunPage)
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_UZI, m_Uzis);
	DDX_Control(pDX, IDC_ROCKETLAUNCHER, m_Rocket_Launcher);
	DDX_Control(pDX, IDC_RIOTGUN, m_Riot_Gun);
	DDX_Control(pDX, IDC_MP5, m_MP5);
	DDX_Control(pDX, IDC_HARPOON, m_Harpoon);
	DDX_Control(pDX, IDC_GUNS, m_Guns);
	DDX_Control(pDX, IDC_GRENADELAUNCHER, m_Grenade_Launcher);
	DDX_Control(pDX, IDC_DESERTEAGLE, m_Desert_Eagle);
	DDX_Control(pDX, IDC_CROWBAR, m_Crowbar);
	DDX_Control(pDX, IDC_BM_GUNS, m_BM_Guns);
	DDX_Control(pDX, IDC_BM_DESERTEAGLE, m_BM_DesertEagle);
	DDX_Control(pDX, IDC_BM_GRENADE, m_BM_Grenade);
	DDX_Control(pDX, IDC_BM_UZIS, m_BM_Uzis);
	DDX_Control(pDX, IDC_BM_HARPOON, m_BM_Harpoon);
	DDX_Control(pDX, IDC_BM_MP5, m_BM_MP5);
	DDX_Control(pDX, IDC_BM_ROCKET, m_BM_Rocket);
	DDX_Control(pDX, IDC_BM_RIOTGUN, m_BM_RiotGun);
	//}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXGunPage, CTRXPropertyPage)
	//{{AFX_MSG_MAP(CTRXGunPage)
	ON_BN_CLICKED(IDC_DESERTEAGLE, &CTRXGunPage::OnBnClickedDeserteagle)
	ON_BN_CLICKED(IDC_GRENADELAUNCHER, &CTRXGunPage::OnBnClickedGrenadelauncher)
	ON_BN_CLICKED(IDC_GUNS, &CTRXGunPage::OnBnClickedGuns)
	ON_BN_CLICKED(IDC_UZI, &CTRXGunPage::OnBnClickedUzi)
	ON_BN_CLICKED(IDC_RIOTGUN, &CTRXGunPage::OnBnClickedRiotgun)
	ON_BN_CLICKED(IDC_HARPOON, &CTRXGunPage::OnBnClickedHarpoon)
	ON_BN_CLICKED(IDC_MP5, &CTRXGunPage::OnBnClickedMp5)
	ON_BN_CLICKED(IDC_ROCKETLAUNCHER, &CTRXGunPage::OnBnClickedRocketlauncher)
	ON_BN_CLICKED(IDC_CROWBAR, &CTRXGunPage::OnBnClickedCrowbar)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXGunPage message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CTRXHelpDialog dlg;
	dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::DisplayValues()
{
	BOOL bModified = IsGUIModified();

	int		iX;

	if ( CTRSaveGame::I() != NULL )
	{	
		m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus () );
	}

	EnableForVersion ();

	if ( CTRSaveGame::IsValid() )
	{
		//
		int level	= CTRSaveGame::GetLevelIndex();

		//
		//	Set Labels
		//
		//		Weapons Order
		//		iMaskCompass			: Weapon0
		//		iMaskPistol				: Weapon1
		//		iMaskDesertEagle		: Weapon2
		//		iMaskUzi				: Weapon3
		//		iMaskShotGun			: Weapon4
		//		iMaskMP5				: Weapon5
		//		iMaskRocket				: Weapon6
		//		iMaskGrenade			: Weapon7
		//		iMaskHarpoon			: Weapon8
		m_Guns.SetWindowTextA( CTRSaveGame::I()->GetLabel1() );
		SetBitmap ( &m_BM_Guns, 1, CTRSaveGame::I()->GetBitmap1() );

		m_Desert_Eagle.SetWindowTextA( CTRSaveGame::I()->GetLabel2() );
		SetBitmap ( &m_BM_DesertEagle, 2, CTRSaveGame::I()->GetBitmap2() );

		m_Uzis.SetWindowTextA( CTRSaveGame::I()->GetLabel3() );
		SetBitmap ( &m_BM_Uzis, 3, CTRSaveGame::I()->GetBitmap3() );

		m_Riot_Gun.SetWindowTextA( CTRSaveGame::I()->GetLabel4() );
		SetBitmap ( &m_BM_RiotGun, 4, CTRSaveGame::I()->GetBitmap4() );

		m_MP5.SetWindowTextA( CTRSaveGame::I()->GetLabel5() );
		SetBitmap ( &m_BM_MP5, 5, CTRSaveGame::I()->GetBitmap5() );

		m_Rocket_Launcher.SetWindowTextA( CTRSaveGame::I()->GetLabel6() );
		SetBitmap ( &m_BM_Rocket, 6, CTRSaveGame::I()->GetBitmap6() );

		m_Grenade_Launcher.SetWindowTextA( CTRSaveGame::I()->GetLabel7() );
		SetBitmap ( &m_BM_Grenade, 7, CTRSaveGame::I()->GetBitmap7() );

		m_Harpoon.SetWindowTextA( CTRSaveGame::I()->GetLabel8() );
		SetBitmap ( &m_BM_Harpoon, 8, CTRSaveGame::I()->GetBitmap8() );

		m_Crowbar.SetWindowTextA ( CTRSaveGame::I()->GetLabel9() );

		/*
	 	 *		Get gun state.
		 */
		iX = CTRSaveGame::GetLevelIndex ();


		m_Guns.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon1 ( iX ) )
		{
			m_Guns.SetCheck ( 1 );
		}

		m_Desert_Eagle.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon2 ( iX ) )
		{
			m_Desert_Eagle.SetCheck ( 1 );
		}

		m_Uzis.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon3 ( iX ) )
		{
			m_Uzis.SetCheck ( 1 );
		}

		m_Riot_Gun.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon4 ( iX ) )
		{
			m_Riot_Gun.SetCheck ( 1 );
		}

		m_MP5.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon5 ( iX ) )
		{
				m_MP5.SetCheck ( 1 );
		}

		m_Grenade_Launcher.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon7 ( iX ) )
		{
				m_Grenade_Launcher.SetCheck ( 1 );
		}

		m_Rocket_Launcher.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon6 ( iX ) )
		{
				m_Rocket_Launcher.SetCheck ( 1 );
		}

		m_Harpoon.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon8 ( iX ) )
		{
				m_Harpoon.SetCheck ( 1 );
		}

		m_Crowbar.SetCheck ( 0 );
		if ( CTRSaveGame::I()->CheckWeapon9 ( iX ) )
		{
			m_Crowbar.SetCheck ( 1 );
		}
	}

	SetGUIModified ( bModified );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXGunPage::OnInitDialog() 
{
	CTRXPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_bToolTip )
	{
		m_ToolTip.AddTool( &m_Status, ("Status"));
		m_ToolTip.Activate(TRUE);
	}

	m_bInitDone	= true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXGunPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
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
void CTRXGunPage::UpdateBuffer()
{
	int						iX;

	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		iX = CTRSaveGame::GetLevelIndex ();

		//
		CTRSaveGame::I()->GrabWeapon0 ( iX );

		//
		if ( m_Guns.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon1 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon1 ( iX, false );
		}

		if ( m_Desert_Eagle.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon2 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon2 ( iX, false );
		}

		if ( m_Uzis.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon3 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon3 ( iX, false );
		}

		if ( m_Riot_Gun.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon4 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon4 ( iX, false );
		}

		if ( m_MP5.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon5 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon5 ( iX, false );
		}

		if ( m_Rocket_Launcher.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon6 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon6 ( iX, false );
		}

		if ( m_Grenade_Launcher.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon7 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon7 ( iX, false );
		}

		if ( m_Harpoon.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon8 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon8 ( iX, false );
		}

		if ( m_Crowbar.GetCheck ( ) )
		{
			CTRSaveGame::I()->GrabWeapon9 ( iX );
		}
		else
		{
			CTRSaveGame::I()->GrabWeapon9 ( iX, false );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXGunPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateBuffer();

	BOOL bModified = IsGUIModified();

	return CTRXPropertyPage::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXGunPage::EnableForVersion()
{
	BOOL bModified = IsGUIModified();

	int iVersion = CTRXPropertyPage::EnableForVersion();

	if ( CTRSaveGame::I() != NULL && iVersion != 0 )
	{	
		m_Guns.EnableWindow ( CTRSaveGame::I()->HasWeapon1 ( ));
		m_Desert_Eagle.EnableWindow ( CTRSaveGame::I()->HasWeapon2 ( ) );
		m_Uzis.EnableWindow ( CTRSaveGame::I()->HasWeapon3 ( ) );
		m_Riot_Gun.EnableWindow ( CTRSaveGame::I()->HasWeapon4 ( ) );
		m_MP5.EnableWindow ( ( iVersion >= 20 ) && CTRSaveGame::I()->HasWeapon5 ( ) );
		m_Rocket_Launcher.EnableWindow ( ( iVersion >= 30 ) && CTRSaveGame::I()->HasWeapon6 ( ) );
		m_Grenade_Launcher.EnableWindow ( (iVersion >= 20 ) && CTRSaveGame::I()->HasWeapon7 ( ) );
		m_Harpoon.EnableWindow ( ( iVersion >= 20 ) && CTRSaveGame::I()->HasWeapon8 ( ) );
		m_Crowbar.EnableWindow ( ( iVersion >= 40 ) && CTRSaveGame::I()->HasWeapon9 ( ) );
	}
	else
	{
		m_Guns.EnableWindow ( FALSE );
		m_Desert_Eagle.EnableWindow ( FALSE );
		m_Uzis.EnableWindow ( FALSE );
		m_Riot_Gun.EnableWindow ( FALSE );
		m_MP5.EnableWindow ( FALSE );
		m_Rocket_Launcher.EnableWindow ( FALSE );
		m_Grenade_Launcher.EnableWindow ( FALSE );
		m_Harpoon.EnableWindow ( FALSE );
		m_Crowbar.EnableWindow ( FALSE );
	}

	SetGUIModified ( bModified );

	//
	return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedDeserteagle()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedGrenadelauncher()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedGuns()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedUzi()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedRiotgun()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedHarpoon()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedMp5()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedRocketlauncher()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnBnClickedCrowbar()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnOK()
{
	// TODO: Add Your handler here
	INT_PTR response = AskToSave("Gun");
	DisplayValues ( );

	CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXGunPage::OnCancel()
{
	// TODO: Add Your handler here
	if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
	{
		INT_PTR response = AskToSave("Gun");
	}

	CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXGunPage::OnApply()
{
	// TODO: Add Code Here
	WriteWhenOnApply("Guns");

	return CTRXPropertyPage::OnApply();
}

