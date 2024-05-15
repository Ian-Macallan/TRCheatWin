#pragma once
#include "afxcmn.h"

#include "resource.h"
#include "TR9SaveGame.h"

#include "TRXDialogBase.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXSecretsDialog
//
/////////////////////////////////////////////////////////////////////////////

class CTRXSecretsDialog : public CTRXDialogBase
{
		DECLARE_DYNAMIC(CTRXSecretsDialog)

	public:
		CTRXSecretsDialog(CWnd* pParent = NULL);   // constructeur standard
		virtual ~CTRXSecretsDialog();

	// Données de boîte de dialogue
		enum { IDD = IDD_LIST_SECRETS };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();

		CTRXListCtrlBase	m_List_Secrets;
		CTRXButtonBase		m_OK;
		CTRXButtonBase		m_Cancel;

		int					m_iTombraider;
		int					m_iBlock;
		int					m_iLevel;
};
