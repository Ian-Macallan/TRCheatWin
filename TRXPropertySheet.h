#pragma once

#include "TRXAmmosPage.h"
#include "TRXMenuBase.h"
#include "TRXNCColor.h"

enum PROPERTY_PAGE
{
    PAGE_REMASTERED,
    PAGE_INFOS,
    PAGE_EQUIPMENTS,
    PAGE_GUNS,
    PAGE_AMMOS,
    PAGE_ITEMS,
    PAGE_ITEMS45,
};

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheet
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertySheet : public CPropertySheet
{
        DECLARE_DYNAMIC(CTRXPropertySheet)

        friend class CTRXRemastered;
        friend class CTRXInfoPage;

    protected  :
        CTRXRemastered          *m_Remastered_Page;
        CTRXInfoPage            *m_Info_Page;
        CTRXGunPage             *m_Gun_Page;
        CTRXEquipmentPage       *m_Equipment_Page;
        CTRXItems               *m_Item_Page;
        CTRXItemsTR4            *m_ItemTR4_Page;
        CTRXAmmosPage           *m_Ammos_Page;

        int                     m_iStandardAdded;
        int                     m_iRemasteredAdded;
        bool                    m_bInitDone;
        char                    m_ParmPathname [ MAX_PATH ];

        //  For Subclassing
        CTRXButtonBase          m_Ok;
        CTRXButtonBase          m_Cancel;
        CTRXButtonBase          m_Help;
        CTRXButtonBase          m_ApplyNow;

        CTRXMenuBase            *m_pMenu;

        //  Handle Npn Client
        CTRXNCColor             m_NC;

    public:
        CTRXPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
        CTRXPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
        virtual ~CTRXPropertySheet();

        void AddStandardPage ();
        void AddRemasteredPage ();
        void RemoveStandardPage ();
        void RemoveRemasteredPage ();

        virtual void SetApply(BOOL bApply );

        void SetParmPathname ( const char *pathname );
        void SetTheActivePage ( PROPERTY_PAGE page );
        void DropToPage ( PROPERTY_PAGE page, const char *pFilename );

    // Attributes
    public:
        BOOL    m_bApplyActive;

    protected:
        HICON   m_hIcon;

    protected:
        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg void OnNcPaint();
        afx_msg BOOL OnNcActivate(BOOL bActive);
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
        afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseLeave();

        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


