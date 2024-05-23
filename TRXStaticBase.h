#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXStaticBase : public CStatic
{
    DECLARE_DYNAMIC(CTRXStaticBase)

    public:
        CTRXStaticBase(void);
        virtual ~CTRXStaticBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

