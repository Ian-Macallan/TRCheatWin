#pragma once

#include <Windows.h>
#include "resource.h"
#include "TRXPropertyPageBase.h"
#include "TRXHelpDialog.h"
#include "TRXMenuBase.h"

//
/////////////////////////////////////////////////////////////////////////////
// DialogBox CTRXPropertyPage
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPropertyPage456 : public CTRXPropertyPageBase
{
        DECLARE_DYNAMIC(CTRXPropertyPage456)

    public:
        CTRXPropertyPage456(UINT code);
        virtual ~CTRXPropertyPage456();

    // Données de boîte de dialogue

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    // Attributes
    public:
        BOOL    m_bApplyActive;

    protected :
        BOOL                    m_bModified;

        bool                    m_SetManualCombo;
        bool                    m_SetManualList;

    public:
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnMove(int x, int y);
        afx_msg void OnTimer(UINT_PTR nIDEvent);

        virtual void OnCancel();
        virtual void OnOK();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual BOOL OnInitDialog();


        virtual int GetValue ( CEdit &edit );
        virtual BOOL GetValue ( CButton &button );

        virtual void SetApply(BOOL bApply );
        void SetGUIModified( BOOL bChanged = TRUE );
        BOOL IsGUIModified();
       

};


