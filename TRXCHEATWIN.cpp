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
#include "TR4NGSaveGame.h"
#include "TR5SaveGame.h"
#include "TR9SaveGame.h"
#include "TRXGlobal.h"

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

#define BOTH_PROGRAMS   "Tombraider Remastered And Standard Cheat Program"
#define TRR_PROGRAM     "Tombraider Remastered Cheat Program"
#define TR_PROGRAM      "Tombraider Standard Cheat Program"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::EndsWithI ( const char *pText, const char *pEnd )
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
//====================================================================================
//
//====================================================================================
const char *CTRXCHEATWINApp::__strstri ( const char *pString, const char *pSearched )
{
    if ( ( pString == NULL ) || ( pSearched == NULL ) )
    {
        return NULL;
    }

    if ( ( strlen ( pString ) == 0 ) || ( strlen ( pSearched ) == 0 ) )
    {
        return NULL;
    }

    size_t  iLen = strlen ( pSearched );
    if ( iLen == 0 )
    {
        return NULL;
    }

    //
    while ( *pString != L'\0' )
    {
        if ( _strnicmp ( pString, pSearched, iLen ) == 0 )
        {
            return pString;
        }

        pString++;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::ResetCustomLabels ()
{
    //
    //  Reset
    for ( int l = 0; l < TR4NGMAXLEVEL; l++ )
    {
        for ( int b = 0; b < NB_BUTTONS; b++ )
        {
            if ( TR49ItemsNameInd [ l ] [ b ] != NULL )
            {
                free ( TR49ItemsNameInd [ l ] [ b ] );
                TR49ItemsNameInd [ l ] [ b ] = NULL;
            }
        }
    }

    //
    for ( int b = 0; b < NB_BUTTONS; b++ )
    {
        if ( TR49ItemsNameGen [ b ] != NULL )
        {
            free ( TR49ItemsNameGen [ b ] );
            TR49ItemsNameGen [ b ] = NULL;
        }
    }
}

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
    //
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
    int         hConHandle;
    intptr_t    lStdHandle;
    FILE        *fp;

    // Allocate a console for this app
    AllocConsole();

    // Redirect unbuffered STDOUT to the console
    lStdHandle  = (intptr_t) GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle  = _open_osfhandle(lStdHandle, _O_TEXT);
    fp          = _fdopen(hConHandle, "w");
    *stdout     = *fp;

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
    bool    bFound = false;

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
char *CTRXCHEATWINApp::RemoveFileType ( char *pText )
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
    //
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();

}

//
/////////////////////////////////////////////////////////////////////////////
//  Strictly Lower
/////////////////////////////////////////////////////////////////////////////
bool CTRXCHEATWINApp::OSVersionLowerThan ( WORD major, WORD minor )
{
    if ( OSVersion [ 0 ] < major )
    {
        return true;
    }

    if ( OSVersion [ 0 ] == major && OSVersion [ 1 ] < minor )
    {
        return true;
    }

    return false;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Greater Or Equal
/////////////////////////////////////////////////////////////////////////////
bool CTRXCHEATWINApp::OSVersionGreaterThan ( WORD major, WORD minor )
{
    if ( OSVersion [ 0 ] > major )
    {
        return true;
    }

    if ( OSVersion [ 0 ] == major && OSVersion [ 1 ] >= minor )
    {
        return true;
    }

    return false;
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
    //  No Sockets
#if 0
    if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }
#endif

    // InitCommonControls();

    //  Dtop
    // Initialize OLE 2.0 libraries
    BOOL bOle = AfxOleInit();

    AfxEnableControlContainer();

    //
    SetRegistryKey("Macallan");

    GetModule();

    //  Custom Levels
    ZeroMemory ( TR49ItemsNameInd, sizeof(TR49ItemsNameInd) );
    ZeroMemory ( TR49ItemsNameGen, sizeof(TR49ItemsNameGen) );

    //  Instanciate Colors
    CTRXColors::Instanciate();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
#ifndef _WIN64
    // Enable3dControls();          // Call this when using MFC in a shared DLL
#endif
#else
    // Enable3dControlsStatic();    // Call this when linking to MFC statically
#endif

    //
    INT_PTR nResponse   = 0;

    //
    CTRXGlobal::m_iUnchecked        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

    //
    int iTmp    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MIN_NG_SIZE, 0 );
    if ( iTmp == 0 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_MIN_NG_SIZE, TR_NG_MIN_SIZE );
    }
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, 0 );
    if ( iTmp == 0 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, TR_NG_MAX_SIZE );
    }

    //  Savegame of TRNG are generally aound 40 Kb.
    //  But there are some of 900Kb.
    CTRXGlobal::m_iMinNGSize    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MIN_NG_SIZE, TR_NG_MIN_SIZE );
    CTRXGlobal::m_iMaxNGSize    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, TR_NG_MAX_SIZE );
    if ( CTRXGlobal::m_iMaxNGSize > MAX_SAVELENGTH )
    {
        CTRXGlobal::m_iMaxNGSize    = MAX_SAVELENGTH;
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, CTRXGlobal::m_iMaxNGSize );
    }

    //  Search Position Extended helps when Lara is ont in standing Position
    //  Note that modifying position for savegame of TRNG > 900 KB will not work
    iTmp    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 1 );
    }
    CTRXGlobal::m_iSearchPosExt     = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );
    }

    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 0 );
    }

    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUAREFORCE, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUAREFORCE, 1 );
    }

    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_THEME_RESTART, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_THEME_RESTART, 1 );
    }
    
    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_EXTEND_VERTICAL, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_EXTEND_VERTICAL, 0 );
    }

    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TRR_BACKUP, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_TRR_BACKUP, 32 );
    }
    
    //
    iTmp        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR_BACKUP, -1 );
    if ( iTmp == -1 )
    {
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_TR_BACKUP, 4 );
    }
    //
    CTRXColors::m_iDarkTheme        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );
    CTRXColors::m_iSquareCorner     = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 0 );
    CTRXColors::m_iSquareForce      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUAREFORCE, 1 );
    CTRXColors::m_iThemeRestart     = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_THEME_RESTART, 1 );
    CTRXGlobal::m_iExtendVertical   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_EXTEND_VERTICAL, 0 );
    CTRXGlobal::m_iTRRBackup        = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TRR_BACKUP, 32 );
    CTRXGlobal::m_iTRBackup         = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR_BACKUP, 4 );

    //
    int iSizeTR1 = sizeof(TABLE_TR1);
    int iSizeTR2 = sizeof(TABLE_TR2);
    int iSizeTR3 = sizeof(TABLE_TR3);

    //
    //  Module Filename has been read
    //  And Ini file name too
    static char szIndicatorsFilename [ MAX_PATH ];

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr4.txt" );
    CTR4SaveGame::ReadIndicators ( IndicatorsTR4Table, IndicatorsTR4TableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr4ng.txt" );
    CTR4NGSaveGame::ReadIndicators ( IndicatorsTR4NGTable, IndicatorsTR4NGTableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr5.txt" );
    CTR5SaveGame::ReadIndicators ( IndicatorsTR5Table, IndicatorsTR5TableCount, szIndicatorsFilename );

    //
	//	Get OS Version
    OSHVersion  = 0;
    OSLVersion  = 0;
    ZeroMemory ( OSVersion, sizeof(OSVersion) );

    //
	const char *kernel32Dll = "kernel32.dll";
	DWORD dwHandle = NULL;
	DWORD dwLen = GetFileVersionInfoSize( kernel32Dll, &dwHandle);

	BYTE *lpData = (BYTE *) malloc ( dwLen );
	BOOL bInfoFound = GetFileVersionInfo( kernel32Dll, dwHandle, dwLen, lpData);
	VS_FIXEDFILEINFO *lpBuffer = NULL;
	if ( bInfoFound )
	{
		UINT uLen = 0;
		BOOL itemFound = VerQueryValueW ( lpData, L"\\", (LPVOID *)&lpBuffer, &uLen );
	}

	if ( lpBuffer != NULL )
	{
        OSHVersion      = lpBuffer->dwFileVersionMS;
        OSLVersion      = lpBuffer->dwFileVersionLS;

		OSVersion [ 0 ] = HIWORD(OSHVersion);
		OSVersion [ 1 ] = LOWORD(OSHVersion);
		OSVersion [ 2 ] = HIWORD(OSLVersion);
		OSVersion [ 3 ] = LOWORD(OSLVersion);
	}

	free ( lpData );

    //
    //
    if ( m_lpCmdLine != NULL )
    {
        char *pCommandLine = SkipSpaces ( m_lpCmdLine );

        //
        if (    _stricmp ( pCommandLine, "-help" ) == 0 ||
                _stricmp ( pCommandLine, "help" ) == 0  ||
                _stricmp ( pCommandLine, "-h" ) == 0 )
        {
            CTRXHelpDialog dlg;
            nResponse = dlg.DoModal();
        }
        else if (   _stricmp ( pCommandLine, "-indicator" ) == 0 ||
                    _stricmp ( pCommandLine, "-indicators" ) == 0 )
        {
            //
            strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
            RemoveFileType ( szIndicatorsFilename );
            strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr4.txt" );
            CTR4SaveGame::WriteIndicators ( IndicatorsTR4Table, IndicatorsTR4TableCount, szIndicatorsFilename );

            //
            strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
            RemoveFileType ( szIndicatorsFilename );
            strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr4ng.txt" );
            CTR4NGSaveGame::WriteIndicators ( IndicatorsTR4NGTable, IndicatorsTR4NGTableCount, szIndicatorsFilename );

            //
            strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
            RemoveFileType ( szIndicatorsFilename );
            strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), ".tr5.txt" );
            CTR5SaveGame::WriteIndicators ( IndicatorsTR5Table, IndicatorsTR5TableCount, szIndicatorsFilename );
        }
        //
        else if (   _stricmp ( pCommandLine, "-123" ) == 0              ||
                    _stricmp ( pCommandLine, "-r" ) == 0                    ||
                    _stricmp ( pCommandLine, "-remastered" ) == 0       )
        {
            CTRXPropertySheetRemastered dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        else if (   _strnicmp ( pCommandLine, "-123 ", strlen("-123 ") ) == 0               ||
                    _strnicmp ( pCommandLine, "-r ", strlen("-r ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-remastered ", strlen("-remastered ") ) == 0     )
        {
            CTRXPropertySheetRemastered dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );

            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        else if (   _stricmp ( pCommandLine, "-1" ) == 0                    ||
                    _stricmp ( pCommandLine, "-2" ) == 0                    ||
                    _stricmp ( pCommandLine, "-3" ) == 0                    ||
                    _stricmp ( pCommandLine, "-4" ) == 0                    ||
                    _stricmp ( pCommandLine, "-5" ) == 0                    ||
                    _stricmp ( pCommandLine, "-s" ) == 0                    ||
                    _stricmp ( pCommandLine, "-standard" ) == 0     )
        {
            CTRXPropertySheetStandard   dlg ( TR_PROGRAM );
            dlg.SetApply ( TRUE );
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        else if (   _strnicmp ( pCommandLine, "-1 ", strlen("-1 ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-2 ", strlen("-2 ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-3 ", strlen("-3 ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-4 ", strlen("-4 ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-5 ", strlen("-5 ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-s ", strlen("-s ") ) == 0                   ||
                    _strnicmp ( pCommandLine, "-standard ", strlen("-standard ") ) == 0     )
        {
            CTRXPropertySheetStandard   dlg ( TR_PROGRAM );
            dlg.SetApply ( TRUE );

            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
#ifdef _DEBUG
        else if ( _strnicmp ( pCommandLine, "-analyze", strlen("-analyze") ) == 0L )
        {
            //
            SetStdOutToNewConsole();

            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        //  Read Script
        else if ( _strnicmp ( pCommandLine, "-rs4", strlen("-rs4") ) == 0L )
        {
            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        //  Read Script
        else if ( _strnicmp ( pCommandLine, "-ws4", strlen("-ws4") ) == 0L )
        {
            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        //  Read Script
        else if ( _strnicmp ( pCommandLine, "-rs5", strlen("-rs5") ) == 0L )
        {
            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        //  Read Script
        else if ( _strnicmp ( pCommandLine, "-ws5", strlen("-ws5") ) == 0L )
        {
            //
            char *pFilename = strchr ( pCommandLine, ' ' );
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
        else if ( _stricmp ( pCommandLine, "-maps" ) == 0L )
        {
            CTRXAllMaps dlg;
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        //  Custom
        else if (   strncmp ( pCommandLine, "-tr1", strlen("-tr1") ) == 0L ||
                    strncmp ( pCommandLine, "-tr2", strlen("-tr2") ) == 0L ||
                    strncmp ( pCommandLine, "-tr3", strlen("-tr3") ) == 0L ||
                    strncmp ( pCommandLine, "-tr4", strlen("-tr4") ) == 0L ||
                    strncmp ( pCommandLine, "-tr5", strlen("-tr5") ) == 0L  )
        {
            //
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                strcpy_s ( szPathname, sizeof(szPathname), pFilename );
                RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                //
                TR_MODE trMode;
                if ( strncmp ( pCommandLine, "-tr1", strlen("-tr1") ) == 0L )
                {
                    trMode  = TRR1_MODE;
                }
                else if ( strncmp ( pCommandLine, "-tr2", strlen("-tr2") ) == 0L )
                {
                    trMode  = TRR2_MODE;
                }
                else if ( strncmp ( pCommandLine, "-tr3", strlen("-tr3") ) == 0L )
                {
                    trMode  = TRR3_MODE;
                }
                else if ( strncmp ( pCommandLine, "-tr4", strlen("-tr4") ) == 0L )
                {
                    trMode  = TR4_MODE;
                }
                else if ( strncmp ( pCommandLine, "-tr5", strlen("-tr5") ) == 0L )
                {
                    trMode  = TR5_MODE;
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
                CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
                m_pMainWnd = &dlg;
                nResponse = dlg.DoModal();
            }
        }
        //
        else if ( __strstri ( pCommandLine, "savegame." ) != NULL )
        {
            strcpy_s ( szPathname, sizeof(szPathname), pCommandLine );
            RemoveEnclosingQuotes ( szPathname, sizeof(szPathname) );

            //
            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );
            if ( PathFileExists(szFullPathname) )
            {
                HANDLE hFile = CreateFile (
                    szFullPathname,             //  _In_      LPCTSTR lpFileName,
                    GENERIC_READ,               //  _In_      DWORD dwDesiredAccess,
                    FILE_SHARE_READ,            //  _In_      DWORD dwShareMode,
                    NULL,                       //  _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                    OPEN_EXISTING,              //  _In_      DWORD dwCreationDisposition,
                    FILE_ATTRIBUTE_NORMAL,      //  _In_      DWORD dwFlagsAndAttributes,
                    NULL                        //  _In_opt_  HANDLE hTemplateFile
                );

                DWORD dwSizeHigh    = 0;
                DWORD dwSize        = INVALID_FILE_SIZE;
                if ( hFile != INVALID_HANDLE_VALUE )
                {
                    dwSize        = GetFileSize ( hFile, &dwSizeHigh );
                    CloseHandle ( hFile );
                }

                //
                if ( dwSize != INVALID_FILE_SIZE  )
                {
                    //
                    if ( dwSize == TR123LEVELSIZE )
                    {
                        CTRXPropertySheetRemastered dlg ( TRR_PROGRAM );
                        dlg.SetApply ( TRUE );
                        dlg.SetParmPathname ( szFullPathname );
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                    }
                    //  TR123
                    else
                    {
                        CTRXPropertySheetStandard   dlg ( TR_PROGRAM );
                        dlg.SetApply ( TRUE );
                        dlg.SetParmPathname ( szFullPathname );
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                    }
                }
                else
                {
                    CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
                    m_pMainWnd = &dlg;
                    nResponse = dlg.DoModal();
                }
            }
            else
            {
                CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
                m_pMainWnd = &dlg;
                nResponse = dlg.DoModal();
            }
        }
        //
        else
        {
            CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
    }
    //
    else
    {
        CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
        m_pMainWnd = &dlg;
        nResponse = dlg.DoModal();
    }

    //
    if (nResponse == IDOK)
    {
        //
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        //
        //  dismissed with Cancel
    }

    //
    ResetCustomLabels ();

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
    // 
    // WritePrivateProfileSection  could be used to delete a section;

    BOOL bWritten =
        WritePrivateProfileString (
            section,            //  __in  LPCTSTR lpAppName,
            keyName,            //  __in  LPCTSTR lpKeyName,
            pValue,             //  __in  LPCTSTR lpString,
            InitFileName        //  __in  LPCTSTR lpFileName
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
            section,                        //  __in   LPCTSTR lpAppName,
            keyName,                        //  __in   LPCTSTR lpKeyName,
            pDefault,                       //  __in   LPCTSTR lpDefault,
            pValue,                         //  __out  LPTSTR lpReturnedString,
            (DWORD) iValue,                 //  __in   DWORD nSize,
            InitFileName                    //  __in   LPCTSTR lpFileName
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
//      Search Init File
////////////////////////////////////////////////////////////////////////
bool CTRXCHEATWINApp::SearchInitFile ( const char *pInitFileName, size_t iInitFileName )
{
    static char initName [ MAX_PATH ];
    static char initPathName [ MAX_PATH ];
    char    *pVariable      = NULL;
    size_t  requiredSize    = 0;

    strcpy_s ( initName, sizeof(initName), FindFileName ( pInitFileName ) );

    //
    //      First If ini file is here.
    if ( PathFileExists ( pInitFileName ) )
    {
        return true;
    }

    //
    //      Then Search Environnement
    getenv_s ( &requiredSize, NULL, 0, "PATH" );
    if ( requiredSize == 0 )
    {
        return false;
    }

    //
    size_t  iVariable   = ( requiredSize + 1 ) * sizeof(char) + 1;
    pVariable   = ( char * ) malloc ( iVariable );

    getenv_s ( &requiredSize, pVariable, requiredSize + 1, "PATH" );

    //      Treat Token for PATH
    char    strDelimit[]   = ";";
    char    *strToken   = NULL;
    char    *context    = NULL;

    //
    //      Treat Tokens
    strToken = strtok_s ( pVariable, strDelimit, &context);
    while( strToken != NULL )
    {
        //
        //      Test Filename
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

        //      Get next token:
        strToken = strtok_s( NULL, strDelimit, &context);
    }

    free ( pVariable );

    return false;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::GetModule ()
{
    DWORD dwResult =
        GetModuleFileName ( NULL,                       //  __in_opt  HMODULE hModule,
                            ModuleFileName,             //  __out     LPTSTR lpFilename,
                            sizeof(ModuleFileName)      //  __in      DWORD nSize
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
    //      Search Init File and in Path
    SearchInitFile ( InitFileName, sizeof(InitFileName) );

    return;

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::ReStartProgram(CWnd *pWnd)
{

#ifndef _DEBUG
    if ( CTRXColors::m_iThemeRestart )
    {
        DWORD dwResult =
            GetModuleFileName ( NULL,                       //  __in_opt  HMODULE hModule,
                                ModuleFileName,             //  __out     LPTSTR lpFilename,
                                sizeof(ModuleFileName)      //  __in      DWORD nSize
                                );

        if ( pWnd != NULL )
        {
            ShellExecute ( NULL, NULL, ModuleFileName, m_lpCmdLine, NULL, SW_SHOW  );
            ::SendMessage ( pWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, NULL );
        }
    }
#endif
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::AcceptedPattern ( const char *pName )
{
    if ( __strstri ( pName, ".bak" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pName, ".zip" ) != NULL )
    {
        return FALSE;
    }

    if ( EndsWithI ( pName, ".sav" ) != NULL )
    {
        return FALSE;
    }

    return TRUE;
}