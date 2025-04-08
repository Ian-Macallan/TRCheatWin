// TRXHelpDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"
#include "TRXHelpDialog.h"

#include "TRXGlobal.h"
#include "TRXColors.h"
#include "GunGrids.h"

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
    m_Tombraider    = -1;
    m_LevelNumber   = -1;
    ZeroMemory ( m_szTitle, sizeof(m_szTitle) );
    ZeroMemory ( m_szLevelName, sizeof(m_szLevelName) );
    ZeroMemory ( m_szWalkThru, sizeof(m_szWalkThru) );
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
    DDX_Control(pDX, ID_WALKTHRU, m_Walk);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXHelpDialog, CTRXDialogBase)
    ON_EN_VSCROLL(IDC_HELP_TEXT, &CTRXHelpDialog::OnVscrollHelpText)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(ID_WALKTHRU, &CTRXHelpDialog::OnBnClickedWalkthru)
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
    static char szTitle [ MAX_PATH ];
    ZeroMemory  ( szTitle, sizeof(szTitle) );

    //
    strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "https://www.google.com/search?q=" );

    switch ( m_Tombraider )
    {
        //
        case GAME_TRR1 :
        case GAME_TR10 :
        case GAME_TR15 :
        {
            if ( m_LevelNumber >= TR1G_START || m_Tombraider == 15 )
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider Unfinished Business" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider Unfinished Business" );
            }
            else
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider 1" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider 1" );
            }
            break;
        }
        case GAME_TR19 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 1 TRLE" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "trle.net" );
            break;
        }
        //
        case GAME_TRR2 :
        case GAME_TR20 :
        case GAME_TR25 :
        {
            if ( m_LevelNumber >= TR2G_START || m_Tombraider == 25 )
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider Golden Mask" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider Golden Mask" );
            }
            else
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider 2" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider 2" );
            }
            break;
        }
        case GAME_TR29 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 2 TRLE" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "trle.net" );
            break;
        }
        //
        case GAME_TRR3 :
        case GAME_TR30 :
        case GAME_TR35 :
        {
            if ( m_LevelNumber >= TR3G_START || m_Tombraider == 35 )
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider Lost Artifact" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider Lost Artifact" );
            }
            else
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider 3" );
                strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider 3" );
            }
            break;
        }
        case GAME_TR39 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 3 TRLE" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "trle.net" );
            break;
        }
        //
        case GAME_TRR4 :
        case GAME_TR40 :
        case GAME_TR45 :
        {
            if ( m_LevelNumber >= TR4G_START || m_Tombraider == 45 )
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider Times Exclusive" );
                strcat_s ( m_szWalkThru, sizeof(szTitle), "Tombraider Times Exclusive" );
            }
            else
            {
                strcat_s ( szTitle, sizeof(szTitle), "Tombraider 4" );
                strcat_s ( m_szWalkThru, sizeof(szTitle), "Tombraider Last Revelation" );
            }
            break;
        }
        //
        case GAME_TR49 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 4 TRLE" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "trle.net" );
            break;
        }
        //
        case GAME_TRR5 :
        case GAME_TR50 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 5" );
            strcat_s ( m_szWalkThru, sizeof(szTitle), "Tombraider Chronicles" );
            break;
        }
        case GAME_TR59 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 5 TRLE" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "trle.net" );
            break;
        }
        //
        case GAME_TRR6 :
        {
            strcat_s ( szTitle, sizeof(szTitle), "Tombraider 6" );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "Tombraider Angel Of Darkness" );
            break;
        }
    }

    //
    if ( strlen(m_szTitle) > 0 )
    {
        strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "+" );

        strcat_s ( szTitle, sizeof(szTitle), " - " );
        if ( m_Tombraider != GAME_TRR6 )
        {
            strcat_s ( szTitle, sizeof(szTitle), m_szTitle );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), m_szTitle );
        }
        else
        {
            char *pDash = strstr ( m_szTitle, " - " );
            if ( pDash != NULL )
            {
                *pDash  = '\0';
            }
            strcat_s ( szTitle, sizeof(szTitle), m_szTitle + 3 );
            strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), m_szTitle + 3 );
        }

        strcat_s ( m_szWalkThru, sizeof(m_szWalkThru), "+WalkThrough" );

    }

    //
    for ( size_t i = 0; i < strlen(m_szWalkThru); i++ )
    {
        if ( m_szWalkThru [ i ] == ' ' || m_szWalkThru [ i ] == '\'' )
        {
            m_szWalkThru [ i ] = '+';
        }
    }

    //
    if ( strlen(szTitle) > 0 )
    {
        SetWindowText ( szTitle );
    }

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

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXHelpDialog::OnBnClickedWalkthru()
{
    HINSTANCE hInst;
    hInst = ShellExecute ( NULL, "open", m_szWalkThru, "", "", SW_SHOWDEFAULT );
}
