#pragma once

#include "TRXCHEATWINDlg.h" // Added by ClassView
#include "TRXRemastered456.h"  // Added by ClassView
#include "TRXPropertySheet.h"

//
/////////////////////////////////////////////////////////////////////////////
// TRXPropertySheet.h : header file
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetRemastered456
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertySheetRemastered456 : public CTRXPropertySheet
{
        DECLARE_DYNAMIC(CTRXPropertySheetRemastered456)

    // Construction
    public:
        CTRXPropertySheetRemastered456(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
        CTRXPropertySheetRemastered456(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

        virtual ~CTRXPropertySheetRemastered456();

    // Attributes
    public:

    // Operations
    public:
        

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTRXPropertySheetRemastered456)
        public:
        virtual BOOL OnInitDialog();
        //}}AFX_VIRTUAL

    // Implementation
    public:


        // Generated message map functions
    public:
        //{{AFX_MSG(CTRXPropertySheetRemastered456)
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

