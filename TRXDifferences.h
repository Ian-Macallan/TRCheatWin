#pragma once


#include "TR9SaveGame.h"
#include "TRSaveGame.h"
#include "resource.h"
#include "afxwin.h"

#include "TRXDialogBase.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXSecretsDialog
//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXDifferences

class CTRXDifferences : public CTRXDialogBase
{
		DECLARE_DYNAMIC(CTRXDifferences)

	public:
		CTRXDifferences(CWnd* pParent = NULL);   // constructeur standard
		virtual ~CTRXDifferences();

	// Données de boîte de dialogue
		enum { IDD = IDD_DIFFERENCES };

	public :
		//	For Remastered
		static INT_PTR MessageBoxR(const char *pMessage, const char *pTitle, UINT buttons);

		//	For Standard
		static INT_PTR MessageBoxS(const char *pMessage, const char *pTitle, UINT buttons);

		void AddToListCtrl(const char *pText );
		const char *GetLabel ( unsigned offset );

		//	Apply Undo Change
		void ApplyChanges();

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()

	public :
		int				m_iVersion;
		BOOL			m_bRemastered;
		BOOL			m_bInitList;
		UINT			m_Buttons;
		char			m_szTitle [  MAX_PATH ];
		char			m_szMessage [  MAX_PATH ];
		INT_PTR			m_Result;

		//
		CTRXListCtrlBase m_ListCtrl;
		CTRXButtonBase m_OK;
		CTRXButtonBase m_Cancel;
		CTRXEditBase m_Message;
		CTRXButtonBase m_Yes;
		CTRXButtonBase m_No;
		CTRXStaticBase m_Picture;

		CTRXListCtrlBase	*m_pListCtrl;

		//
		virtual BOOL OnInitDialog();
		virtual void OnCancel();
		virtual void OnOK();
		virtual INT_PTR DoModal();

		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBnClickedYes();
		afx_msg void OnBnClickedNo();
		afx_msg void OnItemchangedDifferences(NMHDR *pNMHDR, LRESULT *pResult);
};
