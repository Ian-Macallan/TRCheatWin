#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXEditBase : public CEdit
{
    DECLARE_DYNAMIC(CTRXEditBase)

    public:
        CTRXEditBase(void);
        virtual ~CTRXEditBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

