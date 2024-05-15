#include "stdafx.h"
#include "TRXColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static DWORD rgbBlack			= RGB(0,0,0);
static DWORD rgbWhite			= RGB(255,255,255);
static DWORD rgbGrey			= RGB(128,128,128);
static DWORD rgbGrey192			= RGB(192,192,192);

static DWORD rgbRed				= RGB(255,0,0);
static DWORD rgbGreen			= RGB(0,255,0);
static DWORD rgbBlue			= RGB(0,0,255);

static DWORD rgbCyan			= RGB(0,255,255);
static DWORD rgbMagenta			= RGB(255,0,255);
static DWORD rgbYellow			= RGB(255,255,0);

static DWORD rgbBlack32			= RGB(32,32,32);
static DWORD rgbBlack48			= RGB(48,48,48);
static DWORD rgbBlack64			= RGB(64,64,64);
static DWORD rgbWhite208		= RGB(208,208,208);
static DWORD rgbWhite224		= RGB(224,224,224);

static COLORREF crBlack			= COLORREF(rgbBlack);
static COLORREF crWhite			= COLORREF(rgbWhite);
static COLORREF crGrey			= COLORREF(rgbGrey);
static COLORREF crGrey192		= COLORREF(rgbGrey192);

static COLORREF crRed			= COLORREF(rgbRed);
static COLORREF crGreen			= COLORREF(rgbGreen);
static COLORREF crBlue			= COLORREF(rgbBlue);

static COLORREF crCyan			= COLORREF(rgbCyan);
static COLORREF crMagenta		= COLORREF(rgbMagenta);
static COLORREF crYellow		= COLORREF(rgbYellow);

static COLORREF crBlack32			= COLORREF(rgbBlack32);
static COLORREF crBlack48		= COLORREF(rgbBlack48);
static COLORREF crBlack64		= COLORREF(rgbBlack64);
static COLORREF crWhite208		= COLORREF(rgbWhite208);
static COLORREF crWhite224		= COLORREF(rgbWhite224);


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

CBrush CTRXColors::Black32Brush;
CBrush CTRXColors::Black48Brush;
CBrush CTRXColors::Black64Brush;
CBrush CTRXColors::White208Brush;
CBrush CTRXColors::White224Brush;

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
	bBrush = Grey192Brush.CreateSolidBrush ( crGrey192 );
	if ( bBrush )
	{
		Grey192Brush.UnrealizeObject ( );
	}

	//
	bBrush = Black32Brush.CreateSolidBrush ( crBlack32 );
	if ( bBrush )
	{
		Black32Brush.UnrealizeObject ( );
	}

	//
	bBrush = Black48Brush.CreateSolidBrush ( crBlack48 );
	if ( bBrush )
	{
		Black48Brush.UnrealizeObject ( );
	}

	//
	bBrush = Black64Brush.CreateSolidBrush ( crBlack64 );
	if ( bBrush )
	{
		Black64Brush.UnrealizeObject ( );
	}

	//
	bBrush = White208Brush.CreateSolidBrush ( crWhite208 );
	if ( bBrush )
	{
		White208Brush.UnrealizeObject ( );
	}

	//
	bBrush = White224Brush.CreateSolidBrush ( crWhite224 );
	if ( bBrush )
	{
		White224Brush.UnrealizeObject ( );
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

HBRUSH CTRXColors::GetGrey192Brush()
{
	return ( HBRUSH ) Grey192Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack32Brush()
{
	return ( HBRUSH ) Black32Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack48Brush()
{
	return ( HBRUSH ) Black48Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetBlack64Brush()
{
	return ( HBRUSH ) Black64Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhite208Brush()
{
	return ( HBRUSH ) White208Brush.GetSafeHandle ( );
}

HBRUSH CTRXColors::GetWhite224Brush()
{
	return ( HBRUSH ) White224Brush.GetSafeHandle ( );
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

CBrush *CTRXColors::GetGrey192CBrush()
{
	return &Grey192Brush;
}

CBrush *CTRXColors::GetBlack32CBrush()
{
	return &Black32Brush;
}

CBrush *CTRXColors::GetBlack48CBrush()
{
	return &Black48Brush;
}

CBrush *CTRXColors::GetBlack64CBrush()
{
	return &Black64Brush;
}

CBrush *CTRXColors::GetWhite208CBrush()
{
	return &White208Brush;
}

CBrush *CTRXColors::GetWhite224CBrush()
{
	return &White224Brush;
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

COLORREF CTRXColors::GetGrey192CR()
{
	return crGrey192;
}

COLORREF CTRXColors::GetBlack32CR()
{
	return crBlack32;
}

COLORREF CTRXColors::GetBlack48CR()
{
	return crBlack48;
}

COLORREF CTRXColors::GetBlack64CR()
{
	return crBlack64;
}

COLORREF CTRXColors::GetWhite208CR()
{
	return crWhite208;
}

COLORREF CTRXColors::GetWhite224CR()
{
	return crWhite224;
}

