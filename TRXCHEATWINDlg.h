// TRXCHEATWINDlg.h : header file
//

#if !defined(AFX_TR3CHEATWINDLG_H__7B7A8C29_9CDD_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3CHEATWINDLG_H__7B7A8C29_9CDD_11D2_8146_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TRXDialogBase.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTR3CHEATWINDlg dialog
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CTRXDialogBase
{
    public:
        void SetVersionFrom();
        CAboutDlg();

    // Dialog Data
    public:
        //{{AFX_DATA(CAboutDlg)
        enum { IDD = IDD_ABOUTBOX };
        CTRXStaticBase  m_Web;
        CTRXStaticBase  m_Version;
        CTRXStaticBase  m_Email;
        CTRXStaticBase  m_Copyright;
        CTRXButtonBase  m_Ok;
        CTRXButtonBase  m_Web_Site;
        //}}AFX_DATA

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAboutDlg)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        //{{AFX_MSG(CAboutDlg)
        afx_msg void OnMacallan();
        virtual BOOL OnInitDialog();
        afx_msg void OnWeb();
        afx_msg void OnEmail();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
public:
    CStatic m_OSVersion;
};

#endif // !defined(AFX_TR3CHEATWINDLG_H__7B7A8C29_9CDD_11D2_8146_444553540000__INCLUDED_)
