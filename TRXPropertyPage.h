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
class CTRXPropertyPage : public CTRXPropertyPageBase
{
        DECLARE_DYNAMIC(CTRXPropertyPage)

    public:
        CTRXPropertyPage(UINT code);
        virtual ~CTRXPropertyPage();

        int EnableForVersion();

    // Données de boîte de dialogue

    // Attributes
    public:
        BOOL    m_bApplyActive;

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    protected :
        CBitmap *m_pBitmapWeapon1;
        CBitmap *m_pBitmapWeapon2;
        CBitmap *m_pBitmapWeapon3;
        CBitmap *m_pBitmapWeapon4;
        CBitmap *m_pBitmapWeapon5;
        CBitmap *m_pBitmapWeapon6;
        CBitmap *m_pBitmapWeapon7;
        CBitmap *m_pBitmapWeapon8;
        CBitmap *m_pBitmapWeapon9;

    protected :
        static BOOL             m_bModified;
        bool                    m_bFirstTime;

        int                     m_iHitCountForAll;

        void SetBitmap ( CStatic *wnd, int bitmapIndex, UINT resource );

    public:
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnMove(int x, int y);
        virtual void OnCancel();
        virtual void OnOK();
        virtual BOOL OnApply();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual BOOL OnInitDialog();

        virtual int GetValue ( CEdit &edit );
        virtual int GetValue ( CStatic &stat );
        virtual int GetValue ( CButton &button );

        void SetGUIModified( BOOL bChanged = TRUE, const char *pWhere = "" );
        BOOL IsGUIModified();

        //
        virtual void UpdateBuffer() = 0;
        virtual void SetApply(BOOL bApply );
        virtual BOOL OnSetActive();

        INT_PTR AskToSave(const char *pSubTitle, UINT mb = MB_YESNO );
        void WriteWhenOnApply(const char *pSubTitle);
};
