// TRXPropertySheet.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TRXPropertySheet.h"
#include "TRXColors.h"
#include "TRXGlobal.h"

//
extern CTRXCHEATWINApp theApp;

//
#define TAB_TEXT_SIZE       60
#define MAX_TAB_SIZE        60

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheet
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertySheet, CPropertySheet)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheet::CTRXPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
    CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_bApplyActive      = FALSE;

    m_Info_Page         = NULL;
    m_Equipment_Page    = NULL;
    m_Gun_Page          = NULL;
    m_Ammos_Page        = NULL;
    m_Item_Page         = NULL;
    m_ItemTR4_Page      = NULL;

    m_Remastered_Page   = NULL;
    m_bInitDone         = false;

    ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );

    m_hIcon             = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMenu             = NULL;

    m_bToolTip          = FALSE;

    m_pNormalFont       = NULL;
    m_pItalicFont       = NULL;
    m_pBoldFont         = NULL;
    m_pFixedFont        = NULL;
    m_pFixedBoldFont    = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheet::CTRXPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage) :
        CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_bApplyActive      = FALSE;

    m_Info_Page         = NULL;
    m_Equipment_Page    = NULL;
    m_Gun_Page          = NULL;
    m_Ammos_Page        = NULL;
    m_Item_Page         = NULL;
    m_ItemTR4_Page      = NULL;

    m_Remastered_Page   = NULL;
    m_bInitDone         = false;

    ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );

    m_hIcon             = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMenu             = NULL;

    m_bToolTip          = FALSE;

    m_pNormalFont       = NULL;
    m_pItalicFont       = NULL;
    m_pBoldFont         = NULL;
    m_pFixedFont        = NULL;
    m_pFixedBoldFont    = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheet::~CTRXPropertySheet()
{
#define DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

    RemoveStandardPage ();
    RemoveRemasteredPage();

    DELETE_OBJECT(m_pItalicFont)
    DELETE_OBJECT(m_pBoldFont)
    DELETE_OBJECT(m_pFixedFont)
    DELETE_OBJECT(m_pFixedBoldFont)
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::SetParmPathname ( const char *pathname )
{
    if ( pathname != NULL )
    {
        strcpy_s ( m_ParmPathname, sizeof(m_ParmPathname), pathname );
        if ( m_Info_Page != NULL )
        {
            m_Info_Page->SetParmPathname ( m_ParmPathname );
            ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
        }
        if ( m_Remastered_Page != NULL )
        {
            m_Remastered_Page->SetParmPathname ( m_ParmPathname );
            ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::SetContextMenu ( CTRXMenuBase *pMenu )
{
    m_pMenu = pMenu;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::SetTheActivePage ( PROPERTY_PAGE page )
{
    switch ( page )
    {
        case PAGE_REMASTERED :
        {
            if ( m_Remastered_Page != NULL )
            {
                SetActivePage ( m_Remastered_Page );
            }
            break;
        }
        case PAGE_INFOS :
        {
            if ( m_Info_Page != NULL )
            {
                SetActivePage ( m_Info_Page );
            }
            break;
        }
        case PAGE_EQUIPMENTS :
        {
            if ( m_Equipment_Page != NULL )
            {
                SetActivePage ( m_Equipment_Page );
            }
            break;
        }
        case PAGE_GUNS :
        {
            if ( m_Gun_Page != NULL )
            {
                SetActivePage ( m_Gun_Page );
            }
            break;
        }
        case PAGE_AMMOS :
        {
            if ( m_Ammos_Page != NULL )
            {
                SetActivePage ( m_Ammos_Page );
            }
            break;
        }
        case PAGE_ITEMS :
        {
            if ( m_Item_Page != NULL )
            {
                SetActivePage ( m_Item_Page );
            }
            break;
        }
        case PAGE_ITEMS45 :
        {
            if ( m_ItemTR4_Page != NULL )
            {
                SetActivePage ( m_ItemTR4_Page );
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::DropToPage ( PROPERTY_PAGE page, const char *pFilename )
{
    switch ( page )
    {
        case PAGE_REMASTERED :
        {
            if ( m_Remastered_Page != NULL )
            {
                //  Set Active Must Be done Before
                SetTheActivePage ( PAGE_REMASTERED );
                m_Remastered_Page->DoDropFiles ( pFilename );
            }
            break;
        }
        case PAGE_INFOS :
        {
            if ( m_Info_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                m_Info_Page->DoDropFiles ( pFilename );
            }
            break;
        }
        case PAGE_EQUIPMENTS :
        {
            if ( m_Equipment_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                m_Info_Page->DoDropFiles ( pFilename );
            }
            break;
        }
        case PAGE_GUNS :
        {
            if ( m_Gun_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                m_Info_Page->DoDropFiles ( pFilename );
            }
            break;
        }
        case PAGE_AMMOS :
        {
            if ( m_Ammos_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                SetActivePage ( m_Ammos_Page );
            }
            break;
        }
        case PAGE_ITEMS :
        {
            if ( m_Item_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                m_Info_Page->DoDropFiles ( pFilename );
            }
            break;
        }
        case PAGE_ITEMS45 :
        {
            if ( m_ItemTR4_Page != NULL )
            {
                SetTheActivePage ( PAGE_INFOS );
                m_Info_Page->DoDropFiles ( pFilename );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertySheet, CPropertySheet)
    ON_WM_SYSCOMMAND()
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    ON_WM_INITMENUPOPUP()

    ON_WM_NCPAINT()
    ON_WM_NCACTIVATE()

    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCLBUTTONUP()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSEHOVER()
    ON_WM_NCMOUSELEAVE()

    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()

    ON_WM_CREATE()
    ON_WM_NCCALCSIZE()
    ON_WM_NCRBUTTONDOWN()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheet::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();

    //
    //
    TEXTMETRIC textMetrics;
    CDC *pDC = GetDC();
    GetTextMetrics ( pDC->m_hDC, &textMetrics );
    ReleaseDC ( pDC );

    //  Create Fonts
    LOGFONT logDefaultFont;
    LOGFONT logFont;

    m_pNormalFont               = GetFont();
    m_pNormalFont->GetLogFont(&logDefaultFont);

    logFont                     = logDefaultFont;
    logFont.lfWeight            = FW_BOLD;
    m_pBoldFont                 = new CFont();
    m_pBoldFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    logFont.lfItalic            = TRUE;
    m_pItalicFont               = new CFont();
    m_pItalicFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    m_pFixedFont                = new CFont();
    logFont.lfPitchAndFamily    = FIXED_PITCH;
    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
    m_pFixedFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    m_pFixedBoldFont            = new CFont();
    logFont.lfPitchAndFamily    = FIXED_PITCH;
    logFont.lfWeight            = FW_BOLD;
    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
    m_pFixedBoldFont->CreateFontIndirect(&logFont);    // Create the font.

    //
    CTRXColors::SetWindowTheme ( this );

    //
    if ( GetParent() == NULL )
    {
        // modify system menu & window styles so we can be
        // minimized and restored
        DWORD dwStyle   = WS_MINIMIZEBOX | WS_SYSMENU;
        ModifyStyle ( 0, dwStyle );

        //  Makes Rectangle corners but Old Buttons appears

        //  10101 : WS_EX_DLGMODALFRAME WS_EX_WINDOWEDGE WS_EX_CONTROLPARENT
        DWORD dwStyleEx = GetExStyle();

        //
        m_NC.InsertSystemMenu ( this, FALSE, 1, MF_BYPOSITION, SC_MINIMIZE, _T("Mi&nimize") );
        m_NC.InsertSystemMenu ( this, FALSE, 0, MF_BYPOSITION, SC_RESTORE, _T("&Restore") );
    }

    if ( GetParent() == NULL )
    {
        //
        // Add "About..." menu item to system menu.

        // IDM_ABOUTBOX must be in the system command range.
        ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
        ASSERT(IDM_ABOUTBOX < 0xF000);

        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            m_NC.AppendSystemMenu( this, FALSE, MF_SEPARATOR );
            m_NC.AppendSystemMenu( this, FALSE, MF_STRING, IDM_ABOUTBOX, strAboutMenu );
        }

        // Set the icon for this dialog.  The framework does this automatically
        //  when the application's main window is not a dialog
        SetIcon(m_hIcon, TRUE);         // Set big icon
        SetIcon(m_hIcon, FALSE);        // Set small icon
    }

    //
    if ( CTRXColors::m_iDarkTheme != 0 )
    {
        CTabCtrl* pTab = GetTabControl();
        if ( pTab )
        {
            pTab->ModifyStyle ( NULL, TCS_OWNERDRAWFIXED );
        }
    }

    if ( ! m_bToolTip )
    {
        if ( m_ToolTip.Create(this) )
        {
            m_bToolTip  = TRUE;
            m_ToolTip.SetMaxTipWidth ( 392 );
            m_ToolTip.SetFont ( m_pFixedBoldFont );
            m_ToolTip.SetColors ( );
        }
    }

    //  Subclass Buttons
    CWnd *pwndOK        = GetDlgItem ( IDOK );
    if ( pwndOK )
    {
        m_Ok.SubclassDlgItem ( IDOK, this );
        m_Ok.ModifyStyle ( NULL, BS_OWNERDRAW );
        m_Ok.SetWindowText ( "Ok" );
    }

    CWnd *pwndCancel    = GetDlgItem ( IDCANCEL );
    if ( pwndCancel )
    {
        m_Cancel.SubclassDlgItem ( IDCANCEL, this );
        m_Cancel.ModifyStyle ( NULL, BS_OWNERDRAW );
        m_Cancel.SetWindowText ( "Cancel" );
    }

    CWnd *pwndHelp  = GetDlgItem ( IDHELP );
    if ( pwndHelp )
    {
        m_Help.SubclassDlgItem ( IDHELP, this );
        m_Help.ModifyStyle ( NULL, BS_OWNERDRAW );
        m_Help.SetWindowText ( "Help" );
    }

    CWnd *pwndApply = GetDlgItem ( ID_APPLY_NOW );
    if ( pwndApply )
    {
        m_ApplyNow.SubclassDlgItem ( ID_APPLY_NOW, this );
        m_ApplyNow.ModifyStyle ( NULL, BS_OWNERDRAW );
        m_ApplyNow.SetWindowText ( "Apply" );
    }

    //
    if ( m_bToolTip )
    {
        m_ToolTip.Activate(  TRUE );
    }

    //
    if ( CTRXColors::m_iSquareCorner == 1 )
    {
	    if ( theApp.OSVersionGreaterThan ( 6, 1 ) )
	    {
            if ( CTRXColors::m_iDarkTheme == 2 )
            {
                ModifyStyle ( WS_SYSMENU, NULL );
            }
        }
    }

    //
    m_bInitDone = true;

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::AddStandardPage ()
{
    if ( ! m_iStandardAdded )
    {
        m_Info_Page = new CTRXInfoPage();
        if ( strlen(m_ParmPathname) > 0 )
        {
            m_Info_Page->SetParmPathname ( m_ParmPathname );
            ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
        }
        m_Equipment_Page = new CTRXEquipmentPage();
        m_Gun_Page = new CTRXGunPage();
        m_Ammos_Page = new CTRXAmmosPage();
        m_Item_Page = new CTRXItems();
        m_ItemTR4_Page = new CTRXItemsTR4();

        m_Info_Page->SetApply ( m_bApplyActive );
        m_Equipment_Page->SetApply ( m_bApplyActive );
        m_Gun_Page->SetApply ( m_bApplyActive );
        m_Ammos_Page->SetApply ( m_bApplyActive );
        m_Item_Page->SetApply ( m_bApplyActive );
        m_ItemTR4_Page->SetApply ( m_bApplyActive );

        AddPage ( m_Info_Page );
        AddPage ( m_Equipment_Page );
        AddPage ( m_Gun_Page );
        AddPage ( m_Ammos_Page );
        AddPage ( m_Item_Page );
        AddPage ( m_ItemTR4_Page );
        m_iStandardAdded    = TRUE;

#if 0
        m_psh.dwFlags &= ~PSH_HASHELP;
        for(int i = 0; i < GetPageCount(); ++i )
        {
            GetPage(i)->m_psp.dwFlags &= ~PSP_HASHELP;
        }
#endif

    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::AddRemasteredPage ()
{
    if ( ! m_iRemasteredAdded )
    {
        m_Remastered_Page = new CTRXRemastered();
        if ( strlen(m_ParmPathname) > 0 )
        {
            m_Remastered_Page->SetParmPathname ( m_ParmPathname );
            ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
        }
        m_Remastered_Page->SetApply ( m_bApplyActive );

        AddPage ( m_Remastered_Page );
        m_iRemasteredAdded  = TRUE;

#if 0
        m_psh.dwFlags &= ~PSH_HASHELP;
        for(int i = 0; i < GetPageCount(); ++i )
        {
            GetPage(i)->m_psp.dwFlags &= ~PSP_HASHELP;
        }
#endif
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::RemoveStandardPage ()
{
    if ( m_iStandardAdded )
    {
        if ( m_Info_Page != NULL )
        {
            RemovePage ( m_Info_Page );
            delete m_Info_Page;
            m_Info_Page = NULL;
        }

        if ( m_Equipment_Page != NULL )
        {
            RemovePage ( m_Equipment_Page );
            delete m_Equipment_Page;
            m_Equipment_Page = NULL;
        }

        if ( m_Gun_Page != NULL )
        {
            RemovePage ( m_Gun_Page );
            delete m_Gun_Page;
            m_Gun_Page = NULL;
        }

        if ( m_Ammos_Page != NULL )
        {
            RemovePage ( m_Ammos_Page );
            delete m_Ammos_Page;
            m_Ammos_Page = NULL;
        }

        if ( m_Item_Page != NULL )
        {
            RemovePage ( m_Item_Page );
            delete m_Item_Page;
            m_Item_Page = NULL;
        }

        if ( m_ItemTR4_Page != NULL )
        {
            RemovePage ( m_ItemTR4_Page );
            delete m_ItemTR4_Page;
            m_ItemTR4_Page = NULL;
        }

        m_iStandardAdded    = FALSE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::RemoveRemasteredPage ()
{
    if ( m_iRemasteredAdded )
    {
        if ( m_Remastered_Page != NULL )
        {
            RemovePage ( m_Remastered_Page );
            delete m_Remastered_Page;
            m_Remastered_Page = NULL;
        }

        m_iRemasteredAdded  = FALSE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnSysCommand(UINT nID, LPARAM lParam)
{
    //

    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CPropertySheet::OnSysCommand(nID, lParam);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::SetApply(BOOL bApply )
 {
     m_bApplyActive = bApply;

    if ( m_Info_Page != NULL ) m_Info_Page->SetApply ( m_bApplyActive );
    if ( m_Equipment_Page != NULL ) m_Equipment_Page->SetApply ( m_bApplyActive );
    if ( m_Gun_Page != NULL ) m_Gun_Page->SetApply ( m_bApplyActive );
    if ( m_Ammos_Page != NULL ) m_Ammos_Page->SetApply ( m_bApplyActive );
    if ( m_Item_Page != NULL ) m_Item_Page->SetApply ( m_bApplyActive );
    if ( m_ItemTR4_Page != NULL ) m_ItemTR4_Page->SetApply ( m_bApplyActive );
    if ( m_Remastered_Page != NULL ) m_Remastered_Page->SetApply ( m_bApplyActive );

 }

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXPropertySheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

    //
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheet::OnEraseBkgnd(CDC* pDC)
{
    //
    if ( CTRXColors::OnEraseBkgnd ( pDC, this ) )
    {
        return TRUE;
    }
    return CPropertySheet::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // Tabs
    if ( nIDCtl == AFX_IDC_TAB_CONTROL )
    {
        CTabCtrl* pTab = GetTabControl();
        if ( pTab != NULL )
        {
            CDC *pDC = CDC::FromHandle ( lpDrawItemStruct->hDC );
            if ( pDC )
            {
                char        szTabText[TAB_TEXT_SIZE];
                TC_ITEM     tci;

                //
                RECT fullTabRect;
                pTab->GetClientRect ( &fullTabRect );
                
                pDC->FillRect ( &fullTabRect, CTRXColors::GetBKHeaderCBrush ( CTRXColors::m_iDarkTheme != 0 ) );

                //  Redraw All Tabs
                for ( int i = 0; i < pTab->GetItemCount(); i++ )
                {
                    RECT itemRect;
                    pTab->GetItemRect ( i, &itemRect );
                    pDC->FillRect ( &itemRect, CTRXColors::GetBKHeaderCBrush( CTRXColors::m_iDarkTheme != 0 ) );

                    ZeroMemory ( &tci, sizeof(tci) );

                    tci.mask            = TCIF_TEXT;
                    tci.pszText         = szTabText;
                    tci.cchTextMax      = sizeof ( szTabText );

                    ZeroMemory ( szTabText, sizeof(szTabText) );
                    pTab->GetItem ( i, &tci );

                    pDC->SetBkMode( TRANSPARENT );
                    pDC->SetTextColor ( CTRXColors::GetFGHeaderCR ( CTRXColors::m_iDarkTheme != 0 ) );
                    pDC->DrawText ( szTabText, &itemRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
                }


                //
                //      Draw The Tab Selected
                ZeroMemory ( &tci, sizeof(tci) );
                tci.mask            = TCIF_TEXT;
                tci.pszText         = szTabText;
                tci.cchTextMax      = sizeof ( szTabText );

                ZeroMemory ( szTabText, sizeof(szTabText) );
                pTab->GetItem ( lpDrawItemStruct->itemID, &tci );

                RECT rect       = lpDrawItemStruct->rcItem;
                rect.top        = rect.top;
                rect.left       = rect.left;
                rect.bottom     = rect.bottom;
                rect.right      = rect.right;
                pDC->FillRect ( &rect, CTRXColors::GetBKHeaderCBrush( CTRXColors::m_iDarkTheme != 0 ) );

                pDC->SetBkMode( TRANSPARENT );
                pDC->SetTextColor ( CTRXColors::GetFGHeaderCR ( CTRXColors::m_iDarkTheme != 0 ) );

                pDC->DrawText ( szTabText, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
            }

            return;
        }
    }

    //  Menu
    if ( lpDrawItemStruct != NULL )
    {
        if ( lpDrawItemStruct->CtlType == ODT_MENU )
        {
            if ( m_pMenu != NULL )
            {
                m_pMenu->DrawItem(lpDrawItemStruct);
                return;
            }
        }
    }
    
    //
    CPropertySheet::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if ( lpMeasureItemStruct != NULL )
    {
        if ( lpMeasureItemStruct->CtlType == ODT_MENU )
        {
            if ( m_pMenu != NULL )
            {
                m_pMenu->MeasureItem(lpMeasureItemStruct);
                return;
            }
        }
    }

    CPropertySheet::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CPropertySheet::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    CTRXMenuBase::AdjustMenu ( this, pPopupMenu );
    if ( CTRXColors::m_iDarkTheme != 0 )
    {
        static CTRXMenuBase     popMenu;
        m_pMenu = popMenu.SetSystemMenu ( this, pPopupMenu );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcPaint()
{
    //
    BOOL bTreated = m_NC.PaintWindow(this);
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcPaint();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheet::OnNcActivate(BOOL bActive)
{
    //
    BOOL bTreated = m_NC.Activate(this, NULL, bActive);
    if ( bTreated )
    {
        return TRUE;
    }

    return CPropertySheet::OnNcActivate(bActive);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcLButtonDown ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcLButtonDown(nHitTest, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    //
    CPropertySheet::OnNcLButtonUp(nHitTest, point);
    BOOL bTreated = m_NC.OnNcLButtonUp ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcLButtonUp(nHitTest, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnMouseMove(UINT nFlags, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnMouseMove ( this, nFlags, point );
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnMouseMove(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcMouseMove ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcMouseMove(nHitTest, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcMouseHover(UINT nFlags, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcMouseHover ( this, nFlags, point );
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcMouseHover(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcMouseLeave()
{
    //
    BOOL bTreated = m_NC.OnNcMouseLeave (this);
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcMouseLeave();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnLButtonUp(UINT nFlags, CPoint point)
{
    //

    BOOL bTreated = m_NC.OnLButtonUp (this, nFlags, point );
    CPropertySheet::OnLButtonUp(nFlags, point);

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnLButtonDown(UINT nFlags, CPoint point)
{
    //

    CPropertySheet::OnLButtonDown(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::SetThemeChanged ( bool bDarkTheme )
{
    if ( m_bToolTip )
    {
        m_ToolTip.SetColors();
    }

	if ( theApp.OSVersionGreaterThan ( 6, 1 ) )
	{
        if ( CTRXColors::m_iSquareCorner == 1 )
        {
            if ( CTRXColors::m_iDarkTheme == 2 )
            {
                CTRXColors::SetWindowTheme ( this );
                ModifyStyle ( WS_SYSMENU, NULL  );
                SetContextMenu ( NULL );
                CTRXMenuBase::SetOwnDraw ( GetSystemMenu(FALSE), true );
            }
            else
            {
                CTRXColors::SetWindowTheme ( this );
                ModifyStyle ( NULL, WS_SYSMENU );
                SetContextMenu ( NULL );
                CTRXMenuBase::SetOwnDraw ( GetSystemMenu(FALSE), false );
                SetWindowPos ( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
            }
	    }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    //
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }
    //

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    //
    CPropertySheet::OnNcCalcSize(bCalcValidRects, lpncsp);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheet::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
    //
    if ( m_NC.OnNcRButtonDown ( this, nHitTest, point ) )
    {
        return;
    }

    CPropertySheet::OnNcRButtonDown(nHitTest, point);
}
