// TRXDiablogBase.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
#include "TRXDialogBase.h"
#include "afxdialogex.h"
#include "TRXTools.h"
#include "resource.h"
#include "TRXGlobal.h"

// Bo�te de dialogue CTRXDialogBase

IMPLEMENT_DYNAMIC(CTRXDialogBase, CDialog)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXDialogBase::CTRXDialogBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/) : CDialog(nIDTemplate, pParent)
{
    m_pNormalFont       = NULL;
    m_pItalicFont       = NULL;
    m_pBoldFont         = NULL;
    m_pFixedFont        = NULL;
    m_pFixedBoldFont    = NULL;

    m_SetManualCombo    = true;
    m_bInitDone         = false;

    m_hIcon             = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
    m_pMenu             = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXDialogBase::~CTRXDialogBase()
{
#define DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

    //  Do Not Delete m_pNormalFont

    DELETE_OBJECT(m_pItalicFont)
    DELETE_OBJECT(m_pBoldFont)
    DELETE_OBJECT(m_pFixedFont)
    DELETE_OBJECT(m_pFixedBoldFont)


}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CToolTipCtrl *CTRXDialogBase::GetToolTipCtrl()
{
    if ( m_bToolTip )
    {
        return &m_ToolTip;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXDialogBase, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_NCACTIVATE()
    ON_WM_NCPAINT()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCLBUTTONUP()
    ON_WM_INITMENUPOPUP()
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    ON_WM_NCRBUTTONUP()
    ON_WM_NCRBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSEHOVER()
    ON_WM_NCMOUSELEAVE()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXDialogBase
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXDialogBase::OnInitDialog()
{
    CDialog::OnInitDialog();

#if 0
    //  Reset To Old Windows Style
    SetWindowTheme ( GetSafeHwnd(), L"Explorer", L"");
#endif

    // TODO: Add Code Here
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

    // TODO: Add Code Here
    if( ! m_ToolTip.Create(this))
    {
       TRACE0("Unable to create the ToolTip!");
    }
    else
    {
        m_bToolTip  = TRUE;
        m_ToolTip.SetMaxTipWidth ( 392 );
        m_ToolTip.SetFont ( m_pFixedBoldFont );
        m_ToolTip.SetColors ( );
    }

    //  Activate Context Menu
    GetSystemMenu(FALSE);

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXDialogBase::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add Your Code Here
    m_ToolTip.RelayEvent(pMsg);

    return CDialog::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXDialogBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Add Code Here
    HBRUSH hBrush = CTRXTools::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Add Code Here
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXDialogBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add Code Here
    if ( CTRXTools::OnEraseBkgnd(pDC, this) )
    {
        return TRUE;
    }
    return CDialog::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXDialogBase::DoModal()
{
    // TODO: Add Code Here

    return CDialog::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXDialogBase::OnNcActivate(BOOL bActive)
{
    // TODO: Add Code Here
    BOOL bTreated = m_NC.Activate(this, bActive);
    if ( bTreated )
    {
        return TRUE;
    }

    return CDialog::OnNcActivate(bActive);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcPaint()
{
    //
    BOOL bTreated = m_NC.PaintWindow(this);
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcPaint();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcLButtonDown ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcLButtonDown(nHitTest, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcLButtonUp ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcLButtonUp(nHitTest, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

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
void CTRXDialogBase::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add Code Here
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
    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // TODO: Add Code Here
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

    CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnMouseMove ( this, nFlags, point );
    if ( bTreated )
    {
        return;
    }

    CDialog::OnMouseMove(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcMouseMove ( this, nHitTest, point );
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcMouseMove(nHitTest, point);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcMouseHover(UINT nFlags, CPoint point)
{
    // Cette fonctionnalit� requiert Windows�2000 ou une version ult�rieure.
    // Les symboles _WIN32_WINNT et WINVER doivent �tre >= 0x0500.
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcMouseHover ( this, nFlags, point );
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcMouseHover(nFlags, point);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDialogBase::OnNcMouseLeave()
{
    // Cette fonctionnalit� requiert Windows�2000 ou une version ult�rieure.
    // Les symboles _WIN32_WINNT et WINVER doivent �tre >= 0x0500.
    // TODO: Add Code Here
    BOOL bTreated = m_NC.OnNcMouseLeave (this);
    if ( bTreated )
    {
        return;
    }

    CDialog::OnNcMouseLeave();
}
