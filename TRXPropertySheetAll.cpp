// TR3PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TRXGlobal.h"

#include "TRXEquipmentPage.h"   // Added by ClassView
#include "TRXGunPage.h"         // Added by ClassView
#include "TRXInfoPage.h"        // Added by ClassView
#include "TRXItems.h"           // Added by ClassView
#include "TRXItemsTR4.h"        // Added by ClassView
#include "TRXAmmosPage.h"       // Added by ClassView
#include "TRXRemastered.h"      // Added by ClassView

#include "TRXPropertySheetAll.h"
#include "TRXContainer.h"
#include "resource.h"
#include "AutomaticVersionHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetAll
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertySheetAll, CTRXPropertySheet)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetAll::CTRXPropertySheetAll(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
    CTRXPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    int iRemastered = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
    int iStandard   = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

    m_iStandardAdded    = FALSE;
    m_iRemasteredAdded  = FALSE;

    if ( iRemastered || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddRemasteredPage ();
    }

    if ( iStandard || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddStandardPage ();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetAll::CTRXPropertySheetAll(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage) :
        CTRXPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    int iRemastered = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
    int iStandard   = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

    m_iStandardAdded    = FALSE;
    m_iRemasteredAdded  = FALSE;

    if ( iRemastered || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddRemasteredPage ();
    }

    if ( iStandard || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddStandardPage ();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetAll::~CTRXPropertySheetAll()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertySheetAll, CTRXPropertySheet) //{{AFX_MSG_MAP(CTRXPropertySheetAll)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_GETDLGCODE()
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetAll message handlers
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetAll::OnInitDialog()
{
    //  Parent Is NULL
    // m_nFlags |= WF_CONTINUEMODAL;

    BOOL bResult = CTRXPropertySheet::OnInitDialog();

    //
    if ( GetParent() == NULL )
    {
        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);

            {
                int iRemastered     = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );

                CString strRemastered;
                strRemastered.LoadString(IDS_REMASTERED);
                if (!strRemastered.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_REMASTERED, strRemastered);
                    if ( iRemastered ) pSysMenu->CheckMenuItem(IDM_REMASTERED,MF_CHECKED|MF_BYCOMMAND);
                }
            }

            {
                int iStandard       = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

                CString strStandard;
                strStandard.LoadString(IDS_STANDARD);
                if (!strStandard.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_STANDARD, strStandard);
                    if ( iStandard ) pSysMenu->CheckMenuItem(IDM_STANDARD,MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iUnChecked      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

                CString strUnchecked;
                strUnchecked.LoadString(IDS_UNCHECKED);
                if (!strUnchecked.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_UNCHECKED, strUnchecked);
                    if ( iUnChecked ) pSysMenu->CheckMenuItem(IDM_UNCHECKED,MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iSearchExt      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );

                CString strSearchExt;
                strSearchExt.LoadString(IDS_SEARCH_EXT);
                if (!strSearchExt.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_SEARCH_EXT, strSearchExt);
                    if ( iSearchExt ) pSysMenu->CheckMenuItem(IDM_SEARCH_EXT,MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iSearchPosExt   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

                CString strSearchPosExt;
                strSearchPosExt.LoadString(IDS_SEARCH_POS_EXT);
                if (!strSearchPosExt.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_SEARCH_POS_EXT, strSearchPosExt);
                    if ( iSearchPosExt ) pSysMenu->CheckMenuItem(IDM_SEARCH_POS_EXT,MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iDarkTheme      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );

                CString strDarkTheme;
                strDarkTheme.LoadString(IDS_DARK_THEME);
                if (!strDarkTheme.IsEmpty())
                {
                    pSysMenu->AppendMenu(MF_STRING, IDM_DARK_THEME, strDarkTheme);
                    if ( iDarkTheme ) pSysMenu->CheckMenuItem(IDM_DARK_THEME,MF_CHECKED|MF_BYCOMMAND);
                }
            }
        }
    }

    m_bInitDone = true;

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//
/////////////////////////////////////////////////////////////////////////////
HCURSOR CTRXPropertySheetAll::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetAll::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ( ( nID & 0xFFF0 ) == IDM_REMASTERED)
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
                    RemoveRemasteredPage();
                }
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
                pSysMenu->CheckMenuItem(IDM_REMASTERED,MF_CHECKED|MF_BYCOMMAND);
                AddRemasteredPage();
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
                    RemoveStandardPage();
                }
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );
                pSysMenu->CheckMenuItem(IDM_STANDARD,MF_CHECKED|MF_BYCOMMAND);
                AddStandardPage();
            }
        }
    }
    else if ((nID & 0xFFF0) == IDM_UNCHECKED)
    {
        // int iUnchecked = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            if ( CTRXGlobal::m_iUnchecked )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );
                CTRXGlobal::m_iUnchecked    = FALSE;
                pSysMenu->CheckMenuItem(IDM_UNCHECKED,MF_UNCHECKED|MF_BYCOMMAND);
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 1 );
                CTRXGlobal::m_iUnchecked    = TRUE;
                pSysMenu->CheckMenuItem(IDM_UNCHECKED,MF_CHECKED|MF_BYCOMMAND);
            }
        }
    }
    else if ((nID & 0xFFF0) == IDM_SEARCH_EXT)
    {
        // int iSearchExt = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            if ( CTRXGlobal::m_iSearchExt )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );
                CTRXGlobal::m_iSearchExt    = FALSE;
                pSysMenu->CheckMenuItem(IDM_SEARCH_EXT,MF_UNCHECKED|MF_BYCOMMAND);
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 1 );
                CTRXGlobal::m_iSearchExt    = TRUE;
                pSysMenu->CheckMenuItem(IDM_SEARCH_EXT,MF_CHECKED|MF_BYCOMMAND);
            }
        }
    }
    else if ((nID & 0xFFF0) == IDM_SEARCH_POS_EXT)
    {
        // int iSearchExt = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            if ( CTRXGlobal::m_iSearchPosExt )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );
                CTRXGlobal::m_iSearchPosExt = FALSE;
                pSysMenu->CheckMenuItem(IDM_SEARCH_POS_EXT,MF_UNCHECKED|MF_BYCOMMAND);
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 1 );
                CTRXGlobal::m_iSearchPosExt = TRUE;
                pSysMenu->CheckMenuItem(IDM_SEARCH_POS_EXT,MF_CHECKED|MF_BYCOMMAND);
            }
        }
    }
    else if ((nID & 0xFFF0) == IDM_DARK_THEME)
    {
        int iDarkTheme = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if ( pSysMenu )
        {
            if ( CTRXGlobal::m_iDarkTheme )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );
                CTRXGlobal::m_iDarkTheme    = 0;
                pSysMenu->CheckMenuItem(IDM_DARK_THEME,MF_UNCHECKED|MF_BYCOMMAND);
                CTabCtrl* pTab = GetTabControl();
                if ( pTab )
                {
                    pTab->ModifyStyle ( TCS_OWNERDRAWFIXED, NULL );
                }

                //  Reset Theme
                SetWindowTheme( GetSafeHwnd(), NULL, NULL );
            }
            else
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 1 );
                CTRXGlobal::m_iDarkTheme    = 1;
                pSysMenu->CheckMenuItem(IDM_DARK_THEME,MF_CHECKED|MF_BYCOMMAND);

                CTabCtrl* pTab = GetTabControl();
                if ( pTab )
                {
                    pTab->ModifyStyle ( NULL, TCS_OWNERDRAWFIXED );
                }
            }

            if ( m_Info_Page != NULL ) m_Info_Page->SetThemeChanged ( CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_Equipment_Page != NULL ) m_Equipment_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_Gun_Page != NULL ) m_Gun_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_Ammos_Page != NULL ) m_Ammos_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_Item_Page != NULL ) m_Item_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_ItemTR4_Page != NULL ) m_ItemTR4_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );
            if ( m_Remastered_Page != NULL ) m_Remastered_Page->SetThemeChanged (  CTRXGlobal::m_iDarkTheme != 0 );

            //  Redraw Window
            Invalidate();
        }
    }
    else
    {
        CTRXPropertySheet::OnSysCommand(nID, lParam);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetAll::OnSize(UINT nType, int cx, int cy)
{
    CTRXPropertySheet::OnSize(nType, cx, cy);

    // TODO: Add Your handler here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetAll::OnMove(int x, int y)
{
    CTRXPropertySheet::OnMove(x, y);

    // TODO: Add Your handler here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXPropertySheetAll::DoModal()
{
    // TODO: Add Your handler here
    return CTRXPropertySheet::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetAll::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add Your handler here
    return CTRXPropertySheet::PreCreateWindow(cs);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXPropertySheetAll::OnGetDlgCode()
{
    // TODO: Add Code Here
    UINT iResult = CTRXPropertySheet::OnGetDlgCode();
    return iResult;
}
