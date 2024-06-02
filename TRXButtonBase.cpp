#include "stdafx.h"
#include "TRXButtonBase.h"
#include "TRXTools.h"
#include "TRXColors.h"
#include "TRXGlobal.h"
#include "TRXGDI.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXButtonBase, CButton)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXButtonBase::CTRXButtonBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXButtonBase::~CTRXButtonBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXButtonBase, CButton)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXButtonBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Add Code Here
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Add Code Here
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXButtonBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add Code Here
    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CButton::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXButtonBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    UINT uStyle = DFCS_BUTTONPUSH;

    static char szText [ MAX_PATH ];

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if ( pDC )
    {

        // This code only works with buttons.
        ASSERT ( lpDrawItemStruct->CtlType == ODT_BUTTON );

        // If drawing selected, add the pushed style to DrawFrameControl.
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            uStyle |= DFCS_PUSHED;
        }

        // Draw the button frame.
        // pDC->DrawEdge ( &lpDrawItemStruct->rcItem, EDGE_ETCHED, BF_ADJUST );
        pDC->DrawFrameControl ( &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle );
        if ( CTRXGlobal::m_iDarkTheme != 0 )
        {
            RECT rect = lpDrawItemStruct->rcItem;
            rect.top        = rect.top + 1;
            rect.left       = rect.left + 1;
            rect.bottom     = rect.bottom - 2;
            rect.right      = rect.right - 2;
            pDC->FillRect ( &rect, CTRXColors::GetBlack32CBrush() );
        }

        // Get the button's text.
        GetWindowText ( szText, sizeof(szText) );

        // Draw the button text using the text color white.
        COLORREF crOldColor;
        if ( lpDrawItemStruct->itemState & ODS_DISABLED )
        {
            if ( CTRXGlobal::m_iDarkTheme != 0 )
            {
                crOldColor = pDC->SetTextColor ( CTRXColors::GetGreyCR() );
            }
            else
            {
                crOldColor = pDC->SetTextColor ( CTRXColors::GetGreyCR() );
            }
        }
        else
        {
            if ( CTRXGlobal::m_iDarkTheme != 0 )
            {
                crOldColor = pDC->SetTextColor ( CTRXColors::GetWhiteCR() );
            }
            else
            {
                crOldColor = pDC->SetTextColor ( CTRXColors::GetBlackCR() );
            }

        }

        pDC->DrawText ( szText, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER );

        pDC->SetTextColor( crOldColor);
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXButtonBase::OnRButtonUp(UINT nFlags, CPoint point)
{
    //
    NMHDR hdr;
    hdr.code        = NM_RCLICK;
    hdr.hwndFrom    = this->GetSafeHwnd();
    hdr.idFrom      = GetDlgCtrlID();
    this->GetParent()->SendMessage(WM_NOTIFY, (WPARAM)hdr.idFrom, (LPARAM)&hdr );
}
