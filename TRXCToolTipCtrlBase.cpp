#include "stdafx.h"
#include "TRXCToolTipCtrlBase.h"
#include "TRXTools.h"
#include "TRXGlobal.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXCToolTipCtrlBase, CToolTipCtrl)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXCToolTipCtrlBase::CTRXCToolTipCtrlBase(void)
{
    m_bDefaultColor = FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXCToolTipCtrlBase::~CTRXCToolTipCtrlBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXCToolTipCtrlBase, CToolTipCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXCToolTipCtrlBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Add Code Here
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CToolTipCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Add Code Here
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXCToolTipCtrlBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add Code Here
    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CToolTipCtrl::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCToolTipCtrlBase::SetColors ( )
{
    if ( CTRXGlobal::m_iDarkTheme == 0 )
    {
        if ( m_bDefaultColor )
        {
            SetTipBkColor ( m_DefaultBkColor );
            SetTipTextColor ( m_DefaultTextColor );
        }

        return;
    }

    //
    if ( ! m_bDefaultColor )
    {
        m_DefaultTextColor  = GetTipTextColor();
        m_DefaultBkColor    = GetTipBkColor();
        m_bDefaultColor     = TRUE;
    }

    SetTipBkColor ( CTRXColors::GetBkColor() );
    SetTipTextColor ( CTRXColors::GetTextColor() );
}

