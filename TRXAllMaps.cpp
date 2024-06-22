// TRXAllMaps.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"

#include "TRXAllMaps.h"
#include "TR_Areas.h"
#include "TRXMapAreas.h"
#include "TR9SaveGame.h"

// Boîte de dialogue CTRXAllMaps

IMPLEMENT_DYNAMIC(CTRXAllMaps, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXAllMaps::CTRXAllMaps(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXAllMaps::IDD, pParent)
{

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXAllMaps::~CTRXAllMaps()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAllMaps::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_VERSIONS, m_Versions);
    DDX_Control(pDX, IDC_AREAS, m_Areas);
    DDX_Control(pDX, IDC_SHOW, m_Show);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXAllMaps, CTRXDialogBase)
    ON_CBN_SELCHANGE(IDC_VERSIONS, &CTRXAllMaps::OnSelchangeVersions)
    ON_CBN_SELCHANGE(IDC_AREAS, &CTRXAllMaps::OnSelchangeAreas)
    ON_BN_CLICKED(IDC_SHOW, &CTRXAllMaps::OnBnClickedShow)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXAllMaps
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXAllMaps::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    //
    m_Versions.ResetContent();
    m_Versions.AddString ( "01: Remastered Tombraider 1 and Unfinished Business" );
    m_Versions.AddString ( "02: Remastered Tombraider 2 and The Golden Mask" );
    m_Versions.AddString ( "03: Remastered Tombraider 3 and The Lost Artefact" );
    m_Versions.AddString ( "10: Standard Tombraider 1 and Unfinished Business" );
    m_Versions.AddString ( "20: Standard Tombraider 2 and The Golden Mask" );
    m_Versions.AddString ( "30: Standard Tombraider 3 and The Lost Artefact" );
    m_Versions.AddString ( "40: Standard Tombraider 4 Last Revelation and The Times Exclusive" );
    m_Versions.AddString ( "50: Standard Tombraider 5 Chronicles" );
    if ( IsCustomArea() && GetTRTableSize(99) > 0 )
    {
        m_Versions.AddString ( "99: Custom Level" );
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAllMaps::OnSelchangeVersions()
{
    //
    static char szText [ MAX_PATH ];

    int iCurSel = m_Versions.GetCurSel();
    if ( iCurSel >= 0 && iCurSel <  m_Versions.GetCount() )
    {
        m_Versions.GetLBText( iCurSel, szText );
        int tombraider = atoi(szText);
        int iSize = GetTRTableSize(tombraider);

        m_Areas.ResetContent();
        for ( int i = 0; i < iSize; i++ )
        {
            const char *pText = GetAreaLevelName( tombraider, i );
            if ( pText != NULL && strlen(pText) > 0 )
            {
                const char *pLevelName = CTR9SaveGame::GetLevelName ( tombraider, i + 1 );
                sprintf_s ( szText, sizeof(szText), "%03d : %s - %s", i, pText, pLevelName );
                m_Areas.AddString( szText );
            }
        }
    }
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAllMaps::OnSelchangeAreas()
{
    //
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXAllMaps::OnBnClickedShow()
{
    static char szText [ MAX_PATH ];

    int iCurSel = m_Versions.GetCurSel();
    if ( iCurSel >= 0 && iCurSel <  m_Versions.GetCount() )
    {
        m_Versions.GetLBText( iCurSel, szText );
        int tombraider = atoi(szText);

        iCurSel = m_Areas.GetCurSel();
        if ( iCurSel >= 0 && iCurSel <  m_Areas.GetCount() )
        {
            m_Areas.GetLBText( iCurSel, szText );
            int levelIndex = atoi(szText);

            CTRXMapAreas dlg;
            dlg.SetTombraiderLevelIndex ( tombraider, levelIndex );

            dlg.DoModal();
        }
    }
}
