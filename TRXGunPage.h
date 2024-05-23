#include "afxwin.h"
#if !defined(AFX_TR3AMMOSPAGE_H__2C731FC2_A0B6_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3AMMOSPAGE_H__2C731FC2_A0B6_11D2_8146_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
/////////////////////////////////////////////////////////////////////////////
// TRXGunPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "TRXPropertyPage.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXGunPage dialog
//
/////////////////////////////////////////////////////////////////////////////
class CTRXGunPage : public CTRXPropertyPage
{
        DECLARE_DYNCREATE(CTRXGunPage)

    // Construction
    public:
        int EnableForVersion();
        void UpdateBuffer();
        void DisplayValues();
        CTRXGunPage();
        ~CTRXGunPage();

    // Dialog Data
        //{{AFX_DATA(CTRXGunPage)
        enum { IDD = IDD_GUN_PAGE };
        CTRXEditBase    m_Status;
        CTRXButtonBase  m_Uzis;
        CTRXButtonBase  m_Rocket_Launcher;
        CTRXButtonBase  m_Riot_Gun;
        CTRXButtonBase  m_MP5;
        CTRXButtonBase  m_Harpoon;
        CTRXButtonBase  m_Guns;
        CTRXButtonBase  m_Grenade_Launcher;
        CTRXButtonBase  m_Desert_Eagle;
        CTRXButtonBase m_Crowbar;
        CTRXStaticBase m_BM_Guns;
        CTRXStaticBase m_BM_DesertEagle;
        CTRXStaticBase m_BM_Grenade;
        CTRXStaticBase m_BM_Uzis;
        CTRXStaticBase m_BM_Harpoon;
        CTRXStaticBase m_BM_MP5;
        CTRXStaticBase m_BM_Rocket;
        CTRXStaticBase m_BM_RiotGun;
        //}}AFX_DATA


    // Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CTRXGunPage)
        public:
        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        // Generated message map functions
        //{{AFX_MSG(CTRXGunPage)
        afx_msg void OnHelp();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    public:
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnApply();
        afx_msg void OnBnClickedDeserteagle();
        afx_msg void OnBnClickedGrenadelauncher();
        afx_msg void OnBnClickedGuns();
        afx_msg void OnBnClickedUzi();
        afx_msg void OnBnClickedRiotgun();
        afx_msg void OnBnClickedHarpoon();
        afx_msg void OnBnClickedMp5();
        afx_msg void OnBnClickedRocketlauncher();
        afx_msg void OnBnClickedCrowbar();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR3AMMOSPAGE_H__2C731FC2_A0B6_11D2_8146_444553540000__INCLUDED_)
