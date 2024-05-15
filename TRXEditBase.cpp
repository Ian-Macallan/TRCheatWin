#include "stdafx.h"
#include "TRXEditBase.h"
#include "TRXTools.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXEditBase, CEdit)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXEditBase::CTRXEditBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXEditBase::~CTRXEditBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXEditBase, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXEditBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO: Add Code Here
	HBRUSH hBrush = CTRXTools::OnCtlColor ( pDC, pWnd, nCtlColor );
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Add Code Here
	return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXEditBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add Code Here
	if ( CTRXTools::OnEraseBkgnd(pDC, this ) )
	{
		return TRUE;
	}

	return CEdit::OnEraseBkgnd(pDC);
}

