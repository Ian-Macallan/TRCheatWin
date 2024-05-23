#pragma once


//
/////////////////////////////////////////////////////////////////////////////
// CTRXItemsTR4.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "TRXPropertyPage.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXItemsTR4
//
/////////////////////////////////////////////////////////////////////////////

class CTRXItemsTR4 : public CTRXPropertyPage
{
        DECLARE_DYNAMIC(CTRXItemsTR4)

    public:
        CTRXItemsTR4();
        virtual ~CTRXItemsTR4();

    public:
        int EnableForVersion();
        void UpdateBuffer();
        void DisplayValues();
        void SetLabel ( UINT checkId, UINT editId, char **pTableInd, char **pTableGen, int index );
    // Données de boîte de dialogue
        enum { IDD = IDD_ITEMS_TR4 };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        CTRXEditBase m_Status;
        CTRXButtonBase m_Zero;
        CTRXButtonBase m_One;
        CTRXButtonBase m_Four;


        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();

        afx_msg void OnClickedItem01();
        afx_msg void OnClickedItem02();
        afx_msg void OnClickedItem03();
        afx_msg void OnClickedItem04();
        afx_msg void OnClickedItem05();
        afx_msg void OnClickedItem06();
        afx_msg void OnClickedItem07();
        afx_msg void OnClickedItem08();
        afx_msg void OnClickedItem09();
        afx_msg void OnClickedItem10();
        afx_msg void OnClickedItem11();
        afx_msg void OnClickedItem12();
        afx_msg void OnClickedItem13();
        afx_msg void OnClickedItem14();
        afx_msg void OnClickedItem15();
        afx_msg void OnClickedItem16();
        afx_msg void OnClickedItem17();
        afx_msg void OnClickedItem18();
        afx_msg void OnClickedItem19();
        afx_msg void OnClickedItem20();
        afx_msg void OnClickedItem21();
        afx_msg void OnClickedItem22();
        afx_msg void OnClickedItem23();
        afx_msg void OnClickedItem24();
        afx_msg void OnClickedItem25();
        afx_msg void OnClickedItem26();
        afx_msg void OnClickedItem27();
        afx_msg void OnClickedItem28();
        afx_msg void OnClickedItem29();
        afx_msg void OnBnClickedZero();
        afx_msg void OnBnClickedOne();
        afx_msg void OnBnClickedFour();
        afx_msg void OnChangeEdit();
        afx_msg void OnHelp();

        virtual BOOL OnInitDialog();
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnApply();
        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
};
