#include "stdafx.h"

#include "resource.h"

#include "TRXNC.h"
#include "TRXColors.h"
#include "TRXGlobal.h"
#include "TRXMenuBase.h"
#include "TRXGDI.h"

//
static 	int iconSize	= 16;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXNC::CTRXNC(void)
{
	m_pContextMenu	= NULL;
	m_iHover		= ICON_NOT_SET;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXNC::~CTRXNC(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//	Not Used DrawIconEx is better
/////////////////////////////////////////////////////////////////////////////
void CTRXNC::DrawResizedIcon ( CDC *pDC, HICON hIcon, const RECT *pRect )
{
	ICONINFO info;
	ZeroMemory ( &info, sizeof(info) );

	//
	BITMAP bmp;
    ZeroMemory(&bmp, sizeof(bmp));

	int iconWidth	= 0;
	int iconHeight	= 0;
	int pixels		= 0;

	//
	BOOL bRes = GetIconInfo(hIcon, &info);

	//
    if ( info.hbmColor )
    {
        const int nWrittenBytes = GetObject(info.hbmColor, sizeof(bmp), &bmp);
        if ( nWrittenBytes > 0 )
        {
            iconWidth	= bmp.bmWidth;
            iconHeight	= bmp.bmHeight;
            pixels		= bmp.bmBitsPixel;
        }
    }
    else if ( info.hbmMask )
    {
        // Icon has no color plane, image data stored in mask
        const int nWrittenBytes = GetObject(info.hbmMask, sizeof(bmp), &bmp);
        if(nWrittenBytes > 0)
        {
            iconWidth	= bmp.bmWidth;
            iconHeight	= bmp.bmHeight / 2;
            pixels		= 1;
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
			pRect->left,		// x upper left 
			pRect->top,			// y upper left 
			// The next two lines specify the width and height. 
			pRect->right - pRect->left, 
			pRect->bottom - pRect->top, 
			//	Source
			&memDC,				// source device context 
			0, 0,				// x and y upper left 
			iconWidth,			// source bitmap width 
			iconHeight,			// source bitmap height 
			SRCCOPY );			// raster operation 
	}
	else
	{
		bRes = pDC->BitBlt ( 
			pRect->left,		// x upper left 
			pRect->top,			// y upper left 
			// The next two lines specify the width and height. 
			pRect->right - pRect->left, 
			pRect->bottom - pRect->top, 
			//	Source
			&memDC,				// source device context 
			0, 0,				// x and y upper left 
			SRCCOPY );			// raster operation 
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
BOOL CTRXNC::ScreenPointOverRect ( CWnd *pWnd, CPoint pt, CRect cRect )
{
	CRect windowsRECT;
	pWnd->GetWindowRect(&windowsRECT);

	cRect.left		+= windowsRECT.left;
	cRect.top		+= windowsRECT.top;

	cRect.right		+= windowsRECT.left;
	cRect.bottom	+= windowsRECT.top;

	return cRect.PtInRect( pt );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXNC::DrawIconFrame ( CDC *pDC, const CRect crect, bool bFillRect, CBrush *pbr )
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
			pDC->FillRect ( &rect, CTRXColors::GetBlack48CBrush() );
		}
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//	To draw Background only set icon to zero
/////////////////////////////////////////////////////////////////////////////
void CTRXNC::DrawIcon ( CDC *pDC, UINT icon, const CRect crect, bool bFillRect, CBrush *pbr )
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
		int xMargin = ( crect.Width() - iconSize ) / 2;
		int yMargin = ( crect.Height() - iconSize ) / 2;
		HICON hIcon = AfxGetApp()->LoadIcon(icon);
		DrawIconEx(pDC->m_hDC, crect.left + xMargin, crect.top + yMargin, hIcon, iconSize, iconSize, 0, NULL, DI_NORMAL);
	}
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNC::PaintWindow( CWnd *pWnd, BOOL bActive )
{
	if ( CTRXGlobal::m_iDarkTheme == 0 )
	{
		return FALSE;
	}

	CDC* pDC = pWnd->GetWindowDC();

	//
	CRect windowsRECT;
	pWnd->GetWindowRect(&windowsRECT);

	CRect areaRECT;
	areaRECT.top	= 0;
	areaRECT.left	= 0;
	areaRECT.bottom	= windowsRECT.Height();
	areaRECT.right	= windowsRECT.Width();

	//
	areaRECT.top	= areaRECT.top + 1;
	areaRECT.left	= areaRECT.left + 1;
	areaRECT.bottom	= areaRECT.bottom - 1;
	areaRECT.right	= areaRECT.right - 1;

	//
	COLORREF foregroundColor = CTRXColors::GetWhiteCR();

	//	EG : 1
	int xBorder		= GetSystemMetrics(SM_CXBORDER);
	int yBorder		= GetSystemMetrics(SM_CYBORDER);

	//	EG : 3
	int xFrame		= GetSystemMetrics ( SM_CXDLGFRAME );
	int yFrame		= GetSystemMetrics ( SM_CYDLGFRAME );

	int height		= GetSystemMetrics(SM_CYCAPTION);

	int iconWidth	= GetSystemMetrics(SM_CXICON);
	int iconHeight	= GetSystemMetrics(SM_CYICON);

	int xLeft		= xBorder + xFrame;
	int yTop		= yBorder + yFrame;

	//
	int x		= xLeft;
	int y		= yTop;
	int cx		= windowsRECT.Width() - 2 * xFrame - xBorder;
	int cy		= height - yBorder;
   
	CRect captionRECT;
	captionRECT.left	= x;
	captionRECT.top		= y; 
	captionRECT.right	= captionRECT.left + cx;
	captionRECT.bottom	= captionRECT.top + cy;

	if ( bActive )
	{
		pDC->FillSolidRect(&captionRECT, CTRXColors::GetBlack32CR());
	}
	else
	{
		pDC->FillSolidRect(&captionRECT, CTRXColors::GetBlack48CR());
	}

	// Draw Old FRame (Window XP)
	UINT captionOption	= DFCS_CAPTIONMIN | DFCS_CAPTIONMAX | DFCS_CAPTIONCLOSE | DFCS_CAPTIONRESTORE | DFCS_CAPTIONHELP;
	captionOption		|= DFCS_FLAT;
	// pDC->DrawFrameControl ( captionRECT, DFC_CAPTION, captionOption );

	char szTitle [ MAX_PATH ];
	pWnd->GetWindowText ( szTitle, sizeof(szTitle));
	CRect textRECT = captionRECT;
	textRECT.left	= textRECT.left + iconWidth + xBorder;

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (foregroundColor);
	pDC->DrawText ( szTitle, &textRECT, DT_LEFT| DT_VCENTER );

	//
	DWORD dwStyle = pWnd->GetStyle();
	WINDOWPLACEMENT wp;
	pWnd->GetWindowPlacement( &wp );

	//	Icon is small
	m_IconRect.left			= xLeft;
	m_IconRect.top			= captionRECT.top;
	m_IconRect.right		= m_IconRect.left + iconWidth;
	m_IconRect.bottom		= m_IconRect.top + height - 1;
	if ( ( dwStyle & WS_SYSMENU ) && ( dwStyle & WS_CAPTION ) )
	{
		DrawIcon ( pDC, IDR_MAINFRAME, m_IconRect );
	}

	// Close
	m_CloseRect.left		= captionRECT.right - iconWidth;
	m_CloseRect.top			= captionRECT.top;
	m_CloseRect.right		= m_CloseRect.left + iconWidth;
	m_CloseRect.bottom		= m_CloseRect.top + height - 1;
	DrawIcon ( pDC, IDI_CLOSE, m_CloseRect );

	//	Maximize
	m_MaximizeRect.left		= m_CloseRect.left - iconWidth; 
	m_MaximizeRect.top		= m_CloseRect.top; 
	m_MaximizeRect.right	= m_CloseRect.left - 1;
	m_MaximizeRect.bottom	= m_MaximizeRect.top + height - 1;
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

	//	Minilmize
	m_MinimizeRect.left		= m_MaximizeRect.left - iconWidth; 
	m_MinimizeRect.top		= m_MaximizeRect.top; 
	m_MinimizeRect.right	= m_MaximizeRect.left - 1;
	m_MinimizeRect.bottom	= m_MinimizeRect.top + height - 1;
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
BOOL CTRXNC::OnNcLButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point)
{
	// TODO: Add Code Here
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
		if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
		{
			return FALSE;
		}

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

		int iconWidth	= GetSystemMetrics(SM_CXICON);

		//
		CRect iconsRect;
		iconsRect.top		= m_CloseRect.top;
		iconsRect.bottom	= m_CloseRect.bottom;
		iconsRect.left		= min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
		iconsRect.right		= max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
		iconsRect.left		= iconsRect.left - iconWidth;
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
BOOL CTRXNC::OnNcLButtonUp(CWnd *pWnd, UINT nHitTest, CPoint point)
{
	// TODO: Add Code Here
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
		if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
		{
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

		CRect iconsRect;
		iconsRect.top		= m_CloseRect.top;
		iconsRect.bottom	= m_CloseRect.bottom;
		iconsRect.left		= min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
		iconsRect.right		= max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
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
BOOL CTRXNC::OnNcRButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point)
{
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
		CMenu *pMenu = pWnd->GetSystemMenu(FALSE);
		if ( pMenu )
		{
			//
			CTRXMenuBase		menu;
			menu.Attach ( pMenu->m_hMenu );
			m_pContextMenu = menu.GetSubMenu ( 0 );
			m_pContextMenu->TrackPopupMenu ( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, pWnd );
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
BOOL CTRXNC::OnMouseMove(CWnd *pWnd, UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
		TRACKMOUSEEVENT tme;
		ZeroMemory ( &tme, sizeof(tme) );
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= pWnd->m_hWnd;
		tme.dwFlags		= TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
		tme.dwHoverTime	= HOVER_DEFAULT;
		TrackMouseEvent ( &tme );
	}

	//	Always returns FALSE to continue the process
	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXNC::OnNcMouseMove(CWnd *pWnd, UINT nHitTest, CPoint point)
{
	// TODO: Add Code Here
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
		TRACKMOUSEEVENT tme;
		ZeroMemory ( &tme, sizeof(tme) );
		tme.cbSize		= sizeof(tme);
		tme.hwndTrack	= pWnd->m_hWnd;
		tme.dwFlags		= TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
		tme.dwHoverTime	= 250; // HOVER_DEFAULT;
		TrackMouseEvent ( &tme );
	}

	//	Always returns FALSE to continue the process
	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXNC::DrawAllIcons (  CWnd *pWnd, UINT iconOnly )
{
	if ( iconOnly == ICON_NOT_SET )
	{
		return;
	}

	//
	DWORD dwStyle = pWnd->GetStyle();
	WINDOWPLACEMENT wp;
	pWnd->GetWindowPlacement( &wp );

	//
	CDC* pDC = pWnd->GetWindowDC();

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
BOOL CTRXNC::OnNcMouseHover(CWnd *pWnd, UINT nFlags, CPoint point)
{
	//	Always returns FALSE to continue the process
	if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
	{
		if ( m_iHover != IDI_CLOSE )
		{
			DrawAllIcons ( pWnd, m_iHover );

			CDC* pDC = pWnd->GetWindowDC();
			DrawIcon ( pDC, IDI_CLOSE, m_CloseRect, true, CTRXColors::GetRedCBrush() );
			pWnd->ReleaseDC ( pDC );
			m_iHover	= IDI_CLOSE;
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
				DrawIconFrame ( pDC, m_MaximizeRect, true, CTRXColors::GetCyanCBrush() );
			}
			pWnd->ReleaseDC ( pDC );
			m_iHover	= IDI_MAXIMIZE;
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
				DrawIconFrame ( pDC, m_MinimizeRect, true, CTRXColors::GetCyanCBrush() );
			}
			pWnd->ReleaseDC ( pDC );
			m_iHover	= IDI_MINIMIZE;
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
BOOL CTRXNC::OnNcMouseLeave(CWnd *pWnd )
{
	//	Always returns FALSE to continue the process
	DrawAllIcons ( pWnd, m_iHover );
	m_iHover = ICON_NOT_SET;

	return FALSE;
}