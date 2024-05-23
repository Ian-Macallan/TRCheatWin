#pragma once

#include "TRXDialogBase.h"
#include "resource.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Bo�te de dialogue CTRXAllMaps
//
/////////////////////////////////////////////////////////////////////////////

class CTRXAllMaps : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXAllMaps)

    public:
        CTRXAllMaps(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXAllMaps();

    // Donn�es de bo�te de dialogue
        enum { IDD = IDD_TRXALLMAPS };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()
    public:
        CTRXCComboBoxBase m_Versions;
        CTRXCComboBoxBase m_Areas;
        CTRXButtonBase m_Show;
        virtual BOOL OnInitDialog();
        afx_msg void OnSelchangeVersions();
        afx_msg void OnSelchangeAreas();
        afx_msg void OnBnClickedShow();
};
