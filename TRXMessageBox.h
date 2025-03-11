#pragma once

#include "resource.h"
#include "TRXDialogBase.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXMessageBox
//
/////////////////////////////////////////////////////////////////////////////
class CTRXMessageBox : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXMessageBox)

    public:
        CTRXMessageBox(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXMessageBox();

        static BOOL m_bDontShowAgain;
        static BOOL m_bHideShowAgain;

        static UINT_PTR ShowMessage ( const char *pTitle, const char *pMessage, UINT icon = MB_ICONEXCLAMATION, BOOL bAlways = FALSE );
        static UINT_PTR MessageBox ( const char *pMessage, const char *pCaption = NULL, UINT icon = MB_ICONEXCLAMATION, BOOL bAlways = TRUE );

        void SetMessage ( const char *pTitle, const char *pMessage, UINT icon = MB_ICONEXCLAMATION, BOOL bAlways = FALSE );

    // Données de boîte de dialogue
        enum { IDD = IDD_TRXMESSAGEBOX };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    protected :
        char            m_szTitle [ MAX_PATH ];
        char            m_szMessage [ MAX_PATH ];
        UINT            m_Icon;
        INT_PTR         m_Result;

    public:
        CTRXEditBase m_Text;
        CButton m_Dont_Show;
        CStatic m_Picture;
        CTRXButtonBase m_OK;
        CTRXButtonBase m_Cancel;
        CTRXButtonBase m_Yes;
        CTRXButtonBase m_No;

        virtual BOOL OnInitDialog();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual INT_PTR DoModal();

        afx_msg void OnBnClickedDontShow();
        afx_msg void OnBnClickedYes();
        afx_msg void OnBnClickedNo();
        afx_msg void OnBnClickedOk();
        afx_msg void OnBnClickedCancel();
};
