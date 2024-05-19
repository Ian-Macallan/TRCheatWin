// TR3InfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXInfoPage.h"
#include "TRXDifferences.h"
#include "TRXMapAreas.h"
#include "TRXMessageBox.h"
#include "ReadTR2\ReadTRX.h"
#include "ReadTR2\\ReadTRXScript.h"
#include "TRLabelItems.h"

#include "AutomaticVersionHeader.h"

#include <direct.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
static const char *MessageTitle	= "Tombraider Standard Editions";
static const int RoomDivider	= 150;
static const int RoomMargin		= 5;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

extern CTRXCHEATWINApp theApp;

static	int 			iAscending		= 1;		
static	const	int		maxData			= 1000;
typedef	struct	structData
{
	char	szText [ MAX_PATH ];
} STRUCTDATA;

//
static	STRUCTLOCATION	LocationPathname [ LEN_LOCATION ];

//
static	STRUCTLOCATION	CustomPathnames1 [ LEN_LOCATION ];
static	STRUCTLOCATION	CustomPathnames2 [ LEN_LOCATION ];
static	STRUCTLOCATION	CustomPathnames3 [ LEN_LOCATION ];
static	STRUCTLOCATION	CustomPathnames4 [ LEN_LOCATION ];
static	STRUCTLOCATION	CustomPathnames5 [ LEN_LOCATION ];

//
typedef struct CustomDataFilesStruct
{
	char datafile [ 64 ];
	char title [ 128 ];
} CustomDataFilesType;

static CustomDataFilesType CustomDataFiles [ TR4NGMAXLEVEL ];

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static	STRUCTDATA tagData [ maxData ];

static const int ValueMinusOne		= -1;
static const int Value10K			= 10000;
static const int Many32K			= 0x7fff;
static const int Some5K				= 5000;
static const int Value255			= 255;
static const int Value1000			= 1000;
static const int Value1800			= 1800;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  	LPCSTR	pData1 = (LPCSTR)lParam1;
  	LPCSTR	pData2 = (LPCSTR)lParam2;
	DWORD	dwData = *( DWORD *)lParamSort;

	if ( pData1 != NULL && pData2 != NULL )
	{
		if ( dwData == COL_FILENAME  && strchr ( pData1, '.' ) != NULL && strchr ( pData2, '.' ) != NULL )
		{
			int Val1 = atoi ( strchr ( pData1, '.' ) + 1 );
			int Val2 = atoi ( strchr ( pData2, '.' ) + 1 );
			return iAscending * ( Val1 - Val2 );
		}

		return iAscending * strcmp ( pData1, pData2 );
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int SortLocation(const void *pVoid1, const void *pVoid2)
{
  	STRUCTLOCATION	*pData1 = (STRUCTLOCATION*)pVoid1;
  	STRUCTLOCATION	*pData2 = (STRUCTLOCATION*)pVoid2;

	int iResult = 0;
	if ( pData1 != NULL && pData2 != NULL )
	{
		if ( strlen(pData1->szPathname) == 0 && strlen(pData1->szPathname) == 0 )
		{
			return 0;
		}
		if ( strlen(pData1->szPathname) == 0 )
		{
			return 1;
		}
		if ( strlen(pData2->szPathname) == 0 )
		{
			return -1;
		}
		iResult = strcmp(pData1->szPathname, pData2->szPathname );
	}

	return iResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//	what = 0 for items labels
/////////////////////////////////////////////////////////////////////////////
static void AddToItemsLabels ( int what, int level, int button, const char *pText, const char *pTitle )
{
	//	What = 0, level = level, button = button, pText = item label
	if ( what == 0 )
	{
		if ( button >= 0 && button < NB_BUTTONS && pText != NULL )
		{
			if ( level == -1 )
			{
				if ( TR49ItemsNameGen [ button ] != NULL )
				{
					free ( TR49ItemsNameGen [ button ] );
					TR49ItemsNameGen [ button ] = NULL;
				}

				TR49ItemsNameGen [ button ] = ( char * ) malloc ( strlen(pText) + 1 );
				strcpy_s ( TR49ItemsNameGen [ button ], strlen(pText) + 1, pText );
			}
			else if ( level >= 0 && level < TR4NGMAXLEVEL )
			{
				if ( TR49ItemsNameInd [ level ] [ button ] != NULL )
				{
					free ( TR49ItemsNameInd [ level ] [ button ] );
					TR49ItemsNameInd [ level ] [ button ] = NULL;
				}
				TR49ItemsNameInd [ level ] [ button ] = ( char * ) malloc ( strlen(pText) + 1 );
				strcpy_s ( TR49ItemsNameInd [ level ] [ button ], strlen(pText) + 1, pText );
			}
		}
	}
	//	What = 1, level = levelindex, button = 1 is title, 0 is level, pText = DATA/LEVEL
	else if ( what == 1 )
	{
		if ( level >= 0 && level < TR4NGMAXLEVEL && pText != NULL && pTitle != NULL )
		{
			strcpy_s ( CustomDataFiles [ level ].datafile, sizeof(CustomDataFiles [ level ].datafile), pText );
			strcpy_s ( CustomDataFiles [ level ].title, sizeof(CustomDataFiles [ level ].title), pTitle );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int SearchDataFileIndex ( const char *pSavename )
{
	for ( int i = 0; i < TR4NGMAXLEVEL; i++ )
	{
		if ( _strcmpi ( pSavename, CustomDataFiles [ i ].title ) == 0 )
		{
			return i;
		}
	}

	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
// CTRXInfoPage property page
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTRXInfoPage, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXInfoPage::CTRXInfoPage() : CTRXPropertyPage(CTRXInfoPage::IDD)
{
	//{{AFX_DATA_INIT(CTRXInfoPage)
	//}}AFX_DATA_INIT
	m_bFirstTime				= true;

	m_Line						= 0;
	m_Custom_Selected			= -1;

	m_pBitmapLevel				= NULL;
	m_pBitmapContinent			= NULL;

	m_hFirstBitmapLevel			= NULL;
	m_hFirstBitmapContinent		= NULL;

	m_pListCtrl					= NULL;

	m_bRoomCreated				= false;

	for ( int i = 0; i < LEN_LOCATION; i++ )
	{
		ZeroMemory ( LocationPathname [ i ].szPathname, sizeof(LocationPathname [ i ].szPathname) );
		ZeroMemory ( CustomPathnames1 [ i ].szPathname, sizeof(CustomPathnames1 [ i ].szPathname) );
		ZeroMemory ( CustomPathnames2 [ i ].szPathname, sizeof(CustomPathnames2 [ i ].szPathname) );
		ZeroMemory ( CustomPathnames3 [ i ].szPathname, sizeof(CustomPathnames3 [ i ].szPathname) );
		ZeroMemory ( CustomPathnames4 [ i ].szPathname, sizeof(CustomPathnames4 [ i ].szPathname) );
		ZeroMemory ( CustomPathnames5 [ i ].szPathname, sizeof(CustomPathnames5 [ i ].szPathname) );
	}

	//
	//
	LoadLocation ( LocationPathname, "Last Location Count", "Last Location %02d" );

	//
	LoadLocation ( CustomPathnames1, "Last Custom TR1 Count", "Last Custom TR1 %02d" );
	LoadLocation ( CustomPathnames2, "Last Custom TR2 Count", "Last Custom TR2 %02d" );
	LoadLocation ( CustomPathnames3, "Last Custom TR3 Count", "Last Custom TR3 %02d" );
	LoadLocation ( CustomPathnames4, "Last Custom TR4 Count", "Last Custom TR4 %02d" );
	LoadLocation ( CustomPathnames5, "Last Custom TR5 Count", "Last Custom TR5 %02d" );

	//
	qsort ( CustomPathnames1, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames2, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames3, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames4, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames5, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );

	//
	SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXInfoPage::~CTRXInfoPage()
{

#define	DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

	SaveLocation ( LocationPathname, "Last Location Count", "Last Location %02d" );

	//
	qsort ( CustomPathnames1, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames2, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames3, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames4, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );
	qsort ( CustomPathnames5, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );

	//
	SaveLocation ( CustomPathnames1, "Last Custom TR1 Count", "Last Custom TR1 %02d" );
	SaveLocation ( CustomPathnames2, "Last Custom TR2 Count", "Last Custom TR2 %02d" );
	SaveLocation ( CustomPathnames3, "Last Custom TR3 Count", "Last Custom TR3 %02d" );
	SaveLocation ( CustomPathnames4, "Last Custom TR4 Count", "Last Custom TR4 %02d" );
	SaveLocation ( CustomPathnames5, "Last Custom TR5 Count", "Last Custom TR5 %02d" );

	//
	CTRSaveGame *pGame = CTRSaveGame::I(FALSE);
	if ( pGame != NULL )
	{
		delete pGame;
	}

	DELETE_OBJECT(m_pBitmapLevel);
	DELETE_OBJECT(m_pBitmapContinent);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CTRXPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTRXInfoPage)
	DDX_Control(pDX, IDC_LISTCTRL, m_ListCtrl);
	DDX_Control(pDX, IDC_LEVEL_PLACE, m_ImageContinent);
	DDX_Control(pDX, IDC_LEVEL_PICTURE, m_ImageLevel);
	DDX_Control(pDX, IDC_LEVELNUMBER, m_Levelnumber);
	DDX_Control(pDX, IDC_FILENAME, m_Filename);
	DDX_Control(pDX, IDC_SAVENUMBER, m_Savenumber);
	DDX_Control(pDX, IDC_LOAD, m_LoadDirectory);
	DDX_Control(pDX, IDC_WRITE, m_Write);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_LARABURNING, m_Lara_Burning);
	DDX_Control(pDX, IDC_NBSECRETS, m_Nb_Secrets);
	DDX_Control(pDX, IDC_SECRETS, m_Secrets);
	DDX_Control(pDX, IDC_LEVELNAME, m_Level_Name);
	DDX_Control(pDX, IDC_LARA, m_Lara);
	DDX_Control(pDX, IDC_MAX, m_Max);
	DDX_Control(pDX, IDC_REFRESH, m_Refresh);
	DDX_Control(pDX, IDC_DIRECTORY, m_Combo);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDC_JOURNAL, m_Journal);
	DDX_Control(pDX, IDC_SECRET_ALL, m_Secret_All);
	DDX_Control(pDX, IDC_MAP, m_Map);
	DDX_Control(pDX, IDC_ROOM, m_Area);
	DDX_Control(pDX, IDC_DIRECTION, m_Direction);
	DDX_Control(pDX, IDC_WEST_EAST, m_West_East);
	DDX_Control(pDX, IDC_VERTICAL, m_Vertical);
	DDX_Control(pDX, IDC_SOUTH_NORTH, m_South_North);
	DDX_Control(pDX, IDC_POSITION, m_Position);
	DDX_Control(pDX, IDC_CUSTOM_COMBO, m_Custom_Combo);
	DDX_Control(pDX, IDC_REMOVE_CUSTOM, m_Remove_Custom);
	DDX_Control(pDX, IDC_ADD_CUSTOM, m_Add_Custom);
	DDX_Control(pDX, IDC_SEE_CUSTOM, m_See_Custom);
	DDX_Control(pDX, IDC_SORT, m_Sort);
	//}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXInfoPage, CTRXPropertyPage)
	//{{AFX_MSG_MAP(CTRXInfoPage)
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL, OnClickListctrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnItemchangedListctrl)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CTRXInfoPage::OnItemclickListctrl)
	ON_BN_CLICKED(IDC_MAX, &CTRXInfoPage::OnBnClickedMax)
	ON_BN_CLICKED(IDC_REFRESH, &CTRXInfoPage::OnBnClickedRefresh)
	ON_CBN_SELCHANGE(IDC_DIRECTORY, &CTRXInfoPage::OnSelchangeCombo)
	ON_BN_CLICKED(IDC_REMOVE, &CTRXInfoPage::OnBnClickedRemove)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	ON_BN_CLICKED(IDC_LARABURNING, &CTRXInfoPage::OnBnClickedLaraburning)
	ON_BN_CLICKED(IDC_JOURNAL, &CTRXInfoPage::OnBnClickedJournal)
	ON_BN_CLICKED(IDC_SECRET_ALL, &CTRXInfoPage::OnBnClickedSecretAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MAP, &CTRXInfoPage::OnBnClickedMap)
	ON_EN_CHANGE(IDC_ROOM, &CTRXInfoPage::OnChangeRoom)
	ON_EN_CHANGE(IDC_DIRECTION, &CTRXInfoPage::OnChangeOrientation)
	ON_EN_CHANGE(IDC_WEST_EAST, &CTRXInfoPage::OnChangeWestEast)
	ON_EN_CHANGE(IDC_VERTICAL, &CTRXInfoPage::OnChangeEdit)
	ON_EN_CHANGE(IDC_SOUTH_NORTH, &CTRXInfoPage::OnChangeSouthNorth)
	ON_EN_CHANGE(IDC_SECRETS, &CTRXInfoPage::OnChangeEdit)
	ON_EN_CHANGE(IDC_LARA, &CTRXInfoPage::OnChangeEdit)
	ON_BN_CLICKED(IDC_ADD_CUSTOM, &CTRXInfoPage::OnBnClickedAddCustom)
	ON_BN_CLICKED(IDC_REMOVE_CUSTOM, &CTRXInfoPage::OnBnClickedRemoveCustom)
	ON_CBN_SELCHANGE(IDC_CUSTOM_COMBO, &CTRXInfoPage::OnSelchangeCustomCombo)
	ON_CBN_DROPDOWN(IDC_CUSTOM_COMBO, &CTRXInfoPage::OnDropdownCustomCombo)
	ON_BN_CLICKED(IDC_SEE_CUSTOM, &CTRXInfoPage::OnBnClickedSeeCustom)
	ON_BN_CLICKED(IDC_SORT, &CTRXInfoPage::OnBnClickedSort)
	END_MESSAGE_MAP()
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// CTRXInfoPage message handlers
void CTRXInfoPage::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CTRXHelpDialog dlg;
	dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::LoadDirectory()
{
	/*
	 *		The Game.
	 */
	char					szGame [ 64 ];

	/*
	 *		The Game save number.
	 */
	int 					iGame;

	/*
	 *		The Title
	 */
	char					szTitle [ 64 ];

	/*
	 *		The Version.
	 */
	int						iVersion;

	//
	static char BASED_CODE szFilter[] = "Savegames|SAVE*.?;SAVE*.??|All Files (*.*)|*.*||";

	char	szDirectory [ MAX_PATH ];
	char	szFilename [ MAX_PATH ];

	ZeroMemory ( szDirectory, sizeof(szDirectory) );
	ZeroMemory ( szFilename, sizeof(szFilename) );

	m_Write.EnableWindow ( FALSE );
	m_Max.EnableWindow ( FALSE );

	CString lastRead  = theApp.GetProfileString( "Settings", "Last Opened", szFilename );
	if ( lastRead != "" )
	{
		strcpy_s ( szDirectory, sizeof(szDirectory), lastRead );
		theApp.RemoveFilename ( szDirectory );
	}

	CFileDialog dlgFileDialog ( TRUE, NULL, "SAVE*.*", NULL, szFilter );
	dlgFileDialog.m_ofn.lpstrInitialDir = szDirectory;

	/*
	 *		Starts a dialog box.
	 */
	if ( dlgFileDialog.DoModal () == IDOK)
	{
		/*
		 *	Set filename Text.
		 */
		m_Filename.SetWindowText ( dlgFileDialog.GetPathName() );
		
		strcpy_s ( szDirectory, sizeof(szDirectory), dlgFileDialog.GetPathName() );
		theApp.RemoveFilename ( szDirectory );

		/*
		 *	Fills list Ctrl.
		 */
		FillListCtrl ( szDirectory );

		/*
		 *	Retrieve informations.
		 */
		m_Filename.GetWindowText ( szFilename, sizeof ( szFilename ) - 1 );
		BOOL bWritten = theApp.WriteProfileString ( "Settings", "Last Opened", szFilename );

		iVersion = CTRSaveGame::InstanciateVersion ( szFilename );
		if ( iVersion != -1 && CTRSaveGame::I() != NULL )
		{
			int iLevel = 0;
			int iSub = 0;
			iVersion = CTRSaveGame::I()->GetInfo ( szGame, sizeof(szGame), &iGame, &iLevel, szTitle, sizeof(szTitle) );
			CTRSaveGame::I()->RetrieveInformation (  szFilename );
			m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus() );

			//
			DisplayValues ();
		}
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnLoad() 
{
	if ( IsGUIModified() || CTRSaveGame::IsBufferModified() )
	{
		UpdateBuffer();
		INT_PTR response = CTRXDifferences::MessageBoxS ( "Do you want to save the file ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
		if ( response == IDYES )
		{
			DisplayOne ( m_Line );
			CTRSaveGame::I()->writeSaveGame();
		}
		else if ( response == IDCANCEL )
		{
			return;
		}
		else
		{
			CTRSaveGame::Discard();
			DisplayOne ( m_Line );
		}

		SetGUIModified ( FALSE );
	}

	// TODO: Add your control notification handler code here
	LoadDirectory ();	
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnWrite() 
{
	// TODO: Add your control notification handler code here
	if ( CTRSaveGame::I() != NULL )
	{
		UpdateBuffer();
		CTRSaveGame::WriteSaveGame ();
	}
	DisplayValues ();
	SetGUIModified ( FALSE );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::DisplayValues()
{
	BOOL bModified = IsGUIModified();

    int             iLevelIndex;
	static char		szString [ 64 ];

	m_Write.EnableWindow ( FALSE );
	m_Max.EnableWindow ( FALSE );

	EnableForVersion ();

	if ( CTRSaveGame::I() != NULL )
	{
		// m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus () );
	}

	/*
	 *	On recupere le handle initial.
	 */
	if ( m_pBitmapLevel == NULL )
	{
		if ( m_hFirstBitmapLevel == NULL )
		{
			m_hFirstBitmapLevel		= m_ImageLevel.GetBitmap ();
		}
	}
	else
	{
		delete ( m_pBitmapLevel );
		m_pBitmapLevel = NULL;
	}

	if ( m_pBitmapContinent == NULL )
	{
		if ( m_hFirstBitmapContinent == NULL )
		{
			m_hFirstBitmapContinent = m_ImageContinent.GetBitmap ();
		}
	}
	else
	{
		delete ( m_pBitmapContinent );
		m_pBitmapContinent = NULL;
	}

	//
	m_SetManualCombo = false;
	m_Custom_Combo.ResetContent();
	m_Custom_Combo.AddString("Tombraider Standard Levels");
	m_SetManualCombo = true;

	/*
	 *		On recree les bitmaps.
	 */
	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Header information.
		 */
		m_Level_Name.SetWindowText ( CTRSaveGame::I()->GetSaveName () );

		sprintf_s ( szString, sizeof(szString), "%03d", CTRSaveGame::I()->GetSaveNumber () );
		m_Savenumber.SetWindowText ( szString );

		m_pBitmapLevel		= new CBitmap;
		m_pBitmapContinent	= new CBitmap;

		//
		int tombraider	= CTRSaveGame::GetFullVersion ();
		int levelIndex	=  CTRSaveGame::GetLevelIndex ();

		//
		STRUCTLOCATION	*pTable = NULL;
		//
		switch ( tombraider )
		{
			case 1 :
			case 10 :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_TR1_END );
				m_pBitmapContinent->LoadBitmap ( IDB_TR1_START );
				pTable	= CustomPathnames1;
				break;
			}

			case 15 :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_TUB_END );
				m_pBitmapContinent->LoadBitmap ( IDB_TUB_START );
				pTable	= CustomPathnames1;
				break;
			}

			case 2:
			case 20 :
			case 25 :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_TR2_END );
				m_pBitmapContinent->LoadBitmap ( IDB_TR2_START );
				pTable	= CustomPathnames2;
				break;
			}

			case 3 :
			case 30 :
			case 35 :
			{
				switch ( levelIndex )
				{
					case 0:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_0 );
						m_pBitmapContinent->LoadBitmap ( IDB_INDIA );
						break;
					}

					case 1:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_1 );
						m_pBitmapContinent->LoadBitmap ( IDB_INDIA );
						break;
					}

					case 2:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_2 );
						m_pBitmapContinent->LoadBitmap ( IDB_INDIA );
						break;
					}
			
					case 3:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_3 );
						m_pBitmapContinent->LoadBitmap ( IDB_INDIA );
						break;
					}
			
					case 4:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_4 );
						m_pBitmapContinent->LoadBitmap ( IDB_SOUTHPAC );
						break;
					}
			
					case 5:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_5 );
						m_pBitmapContinent->LoadBitmap ( IDB_SOUTHPAC );
						break;
					}
			
					case 6:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_6 );
						m_pBitmapContinent->LoadBitmap ( IDB_SOUTHPAC );
						break;
					}
			
					case 7:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_7 );
						m_pBitmapContinent->LoadBitmap ( IDB_SOUTHPAC );
						break;
					}
			
					case 8:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_8 );
						m_pBitmapContinent->LoadBitmap ( IDB_LONDON );
						break;
					}
			
					case 9:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_9 );
						m_pBitmapContinent->LoadBitmap ( IDB_LONDON );
						break;
					}
			
					case 10:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_10 );
						m_pBitmapContinent->LoadBitmap ( IDB_LONDON );
						break;
					}
			
					case 11:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_11 );
						m_pBitmapContinent->LoadBitmap ( IDB_LONDON );
						break;
					}
			
					case 12:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_12 );
						m_pBitmapContinent->LoadBitmap ( IDB_NEVADA );
						break;
					}
			
					case 13:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_13 );
						m_pBitmapContinent->LoadBitmap ( IDB_NEVADA );
						break;
					}
			
					case 14:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_14 );
						m_pBitmapContinent->LoadBitmap ( IDB_NEVADA );
						break;
					}
			
					case 15:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_15 );
						m_pBitmapContinent->LoadBitmap ( IDB_ANTARC );
						break;
					}
			
					case 16:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_16 );
						m_pBitmapContinent->LoadBitmap ( IDB_ANTARC );
						break;
					}
			
					case 17:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_17 );
						m_pBitmapContinent->LoadBitmap ( IDB_ANTARC );
						break;
					}
			
					case 18:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_18 );
						m_pBitmapContinent->LoadBitmap ( IDB_ANTARC );
						break;
					}
			
					case 19:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_LEVEL_19 );
						m_pBitmapContinent->LoadBitmap ( IDB_LONDON );
						break;
					}
			
					default:
					{
						m_pBitmapLevel->LoadBitmap ( IDB_THEEND );
						m_pBitmapContinent->LoadBitmap ( IDB_LEVEL_TR3 );
						break;
					}
				} /* Swich Game */

				pTable	= CustomPathnames3;
				break;

			} /* Case Version */

			case 4 :
			case 40 :
			case 45 :
			case 49 :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_LAST_REV_0 );
				m_pBitmapContinent->LoadBitmap ( IDB_LAST_REV_1 );
				pTable	= CustomPathnames4;
				break;
			}

			case 5 :
			case 50 :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_CHRONICLES_0 );
				m_pBitmapContinent->LoadBitmap ( IDB_CHRONICLES_1 );

				pTable	= CustomPathnames5;
				break;
			}

			default :
			{
				m_pBitmapLevel->LoadBitmap ( IDB_TR1_END );
				m_pBitmapContinent->LoadBitmap ( IDB_TR1_START );
				break;
			}

		} /* Switch Version */

		if ( pTable != NULL )
		{
			for ( int i = 0; i < LEN_LOCATION; i++ )
			{
				if ( strlen(pTable [ i ].szPathname) > 0 )
				{
					m_Custom_Combo.AddString ( pTable [ i ].szPathname );
				}
			}

			if ( m_Custom_Selected >= 0 && m_Custom_Selected < m_Custom_Combo.GetCount() )
			{
				m_SetManualCombo		= false;
				m_Custom_Combo.SetCurSel(m_Custom_Selected);
				m_SetManualCombo		= true;
			}
			else
			{
				InitCustomArea();
				m_SetManualCombo		= false;
				m_Custom_Selected		= 0;
				m_Custom_Combo.SetCurSel(m_Custom_Selected);
				m_SetManualCombo		= true;
			}
		}
		else
		{
			InitCustomArea();
			m_SetManualCombo		= false;
			m_Custom_Selected		= 0;
			m_Custom_Combo.SetCurSel(m_Custom_Selected);
			m_SetManualCombo		= true;
		}

		//
		m_hBitmapLevel = HBITMAP ( *m_pBitmapLevel );
		m_hBitmapContinent = HBITMAP ( *m_pBitmapContinent );

		m_ImageLevel.SetBitmap ( m_hBitmapLevel );
		m_ImageContinent.SetBitmap ( m_hBitmapContinent );

		sprintf_s ( szString, sizeof(szString), "%02d", CTRSaveGame::GetLevel ( ) );
		m_Levelnumber.SetWindowText ( szString );

		/*
		 *		Display the number of secrets.
		 */
		sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetCurrentSecrets () );
		m_Secrets.SetWindowText ( szString );

		iLevelIndex = CTRSaveGame::GetLevelIndex ();
		sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetMaximumSecrets ( iLevelIndex ) );
		m_Nb_Secrets.SetWindowText ( szString );

		/*
		 *      Display if we are burning.
	 	 *		0x10 means that you are burning.
		 */
		m_Lara_Burning.SetCheck ( 0 );
		if ( CTRSaveGame::I()->IsLaraBurning ( ) )
		{
			m_Lara_Burning.SetCheck ( 1 );
		}

		sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetLaraState() );
		m_Lara.SetWindowText ( szString );

		//
		const TR_POSITION *position =  CTRSaveGame::I()->GetPosition();
		if ( position != NULL )
		{

			DWORD dwWestToEast		= position->dwWestToEast;
			DWORD dwVertical		= position->dwVertical;
			DWORD dwSouthToNorth	= position->dwSouthToNorth;
			double dfDirection		= CTRXTools::ConvertOrientationFromWORD ( position->wOrientation );
			WORD wDirection			= (WORD) ( dfDirection  );
			WORD wRoom				= position->wRoom;

			//
			sprintf_s ( szString, sizeof(szString), "%ld", dwWestToEast );
			m_West_East.SetWindowText ( szString );
			sprintf_s ( szString, sizeof(szString), "%ld", dwVertical );
			m_Vertical.SetWindowText ( szString );
			sprintf_s ( szString, sizeof(szString), "%ld", dwSouthToNorth );
			m_South_North.SetWindowText ( szString );
			sprintf_s ( szString, sizeof(szString), "%.2f", dfDirection );
			m_Direction.SetWindowText ( szString );
			sprintf_s ( szString, sizeof(szString), "%d", wRoom );
			m_Area.SetWindowText ( szString );

			m_West_East.EnableWindow ( TRUE );
			m_Vertical.EnableWindow ( TRUE );
			m_South_North.EnableWindow ( TRUE );
			m_Direction.EnableWindow ( TRUE );
			m_Area.EnableWindow ( TRUE );

			TR_AREA *pArea = NULL;
			if ( IsCustomArea() )
			{
				pArea = GetTRArea ( CUSTOM_GAME, CUSTOM_LEVEL_INDEX, wRoom );
			}
			else
			{
				pArea = GetTRArea ( tombraider, levelIndex, wRoom );
			}
			ShowRoom ( pArea, dwWestToEast, dwSouthToNorth, wDirection );

			m_Room.EnableWindow ( TRUE );
		}
		else
		{
			m_West_East.EnableWindow ( FALSE );
			m_Vertical.EnableWindow ( FALSE );
			m_South_North.EnableWindow ( FALSE );
			m_Direction.EnableWindow ( FALSE );
			m_Area.EnableWindow ( FALSE );

			m_West_East.SetWindowText ( "" );
			m_Vertical.SetWindowText ( "" );
			m_South_North.SetWindowText ( "" );
			m_Direction.SetWindowText ( "" );
			m_Area.SetWindowText ( "" );

			if ( m_bRoomCreated )
			{
				m_Room.EnableWindow ( FALSE );
				m_Room.ShowWindow ( SW_HIDE );
			}
		}

		/*
		 *	Set the write button state.
		 */
		m_Write.EnableWindow ( TRUE );
		m_Max.EnableWindow ( TRUE );

	}

	SetGUIModified ( bModified, "Info DisplayValues" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::SetCurrent ( )
{
	//
	static char szFilename [ MAX_PATH ];
	ZeroMemory ( szFilename, sizeof(szFilename) );

	static char szCurrentFilename [ MAX_PATH ];
	ZeroMemory ( szCurrentFilename, sizeof(szCurrentFilename) );

	m_Filename.GetWindowText ( szFilename, sizeof(szFilename) );

	for ( int iX = 0; iX < m_ListCtrl.GetItemCount(); iX++ )
	{
		m_ListCtrl.GetItemText ( iX, COL_PATHNAME, szCurrentFilename, sizeof(szCurrentFilename) );
		if ( strcmp ( szFilename, szCurrentFilename ) == 0 )
		{
			m_ListCtrl.SetItemState(-1, 0, LVIS_SELECTED);
			m_ListCtrl.SetItemState(iX, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			m_ListCtrl.Update(iX);
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::ShowRoom ( TR_AREA *pArea, DWORD x, DWORD z, WORD wDirection )
{
	//	Create the Room
	if ( pArea != NULL )
	{
		TR_CUR_POSITION					currentPosition;
		ZeroMemory ( &currentPosition, sizeof(currentPosition) );
		currentPosition.x				= x;
		currentPosition.z				= z;
		currentPosition.orientation		= wDirection;
		m_Room.SetAreaAndPosition ( pArea, &currentPosition );

		//
		RECT rectPosition;
		m_Position.GetWindowRect ( &rectPosition );
		ScreenToClient ( &rectPosition );
		RECT roomRect;
		roomRect.top		= rectPosition.bottom + RoomMargin;
		roomRect.left		= rectPosition.left + RoomMargin;
		roomRect.bottom		= roomRect.top + pArea->xSectors * TR_SECTOR_SIZE / RoomDivider;
		roomRect.right		= roomRect.left + pArea->zSectors * TR_SECTOR_SIZE / RoomDivider;

		m_Room.SetPointMode ( TRUE );
		if ( ! m_bRoomCreated )
		{
			CTRXRoomPicture::CreateInstanceInside ( this, &m_Room, roomRect, "", SS_NOTIFY );
#if 0
			DWORD dwStyle = SS_OWNERDRAW;
			dwStyle |= SS_NOTIFY;
			BOOL bCreated = m_Room.Create ( 
				"",								//	LPCTSTR lpszCaption,
				dwStyle,						//	DWORD dwStyle,
				roomRect,						//   const RECT& rect,
				this,							//	CWnd* pParentWnd,
				-1								//	UINT nID 
			);
#endif

			if ( m_bToolTip )
			{
				m_ToolTip.AddTool( &m_Room, "You can click on the bitmap\r\nto position Lara precisely\r\nGenerally walls are 1.0 width\r\n" );
			}

			m_bRoomCreated	= TRUE;
		}
		else
		{
			m_Room.MoveWindow ( &roomRect, TRUE );
			InvalidateRect ( &roomRect );
		}

		m_Room.EnableWindow ( TRUE );
			
		//
		CPoint roomPoint = m_Room.ComputeRoomPoint ( x, z );
		double dfOrientation = wDirection;
		m_Room.SetRoomPoint ( roomPoint, &dfOrientation );
		BOOL bShow = m_Room.ShowWindow ( SW_NORMAL );
	}
	else
	{
		m_Room.EnableWindow ( FALSE );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bModified = IsGUIModified();
	if ( CTRSaveGame::IsValid( ) )
	{
		bModified = CTRSaveGame::IsBufferModified();
	}

	//
	// m_ListCtrl.SetColorsNotUsed();

	if ( m_pListCtrl == NULL )
	{
		m_pListCtrl = &m_ListCtrl;

		m_ListCtrl.InsertColumn ( COL_FILENAME, "Filename", LVCFMT_LEFT, 80);
		m_ListCtrl.InsertColumn ( COL_GAME, "Game", LVCFMT_LEFT, 48);
		m_ListCtrl.InsertColumn ( COL_GAME_NO, "Save", LVCFMT_LEFT, 40);
		m_ListCtrl.InsertColumn ( COL_LEVEL_NO, "Level", LVCFMT_LEFT, 40);
		m_ListCtrl.InsertColumn ( COL_TITLE, "Title", LVCFMT_LEFT, 120);
		m_ListCtrl.InsertColumn ( COL_PATHNAME, "Pathname", LVCFMT_LEFT, 280);
		m_ListCtrl.InsertColumn ( COL_DATE, "Date", LVCFMT_LEFT, 128);

		m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

		for ( int i = 0; i < LEN_LOCATION; i++ )
		{
			if ( strlen(LocationPathname [ i ].szPathname) > 0 )
			{
				m_Combo.AddString ( LocationPathname [ i ].szPathname );
			}
		}
	}

	//
	if ( m_bFirstTime )
	{
		static char	szDirectory [ MAX_PATH ];
		ZeroMemory ( szDirectory, sizeof(szDirectory) );

		m_Write.EnableWindow ( FALSE );
		m_Max.EnableWindow ( FALSE );

		//
		CString lastRead  = "";
		if ( strlen(m_ParmPathname) > 0 )
		{
			lastRead = m_ParmPathname;
			ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
		}
		else
		{
			lastRead = theApp.GetProfileString( "Settings", "Last Opened", "" );
		}

		//
		if ( lastRead != "" )
		{
			strcpy_s ( szDirectory, sizeof(szDirectory), lastRead );
			theApp.RemoveFilename ( szDirectory );
		}
		else
		{
			/*
			 *	Set directory Text.
			 */
			if ( m_Combo.GetCount() == 0 )
			{
				_getcwd ( szDirectory, sizeof ( szDirectory ) - 1 );
				m_Combo.AddString ( szDirectory );
			}
		}

		//
		DisplayDirectory (szDirectory);
		m_bFirstTime = false;
	}

	//
	m_Status.SetWindowText ( "Tombraider 1, Unfinished Busines, 2, Golden Mask, 3, Lost Artifact, 4 and 5 " );

	//	Not Necessary
	// SetCurrent ( );
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
	DisplayValues ();

	SetGUIModified ( bModified );

	//
	return CTRXPropertyPage::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::ClickRoom ( UINT nFlags, CPoint point )
{
	static char szText [ 64 ];
	DWORD dwX = 0;
	DWORD dwZ = 0;

	m_Room.GetXZ ( point, dwX, dwZ );

	//
	sprintf_s ( szText, sizeof(szText), "%ld", dwX );
	m_West_East.SetWindowText ( szText );

	sprintf_s ( szText, sizeof(szText), "%ld", dwZ );
	m_South_North.SetWindowText ( szText );
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::UpdateBuffer()
{
	int						iLevelIndex;

    char					szString [ 32 ];

	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		iLevelIndex = CTRSaveGame::GetLevelIndex ();

		/*
		 *		Set the number of secrets.
		 */
		m_Secrets.GetWindowText ( szString, sizeof ( szString ) );
		CTRSaveGame::I()->SetCurrentSecrets ( szString, iLevelIndex );

		/*
		 *		Objects mask.
		 */
		CTRSaveGame::I()->SetAllObjects ();

		/*
		 *		Lara burning.
		 */
		if ( m_Lara_Burning.GetCheck () )
		{
			if( ! CTRSaveGame::I()->IsLaraBurning() )
			{
				CTRSaveGame::I()->SetLaraBurning ( 0 );
			}
		}
		else
		{
			if( CTRSaveGame::I()->IsLaraBurning() )
			{
				CTRSaveGame::I()->SetLaraNoBurning ( 0 );
			}
		}
		
		m_Lara.GetWindowText ( szString, sizeof ( szString ) );
		CTRSaveGame::I()->SetLaraState ( atoi ( szString ) );

		//
		m_West_East.GetWindowText ( szString, sizeof(szString) );
		DWORD dwWestToEast		= atol(szString);
		m_Vertical.GetWindowText ( szString, sizeof(szString) );
		DWORD dwVertical		= atol(szString);
		m_South_North.GetWindowText ( szString, sizeof(szString) );
		DWORD dwSouthToNorth	= atol(szString);
		m_Direction.GetWindowText ( szString, sizeof(szString) );
		double dfOrientation	= atof ( szString );
		WORD wDirection			= CTRXTools::ConvertOrientationFromDouble ( dfOrientation );
		m_Area.GetWindowText ( szString, sizeof(szString) );
		WORD wRoom				= atoi ( szString );

		CTRSaveGame::I()->SetPosition ( dwWestToEast, dwVertical, dwSouthToNorth, wDirection, wRoom );

	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::OnKillActive() 
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
void CTRXInfoPage::SetComboSelection ( const char *pDirectory )
{
	//	Show Current File
	for ( int i = 0; i < m_Combo.GetCount(); i++ )
	{
		CString directory;
		m_Combo.GetLBText ( i, directory );
		if ( strcmp ( directory, pDirectory ) == 0 )
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
void CTRXInfoPage::SetComboSelection ( )
{
	static char	szFilename [ MAX_PATH ];
	ZeroMemory ( szFilename, sizeof(szFilename) );
	m_Filename.GetWindowText ( szFilename, sizeof(szFilename) );
	theApp.RemoveFilename ( szFilename );
	SetComboSelection ( szFilename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::FillListCtrl(const char *pDirectory)
{

    /*
     *      General purpose index.
     */
    intptr_t               	iStream;

    /*
     *      Structure for find file.
     */
    struct _finddatai64_t   tagFile;

	/*
	 *		An index.
	 */
	int						iLine;

	/*
	 *		The Game.
	 */
	char					szGame [ 64 ];

	/*
	 *		The Game save number.
	 */
	int 					iGame;

	/*
	 *		The Title
	 */
	char					szTitle [ 64 ];

	/*
	 *		The Version.
	 */
	int						iVersion;

	//
	BOOL bModified = IsGUIModified();

	//
	m_SetManualList = false;

	//
	BOOL bAdded = AddLocation ( LocationPathname, pDirectory );
	if ( bAdded )
	{
		m_Combo.AddString ( pDirectory );
	}

	//
	SetComboSelection ( pDirectory );

	/*
	 *		Reset counter.
	 */
	iLine = 0;
	m_ListCtrl.DeleteAllItems ();

	char					szDirectory  [ MAX_PATH ];
	char					szFiles  [ MAX_PATH ];

	strcpy_s ( szDirectory, sizeof(szDirectory), pDirectory );
	strcpy_s ( szFiles, sizeof(szFiles), pDirectory );
	strcat_s ( szFiles, sizeof(szFiles), "\\SAVE*.*" );

    /*
     *      Find first name.
     */
    iStream = _findfirsti64 ( szFiles, &tagFile );
    if ( iStream != -1 )
    {
#if 0
		strcpy_s ( szFiles, sizeof(szFiles), szDirectory );
		strcat_s ( szFiles, sizeof(szFiles), "\\" );
		strcat_s ( szFiles, sizeof(szFiles), tagFile.name );

		//
		int iLevel	= 0;
		int iSub	= 0;

		//	Treat First
		if ( (  tagFile.attrib & _A_SUBDIR ) == 0 &&
				strstr ( tagFile.name, ".bak" ) == NULL && strstr ( tagFile.name, ".BAK" ) == NULL )
		{
			iVersion = CTRSaveGame::InstanciateVersion ( szFiles );

			if ( iVersion != -1 && CTRSaveGame::I() != NULL  )
			{
				iVersion = CTRSaveGame::I()->GetInfo ( szGame, sizeof(szGame), &iGame, &iLevel, szTitle, sizeof(szTitle) );
			}

			if ( iVersion > 0 )
			{
				m_ListCtrl.InsertItem ( iLine, tagFile.name, 0 );
				m_ListCtrl.SetItemText ( iLine, COL_GAME, szGame);

				sprintf_s ( szGame, sizeof(szGame), "%03d", iGame );
				m_ListCtrl.SetItemText ( iLine, COL_GAME_NO, szGame);

				sprintf_s ( szGame, sizeof(szGame), "%02d", iLevel );
				m_ListCtrl.SetItemText ( iLine, COL_LEVEL_NO, szGame);

				m_ListCtrl.SetItemText ( iLine, COL_TITLE, szTitle);

				m_ListCtrl.SetItemText ( iLine, COL_PATHNAME, szFiles);
				char szDate [ 64 ];
				struct tm tmDate;
				_localtime64_s ( &tmDate, &tagFile.time_write );
				sprintf_s ( szDate, sizeof(szDate), "%04d/%02d/%02d %02d:%02d:%02d",
						tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday,
						tmDate.tm_hour, tmDate.tm_min, tmDate.tm_sec );
				m_ListCtrl.SetItemText ( iLine, COL_DATE, szDate);
				iLine++;
			}
		}
#endif
		//	Treat Next
		do
		{
			strcpy_s ( szFiles, sizeof(szFiles), szDirectory );
			strcat_s ( szFiles, sizeof(szFiles), "\\" );
			strcat_s ( szFiles, sizeof(szFiles), tagFile.name );

			int iLevel	= 0;
			int iSub	= 0;

			if (  ( tagFile.attrib & _A_SUBDIR ) == 0  &&
					strstr ( tagFile.name, ".bak" ) == NULL && strstr ( tagFile.name, ".BAK" ) == NULL )
			{
				iVersion	= CTRSaveGame::InstanciateVersion ( szFiles );

				// 
				if ( iVersion != -1 && CTRSaveGame::I() != NULL  )
				{
					iVersion = CTRSaveGame::I()->GetInfo ( szGame, sizeof(szGame), &iGame, &iLevel, szTitle, sizeof(szTitle) );
				}

				if ( iVersion > 0 )
				{
					m_ListCtrl.InsertItem ( iLine, tagFile.name, 0 );
					m_ListCtrl.SetItemText ( iLine, COL_GAME, szGame);

					sprintf_s ( szGame, sizeof(szGame), "%03d", iGame );
					m_ListCtrl.SetItemText ( iLine, COL_GAME_NO, szGame);

					sprintf_s ( szGame, sizeof(szGame), "%02d", iLevel );
					m_ListCtrl.SetItemText ( iLine, COL_LEVEL_NO, szGame);

					m_ListCtrl.SetItemText ( iLine, COL_TITLE, szTitle);
					m_ListCtrl.SetItemText ( iLine, COL_PATHNAME, szFiles);
					char szDate [ 64 ];
					struct tm tmDate;
					_localtime64_s ( &tmDate, &tagFile.time_write );
					sprintf_s ( szDate, sizeof(szDate), "%04d/%02d/%02d %02d:%02d:%02d",
							tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday,
							tmDate.tm_hour, tmDate.tm_min, tmDate.tm_sec );
					m_ListCtrl.SetItemText ( iLine, COL_DATE, szDate);

					iLine++;
				}
			}
		}
		while ( _findnexti64 ( iStream, &tagFile ) == 0 );

		//
		iAscending = -1;
		SortItems ( COL_DATE );
		iAscending = 1;

		if ( m_ListCtrl.GetItemCount() >= 1 )
		{
			DisplayOne(0);
		}
	}

	SetGUIModified ( bModified, "InfoPage FillListCtrl" );

	m_SetManualList = true;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::DisplayOne ( int line )
{
	BOOL bModified = IsGUIModified();

	/*
	 *		The Game.
	 */
	char					szGame [ 64 ];

	/*
	 *		The Game save number.
	 */
	int 					iGame;

	/*
	 *		The Title
	 */
	char					szTitle [ 64 ];

	/*
	 *		The Version.
	 */
	int						iVersion;

	// TODO: Add your control notification handler code here
	char	szFilename [ MAX_PATH ];

	//
	m_Line		= line;

	//
	m_Write.EnableWindow ( FALSE );
	m_Max.EnableWindow ( FALSE );

	// check that one item gets the focus
	if ( line >= 0 && line < m_ListCtrl.GetItemCount() )
	{
		m_ListCtrl.GetItemText( line, COL_PATHNAME, szFilename, sizeof ( szFilename ) - 1 );
		m_Filename.SetWindowText ( szFilename );
		BOOL bWritten = theApp.WriteProfileString ( "Settings", "Last Opened", szFilename );

		/*
		 *	Retrieve informations.
		 */
		iVersion = CTRSaveGame::InstanciateVersion ( szFilename );
		if ( iVersion != -1 && CTRSaveGame::I() != NULL )
		{
			int iLevel	= 0;
			int iSub	= 0;
			iVersion = CTRSaveGame::I()->GetInfo ( szGame, sizeof(szGame), &iGame, &iLevel, szTitle, sizeof(szTitle) );
			CTRSaveGame::I()->RetrieveInformation ( szFilename );
			m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus() );

			DisplayValues ();
		}
	}

	SetGUIModified ( bModified, "Info DisplayOne" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnClickListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	//	Unselection
	if (	( pNMListView->uNewState & LVIS_SELECTED ) == 0 &&
			( pNMListView->uOldState & LVIS_SELECTED ) == LVIS_SELECTED )
	{
		if ( m_SetManualList && IsGUIModified() )
		{
			UpdateBuffer();
		}

		m_Line = -1;

		*pResult = TRUE;
		return;
	}

	//
	if (	( pNMListView->uOldState & LVIS_SELECTED ) == 0 &&
			( pNMListView->uNewState & LVIS_SELECTED ) == LVIS_SELECTED )
	{
		if ( m_SetManualList )
		{
			if ( CTRSaveGame::IsValid( ) && IsGUIModified() )
			{
				INT_PTR response = CTRXDifferences::MessageBoxS ( "Do you want to save the file ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
				if ( response == IDYES )
				{
					DisplayOne ( m_Line );
					CTRSaveGame::I()->writeSaveGame();
				}
				else if ( response == IDCANCEL )
				{
					*pResult = TRUE;
					return;
				}
				else
				{
					CTRSaveGame::Discard();
					DisplayOne ( m_Line );
				}
			}

			m_Line = pNMListView->iItem;
			DisplayOne ( pNMListView->iItem );

			SetGUIModified ( FALSE );
		}
	}

	*pResult = TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnItemchangedListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	//	Unselection
	if (	( pNMListView->uNewState & LVIS_SELECTED ) == 0 &&
			( pNMListView->uOldState & LVIS_SELECTED ) == LVIS_SELECTED )
	{
		if ( m_SetManualList && IsGUIModified() )
		{
			UpdateBuffer();
		}
		*pResult = TRUE;
		return;
	}

	//
	if (	( pNMListView->uOldState & LVIS_SELECTED ) == 0 &&
			( pNMListView->uNewState & LVIS_SELECTED ) == LVIS_SELECTED )
	{
		if ( m_SetManualList )
		{
			if ( CTRSaveGame::IsValid( ) && IsGUIModified() )
			{
				INT_PTR response = CTRXDifferences::MessageBoxS ( "Do you want to save the file ?", MessageTitle, MB_YESNO|MB_ICONQUESTION );
				if ( response == IDYES )
				{
					DisplayOne ( m_Line );
					CTRSaveGame::I()->writeSaveGame();
				}
				else if ( response == IDCANCEL )
				{
					*pResult = TRUE;
					return;
				}
				else
				{
					CTRSaveGame::Discard();
					DisplayOne ( m_Line );
				}
			}

			m_Line = pNMListView->iItem;
			DisplayOne ( pNMListView->iItem );

			SetGUIModified ( FALSE );
		}
	}

	*pResult = TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXInfoPage::EnableForVersion()
{
	BOOL bModified = IsGUIModified();

	int iVersion = CTRXPropertyPage::EnableForVersion();

	//
	if ( CTRSaveGame::I() != NULL && iVersion != 0 )
	{
		m_Secrets.EnableWindow ( iVersion >= 20 );
		m_Lara_Burning.EnableWindow ( iVersion >= 20 );
		m_Lara.EnableWindow ( iVersion >= 40 );
	}
	else
	{
		m_Secrets.EnableWindow ( FALSE );
		m_Lara_Burning.EnableWindow ( FALSE );
		m_Lara.EnableWindow ( FALSE );
	}

	//
	SetGUIModified ( bModified );

	return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::DisplayDirectory(const char *pDirectory)
{

	/*
	 *	Fills list Ctrl.
	 */
	FillListCtrl (pDirectory);

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::SortItems ( int col )
{
	m_SetManualList = false;

	for ( int i = 0; i < m_ListCtrl.GetItemCount(); i++ )
	{
		strcpy_s ( tagData [ i ].szText, MAX_PATH, m_ListCtrl.GetItemText( i, col ) );
		m_ListCtrl.SetItemData ( i, (DWORD_PTR)( &tagData [ i ].szText ) );
	}

	DWORD		dwData	= col;
	DWORD_PTR	pdwData = ( DWORD_PTR ) &dwData;
  	m_ListCtrl.SortItems(SortFunc, pdwData );

	m_SetManualList = true;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add Your handler here
	NMLISTVIEW *pLV = (NMLISTVIEW *) pNMHDR;
  	
	SortItems ( pLV->iItem );
  	
	iAscending *= -1;

	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//	Get Max
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedMax()
{
	// TODO: Add Your handler here
	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *		Get gun state.
		 */
		int iLevelIndex = CTRSaveGame::GetLevelIndex ();

		/*
		 *      Get current values for ammos.
		 */
		if ( CTRSaveGame::GetVersion () >= 40 )
		{
			CTRSaveGame::I()->SetSmallMedipak ( Value10K, iLevelIndex );
			CTRSaveGame::I()->SetLargeMedipak ( Value10K, iLevelIndex );
			CTRSaveGame::I()->SetFlares ( Value10K, iLevelIndex );
		}
		else
		{
			CTRSaveGame::I()->SetSmallMedipak ( Value255, iLevelIndex );
			CTRSaveGame::I()->SetLargeMedipak ( Value255, iLevelIndex );
			CTRSaveGame::I()->SetFlares ( Value255, iLevelIndex );
		} 

		//	Ammos
		if ( CTRSaveGame::GetVersion() >= 40 ) 
		{
			if ( CTRSaveGame::I()->HasAmmos2() ) CTRSaveGame::I()->SetAmmos2 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos3() ) CTRSaveGame::I()->SetAmmos3 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4b ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos5() ) CTRSaveGame::I()->SetAmmos5 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos6() ) CTRSaveGame::I()->SetAmmos6 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7b ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7c ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8b ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8c ( ValueMinusOne, iLevelIndex );
		}
		else
		{
			if ( CTRSaveGame::I()->HasAmmos2() ) CTRSaveGame::I()->SetAmmos2 ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos3() ) CTRSaveGame::I()->SetAmmos3 ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4a ( Value1000, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4b ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos5() ) CTRSaveGame::I()->SetAmmos5 ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos6() ) CTRSaveGame::I()->SetAmmos6 ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7a ( Value1000, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7b ( Value1000, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7c ( Value1000, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8a ( Value1000, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8b ( Value1000, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8c ( Value1000, iLevelIndex );
		}

		//	Unlimited
		if ( CTRSaveGame::GetVersion() >= 40 ) 
		{
			if ( CTRSaveGame::I()->HasAmmos2() ) CTRSaveGame::I()->SetAmmos2 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos3() ) CTRSaveGame::I()->SetAmmos3 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos4() ) CTRSaveGame::I()->SetAmmos4b ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos5() ) CTRSaveGame::I()->SetAmmos5 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos6() ) CTRSaveGame::I()->SetAmmos6 ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7b ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos7() ) CTRSaveGame::I()->SetAmmos7c ( ValueMinusOne, iLevelIndex );

			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8a ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8b ( ValueMinusOne, iLevelIndex );
			if ( CTRSaveGame::I()->HasAmmos8() ) CTRSaveGame::I()->SetAmmos8c ( ValueMinusOne, iLevelIndex );

		}
		else
		{
			CTRSaveGame::I()->SetUnlimitedAmmos ( Value255 );
		}

		/*
 		 *		Set air.
		 */
		CTRSaveGame::I()->SetAir ( Value1800 );

		/*
		 *		Set Health.
		 */
		CTRSaveGame::I()->SetGunAmmos ( Value1000 );

		/*
		 *		Set Life.
		*/
		CTRSaveGame::I()->SetLife ( Many32K );

		//	Binocular
		CTRSaveGame::I()->SetBinocular ( 1 );

		//	Laser
		CTRSaveGame::I()->SetLaser ( 1 );

		//	Get All Treasure
		if ( CTRSaveGame::GetVersion () == 30 )
		{
			if ( CTRSaveGame::I()->GetTreasure1(iLevelIndex) <= 0 ) CTRSaveGame::I()->SetTreasure1 ( 1, iLevelIndex );
			if ( CTRSaveGame::I()->GetTreasure2(iLevelIndex) <= 0 ) CTRSaveGame::I()->SetTreasure2 ( 1, iLevelIndex );
			if ( CTRSaveGame::I()->GetTreasure3(iLevelIndex) <= 0 ) CTRSaveGame::I()->SetTreasure3 ( 1, iLevelIndex );
			if ( CTRSaveGame::I()->GetTreasure4(iLevelIndex) <= 0 ) CTRSaveGame::I()->SetTreasure4 ( 1, iLevelIndex );
		}

		if ( CTRSaveGame::GetVersion () <= 30 )
		{

			/*
			 *		Maps.
			 */
			if ( CTRSaveGame::I()->GetMap1() <= 0 ) CTRSaveGame::I()->SetMap1 ( 1 );
			if ( CTRSaveGame::I()->GetMap2() <= 0 ) CTRSaveGame::I()->SetMap2 ( 1 );

			/*
			 *		Items.
			 */
			if ( CTRSaveGame::I()->GetItem1() <= 0 ) CTRSaveGame::I()->SetItem1 ( 1 );
			if ( CTRSaveGame::I()->GetItem2() <= 0 ) CTRSaveGame::I()->SetItem2 ( 1 );
			if ( CTRSaveGame::I()->GetItem3() <= 0 ) CTRSaveGame::I()->SetItem3 ( 1 );
			if ( CTRSaveGame::I()->GetItem4() <= 0 ) CTRSaveGame::I()->SetItem4 ( 1 );

			/*
			 *		Keys.
			 */
			if ( CTRSaveGame::I()->GetKey1() <= 0 ) CTRSaveGame::I()->SetKey1 ( 1 );
			if ( CTRSaveGame::I()->GetKey2() <= 0 ) CTRSaveGame::I()->SetKey2 ( 1 );
			if ( CTRSaveGame::I()->GetKey3() <= 0 ) CTRSaveGame::I()->SetKey3 ( 1 );
			if ( CTRSaveGame::I()->GetKey4() <= 0 ) CTRSaveGame::I()->SetKey4 ( 1 );

			/*
			 *		Objects mask.
			 */
			CTRSaveGame::I()->SetAllObjects ();

		}

		//
		//	Guns
		if ( CTRSaveGame::GetVersion () <= 49 )
		{
			if ( CTRSaveGame::I()->HasWeapon1() ) CTRSaveGame::I()->GrabWeapon1 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon2() ) CTRSaveGame::I()->GrabWeapon2 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon3() ) CTRSaveGame::I()->GrabWeapon3 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon4() ) CTRSaveGame::I()->GrabWeapon4 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon5() ) CTRSaveGame::I()->GrabWeapon5 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon6() ) CTRSaveGame::I()->GrabWeapon6 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon7() ) CTRSaveGame::I()->GrabWeapon7 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon8() ) CTRSaveGame::I()->GrabWeapon8 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon9() ) CTRSaveGame::I()->GrabWeapon9 ( iLevelIndex );
		}

		if ( CTRSaveGame::GetVersion () >= 50 && CTRSaveGame::GetVersion () <= 59 )
		{
			if ( CTRSaveGame::I()->HasWeapon1() ) CTRSaveGame::I()->GrabWeapon1 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon2() ) CTRSaveGame::I()->GrabWeapon2 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon3() ) CTRSaveGame::I()->GrabWeapon3 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon4() ) CTRSaveGame::I()->GrabWeapon4 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon5() ) CTRSaveGame::I()->GrabWeapon5 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon6() ) CTRSaveGame::I()->GrabWeapon6 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon7() ) CTRSaveGame::I()->GrabWeapon7 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon8() ) CTRSaveGame::I()->GrabWeapon8 ( iLevelIndex );
			if ( CTRSaveGame::I()->HasWeapon9() ) CTRSaveGame::I()->GrabWeapon9 ( iLevelIndex );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedRefresh()
{
	// TODO: Add Your handler here
	if ( CTRSaveGame::IsValid( ) && IsGUIModified() )
	{
		UpdateBuffer();
		INT_PTR response = CTRXDifferences::MessageBoxS ( "Do you want to save the file ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
		if ( response == IDYES )
		{
			DisplayOne ( m_Line );
			CTRSaveGame::I()->writeSaveGame();
		}
		else if ( response == IDCANCEL )
		{
			return;
		}
		else
		{
			CTRSaveGame::Discard();
			DisplayOne ( m_Line );
		}

		SetGUIModified ( FALSE );
	}

	char	szDirectory [ MAX_PATH ];
	m_Filename.GetWindowText ( szDirectory, sizeof ( szDirectory ) );
	theApp.RemoveFilename ( szDirectory );

	DisplayDirectory (szDirectory);
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//	Combo
///////////////////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnSelchangeCombo()
{
	if ( m_SetManualCombo )
	{
		if ( IsGUIModified() || CTRSaveGame::IsBufferModified() )
		{
			UpdateBuffer();
			INT_PTR response = CTRXDifferences::MessageBoxS ( "Do you want to save the file ?", MessageTitle, MB_YESNOCANCEL|MB_ICONQUESTION );
			if ( response == IDYES )
			{
				DisplayOne ( m_Line );
				CTRSaveGame::I()->writeSaveGame();
			}
			else if ( response == IDCANCEL )
			{
				SetComboSelection();

				return;
			}
			else
			{
				CTRSaveGame::Discard();
				DisplayOne ( m_Line );
			}

			SetGUIModified ( FALSE );
		}

		//
		// TODO: Add Your handler here
		int iCurSel = m_Combo.GetCurSel();
		if ( iCurSel >= 0 && iCurSel < m_Combo.GetCount() )
		{
			// m_Combo.Get
			char szDirectory [ MAX_PATH ];
			m_Combo.GetLBText( iCurSel, szDirectory );
			DisplayDirectory (szDirectory);
		}
	}

	SetGUIModified ( FALSE );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedRemove()
{
	// TODO: Add Your handler here
	int iCurSel = m_Combo.GetCurSel();
	if ( iCurSel >= 0 && iCurSel < m_Combo.GetCount() )
	{
		// m_Combo.Get
		char szDirectory [ MAX_PATH ];
		m_Combo.GetLBText( iCurSel, szDirectory );
		BOOL bRemoved = RemoveLocation ( LocationPathname, szDirectory );

		//
		RemoveFromCombo ( &m_Combo, iCurSel );

		//
		iCurSel = m_Combo.GetCurSel();

		//
		m_Combo.GetLBText( iCurSel, szDirectory );
		DisplayDirectory (szDirectory);
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::OnInitDialog()
{
	CTRXPropertyPage::OnInitDialog();

	// TODO: Add Code Here
	if ( m_bToolTip )
	{
		m_ToolTip.AddTool( &m_Status, ("Status"));

		m_ToolTip.AddTool( &m_LoadDirectory, ("Load Savegames from a Directory"));
		m_ToolTip.AddTool( &m_Refresh, ("Re Load Savegames from a Directory"));
		m_ToolTip.AddTool( &m_Remove, ("Remove from the list of directories"));
		m_ToolTip.AddTool( &m_Add_Custom, ("Add Custom file To the List"));
		m_ToolTip.AddTool( &m_Remove_Custom, ("Remove Custom file From the List"));
		m_ToolTip.AddTool( &m_See_Custom, ("Search In the list"));
		m_ToolTip.AddTool( &m_Write, ("Write Current File"));
		m_ToolTip.AddTool( &m_Map, "Show Corresponding Map" );
		m_ToolTip.AddTool( &m_Sort, "Sort List" );
		m_ToolTip.AddTool( &m_ListCtrl, LPSTR_TEXTCALLBACK );
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
BOOL CTRXInfoPage::  OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
	//
	if ( ! m_bToolTip )
	{
		return FALSE;
	}

	TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMH;

	POINT			ptAction;

	GetCursorPos ( &ptAction );
	m_ListCtrl.ScreenToClient ( &ptAction );

	//	
	int iHit = m_ListCtrl.HitTest ( ptAction );
	if ( iHit != -1 )
	{
		LVHITTESTINFO hitInfo;
		memset ( &hitInfo, 0, sizeof ( hitInfo ) );
		hitInfo.pt =	ptAction;
		//	iiTem and iSubItem from hitInfo
		int iSub = m_ListCtrl.SubItemHitTest ( &hitInfo );
		if ( iSub != -1 )
		{
			if ( hitInfo.iItem >= 0 && hitInfo.iSubItem >= 0 )
			{
				static char szFilename [ MAX_PATH ] = "";
				ZeroMemory ( szFilename, sizeof(szFilename) );
				m_ListCtrl.GetItemText ( hitInfo.iItem, hitInfo.iSubItem, szFilename, sizeof(szFilename) );
				pText->lpszText = szFilename;

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
void CTRXInfoPage::OnBnClickedLaraburning()
{
	SetGUIModified ( TRUE, "Info Laraburning" );

	//	To Burn
	if ( m_Lara_Burning.GetCheck () )
	{
		if( ! CTRSaveGame::I()->IsLaraBurning() )
		{
			CTRSaveGame::I()->SetLaraBurning ( 0 );
		}
	}
	else
	{
		if( CTRSaveGame::I()->IsLaraBurning() )
		{
			CTRSaveGame::I()->SetLaraNoBurning ( 0 );
		}
	}

	char szString [ 32 ];
	sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetLaraState() );
	m_Lara.SetWindowText ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT_PTR response = AskToSave("Info");
	DisplayOne ( m_Line );

	CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	WriteWhenOnApply("Info");

	return CTRXPropertyPage::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
	{
		INT_PTR response = AskToSave("Info");
		DisplayOne ( m_Line );
	}

	CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedJournal()
{
	if ( CTRSaveGame::IsValid() )
	{
		//
		UpdateBuffer();

		//
		CTRXDifferences		dlg;
		dlg.m_bRemastered	= FALSE;
		dlg.m_iVersion		= CTRSaveGame::GetVersion();

		dlg.DoModal();

		DisplayValues();

		SetGUIModified ( CTRSaveGame::IsBufferModified() );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedSecretAll()
{
	if ( CTRSaveGame::IsValid() )
	{
		/*
		 *      Set all secrets every time.
		 */
		CTRSaveGame::I()->SetAllSecrets ();
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedMap()
{
	static char szText [ 32 ];

	if ( CTRSaveGame::I(FALSE) != NULL )
	{
		int version		= CTRSaveGame::GetFullVersion();
		int levelIndex	= CTRSaveGame::GetLevelIndex ();

		//
		//
		//	Find Room
		m_West_East.GetWindowText ( szText, sizeof(szText) );
		long dwWestToEast	= atol(szText);
		m_Vertical.GetWindowText ( szText, sizeof(szText) );
		long dwVertical		= atol(szText);
		m_South_North.GetWindowText ( szText, sizeof(szText) );
		long dwSouthToNorth	= atol(szText);
		m_Direction.GetWindowText ( szText, sizeof(szText) );
		WORD wDirection		= atoi(szText);

		//
		if ( IsCustomArea() )
		{
			version		= CUSTOM_GAME;
			levelIndex	= CUSTOM_LEVEL_INDEX;
		}

		CTRXMapAreas dlg;
		dlg.SetTombraiderLevelIndex ( version, levelIndex );
		m_Area.GetWindowText ( szText, sizeof(szText) );
		if ( strlen( szText ) > 0 )
		{
			int iRoom = atoi(szText);
			TR_AREA *pArea = NULL;
			if ( IsCustomArea() )
			{
				pArea = GetTRArea ( CUSTOM_GAME, CUSTOM_LEVEL_INDEX, iRoom );
			}
			else
			{
				pArea = GetTRArea ( version, levelIndex, iRoom );
			}

			if ( pArea )
			{
				TR_CUR_POSITION					currentPosition;
				ZeroMemory ( &currentPosition, sizeof(currentPosition) );
				currentPosition.x				= dwWestToEast;
				currentPosition.y				= dwVertical;
				currentPosition.z				= dwSouthToNorth;
				currentPosition.orientation		= wDirection;

				dlg.SetCurrentArea ( pArea, &currentPosition );
			}
		}

		INT_PTR room = dlg.DoModal();
		if ( room != -1 )
		{
			// Change Values
			TR_AREA *pArea = NULL;
			if ( IsCustomArea() )
			{
				pArea = GetTRArea ( CUSTOM_GAME, CUSTOM_LEVEL_INDEX, (int) room );
			}
			else
			{
				pArea = GetTRArea ( version, levelIndex, (int) room );
			}
			if ( pArea )
			{
				//
				long newX			= ( pArea->x + pArea->x + pArea->xSectors * TR_SECTOR_SIZE ) / 2;
				long newZ			= ( pArea->z + pArea->z + pArea->zSectors * TR_SECTOR_SIZE ) / 2;
				long newY			= pArea->yBottom - FROM_THE_GROUND;

				ZeroMemory ( szText, sizeof(szText) );
				sprintf_s ( szText, sizeof(szText), "%ld", newX );
				m_West_East.SetWindowText ( szText );

				//	T
				ZeroMemory ( szText, sizeof(szText) );
				sprintf_s ( szText, sizeof(szText), "%ld", newY );
				m_Vertical.SetWindowText ( szText );

				//	Z
				ZeroMemory ( szText, sizeof(szText) );
				sprintf_s ( szText, sizeof(szText), "%ld", newZ );
				m_South_North.SetWindowText ( szText );

				ZeroMemory ( szText, sizeof(szText) );
				sprintf_s ( szText, sizeof(szText), "%d", room );
				m_Area.SetWindowText ( szText );

				ChangeRoom();

				SetGUIModified ();
			}
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnChangeEdit()
{
	SetGUIModified ( TRUE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnChangeWestEast()
{
	if ( m_bInitDone && m_bRoomCreated )
	{
		char szText [ 32 ];
		m_West_East.GetWindowText ( szText, sizeof(szText) );
		DWORD x = atol(szText);
		if ( strlen(szText) > 0 )
		{
			m_South_North.GetWindowText ( szText, sizeof(szText) );
			DWORD z = atol(szText);

			RECT roomRect;
			m_Room.GetWindowRect ( &roomRect );
			ScreenToClient ( &roomRect );
			m_Room.SetRoomPoint ( m_Room.ComputeRoomPoint (x,z));
			InvalidateRect ( &roomRect );

			SetGUIModified ( TRUE );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnChangeSouthNorth()
{
	if ( m_bInitDone && m_bRoomCreated )
	{
		char szText [ 32 ];
		m_West_East.GetWindowText ( szText, sizeof(szText) );
		DWORD x = atol(szText);
		m_South_North.GetWindowText ( szText, sizeof(szText) );
		DWORD z = atol(szText);
		if ( strlen(szText) > 0 )
		{
			RECT roomRect;
			m_Room.GetWindowRect ( &roomRect );
			ScreenToClient ( &roomRect );
			m_Room.SetRoomPoint ( m_Room.ComputeRoomPoint (x, z) );
			InvalidateRect ( &roomRect );

			SetGUIModified ( TRUE );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnChangeOrientation()
{
	if ( m_bInitDone && m_bRoomCreated )
	{
		char szText [ 32 ];
		m_Direction.GetWindowText ( szText, sizeof(szText) );

		if ( strlen(szText) > 0 )
		{
			double dfOrientation = atof(szText);
			m_Room.SetOrientation ( dfOrientation );

			RECT roomRect;
			m_Room.GetWindowRect ( &roomRect );
			ScreenToClient ( &roomRect );
			InvalidateRect ( &roomRect );

			SetGUIModified ( TRUE );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::ResizeRoom ( long x, long z, long xSectors, long zSectors, long yTop, long yBottom )
{
	//
	RECT roomRect;
	m_Room.GetWindowRect ( &roomRect );
	ScreenToClient ( &roomRect );
	InvalidateRect ( &roomRect );
	roomRect.bottom		= roomRect.top + xSectors * TR_SECTOR_SIZE / RoomDivider;
	roomRect.right		= roomRect.left + zSectors * TR_SECTOR_SIZE / RoomDivider;
	m_Room.MoveWindow ( &roomRect, TRUE );
	m_Room.SetRoomPoint ( m_Room.ComputeRoomPoint ( x, z ) );
	InvalidateRect ( &roomRect );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::ChangeRoom()
{
	if ( m_bInitDone && m_bRoomCreated )
	{
		char szText [ 32 ];
		m_Area.GetWindowText ( szText, sizeof(szText) );
		WORD wRoom = atoi ( szText );

		if ( strlen(szText) > 0 )
		{
			m_West_East.GetWindowText ( szText, sizeof(szText) );
			DWORD dwWestToEast		= atol(szText);

			m_South_North.GetWindowText ( szText, sizeof(szText) );
			DWORD dwSouthToNorth		= atol(szText);

			m_Direction.GetWindowText ( szText, sizeof(szText) );
			WORD wDirection			= atoi(szText);

			//
			int tombraider	= CTRSaveGame::GetFullVersion ();
			int levelIndex	=  CTRSaveGame::GetLevelIndex ();

			TR_AREA *pArea = NULL;
			if ( IsCustomArea() )
			{
				pArea = GetTRArea ( CUSTOM_GAME, CUSTOM_LEVEL_INDEX, wRoom );
			}
			else
			{
				pArea = GetTRArea ( tombraider, levelIndex, wRoom );
			}
			if ( pArea != NULL )
			{
				ResizeRoom ( pArea->x, pArea->z, pArea->xSectors, pArea->zSectors, pArea->yTop, pArea->yBottom );

				ShowRoom ( pArea, dwWestToEast, dwSouthToNorth, wDirection );
			}

			SetGUIModified ( TRUE );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnChangeRoom()
{
	if ( m_bInitDone && m_bRoomCreated )
	{
		ChangeRoom();
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedAddCustom()
{
	static char szPathname [ MAX_PATH ] = "";
	static char szDirectory [ MAX_PATH ] = "";

	static char szScript [ MAX_PATH ] = "";
	static char szScriptDirectory [ MAX_PATH ] = "";

	int iCurSel = m_Custom_Combo.GetCurSel();
	if ( iCurSel >= 0 && iCurSel < m_Custom_Combo.GetCount() )
	{
		m_Custom_Combo.GetLBText(iCurSel, szDirectory );
		theApp.RemoveFilename ( szDirectory );
	}

	//
	int tombraider	= CTRSaveGame::GetFullVersion ();
	TR_MODE	trMode				= TR_NONE;
	STRUCTLOCATION	*pTable		= NULL;
	char BASED_CODE *pFilter	= NULL;
	char			*pNames		= NULL;

	//
	switch ( tombraider )
	{
		case 1 :
		case 10 :
		case 15 :
		{
			trMode	= TRR1_MODE;
			pTable	= CustomPathnames1;
			pFilter = "Tombraider 1 Files|*.PHD|All Files (*.*)|*.*||";
			pNames	= "*.PHD";
			break;
		}
		case 2:
		case 20 :
		case 25 :
		{
			trMode	= TRR2_MODE;
			pTable	= CustomPathnames2;
			pFilter = "Tombraider 2 Files|*.tr2|All Files (*.*)|*.*||";
			pNames	= "*.tr2";
			break;
		}
		case 3:
		case 30 :
		case 35 :
		{
			trMode	= TRR3_MODE;
			pTable	= CustomPathnames3;
			pFilter = "Tombraider 3 Files|*.tr2|All Files (*.*)|*.*||";
			pNames	= "*.tr2";
			break;
		}
		case 4:
		case 40 :
		case 45 :
		case 49 :
		{
			trMode	= TR4_MODE;
			pTable	= CustomPathnames4;
			pFilter = "Tombraider 4 Files|*.tr4|All Files (*.*)|*.*||";
			pNames	= "*.tr4";
			break;
		}
		case 5:
		case 50 :
		{
			trMode	= TR5_MODE;
			pTable	= CustomPathnames5;
			pFilter = "Tombraider 5 Files|*.trc|All Files (*.*)|*.*||";
			pNames	= "*.trc";
			break;
		}
		default:
		{
			CTRXMessageBox::ShowMessage( "Load Custom Data File",
				"You must load a savegame first to determine the version of Tombraider" );
			return;
		}
	}

	//
	CFileDialog dlgFileDialog ( TRUE, NULL, pNames, NULL, pFilter );
	dlgFileDialog.m_ofn.lpstrInitialDir = szDirectory;

	/*
	 *		Starts a dialog box.
	 */
	if ( dlgFileDialog.DoModal () == IDOK)
	{
		strcpy_s ( szPathname, sizeof(szPathname), dlgFileDialog.GetPathName() );
		InitCustomArea();
		SetCustomLevelName ( CTRXCHEATWINApp::FindFileName ( szPathname ) );
		BOOL bExtracted = ExtractData ( NULL, 99, szPathname, "Custom", trMode, "TRC_", AddToCustomArea );
		if ( bExtracted )
		{
			BOOL bAdded = AddLocation ( pTable, szPathname );
			if ( bAdded )
			{
				m_Custom_Combo.AddString ( szPathname );
			}
		}
		else
		{
			CTRXMessageBox::ShowMessage( "Load Custom Data File", "Fail to read the level data file" );
		}

		//
		//	Read Script
		strcpy_s ( szScriptDirectory, sizeof(szScriptDirectory), szPathname );
		theApp.RemoveFilename ( szScriptDirectory );
		theApp.RemoveFilename ( szScriptDirectory );
		strcpy_s ( szScript, sizeof(szScript), szScriptDirectory );
		strcat_s ( szScript, sizeof(szScript), "\\SCRIPT.DAT" );

		//
		//	Reset
		ZeroMemory ( CustomDataFiles, sizeof(CustomDataFiles) );
		ResetCustomLabels ();

		//
		BOOL bRead = ReadTRXScript ( szScript, szScriptDirectory, tombraider / 10, false, AddToItemsLabels );

		//
		//	When A User choose a file we let him see it
		bool bSearchBestMap = false;
		if ( bSearchBestMap )
		{
			ExtractAfterScript ( tombraider, trMode, pTable, szScriptDirectory, szPathname );
		}

		//
		DisplayValues();
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedRemoveCustom()
{
	static char szPathname [ MAX_PATH ] = "";

	int iCurSel = m_Custom_Combo.GetCurSel();
	if ( iCurSel >= 1 && iCurSel < m_Custom_Combo.GetCount() )
	{
		m_Custom_Combo.GetLBText(iCurSel, szPathname );
	}
	else
	{
		return;
	}

	//
	int tombraider	= CTRSaveGame::GetFullVersion ();
	STRUCTLOCATION	*pTable		= NULL;

	//
	switch ( tombraider )
	{
		case 1 :
		case 10 :
		case 15 :
		{
			pTable	= CustomPathnames1;
			break;
		}
		case 2:
		case 20 :
		case 25 :
		{
			pTable	= CustomPathnames2;
			break;
		}
		case 3:
		case 30 :
		case 35 :
		{
			pTable	= CustomPathnames3;
			break;
		}
		case 4:
		case 40 :
		case 45 :
		case 49 :
		{
			pTable	= CustomPathnames4;
			break;
		}
		case 5:
		case 50 :
		{
			pTable	= CustomPathnames5;
			break;
		}
		default:
		{
			return;
		}
	}

	//
	BOOL bRemoved = RemoveLocation ( pTable, szPathname );
	if ( bRemoved )
	{
		RemoveFromCombo ( &m_Custom_Combo, iCurSel );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//	If bManualChange we will not search best map
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::ChangeCustomCombo(bool bManualChange)
{
	static char szPathname [ MAX_PATH ] = "";
	static char szDirectory [ MAX_PATH ] = "";

	static char szScript [ MAX_PATH ] = "";
	static char szScriptDirectory [ MAX_PATH ] = "";

	if ( m_SetManualCombo )
	{
		m_Custom_Selected = m_Custom_Combo.GetCurSel();
		if ( m_Custom_Selected >= 0 && m_Custom_Selected < m_Custom_Combo.GetCount() )
		{
			m_Custom_Combo.GetLBText(m_Custom_Selected,szPathname );
			int tombraider	= CTRSaveGame::GetFullVersion ();
			TR_MODE	trMode				= TR_NONE;
			STRUCTLOCATION	*pTable		= NULL;

			//
			switch ( tombraider )
			{
				case 1 :
				case 10 :
				case 15 :
				{
					trMode	= TRR1_MODE;
					pTable	= CustomPathnames1;
					break;
				}
				case 2:
				case 20 :
				case 25 :
				{
					trMode	= TRR2_MODE;
					pTable	= CustomPathnames2;
					break;
				}
				case 3:
				case 30 :
				case 35 :
				{
					trMode	= TRR3_MODE;
					pTable	= CustomPathnames3;
					break;
				}
				case 4:
				case 40 :
				case 45 :
				case 49 :
				{
					trMode	= TR4_MODE;
					pTable	= CustomPathnames4;
					break;
				}
				case 5:
				case 50 :
				{
					trMode	= TR5_MODE;
					pTable	= CustomPathnames5;
					break;
				}
				default:
				{
					return;
				}
			}

			/*
			 *		Starts a dialog box.
			 */
			InitCustomArea();

			//
			if ( m_Custom_Selected >= 1 )
			{
				SetCustomLevelName ( CTRXCHEATWINApp::FindFileName ( szPathname ) );
				BOOL bExtracted = ExtractData ( NULL, 99, szPathname, "Custom", trMode, "TRC_", AddToCustomArea );
				if ( ! bExtracted )
				{
					CTRXMessageBox::ShowMessage( "Load Custom Data File", "Fail to read the level data file" );
				}

				//
				//	Read Script
				strcpy_s ( szScriptDirectory, sizeof(szScriptDirectory), szPathname );
				theApp.RemoveFilename ( szScriptDirectory );
				theApp.RemoveFilename ( szScriptDirectory );
				strcpy_s ( szScript, sizeof(szScript), szScriptDirectory );
				strcat_s ( szScript, sizeof(szScript), "\\SCRIPT.DAT" );

				//
				//	Reset
				ZeroMemory ( CustomDataFiles, sizeof(CustomDataFiles) );
				ResetCustomLabels ();

				//
				BOOL bRead = ReadTRXScript ( szScript, szScriptDirectory, tombraider / 10, false, AddToItemsLabels );

				//
				//	Search The best DATA file for this level
				bool bSearchBestMap = true;
				if ( bSearchBestMap && ! bManualChange )
				{
					ExtractAfterScript ( tombraider, trMode, pTable, szScriptDirectory, szPathname );
				}
			}

			//
			DisplayValues();
		}
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::ExtractAfterScript ( int tombraider, TR_MODE trMode, STRUCTLOCATION *pTable,
										const char *pScriptDirectory, const char *pathname )
{
	//
	static char szTRPathname [ MAX_PATH ] = "";

	BOOL bExtracted = FALSE;
	int datafileIndex = SearchDataFileIndex ( CTRSaveGame::I()->GetSaveName() );
	if ( datafileIndex >= 0 )
	{
		strcpy_s ( szTRPathname, sizeof(szTRPathname), pScriptDirectory );
		strcat_s ( szTRPathname, sizeof(szTRPathname), "\\" );
		strcat_s ( szTRPathname, sizeof(szTRPathname), CustomDataFiles[datafileIndex].datafile );
		if ( tombraider / 10 == 4 )
		{
			strcat_s ( szTRPathname, sizeof(szTRPathname), ".tr4" );
		}
		else
		{
			strcat_s ( szTRPathname, sizeof(szTRPathname), ".trc" );
		}

		//
		if ( _strcmpi ( szTRPathname, pathname ) != 0 )
		{
			InitCustomArea();
			SetCustomLevelName ( CTRXCHEATWINApp::FindFileName ( szTRPathname ) );
			bExtracted = ExtractData ( NULL, 99, szTRPathname, "Custom", trMode, "TRC_", AddToCustomArea );
			if ( bExtracted )
			{
				BOOL bAdded = AddLocation ( pTable, szTRPathname );
				if ( bAdded )
				{
					m_Custom_Combo.AddString ( szTRPathname );
				}

				//
				m_SetManualCombo = false;
				SelectCustomFromDir ( szTRPathname );
				int iCurSel = m_Custom_Combo.GetCurSel();
				if ( iCurSel >= 0 )
				{
					m_Custom_Selected = iCurSel;
				}
				m_SetManualCombo = true;
			}
			//	Failed Re do with file
			else
			{
				InitCustomArea();
				SetCustomLevelName ( CTRXCHEATWINApp::FindFileName ( pathname ) );
				bExtracted = ExtractData ( NULL, 99, pathname, "Custom", trMode, "TRC_", AddToCustomArea );
			}
		}

	}

	return bExtracted;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnSelchangeCustomCombo()
{
	ZeroMemory ( CustomDataFiles, sizeof(CustomDataFiles) );
	ResetCustomLabels ();

	//
	ChangeCustomCombo( true );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnDropdownCustomCombo()
{
	// TODO: Add Code Here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::SelectCustomFromDir (const char *pDirectory)
{
	static char szFilename [ MAX_PATH ];

	//	Only If nothing selected
	for ( int i = 1; i < m_Custom_Combo.GetCount(); i++ )
	{
		ZeroMemory ( szFilename, sizeof(szFilename) );
		m_Custom_Combo.GetLBText ( i, szFilename );
		if ( strncmp ( szFilename, pDirectory, strlen(pDirectory) ) == 0 )
		{
			m_Custom_Combo.SetCurSel ( i );
			return TRUE;
		}
	}

	//
	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXInfoPage::SelectCustomFromPath (const char *pathname)
{
	static char szFilename [ MAX_PATH ];
	ZeroMemory ( szFilename, sizeof(szFilename) );

	strcpy_s ( szFilename, sizeof(szFilename), pathname );
	if ( SelectCustomFromDir ( szFilename ) )
	{
		return TRUE;
	}
	theApp.RemoveFilename ( szFilename );
	return SelectCustomFromDir ( szFilename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedSeeCustom()
{
	static char szFilename [ MAX_PATH ];
	ZeroMemory ( szFilename, sizeof(szFilename) );

	//
	m_Filename.GetWindowText(szFilename, sizeof(szFilename));
	if ( strlen(szFilename) > 0 )
	{
		//
		BOOL bSelected = SelectCustomFromPath ( szFilename );
		if ( bSelected )
		{
			ChangeCustomCombo(false);
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::OnBnClickedSort()
{
	//	Sort
	qsort ( LocationPathname, LEN_LOCATION, sizeof ( STRUCTLOCATION ), SortLocation );

	//
	//	Refill Combo
	m_SetManualCombo = false;

	//
	m_Combo.ResetContent();
	for ( int i = 0; i < LEN_LOCATION; i++ )
	{
		if ( strlen(LocationPathname [ i ].szPathname) > 0 )
		{
			m_Combo.AddString ( LocationPathname [ i ].szPathname );
		}
	}
	SetComboSelection();
	m_SetManualCombo = true;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXInfoPage::SetThemeChanged ( bool bDarkTheme )
{
	if ( m_bInitDone )
	{
		// m_ListCtrl.SetColorsNotUsed();
	}

	CTRXPropertyPage::SetThemeChanged ( bDarkTheme );
}
