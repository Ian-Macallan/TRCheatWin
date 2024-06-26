// TRXPropertyPage.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
#include "TRXPropertyPage123.h"
// #include "afxdialogex.h"

#include "TRXCHEATWIN.h"
#include "TR9SaveGame.h"

//
/////////////////////////////////////////////////////////////////////////////
// DialogBox CTRXPropertyPage123
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertyPage123, CTRXPropertyPageBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPage123::CTRXPropertyPage123( UINT code ) : CTRXPropertyPageBase(code)
{
    m_bModified         = FALSE;
    m_bApplyActive      = FALSE;

    m_SetManualCombo    = true;
    m_SetManualList     = true;

    m_pContextMenu      = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPage123::~CTRXPropertyPage123()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPageBase::DoDataExchange(pDX);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertyPage123, CTRXPropertyPageBase)
    ON_WM_SIZE()
    ON_WM_MOVE()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// Message Handlers CTRXPropertyPage123
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::OnSize(UINT nType, int cx, int cy)
{
    CTRXPropertyPageBase::OnSize(nType, cx, cy);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::OnMove(int x, int y)
{
    CTRXPropertyPageBase::OnMove(x, y);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::OnCancel()
{
    //

    CTRXPropertyPageBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::OnOK()
{
    //

    CTRXPropertyPageBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXPropertyPage123::GetValue ( CEdit &edit )
{
    //
    char szString [ 32 ];
    ZeroMemory ( szString, sizeof(szString) );
    edit.GetWindowText ( szString, sizeof(szString) );
    int result = 0;
    if ( strncmp ( szString, "0x", 2 ) == 0 )
    {
        sscanf_s ( szString + 2, "%x", &result );
    }
    else
    {
        result = atoi(szString);
    }

    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXPropertyPage123::GetValue ( CButton &button )
{
    return button.GetCheck();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage123::PreTranslateMessage(MSG* pMsg)
{
    //
    m_ToolTip.RelayEvent(pMsg);

    return CTRXPropertyPageBase::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage123::OnInitDialog()
{
    CTRXPropertyPageBase::OnInitDialog();

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void  CTRXPropertyPage123::SetGUIModified( BOOL bChanged )
{
    if ( CTR9SaveGame::I(FALSE) == NULL )
    {
        m_bModified = FALSE;
        return;
    }

    m_bModified = bChanged;
    if ( m_bApplyActive )
    {
        CTRXPropertyPageBase::SetModified ( bChanged );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage123::IsGUIModified()
{
    return m_bModified;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage123::SetApply(BOOL bApply )
{
     m_bApplyActive = bApply;
}

