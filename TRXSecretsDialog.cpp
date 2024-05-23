// TRXSecretsDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXSecretsDialog.h"
#include "afxdialogex.h"


//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXSecretsDialog
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXSecretsDialog, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXSecretsDialog::CTRXSecretsDialog(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXSecretsDialog::IDD, pParent)
{

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXSecretsDialog::~CTRXSecretsDialog()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXSecretsDialog::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SECRETS, m_List_Secrets);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXSecretsDialog, CTRXDialogBase)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXSecretsDialog
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXSecretsDialog::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    // TODO: Add Code Here
    char szText [ 16 ];

    // m_List_Secrets.SetColorsNotUsed();

    m_List_Secrets.InsertColumn ( 0, "Level", LVCFMT_LEFT, 128);
    m_List_Secrets.InsertColumn ( 1, "Secrets", LVCFMT_RIGHT, 48);
    m_List_Secrets.InsertColumn ( 2, "Maximum", LVCFMT_RIGHT, 60);
    m_List_Secrets.InsertColumn ( 3, "State", LVCFMT_LEFT, 40);
    m_List_Secrets.InsertColumn ( 4, "Incremental", LVCFMT_LEFT, 80);
    m_List_Secrets.InsertColumn ( 5, "Completed", LVCFMT_RIGHT, 64);

    m_List_Secrets.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    BYTE secrets [ 64  ];
    ZeroMemory ( secrets, sizeof(secrets) );
    CTR9SaveGame::I()->GetBlocksSecrets ( m_iTombraider, m_iBlock, secrets );

    int totalAcquired = 0;
    int totalPossible = 0;

    for ( int iX = 0; iX < CTR9SaveGame::GetLevelCount(m_iTombraider); iX++ )
    {
        const char *pLevelName = CTR9SaveGame::GetLevelName ( m_iTombraider, iX + 1 );
        m_List_Secrets.InsertItem ( iX, pLevelName );

        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%d", secrets [ iX ] );
        totalAcquired += secrets [ iX ];
        m_List_Secrets.SetItemText ( iX, 1, szText );

        ZeroMemory ( szText, sizeof(szText) );
        int iMax = CTR9SaveGame::I()->GetSecretsForLevel ( m_iTombraider, iX );
        totalPossible += iMax;
        sprintf_s ( szText, sizeof(szText), "%d", iMax );
        m_List_Secrets.SetItemText ( iX, 2, szText );

        if ( iMax == secrets [ iX ] )
        {
            if ( iMax != 0 )
            {
                m_List_Secrets.SetItemText ( iX, 3, "OK" );
            }
            else
            {
                m_List_Secrets.SetItemText ( iX, 3, "-" );
            }
        }
        else
        {
            m_List_Secrets.SetItemText ( iX, 3, "" );
        }

        sprintf_s ( szText, sizeof(szText), "%2d/%2d", totalAcquired, totalPossible );
        m_List_Secrets.SetItemText ( iX, 4, szText );

        ZeroMemory ( szText, sizeof(szText) );
        if ( m_iTombraider == 1  || m_iTombraider == 2 || m_iTombraider == 3 )
        {
            int iEnabled = CTR9SaveGame::I()->GetSecretsCompleted ( m_iTombraider, m_iBlock, iX );
            sprintf_s ( szText, sizeof(szText), "%d", iEnabled );
        }
        else if ( m_iLevel == iX + 1 )
        {
            int iEnabled = CTR9SaveGame::I()->GetSecretsCompleted ( m_iTombraider, m_iBlock, iX );
            sprintf_s ( szText, sizeof(szText), "%d", iEnabled );
        }
        m_List_Secrets.SetItemText ( iX, 5, szText );
    }

    int count = m_List_Secrets.GetItemCount();
    m_List_Secrets.InsertItem ( count, "Total" );

    ZeroMemory ( szText, sizeof(szText) );
    sprintf_s ( szText, sizeof(szText), "%d", totalAcquired );
    m_List_Secrets.SetItemText ( count, 1, szText );

    ZeroMemory ( szText, sizeof(szText) );
    sprintf_s ( szText, sizeof(szText), "%d", totalPossible );
    m_List_Secrets.SetItemText ( count, 2, szText );

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}
