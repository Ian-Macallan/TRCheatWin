#if !defined(AFX_TR3ITEMS_H__4AB016C6_A177_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3ITEMS_H__4AB016C6_A177_11D2_8146_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
/////////////////////////////////////////////////////////////////////////////
// TRXItems.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "TRXPropertyPage.h"
#include "afxcmn.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXItems dialog
//
/////////////////////////////////////////////////////////////////////////////
class CTRXItems : public CTRXPropertyPage
{
        DECLARE_DYNCREATE(CTRXItems)

    // Construction
    public:
        int EnableForVersion();
        void UpdateBuffer();
        void DisplayValues();

        CTRXItems();
        ~CTRXItems();

    // Dialog Data
        //{{AFX_DATA(CTRXItems)
        enum { IDD = IDD_ITEMS };
        CTRXEditBase    m_Status;

        CTRXButtonBase  m_Treasure1;
        CTRXButtonBase  m_Treasure2;
        CTRXButtonBase  m_Treasure3;
        CTRXButtonBase  m_Treasure4;

        CTRXButtonBase  m_Map1;
        CTRXButtonBase  m_Map2;

        CTRXButtonBase  m_Item1;
        CTRXButtonBase  m_Item2;
        CTRXButtonBase  m_Item3;
        CTRXButtonBase  m_Item4;

        CTRXButtonBase  m_Key1;
        CTRXButtonBase  m_Key2;
        CTRXButtonBase  m_Key3;
        CTRXButtonBase  m_Key4;

        CTRXEditBase    m_N_Treasure1;
        CTRXEditBase    m_N_Treasure2;
        CTRXEditBase    m_N_Treasure3;
        CTRXEditBase    m_N_Treasure4;

        CTRXEditBase    m_N_Map1;
        CTRXEditBase    m_N_Map2;

        CTRXEditBase    m_N_Key1;
        CTRXEditBase    m_N_Key2;
        CTRXEditBase    m_N_Key3;
        CTRXEditBase    m_N_Key4;

        CTRXEditBase    m_N_Item1;
        CTRXEditBase    m_N_Item2;
        CTRXEditBase    m_N_Item3;
        CTRXEditBase    m_N_Item4;

        CTRXSpinButtonCtrlBase m_N_Key1Spin;
        CTRXSpinButtonCtrlBase m_N_Key2Spin;
        CTRXSpinButtonCtrlBase m_N_Key3Spin;
        CTRXSpinButtonCtrlBase m_N_Key4Spin;

        CTRXButtonBase m_Zero;
        CTRXButtonBase m_One;
        CTRXButtonBase m_Four;

            // NOTE - ClassWizard will add data members here.
            //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_DATA


    // Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CTRXItems)
        public:
        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        // Generated message map functions
        //{{AFX_MSG(CTRXItems)
        afx_msg void OnKey1();
        afx_msg void OnKey2();
        afx_msg void OnKey3();
        afx_msg void OnKey4();
        afx_msg void OnMap1();
        afx_msg void OnMap2();
        afx_msg void OnItem1();
        afx_msg void OnItem2();
        afx_msg void OnItem3();
        afx_msg void OnItem4();
        afx_msg void OnTreasure1();
        afx_msg void OnTreasure2();
        afx_msg void OnTreasure3();
        afx_msg void OnTreasure4();
        afx_msg void OnHelp();
            // NOTE: the ClassWizard will add member functions here
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    public:
        afx_msg void OnBnClickedZero();
        afx_msg void OnBnClickedOne();
        afx_msg void OnBnClickedFour();
        afx_msg void OnChangeEdit();

        virtual BOOL OnInitDialog();
        virtual BOOL OnApply();
        virtual void OnCancel();
        virtual void OnOK();
        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR3ITEMS_H__4AB016C6_A177_11D2_8146_444553540000__INCLUDED_)
