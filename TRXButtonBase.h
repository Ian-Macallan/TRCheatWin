#pragma once
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXButtonBase : public CButton
{
    DECLARE_DYNAMIC(CTRXButtonBase)

    public:
        CTRXButtonBase(void);
        virtual ~CTRXButtonBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

