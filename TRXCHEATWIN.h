// TRXCHEATWIN.h : main header file for the TR3CHEATWIN application
//
#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"       // main symbols

#include <afx.h>
#include "afxdlgs.h"

#include "TR10SaveGame.h"
#include "TR15SaveGame.h"
#include "TR2SaveGame.h"
#include "TR3SaveGame.h"
#include "TR4SaveGame.h"
#include "TR4NGSaveGame.h"
#include "TR5SaveGame.h"
#include "ReadTR2\ReadTRXScript.h"

#include "AutomaticVersionHeader.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXCHEATWINApp:
// See TR3CHEATWIN.cpp for the implementation of this class
//
/////////////////////////////////////////////////////////////////////////////
class CTRXCHEATWINApp : public CWinApp
{
    public:
        CTRXCHEATWINApp();

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTRXCHEATWINApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

    // Implementation

        //{{AFX_MSG(CTRXCHEATWINApp)
            // NOTE - the ClassWizard will add and remove member functions here.
            //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    //
    protected :
        char                ModuleFileName [ MAX_PATH ];
        char                InitFileName [ MAX_PATH ];

	public :
		WORD		        OSVersion [ 4 ];
        //  Ex 0x00060001   for Windows 7
        DWORD               OSHVersion;
        //  Release Details
        DWORD               OSLVersion;

    //
    public :
        static char *FindFileName ( char *pText );
        static const char *FindFileName ( const char *pText );
        static char *FindFileType( char *pText );
        static const char *FindFileType( const char *pText );

        //  Remove filename including trailing \ or /
        static char *RemoveFilename ( char *pText );
        static char *SkipSpaces ( char *pText );
        static bool __strrepiA ( char *lineRead, size_t sizeType, const char *pSearched, const char *pReplaced, bool bGlobal );
        static void RemoveEnclosingQuotes ( char *pText, size_t iLength );
        static char *CopyBetweenQuotes ( char *pTarget, size_t iLength, char *pSource );

        const char *SearchRemasteredDirectory( const char *pathname );
        void SearchRemasteredDirectory();

        void ReadIndicators();
        void WriteIndicators();

        //  Strictly Lower
        bool OSVersionLowerThan ( WORD major, WORD minor );

        //  Greater Or Equal
        bool OSVersionGreaterThan ( WORD major, WORD minor );

        void GetModule ();
        bool SearchInitFile ( const char *pInitFileName, size_t iInitFileName );

        BOOL WriteProfileIniFile ( const char *section,  const char *keyName, const char *pValue );
        BOOL ReadProfileIniFile ( const char *section,  const char *keyName, char *pValue, size_t iValue, const char *pDefault );

        UINT GetProfileInt ( const char *section,  const char *keyName, int defaultValue );
        UINT GetProfileInt ( const char *section,  const char *keyName, int novalue, int defaultValue );
        ULONG GetProfileLong ( const char *section,  const char *keyName, int defaultValue );
        double GetProfileDouble ( const char *section,  const char *keyName, double defaultValue );
        CString GetProfileString ( const char *section,  const char *keyName, const char *pDefaultValue = "",
                                    char *pTarget = NULL, size_t iTarget = 0 );

        BOOL WriteProfileLong ( const char *section,  const char *keyName, long value, bool bHexa = false );
        BOOL WriteProfileInt ( const char *section,  const char *keyName, int value, bool bHexa = false );
        BOOL WriteProfileDouble ( const char *section,  const char *keyName, double value );
        BOOL WriteProfileString ( const char *section,  const char *keyName, const char *pValue );

        void ReStartProgram(CWnd *pWnd);

        static BOOL AcceptedPattern ( const char *pName );

        static char *RemoveFileType ( char *pText );
        static const char *__strstri ( const char *pString, const char *pSearched );
        static int __strnicmp ( const char *pString, const char *pBegining );
        static BOOL EndsWithI ( const char *pText, const char *pEnd );
        static void ResetCustomLabels ();

        afx_msg void OnHelp();

};

//
#define PROFILE_SETTING             "Settings"
#define PROFILE_MENU                "Menu"
#define PROFILE_REMASTERED          "Remastered"
#define PROFILE_STANDARD            "Standard"
#define PROFILE_LAST_OPENED         "Last Opened"
#define PROFILE_TRX_LAST_OPENED     "TRX Last Opened"
#define PROFILE_TRX_LOC_COUNT       "TRX Last Location Count"
#define PROFILE_TRX_LOCATION        "TRX Last Location %02d"

#define PROFILE_TRX2_LAST_OPENED    "TRX2 Last Opened"
#define PROFILE_TRX2_LOC_COUNT      "TRX2 Last Location Count"
#define PROFILE_TRX2_LOCATION       "TRX2 Last Location %02d"

#define PROFILE_UNCHECKED           "Unchecked"
#define PROFILE_SEARCH_EXT          "Search Extended"
#define PROFILE_SEARCH_POS_EXT      "Search Position Extended"
#define PROFILE_MIN_NG_SIZE         "Min NG Size"
#define PROFILE_MAX_NG_SIZE         "Max NG Size"
#define PROFILE_DARKTHEME           "DarkTheme"
#define PROFILE_THEME_RESTART       "Restart on Change Theme"
#define PROFILE_SQUARECORNERS       "Square Corners"
#define PROFILE_SQUAREFORCE         "Square Force"
#define PROFILE_RECURSE_WRITE       "TRX Recurse Write"
#define PROFILE_EXTEND_VERTICAL     "Extend Vertical"
#define PROFILE_TRR_BACKUP          "TRR Backup"
#define PROFILE_TR_BACKUP           "TR Backup"
#define PROFILE_EXT_SEA_POS         "Extra Search Position v1.0"

#define PROFILE_USE_TR4_RANGE       "Use TR4 Position Range"
#define PROFILE_USE_TR5_RANGE       "Use TR5 Position Range"

#define PROFILE_TR123_INDEX         "TR123 Index Maximum"
#define PROFILE_TR4_INDEX           "TR4 Index Maximum"
#define PROFILE_TR4NG_INDEX         "TR4 NG Index Maximum"
#define PROFILE_TR5_INDEX           "TR5 Index Maximum"
#define PROFILE_TRR_INDEX           "TRR Index Maximum"

#define PROFILE_TRACE_TRNG_SAVE     "Trace TRNG Savegame"
#define PROFILE_ALTER_TRNG_POS      "Alter TRNG Position"
#define PROFILE_ALTER_TRNG_IND      "Alter TRNG Indice"
#define PROFILE_ALTER_TRNG_AGUNS    "Alter TRNG Ammos and Guns"

#define PROFILE_TRX                 "TRX Number"
#define PROFILE_TRX2                "TRX2 Number"

/////////////////////////////////////////////////////////////////////////////
