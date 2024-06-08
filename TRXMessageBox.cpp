// TRXMessageBox.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXMessageBox.h"
#include "afxdialogex.h"


//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXMessageBox
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXMessageBox, CTRXDialogBase)

BOOL CTRXMessageBox::bDontShowAgain = FALSE;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMessageBox::CTRXMessageBox(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXMessageBox::IDD, pParent)
{

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMessageBox::~CTRXMessageBox()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::SetMessage ( const char *pTitle, const char *pMessage, UINT icon )
{
    ZeroMemory ( m_szTitle, sizeof(m_szTitle) );
    ZeroMemory ( m_szMessage, sizeof(m_szMessage) );

    strcpy_s ( m_szTitle, sizeof(m_szTitle), pTitle );
    strcpy_s ( m_szMessage, sizeof(m_szMessage), pMessage );

    m_Icon  = icon;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT_PTR CTRXMessageBox::ShowMessage ( const char *pTitle, const char *pMessage, UINT icon )
{
    if ( ! bDontShowAgain )
    {
        CTRXMessageBox  dlg;
        dlg.SetMessage ( pTitle, pMessage, icon );

        return dlg.DoModal();
    }

    return IDCANCEL;
}
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEXT, m_Text);
    DDX_Control(pDX, IDC_DONT_SHOW, m_Dont_Show);
    DDX_Control(pDX, IDC_PICTURE, m_Picture);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXMessageBox, CTRXDialogBase)
    ON_BN_CLICKED(IDC_DONT_SHOW, &CTRXMessageBox::OnBnClickedDontShow)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXMessageBox
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMessageBox::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    //
    SetWindowText ( m_szTitle );
    m_Text.SetWindowText ( m_szMessage );

    switch ( m_Icon )
    {
        case MB_ICONHAND:
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_HAND);
            m_Picture.SetIcon(hIconQuestion);
            break;
        }

        case MB_ICONQUESTION :
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_QUESTION);
            m_Picture.SetIcon(hIconQuestion);
            break;
        }


        case MB_ICONEXCLAMATION :
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION);
            m_Picture.SetIcon(hIconQuestion);
            break;
        }


        case MB_ICONASTERISK :
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_ASTERISK);
            m_Picture.SetIcon(hIconQuestion);
            break;
        }

        default :
        {
            m_Picture.ShowWindow ( SW_HIDE );
            break;
        }
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMessageBox::PreTranslateMessage(MSG* pMsg)
{
    //

    return CTRXDialogBase::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::OnBnClickedDontShow()
{
    if ( m_Dont_Show.GetCheck() )
    {
        bDontShowAgain  = TRUE;
    }
    else
    {
        bDontShowAgain  = FALSE;
    }
}
