#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXSpinButtonCtrlBase : public CSpinButtonCtrl
{
    DECLARE_DYNAMIC(CTRXSpinButtonCtrlBase)

    public:
        CTRXSpinButtonCtrlBase(void);
        virtual ~CTRXSpinButtonCtrlBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

