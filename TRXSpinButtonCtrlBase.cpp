#include "stdafx.h"
#include "TRXSpinButtonCtrlBase.h"
#include "TRXTools.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXSpinButtonCtrlBase, CSpinButtonCtrl)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXSpinButtonCtrlBase::CTRXSpinButtonCtrlBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXSpinButtonCtrlBase::~CTRXSpinButtonCtrlBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXSpinButtonCtrlBase, CSpinButtonCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXSpinButtonCtrlBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CSpinButtonCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    //
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXSpinButtonCtrlBase::OnEraseBkgnd(CDC* pDC)
{
    //
    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CSpinButtonCtrl::OnEraseBkgnd(pDC);
}

