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

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()

	public:
		virtual BOOL OnInitDialog();

		void SetVersionFrom();

		CTRXStaticBase		m_Web;
		CTRXStaticBase		m_Version;
		CTRXStaticBase		m_Email;
		CTRXStaticBase		m_Copyright; 
		CTRXEditBase		m_Help_Text;
		CTRXButtonBase		m_OK;
		CTRXButtonBase		m_Cancel;

		afx_msg void OnVscrollHelpText();
};
