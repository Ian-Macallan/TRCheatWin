// TRXRemasteredInfos.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
// #include "afxdialogex.h"
#include "TRXRemasteredInfos.h"


//
/////////////////////////////////////////////////////////////////////////////
// Bo�te de dialogue CTRXRemasteredInfos
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTRXRemasteredInfos, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemasteredInfos::CTRXRemasteredInfos(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXRemasteredInfos::IDD, pParent)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRemasteredInfos::~CTRXRemasteredInfos()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRemasteredInfos::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEXT, m_Text);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXRemasteredInfos, CTRXDialogBase)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXRemasteredInfos
//
/////////////////////////////////////////////////////////////////////////////
//                      
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRemasteredInfos::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    //
    //
    m_Text.SetFont ( m_pFixedFont );
    if ( m_iTombraider >= 1 && m_iTombraider <= 3 )
    {
        m_Text.SetWindowText ( CTR9SaveGame::I()->GetInterest(m_iTombraider, m_iBlock, m_iSlot ) );
    }
    else if ( m_iTombraider >= 4 && m_iTombraider <= 5 )
    {
        m_Text.SetWindowText ( CTR8SaveGame::I()->GetInterest(m_iTombraider, m_iBlock ) );
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}
