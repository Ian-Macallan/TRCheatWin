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

static const char *pValueMinusOne	= "-1";
static const char *pValue10K		= "10000";
static const char *pSome5K			= "5000";
static const char *pSome1K			= "1000";
static const char *pValue255		= "255";
static const char *pMany32K			= "0x7fff";

static const int Many32K			= 0x7fff;

//
static const	char *MessageTitle = "Tombraider Standard Editions";

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
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXEquipmentPage message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnHelp() 
{
	// TODO: Add your control notification handler code here
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
	int		iX;
	int		iVal;

	if ( CTRSaveGame::I() != NULL )
	{
		m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus () );
	}

	EnableForVersion ();

	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		iX = CTRSaveGame::GetLevelIndex ();

		/*
		 *      Get current values for ammos.
		 */
		sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetGunAmmos ( ) );
		m_GunAmmos.SetWindowText ( szString );

		iVal = CTRSaveGame::I()->GetLife ( );
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
				m_God.SetCheck ( 1 );
			}
			else
			{
				m_God.SetCheck ( 0 );
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

		m_Unfinite_Air.SetCheck ( 0 );
		if ( CTRSaveGame::I()->GetAir () == Many32K )
		{
			m_Unfinite_Air.SetCheck ( 1 );
		}

		//
		m_Laser.SetCheck ( 0 );
		if ( CTRSaveGame::I()->GetLaser ( 0 ) != 0 )
		{
			m_Laser.SetCheck ( 1 );
		}

		//
		m_Binocular.SetCheck ( 0 );
		if ( CTRSaveGame::I()->GetBinocular ( 0 ) != 0 )
		{
			m_Binocular.SetCheck ( 1 );
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
void CTRXEquipmentPage::UpdateBuffer()
{
	int						iX;

	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		iX = CTRSaveGame::GetLevelIndex ();

		/*
		 *      Get current values for ammos.
		 */
		CTRSaveGame::I()->SetSmallMedipak ( GetValue ( m_Small_Medipak ), iX );

		CTRSaveGame::I()->SetLargeMedipak ( GetValue ( m_Large_Medipak ), iX );

		CTRSaveGame::I()->SetFlares ( GetValue ( m_Flares ), iX );

		/*
		 *		Set air.
		 */
		CTRSaveGame::I()->SetAir ( GetValue ( m_Air ) );

		/*
		 *		Set Health.
		 */
		CTRSaveGame::I()->SetGunAmmos ( GetValue ( m_GunAmmos ) );

		/*
		 *		Set Life.
		 */
		CTRSaveGame::I()->SetLife ( GetValue ( m_Life ) );

		//	Binocular
		CTRSaveGame::I()->SetBinocular ( GetValue ( m_Binocular ) );

		//	Laser
		CTRSaveGame::I()->SetLaser ( GetValue ( m_Laser ) );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEquipmentPage::OnKillActive() 
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
int CTRXEquipmentPage::EnableForVersion()
{
	BOOL bModified = IsGUIModified();

	int iVersion =  CTRXPropertyPage::EnableForVersion();

	//		m_Air.EnableWindow ( bEnable2 );
	if ( iVersion != 0 ) 
	{
		m_Flares.EnableWindow ( iVersion >= 20 );
		m_But_Flares.EnableWindow ( iVersion >= 20 ); 

		m_Laser.EnableWindow ( iVersion >= 40 );
		m_Binocular.EnableWindow ( iVersion >= 40 );

		m_Life.EnableWindow ( iVersion <= 30 || iVersion == 50 );
		m_GunAmmos.EnableWindow ( iVersion <= 30 );
		m_Unfinite_Air.EnableWindow ( iVersion <= 30 || iVersion == 40 || iVersion == 45 || iVersion == 49 || iVersion == 50 );
		m_Air.EnableWindow ( iVersion <= 30 || iVersion == 40 || iVersion == 45 || iVersion == 49 || iVersion == 50 );
		m_God.EnableWindow ( iVersion <= 30 );
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

	char		szString [ 32 ];
	// TODO: Add your control notification handler code here
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
	// TODO: Add your control notification handler code here
	SetGUIModified ( TRUE, "Equipment Smedi" );
	m_But_Small_MediPak.SetCheck ( 0 );
	if ( CTRSaveGame::I() != NULL && CTRSaveGame::GetVersion () >= 40 )
	{
		m_Small_Medipak.SetWindowText ( pValue10K );
	}
	else
	{
		m_Small_Medipak.SetWindowText ( pValue255 );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnButLmedi() 
{
	// TODO: Add your control notification handler code here
	SetGUIModified ( TRUE, "Equipment Lmedi" );

	m_But_Large_MediPak.SetCheck ( 0 );
	if ( CTRSaveGame::I() != NULL && CTRSaveGame::GetVersion () >= 40 )
	{
		m_Large_Medipak.SetWindowText ( pValue10K );
	}
	else
	{
		m_Large_Medipak.SetWindowText ( pValue255 );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnButFlares() 
{
	// TODO: Add your control notification handler code here
	int		iX;

	SetGUIModified ( TRUE );

	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		iX = CTRSaveGame::GetLevelIndex ();

		if ( CTRSaveGame::GetVersion () >= 40 )
		{
			m_Flares.SetWindowText ( pValue10K );
		}
		else
		{
			m_Flares.SetWindowText ( pValue255 );
		}

	}
	m_But_Flares.SetCheck ( 0 );
	
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXEquipmentPage::OnGod() 
{
	SetGUIModified ( TRUE );

    char					szString [ 32 ];

	// TODO: Add your control notification handler code here
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

	// TODO: Add Code Here
	if ( m_bToolTip )
	{
		m_ToolTip.AddTool( &m_Status, ("Status"));
		m_ToolTip.Activate(TRUE);
	}

	m_bInitDone	= true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
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
	// TODO: Add Your handler here
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
	// TODO: Add Your handler here
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

