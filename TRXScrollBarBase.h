#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXScrollBarBase : public CScrollBar
{
    DECLARE_DYNAMIC(CTRXScrollBarBase)

    public:
        CTRXScrollBarBase(void);
        virtual ~CTRXScrollBarBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

