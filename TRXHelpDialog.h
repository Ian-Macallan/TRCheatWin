#pragma once

#include <afxdlgs.h>
#include "resource.h"
#include "TRXDialogBase.h"
#include "afxwin.h"

// Boîte de dialogue CTRXHelpDialog

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXHelpDialog : public CTRXDialogBase
{
    DECLARE_DYNAMIC(CTRXHelpDialog)

    public:
        CTRXHelpDialog(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXHelpDialog();

    // Données de boîte de dialogue
        enum { IDD = IDD_TRXHELPDIALOG };


    public :
        DWORD   m_ID_Resource;
        int     m_Tombraider;
        int     m_LevelNumber;
        char    m_szTitle [ MAX_PATH ];
        char    m_szLevelName [ MAX_PATH ];
        char    m_szWalkThru [ MAX_PATH ];

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();

        void SetVersionFrom();

        CTRXStaticBase      m_Web;
        CTRXStaticBase      m_Version;
        CTRXStaticBase      m_Email;
        CTRXStaticBase      m_Copyright;
        CTRXEditBase        m_Help_Text;
        CTRXButtonBase      m_OK;
        CTRXButtonBase      m_Cancel;
        CTRXButtonBase      m_Walk;

        afx_msg void OnVscrollHelpText();
        afx_msg void OnBnClickedWalkthru();
};
