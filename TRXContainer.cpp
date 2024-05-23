// TRXContainer.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TR9SaveGame.h"

#include "TRXContainer.h"
// #include "afxdialogex.h"


// Boîte de dialogue CTRXContainer
extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXContainer, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXContainer::CTRXContainer(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXContainer::IDD, pParent)
{
    m_pPropertySheet    = NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXContainer::~CTRXContainer()
{
    if ( m_pPropertySheet != NULL )
    {
        delete m_pPropertySheet;
        m_pPropertySheet = NULL;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXContainer, CTRXDialogBase)
    ON_WM_SYSCOMMAND()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDOK, &CTRXContainer::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CTRXContainer::OnBnClickedCancel)
    ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Message Handler CTRXContainer
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXContainer::DoModal()
{
    // TODO: Add Your handler here
    return CTRXDialogBase::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXContainer::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    DWORD dwStyle   = WS_MINIMIZEBOX | WS_SYSMENU;
    ModifyStyle( 0, dwStyle );

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        pSysMenu->InsertMenu(1,MF_BYPOSITION,SC_MINIMIZE, _T("Mi&nimize"));
        pSysMenu->InsertMenu(0,MF_BYPOSITION,SC_RESTORE, _T("&Restore"));
    }

    // TODO: Add your specialized code here
    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    int iRemastered = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
    int iStandard   = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

    //
    if (pSysMenu != NULL)
    {
        pSysMenu->AppendMenu(MF_SEPARATOR);
        //
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }

        pSysMenu->AppendMenu(MF_SEPARATOR);

        CString strRemastered;
        strRemastered.LoadString(IDS_REMASTERED);
        if (!strRemastered.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_STRING, IDM_REMASTERED, strRemastered);
            if ( iRemastered ) pSysMenu->CheckMenuItem(IDM_REMASTERED,MF_CHECKED|MF_BYCOMMAND);
        }

        CString strStandard;
        strStandard.LoadString(IDS_STANDARD);
        if (!strRemastered.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_STRING, IDM_STANDARD, strStandard);
            if ( iStandard ) pSysMenu->CheckMenuItem(IDM_STANDARD,MF_CHECKED|MF_BYCOMMAND);
        }

    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO:  Add Your handler here
    m_pPropertySheet = new CTRXPropertySheetAll("Tombraider Cheat Program", this );
    m_pPropertySheet->Create ( this, WS_CHILD | WS_TABSTOP | DS_3DLOOK | DS_CONTROL );
    m_pPropertySheet->ShowWindow ( SW_NORMAL );
    m_pPropertySheet->SetFocus ( );

    SetWindowText ( "Tombraider Remastered And Standard Cheat Program" );

    //
    RECT rectOK;
    ZeroMemory ( &rectOK, sizeof(rectOK) );
#if 0
    m_Ok.GetWindowRect ( &rectOK );
#endif
    int iOkHeight       = rectOK.bottom - rectOK.top;
    int iOkWidth        = rectOK.right - rectOK.left;
    int iSpaceButton    = 7;

    RECT rectCancel;
    ZeroMemory ( &rectCancel, sizeof(rectCancel) );
#if 0
    m_Cancel.GetWindowRect ( &rectCancel );
#endif
    int iCancelHeight   = rectCancel.bottom - rectCancel.top;
    int iCancelWidth    = rectCancel.right - rectCancel.left;

    //
    RECT rectSheet;
    m_pPropertySheet->GetClientRect ( &rectSheet );
    int iMargin         = 7;
    int iMargins        = 2 * iMargin;
    int iSheetSpace     = 22;
    int iSheetHeight    = rectSheet.bottom - rectSheet.top;
    int iSheetWidth     = rectSheet.right - rectSheet.left;
    int iWidth          = iSheetWidth + iMargins;
    int iHeight         = iSheetHeight + iMargins + iSheetSpace + iOkHeight + iSpaceButton;
    MoveWindow ( 0, 0, iWidth, iHeight, 1 );
    
#if 0
    rectCancel.top      = iSheetHeight;
    rectCancel.bottom   = rectCancel.top + iCancelHeight;
    rectCancel.left     = iSheetWidth - iCancelWidth - iMargin;
    rectCancel.right    = rectCancel.left + iCancelWidth;
    m_Cancel.MoveWindow ( &rectCancel, 1 );

    rectOK.top          = iSheetHeight;
    rectOK.bottom       = rectOK.top + iOkHeight;
    rectOK.left         = rectCancel.left - iOkWidth - iMargin;
    rectOK.right        = rectOK.left + iOkWidth;
    m_Ok.MoveWindow ( &rectOK, 1 );
#endif

    m_bInitDone = true;

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : OCX Page should return FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::OnSize(UINT nType, int cx, int cy)
{
    CTRXDialogBase::OnSize(nType, cx, cy);

    // TODO: Add Code Here
    if ( m_pPropertySheet != NULL )
    {
        m_pPropertySheet->OnSize ( nType, cx, cy );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::OnMove(int x, int y)
{
    CTRXDialogBase::OnMove(x, y);

    // TODO: Add Code Here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if ((nID & 0xFFF0) == IDM_REMASTERED)
    {
        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            UINT remastered = pSysMenu->GetMenuState (IDM_REMASTERED,MF_BYCOMMAND );
            UINT standard   = pSysMenu->GetMenuState (IDM_STANDARD,MF_BYCOMMAND );
            if ( remastered == MF_CHECKED  )
            {
                if ( standard == MF_CHECKED )   // Only Remove Remastered if standard is enabled
                {
                    theApp.WriteProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 0 );
                    pSysMenu->CheckMenuItem(IDM_REMASTERED,MF_UNCHECKED|MF_BYCOMMAND);
                    m_pPropertySheet->RemoveRemasteredPage();
                }
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
                pSysMenu->CheckMenuItem(IDM_REMASTERED,MF_CHECKED|MF_BYCOMMAND);
                m_pPropertySheet->AddRemasteredPage();
            }
        }
    }
    else if ((nID & 0xFFF0) == IDM_STANDARD)
    {
        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            UINT remastered = pSysMenu->GetMenuState (IDM_REMASTERED,MF_BYCOMMAND );
            UINT standard   = pSysMenu->GetMenuState (IDM_STANDARD,MF_BYCOMMAND );
            if ( standard == MF_CHECKED )
            {
                if ( remastered == MF_CHECKED ) // Only Remove Standard if Remastered is enabled
                {
                    theApp.WriteProfileInt( PROFILE_MENU, PROFILE_STANDARD, 0 );
                    pSysMenu->CheckMenuItem(IDM_STANDARD,MF_UNCHECKED|MF_BYCOMMAND);
                    m_pPropertySheet->RemoveStandardPage();
                }
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );
                pSysMenu->CheckMenuItem(IDM_STANDARD,MF_CHECKED|MF_BYCOMMAND);
                m_pPropertySheet->AddStandardPage();
            }
        }
    }
    else
    {
        CTRXDialogBase::OnSysCommand(nID, lParam);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXContainer::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add Your handler here
    return CTRXDialogBase::PreCreateWindow(cs);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::OnBnClickedOk()
{
    // TODO: Add Your handler here
    if ( CTRSaveGame::IsValid( ) &&
            CTRSaveGame::IsBufferModified() )
    {
        //
    }

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXContainer::OnBnClickedCancel()
{
    // TODO: Add Your handler here
    if ( CTRSaveGame::IsValid( ) &&
            CTRSaveGame::IsBufferModified() )
    {
        //
    }

    CTRXDialogBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXContainer::OnGetDlgCode()
{
    // TODO: Add Code Here

    return CTRXDialogBase::OnGetDlgCode();
}
