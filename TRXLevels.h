#pragma once

#include "resource.h"

#include "TRXDialogBase.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "TRXButtonBase.h"
#include "TRXListCtrlBase.h"

// Boîte de dialogue CTRXLevels

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXLevels : public CTRXDialogBase
{
	    DECLARE_DYNAMIC(CTRXLevels)

    public:
	    CTRXLevels(CWnd* pParent = NULL);   // constructeur standard
	    virtual ~CTRXLevels();

    // Données de boîte de dialogue
	    enum { IDD = IDD_TRXLEVELS };

    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	    DECLARE_MESSAGE_MAP()

    private :
        CTRXListCtrlBase    *m_pListCtrl;

    public:
        CTRXButtonBase      m_OK;
        CTRXButtonBase      m_Cancel;
        CTRXListCtrlBase    m_LevelList;

        int                 m_iVersion;
        BOOL                m_bRemastered;
        char                m_szSaveName [ MAX_PATH ];

    public :
        virtual BOOL OnInitDialog();
};
