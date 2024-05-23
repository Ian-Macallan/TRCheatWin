#pragma once

#include "resource.h"
#include "TRXPropertySheetAll.h"
#include "afxwin.h"

#include "TRXDialogBase.h"


//
/////////////////////////////////////////////////////////////////////////////
// DialogBox CTRXContainer
//
/////////////////////////////////////////////////////////////////////////////
class CTRXContainer : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXContainer)

    public:
        CTRXContainer(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXContainer();

    // Données de boîte de dialogue
        enum { IDD = IDD_CONTAINER };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

        CTRXPropertySheetAll    *m_pPropertySheet;
    public:
        virtual INT_PTR DoModal();
        virtual BOOL OnInitDialog();
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnMove(int x, int y);
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        afx_msg void OnBnClickedOk();
        afx_msg void OnBnClickedCancel();
        afx_msg UINT OnGetDlgCode();
};
