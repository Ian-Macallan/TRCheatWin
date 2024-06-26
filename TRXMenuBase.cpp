#include "stdafx.h"
#include "TRXMenuBase.h"
#include "TRXTools.h"
#include "TRXColors.h"
#include "TRXGlobal.h"
#include "resource.h"
#include "TRXGDI.h"
#include "TRXCHEATWIN.h"

static  const int EXTRA_PIXELS_WIDTH    = 8;
static  const int EXTRA_PIXELS_HEIGHT   = 6;
static  const int SEPARATOR_HEIGHT      = 3;

#define MENU_TEXT_SIZE  256

//
extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXMenuBase, CMenu)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMenuBase::CTRXMenuBase(void)
{
    m_pWnd              = NULL;
    m_pSubMenu          = NULL;
    m_hCheckWhiteIcon   = AfxGetApp()->LoadIcon(IDI_CHECK_WHITE);
    m_hCheckBlackIcon   = AfxGetApp()->LoadIcon(IDI_CHECK_BLACK);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMenuBase::~CTRXMenuBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::CreateMenu()
{
    return CMenu::CreateMenu();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::CreatePopupMenu()
{
    return CMenu::CreatePopupMenu();
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::LoadMenu(LPCTSTR lpszResourceName)
{
    return CMenu::LoadMenu(lpszResourceName);
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::LoadMenu(UINT nIDResource)
{
    return CMenu::LoadMenu(nIDResource);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMenuBase *CTRXMenuBase::SetSystemMenu(CWnd* pWnd, CMenu* pSysMenu)
{
    m_pWnd = pWnd;

    //
    if ( pSysMenu != NULL )
    {
        m_pSubMenu = pSysMenu;
        SetOwnDraw ( m_pSubMenu->m_hMenu );
    }

    return this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::UnSetSystemMenu(CMenu* pSysMenu)
{
    //
    if ( pSysMenu != NULL )
    {
        SetOwnDraw ( pSysMenu->m_hMenu, false );
    }
}
//
/////////////////////////////////////////////////////////////////////////////
//  The hMenu must be the handle of the submenu
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::SetOwnDraw ( HMENU hMenu, bool bOwnDrawn )
{
    //  MFT_OWNERDRAW
    //
    //      Retrieve the Menu
    CMenu   *menu = CMenu::FromHandle ( hMenu );

    //
    //      If no handle : return
    if ( menu == NULL )
    {
        return;
    }

    //
    //      If menu is not correct: return
    if ( menu->m_hMenu == NULL )
    {
        return;
    }

    for ( UINT iPos = 0; iPos < (UINT) menu->GetMenuItemCount ( ); iPos++ )
    {
        char            szText [ MENU_TEXT_SIZE ];
        ZeroMemory ( szText, sizeof(szText) );

        MENUITEMINFO    menuItemInfo;
        ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
        menuItemInfo.cbSize     = sizeof ( menuItemInfo );

        menuItemInfo.cch        = sizeof ( szText );
        menuItemInfo.dwTypeData = szText;
        menuItemInfo.fMask      =   MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA |
                                    MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING |
                                    MIIM_SUBMENU;
        BOOL bRes = menu->GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
        if ( bRes )
        {
            menuItemInfo.fMask          = MIIM_FTYPE | MIIM_DATA;
            if ( bOwnDrawn )
            {
                menuItemInfo.fType          |= MFT_OWNERDRAW;
            }
            else if ( menuItemInfo.fType & MFT_OWNERDRAW )
            {
                menuItemInfo.fType          ^= MFT_OWNERDRAW;
            }
            //  Set Index in dwItemData
            menuItemInfo.dwItemData     = iPos;
            menuItemInfo.cch            = ( UINT ) strlen ( szText );
            bRes = menu->SetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
            if ( ! bRes )
            {
                DWORD dwError = GetLastError();
                int i = 0;
            }
            if ( menuItemInfo.hSubMenu )
            {
                SetOwnDraw ( menuItemInfo.hSubMenu, bOwnDrawn );
            }
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::SetOwnDraw ( CMenu *pMenu, bool bOwnDrawn )
{
    if ( pMenu )
    {
        SetOwnDraw ( pMenu->GetSafeHmenu(), bOwnDrawn );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
CTRXMenuBase *CTRXMenuBase::GetSubMenu ( int nPos )
{
    m_pSubMenu  = CMenu::GetSubMenu( nPos );
    return this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    if ( m_pSubMenu != NULL )
    {
        DrawMenuItem( lpDrawItemStruct );
    }
    else
    {
        CMenu::DrawItem(lpDrawItemStruct);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
    if ( m_pSubMenu != NULL )
    {
        MeasureMenuItem(lpMeasureItemStruct);
    }
    else
    {
        CMenu::MeasureItem(lpMeasureItemStruct);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect )
{
    m_pWnd = pWnd;

    if ( m_pSubMenu != NULL )
    {
        if ( CTRXColors::m_iDarkTheme != 0 )
        {
            SetOwnDraw(m_pSubMenu->m_hMenu);
        }
        return  m_pSubMenu->TrackPopupMenu( nFlags,  x,  y, pWnd, lpRect );
    }
    else
    {
        if ( CTRXColors::m_iDarkTheme != 0 )
        {
            SetOwnDraw(m_hMenu);
        }
        return  CMenu::TrackPopupMenu( nFlags,  x,  y, pWnd, lpRect );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXMenuBase::GetMenuItemCount ( )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->GetMenuItemCount ( );
    }
    else
    {
        return CMenu::GetMenuItemCount ( );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::GetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->GetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
    else
    {
        return CMenu::GetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMenuBase::SetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->SetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
    else
    {
        return CMenu::SetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
UINT CTRXMenuBase::EnableMenuItem ( UINT nIDEnableItem, UINT nEnable )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->EnableMenuItem ( nIDEnableItem, nEnable );
    }
    else
    {
        return CMenu::EnableMenuItem ( nIDEnableItem, nEnable );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::MeasureMenuItem ( CDC *pDC, const char *pText, CSize *pSize )
{
    //
    //      Initial Size
    int xCheckIcon  = GetSystemMetrics(SM_CXMENUCHECK);
    int yCheckIcon  = GetSystemMetrics(SM_CYMENUCHECK);

    pSize->cx = xCheckIcon + EXTRA_PIXELS_WIDTH;

    //
    if ( pText != NULL && strlen ( pText ) > 0 )
    {
        pSize->cy = yCheckIcon + EXTRA_PIXELS_HEIGHT;

        CFont* pOldFont     = ( CFont * ) pDC->SelectStockObject( ANSI_VAR_FONT );
        INT tabStop         = ( INT ) strlen ( pText );
        CSize sizeText      = pDC->GetOutputTabbedTextExtent ( pText, -1, 1, &tabStop );
        pSize->cx           += sizeText.cx + xCheckIcon;
        if ( sizeText.cy > pSize->cy )
        {
            pSize->cy = sizeText.cy;
        }

        pDC->SelectObject ( pOldFont );
    }
    else
    {
        if ( pSize->cy < SEPARATOR_HEIGHT )
        {
            pSize->cy       = SEPARATOR_HEIGHT;
        }

        pSize->cx           += (EXTRA_PIXELS_WIDTH * 2 );
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::MeasureMenuItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
    //
    if ( lpMeasureItemStruct == NULL )
    {
        return;
    }

    //
    //      The Size
    CSize               size (0,0);

    //
    //      Get Info
    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );

    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
    menuItemInfo.cbSize  = sizeof (menuItemInfo);

    menuItemInfo.fMask          = MIIM_STRING;
    menuItemInfo.cch            = sizeof ( szText );
    menuItemInfo.dwTypeData     = szText;
    //  itemData contains the position / index
    UINT iPos   = ( UINT ) lpMeasureItemStruct->itemData;
    //  We could search by itemID
    UINT id     = lpMeasureItemStruct->itemID;

    //
    BOOL bRes = GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
    if ( bRes )
    {
        if ( m_pWnd )
        {
            CDC *pDC    = m_pWnd->GetDC ( );
            if ( pDC )
            {
                strcat_s  ( szText, sizeof(szText), "ALT+F99" );
                MeasureMenuItem ( pDC, szText, &size );
                if ( strlen(szText) > 0 )
                {
                    size.cx     = size.cx + 1;
                    size.cy     = size.cy + 1;
                }
                int iRes    = m_pWnd->ReleaseDC ( pDC );
            }
        }
    }

    lpMeasureItemStruct->itemWidth      = size.cx;
    lpMeasureItemStruct->itemHeight     = size.cy;

}

//
//====================================================================================
//      Draw an Item
//====================================================================================
void CTRXMenuBase::DrawMenuItem (   LPDRAWITEMSTRUCT lpDrawItemStruct, CDC *pDC,
                                    CRect *pRect, const char *pText )
{
    int xCheckIcon  = GetSystemMetrics(SM_CXMENUCHECK);
    int yCheckIcon  = GetSystemMetrics(SM_CYMENUCHECK);

    const   int     TextAttrLeft    =   DT_EXPANDTABS | DT_LEFT | DT_VCENTER | DT_SINGLELINE;
    const   int     TextAttrRight   =   DT_EXPANDTABS | DT_RIGHT | DT_VCENTER | DT_SINGLELINE;

    COLORREF        colorFg         = 0;
    int             bkMode          = 0;

    HBRUSH      hOldBrush       = NULL;
    BOOL        bDone           = FALSE;

    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );


    //  The entire control needs to be drawn
    //  This is called line by line
    if ( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
    {
        bDone = pDC->DrawFrameControl ( &lpDrawItemStruct->rcItem, DFC_POPUPMENU, 0 );
        RECT rect = *pRect;
        rect.top        = rect.top + 1;
        rect.left       = rect.left + 1;
        rect.right      = rect.right  - 1;
        rect.bottom     = rect.bottom  - 1;
        pDC->FillRect ( pRect, CTRXColors::GetBKMenuCBrush(CTRXColors::m_iDarkTheme != 0));
    }

    //
    if ( lpDrawItemStruct->itemState & ODS_CHECKED )
    {
        // We Will Have to Draw a Bitmap
        hOldBrush       = ( HBRUSH ) pDC->SelectObject ( CTRXColors::GetFGMenuBrush(CTRXColors::m_iDarkTheme != 0) );
        int xIconSmall  = GetSystemMetrics(SM_CXSMICON);
        int yIconSmall  = GetSystemMetrics(SM_CYSMICON);

        if ( CTRXColors::m_iDarkTheme != 0 )
        {
            //
            DrawIconEx ( pDC->m_hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, 
                m_hCheckWhiteIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        else
        {
            //
            DrawIconEx ( pDC->m_hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, 
                m_hCheckBlackIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        //
        pDC->SelectObject ( hOldBrush );
    }

    //
    if ( lpDrawItemStruct->itemAction & ( ODA_FOCUS | ODA_SELECT ) )
    {
        hOldBrush = ( HBRUSH ) pDC->SelectObject ( CTRXColors::GetFGMenuSelectedBrush(CTRXColors::m_iDarkTheme != 0) );
    }
    else
    {
        hOldBrush = ( HBRUSH ) pDC->SelectObject ( CTRXColors::GetFGMenuBrush(CTRXColors::m_iDarkTheme != 0) );
    }

    //
    if ( lpDrawItemStruct->itemState & ODS_SELECTED )
    {
        if ( lpDrawItemStruct->itemState & ( ODS_GRAYED | ODS_DISABLED ) )
        {
            colorFg = pDC->SetTextColor ( CTRXColors::GetFGDisabledCR(CTRXColors::m_iDarkTheme != 0) );
        }
        else
        {
            colorFg = pDC->SetTextColor ( CTRXColors::GetFGMenuSelectedCR(CTRXColors::m_iDarkTheme != 0) );
        }
    }
    else
    {
        if ( lpDrawItemStruct->itemState & ( ODS_GRAYED | ODS_DISABLED ) )
        {
            colorFg = pDC->SetTextColor ( CTRXColors::GetFGDisabledCR(CTRXColors::m_iDarkTheme != 0) );
        }
        else
        {
            colorFg = pDC->SetTextColor ( CTRXColors::GetFGMenuCR(CTRXColors::m_iDarkTheme != 0) );
        }
    }

    //
    pRect->left += xCheckIcon;

    if ( pText != NULL && strlen ( pText ) > 0 )
    {
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            pDC->FillRect ( pRect, CTRXColors::GetBKMenuSelectedCBrush(CTRXColors::m_iDarkTheme != 0) );
        }
        else
        {
            pDC->FillRect ( pRect, CTRXColors::GetBKMenuCBrush(CTRXColors::m_iDarkTheme != 0) );
        }

        bkMode          = pDC->SetBkMode ( TRANSPARENT );
        pRect->left     += EXTRA_PIXELS_WIDTH / 2;

        strcpy_s ( szText, sizeof(szText), pText );
        char *pTab = strchr( szText, '\t' );
        if ( pTab != NULL )
        {
            *pTab = '\0';
            pTab++;
        }

        pDC->DrawText ( szText, pRect, TextAttrLeft );
        if ( pTab )
        {
            CRect rect = pRect;
            rect.right = rect.right - EXTRA_PIXELS_WIDTH / 2;
            pDC->DrawText ( pTab, &rect, TextAttrRight );
        }

        pDC->SetBkMode ( bkMode );
    }
    else
    {
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            pDC->FillRect ( pRect, CTRXColors::GetBKMenuSelectedCBrush(CTRXColors::m_iDarkTheme != 0) );
        }
        else
        {
            pDC->FillRect ( pRect, CTRXColors::GetBKMenuCBrush(CTRXColors::m_iDarkTheme != 0) );
        }

        bkMode          = pDC->SetBkMode ( TRANSPARENT );

        int penSize = 1;
        CTRXPen penBlack;
        penBlack.CreatePen(PS_SOLID, penSize, CTRXColors::GetFGMenuCR(CTRXColors::m_iDarkTheme != 0) );
        CPen* pOldPen = pDC->SelectObject(&penBlack);

        pRect->left     += EXTRA_PIXELS_WIDTH / 2;
        int middle = ( pRect->bottom  - pRect->top ) / 2;
        pDC->MoveTo ( pRect->left, pRect->top + middle );
        pDC->LineTo ( pRect->right - EXTRA_PIXELS_WIDTH / 2, pRect->top + middle );

        pDC->SelectObject(pOldPen);
        penBlack.DeleteObject();

    }

    pDC->SetTextColor ( colorFg );

    pDC->SelectObject ( hOldBrush );

}

//
//====================================================================================
//      Main Entry DrawItem
//
//      Must be a little cleaned
//====================================================================================
void CTRXMenuBase::DrawMenuItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    //
    if ( lpDrawItemStruct == NULL )
    {
        return;
    }

    //
    //      Get Menu Informations
    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );

    //
    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
    menuItemInfo.cbSize  = sizeof ( menuItemInfo );

    //
    menuItemInfo.fMask      =  MIIM_STRING;
    menuItemInfo.cch        = sizeof ( szText ) - 1;
    menuItemInfo.dwTypeData = szText;
    //  itemData contains the position / index
    UINT iPos   = ( UINT ) lpDrawItemStruct->itemData;
    //  We could search by itemID
    UINT id     = lpDrawItemStruct->itemID;

    //
    BOOL bRes = GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
    if ( bRes )
    {
        //
        //      Menu rectangle
        CRect   rectMenu = lpDrawItemStruct->rcItem;

        CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        if ( pDC )
        {
            DrawMenuItem( lpDrawItemStruct, pDC, &rectMenu, szText );
        }
    }
}

//
//====================================================================================
//      Compute the maximum Bitmap size
//====================================================================================
void CTRXMenuBase::ComputeBitmapMaxSize ( HBITMAP hBitmap )
{
    if ( hBitmap )
    {
        //
        //      Bitmap
        CBitmap *pBitmap = CBitmap::FromHandle ( hBitmap );
        if ( pBitmap )
        {
            //
            //      Bitmap Info
            BITMAP              bmInfo;
            ZeroMemory ( &bmInfo, sizeof ( bmInfo ) );

            if ( pBitmap->GetBitmap ( &bmInfo ) )
            {
                if ( m_iBitmapWidth < bmInfo.bmWidth )
                {
                    m_iBitmapWidth = bmInfo.bmWidth;
                }

                if ( m_iBitmapHeight < bmInfo.bmHeight )
                {
                    m_iBitmapHeight = bmInfo.bmHeight;
                }
            }
        }
    }
}

//
//====================================================================================
//      Compute the maximum Bitmap size
//====================================================================================
void CTRXMenuBase::ComputeBitmapMaxSize (  )
{
    m_iBitmapWidth  = 0;
    m_iBitmapHeight = 0;

    for ( UINT iPos = 0; iPos < GetMenuItemCount ( ); iPos++ )
    {
        MENUITEMINFO    menuItemInfo;
        ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
        menuItemInfo.cbSize     = sizeof ( menuItemInfo );

        menuItemInfo.fMask      =   MIIM_BITMAP | MIIM_CHECKMARKS;
        BOOL bRes = GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
        if ( bRes )
        {
            if ( menuItemInfo.hbmpChecked )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpChecked  );
            }

            if ( menuItemInfo.hbmpUnchecked )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpUnchecked );
            }

            if ( menuItemInfo.hbmpItem )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpItem  );
            }

        }
    }

    m_iBitmapWidth  += (EXTRA_PIXELS_WIDTH*2);
    m_iBitmapHeight += (EXTRA_PIXELS_HEIGHT*2);
}

//
//====================================================================================
//      Draw a bitmap
//====================================================================================
void CTRXMenuBase::DrawBitmap(CDC *pDC, CBitmap *pBitmap, CRect *pRect)
{
    //
    //      Memory DC
    CDC     memDC;
    memDC.CreateCompatibleDC ( pDC );           // ...create it

    //
    //      Select the Bitmap
    if ( memDC.m_hDC )
    {
        CBitmap *pOldBitmap = memDC.SelectObject( pBitmap );
        
        *pRect -= CRect ( 1, 1, 2, 2 );             // shrink

        // blast bits to screen
        pDC->BitBlt ( pRect->left, pRect->top,
            pRect->Width(), pRect->Height(), &memDC, 0, 0, SRCCOPY );

        memDC.SelectObject ( pOldBitmap );          // restore DC
    }
}

//
//====================================================================================
//      Draw a Bitmap
//====================================================================================
int CTRXMenuBase::DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if ( lpDrawItemStruct == NULL )
    {
        return m_iBitmapWidth;
    }

    CRect rectBitmap    = lpDrawItemStruct->rcItem;

    //
    //      Bitmap Info
    BITMAP              bmInfo;
    ZeroMemory ( &bmInfo, sizeof ( bmInfo ) );

    //
    //      Get Menu Informations
    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
    menuItemInfo.cbSize  = sizeof ( menuItemInfo );

    menuItemInfo.fMask      = MIIM_BITMAP | MIIM_CHECKMARKS;
    //  itemData contains the position / index
    UINT iPos   = ( UINT ) lpDrawItemStruct->itemData;
    //  We could search by itemID
    UINT id     = lpDrawItemStruct->itemID;

    //
    BOOL bRes = GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
    if ( bRes )
    {

        CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        if ( pDC )
        {
            // EraseRectWithGrade1 ( &rectBitmap, pDC );

            //
            //      No Bitmap to display
            if ( menuItemInfo.hbmpUnchecked == NULL )
            {
                return m_iBitmapWidth;
            }

            CBitmap *pBitmap;
            pBitmap             = CBitmap::FromHandle ( menuItemInfo.hbmpUnchecked );
            if ( pBitmap == NULL )
            {
                return m_iBitmapWidth;
            }

            pBitmap->GetBitmap ( &bmInfo );
            rectBitmap.top      += EXTRA_PIXELS_HEIGHT;
            rectBitmap.left     += EXTRA_PIXELS_WIDTH;
            rectBitmap.right    = rectBitmap.left + bmInfo.bmWidth + EXTRA_PIXELS_WIDTH;
            rectBitmap.bottom   = rectBitmap.top + bmInfo.bmHeight + EXTRA_PIXELS_HEIGHT;

            //
            if ( lpDrawItemStruct->itemState & ODS_SELECTED )
            {
                pDC->DrawFrameControl( &rectBitmap, DFC_BUTTON, DFCS_BUTTONPUSH );
            }

            //
            //      Draw the Bitmap
            DrawBitmap ( pDC, pBitmap, &rectBitmap );
        }
    }

    return ( m_iBitmapWidth );

}

//
//====================================================================================
//
//====================================================================================
BOOL CTRXMenuBase::SetMenuItemBitmaps ( UINT nPosition, UINT nFlags, const CBitmap *pBmpUnchecked,
                                        const CBitmap *pBmpChecked )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->SetMenuItemBitmaps ( nPosition, nFlags, pBmpUnchecked, pBmpChecked );
    }

    return FALSE;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMenuBase::AdjustMenu ( CWnd *pWnd, CMenu *pMenu )
{
    if ( pWnd == NULL || pMenu == NULL )
    {
        return;
    }

    DWORD dwStyle = pWnd->GetStyle();
    WINDOWPLACEMENT wp;
    pWnd->GetWindowPlacement( &wp );

    int iRemastered     = theApp.GetProfileInt( PROFILE_MENU, PROFILE_REMASTERED, 1 );
    int iStandard       = theApp.GetProfileInt( PROFILE_MENU, PROFILE_STANDARD, 1 );

    UINT bChecked       = MF_CHECKED | MF_BYCOMMAND;
    UINT bUnChecked     = MF_UNCHECKED | MF_BYCOMMAND;

    if ( CTRXColors::m_iDarkTheme == 1 )
    {
        pMenu->CheckMenuItem ( IDM_DARK_THEME, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_DARK_THEME, bUnChecked );
    }

    if ( CTRXColors::m_iDarkTheme == 2 )
    {
        pMenu->CheckMenuItem ( IDM_DARK_FULL, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_DARK_FULL, bUnChecked );
    }

    if ( iRemastered |= 0 )
    {
        pMenu->CheckMenuItem ( IDM_REMASTERED, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_REMASTERED, bUnChecked );
    }

    if ( iStandard |= 0 )
    {
        pMenu->CheckMenuItem ( IDM_STANDARD, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_STANDARD, bUnChecked );
    }

    if ( CTRXGlobal::m_iUnchecked |= 0 )
    {
        pMenu->CheckMenuItem ( IDM_UNCHECKED, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_UNCHECKED, bUnChecked );
    }

    if ( CTRXGlobal::m_iSearchExt |= 0 )
    {
        pMenu->CheckMenuItem ( IDM_SEARCH_EXT, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_SEARCH_EXT, bUnChecked );
    }

    if ( CTRXGlobal::m_iSearchPosExt |= 0 )
    {
        pMenu->CheckMenuItem ( IDM_SEARCH_POS_EXT, bChecked );
    }
    else
    {
        pMenu->CheckMenuItem ( IDM_SEARCH_POS_EXT, bUnChecked );
    }

    UINT bDisabled      = MF_DISABLED | MF_GRAYED | MF_BYCOMMAND;
    UINT bEnabled       = MF_ENABLED | MF_BYCOMMAND;

    if ( ( dwStyle & WS_MAXIMIZEBOX ) != 0 )
    {
        if ( wp.showCmd == SW_NORMAL )
        {
            pMenu->EnableMenuItem  ( SC_MAXIMIZE, bEnabled );

            pMenu->EnableMenuItem  ( SC_RESTORE, bDisabled );
        }
        else
        {
            pMenu->EnableMenuItem  ( SC_MAXIMIZE, bDisabled );

            pMenu->EnableMenuItem  ( SC_RESTORE, bEnabled );
        }
    }
    else
    {
        pMenu->EnableMenuItem  ( SC_MAXIMIZE, bDisabled );

        pMenu->EnableMenuItem  ( SC_RESTORE, bDisabled );
    }

    if ( ( dwStyle & WS_MINIMIZEBOX ) != 0 )
    {
        pMenu->EnableMenuItem  ( SC_MINIMIZE, bEnabled );
    }
    else
    {
        pMenu->EnableMenuItem  ( SC_MINIMIZE, bDisabled );
    }

    if ( ( dwStyle & WS_SIZEBOX ) != 0 )
    {
        pMenu->EnableMenuItem  ( SC_SIZE, bEnabled );
    }
    else
    {
        pMenu->EnableMenuItem  ( SC_SIZE, bDisabled );
    }

}
