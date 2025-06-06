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
#include "TRXPropertySheetRemastered456.h"
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
int CTRXCHEATWINApp::__strnicmp ( const char *pString, const char *pBegining )
{
    if ( pBegining == NULL )
    {
        return 1;
    }

    if ( strlen(pBegining) == 0 )
    {
        return 1;
    }

    return _strnicmp ( pString, pBegining, strlen(pBegining) );
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

    //
    for ( int l = 0; l < TR4NGMAXLEVEL; l++ )
    {
        for ( int b = 0; b < 4; b++ )
        {
            if ( TR39PuzzleName [ l ].label1 != NULL )
            {
                free ( TR39PuzzleName [ l ].label1 );
                TR39PuzzleName [ l ].label1 = NULL;
            }
            if ( TR39PuzzleName [ l ].label2 != NULL )
            {
                free ( TR39PuzzleName [ l ].label2 );
                TR39PuzzleName [ l ].label2 = NULL;
            }
            if ( TR39PuzzleName [ l ].label3 != NULL )
            {
                free ( TR39PuzzleName [ l ].label3 );
                TR39PuzzleName [ l ].label3 = NULL;
            }
            if ( TR39PuzzleName [ l ].label4 != NULL )
            {
                free ( TR39PuzzleName [ l ].label4 );
                TR39PuzzleName [ l ].label4 = NULL;
            }

            //
            if ( TR39PickupName [ l ].label1 != NULL )
            {
                free ( TR39PickupName [ l ].label1 );
                TR39PickupName [ l ].label1 = NULL;
            }
            if ( TR39PickupName [ l ].label2 != NULL )
            {
                free ( TR39PickupName [ l ].label2 );
                TR39PickupName [ l ].label2 = NULL;
            }

            //
            if ( TR39KeyName [ l ].label1 != NULL )
            {
                free ( TR39KeyName [ l ].label1 );
                TR39KeyName [ l ].label1 = NULL;
            }
            if ( TR39KeyName [ l ].label2 != NULL )
            {
                free ( TR39KeyName [ l ].label2 );
                TR39KeyName [ l ].label2 = NULL;
            }
            if ( TR39KeyName [ l ].label3 != NULL )
            {
                free ( TR39KeyName [ l ].label3 );
                TR39KeyName [ l ].label3 = NULL;
            }
            if ( TR39KeyName [ l ].label4 != NULL )
            {
                free ( TR39KeyName [ l ].label4 );
                TR39KeyName [ l ].label4 = NULL;
            }
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
//  Copyt between quotes or end at space or end at NULL
//  Return the next string pointer
/////////////////////////////////////////////////////////////////////////////
char *CTRXCHEATWINApp::CopyBetweenQuotes ( char *pTarget, size_t iLength, char *pSource )
{
    char pEnd = ' ';

    if ( *pSource == '\'' )
    {
        pEnd = *pSource;
        pSource++;
    }
    else if ( *pSource == '\"' )
    {
        pEnd =  *pSource;
        pSource++;
    }

    while ( *pSource != '\0' && *pSource != pEnd && iLength > 1 )
    {
        *pTarget = *pSource;
        iLength--;
        pSource++;
        pTarget++;
    }

    if ( *pSource != '\0' )
    {
        if ( *pSource == pEnd )
        {
            pSource++;
        }
    }

    if ( iLength > 1 )
    {
        *pTarget = '\0';
        iLength--;
    }

    return pSource;
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
//====================================================================================
//  Search String and Replace Ignore Case
//====================================================================================
bool CTRXCHEATWINApp::__strrepiA (   char *lineRead, size_t sizeType,
                                    const char *pSearched, const char *pReplaced, bool bGlobal )
{
    bool replaced   = false;

    if ( lineRead == NULL || strlen ( lineRead ) == 0 )
    {
        return replaced;
    }

    if ( pSearched == NULL || strlen ( pSearched ) == 0 )
    {
        return replaced;
    }

    if ( pReplaced == NULL )
    {
        return replaced;
    }

    if ( _stricmp ( pSearched, pReplaced ) == 0 )
    {
        return replaced;
    }

    //
    while ( sizeType > 0 && *lineRead != '\0' )
    {
        int compare = _strnicmp ( lineRead, pSearched, strlen ( pSearched ) );
        if ( compare == 0 )
        {
            if ( sizeType >= strlen ( pReplaced ) )
            {
                size_t  lenSearched = strlen ( pSearched );
                size_t  lenReplaced = strlen ( pReplaced );
                size_t  lenLine     = strlen ( lineRead );

                //
                //      Move
                int deltaType   = ( int ) lenSearched - ( int ) lenReplaced;

                int sizeToMove = ( int ) lenLine - deltaType + 1;
                if ( ( lenSearched != lenReplaced ) && ( sizeToMove > 0 ) )
                {
                    memmove_s ( lineRead + lenReplaced, sizeType, lineRead + lenSearched, sizeToMove );
                }
                else
                {
                    //
                    //  Zero End Of Line
//                  wmemset ( lineRead, NULL, lenReplaced + 1 );
                }
                //
                //  Copy String
                memcpy_s ( lineRead, sizeType, pReplaced, lenReplaced );

                //
                //  Do Not Advance when global
                if ( ! bGlobal || ( lenReplaced >= lenSearched ) ) 
                {
                    lineRead    += lenReplaced;
                    sizeType    -= lenReplaced;
                }

                //
                //      Say it has been replaced
                replaced    = true;

                if ( ! bGlobal )
                {
                    return replaced;
                }
            }
            else
            {
                sizeType    = 0;
                return  replaced;
            }
        }
        else
        {
            lineRead++;
            sizeType--;
        }
    }

    return replaced;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTRXCHEATWINApp::FindFileName ( char *pText )
{
    for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
    {
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
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
char *CTRXCHEATWINApp::FindFileType ( char *pText )
{
    for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
    {
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
        {
            return pText + strlen(pText);
        }
        if ( pText [ i ] == ':' )
        {
            return pText + strlen(pText);
        }
        if ( pText [ i ] == '.' )
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
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
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
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
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
const char *CTRXCHEATWINApp::FindFileType ( const char *pText )
{
    for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
    {
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
        {
            return pText + strlen(pText);
        }
        if ( pText [ i ] == ':' )
        {
            return pText + strlen(pText);
        }
        if ( pText [ i ] == '.' )
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

        if ( pText [ i ] == '\\' || pText [ i ] == '/' || pText [ i ] == ':' )
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
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::ReadIndicators()
{
    static char szIndicatorsFilename [ MAX_PATH ];

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTRSaveGame::GetIndicatorFileType1() );
    CTRSaveGame::ReadIndicators ( IndicatorsTR123Table1, IndicatorsTR123Table1Count, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTRSaveGame::GetIndicatorFileType2() );
    CTRSaveGame::ReadIndicators ( IndicatorsTR123Table2, IndicatorsTR123Table2Count, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR4SaveGame::GetIndicatorFileType() );
    CTR4SaveGame::ReadIndicators ( IndicatorsTR4Table, IndicatorsTR4TableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR4NGSaveGame::GetIndicatorFileType() );
    CTR4NGSaveGame::ReadIndicators ( IndicatorsTR4NGTable, IndicatorsTR4NGTableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR5SaveGame::GetIndicatorFileType() );
    CTR5SaveGame::ReadIndicators ( IndicatorsTR5Table, IndicatorsTR5TableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR9SaveGame::GetIndicatorFileType() );
    CTR9SaveGame::ReadIndicators ( IndicatorsTRRTableBytes, IndicatorsTRRTableBytesCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR8SaveGame::GetIndicator4FileType() );
#ifdef _DEBUG
    CTR8SaveGame::ReadIndicators ( IndicatorsTRR4Table, IndicatorsTRR4TableCount, szIndicatorsFilename );
#endif

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR8SaveGame::GetIndicator5FileType() );
#ifdef _DEBUG
    CTR8SaveGame::ReadIndicators ( IndicatorsTRR5Table, IndicatorsTRR5TableCount, szIndicatorsFilename );
#endif

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::WriteIndicators()
{
    static char szIndicatorsFilename [ MAX_PATH ];

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTRSaveGame::GetIndicatorFileType1() );
    CTRSaveGame::WriteIndicators ( IndicatorsTR123Table1, IndicatorsTR123Table1Count, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTRSaveGame::GetIndicatorFileType2() );
    CTRSaveGame::WriteIndicators ( IndicatorsTR123Table2, IndicatorsTR123Table2Count, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR4SaveGame::GetIndicatorFileType() );
    CTR4SaveGame::WriteIndicators ( IndicatorsTR4Table, IndicatorsTR4TableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR4NGSaveGame::GetIndicatorFileType() );
    CTR4NGSaveGame::WriteIndicators ( IndicatorsTR4NGTable, IndicatorsTR4NGTableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR5SaveGame::GetIndicatorFileType() );
    CTR5SaveGame::WriteIndicators ( IndicatorsTR5Table, IndicatorsTR5TableCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR9SaveGame::GetIndicatorFileType() );
    CTR9SaveGame::WriteIndicators ( IndicatorsTRRTableBytes, IndicatorsTRRTableBytesCount, szIndicatorsFilename );

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR8SaveGame::GetIndicator4FileType() );
#ifdef _DEBUG
    CTR8SaveGame::WriteIndicators ( IndicatorsTRR4Table, IndicatorsTRR4TableCount, szIndicatorsFilename );
#endif

    //
    strcpy_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), InitFileName );
    RemoveFileType ( szIndicatorsFilename );
    strcat_s ( szIndicatorsFilename, sizeof(szIndicatorsFilename), CTR8SaveGame::GetIndicator5FileType() );
#ifdef _DEBUG
    CTR8SaveGame::WriteIndicators ( IndicatorsTRR5Table, IndicatorsTRR5TableCount, szIndicatorsFilename );
#endif

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXCHEATWINApp::SearchRemasteredDirectory( const char *pathname )
{
    static  char szfilename [ MAX_PATH ];
    ZeroMemory ( szfilename, sizeof(szfilename) );

    //
    struct _finddata_t fileinfo;
    intptr_t handle = _findfirst ( pathname, &fileinfo );
    if ( handle !=  -1 )
    {
        int iRes = 0;
        do
        {
            if ( *fileinfo.name != '.' )
            {
                strcat_s ( szfilename, sizeof(szfilename), fileinfo.name );
                break;
            }

            //
            iRes = _findnext ( handle, &fileinfo );
        }
        while ( iRes == 0 );

        _findclose(handle);
    }

    return szfilename;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHEATWINApp::SearchRemasteredDirectory()
{
    static char szDefaultTRX [ MAX_PATH ];
    static char szDefaultTRX2 [ MAX_PATH ];

    static char szProfile [ MAX_PATH ];
    size_t iProfile = sizeof(szProfile);

    static char szUsername [ MAX_PATH ];
    size_t iUsername = sizeof(szUsername);

    ZeroMemory ( szDefaultTRX, sizeof(szDefaultTRX) );
    ZeroMemory ( szDefaultTRX2, sizeof(szDefaultTRX2) );
    ZeroMemory ( szProfile, sizeof(szProfile) );
    ZeroMemory ( szUsername, sizeof(szUsername) );

    getenv_s ( &iProfile, szProfile, sizeof(szProfile), "USERPROFILE");
    getenv_s ( &iUsername, szUsername, sizeof(szUsername), "USERNAME");


    if ( iProfile > 0 )
    {
        strcat_s ( szDefaultTRX, sizeof(szDefaultTRX), szProfile );
    }
    else if ( iUsername > 0 )
    {
        strcat_s ( szDefaultTRX, sizeof(szDefaultTRX), "C:\\Users\\" );
        strcat_s ( szDefaultTRX, sizeof(szDefaultTRX), szUsername );
    }
    else
    {
        strcat_s ( szDefaultTRX, sizeof(szDefaultTRX), "C:`\\Users\\yourname" );
    }
    strcat_s ( szDefaultTRX2, sizeof(szDefaultTRX2), szDefaultTRX );

    //
    strcat_s ( szDefaultTRX, sizeof(szDefaultTRX), "\\AppData\\Roaming\\TRX\\*" );
    const char *pTrx = SearchRemasteredDirectory ( szDefaultTRX );
    strcpy_s ( CTRXGlobal::m_szTRXNumber, sizeof(CTRXGlobal::m_szTRXNumber), pTrx );

    strcat_s ( szDefaultTRX2, sizeof(szDefaultTRX2), "\\AppData\\Roaming\\TRX2\\*" );
    const char *pTrx2 = SearchRemasteredDirectory ( szDefaultTRX2 );
    strcpy_s ( CTRXGlobal::m_szTRX2Number, sizeof(CTRXGlobal::m_szTRX2Number), pTrx2 );
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
    static char szArguments [ MAX_PATH ];

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

    ZeroMemory ( TR39PuzzleName, sizeof(TR39PuzzleName) );
    ZeroMemory ( TR39PickupName, sizeof(TR39PickupName) );
    ZeroMemory ( TR39KeyName, sizeof(TR39KeyName) );

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

    //  Savegame of TRNG are generally aound 40 Kb.
    //  But there are some of 900Kb.
    CTRXGlobal::m_iMinNGSize    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MIN_NG_SIZE, 0, TR_NG_MIN_SIZE );
    CTRXGlobal::m_iMaxNGSize    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, 0, TR_NG_MAX_SIZE );
    if ( CTRXGlobal::m_iMaxNGSize > MAX_SAVELENGTH )
    {
        CTRXGlobal::m_iMaxNGSize    = MAX_SAVELENGTH;
        theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_MAX_NG_SIZE, CTRXGlobal::m_iMaxNGSize );
    }

    //  Search Position Extended helps when Lara is ont in standing Position
    //  Note that modifying position for savegame of TRNG > 900 KB will not work
    
    //
    CTRXColors::m_iDarkTheme            = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, -1, 0 );
    CTRXColors::m_iSquareCorner         = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, -1, 0 );
    CTRXColors::m_iSquareForce          = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SQUAREFORCE, -1, 1 );
    CTRXColors::m_iThemeRestart         = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_THEME_RESTART, -1, 1 );

    CTRXGlobal::m_iSearchPosExt         = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, -1, 1 );
    CTRXGlobal::m_iExtendVertical       = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_EXTEND_VERTICAL, -1, 0 );
    CTRXGlobal::m_iTRRBackup            = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TRR_BACKUP, -1, 32 );
    CTRXGlobal::m_iTRBackup             = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR_BACKUP, -1, 4 );
    CTRXGlobal::m_iExtSearchPos         = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_EXT_SEA_POS, -1, 4 );
    CTRXGlobal::m_UseTR4PositionRange   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_USE_TR4_RANGE, -1, TRUE );
    CTRXGlobal::m_UseTR5PositionRange   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_USE_TR5_RANGE, -1, TRUE );

    //
    CTRXGlobal::m_TR123IndexMaximum     = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR123_INDEX, -1, TR_SECURE_POSITION );

    CTRXGlobal::m_TR4IndexMaximum       = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR4_INDEX, -1, TR_SECURE_POSITION );
    CTRXGlobal::m_TR4NGIndexMaximum     = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR4NG_INDEX, -1, TR_SECURE_POSITION );

    CTRXGlobal::m_TR5IndexMaximum       = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TR5_INDEX, -1, TR_SECURE_POSITION );

    CTRXGlobal::m_TRRIndexMaximum       = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TRR_INDEX, -1, TR_SECURE_POSITION );

    CTRXGlobal::m_bTraceTRNGSavegame    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_TRACE_TRNG_SAVE, -1, FALSE );
    CTRXGlobal::m_bAlterTRNGPosition    = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_ALTER_TRNG_POS, -1, TRUE );
    CTRXGlobal::m_iAlterTRNGIndice      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_ALTER_TRNG_IND, -1, 84 );
    CTRXGlobal::m_bAlterTRNGAmmosGuns   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_ALTER_TRNG_AGUNS, -1, FALSE );

    //
    //  Get Default directories for Remastered
    SearchRemasteredDirectory();

    //
    CString trx = theApp.GetProfileString ( PROFILE_SETTING, PROFILE_TRX, "NA" );
    if ( trx == "NA" )
    {
        theApp.WriteProfileString ( PROFILE_SETTING, PROFILE_TRX, CTRXGlobal::m_szTRXNumber );
    }
    else
    {
        strcpy_s ( CTRXGlobal::m_szTRXNumber, sizeof(CTRXGlobal::m_szTRXNumber), trx );
    }

    CString trx2 = theApp.GetProfileString ( PROFILE_SETTING, PROFILE_TRX2, "NA" );
    if ( trx2 == "NA" )
    {
        theApp.WriteProfileString ( PROFILE_SETTING, PROFILE_TRX2, CTRXGlobal::m_szTRX2Number );
    }
    else
    {
        strcpy_s ( CTRXGlobal::m_szTRX2Number, sizeof(CTRXGlobal::m_szTRX2Number), trx2 );
    }

    //
    //  Module Filename has been read
    //  And Ini file name too
    //
    ReadIndicators();

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
            WriteIndicators();
        }
        //
        else if (   _stricmp ( pCommandLine, "-123" ) == 0              ||
                    _stricmp ( pCommandLine, "-r123" ) == 0             ||
                    _stricmp ( pCommandLine, "-remastered123" ) == 0       )
        {
            CTRXPropertySheetRemastered dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        else if (   __strnicmp ( pCommandLine, "-123 " ) == 0               ||
                    __strnicmp ( pCommandLine, "-r123 " ) == 0              ||
                    __strnicmp ( pCommandLine, "-remastered123 " ) == 0         )
        {
            CTRXPropertySheetRemastered dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );

            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                dlg.SetParmPathname ( szFullPathname );
            }
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        else if (   _stricmp ( pCommandLine, "-456" ) == 0              ||
                    _stricmp ( pCommandLine, "-r456" ) == 0             ||
                    _stricmp ( pCommandLine, "-remastered456" ) == 0       )
        {
            CTRXPropertySheetRemastered456 dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        else if (   __strnicmp ( pCommandLine, "-456 " ) == 0               ||
                    __strnicmp ( pCommandLine, "-r456 " ) == 0              ||
                    __strnicmp ( pCommandLine, "-remastered456 " ) == 0     )
        {
            CTRXPropertySheetRemastered456 dlg ( TRR_PROGRAM );
            dlg.SetApply ( TRUE );

            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

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
        else if (   __strnicmp ( pCommandLine, "-1 " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-2 " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-3 " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-4 " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-5 " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-s " ) == 0                   ||
                    __strnicmp ( pCommandLine, "-standard " ) == 0     )
        {
            //
            CTRXPropertySheetStandard   dlg ( TR_PROGRAM );
            dlg.SetApply ( TRUE );

            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                dlg.SetParmPathname ( szFullPathname );
            }

            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
#ifdef _DEBUG
        //  Analyze a TRR Remastered savegame
        else if ( __strnicmp ( pCommandLine, "-analyze" ) == 0 )
        {
            //
            SetStdOutToNewConsole();
            ZeroMemory ( szPathname, sizeof(szPathname) );

            //
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

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
        else if ( __strnicmp ( pCommandLine, "-force" ) == 0 )
        {
            ZeroMemory ( szPathname, sizeof(szPathname) );

            char *pGame = strchr ( pCommandLine, ' ' );
            if ( pGame != NULL )
            {
                static char MainWindowsTitle [ MAX_PATH ] = "";

                //
                pGame = SkipSpaces ( pGame );
                if ( _strnicmp ( pGame, "TR1", strlen("TR1") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR1;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 1 Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TUB", strlen("TUB") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TUB;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 1 UB Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR2G", strlen("TR2G") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR2G;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 2 Gold Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR2", strlen("TR2") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR2;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 2 Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR3G", strlen("TR3G") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR3G;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 3 Gold Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR3", strlen("TR3") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR3;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 3 Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR4NG", strlen("TR4NG") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR4NG;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 4 Next Gen Program" );
                }
                else if ( _strnicmp ( pGame, "TR4G", strlen("TR4G") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR4G;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 4 Gold Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR4", strlen("TR4") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR4;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 4 Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TR5", strlen("TR5") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TR5;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 5 Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TRR123", strlen("TRR123") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TRR123;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 123 Remastered Cheat Program" );
                }
                else if ( _strnicmp ( pGame, "TRR456", strlen("TRR456") ) == 0 )
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_TRR456;
                    strcpy_s ( MainWindowsTitle, sizeof(MainWindowsTitle), "Tombraider 456 Remastered Cheat Program" );
                }
                else
                {
                    CTRXGlobal::m_ForceSaveGame = FORCE_NONE;
                }

                //
                char *pFilename = strchr ( pGame, ' ' );
                if ( pFilename != NULL )
                {
                    pFilename = SkipSpaces ( pFilename );
                    CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                    LPSTR lpFilepart = NULL;
                    GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );
                }

                if ( ! PathFileExists(szFullPathname) )
                {
                    ZeroMemory ( szFullPathname, sizeof(szFullPathname) );
                }

                //
                switch ( CTRXGlobal::m_ForceSaveGame )
                {
                    //
                    case FORCE_TRR123 :
                    {
                        CTRXPropertySheetRemastered dlg ( MainWindowsTitle );
                        dlg.SetApply ( TRUE );
                        if ( strlen(szFullPathname) > 0 )
                        {
                            dlg.SetParmPathname ( szFullPathname );
                        }
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                        break;
                    }
                    //
                    case FORCE_TRR456 :
                    {
                        CTRXPropertySheetRemastered456 dlg ( MainWindowsTitle );
                        dlg.SetApply ( TRUE );
                        if ( strlen(szFullPathname) > 0 )
                        {
                            dlg.SetParmPathname ( szFullPathname );
                        }
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                        break;
                    }
                    //
                    case FORCE_NONE :
                    {
                        CTRXPropertySheetAll    dlg ( BOTH_PROGRAMS );
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                        break;
                    }
                    //
                    default :
                    {
                        CTRXPropertySheetStandard   dlg ( MainWindowsTitle );
                        dlg.SetApply ( TRUE );
                        if ( strlen(szFullPathname) > 0 )
                        {
                            dlg.SetParmPathname ( szFullPathname );
                        }
                        m_pMainWnd = &dlg;
                        nResponse = dlg.DoModal();
                        break;
                    }
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
        //  Read Script
        else if ( __strnicmp ( pCommandLine, "-rs2" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "data\\tombpc.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 2 );
        }
        //
        //  Read Script
        else if ( __strnicmp ( pCommandLine, "-rs3" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "data\\tombpc.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 3 );
        }
        //
        //  Read Script
        else if ( __strnicmp ( pCommandLine, "-rs4" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 4 );
        }
        //
        //  Read Savegame
        else if ( __strnicmp ( pCommandLine, "-unziptrx" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "saves\\save_tr1_00.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = UnZipTRXSavegame ( szFullPathname, szDirectory, 1 );
        }
        //
        //  Read .TR4 for NG values
        else if ( __strnicmp ( pCommandLine, "-tr4ng" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                //
                strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
                RemoveFilename ( szDirectory );
                BOOL bRead = ReadTR4ForNG ( szFullPathname );
            }
        }
        //
        //  Unblind Script
        else if ( __strnicmp ( pCommandLine, "-unblind" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }
            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = UnBlindTRXScript ( szFullPathname, szDirectory );
        }
        //
        //  Unsoft Script
        else if ( __strnicmp ( pCommandLine, "-unsoft" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = UnSoftTRXScript ( szFullPathname, szDirectory );
        }
        //
        //  Remove Some NG Script
        else if ( __strnicmp ( pCommandLine, "-remove" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                char *pArguments = CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                //
                strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
                RemoveFilename ( szDirectory );

                //
                pArguments = SkipSpaces ( pArguments );
                if ( pArguments != NULL )
                {
                    pArguments = SkipSpaces ( pArguments );

                    //
                    strcpy_s ( szArguments, sizeof(szArguments), pArguments );

                    //
                    BOOL bRead = RemoveTRXScript ( szFullPathname, szDirectory, szArguments );
                }
            }
        }
        //
        //  Alter PURE TRNG Script
        else if ( __strnicmp ( pCommandLine, "-anylevel" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                if ( PathFileExists ( "data\\tombpc.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "data\\tombpc.dat" );
                }
                else if ( PathFileExists ( "tombpc.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "tombpc.dat" );
                }
                else if ( PathFileExists ( "script.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
                }
                else
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
                }
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );

            //
            //
            BOOL bAltered = AlterTRXScript ( szFullPathname, szDirectory, true );
        }
        //
        //  Alter PURE TRNG Script
        else if ( __strnicmp ( pCommandLine, "-onelevel" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                if ( PathFileExists ( "data\\tombpc.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "data\\tombpc.dat" );
                }
                else if ( PathFileExists ( "tombpc.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "tombpc.dat" );
                }
                else if ( PathFileExists ( "script.dat" ) )
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
                }
                else
                {
                    strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
                }
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );

            //
            //
            BOOL bAltered = AlterTRXScript ( szFullPathname, szDirectory, false );
        }
        //
        //  Write Script
        else if ( __strnicmp ( pCommandLine, "-ws4" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

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
        else if ( __strnicmp ( pCommandLine, "-rs5" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );
            }

            if ( strlen (szPathname) == 0 )
            {
                strcpy_s ( szPathname, sizeof(szPathname), "script.dat" );
            }

            LPSTR lpFilepart = NULL;
            GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

            //
            strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
            RemoveFilename ( szDirectory );
            BOOL bRead = ReadTRXScript ( szFullPathname, szDirectory, 5 );
        }
        //
        //  Read Script
        else if ( __strnicmp ( pCommandLine, "-ws5" ) == 0 )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                //
                strcpy_s ( szDirectory, sizeof(szDirectory), szFullPathname );
                RemoveFilename ( szDirectory );
                BOOL bWritten = WriteTRXScript ( szFullPathname, szDirectory, 5 );
            }
        }
        //
        //  Show Map
        else if ( _stricmp ( pCommandLine, "-maps" ) == 0L )
        {
            CTRXAllMaps dlg;
            m_pMainWnd = &dlg;
            nResponse = dlg.DoModal();
        }
        //
        //  Custom
        else if (   __strnicmp ( pCommandLine, "-tr1" ) == 0L ||
                    __strnicmp ( pCommandLine, "-tr2" ) == 0L ||
                    __strnicmp ( pCommandLine, "-tr3" ) == 0L ||
                    __strnicmp ( pCommandLine, "-tr4" ) == 0L ||
                    __strnicmp ( pCommandLine, "-tr5" ) == 0L  )
        {
            //
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = strchr ( pCommandLine, ' ' );
            if ( pFilename != NULL )
            {
                pFilename = SkipSpaces ( pFilename );
                CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

                LPSTR lpFilepart = NULL;
                GetFullPathName ( szPathname, sizeof(szFullPathname), szFullPathname, &lpFilepart );

                //
                TR_MODE trMode;
                if ( __strnicmp ( pCommandLine, "-tr1" ) == 0 )
                {
                    trMode  = TRR1_MODE;
                }
                else if ( __strnicmp ( pCommandLine, "-tr2" ) == 0 )
                {
                    trMode  = TRR2_MODE;
                }
                else if ( __strnicmp ( pCommandLine, "-tr3" ) == 0 )
                {
                    trMode  = TRR3_MODE;
                }
                else if ( __strnicmp ( pCommandLine, "-tr4" ) == 0 )
                {
                    trMode  = TR4_MODE;
                }
                else if ( __strnicmp ( pCommandLine, "-tr5" ) == 0 )
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
        else if (   __strstri ( pCommandLine, "savegame." ) != NULL ||
                    __strstri ( pCommandLine, "saveub." ) != NULL ||
                    __strstri ( pCommandLine, "saveati." ) != NULL )
        {
            ZeroMemory ( szPathname, sizeof(szPathname) );
            char *pFilename = SkipSpaces ( pCommandLine );
            CopyBetweenQuotes ( szPathname, sizeof(szPathname), pFilename );

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
                    else if ( dwSize == TR456LEVELSIZE )
                    {
                        CTRXPropertySheetRemastered456 dlg ( TRR_PROGRAM );
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
BOOL CTRXCHEATWINApp::WriteProfileInt ( const char *section,  const char *keyName, int value, bool bHexa )
{
    char szText [ 64 ];
    if ( bHexa )
    {
        sprintf_s ( szText, sizeof(szText), "0x%x", value );
    }
    else
    {
        sprintf_s ( szText, sizeof(szText), "%d", value );
    }
    return WriteProfileIniFile ( section, keyName, szText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CTRXCHEATWINApp::WriteProfileLong ( const char *section,  const char *keyName, long value, bool bHexa )
{
    char szText [ 64 ];
    if ( bHexa )
    {
        sprintf_s ( szText, sizeof(szText), "0x%lx", value );
    }
    else
    {
        sprintf_s ( szText, sizeof(szText), "%ld", value );
    }
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
    if ( __strnicmp ( szText, "0x" ) == 0 )
    {
        int iResult    = 0;
        sscanf_s ( szText + 2, "%x", &iResult );
        result      = (UINT) iResult;
    }
    else if ( __strnicmp ( szText, "x" ) == 0 )
    {
        int iResult    = 0;
        sscanf_s ( szText + 1, "%x", &iResult );
        result      = (UINT) iResult;
    }
    else
    {
        result = (UINT) atoi ( szText );
    }
    return result;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
UINT CTRXCHEATWINApp::GetProfileInt ( const char *section,  const char *keyName, int novalue, int defaultValue )
{
    int value = (int) theApp.GetProfileInt ( section, keyName, novalue );
    if ( value == novalue )
    {
        theApp.WriteProfileInt ( section, keyName, defaultValue );
        value = defaultValue;
    }
    return (UINT) value;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
ULONG CTRXCHEATWINApp::GetProfileLong ( const char *section,  const char *keyName, int defaultValue )
{
    ULONG result = 0;
    char szText [ 64 ];
    char szDefault [ 64 ];
    sprintf_s ( szText, sizeof(szText), "%d", defaultValue );
    strcpy_s ( szDefault, szText );
    BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), szDefault );
    if ( ! bRead )
    {
        return defaultValue;
    }
    if ( __strnicmp ( szText, "0x" ) == 0 )
    {
        long lResult    = 0;
        sscanf_s ( szText + 2, "%lx", &lResult );
        result      = (UINT) lResult;
    }
    else if ( __strnicmp ( szText, "x" ) == 0 )
    {
        long lResult    = 0;
        sscanf_s ( szText + 1, "%lx", &lResult );
        result      = (UINT) lResult;
    }
    else
    {
        result = (ULONG) atol ( szText );
    }
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
CString CTRXCHEATWINApp::GetProfileString ( const char *section,  const char *keyName, const char *pDefaultValue,
                                            char *pTarget, size_t iTarget )
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

    if ( pTarget != NULL && iTarget > 0 )
    {
        strcpy_s ( pTarget, iTarget, result );
    }

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
    const char *pFileName = FindFileName ( pName );

    if ( __strstri ( pFileName, ".bak" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".log" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".zip" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".sav" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".txt" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".doc" ) != NULL )
    {
        return FALSE;
    }

    if ( __strstri ( pFileName, ".pdf" ) != NULL )
    {
        return FALSE;
    }

    const char *pType = FindFileType ( pName );

    return TRUE;
}