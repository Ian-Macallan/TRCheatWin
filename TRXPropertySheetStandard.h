#pragma once

#include "TRXCHEATWINDlg.h" // Added by ClassView
#include "TRXRemastered.h"  // Added by ClassView
#include "TRXPropertySheet.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertySheetStandard : public CTRXPropertySheet
{
    DECLARE_DYNAMIC(CTRXPropertySheetStandard)

    public:
        CTRXPropertySheetStandard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
        CTRXPropertySheetStandard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

        virtual ~CTRXPropertySheetStandard(void);

    // Attributes
    public:
        

    // Operations
    public:
        

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTRXPropertySheetRemastered)
        public:
        virtual BOOL OnInitDialog();
        //}}AFX_VIRTUAL

    // Implementation
    public:


        // Generated message map functions
    public:
        //{{AFX_MSG(CTRXPropertySheetRemastered)
            // NOTE - the ClassWizard will add and remove member functions here.
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg HCURSOR OnQueryDragIcon();
        afx_msg void OnMove(int x, int y);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg UINT OnGetDlgCode();
        //}}AFX_MSG

    protected:
        DECLARE_MESSAGE_MAP()

    public:
        virtual INT_PTR DoModal();
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

