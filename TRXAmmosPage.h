#if !defined(AFX_TR3AMMOSPAGE_H__44DE8B01_A195_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3AMMOSPAGE_H__44DE8B01_A195_11D2_8146_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TRXAmmosPage.h : header file
//
#include "TRXPropertyPage.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXAmmosPage dialog
//
/////////////////////////////////////////////////////////////////////////////
class CTRXAmmosPage : public CTRXPropertyPage
{
	DECLARE_DYNCREATE(CTRXAmmosPage)

	// Construction
	public:
		int EnableForVersion();
		void UpdateBuffer();
		void DisplayValues();
		CTRXAmmosPage();
		~CTRXAmmosPage();

	// Dialog Data
		//{{AFX_DATA(CTRXAmmosPage)
		enum { IDD = IDD_AMMOS };

		CTRXButtonBase m_C_Guns;
		CTRXButtonBase	m_C_Uzis;
		CTRXButtonBase	m_C_Rocket_Launcher;
		CTRXButtonBase	m_C_MP5;
		CTRXButtonBase	m_C_Riot_Gun;
		CTRXButtonBase	m_C_Harpoon;
		CTRXButtonBase	m_C_Grenade_Launcher;
		CTRXButtonBase	m_C_Desert_Eagle;
		CTRXButtonBase	m_Unlimited;

		CTRXEditBase	m_Ammos_Guns;

		CTRXEditBase	m_Ammos_Uzis;
		CTRXEditBase	m_Ammos_Rocket_Launcher;

		CTRXEditBase	m_Ammos_Riot_Gun;
		CTRXEditBase	m_Ammos_Riot_Gun2;

		CTRXEditBase	m_Ammos_MP5;

		CTRXEditBase	m_Ammos_Harpoon;
		CTRXEditBase	m_Ammos_Harpoon2;
		CTRXEditBase	m_Ammos_Harpoon3;

		CTRXEditBase	m_Ammos_Grenade_Launcher;
		CTRXEditBase	m_Ammos_Grenade_Launcher2;
		CTRXEditBase	m_Ammos_Grenade_Launcher3;

		CTRXEditBase	m_Ammos_Desert_Eagle;

		CTRXEditBase	m_Status;

		CTRXStaticBase m_BM_RiotGun;
		CTRXStaticBase m_BM_DesertEagle;
		CTRXStaticBase m_BM_Uzis;
		CTRXStaticBase m_BM_Grenade;
		CTRXStaticBase m_BM_MP5;
		CTRXStaticBase m_BM_Rocket;
		CTRXStaticBase m_BM_Arrow;

		//}}AFX_DATA


	// Overrides
		// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CTRXAmmosPage)
		public:
		virtual BOOL OnSetActive();
		virtual BOOL OnKillActive();
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CTRXAmmosPage)
		afx_msg void OnHelp();
		afx_msg void OnCrocketlauncher();
		afx_msg void OnCuzis();
		afx_msg void OnCmp5();
		afx_msg void OnCdeserteagle();
		afx_msg void OnCgrenadelauncher();
		afx_msg void OnCriotgun();
		afx_msg void OnCharpoon();
		afx_msg void OnCguns();
		afx_msg void OnChangeEdit();

		afx_msg void OnBnClickedUnlimited();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	public:
		virtual void OnCancel();
		virtual void OnOK();
		virtual BOOL OnInitDialog();
		virtual BOOL OnApply();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR3AMMOSPAGE_H__44DE8B01_A195_11D2_8146_444553540000__INCLUDED_)
