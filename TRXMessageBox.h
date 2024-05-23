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

        static BOOL bDontShowAgain;
        static UINT_PTR ShowMessage ( const char *pTitle, const char *pMessage, UINT icon = MB_ICONEXCLAMATION );

        void SetMessage ( const char *pTitle, const char *pMessage, UINT icon = MB_ICONEXCLAMATION );

    // Données de boîte de dialogue
        enum { IDD = IDD_TRXMESSAGEBOX };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    protected :
        char    m_szTitle [ MAX_PATH ];
        char    m_szMessage [ MAX_PATH ];
        UINT    m_Icon;

    public:
        CTRXEditBase m_Text;
        CButton m_Dont_Show;
        CStatic m_Picture;
        CTRXButtonBase m_OK;
        CTRXButtonBase m_Cancel;
        virtual BOOL OnInitDialog();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        afx_msg void OnBnClickedDontShow();
};
