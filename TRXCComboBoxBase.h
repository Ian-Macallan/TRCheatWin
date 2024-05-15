#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXCComboBoxBase : public CComboBox
{
	DECLARE_DYNAMIC(CTRXCComboBoxBase)

	public:
		CTRXCComboBoxBase(void);
		virtual ~CTRXCComboBoxBase(void);
		DECLARE_MESSAGE_MAP()
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

