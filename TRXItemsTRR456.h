#pragma once

#include "afxcmn.h"

#include "resource.h"
#include "TR8SaveGame.h"

#include "resource.h"
#include "TRXDialogBase.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXItemsTRR456
//
/////////////////////////////////////////////////////////////////////////////
class CTRXItemsTRR456 : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXItemsTRR456)

    public:
        CTRXItemsTRR456(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXItemsTRR456();

        int EnableForVersion();
        void UpdateBuffer();
        void DisplayValues();
        void SetLabel ( UINT checkId, UINT editId, char **pTableInd, char **pTableGen, int index );

        // Données de boîte de dialogue
        enum { IDD = IDD_ITEMS_TRR456 };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        CTRXEditBase    m_Status;

        CTRXButtonBase  m_Zero;
        CTRXButtonBase  m_One;
        CTRXButtonBase  m_Four;

        CTRXButtonBase  m_Cancel;
        CTRXButtonBase  m_Ok;
        CTRXMenuBase    *m_pContextMenu;
        //
        int             m_MenuItemIndex;

        //
        int             m_iTombraider;
        int             m_iBlock;
        int             m_iLevel;

        BOOL GetTables ( char ***pTableGen, char ***pTableInd );

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
        afx_msg void OnRClicked( NMHDR * pNotifyStruct, LRESULT * result );
        afx_msg void OnItemsItem1();
        afx_msg void OnItemsItem2();
        afx_msg void OnItemsItem3();
        afx_msg void OnItemsItem4();
        afx_msg void OnItemsItem5();
        afx_msg void OnItemsItem6();
        afx_msg void OnItemsItem7();
        afx_msg void OnItemsItem8();
        afx_msg void OnItemsAll();

        virtual BOOL OnInitDialog();
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);

        void OnItemsItemN( int menuId, bool bForceCheck = false );
        BOOL GetMenuLabel( int button );

};
