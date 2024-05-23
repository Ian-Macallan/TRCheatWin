#pragma once

#include <Windows.h>

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXColors
{
    public:
        CTRXColors(void);
        virtual ~CTRXColors(void);

        //
        static HBRUSH GetBlackBrush();
        static HBRUSH GetWhiteBrush();
        static HBRUSH GetGreyBrush();

        static HBRUSH GetRedBrush();
        static HBRUSH GetGreenBrush();
        static HBRUSH GetBlueBrush();

        static HBRUSH GetCyanBrush();
        static HBRUSH GetMagentaBrush();
        static HBRUSH GetYellowBrush();

        static HBRUSH GetGrey192Brush();

        static HBRUSH GetBlack32Brush();
        static HBRUSH GetBlack48Brush();
        static HBRUSH GetBlack64Brush();
        static HBRUSH GetWhite208Brush();
        static HBRUSH GetWhite224Brush();

        //
        static CBrush *GetBlackCBrush();
        static CBrush *GetWhiteCBrush();
        static CBrush *GetGreyCBrush();

        static CBrush *GetRedCBrush();
        static CBrush *GetGreenCBrush();
        static CBrush *GetBlueCBrush();

        static CBrush *GetCyanCBrush();
        static CBrush *GetMagentaCBrush();
        static CBrush *GetYellowCBrush();

        static CBrush *GetGrey192CBrush();

        static CBrush *GetBlack32CBrush();
        static CBrush *GetBlack48CBrush();
        static CBrush *GetBlack64CBrush();
        static CBrush *GetWhite208CBrush();
        static CBrush *GetWhite224CBrush();

        //
        static COLORREF GetBlackCR();
        static COLORREF GetWhiteCR();
        static COLORREF GetGreyCR();

        static COLORREF GetRedCR();
        static COLORREF GetGreenCR();
        static COLORREF GetBlueCR();

        static COLORREF GetCyanCR();
        static COLORREF GetMagentaCR();
        static COLORREF GetYellowCR();

        static COLORREF GetGrey192CR();

        static COLORREF GetBlack32CR();
        static COLORREF GetBlack48CR();
        static COLORREF GetBlack64CR();
        static COLORREF GetWhite208CR();
        static COLORREF GetWhite224CR();

    public :
        static void Instanciate();

        //
    protected :
        static CBrush BlackBrush;
        static CBrush WhiteBrush;
        static CBrush GreyBrush;

        static CBrush RedBrush;
        static CBrush GreenBrush;
        static CBrush BlueBrush;

        static CBrush CyanBrush;
        static CBrush MagentaBrush;
        static CBrush YellowBrush;

        static CBrush Grey192Brush;

        static CBrush Black32Brush;
        static CBrush Black48Brush;
        static CBrush Black64Brush;
        static CBrush White208Brush;
        static CBrush White224Brush;
};

