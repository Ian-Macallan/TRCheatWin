#pragma once

#include "resource.h"

#include "afxdlgs.h"

#include "TRXEditBase.h"
#include "TRXStaticBase.h"
#include "TRXButtonBase.h"
#include "TRXListCtrlBase.h"
#include "TRXSpinButtonCtrlBase.h"
#include "TRXCComboBoxBase.h"
#include "TRXCToolTipCtrlBase.h"
#include "TRXMenuBase.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
typedef struct  structLocation
{
    char    szPathname [ MAX_PATH ];
} STRUCTLOCATION;

#define LEN_LOCATION    128

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXPropertyPageBase
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertyPageBase : public CPropertyPage
{
    DECLARE_DYNAMIC(CTRXPropertyPageBase)

    public:
        CTRXPropertyPageBase(UINT code);
        virtual ~CTRXPropertyPageBase();

    // Données de boîte de dialogue
    protected :
        CTRXCToolTipCtrlBase    m_ToolTip;
        BOOL                    m_bToolTip;

        CFont                   *m_pBoldFont;
        CFont                   *m_pItalicFont;
        CFont                   *m_pNormalFont;
        CFont                   *m_pFixedFont;
        CFont                   *m_pFixedBoldFont;

        bool                    m_SetManualCombo;
        bool                    m_SetManualList;
        bool                    m_bInitDone;
        char                    m_ParmPathname [ MAX_PATH ];

        CTRXMenuBase            *m_pContextMenu;

    public :
        static BOOL AddLocation ( STRUCTLOCATION *pTable, const char *pLocation );
        static BOOL RemoveLocation ( STRUCTLOCATION *pTable, const char *pLocation );
        static int CountLocation ( STRUCTLOCATION *pTable );

        static void LoadLocation ( STRUCTLOCATION *pTable, const char *pCountKey, const char *pKeyFormat );
        static void SaveLocation ( STRUCTLOCATION *pTable, const char *pCountKey, const char *pKeyFormat );

        BOOL RemoveFromCombo ( CComboBox *pCombo, const char *pString );
        BOOL RemoveFromCombo ( CComboBox *pCombo, int sel );

        //
        void SetParmPathname ( const char *pathname );
        virtual void SetThemeChanged ( bool bDarkTheme );

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()
    public:
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

        virtual BOOL OnInitDialog();
};
