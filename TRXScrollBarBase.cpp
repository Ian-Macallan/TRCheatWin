#include "stdafx.h"
#include "TRXScrollBarBase.h"
#include "TRXTools.h"
#include "TRXRemastered.h"
#include "TRXInfoPage.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXScrollBarBase, CScrollBar)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXScrollBarBase::CTRXScrollBarBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXScrollBarBase::~CTRXScrollBarBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXScrollBarBase, CScrollBar)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXScrollBarBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CScrollBar::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXScrollBarBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO
    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CScrollBar::OnEraseBkgnd(pDC);
}

