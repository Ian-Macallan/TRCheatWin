#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <Windows.h>

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXCHeaderCtrlBase : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CTRXCHeaderCtrlBase)

    public:
        CTRXCHeaderCtrlBase(void);
        virtual ~CTRXCHeaderCtrlBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

