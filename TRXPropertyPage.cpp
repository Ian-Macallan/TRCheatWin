// TRXPropertyPage.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXPropertyPage.h"
// #include "afxdialogex.h"

#include "TRXCHEATWIN.h"
#include "TRXDifferences.h"

//
BOOL        CTRXPropertyPage::m_bModified   = FALSE;

//
static const    char *MessageTitle = "Tombraider Standard Editions";

/////////////////////////////////////////////////////////////////////////////
// DialogBox CTRXPropertyPage
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertyPage, CTRXPropertyPageBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPage::CTRXPropertyPage( UINT code ) : CTRXPropertyPageBase(code)
{
    m_bModified         = FALSE;

    m_SetManualCombo    = true;
    m_SetManualList     = true;

    m_bFirstTime        = true;

    m_pBitmapWeapon1    = NULL;
    m_pBitmapWeapon2    = NULL;
    m_pBitmapWeapon3    = NULL;
    m_pBitmapWeapon4    = NULL;
    m_pBitmapWeapon5    = NULL;
    m_pBitmapWeapon6    = NULL;
    m_pBitmapWeapon7    = NULL;
    m_pBitmapWeapon8    = NULL;

    m_bInitDone         = false;

    m_pContextMenu      = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPage::~CTRXPropertyPage()
{
#define DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

    DELETE_OBJECT(m_pBitmapWeapon1);
    DELETE_OBJECT(m_pBitmapWeapon2);
    DELETE_OBJECT(m_pBitmapWeapon3);
    DELETE_OBJECT(m_pBitmapWeapon4);
    DELETE_OBJECT(m_pBitmapWeapon5);
    DELETE_OBJECT(m_pBitmapWeapon6);
    DELETE_OBJECT(m_pBitmapWeapon7);
    DELETE_OBJECT(m_pBitmapWeapon8);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPageBase::DoDataExchange(pDX);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertyPage, CTRXPropertyPageBase)
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// Message Handlers CTRXPropertyPage
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////
int CTRXPropertyPage::EnableForVersion()
{
    //  For Version n and below
    BOOL bModified = IsGUIModified();

    int iVersion        = 0;
    if ( CTRSaveGame::IsValid()  )
    {
        iVersion = CTRSaveGame::GetVersion ();
    }

    SetGUIModified ( bModified, "EnableForVersion" );

    return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::SetBitmap ( CStatic *wnd, int bitmapIndex, UINT resource )
{
    CBitmap **ppBitmap;
    switch ( bitmapIndex )
    {
        case 1 :
        {
            ppBitmap = &m_pBitmapWeapon1;
            break;
        }
        case 2 :
        {
            ppBitmap = &m_pBitmapWeapon2;
            break;
        }
        case 3 :
        {
            ppBitmap = &m_pBitmapWeapon3;
            break;
        }
        case 4 :
        {
            ppBitmap = &m_pBitmapWeapon4;
            break;
        }
        case 5 :
        {
            ppBitmap = &m_pBitmapWeapon5;
            break;
        }
        case 6 :
        {
            ppBitmap = &m_pBitmapWeapon6;
            break;
        }
        case 7 :
        {
            ppBitmap = &m_pBitmapWeapon7;
            break;
        }
        case 8 :
        {
            ppBitmap = &m_pBitmapWeapon8;
            break;
        }
        default  :
        {
            return;
        }
    }

    if ( *ppBitmap != NULL )
    {
        delete *ppBitmap;
    }
    *ppBitmap = new CBitmap;
    (*ppBitmap)->LoadBitmap ( resource );

    wnd->SetBitmap ( HBITMAP ( **ppBitmap ) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::OnSize(UINT nType, int cx, int cy)
{
    CTRXPropertyPageBase::OnSize(nType, cx, cy);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::OnMove(int x, int y)
{
    CTRXPropertyPageBase::OnMove(x, y);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::OnCancel()
{
    //

    CTRXPropertyPageBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::OnOK()
{
    //

    CTRXPropertyPageBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage::OnApply()
{
    return CTRXPropertyPageBase::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXPropertyPage::GetValue ( CEdit &edit )
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
int CTRXPropertyPage::GetValue ( CButton &button )
{
    return button.GetCheck();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage::PreTranslateMessage(MSG* pMsg)
{
    //
    m_ToolTip.RelayEvent(pMsg);

    return CTRXPropertyPageBase::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPage::OnInitDialog()
{
    CTRXPropertyPageBase::OnInitDialog();


    m_bInitDone = true;

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void  CTRXPropertyPage::SetGUIModified( BOOL bChanged, const char *pWhere )
{
    if ( CTRSaveGame::I(FALSE) == NULL )
    {
        m_bModified = FALSE;
        return;
    }

#if 0
#ifdef _DEBUG
    if (bChanged )
    {
        OutputDebugString ( pWhere );
        OutputDebugString ( " - SetGUIModified Changed to TRUE\n" );
    }
    else
    {
        OutputDebugString ( pWhere );
        OutputDebugString ( " - SetGUIModified Changed to FALSE\n" );
    }
#endif
#endif

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
BOOL CTRXPropertyPage::IsGUIModified()
{
    return m_bModified;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXPropertyPage::AskToSave( const char *pSubTitle, UINT mb )
{
    INT_PTR     iResponse = IDOK;
    static char szTitle [ MAX_PATH];
    sprintf_s ( szTitle, sizeof(szTitle), "%s (%s)", MessageTitle, pSubTitle );

    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        UpdateBuffer();
        iResponse = CTRXDifferences::MessageBoxS ( "Do you want to Save the File ?", szTitle, mb | MB_ICONQUESTION );
        if ( iResponse == IDYES )
        {
            CTRSaveGame::WriteSaveGame ();
        }
        else
        {
            CTRSaveGame::Discard();
        }
    }

    SetGUIModified ( FALSE );

    return iResponse;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::WriteWhenOnApply(const char *pSubTitle )
{
    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        UpdateBuffer();
        CTRSaveGame::WriteSaveGame ();
    }

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPage::SetApply(BOOL bApply )
 {
     m_bApplyActive = bApply;
 }

