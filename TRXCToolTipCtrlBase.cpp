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
    // TODO
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CToolTipCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXCToolTipCtrlBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO
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
    if ( CTRXColors::m_iDarkTheme == 0 )
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

    SetTipTextColor ( CTRXColors::GetFGTipColor(CTRXColors::m_iDarkTheme != 0) );
    SetTipBkColor ( CTRXColors::GetBKTipColor(CTRXColors::m_iDarkTheme != 0) );
}

