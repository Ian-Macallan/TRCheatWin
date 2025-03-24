// TRXHelpDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"
#include "TRXHelpDialog.h"

#include "TRXGlobal.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXHelpDialog
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXHelpDialog, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXHelpDialog::CTRXHelpDialog(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXHelpDialog::IDD, pParent)
{
    //
    m_ID_Resource   = 0xffffffff;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXHelpDialog::~CTRXHelpDialog()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXHelpDialog::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_HELP_TEXT, m_Help_Text);
    DDX_Control(pDX, IDC_COPYRIGHT, m_Copyright);
    DDX_Control(pDX, IDC_WEB, m_Web);
    DDX_Control(pDX, IDC_VERSION, m_Version);
    DDX_Control(pDX, IDC_EMAIL, m_Email);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXHelpDialog, CTRXDialogBase)
    ON_EN_VSCROLL(IDC_HELP_TEXT, &CTRXHelpDialog::OnVscrollHelpText)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXHelpDialog
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXHelpDialog::SetVersionFrom()
{
    DWORD           dwLength;
    DWORD           dwHandle;
    char            szFilename [ 512 ];
    char            szText [ 512 ];
    char            *pData;
    BOOL            bResult;
    DWORD           dwResult;
    void            *pBufferInfo;
    UINT            iBufferLen;

    //      Get module name
    dwResult =
        GetModuleFileName(  NULL, szFilename, sizeof ( szFilename ) );

    //      Get Size to allocate for info
    dwLength = GetFileVersionInfoSize ( szFilename, &dwHandle );
    if ( dwLength > 0 )
    {
        pData = new char [ dwLength ];

        bResult =
            GetFileVersionInfo(szFilename, dwHandle, dwLength, pData );
        if ( bResult )
        {
            bResult =
                VerQueryValue ( pData, "\\", &pBufferInfo, &iBufferLen );

            bResult =
                VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\ProductName", &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Version.SetWindowText ( szText );
            }

            bResult =
                VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\LegalCopyright", &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Copyright.SetWindowText ( szText );
            }

            bResult =
                VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\InternalName", &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Web.SetWindowText ( szText );
            }

            bResult =
                VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\Comments", &pBufferInfo, &iBufferLen );
            if ( bResult )
            {
                memset ( szText, 0, sizeof ( szText ) );
                memcpy ( szText, pBufferInfo, iBufferLen );
                m_Email.SetWindowText ( szText );
            }

        }

        delete pData;

    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXHelpDialog::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    //

    SetVersionFrom ();

    //
    if ( CTRXColors::m_iDarkTheme == 0 )
    {
        m_Help_Text.ModifyStyle ( ES_AUTOVSCROLL, WS_VSCROLL );

    }
    else
    {
        m_Help_Text.ModifyStyle ( ES_AUTOVSCROLL, WS_VSCROLL );
        // m_Help_Text.ModifyStyle ( WS_VSCROLL, ES_AUTOVSCROLL );
    }

    m_Help_Text.SetFont ( m_pFixedFont );

    //      Load resource and so on
    HRSRC hRes = NULL;
    if ( m_ID_Resource != 0xffffffff )
    {
        hRes = FindResource ( NULL, ( char * ) m_ID_Resource, "HELPTEXT" );
    }

    //
    if ( hRes == NULL )
    {
        hRes = FindResource ( NULL, ( char * ) IDR_HELP_TEXT, "HELPTEXT" );
    }

    //
    if ( hRes != NULL )
    {
        char        *pBuffer;

        DWORD dwSize = SizeofResource ( NULL, hRes );
        if ( dwSize > 0 )
        {
            pBuffer = ( char * ) malloc ( dwSize + 1 );
            ZeroMemory ( pBuffer, dwSize + 1 );
            HGLOBAL hResData = LoadResource ( NULL, hRes );
            if ( hResData != NULL )
            {
                char        *pResData = ( char * ) LockResource ( hResData );

                memcpy ( pBuffer, pResData, dwSize );
            }

            m_Help_Text.SetWindowTextA ( pBuffer );

            //      Then free the buffer
            free ( pBuffer );
        }
    }

    m_bInitDone = true;

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXHelpDialog::OnVscrollHelpText()
{
}

