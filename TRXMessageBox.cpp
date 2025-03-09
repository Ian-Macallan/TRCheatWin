// TRXMessageBox.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "AutomaticVersionHeader.h"

// #include "afxdialogex.h"
#include "TRXMessageBox.h"


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
    m_Result = -1;
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
UINT_PTR CTRXMessageBox::MessageBox ( const char *pMessage, const char *pCaption, UINT icon )
{
    if ( ! bDontShowAgain )
    {
        CTRXMessageBox  dlg;
        if ( pCaption == NULL )
        {
            pCaption = PROGRAM_NAME;
        }
        dlg.SetMessage ( pCaption, pMessage, icon );

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
    DDX_Control(pDX, IDYES, m_Yes);
    DDX_Control(pDX, IDNO, m_No);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXMessageBox, CTRXDialogBase)
    ON_BN_CLICKED(IDC_DONT_SHOW, &CTRXMessageBox::OnBnClickedDontShow)
    ON_BN_CLICKED(IDYES, &CTRXMessageBox::OnBnClickedYes)
    ON_BN_CLICKED(IDNO, &CTRXMessageBox::OnBnClickedNo)
    ON_BN_CLICKED(IDOK, &CTRXMessageBox::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CTRXMessageBox::OnBnClickedCancel)
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

    m_Yes.ShowWindow ( SW_HIDE );
    m_No.ShowWindow ( SW_HIDE );
    m_OK.ShowWindow ( SW_HIDE );
    m_Cancel.ShowWindow ( SW_HIDE );

    switch ( m_Icon & 0x0f )
    {
        case MB_OK:
        {
            m_OK.ShowWindow ( SW_NORMAL );
            m_OK.SetFocus();
            break;
        }

        case MB_OKCANCEL:
        {
            m_OK.ShowWindow ( SW_NORMAL );
            m_Cancel.ShowWindow ( SW_NORMAL );
            m_OK.SetFocus();
            break;
        }

        case MB_YESNO:
        {
            m_Yes.ShowWindow ( SW_NORMAL );
            m_No.ShowWindow ( SW_NORMAL );
            m_Yes.SetFocus();
            break;
        }

        case MB_YESNOCANCEL:
        {
            m_Yes.ShowWindow ( SW_NORMAL );
            m_No.ShowWindow ( SW_NORMAL );
            m_Cancel.ShowWindow ( SW_NORMAL );
            m_Yes.SetFocus();
            break;
        }

        default:
        {
            m_OK.ShowWindow ( SW_NORMAL );
            m_OK.SetFocus();
            break;
        }
    }

    switch ( m_Icon & 0xf0 )
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

    return FALSE;  // return TRUE unless you set the focus to a control
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

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXMessageBox::DoModal()
{
    // Add Handler Here

    INT_PTR result = CTRXDialogBase::DoModal();
    if ( m_Result != -1 )
    {
        return m_Result;
    }
    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::OnBnClickedYes()
{
    if ( m_Result == -1 )
    {
        m_Result    = IDYES;
    }

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::OnBnClickedNo()
{
    if ( m_Result == -1 )
    {
        m_Result    = IDNO;
    }

    CTRXDialogBase::OnCancel();
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::OnBnClickedOk()
{
    if ( m_Result == -1 )
    {
        m_Result    = IDOK;
    }

    CTRXDialogBase::OnOK();
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMessageBox::OnBnClickedCancel()
{
    if ( m_Result == -1 )
    {
        m_Result    = IDCANCEL;
    }

    CTRXDialogBase::OnCancel();
}
