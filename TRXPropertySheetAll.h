#pragma once

#include "TRXCHEATWINDlg.h"		// Added by ClassView
#include "TRXEquipmentPage.h"	// Added by ClassView
#include "TRXGunPage.h"			// Added by ClassView
#include "TRXInfoPage.h"		// Added by ClassView
#include "TRXItems.h"			// Added by ClassView
#include "TRXItemsTR4.h"		// Added by ClassView
#include "TRXAmmosPage.h"		// Added by ClassView
#include "TRXRemastered.h"		// Added by ClassView
#include "TRXPropertySheet.h"

//
/////////////////////////////////////////////////////////////////////////////
// TRXPropertySheet.h : header file
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetAll
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertySheetAll : public CTRXPropertySheet
{
		DECLARE_DYNAMIC(CTRXPropertySheetAll)

	// Construction
	public:
		CTRXPropertySheetAll(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
		CTRXPropertySheetAll(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
		virtual ~CTRXPropertySheetAll();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTRXPropertySheetAll)
		public:
		virtual BOOL OnInitDialog();
		//}}AFX_VIRTUAL

	// Implementation
	public:


		// Generated message map functions
	public:
		//{{AFX_MSG(CTRXPropertySheetAll)
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

