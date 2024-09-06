#include "afxwin.h"
#if !defined(AFX_TR3EQUIPMENTPAGE_H__2C731FC3_A0B6_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3EQUIPMENTPAGE_H__2C731FC3_A0B6_11D2_8146_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
/////////////////////////////////////////////////////////////////////////////
// TRXEquipmentPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "TRXPropertyPage.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXEquipmentPage dialog
//
/////////////////////////////////////////////////////////////////////////////
class CTRXEquipmentPage : public CTRXPropertyPage
{
        DECLARE_DYNCREATE(CTRXEquipmentPage)

    // Construction
    public:
        int EnableForVersion();
        void UpdateBuffer();
        void DisplayValues();
        CTRXEquipmentPage();
        ~CTRXEquipmentPage();

    // Dialog Data
        //{{AFX_DATA(CTRXEquipmentPage)
        enum { IDD = IDD_EQUIPMENT };
        CTRXButtonBase  m_God;
        CTRXEditBase    m_Life;
        CTRXButtonBase  m_But_Flares;
        CTRXButtonBase  m_But_Small_MediPak;
        CTRXButtonBase  m_But_Large_MediPak;
        CTRXButtonBase  m_Unfinite_Air;
        CTRXEditBase    m_Status;
        CTRXEditBase    m_Small_Medipak;
        CTRXEditBase    m_Large_Medipak;
        CTRXEditBase    m_GunAmmos;
        CTRXEditBase    m_Flares;
        CTRXEditBase    m_Air;
        CTRXButtonBase  m_Laser;
        CTRXButtonBase  m_Binocular;
        CTRXButtonBase  m_Full_Kits;
        CTRXButtonBase  m_TRNG_Guns;
        CTRXButtonBase  m_TRNG_GodMode;
        CTRXButtonBase  m_TRNG_Doors;
        CTRXButtonBase  m_TRNG_Enemies;

        //}}AFX_DATA


    // Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CTRXEquipmentPage)
        public:
        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        // Generated message map functions
        //{{AFX_MSG(CTRXEquipmentPage)
        afx_msg void OnHelp();
        afx_msg void OnUnfiniteAir();
        afx_msg void OnButSmedi();
        afx_msg void OnButLmedi();
        afx_msg void OnButFlares();
        afx_msg void OnGod();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    public:
        afx_msg void OnBnClickedLaser();
        afx_msg void OnBnClickedBinocular();
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnInitDialog();
        virtual BOOL OnApply();
        afx_msg void OnChangeEdit();
        afx_msg void OnBnClickedFull();
        afx_msg void OnBnClickedTrngGuns();
        afx_msg void OnBnClickedTrngGodMode();
        afx_msg void OnBnClickedTrngDoors();
        afx_msg void OnBnClickedTrngEnemies();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR3EQUIPMENTPAGE_H__2C731FC3_A0B6_11D2_8146_444553540000__INCLUDED_)
