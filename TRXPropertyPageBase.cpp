// TRXPropertyPageBase.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXPropertyPageBase.h"
#include "TRXCHEATWIN.h"
#include "afxdialogex.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXPropertyPageBase
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertyPageBase, CPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPageBase::CTRXPropertyPageBase(UINT code) : CPropertyPage(code)
{
    m_bToolTip          = FALSE;

    m_pNormalFont       = NULL;
    m_pItalicFont       = NULL;
    m_pBoldFont         = NULL;
    m_pFixedFont        = NULL;
    m_pFixedBoldFont    = NULL;

    m_SetManualCombo    = true;
    m_SetManualList     = true;
    m_bInitDone         = false;

    ZeroMemory ( m_ParmPathname, sizeof(m_ParmPathname) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertyPageBase::~CTRXPropertyPageBase()
{
#define DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

    //  Do Not Delete m_pNormalFont

    DELETE_OBJECT(m_pItalicFont)
    DELETE_OBJECT(m_pBoldFont)
    DELETE_OBJECT(m_pFixedFont)
    DELETE_OBJECT(m_pFixedBoldFont)

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::SetParmPathname ( const char *pathname )
{
    if ( pathname != NULL )
    {
        strcpy_s ( m_ParmPathname, sizeof(m_ParmPathname), pathname );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertyPageBase, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_MEASUREITEM()
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXPropertyPageBase
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::AddLocation ( STRUCTLOCATION *pTable, const char *pLocation )
{
    if ( pTable == NULL )
    {
        return FALSE;
    }

    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        //  Already Here
        if ( _stricmp ( pTable [ i ].szPathname, pLocation ) == 0 )
        {
            return FALSE;
        }

        //  Empty Slot
        if ( strlen(pTable [ i ].szPathname) == 0 )
        {
            strcpy_s ( pTable [ i ].szPathname, MAX_PATH, pLocation );
            return TRUE;
        }
    }

    // Move Up
    for ( int i = 1; i < LEN_LOCATION; i++ )
    {
        strcpy_s ( pTable [ i - 1 ].szPathname, MAX_PATH, pTable [ i ].szPathname );
    }

    //  Add at End
    strcpy_s ( pTable [ LEN_LOCATION - 1 ].szPathname, MAX_PATH, pLocation );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::RemoveLocation ( STRUCTLOCATION *pTable, const char *pLocation )
{
    if ( pTable == NULL )
    {
        return FALSE;
    }

    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        //  Here
        if ( _stricmp ( pTable [ i ].szPathname, pLocation ) == 0 )
        {
            // Move Up
            for ( int j = i + 1; j < LEN_LOCATION; j++ )
            {
                strcpy_s ( pTable [ j - 1 ].szPathname, MAX_PATH, pTable [ j ].szPathname );
            }

            strcpy_s ( pTable [ LEN_LOCATION - 1 ].szPathname, MAX_PATH, "" );

            // Return
            return TRUE;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::RemoveFromCombo ( CComboBox *pCombo, const char *pTString )
{
    static char szText [ MAX_PATH ];

    //
    int iCurSel = pCombo->GetCurSel();
    for ( int i = pCombo->GetCount() - 1; i >= 0; i-- )
    {
        ZeroMemory ( szText, sizeof(szText) );
        pCombo->GetLBText( i, szText );
        if ( _stricmp ( szText, pTString ) == 0 )
        {
            pCombo->DeleteString ( i );
        }
    }

    if ( iCurSel >= 0 && iCurSel < pCombo->GetCount() )
    {
        pCombo->SetCurSel ( iCurSel );
    }
    else if ( iCurSel - 1 >= 0 && iCurSel - 1 < pCombo->GetCount() )
    {
        pCombo->SetCurSel ( iCurSel - 1 );
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::RemoveFromCombo ( CComboBox *pCombo, int sel )
{
    int iCurSel = pCombo->GetCurSel();

    if ( sel >= 0 && sel < pCombo->GetCount() )
    {
        pCombo->DeleteString ( sel );
    }

    if ( iCurSel >= 0 && iCurSel < pCombo->GetCount() )
    {
        pCombo->SetCurSel ( iCurSel );
    }
    else if ( iCurSel - 1 >= 0 && iCurSel - 1 < pCombo->GetCount() )
    {
        pCombo->SetCurSel ( iCurSel - 1 );
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXPropertyPageBase::CountLocation ( STRUCTLOCATION *pTable )
{
    int iCount = 0;

    //
    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        if ( strlen ( pTable [ i ].szPathname ) > 0 )
        {
            iCount++;
        }
    }

    return iCount;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::LoadLocation ( STRUCTLOCATION *pTable, const char *pCountKey, const char *pKeyFormat )
{
    char szKeyname [ 64 ];
    sprintf_s ( szKeyname, sizeof(szKeyname), pCountKey );
    int iCount = theApp.GetProfileInt( PROFILE_SETTING, szKeyname, 0 );

    for ( int i = 0; i < LEN_LOCATION && i < iCount; i++ )
    {
        char szTemp [ MAX_PATH ];
        sprintf_s ( szKeyname, sizeof(szKeyname), pKeyFormat, i );
        CString location  = theApp.GetProfileString( PROFILE_SETTING, szKeyname, "" );
        strcpy_s  ( szTemp, sizeof(szTemp), location );
        if ( strlen(szTemp) > 0 )
        {
            AddLocation ( pTable, szTemp );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::SaveLocation ( STRUCTLOCATION *pTable, const char *pCountKey, const char *pKeyFormat )
{
    char szKeyname [ 64 ];

    //
    int iCount = 0;
    for ( int i = 0; i < LEN_LOCATION; i++ )
    {
        if ( strlen(pTable [ i ].szPathname) > 0 )
        {
            sprintf_s ( szKeyname, sizeof(szKeyname), pKeyFormat, iCount );
            theApp.WriteProfileString( PROFILE_SETTING, szKeyname, pTable [ i ].szPathname );
            iCount++;
        }
        else
        {
            //  Delete Key
            sprintf_s ( szKeyname, sizeof(szKeyname), pKeyFormat, iCount );
            theApp.WriteProfileString( PROFILE_SETTING, szKeyname, NULL );
        }
    }

    sprintf_s ( szKeyname, sizeof(szKeyname), pCountKey );
    theApp.WriteProfileInt( PROFILE_SETTING, szKeyname, iCount );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXPropertyPageBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Add Code Here
    HBRUSH hBrush = CTRXTools::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO: Add Code Here
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add Code Here
    if ( CTRXTools::OnEraseBkgnd(pDC, this) )
    {
        return TRUE;
    }
    return CPropertyPage::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::SetThemeChanged(bool bDarkTheme )
{
    if ( m_bToolTip )
    {
        m_ToolTip.SetColors ( );
    }

    if ( m_bInitDone )
    {
        Invalidate();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertyPageBase::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO: Add Code Here
    //
    TEXTMETRIC textMetrics;
    CDC *pDC = GetDC();
    GetTextMetrics ( pDC->m_hDC, &textMetrics );
    ReleaseDC ( pDC );

    //
    //  Create Fonts
    LOGFONT logDefaultFont;
    LOGFONT logFont;

    m_pNormalFont               = GetFont();
    m_pNormalFont->GetLogFont(&logDefaultFont);

    logFont                     = logDefaultFont;
    logFont.lfWeight            = FW_BOLD;
    m_pBoldFont                 = new CFont();
    m_pBoldFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    logFont.lfItalic            = TRUE;
    m_pItalicFont               = new CFont();
    m_pItalicFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    m_pFixedFont                = new CFont();
    logFont.lfPitchAndFamily    = FIXED_PITCH;
    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
    m_pFixedFont->CreateFontIndirect(&logFont);    // Create the font.

    logFont                     = logDefaultFont;
    m_pFixedBoldFont            = new CFont();
    logFont.lfPitchAndFamily    = FIXED_PITCH;
    logFont.lfWeight            = FW_BOLD;
    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
    m_pFixedBoldFont->CreateFontIndirect(&logFont);    // Create the font.

    // TODO: Add Code Here
    if( ! m_ToolTip.Create(this))
    {
       TRACE0("Unable to create the ToolTip!");
    }
    else
    {
        m_bToolTip  = TRUE;
        m_ToolTip.SetMaxTipWidth ( 392 );
        m_ToolTip.SetFont ( m_pFixedBoldFont );
        m_ToolTip.SetColors ( );
    }

    //
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // TODO: Add Code Here
    if ( lpMeasureItemStruct != NULL )
    {
        if ( lpMeasureItemStruct->CtlType == ODT_MENU )
        {
            if ( m_pContextMenu != NULL )
            {
                m_pContextMenu->MeasureItem(lpMeasureItemStruct);
                return;
            }
        }
    }

    //
    CPropertyPage::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertyPageBase::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add Code Here
    if ( lpDrawItemStruct != NULL )
    {
        if ( lpDrawItemStruct->CtlType == ODT_MENU )
        {
            if ( m_pContextMenu != NULL )
            {
                m_pContextMenu->DrawItem(lpDrawItemStruct);
                return;
            }
        }
    }

    CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
