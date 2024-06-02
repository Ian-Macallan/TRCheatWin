#pragma once
#include "afxwin.h"
#include "TRXCHeaderCtrlBase.h"
#include "TRXScrollBarBase.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXListCtrlBase : public CListCtrl
{
    DECLARE_DYNAMIC(CTRXListCtrlBase)

    public:
        CTRXListCtrlBase(void);
        virtual ~CTRXListCtrlBase(void);
        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

    public :

    //
    protected :
        BOOL                    m_bDefaultColor;
        COLORREF                m_DefaultBkColor;
        COLORREF                m_DefaultTextColor;

        CTRXCHeaderCtrlBase     m_HeaderCtrl;
        bool                    m_bHeaderControlSubclassed;
};

