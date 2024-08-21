// TR3CHEATWINDlg.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>

#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "AutomaticCompilerVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//
/////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CTRXDialogBase(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_WEB, m_Web);
    DDX_Control(pDX, IDC_VERSION, m_Version);
    DDX_Control(pDX, IDC_EMAIL, m_Email);
    DDX_Control(pDX, IDC_COPYRIGHT, m_Copyright);
    DDX_Control(pDX, IDOK, m_Ok);
    DDX_Control(pDX, IDC_MACALLAN, m_Web_Site);
    DDX_Control(pDX, IDC_OSVER, m_OSVersion);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CTRXDialogBase)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_BN_CLICKED(IDC_MACALLAN, OnMacallan)
    ON_BN_CLICKED(IDC_WEB, OnWeb)
    ON_BN_CLICKED(IDC_EMAIL, OnEmail)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTR3CHEATWINDlg dialog
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
// CTR3CHEATWINDlg message handlers
//
/////////////////////////////////////////////////////////////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnMacallan()
{
    //
    HINSTANCE hInst;
    hInst = ShellExecute ( NULL, "open", "http://www.macallan.org", "", "", SW_SHOWDEFAULT );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::SetVersionFrom()
{
    DWORD           dwLength;
    DWORD           dwHandle;
    static char     szFilename [ MAX_PATH ];
    static char     szLabel [ 128 ];
    static char     szText [ 512 ];
    char            *pData;
    BOOL            bResult;
    DWORD           dwResult;
    void            *pBufferInfo;
    UINT            iBufferLen;

    //      Get module name
    dwResult = GetModuleFileName(  NULL, szFilename, sizeof ( szFilename ) );

    //      Get Size to allocate for info
    dwLength = GetFileVersionInfoSize ( szFilename, &dwHandle );
    if ( dwLength > 0 )
    {
        pData = new char [ dwLength ];

        bResult = GetFileVersionInfo(szFilename, dwHandle, dwLength, pData );
        if ( bResult )
        {
            //  Could be used to retrieve version
            bResult = VerQueryValue ( pData, "\\", &pBufferInfo, &iBufferLen );
            if (bResult)
            {
                if (iBufferLen)
                {             
                    VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)pBufferInfo;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        // Doesn't matter if you are on 32 bit or 64 bit,
                        // DWORD is always 32 bits, so first two revision numbers
                        // come from dwFileVersionMS, last two come from dwFileVersionLS
                        WORD w1 = ( verInfo->dwFileVersionMS >> 16 ) & 0xffff;
                        WORD w2 = ( verInfo->dwFileVersionMS >>  0 ) & 0xffff;
                        WORD w3 = ( verInfo->dwFileVersionLS >> 16 ) & 0xffff;
                        WORD w4 = ( verInfo->dwFileVersionLS >>  0 ) & 0xffff;
                    }   
                }
            }

            //
            struct LANGANDCODEPAGE
            {
                WORD wLanguage;
                WORD wCodePage;
            } *lpTranslate;

            struct LANGANDCODEPAGE Translate;

            // Read the list of languages and code pages.
            UINT cbTranslate;

            bResult = VerQueryValue(pData, "\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &cbTranslate);
            if ( bResult )
            {
                Translate   = *lpTranslate;
            }
            else
            {
                Translate.wLanguage = 0x0409;
                Translate.wCodePage = 0x04b0;
            }

            //
            sprintf_s ( szLabel, sizeof(szLabel),  "\\StringFileInfo\\%04x%04x\\ProductName", Translate.wLanguage, Translate.wCodePage );
            bResult = VerQueryValue ( pData, szLabel, &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                sprintf_s ( szText + strlen(szText), sizeof ( szText ) - strlen(szText), "\r\n%s\r\n(%d)\r\n", VS_VERSION_A, _MSC_FULL_VER );
                m_Version.SetWindowText ( szText );
            }

            sprintf_s ( szLabel, sizeof(szLabel),  "\\StringFileInfo\\%04x%04x\\LegalCopyright", Translate.wLanguage, Translate.wCodePage );
            bResult = VerQueryValue ( pData, szLabel, &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Copyright.SetWindowText ( szText );
            }

            sprintf_s ( szLabel, sizeof(szLabel),  "\\StringFileInfo\\%04x%04x\\InternalName", Translate.wLanguage, Translate.wCodePage );
            bResult = VerQueryValue ( pData, szLabel, &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Web.SetWindowText ( szText );
            }

            sprintf_s ( szLabel, sizeof(szLabel),  "\\StringFileInfo\\%04x%04x\\Comments", Translate.wLanguage, Translate.wCodePage );
            bResult = VerQueryValue ( pData, szLabel, &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Email.SetWindowText ( szText );
            }

        }

        delete pData;
    }

    memset ( szText, 0, sizeof ( szText ) );
    sprintf_s ( szText, sizeof ( szText ), "OS : %u.%u %u.%u", 
        theApp.OSVersion [ 0 ], theApp.OSVersion [ 1 ], theApp.OSVersion [ 2 ], theApp.OSVersion [ 3 ] );
    m_OSVersion.SetWindowText ( szText );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();
    
    //
    SetVersionFrom ( );

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnWeb()
{
    //
    HINSTANCE   hInst;
    CString     strText;

    m_Web.GetWindowText ( strText );
    hInst = ShellExecute ( NULL, "open", strText, "", "", SW_SHOWDEFAULT );
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnEmail()
{
    //
    HINSTANCE   hInst;
    CString     strText;

    m_Email.GetWindowText ( strText );
    hInst = ShellExecute ( NULL, "open", strText, "", "", SW_SHOWDEFAULT );
    
}
