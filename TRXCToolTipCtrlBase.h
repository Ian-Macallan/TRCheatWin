#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXCToolTipCtrlBase : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CTRXCToolTipCtrlBase)

	public:
		CTRXCToolTipCtrlBase(void);
		virtual ~CTRXCToolTipCtrlBase(void);

		DECLARE_MESSAGE_MAP()
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	public :
		void SetColors ( );

	//
	protected :
		BOOL		m_bDefaultColor;
		COLORREF	m_DefaultBkColor;
		COLORREF	m_DefaultTextColor;

};

