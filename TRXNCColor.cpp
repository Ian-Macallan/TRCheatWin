#include "stdafx.h"

#include "resource.h"

#include "TRXNCColor.h"
#include "TRXColors.h"
#include "TRXGlobal.h"
#include "TRXMenuBase.h"
#include "TRXGDI.h"
#include "TRXCHEATWIN.h"
#include "TRXDialogBase.h"
#include "TRXMapAreas.h"
#include "TRXPropertySheet.h"

extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CRect CTRXNCColor::GetFrameFullRect ( const CRect &windowRECT )
{
    CRect frameFullRECT;
    frameFullRECT.left      = 0;
    frameFullRECT.top       = 0;
    frameFullRECT.right     = windowRECT.Width();
    frameFullRECT.bottom    = windowRECT.Height();
    return frameFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CRect CTRXNCColor::GetCaptionFullRect ( const CRect &windowRECT )
{
    //  EG : 1
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    CRect captionFullRECT;

    captionFullRECT.left    = 1;
    captionFullRECT.top     = 1;
    captionFullRECT.right   = windowRECT.Width() - 1;
    captionFullRECT.bottom  = yCaption + 2 * yBorder + yFrame;

    return captionFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CRect CTRXNCColor::GetCaptionInsideRect ( const CRect &windowRECT )
{
    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    int xLeft           = xBorder + xFrame;
    int yTop            = yBorder + yFrame;

    int x               = xLeft;
    int y               = yTop;
    int cx              = windowRECT.Width() - 2 * xFrame - xBorder;
    int cy              = yCaption + yBorder + yFrame;

    CRect   captionInsideRECT;
    captionInsideRECT.left      = x; 
    captionInsideRECT.top       = y;
    captionInsideRECT.right     = captionInsideRECT.left + cx;
    captionInsideRECT.bottom    = captionInsideRECT.top + cy;
    return captionInsideRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CRect CTRXNCColor::GetClientFullRect ( const CRect &windowRECT )
{
    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int xIcon           = GetSystemMetrics(SM_CXICON);
    int yIcon           = GetSystemMetrics(SM_CYICON);

    int xIconSmall      = GetSystemMetrics(SM_CXSMICON);
    int yIconSmall      = GetSystemMetrics(SM_CYSMICON);

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    CRect clientFullRECT;

    clientFullRECT.top        = yBorder + yFrame + yCaption + yBorder;
    clientFullRECT.left       = 0;
    clientFullRECT.bottom     = windowRECT.Height();
    clientFullRECT.right      = windowRECT.Width();

    return clientFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXNCColor::GetIconWidth ( )
{
    int xIcon           = GetSystemMetrics(SM_CXICON);
    return xIcon * 3 / 2 - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXNCColor::GetIconHeight ( )
{
    int yCaption        = GetSystemMetrics(SM_CYCAPTION);
    return yCaption - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CRect CTRXNCColor::GetIconFullRECT ( const CRect &windowRECT, int left )
{
    int yBorder         = GetSystemMetrics(SM_CYBORDER);
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    CRect captionFullRECT   = GetCaptionFullRect ( windowRECT );

    CRect iconFullRECT;

    iconFullRECT.left       = left;
    iconFullRECT.top        = captionFullRECT.top; // yBorder + yFrame;
    iconFullRECT.right      = iconFullRECT.left + GetIconWidth ( );
    iconFullRECT.bottom     = iconFullRECT.top + GetIconHeight ( ) + yFrame - 1; // iconFullRECT.top + yCaption - 1;

    return iconFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXNCColor::CTRXNCColor(void)
{
    m_pContextMenu          = NULL;

    m_iHover                = ICON_NOT_SET;

   m_bLeftPressed           = FALSE;
   m_windowRECT.top         = -1;
   m_windowRECT.left        = -1;
   m_LeftPressedPoint.x     = -1;
   m_LeftPressedPoint.y     = -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXNCColor::~CTRXNCColor(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//  Not Used DrawIconEx is better
/////////////////////////////////////////////////////////////////////////////
void CTRXNCColor::DrawResizedIcon ( CDC *pDC, HICON hIcon, const RECT *pRect )
{
    ICONINFO info;
    ZeroMemory ( &info, sizeof(info) );

    //
    BITMAP bmp;
    ZeroMemory(&bmp, sizeof(bmp));

    int xIcon   = 0;
    int yIcon  = 0;
    int pixels      = 0;

    //
    BOOL bRes = GetIconInfo(hIcon, &info);

    //
    if ( info.hbmColor )
    {
        const int nWrittenBytes = GetObject(info.hbmColor, sizeof(bmp), &bmp);
        if ( nWrittenBytes > 0 )
        {
            xIcon       = bmp.bmWidth;
            yIcon       = bmp.bmHeight;
            pixels      = bmp.bmBitsPixel;
        }
    }
    else if ( info.hbmMask )
    {
        // Icon has no color plane, image data stored in mask
        const int nWrittenBytes = GetObject(info.hbmMask, sizeof(bmp), &bmp);
        if(nWrittenBytes > 0)
        {
            xIcon   = bmp.bmWidth;
            yIcon  = bmp.bmHeight / 2;
            pixels      = 1;
        }
    }

    //
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    //
    CTRXBitmap bitmap;
    bitmap.CreateBitmapIndirect ( &bmp );
    CBitmap* pOldBitmap = memDC.SelectObject ( &bitmap );
    memDC.DrawIcon ( 0, 0, hIcon );

    if ( true )
    {
        bRes = pDC->StretchBlt (
            pRect->left,        // x upper left
            pRect->top,         // y upper left
            // The next two lines specify the width and height.
            pRect->right - pRect->left,
            pRect->bottom - pRect->top,
            //  Source
            &memDC,             // source device context
            0, 0,               // x and y upper left
            xIcon,          // source bitmap width
            yIcon,         // source bitmap height
            SRCCOPY );          // raster operation
    }
    else
    {
        bRes = pDC->BitBlt (
            pRect->left,        // x upper left
            pRect->top,         // y upper left
            // The next two lines specify the width and height.
            pRect->right - pRect->left,
            pRect->bottom - pRect->top,
            //  Source
            &memDC,             // source device context
            0, 0,               // x and y upper left
            SRCCOPY );          // raster operation
    }

    //
    if ( info.hbmColor)
    {
        DeleteObject(info.hbmColor);
    }

    if ( info.hbmMask )
    {
        DeleteObject(info.hbmMask);
    }

    memDC.SelectObject ( pOldBitmap );

    //
    DeleteDC(memDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::ScreenPointOverRect ( CWnd *pWnd, CPoint pt, CRect cRect )
{
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    cRect.left      += windowRECT.left;
    cRect.top       += windowRECT.top;

    cRect.right     += windowRECT.left;
    cRect.bottom    += windowRECT.top;

    return cRect.PtInRect( pt );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXNCColor::DrawIconFrame ( CDC *pDC, const CRect crect, bool bFillRect, CBrush *pbr )
{
    if ( pDC == NULL )
    {
        return;
    }

    if ( bFillRect )
    {
        RECT rect = crect;
        if ( pbr != NULL )
        {
            pDC->FillRect ( &rect, pbr );
        }
        else
        {
            pDC->FillRect ( &rect, CTRXColors::GetBKNormalCBrush( true ) );
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//  To draw Background only set icon to zero
/////////////////////////////////////////////////////////////////////////////
void CTRXNCColor::DrawIcon ( CDC *pDC, UINT icon, const CRect crect, bool bFillRect, CBrush *pbr )
{
    if ( pDC == NULL )
    {
        return;
    }

    //
    DrawIconFrame ( pDC, crect, bFillRect, pbr );

    //
    if ( icon != 0 )
    {
        int xIconSmall  = GetSystemMetrics(SM_CXSMICON);
        int yIconSmall  = GetSystemMetrics(SM_CYSMICON);

        int xMargin = ( crect.Width() - xIconSmall ) / 2;
        int yMargin = ( crect.Height() - yIconSmall ) / 2;

        HICON hIcon = AfxGetApp()->LoadIcon(icon);
        DrawIconEx(pDC->m_hDC, crect.left + xMargin, crect.top + yMargin, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CDC *CTRXNCColor::GetDeviceContext ( CWnd *pWnd )
{
    return  pWnd->GetWindowDC();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::PaintCaption( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

    if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    // int xIcon           = GetSystemMetrics(SM_CXICON);
    // int yIcon           = GetSystemMetrics(SM_CYICON);

    // int xIconSmall      = GetSystemMetrics(SM_CXSMICON);
    // int yIconSmall      = GetSystemMetrics(SM_CYSMICON);

    // int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    int xLeft           = xBorder + xFrame;
    int yTop            = yBorder + yFrame;

    int iconWidth       = GetIconWidth();
    int iconHeight      = GetIconHeight();

    //
    CDC* pDC = GetDeviceContext ( pWnd );

    //
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    //
    COLORREF foregroundColor = CTRXColors::GetFGNormalCR(CTRXColors::m_iDarkTheme != 0);

    //  First Draw Frmae Rect Around the window
    CRect frameFullRECT     = GetFrameFullRect ( windowRECT );
    pDC->FrameRect ( &frameFullRECT, CTRXColors::GetCyanCBrush() );

    //  The The Caption Rect
    CRect captionFullRECT   = GetCaptionFullRect ( windowRECT );

    //
    if ( bActive )
    {
        pDC->FillSolidRect(&captionFullRECT, CTRXColors::GetBKSelectedCR(CTRXColors::m_iDarkTheme != 0));
    }
    else
    {
        pDC->FillSolidRect(&captionFullRECT, CTRXColors::GetBKNormalCR(CTRXColors::m_iDarkTheme != 0));
    }

    //
    //  Caption Rect
    CRect captionInsideRECT = GetCaptionInsideRect ( windowRECT );

    // Draw Old FRame (Window XP)
    UINT captionOption  = DFCS_CAPTIONMIN | DFCS_CAPTIONMAX | DFCS_CAPTIONCLOSE | DFCS_CAPTIONRESTORE | DFCS_CAPTIONHELP;
    captionOption       |= DFCS_FLAT;
    // pDC->DrawFrameControl ( captionInsideRECT, DFC_CAPTION, captionOption );

    char szTitle [ MAX_PATH ];
    pWnd->GetWindowText ( szTitle, sizeof(szTitle));
    CRect textRECT  = captionInsideRECT;
    textRECT.left   = textRECT.left + iconWidth + xBorder;

    pDC->SetBkMode (TRANSPARENT);
    pDC->SetTextColor (foregroundColor);
    pDC->DrawText ( szTitle, &textRECT, DT_LEFT | DT_VCENTER );

    //
    DWORD dwStyle = pWnd->GetStyle();
    WINDOWPLACEMENT wp;
    pWnd->GetWindowPlacement( &wp );

    //  ICONS
    //////////////////////////////

    //  LEFT
    m_IconRect = GetIconFullRECT ( windowRECT, 1 /* xLeft */ );
    if ( CTRXColors::m_iSquareCorner == 1 )
    {
        DrawIcon ( pDC, IDR_MAINFRAME, m_IconRect );
    }
    else
    {
        if ( ( dwStyle & WS_SYSMENU ) && ( dwStyle & WS_CAPTION ) )
        {
            DrawIcon ( pDC, IDR_MAINFRAME, m_IconRect );
        }
    }

    //  RIGHT
    // Close
    m_CloseRect = GetIconFullRECT ( windowRECT, /* captionInsideRECT.right */ captionFullRECT.right - iconWidth );
    DrawIcon ( pDC, IDI_CLOSE, m_CloseRect );

    //  Maximize
    m_MaximizeRect = GetIconFullRECT ( windowRECT, m_CloseRect.left - iconWidth );
    if ( dwStyle & WS_MAXIMIZEBOX )
    {
        if ( wp.showCmd == SW_NORMAL )
        {
            DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect );
        }
        else
        {
            DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect );
        }
    }

    //  Minilmize
    m_MinimizeRect = GetIconFullRECT ( windowRECT, m_MaximizeRect.left - iconWidth );
    if ( dwStyle & WS_MINIMIZEBOX )
    {
        DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect );
    }

    //
    pWnd->ReleaseDC ( pDC );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::PaintWindow( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

	if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    //
    CDC* pDC = GetDeviceContext ( pWnd );

    //
    //  Fill Background
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    //  First Draw Frmae Rect Around the window
    CRect frameFullRECT = GetFrameFullRect ( windowRECT );
    pDC->FrameRect ( &frameFullRECT, CTRXColors::GetCyanCBrush() );

    //
    RECT clientFullRect = GetClientFullRect ( windowRECT );
    pDC->FillRect ( &clientFullRect, CTRXColors::GetBKNormalCBrush(CTRXColors::m_iDarkTheme != 0) );

    //
    pWnd->ReleaseDC ( pDC );

    //
    PaintCaption ( pWnd, bActive, darkIndicator );

    //
    {
        RECT clientRect;
        pWnd->GetClientRect( &clientRect );
        pWnd->InvalidateRect ( &clientRect );
    }
    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::Activate( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

	if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    return PaintCaption ( pWnd, bActive, darkIndicator );
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcLButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

	//
    if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == darkIndicator )
    {
        if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
        {
            //  Default Treatment
            if ( CTRXColors::m_iSquareCorner == 1 )
            {
                RECT clientRect;
                pWnd->GetClientRect ( &clientRect );
                pWnd->ClientToScreen ( &clientRect );
                CPoint pt;
                pt.x = clientRect.left;
                pt.y = clientRect.top;

                BOOL bTreated = PopupSystemMenu ( pWnd, nHitTest, pt, darkIndicator );
                return bTreated;
            }
            else
            {
                return FALSE;
            }
        }

        //
        else if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
        {
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
        {
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
        {
            return TRUE;
        }

        //  Icons At RIght
        int xIcon   = GetSystemMetrics(SM_CXICON);

        //
        CRect iconsRect;
        iconsRect.top       = m_CloseRect.top;
        iconsRect.bottom    = m_CloseRect.bottom;
        iconsRect.left      = min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
        iconsRect.right     = max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
        iconsRect.left      = iconsRect.left - xIcon;
        if ( ScreenPointOverRect( pWnd, point, iconsRect ) )
        {
            return TRUE;
        }

        //  We Could handle move here
        CRect windowRECT;
        pWnd->GetWindowRect(&windowRECT);
        CRect captionRect = GetCaptionFullRect ( windowRECT );
        if ( ScreenPointOverRect( pWnd, point, captionRect ) )
        {
            m_bLeftPressed      = FALSE;
            m_windowRECT        = windowRECT;
            m_LeftPressedPoint  = point;
            return m_bLeftPressed;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcLButtonUp(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator)
{
    m_bLeftPressed  = FALSE;

	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

	//
    //
    if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == darkIndicator )
    {
        if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
        {
            //  Default Treatment
            return FALSE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
        {
            ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL );
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
        {
            //
            DWORD dwStyle = pWnd->GetStyle();
            WINDOWPLACEMENT wp;
            pWnd->GetWindowPlacement( &wp );
            if ( dwStyle & WS_MINIMIZEBOX )
            {
                ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL );
            }
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
        {
            //
            DWORD dwStyle = pWnd->GetStyle();
            WINDOWPLACEMENT wp;
            pWnd->GetWindowPlacement( &wp );
            if ( dwStyle & WS_MAXIMIZEBOX )
            {
                if ( wp.showCmd == SW_NORMAL )
                {
                    ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, NULL );
                }
                else
                {
                    ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_RESTORE, NULL );
                }
            }
            return TRUE;
        }

        //  Icons At RIght
        int xIcon   = GetSystemMetrics(SM_CXICON);

        CRect iconsRect;
        iconsRect.top       = m_CloseRect.top;
        iconsRect.bottom    = m_CloseRect.bottom;
        iconsRect.left      = min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
        iconsRect.right     = max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
        iconsRect.left      = iconsRect.left - xIcon;
        if ( ScreenPointOverRect( pWnd, point, iconsRect ) )
        {
            return TRUE;
        }

    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::SetThemeChanged ( CWnd *pWnd )
{
    //
	if ( theApp.OSVersionGreaterThan ( 6, 1 ) )
	{
        CMenu *pMenu = GetSystemMenu(pWnd, FALSE);

        if ( pMenu != NULL )
        {
            //  Square Corners
            if ( CTRXColors::m_iSquareCorner == 1 )
            {
                //  Dark Theme Full
                if ( CTRXColors::m_iDarkTheme == 2 )
                {
                    CTRXColors::SetWindowTheme ( pWnd );
                    //  Remove System Menu
                    pWnd->ModifyStyle ( WS_SYSMENU, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
                    CTRXMenuBase::SetOwnDraw ( pMenu->GetSubMenu(0), true, ID_SYS_MENU );
                }
                //  Dark Theme and Normal Theme
                else
                {
                    CTRXColors::SetWindowTheme ( pWnd );
                    //  Add System Menu
                    pWnd->ModifyStyle ( NULL, WS_SYSMENU | WS_MINIMIZEBOX, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
                    if ( false )
                    {
                        DWORD dwStyle = GetWindowLong( pWnd->GetSafeHwnd(), GWL_STYLE );
                        dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX;
                        SetWindowLong( pWnd->GetSafeHwnd(), GWL_STYLE, dwStyle );
                    }
                    CTRXMenuBase::SetOwnDraw ( pMenu->GetSubMenu(0), false, ID_SYS_MENU );

                    pWnd->SetWindowPos ( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
                }
	        }
            //  No Square Corners
            else
            {
                CTRXColors::SetWindowTheme ( pWnd );

                //  Add System Menu
                pWnd->ModifyStyle ( NULL, WS_SYSMENU | WS_MINIMIZEBOX, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
                if ( false )
                {
                    DWORD dwStyle = GetWindowLong( pWnd->GetSafeHwnd(), GWL_STYLE );
                    dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX;
                    SetWindowLong( pWnd->GetSafeHwnd(), GWL_STYLE, dwStyle );
                }

                if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == 1 )
                {
                    if ( pMenu )
                    {
                        CTRXMenuBase::SetOwnDraw ( pMenu->GetSubMenu(0), true, ID_SYS_MENU );
                    }
                }
                else
                {
                    if ( pMenu )
                    {
                        CTRXMenuBase::SetOwnDraw ( pMenu->GetSubMenu(0), false, ID_SYS_MENU );
                    }
                }

                pWnd->SetWindowPos ( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

            }

            if ( CTRXColors::m_iDarkTheme == 0 )
            {
                //
                CTRXPropertySheet *pSheet = dynamic_cast<CTRXPropertySheet *>(pWnd);
                if ( pSheet != NULL )
                {
                    pSheet->SetContextMenu ( NULL );
                }

                //
                CTRXDialogBase *pDialog = dynamic_cast<CTRXDialogBase *>(pWnd);
                if ( pDialog != NULL )
                {
                    pDialog->SetContextMenu ( NULL );
                }
            }
        }
    }

    //
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::HandleSquareCorners ( CWnd *pWnd )
{
    //
    if ( CTRXColors::m_iSquareCorner == 1 )
    {
	    if ( theApp.OSVersionGreaterThan ( 6, 1 ) )
	    {
            if ( CTRXColors::m_iDarkTheme == 2 )
            {
                CTRXColors::SetWindowTheme ( pWnd );
                //  Remove System Menu
                pWnd->ModifyStyle ( WS_SYSMENU, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
                return TRUE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::PopupSystemMenu ( CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator )
{
    CMenu *pMenu = pWnd->GetSystemMenu(FALSE);
    if ( pMenu )
    {
        //
        CTRXPropertySheet *pSheet = dynamic_cast<CTRXPropertySheet *>(pWnd);
        if ( pSheet != NULL )
        {
            CTRXMenuBase        menu;
            menu.Attach ( pMenu->m_hMenu );
            m_pContextMenu = menu.GetSubMenu ( 0 );
            pSheet->SetContextMenu ( m_pContextMenu );
            //  Use System Menu as Popup Menu
            LPARAM lParam = m_pContextMenu->TrackPopupMenu ( 
                TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON /* | TPM_NONOTIFY */ | TPM_RETURNCMD, 
                point.x, point.y, pWnd );
            if ( lParam ) 
            {
                PostMessage(pWnd->GetSafeHwnd(), WM_SYSCOMMAND, lParam, 0);
            }
            menu.Detach ( );
            return TRUE;
        }

        //
        CTRXDialogBase *pDialog = dynamic_cast<CTRXDialogBase *>(pWnd);
        if ( pDialog != NULL )
        {
            CTRXMenuBase        menu;
            menu.Attach ( pMenu->m_hMenu );
            m_pContextMenu = menu.GetSubMenu ( 0 );
            pDialog->SetContextMenu ( m_pContextMenu );
            //  Use System Menu as Popup Menu
            LPARAM lParam = m_pContextMenu->TrackPopupMenu ( 
                TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON /* | TPM_NONOTIFY */ | TPM_RETURNCMD, 
                point.x, point.y, pWnd );
            if ( lParam ) 
            {
                PostMessage(pWnd->GetSafeHwnd(), WM_SYSCOMMAND, lParam, 0);
            }
            menu.Detach ( );
            return TRUE;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcRButtonDown( CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

	if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == darkIndicator )
    {
        BOOL bTreated = PopupSystemMenu ( pWnd, nHitTest, point, darkIndicator );
        return bTreated;
    }

    CTRXPropertySheet *pSheet = dynamic_cast<CTRXPropertySheet *>(pWnd);
    if ( pSheet != NULL )
    {
        BOOL bTreated = PopupSystemMenu ( pWnd, nHitTest, point, darkIndicator );
        return bTreated;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnLButtonUp(CWnd *pWnd, UINT nFlags, CPoint point, int darkIndicator)
{
    m_bLeftPressed  = FALSE;
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcMouseMove(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

    //
    if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == darkIndicator )
    {
        TRACKMOUSEEVENT tme;
        ZeroMemory ( &tme, sizeof(tme) );
        tme.cbSize      = sizeof(tme);
        tme.hwndTrack   = pWnd->m_hWnd;
        tme.dwFlags     = TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = 100; // HOVER_DEFAULT;
        TrackMouseEvent ( &tme );

        if ( m_bLeftPressed )
        {
            pWnd->SetWindowPos ( NULL, 
                m_windowRECT.left + point.x - m_LeftPressedPoint.x,
                m_windowRECT.top + point.y - m_LeftPressedPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        }

    }

    //  Always returns FALSE to continue the process
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnMouseMove(CWnd *pWnd, UINT nFlags, CPoint point, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

    //
    if ( CTRXColors::m_iDarkTheme == 2 || CTRXColors::m_iDarkTheme == darkIndicator )
    {
        //  Track Event
        TRACKMOUSEEVENT tme;
        ZeroMemory ( &tme, sizeof(tme) );
        tme.cbSize      = sizeof(tme);
        tme.hwndTrack   = pWnd->m_hWnd;
        tme.dwFlags     = TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent ( &tme );

        m_bLeftPressed  = FALSE;
    }

    //  Always returns FALSE to continue the process
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXNCColor::DrawAllIcons (  CWnd *pWnd, UINT iconOnly, int darkIndicator )
{
    if ( iconOnly == ICON_NOT_SET )
    {
        return;
    }

	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return;
	}

    //
    if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return;
	}

    //
    DWORD dwStyle = pWnd->GetStyle();
    WINDOWPLACEMENT wp;
    pWnd->GetWindowPlacement( &wp );

    //
    CDC* pDC = GetDeviceContext ( pWnd );

    if ( iconOnly == 0 || iconOnly == IDI_CLOSE )
    {
        DrawIcon ( pDC, IDI_CLOSE, m_CloseRect );
    }

    if ( iconOnly == 0 || iconOnly == IDI_MAXIMIZE || iconOnly == IDI_WINDOWED )
    {
        if ( dwStyle & WS_MAXIMIZEBOX )
        {
            if ( wp.showCmd == SW_NORMAL )
            {
                DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect );
            }
            else
            {
                DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect );
            }
        }
        else
        {
            DrawIconFrame ( pDC, m_MaximizeRect );
        }
    }

    if ( iconOnly == 0 || iconOnly == IDI_MINIMIZE )
    {
        if ( dwStyle & WS_MINIMIZEBOX )
        {
            DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect );
        }
        else
        {
            DrawIconFrame ( pDC, m_MinimizeRect );
        }
    }

    pWnd->ReleaseDC ( pDC );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcMouseHover(CWnd *pWnd, UINT nFlags, CPoint point, int darkIndicator)
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

    //
    if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
	}

    //  Always returns FALSE to continue the process
    if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
    {
        if ( m_iHover != IDI_CLOSE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();
            DrawIcon ( pDC, IDI_CLOSE, m_CloseRect, true, CTRXColors::GetRedCBrush() );
            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_CLOSE;
        }
        return FALSE;
    }
    else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
    {
        //
        DWORD dwStyle = pWnd->GetStyle();
        WINDOWPLACEMENT wp;
        pWnd->GetWindowPlacement( &wp );

        if ( m_iHover != IDI_MAXIMIZE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();
            if ( dwStyle & WS_MAXIMIZEBOX )
            {
                if ( wp.showCmd == SW_NORMAL )
                {
                    DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect, true, CTRXColors::GetCyanCBrush() );
                }
                else
                {
                    DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect, true, CTRXColors::GetCyanCBrush() );
                }
            }
            else
            {
                DrawIconFrame ( pDC, m_MaximizeRect, true, CTRXColors::GetBKNormalCBrush(CTRXColors::m_iDarkTheme != 0) );
            }
            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_MAXIMIZE;
        }
        return FALSE;
    }
    else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
    {
        //
        DWORD dwStyle = pWnd->GetStyle();
        WINDOWPLACEMENT wp;
        pWnd->GetWindowPlacement( &wp );

        if ( m_iHover != IDI_MINIMIZE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();
            if ( dwStyle & WS_MINIMIZEBOX )
            {
                DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect, true, CTRXColors::GetCyanCBrush() );
            }
            else
            {
                DrawIconFrame ( pDC, m_MinimizeRect, true, CTRXColors::GetBKNormalCBrush(CTRXColors::m_iDarkTheme != 0) );
            }
            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_MINIMIZE;
        }
        return FALSE;
    }

    DrawAllIcons ( pWnd, m_iHover );
    m_iHover = ICON_NOT_SET;

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::OnNcMouseLeave(CWnd *pWnd, int darkIndicator )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
		return FALSE;
	}

    //
    if ( CTRXColors::m_iDarkTheme != 2 && CTRXColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
	}

    //  Always returns FALSE to continue the process
    DrawAllIcons ( pWnd, m_iHover );
    m_iHover        = ICON_NOT_SET;
    m_bLeftPressed  = FALSE;
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMenu *CTRXNCColor::GetSystemMenu(CWnd *pWnd, BOOL bRevert )
{
	if ( theApp.OSVersionLowerThan ( 6, 1 ) )
	{
        //
        CMenu *pMenu  = pWnd->GetSystemMenu ( bRevert );
        return pMenu;
	}

	//
    if ( CTRXColors::m_iDarkTheme != 2 )
    {
        //
        CMenu *pMenu  = pWnd->GetSystemMenu ( bRevert );
        return pMenu;
    }

    //
    CMenu *pMenu  = pWnd->GetSystemMenu ( bRevert );
    return pMenu;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::InsertSystemMenu ( CWnd *pWnd, BOOL bRevert, UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem )
{
    BOOL bInserted = FALSE;

    CMenu *pMenu  = GetSystemMenu ( pWnd, bRevert );
    if ( pMenu != NULL )
    {
        bInserted = pMenu->InsertMenu ( nPosition, nFlags, nIDNewItem, lpszNewItem );
    }

    return bInserted;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNCColor::AppendSystemMenu ( CWnd *pWnd, BOOL bRevert, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem )
{
    BOOL bAppended = FALSE;

    CMenu *pMenu  = GetSystemMenu ( pWnd, bRevert );
    if ( pMenu != NULL )
    {
        bAppended = pMenu->AppendMenu ( nFlags, nIDNewItem, lpszNewItem );
    }

    return bAppended;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXNCColor::CheckSystemMenuItem ( CWnd *pWnd, BOOL bRevert, UINT nIDCheckItem, UINT nCheck )
{
    UINT result = 0;
    CMenu *pMenu  = GetSystemMenu ( pWnd, bRevert );
    if ( pMenu != NULL )
    {
        result = pMenu->CheckMenuItem ( nIDCheckItem, nCheck );
    }
    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXNCColor::GetSystemMenuState ( CWnd *pWnd, BOOL bRevert, UINT nID, UINT nFlags )
{
    UINT result = 0;
    CMenu *pMenu  = GetSystemMenu ( pWnd, bRevert );
    if ( pMenu != NULL )
    {
        result = pMenu->GetMenuState ( nID, nFlags );
    }

    return result;
}
