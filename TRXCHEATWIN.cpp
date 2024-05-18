// TR3CHEATWIN.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxdisp.h>

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TR9SaveGame.h"
#include "TR2SaveGame.h"
#include "TR3SaveGame.h"
#include "TR4SaveGame.h"
#include "TR5SaveGame.h"
#include "TR9SaveGame.h"
#include "TRXGlobal.h"

#include "TRXContainer.h"
#include "TRXPropertySheetAll.h"
#include "TRXPropertySheetStandard.h"
#include "TRXPropertySheetRemastered.h"
#include "TRXAllMaps.h"
#include "TRXMessageBox.h"
#include "TR_Areas.h"
#include "TRXHelpDialog.h"
#include "TRLabelItems.h"

#include "ReadTR2/ReadTRX.h"
#include "ReadTR2/ReadTRXScript.h"

#include "TRXColors.h"

extern BOOL TraceMode;

#include "AutomaticVersionHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
/////////////////////////////////////////////////////////////////////////////
// CTRXCHEATWINApp
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXCHEATWINApp, CWinApp)
	//{{AFX_MSG_MAP(CTRXCHEATWINApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXCHEATWINApp construction
//
/////////////////////////////////////////////////////////////////////////////
CTRXCHEATWINApp::CTRXCHEATWINApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// The one and only CTRXCHEATWINApp object

CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CTR3SaveGame variable
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void SetStdOutToNewConsole()
{
    int hConHandle;
    long lStdHandle;
    FILE *fp;

    // Allocate a console for this app
    AllocConsole();

    // Redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;

    setvbuf(stdout, NULL, _IONBF, 0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::RemoveEnclosingQuotes ( char *pText, size_t iLength )
{
	if ( strlen(pText) >= 2 && *pText == '"' && pText [strlen(pText)-1] == '"'  )
	{
		memmove ( pText, pText + 1, strlen(pText + 1) + 1);
		if ( pText [strlen(pText)-1] == '"' )
		{
			pText [strlen(pText)-1] = 0;
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTRXCHEATWINApp::SkipSpaces ( char *pText )
{
	while ( *pText == ' ' )
	{
		pText++;
	}

	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTRXCHEATWINApp::FindFileName ( char *pText )
{
	for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
	{
		if ( pText [ i ] == '\\' )
		{
			return pText + i + 1;
		}
		if ( pText [ i ] == ':' )
		{
			return pText + i + 1;
		}
	}

	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTRXCHEATWINApp::RemoveFilename ( char *pText )
{
	//
	bool	bFound = false;

	//
	for ( int i = (int) strlen(pText) - 1; i > 0; i-- )
	{
		if ( pText [ i ] == '\\' )
		{
			pText [ i ] = '\0';
			bFound = true;
			break;
		}
		if ( pText [ i ] == ':' )
		{
			pText [ i + 1 ] = '\0';
			bFound = true;
			break;
		}
	}

	if ( ! bFound )
	{
		*pText = '\0';
	}

	//
	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXCHEATWINApp::FindFileName ( const char *pText )
{
	for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
	{
		if ( pText [ i ] == '\\' )
		{
			return pText + i + 1;
		}
		if ( pText [ i ] == ':' )
		{
			return pText + i + 1;
		}
	}

	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *RemoveFileType ( char *pText )
{
	for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
	{
		if ( pText [ i ] == '.' )
		{
			pText [ i ] = '\0';
			return pText;
		}

		if ( pText [ i ] == '\\' || pText [ i ] == ':' )
		{
			return pText;
		}
	}

	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();

}

//
/////////////////////////////////////////////////////////////////////////////
// CTRXCHEATWINApp initialization
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::InitInstance()
{
	//
	static char szFullPathname [ MAX_PATH ];
	static char szPathname [ MAX_PATH ];
	static char szDirectory [ MAX_PATH ];

	//
	//	No Sockets
#if 0
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
#endif

	//	No OLE
#if 0
	AfxEnableControlContainer();
#endif

	SetRegistryKey("Macallan");

	GetModule();

	//	Custom Levels
	ZeroMemory ( TR49ItemsNameInd, sizeof(TR49ItemsNameInd) );
	ZeroMemory ( TR49ItemsNameGen, sizeof(TR49ItemsNameGen) );

	//	Instanciate Colors
	CTRXColors::Instanciate();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
#ifndef _WIN64
	// Enable3dControls();			// Call this when using MFC in a shared DLL
#endif
#else
	// Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//
	INT_PTR nResponse	= 0;

	CTRXGlobal::m_iUnchecked	= theApp.GetProfileInt( "Settings", "Unchecked", 0 );

	//
	int iTmp	= theApp.GetProfileInt( "Settings", "Min NG Size", 0 );
	if ( iTmp == 0 )
	{
		theApp.WriteProfileInt( "Settings", "Min NG Size", TR_NG_MIN_SIZE );
	}
	iTmp		= theApp.GetProfileInt( "Settings", "Max NG Size", 0 );
	if ( iTmp == 0 )
	{
		theApp.WriteProfileInt( "Settings", "Max NG Size", TR_NG_MAX_SIZE );
	}

	CTRXGlobal::m_iMinNGSize	= theApp.GetProfileInt( "Settings", "Min NG Size", TR_NG_MIN_SIZE );
	CTRXGlobal::m_iMaxNGSize	= theApp.GetProfileInt( "Settings", "Max NG Size", TR_NG_MAX_SIZE );
	if ( CTRXGlobal::m_iMaxNGSize > MAX_SAVELENGTH )
	{
		CTRXGlobal::m_iMaxNGSize	= MAX_SAVELENGTH;
		theApp.WriteProfileInt( "Settings", "Max NG Size", CTRXGlobal::m_iMaxNGSize );
	}

	//
	iTmp		= theApp.GetProfileInt( "Settings", "DarkTheme", -1 );
	if ( iTmp == -1 )
	{
		theApp.WriteProfileInt( "Settings", "DarkTheme", 0 );
	}

	//
	CTRXGlobal::m_iDarkTheme	= theApp.GetProfileInt( "Settings", "DarkTheme", 0 );

	//
	int iSizeTR1 = sizeof(TABLE_TR1);
	int iSizeTR2 = sizeof(TABLE_TR2);
	int iSizeTR3 = sizeof(TABLE_TR3);

	//
	if ( m_lpCmdLine != NULL )
	{
		//
		if (	_stricmp ( m_lpCmdLine, "-help" ) == 0	||
				_stricmp ( m_lpCmdLine, "help" ) == 0	||
				_stricmp ( m_lpCmdLine, "-h" ) == 0 )
		{
			CTRXHelpDialog dlg;
			nResponse = dlg.DoModal();
		}
		//
		else if (	_stricmp ( m_lpCmdLine, "-123" ) == 0				||
					_stricmp ( m_lpCmdLine, "-r" ) == 0					||
					_stricmp ( m_lpCmdLine, "-remastered" ) == 0		)
		{
			CTRXPropertySheetRemastered	dlg ( "Tombraider Remastered Cheat Program" );
			dlg.SetApply ( TRUE );
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		//
		else if (	_strnicmp ( m_lpCmdLine, "-123 ", strlen("-123 ") ) == 0				||
					_strnicmp ( m_lpCmdLine, "-r ", strlen("-r ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-remastered ", strlen("-remastered ") ) == 0		)
		{
			CTRXPropertySheetRemastered	dlg ( "Tombraider Remastered Cheat Program" );
			dlg.SetApply ( TRUE );

			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				dlg.SetParmPathname ( szFullPathname );
			}
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		else if (	_stricmp ( m_lpCmdLine, "-1" ) == 0					||
					_stricmp ( m_lpCmdLine, "-2" ) == 0					||
					_stricmp ( m_lpCmdLine, "-3" ) == 0					||
					_stricmp ( m_lpCmdLine, "-4" ) == 0					||
					_stricmp ( m_lpCmdLine, "-5" ) == 0					||
					_stricmp ( m_lpCmdLine, "-s" ) == 0					||
					_stricmp ( m_lpCmdLine, "-standard" ) == 0		)
		{
			CTRXPropertySheetStandard	dlg ( "Tombraider Standard Cheat Program" );
			dlg.SetApply ( TRUE );
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		else if (	_strnicmp ( m_lpCmdLine, "-1 ", strlen("-1 ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-2 ", strlen("-2 ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-3 ", strlen("-3 ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-4 ", strlen("-4 ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-5 ", strlen("-5 ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-s ", strlen("-s ") ) == 0					||
					_strnicmp ( m_lpCmdLine, "-standard ", strlen("-standard ") ) == 0		)
		{
			CTRXPropertySheetStandard	dlg ( "Tombraider Standard Cheat Program" );
			dlg.SetApply ( TRUE );

			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				dlg.SetParmPathname ( szFullPathname );
			}

			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		//
		else if ( _stricmp ( m_lpCmdLine, "-resizable" ) == 0L )
		{
			CTRXGlobal::m_iDarkTheme	= 0;
			CTRXContainer dlg;
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		//
#ifdef _DEBUG
		else if ( _strnicmp ( m_lpCmdLine, "-analyze", strlen("-analyze") ) == 0L )
		{
			//
			SetStdOutToNewConsole();

			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				CTR9SaveGame *pTombraider = CTR9SaveGame::I();
				BOOL bRead = pTombraider->ReadFile(szFullPathname);
				if ( bRead )
				{
					TraceMode = TRUE;
					pTombraider->Load ( );
				}
				delete pTombraider;
			}
		}
#endif
		//
		//	Read Script
		else if ( _strnicmp ( m_lpCmdLine, "-rs4", strlen("-rs4") ) == 0L )
		{
			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
				RemoveFilename ( szDirectory );
				BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 4 );
			}
		}
		//
		//	Read Script
		else if ( _strnicmp ( m_lpCmdLine, "-ws4", strlen("-ws4") ) == 0L )
		{
			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
				RemoveFilename ( szDirectory );
				BOOL bWritten = WriteTRXScript ( szFullPathname, szDirectory, 4 );
			}
		}
		//
		//	Read Script
		else if ( _strnicmp ( m_lpCmdLine, "-rs5", strlen("-rs5") ) == 0L )
		{
			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
				RemoveFilename ( szDirectory );
				BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 5 );
			}
		}
		//
		//	Read Script
		else if ( _strnicmp ( m_lpCmdLine, "-ws5", strlen("-ws5") ) == 0L )
		{
			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
				RemoveFilename ( szDirectory );
				BOOL bWritten = WriteTRXScript ( szFullPathname, szDirectory, 5 );
			}
		}
		//
		else if ( _stricmp ( m_lpCmdLine, "-maps" ) == 0L )
		{
			CTRXAllMaps dlg;
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
		//
		//	Custom
		else if (	strncmp ( m_lpCmdLine, "-tr1", strlen("-tr1") ) == 0L ||
					strncmp ( m_lpCmdLine, "-tr2", strlen("-tr2") ) == 0L ||
					strncmp ( m_lpCmdLine, "-tr3", strlen("-tr3") ) == 0L ||
					strncmp ( m_lpCmdLine, "-tr4", strlen("-tr4") ) == 0L ||
					strncmp ( m_lpCmdLine, "-tr5", strlen("-tr5") ) == 0L	)
		{
			//
			char *pFilename = strchr ( m_lpCmdLine, ' ' );
			if ( pFilename != NULL )
			{
				pFilename = SkipSpaces ( pFilename );
				strcpy_s ( szPathname, sizeof(szPathname), pFilename );
				RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

				LPSTR lpFilepart = NULL;
				GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

				//
				TR_MODE	trMode;
				if ( strncmp ( m_lpCmdLine, "-tr1", strlen("-tr1") ) == 0L )
				{
					trMode	= TRR1_MODE;
				}
				else if ( strncmp ( m_lpCmdLine, "-tr2", strlen("-tr2") ) == 0L )
				{
					trMode	= TRR2_MODE;
				}
				else if ( strncmp ( m_lpCmdLine, "-tr3", strlen("-tr3") ) == 0L )
				{
					trMode	= TRR3_MODE;
				}
				else if ( strncmp ( m_lpCmdLine, "-tr4", strlen("-tr4") ) == 0L )
				{
					trMode	= TR4_MODE;
				}
				else if ( strncmp ( m_lpCmdLine, "-tr5", strlen("-tr5") ) == 0L )
				{
					trMode	= TR5_MODE;
				}

				//
				InitCustomArea();
				SetCustomLevelName ( FindFileName ( szFullPathname ) );
				BOOL bExtracted = ExtractData ( NULL, 99, szFullPathname, "Custom", trMode, "TRC_", AddToCustomArea );
				if ( ! bExtracted )
				{
					CTRXMessageBox::ShowMessage( "Load Custom Data file", "Fails to read the level data file" );
				}

				//
				CTRXPropertySheetAll	dlg ( "Tombraider Remastered And Standard Cheat Program" );
				m_pMainWnd = &dlg;
				nResponse = dlg.DoModal();
			}
		}
		//
		else
		{
			CTRXPropertySheetAll	dlg ( "Tombraider Remastered And Standard Cheat Program" );
			m_pMainWnd = &dlg;
			nResponse = dlg.DoModal();
		}
	}
	//
	else
	{
		CTRXPropertySheetAll	dlg ( "Tombraider Remastered And Standard Cheat Program" );
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}

	//
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::WriteProfileIniFile ( const char *section,  const char *keyName, const char *pValue )
{
	BOOL bWritten = 
		WritePrivateProfileString (
			section,			//	__in  LPCTSTR lpAppName,
			keyName,			//	__in  LPCTSTR lpKeyName,
			pValue,				//	__in  LPCTSTR lpString,
			InitFileName		//	__in  LPCTSTR lpFileName
		);
	if ( bWritten <= 0 )
	{
		return FALSE;
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::WriteProfileInt ( const char *section,  const char *keyName, int value )
{
	char szText [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%d", value );
	return WriteProfileIniFile ( section, keyName, szText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::WriteProfileDouble ( const char *section, const char *keyName, double value )
{
	char szText [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%f", value );
	return WriteProfileIniFile ( section, keyName, szText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::WriteProfileString ( const char *section,  const char *keyName, const char *pValue )
{
	return WriteProfileIniFile ( section, keyName, pValue );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::ReadProfileIniFile ( const char *section, const char *keyName, char *pValue, size_t iValue, const char *pDefault )
{
	ZeroMemory ( pValue, iValue * sizeof(char) );

	DWORD dwResult = 
		GetPrivateProfileString(
			section,						//	__in   LPCTSTR lpAppName,
			keyName,						//	__in   LPCTSTR lpKeyName,
			pDefault,						//	__in   LPCTSTR lpDefault,
			pValue,							//	__out  LPTSTR lpReturnedString,
			(DWORD) iValue,					//	__in   DWORD nSize,
			InitFileName					//	__in   LPCTSTR lpFileName
		);
	if ( dwResult <= 0 )
	{
		return FALSE;
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
UINT CTRXCHEATWINApp::GetProfileInt ( const char *section,  const char *keyName, int defaultValue )
{
	UINT result = 0;
	char szText [ 64 ];
	char szDefault [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%d", defaultValue );
	strcpy_s ( szDefault, szText );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), szDefault );
	if ( ! bRead )
	{
		return defaultValue;
	}
	result = atoi ( szText );
	return result;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
double CTRXCHEATWINApp::GetProfileDouble ( const char *section,  const char *keyName, double defaultValue )
{
	double result = 0;

	char szText [ 64 ];
	char szDefault [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%f", defaultValue );
	strcpy_s ( szDefault, szText );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), szDefault );
	if ( ! bRead )
	{
		return defaultValue;
	}
	result = atof ( szText );

	return result;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CString CTRXCHEATWINApp::GetProfileString( const char *section,  const char *keyName, const char *pDefaultValue )
{
	if ( pDefaultValue == NULL )
	{
		pDefaultValue = "";
	}
	CString result;

	char szText [ MAX_PATH ];
	strcpy_s ( szText, sizeof(szText), pDefaultValue );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), pDefaultValue );
	if ( ! bRead )
	{
		return pDefaultValue;
	}
	result = szText;

	return result;
}


//
////////////////////////////////////////////////////////////////////////
//		Search Init File
////////////////////////////////////////////////////////////////////////
bool CTRXCHEATWINApp::SearchInitFile ( const char *pInitFileName, size_t iInitFileName )
{
	static char	initName [ MAX_PATH ];
	static char	initPathName [ MAX_PATH ];
	char	*pVariable		= NULL;
	size_t	requiredSize	= 0;

	strcpy_s ( initName, sizeof(initName), FindFileName ( pInitFileName ) );

	//
	//		First If ini file is here.
	if ( PathFileExists ( pInitFileName ) )
	{
		return true;
	}

	//
	//		Then Search Environnement
	getenv_s ( &requiredSize, NULL, 0, "PATH" );
	if ( requiredSize == 0 )
	{
		return false;
	}

	//
	size_t	iVariable	= ( requiredSize + 1 ) * sizeof(char) + 1;
	pVariable	= ( char * ) malloc ( iVariable );

	getenv_s ( &requiredSize, pVariable, requiredSize + 1, "PATH" );

	//		Treat Token for PATH
	char	strDelimit[]   = ";";
	char	*strToken	= NULL;
	char	*context	= NULL;

	//
	//		Treat Tokens
	strToken = strtok_s ( pVariable, strDelimit, &context);
	while( strToken != NULL )
	{
		//
		//		Test Filename
		strcpy_s ( initPathName, sizeof(initPathName), strToken );
		if ( ( strlen(initPathName) > 0 ) && ( initPathName [ strlen(initPathName) - 1 ] != '\\' ) )
		{
			strcat_s ( initPathName, sizeof(initPathName), "\\" );
		}
		strcat_s ( initPathName, sizeof(initPathName), initName );

		if ( PathFileExists ( initPathName ) )
		{
			strcpy_s ( InitFileName, iInitFileName, initPathName );
			free ( pVariable );
			return true;
		}

		//		Get next token: 
		strToken = strtok_s( NULL, strDelimit, &context);
	}

	free ( pVariable );

	return false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL EndsWithI ( const char *pText, const char *pEnd )
{
	if ( strlen(pText) >= strlen(pEnd) )
	{
		if ( _stricmp ( pText + strlen(pText) - strlen(pEnd), pEnd ) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::GetModule ()
{
	DWORD dwResult = 
		GetModuleFileName ( NULL,						//  __in_opt  HMODULE hModule,
							ModuleFileName,				//	__out     LPTSTR lpFilename,
							sizeof(ModuleFileName)		//	__in      DWORD nSize
							);
	strcpy_s ( InitFileName, sizeof(InitFileName), ModuleFileName );
	BOOL endsWith = EndsWithI ( InitFileName, "x64.exe" );
	if ( endsWith )
	{
		InitFileName [ strlen(InitFileName) - strlen("x64.exe") ] = '\0';
	}
	else
	{
		RemoveFileType ( InitFileName );
	}

	strcat_s ( InitFileName, sizeof(InitFileName), ".ini" );


	//
	//		Search Init File and in Path
	SearchInitFile ( InitFileName, sizeof(InitFileName) );

	return;

}
