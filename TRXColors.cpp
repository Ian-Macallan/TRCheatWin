#include "stdafx.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static DWORD rgbBlack           = RGB(0x00,0x00,0x00);
static DWORD rgbWhite           = RGB(0xFF,0xFF,0xFF);
static DWORD rgbGrey            = RGB(0x80,0x80,0x80);
static DWORD rgbGreyC0          = RGB(0xC0,0xC0,0xC0);

static DWORD rgbRed             = RGB(0xFF,0x00,0x00);
static DWORD rgbGreen           = RGB(0x00,0xFF,0x00);
static DWORD rgbBlue            = RGB(0x00,0x00,0xFF);

static DWORD rgbCyan            = RGB(0x00,0xFF,0xFF);
static DWORD rgbMagenta         = RGB(0xFF,0x00,0xFF);
static DWORD rgbYellow          = RGB(0xFF,0xFF,0x00);

static DWORD rgbBlack10         = RGB(0x10,0x10,0x10);
static DWORD rgbBlack20         = RGB(0x20,0x20,0x20);
static DWORD rgbBlack30         = RGB(0x30,0x30,0x30);
static DWORD rgbBlack40         = RGB(0x40,0x40,0x40);
static DWORD rgbWhiteCF         = RGB(0xCF,0xCF,0xCF);
static DWORD rgbWhiteDF         = RGB(0xDF,0xDF,0xDF);
static DWORD rgbWhiteEF         = RGB(0xEF,0xEF,0xEF);

static COLORREF crBlack         = COLORREF(rgbBlack);
static COLORREF crWhite         = COLORREF(rgbWhite);
static COLORREF crGrey          = COLORREF(rgbGrey);
static COLORREF crGreyC0        = COLORREF(rgbGreyC0);

static COLORREF crRed           = COLORREF(rgbRed);
static COLORREF crGreen         = COLORREF(rgbGreen);
static COLORREF crBlue          = COLORREF(rgbBlue);

static COLORREF crCyan          = COLORREF(rgbCyan);
static COLORREF crMagenta       = COLORREF(rgbMagenta);
static COLORREF crYellow        = COLORREF(rgbYellow);

static COLORREF crBlack10       = COLORREF(rgbBlack10);
static COLORREF crBlack20       = COLORREF(rgbBlack20);
static COLORREF crBlack30       = COLORREF(rgbBlack30);
static COLORREF crBlack40       = COLORREF(rgbBlack40);
static COLORREF crWhiteCF       = COLORREF(rgbWhiteCF);
static COLORREF crWhiteDF       = COLORREF(rgbWhiteDF);
static COLORREF crWhiteEF       = COLORREF(rgbWhiteEF);


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CBrush CTRXColors::BlackBrush;
CBrush CTRXColors::WhiteBrush;
CBrush CTRXColors::GreyBrush;

CBrush CTRXColors::RedBrush;
CBrush CTRXColors::GreenBrush;
CBrush CTRXColors::BlueBrush;

CBrush CTRXColors::CyanBrush;
CBrush CTRXColors::MagentaBrush;
CBrush CTRXColors::YellowBrush;

CBrush CTRXColors::Grey192Brush;

CBrush CTRXColors::Black10Brush;
CBrush CTRXColors::Black20Brush;
CBrush CTRXColors::Black30Brush;
CBrush CTRXColors::Black40Brush;
CBrush CTRXColors::WhiteCFBrush;
CBrush CTRXColors::WhiteDFBrush;
CBrush CTRXColors::WhiteEFBrush;

//
/////////////////////////////////////////////////////////////////////////////
//  0 Default Color
//  1 Dark Client Area
//  2 Dark Client Area and Caption
int             CTRXColors::m_iDarkTheme        = 0;
int             CTRXColors::m_iSquareCorner     = 0;
int             CTRXColors::m_iSquareForce      = 0;
int             CTRXColors::m_iThemeRestart     = 0;


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXColors::CTRXColors(void)
{
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXColors::~CTRXColors(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXColors::Instanciate()
{
    BOOL bBrush;

    //
    bBrush = BlackBrush.CreateSolidBrush ( crBlack );
    if ( bBrush )
    {
        BlackBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteBrush.CreateSolidBrush ( crWhite );
    if ( bBrush )
    {
        WhiteBrush.UnrealizeObject ( );
    }

    //
    bBrush = GreyBrush.CreateSolidBrush ( crGrey );
    if ( bBrush )
    {
        GreyBrush.UnrealizeObject ( );
    }

    //
    bBrush = RedBrush.CreateSolidBrush ( crRed );
    if ( bBrush )
    {
        RedBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = GreenBrush.CreateSolidBrush ( crGreen );
    if ( bBrush )
    {
        GreenBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = BlueBrush.CreateSolidBrush ( crBlue );
    if ( bBrush )
    {
        BlueBrush.UnrealizeObject ( );
    }

    //
    bBrush = CyanBrush.CreateSolidBrush ( crCyan );
    if ( bBrush )
    {
        CyanBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = MagentaBrush.CreateSolidBrush ( crMagenta );
    if ( bBrush )
    {
        MagentaBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = YellowBrush.CreateSolidBrush ( crYellow );
    if ( bBrush )
    {
        YellowBrush.UnrealizeObject ( );
    }

    //
    bBrush = Grey192Brush.CreateSolidBrush ( crGreyC0 );
    if ( bBrush )
    {
        Grey192Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black10Brush.CreateSolidBrush ( crBlack10 );
    if ( bBrush )
    {
        Black10Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black20Brush.CreateSolidBrush ( crBlack20 );
    if ( bBrush )
    {
        Black20Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black30Brush.CreateSolidBrush ( crBlack30 );
    if ( bBrush )
    {
        Black30Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black40Brush.CreateSolidBrush ( crBlack40 );
    if ( bBrush )
    {
        Black40Brush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteCFBrush.CreateSolidBrush ( crWhiteCF );
    if ( bBrush )
    {
        WhiteCFBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteDFBrush.CreateSolidBrush ( crWhiteDF );
    if ( bBrush )
    {
        WhiteDFBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteEFBrush.CreateSolidBrush ( crWhiteEF );
    if ( bBrush )
    {
        WhiteEFBrush.UnrealizeObject ( );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXColors::GetBlackBrush()
{
    return ( HBRUSH ) BlackBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhiteBrush()
{
    return ( HBRUSH ) WhiteBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetGreyBrush()
{
    return ( HBRUSH ) GreyBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetRedBrush()
{
    return ( HBRUSH ) RedBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetGreenBrush()
{
    return ( HBRUSH ) GreenBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlueBrush()
{
    return ( HBRUSH ) BlueBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetCyanBrush()
{
    return ( HBRUSH ) CyanBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetMagentaBrush()
{
    return ( HBRUSH ) MagentaBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetYellowBrush()
{
    return ( HBRUSH ) YellowBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetGreyC0Brush()
{
    return ( HBRUSH ) Grey192Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack10Brush()
{
    return ( HBRUSH ) Black10Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack20Brush()
{
    return ( HBRUSH ) Black20Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack30Brush()
{
    return ( HBRUSH ) Black30Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack40Brush()
{
    return ( HBRUSH ) Black40Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhiteCFBrush()
{
    return ( HBRUSH ) WhiteCFBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhiteDFBrush()
{
    return ( HBRUSH ) WhiteDFBrush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhiteEFBrush()
{
    return ( HBRUSH ) WhiteEFBrush.GetSafeHandle ( );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CBrush *CTRXColors::GetBlackCBrush()
{
    return &BlackBrush;
}

CBrush *CTRXColors::GetWhiteCBrush()
{
    return &WhiteBrush;
}

CBrush *CTRXColors::GetGreyCBrush()
{
    return &GreyBrush;
}

CBrush *CTRXColors::GetRedCBrush()
{
    return &RedBrush;
}

CBrush *CTRXColors::GetGreenCBrush()
{
    return &GreenBrush;
}

CBrush *CTRXColors::GetBlueCBrush()
{
    return &BlueBrush;
}

CBrush *CTRXColors::GetCyanCBrush()
{
    return &CyanBrush;
}

CBrush *CTRXColors::GetMagentaCBrush()
{
    return &MagentaBrush;
}

CBrush *CTRXColors::GetYellowCBrush()
{
    return &YellowBrush;
}

CBrush *CTRXColors::GetGreyC0CBrush()
{
    return &Grey192Brush;
}

CBrush *CTRXColors::GetBlack10CBrush()
{
    return &Black10Brush;
}

CBrush *CTRXColors::GetBlack20CBrush()
{
    return &Black20Brush;
}

CBrush *CTRXColors::GetBlack30CBrush()
{
    return &Black30Brush;
}

CBrush *CTRXColors::GetBlack40CBrush()
{
    return &Black40Brush;
}

CBrush *CTRXColors::GetWhiteCFCBrush()
{
    return &WhiteCFBrush;
}

CBrush *CTRXColors::GetWhiteDFCBrush()
{
    return &WhiteDFBrush;
}

CBrush *CTRXColors::GetWhiteEFCBrush()
{
    return &WhiteEFBrush;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CTRXColors::GetBlackCR()
{
    return crBlack;
}

COLORREF CTRXColors::GetWhiteCR()
{
    return crWhite;
}

COLORREF CTRXColors::GetGreyCR()
{
    return crGrey;
}

COLORREF CTRXColors::GetRedCR()
{
    return crRed;
}

COLORREF CTRXColors::GetGreenCR()
{
    return crGreen;
}

COLORREF CTRXColors::GetBlueCR()
{
    return crBlue;
}

COLORREF CTRXColors::GetCyanCR()
{
    return crCyan;
}

COLORREF CTRXColors::GetMagentaCR()
{
    return crMagenta;
}

COLORREF CTRXColors::GetYellowCR()
{
    return crYellow;
}

COLORREF CTRXColors::GetGreyC0CR()
{
    return crGreyC0;
}

COLORREF CTRXColors::GetBlack10CR()
{
    return crBlack10;
}

COLORREF CTRXColors::GetBlack20CR()
{
    return crBlack20;
}

COLORREF CTRXColors::GetBlack30CR()
{
    return crBlack30;
}

COLORREF CTRXColors::GetBlack40CR()
{
    return crBlack40;
}

COLORREF CTRXColors::GetWhiteCFCR()
{
    return crWhiteCF;
}

COLORREF CTRXColors::GetWhiteDFCR()
{
    return crWhiteDF;
}

COLORREF CTRXColors::GetWhiteEFCR()
{
    return crWhiteEF;
}


//
/////////////////////////////////////////////////////////////////////////////
//
//  BUTTON, CLOCK, COMBOBOX, COMMUNICATIONS, CONTROLPANEL, DATEPICKER, DRAGDROP,
//  EDIT, EXPLORERBAR, FLYOUT, GLOBALS, HEADER, LISTBOX, LISTVIEW, MENU, MENUBAND,
//  NAVIGATION, PAGE, PROGRESS, REBAR, SCROLLBAR, SEARCHEDITBOX, SPIN, STARTPANEL,
//  STATUS, TAB, TASKBAND, TASKBAR, TASKDIALOG, TEXTSTYLE, TOOLBAR, TOOLTIP,
//  TRACKBAR, TRAYNOTIFY, TREEVIEW, WINDOW
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXColors::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //
    HBRUSH hBrush = NULL;

    if ( CTRXColors::m_iDarkTheme == 0 )
    {
        return hBrush;
    }

    if ( pDC == NULL )
    {
        return hBrush;
    }

    //
    switch(nCtlColor)
    {
        case CTLCOLOR_MSGBOX :
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_EDIT:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack10CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack10Brush();
            break;
        }

        case CTLCOLOR_LISTBOX:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack20Brush();
            break;
        }

        case CTLCOLOR_BTN:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_DLG:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_SCROLLBAR:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }

        //  Static And Read Only Edit
        case CTLCOLOR_STATIC:
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            //  Transparent makes scroll garbage
            // pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }

        default :
        {
            pDC->SetTextColor(CTRXColors::GetWhiteCR());
            pDC->SetBkColor(CTRXColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CTRXColors::GetBlack30Brush();
            break;
        }
    }

    //
    return hBrush;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXColors::OnEraseBkgnd(CDC* pDC, CWnd* pWnd)
{
    if ( CTRXColors::m_iDarkTheme == 0 )
    {
        return FALSE;
    }

    if ( pDC == NULL )
    {
        return FALSE;
    }

    //
    RECT rectFrame;
    pWnd->GetClientRect ( &rectFrame );

    //
    pDC->FillRect ( &rectFrame, CTRXColors::GetBlack30CBrush() );

    //
    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXColors::SetWindowTheme ( CWnd *pWnd )
{
    if ( CTRXColors::m_iSquareCorner == 1 )
    {
        if ( CTRXColors::m_iDarkTheme == 2 )
        {
            ::SetWindowTheme ( pWnd->GetSafeHwnd(), WINDOWS_THEME_SQUARE  );
        }
        else
        {
            ::SetWindowTheme ( pWnd->GetSafeHwnd(), WINDOWS_THEME_NULL  );
        }
    }
    else
    {
        ::SetWindowTheme ( pWnd->GetSafeHwnd(), WINDOWS_THEME_NULL  );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  Public
/////////////////////////////////////////////////////////////////////////////
//
//  Foreground COLORREF
COLORREF CTRXColors::GetFGNormalCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCR();
    }
    else
    {
        return GetBlack20CR();
    }
}

COLORREF CTRXColors::GetFGSelectedCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCR();
    }
    else
    {
        return GetBlackCR();
    }
}


COLORREF CTRXColors::GetFGDisabledCR ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyCR();
    }
    else
    {
        return GetGreyCR();
    }
}

COLORREF CTRXColors::GetFGMenuCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCR();
    }
    else
    {
        return GetBlack20CR();
    }
}

COLORREF CTRXColors::GetFGMenuSelectedCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCR();
    }
    else
    {
        return GetBlackCR();
    }
}

COLORREF CTRXColors::GetFGHeaderCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFCR();
    }
    else
    {
        return GetBlack10CR();
    }
}

//
//  Background COLORREF
COLORREF CTRXColors::GetBKNormalCR ( bool bDark )
{
    return GetFGNormalCR ( ! bDark );
}

COLORREF CTRXColors::GetBKSelectedCR ( bool bDark )
{
    return GetFGSelectedCR ( ! bDark );
}

COLORREF CTRXColors::GetBKDisabledCR ( bool bDark )
{
    return GetFGDisabledCR ( ! bDark );
}

COLORREF CTRXColors::GetBKMenuCR ( bool bDark )
{
    return GetFGMenuCR ( ! bDark );
}

COLORREF CTRXColors::GetBKMenuSelectedCR ( bool bDark )
{
    return GetFGMenuSelectedCR ( ! bDark );
}

COLORREF CTRXColors::GetBKHeaderCR ( bool bDark )
{
    return GetFGHeaderCR ( ! bDark );
}

//  Foreground CBrush
CBrush * CTRXColors::GetFGNormalCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCBrush();
    }
    else
    {
        return GetBlack20CBrush();
    }
}

CBrush * CTRXColors::GetFGSelectedCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCBrush();
    }
    else
    {
        return GetBlackCBrush();
    }
}

CBrush * CTRXColors::GetFGDisabledCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyCBrush();
    }
    else
    {
        return GetGreyCBrush();
    }
}

CBrush * CTRXColors::GetFGMenuCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCBrush();
    }
    else
    {
        return GetBlack20CBrush();
    }
}

CBrush * CTRXColors::GetFGMenuSelectedCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCBrush();
    }
    else
    {
        return GetBlackCBrush();
    }
}

CBrush * CTRXColors::GetFGHeaderCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFCBrush();
    }
    else
    {
        return GetBlack10CBrush();
    }
}

//
CBrush * CTRXColors::GetBKNormalCBrush ( bool bDark )
{
    return GetFGNormalCBrush ( ! bDark );
}

CBrush * CTRXColors::GetBKSelectedCBrush( bool bDark )
{
    return GetFGSelectedCBrush ( ! bDark );
}

CBrush * CTRXColors::GetBKDisabledCBrush ( bool bDark )
{
    return GetFGDisabledCBrush ( ! bDark );
}

CBrush * CTRXColors::GetBKMenuCBrush( bool bDark )
{
    return GetFGMenuCBrush ( ! bDark );
}

CBrush * CTRXColors::GetBKMenuSelectedCBrush( bool bDark )
{
    return GetFGMenuSelectedCBrush ( ! bDark );
}

CBrush * CTRXColors::GetBKHeaderCBrush( bool bDark )
{
    return GetFGHeaderCBrush ( ! bDark );
}

//  Foreground HBRUSH
HBRUSH CTRXColors::GetFGNormalBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFBrush();
    }
    else
    {
        return GetBlack20Brush();
    }
}

HBRUSH CTRXColors::GetFGSelectedBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteBrush();
    }
    else
    {
        return GetBlackBrush();
    }
}

HBRUSH CTRXColors::GetFGDisabledBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyBrush();
    }
    else
    {
        return GetGreyBrush();
    }
}

HBRUSH CTRXColors::GetFGMenuBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFBrush();
    }
    else
    {
        return GetBlack20Brush();
    }
}

HBRUSH CTRXColors::GetFGMenuSelectedBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteBrush();
    }
    else
    {
        return GetBlackBrush();
    }
}

HBRUSH CTRXColors::GetFGHeaderBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFBrush();
    }
    else
    {
        return GetBlack10Brush();
    }
}

//
HBRUSH CTRXColors::GetBKNormalBrush ( bool bDark )
{
    return GetFGNormalBrush ( ! bDark );
}

HBRUSH CTRXColors::GetBKSelectedBrush( bool bDark )
{
    return GetFGSelectedBrush ( ! bDark );
}

HBRUSH CTRXColors::GetBKDisabledBrush ( bool bDark )
{
    return GetFGDisabledBrush ( ! bDark );
}

HBRUSH CTRXColors::GetBKMenuBrush( bool bDark )
{
    return GetFGMenuBrush ( ! bDark );
}

HBRUSH CTRXColors::GetBKMenuSelectedBrush( bool bDark )
{
    return GetFGMenuSelectedBrush ( ! bDark );
}

HBRUSH CTRXColors::GetBKHeaderBrush( bool bDark )
{
    return GetFGHeaderBrush ( ! bDark );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CTRXColors::GetFGTipColor(bool bDark)
{
    if ( bDark )
    {
        return CTRXColors::GetWhiteCR();
    }
    else
    {
        return NULL;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CTRXColors::GetBKTipColor(bool bDark)
{
    if ( bDark )
    {
        return CTRXColors::GetBlackCR();
    }
    else
    {
        return NULL;
    }
}


