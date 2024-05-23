// TRXPropertySheet.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TRXEquipmentPage.h"   // Added by ClassView
#include "TRXGunPage.h" // Added by ClassView
#include "TRXInfoPage.h"    // Added by ClassView
#include "TRXItems.h"   // Added by ClassView
#include "TRXItemsTR4.h"    // Added by ClassView
#include "TRXAmmosPage.h"   // Added by ClassView
#include "TRXRemastered.h"  // Added by ClassView

#include "TRXPropertySheet.h"
#include "TRXColors.h"
#include "TRXGlobal.h"

//
extern CTRXCHEATWINApp theApp;

//
#define TAB_TEXT_SIZE   60

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
    m_Equipment         = NULL;
    m_Gun_Page          = NULL;
    m_Ammos_Page        = NULL;
    m_Item_Page         = NULL;
    m_ItemTR4_Page      = NULL;

    m_Remastered_Page   = NULL;
    m_bInitDone         = false;

    ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );

    m_hIcon             = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMenu             = NULL;
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
    m_Equipment         = NULL;
    m_Gun_Page          = NULL;
    m_Ammos_Page        = NULL;
    m_Item_Page         = NULL;
    m_ItemTR4_Page      = NULL;

    m_Remastered_Page   = NULL;
    m_bInitDone         = false;

    ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );

    m_hIcon             = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMenu             = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheet::~CTRXPropertySheet()
{
    RemoveStandardPage ();
    RemoveRemasteredPage();
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
    ON_WM_MOUSEMOVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSEHOVER()
    ON_WM_NCMOUSELEAVE()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheet::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();

    // TODO: Add Code Here
    if ( GetParent() == NULL )
    {
        // modify system menu & window styles so we can be
        // minimized and restored
        //
        DWORD dwStyle   = WS_MINIMIZEBOX | WS_SYSMENU;

        ModifyStyle( 0, dwStyle );

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
            pSysMenu->InsertMenu(1,MF_BYPOSITION,SC_MINIMIZE, _T("Mi&nimize"));
            pSysMenu->InsertMenu(0,MF_BYPOSITION,SC_RESTORE, _T("&Restore"));
        }
    }

    if ( GetParent() == NULL )
    {

        // TODO: Add your specialized code here
        // Add "About..." menu item to system menu.

        // IDM_ABOUTBOX must be in the system command range.
        ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
        ASSERT(IDM_ABOUTBOX < 0xF000);

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
            CString strAboutMenu;
            strAboutMenu.LoadString(IDS_ABOUTBOX);
            if (!strAboutMenu.IsEmpty())
            {
                pSysMenu->AppendMenu(MF_SEPARATOR);
                pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
            }
        }

        // Set the icon for this dialog.  The framework does this automatically
        //  when the application's main window is not a dialog
        SetIcon(m_hIcon, TRUE);         // Set big icon
        SetIcon(m_hIcon, FALSE);        // Set small icon
    }

    //
    if ( CTRXGlobal::m_iDarkTheme != 0 )
    {
        CTabCtrl* pTab = GetTabControl();
        if ( pTab )
        {
            pTab->ModifyStyle ( NULL, TCS_OWNERDRAWFIXED );
        }
    }

    //  Subclass Buttons
    CWnd *pwndOK        = GetDlgItem ( IDOK );
    if ( pwndOK )
    {
        m_Ok.SubclassDlgItem ( IDOK, this );
        m_Ok.ModifyStyle ( NULL, BS_OWNERDRAW );
    }

    CWnd *pwndCancel    = GetDlgItem ( IDCANCEL );
    if ( pwndCancel )
    {
        m_Cancel.SubclassDlgItem ( IDCANCEL, this );
        m_Cancel.ModifyStyle ( NULL, BS_OWNERDRAW );
    }

    CWnd *pwndHelp  = GetDlgItem ( IDHELP );
    if ( pwndHelp )
    {
        m_Help.SubclassDlgItem ( IDHELP, this );
        m_Help.ModifyStyle ( NULL, BS_OWNERDRAW );
    }

    CWnd *pwndApply = GetDlgItem ( ID_APPLY_NOW );
    if ( pwndApply )
    {
        m_ApplyNow.SubclassDlgItem ( ID_APPLY_NOW, this );
        m_ApplyNow.ModifyStyle ( NULL, BS_OWNERDRAW );
    }

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
        m_Equipment = new CTRXEquipmentPage();
        m_Gun_Page = new CTRXGunPage();
        m_Ammos_Page = new CTRXAmmosPage();
        m_Item_Page = new CTRXItems();
        m_ItemTR4_Page = new CTRXItemsTR4();

        m_Info_Page->SetApply ( m_bApplyActive );
        m_Equipment->SetApply ( m_bApplyActive );
        m_Gun_Page->SetApply ( m_bApplyActive );
        m_Ammos_Page->SetApply ( m_bApplyActive );
        m_Item_Page->SetApply ( m_bApplyActive );
        m_ItemTR4_Page->SetApply ( m_bApplyActive );

        AddPage ( m_Info_Page );
        AddPage ( m_Equipment );
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

        if ( m_Equipment != NULL )
        {
            RemovePage ( m_Equipment );
            delete m_Equipment;
            m_Equipment = NULL;
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
    // TODO: Add Code Here

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
    if ( m_Equipment != NULL ) m_Equipment->SetApply ( m_bApplyActive );
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
    // TODO: Add Code Here
    HBRUSH hBrush = CTRXTools::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Add Code Here
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheet::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add Code Here
    if ( CTRXTools::OnEraseBkgnd ( pDC, this ) )
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
            //
            //      Get Text
            char                szTabText[TAB_TEXT_SIZE];
            TC_ITEM             tci;

            ZeroMemory ( szTabText, sizeof(szTabText) );

            tci.mask            = TCIF_TEXT;
            tci.pszText         = szTabText;
            tci.cchTextMax       = sizeof ( szTabText );

            pTab->GetItem ( lpDrawItemStruct->itemID, &tci );

            //
            //      Get DC
            CDC *pDC = CDC::FromHandle ( lpDrawItemStruct->hDC );
            if ( pDC )
            {
                UINT uStyle = 0; // DFCS_ADJUSTRECT; // DFCS_FLAT; // ;
                CRect   rect = lpDrawItemStruct->rcItem;
                // pDC->DrawFrameControl ( &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle );
                if ( CTRXGlobal::m_iDarkTheme != 0 )
                {
                    RECT rect       = lpDrawItemStruct->rcItem;
                    rect.top        = rect.top;
                    rect.left       = rect.left;
                    rect.bottom     = rect.bottom;
                    rect.right      = rect.right;
                    pDC->FillRect ( &rect, CTRXColors::GetBlack32CBrush() );
                }
                else
                {
                    RECT rect       = lpDrawItemStruct->rcItem;
                    rect.top        = rect.top;
                    rect.left       = rect.left;
                    rect.bottom     = rect.bottom;
                    rect.right      = rect.right;
                    pDC->FillRect ( &rect, CTRXColors::GetWhite224CBrush() );
                }
                if ( CTRXGlobal::m_iDarkTheme != 0 )
                {
                    pDC->SetBkMode( TRANSPARENT );
                    pDC->SetTextColor ( CTRXColors::GetWhiteCR() );
                }
                else
                {
                    pDC->SetBkMode( TRANSPARENT );
                    //pDC->SetBkColor( );
                    pDC->SetTextColor ( CTRXColors::GetBlackCR() );
                }

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

    if ( CTRXGlobal::m_iDarkTheme != 0 )
    {
        static CTRXMenuBase     sysMenu;
        m_pMenu = sysMenu.SetSystemMenu ( this, pPopupMenu );
    }
    else if ( false && m_pMenu != NULL )
    {
        static CTRXMenuBase     sysMenu;
        sysMenu.UnSetSystemMenu ( pPopupMenu );
        m_pMenu = NULL;
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
    // TODO: Add Code Here
    BOOL bTreated = m_NC.PaintWindow(this, bActive);
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
    // TODO: Add Code Here
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
    // TODO: Add Code Here
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
    // TODO: Add Code Here
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
    // TODO: Add Code Here
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
    // Cette fonctionnalité requiert Windows 2000 ou une version ultérieure.
    // Les symboles _WIN32_WINNT et WINVER doivent être >= 0x0500.
    // TODO: Add Code Here
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
    // Cette fonctionnalité requiert Windows 2000 ou une version ultérieure.
    // Les symboles _WIN32_WINNT et WINVER doivent être >= 0x0500.
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcMouseLeave (this);
    if ( bTreated )
    {
        return;
    }

    CPropertySheet::OnNcMouseLeave();
}
