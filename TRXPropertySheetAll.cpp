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
#include "resource.h"
#include "AutomaticVersionHeader.h"
#include "TRXColors.h"

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

    m_iStandardAdded        = FALSE;
    m_iRemastered123Added   = FALSE;
    m_iRemastered456Added   = FALSE;

    if ( iRemastered || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddRemastered123Page ();
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

    m_iStandardAdded        = FALSE;
    m_iRemastered123Added   = FALSE;
    m_iRemastered456Added   = FALSE;

    if ( iRemastered || ( iRemastered == 0 && iStandard == 0 ) )
    {
        AddRemastered123Page ();
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
        CMenu* pSysMenu = m_NC.GetSystemMenu(this, FALSE);
        if (pSysMenu != NULL)
        {
            m_NC.AppendSystemMenu ( this, FALSE, MF_SEPARATOR);

            {
                int iRemastered     = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );

                CString strRemastered;
                strRemastered.LoadString(IDS_REMASTERED);
                if (!strRemastered.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_REMASTERED, strRemastered);
                    if ( iRemastered ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_REMASTERED, MF_CHECKED|MF_BYCOMMAND );
                }
            }

            {
                int iStandard       = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

                CString strStandard;
                strStandard.LoadString(IDS_STANDARD);
                if (!strStandard.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_STANDARD, strStandard);
                    if ( iStandard ) m_NC.CheckSystemMenuItem ( this, FALSE, IDM_STANDARD,MF_CHECKED|MF_BYCOMMAND );
                }
            }

            //
            {
                int iUnChecked      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

                CString strUnchecked;
                strUnchecked.LoadString(IDS_UNCHECKED);
                if (!strUnchecked.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_UNCHECKED, strUnchecked);
                    if ( iUnChecked ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_UNCHECKED, MF_CHECKED|MF_BYCOMMAND );
                }
            }

            //
            {
                int iSearchExt      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );

                CString strSearchExt;
                strSearchExt.LoadString(IDS_SEARCH_EXT);
                if (!strSearchExt.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_SEARCH_EXT, strSearchExt);
                    if ( iSearchExt ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_EXT, MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iSearchPosExt   = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

                CString strSearchPosExt;
                strSearchPosExt.LoadString(IDS_SEARCH_POS_EXT);
                if (!strSearchPosExt.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_SEARCH_POS_EXT, strSearchPosExt);
                    if ( iSearchPosExt ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_POS_EXT, MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iDarkTheme      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );

                CString strDarkTheme;
                strDarkTheme.LoadString(IDS_DARK_THEME);
                if (!strDarkTheme.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_DARK_THEME, strDarkTheme);
                    if ( iDarkTheme == 1 ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_DARK_THEME, MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iDarkTheme      = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );

                CString strDarkFull;
                strDarkFull.LoadString(IDS_DARK_FULL);
                if (!strDarkFull.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_DARK_FULL, strDarkFull);
                    if ( iDarkTheme == 2 ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_DARK_FULL, MF_CHECKED|MF_BYCOMMAND);
                }
            }

            //
            {
                int iLogTrace      = CTRXGlobal::m_bTraceTRNGSavegame;

                CString strLogTrace;
                strLogTrace.LoadString(IDS_LOG_TRACE);
                if (!strLogTrace.IsEmpty())
                {
                    m_NC.AppendSystemMenu ( this, FALSE, MF_STRING, IDM_LOG_TRACE, strLogTrace);
                    if ( iLogTrace ) m_NC.CheckSystemMenuItem( this, FALSE, IDM_LOG_TRACE, MF_CHECKED|MF_BYCOMMAND);
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
        UINT remastered = m_NC.GetSystemMenuState ( this, FALSE, IDM_REMASTERED, MF_BYCOMMAND );
        UINT standard   = m_NC.GetSystemMenuState ( this, FALSE, IDM_STANDARD, MF_BYCOMMAND );
        if ( remastered == MF_CHECKED  )
        {
            if ( standard == MF_CHECKED )   // Only Remove Remastered if standard is enabled
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 0 );
                m_NC.CheckSystemMenuItem( this, FALSE, IDM_REMASTERED, MF_UNCHECKED|MF_BYCOMMAND);
                RemoveRemastered123Page();
            }
        }
        else
        {
            theApp.WriteProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_REMASTERED, MF_CHECKED|MF_BYCOMMAND);
            AddRemastered123Page();
        }
    }
    else if ((nID & 0xFFF0) == IDM_STANDARD)
    {
        UINT remastered = m_NC.CheckSystemMenuItem( this, FALSE, IDM_REMASTERED, MF_BYCOMMAND );
        UINT standard   = m_NC.CheckSystemMenuItem( this, FALSE, IDM_STANDARD, MF_BYCOMMAND );
        if ( standard == MF_CHECKED )
        {
            if ( remastered == MF_CHECKED ) // Only Remove Standard if Remastered is enabled
            {
                theApp.WriteProfileInt( PROFILE_MENU, PROFILE_STANDARD, 0 );
                m_NC.CheckSystemMenuItem( this, FALSE, IDM_STANDARD, MF_UNCHECKED|MF_BYCOMMAND);
                RemoveStandardPage();
            }
        }
        else
        {
            theApp.WriteProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_STANDARD, MF_CHECKED|MF_BYCOMMAND);
            AddStandardPage();
        }
    }
    else if ((nID & 0xFFF0) == IDM_UNCHECKED)
    {
        // int iUnchecked = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

        if ( CTRXGlobal::m_iUnchecked )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );
            CTRXGlobal::m_iUnchecked    = FALSE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_UNCHECKED, MF_UNCHECKED|MF_BYCOMMAND);
        }
        else
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 1 );
            CTRXGlobal::m_iUnchecked    = TRUE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_UNCHECKED, MF_CHECKED|MF_BYCOMMAND);
        }
    }
    else if ((nID & 0xFFF0) == IDM_SEARCH_EXT)
    {
        // int iSearchExt = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );

        if ( CTRXGlobal::m_iSearchExt )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 0 );
            CTRXGlobal::m_iSearchExt    = FALSE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_EXT, MF_UNCHECKED|MF_BYCOMMAND);
        }
        else
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_EXT, 1 );
            CTRXGlobal::m_iSearchExt    = TRUE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_EXT, MF_CHECKED|MF_BYCOMMAND);
        }
    }
    else if ((nID & 0xFFF0) == IDM_SEARCH_POS_EXT)
    {
        // int iSearchExt = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

        if ( CTRXGlobal::m_iSearchPosExt )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );
            CTRXGlobal::m_iSearchPosExt = FALSE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_POS_EXT, MF_UNCHECKED|MF_BYCOMMAND);
        }
        else
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 1 );
            CTRXGlobal::m_iSearchPosExt = TRUE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_SEARCH_POS_EXT,MF_CHECKED|MF_BYCOMMAND);
        }
    }
    else if ((nID & 0xFFF0) == IDM_LOG_TRACE )
    {
        // int iSearchExt = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_SEARCH_POS_EXT, 0 );

        if ( CTRXGlobal::m_bTraceTRNGSavegame )
        {
            CTRXGlobal::m_bTraceTRNGSavegame = FALSE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_LOG_TRACE, MF_UNCHECKED|MF_BYCOMMAND);
        }
        else
        {
            CTRXGlobal::m_bTraceTRNGSavegame = TRUE;
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_LOG_TRACE,MF_CHECKED|MF_BYCOMMAND);
        }
    }
    else if ( (nID & 0xFFF0) == IDM_DARK_THEME || (nID & 0xFFF0) == IDM_DARK_FULL )
    {
        int iDarkTheme = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );

        if ( (nID & 0xFFF0) == IDM_DARK_THEME && CTRXColors::m_iDarkTheme == 1 )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );
            CTRXColors::m_iDarkTheme    = 0;
            if ( CTRXColors::m_iSquareForce )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 0 );
                CTRXColors::m_iSquareCorner     = 0;
            }
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_DARK_THEME, MF_UNCHECKED|MF_BYCOMMAND);
            CTabCtrl* pTab = GetTabControl();
            if ( pTab )
            {
                pTab->ModifyStyle ( TCS_OWNERDRAWFIXED, NULL );
            }

            //  Reset Theme
            CTRXColors::SetWindowTheme ( this );

            //
            theApp.ReStartProgram(this);
        }
        else if ( (nID & 0xFFF0) == IDM_DARK_FULL && CTRXColors::m_iDarkTheme == 2 )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 0 );
            CTRXColors::m_iDarkTheme        = 0;
            if ( CTRXColors::m_iSquareForce )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 0 );
                CTRXColors::m_iSquareCorner     = 0;
            }
            m_NC.CheckSystemMenuItem( this, FALSE, IDM_DARK_FULL, MF_UNCHECKED|MF_BYCOMMAND);
            CTabCtrl* pTab = GetTabControl();
            if ( pTab )
            {
                pTab->ModifyStyle ( TCS_OWNERDRAWFIXED, NULL );
            }

            //  Reset Theme
            CTRXColors::SetWindowTheme ( this );

            //
            theApp.ReStartProgram(this);
        }
        else if ( (nID & 0xFFF0) == IDM_DARK_THEME )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 1 );
            CTRXColors::m_iDarkTheme        = 1;
            if ( CTRXColors::m_iSquareForce )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 0 );
                CTRXColors::m_iSquareCorner     = 0;
            }
            m_NC.CheckSystemMenuItem ( this, FALSE, IDM_DARK_THEME, MF_CHECKED|MF_BYCOMMAND);
            m_NC.CheckSystemMenuItem ( this, FALSE, IDM_DARK_FULL, MF_UNCHECKED|MF_BYCOMMAND);
            CTabCtrl* pTab = GetTabControl();
            if ( pTab )
            {
                pTab->ModifyStyle ( NULL, TCS_OWNERDRAWFIXED );
            }

            //  Reset Theme
            CTRXColors::SetWindowTheme ( this );

            //
            theApp.ReStartProgram(this);
        }
        else if ( (nID & 0xFFF0) == IDM_DARK_FULL )
        {
            theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_DARKTHEME, 2 );
            CTRXColors::m_iDarkTheme        = 2;
            if ( CTRXColors::m_iSquareForce )
            {
                theApp.WriteProfileInt( PROFILE_SETTING, PROFILE_SQUARECORNERS, 1 );
                CTRXColors::m_iSquareCorner     = 1;
            }
            m_NC.CheckSystemMenuItem ( this, FALSE, IDM_DARK_FULL, MF_CHECKED|MF_BYCOMMAND);
            m_NC.CheckSystemMenuItem ( this, FALSE, IDM_DARK_THEME, MF_UNCHECKED|MF_BYCOMMAND);

            CTabCtrl* pTab = GetTabControl();
            if ( pTab )
            {
                pTab->ModifyStyle ( NULL, TCS_OWNERDRAWFIXED );
            }

            //  Reset Theme
            CTRXColors::SetWindowTheme ( this );

            //
            theApp.ReStartProgram(this);
        }

        if ( m_Info_Page != NULL ) m_Info_Page->SetThemeChanged ( CTRXColors::m_iDarkTheme != 0 );
        if ( m_Equipment_Page != NULL ) m_Equipment_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );
        if ( m_Gun_Page != NULL ) m_Gun_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );
        if ( m_Ammos_Page != NULL ) m_Ammos_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );
        if ( m_Item_Page != NULL ) m_Item_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );
        if ( m_ItemTR4_Page != NULL ) m_ItemTR4_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );
        if ( m_Remastered123_Page != NULL ) m_Remastered123_Page->SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );

        SetThemeChanged (  CTRXColors::m_iDarkTheme != 0 );

        //  Redraw Window
        Invalidate();
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

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetAll::OnMove(int x, int y)
{
    CTRXPropertySheet::OnMove(x, y);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXPropertySheetAll::DoModal()
{
    //
    return CTRXPropertySheet::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetAll::PreCreateWindow(CREATESTRUCT& cs)
{
    //
    return CTRXPropertySheet::PreCreateWindow(cs);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXPropertySheetAll::OnGetDlgCode()
{
    //
    UINT iResult = CTRXPropertySheet::OnGetDlgCode();
    return iResult;
}
