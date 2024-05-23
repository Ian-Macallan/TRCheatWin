#pragma once

#include "resource.h"
#include "TR9SaveGame.h"
#include "afxwin.h"
#include "TRXDialogBase.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXRemasteredInfos
//
/////////////////////////////////////////////////////////////////////////////
class CTRXRemasteredInfos : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXRemasteredInfos)

    public:
        CTRXRemasteredInfos(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXRemasteredInfos();

    // Données de boîte de dialogue
        enum { IDD = IDD_REMASTERED_INFO };

    public:
        int             m_iTombraider;
        int             m_iBlock;
        int             m_iSlot;
        int             m_iLevel;

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()
    public:
        virtual BOOL OnInitDialog();
        CTRXEditBase m_Text;
        CTRXButtonBase m_OK;
        CTRXButtonBase m_Cancel;
};
