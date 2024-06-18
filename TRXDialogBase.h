#pragma once

#include <Windows.h>
#include <afx.h>
#include <afxwin.h>

#include "TRXEditBase.h"
#include "TRXStaticBase.h"
#include "TRXButtonBase.h"
#include "TRXListCtrlBase.h"
#include "TRXSpinButtonCtrlBase.h"
#include "TRXCComboBoxBase.h"
#include "TRXCToolTipCtrlBase.h"
#include "TRXMenuBase.h"
#include "TRXNCColor.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXDialogBase
//
/////////////////////////////////////////////////////////////////////////////
class CTRXDialogBase : public CDialog
{
        DECLARE_DYNAMIC(CTRXDialogBase)

    public:
        CTRXDialogBase(UINT nIDTemplate, CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXDialogBase();

        CToolTipCtrl    *GetToolTipCtrl();

    // Données de boîte de dialogue
    public :    
        CFont                   *m_pBoldFont;
        CFont                   *m_pItalicFont;
        CFont                   *m_pNormalFont;
        CFont                   *m_pFixedFont;
        CFont                   *m_pFixedBoldFont;

    protected :
        CTRXCToolTipCtrlBase    m_ToolTip;
        BOOL                    m_bToolTip;

        bool                    m_SetManualCombo;
        bool                    m_bInitDone;

        HICON                   m_hIcon;

        //
        CTRXMenuBase            *m_pMenu;

        //  Handle Npn Client
        CTRXNCColor             m_NC;

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        virtual INT_PTR DoModal();
        afx_msg BOOL OnNcActivate(BOOL bActive);
        afx_msg void OnNcPaint();
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
        afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseLeave();
        
};
