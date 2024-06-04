#pragma once

#include <Windows.h>

//
/////////////////////////////////////////////////////////////////////////////
//  See Also https://www.codeproject.com/Articles/620045/Custom-Controls-in-Win-API-Visual-Styles
/////////////////////////////////////////////////////////////////////////////
#define WINDOWS_THEME_NULL      NULL,NULL
#define WINDOWS_THEME_EXPLORER  L"Explorer",NULL
#define WINDOWS_THEME_OLD       L"",L""

#define WINDOWS_THEME_RESET     WINDOWS_THEME_EXPLORER

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXColors
{
    public:
        CTRXColors(void);
        virtual ~CTRXColors(void);

    public :
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

        //
        static HBRUSH GetBlackBrush();
        static HBRUSH GetWhiteBrush();
        static HBRUSH GetGreyBrush();

        static HBRUSH GetCyanBrush();
        static HBRUSH GetMagentaBrush();
        static HBRUSH GetYellowBrush();

        static HBRUSH GetRedBrush();
        static HBRUSH GetGreenBrush();
        static HBRUSH GetBlueBrush();

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

        //
    protected:
        static HBRUSH GetGreyC0Brush();

        static HBRUSH GetBlack10Brush();
        static HBRUSH GetBlack20Brush();
        static HBRUSH GetBlack30Brush();
        static HBRUSH GetBlack40Brush();
        static HBRUSH GetWhiteCFBrush();
        static HBRUSH GetWhiteDFBrush();
        static HBRUSH GetWhiteEFBrush();


        static CBrush *GetGreyC0CBrush();

        static CBrush *GetBlack10CBrush();
        static CBrush *GetBlack20CBrush();
        static CBrush *GetBlack30CBrush();
        static CBrush *GetBlack40CBrush();
        static CBrush *GetWhiteCFCBrush();
        static CBrush *GetWhiteDFCBrush();
        static CBrush *GetWhiteEFCBrush();

        //
        static COLORREF GetGreyC0CR();

        static COLORREF GetBlack10CR();
        static COLORREF GetBlack20CR();
        static COLORREF GetBlack30CR();
        static COLORREF GetBlack40CR();
        static COLORREF GetWhiteCFCR();
        static COLORREF GetWhiteDFCR();
        static COLORREF GetWhiteEFCR();


    //
    public :
        //  COLORREF 
        static COLORREF GetFGNormalCR ( bool bDark );
        static COLORREF GetFGSelectedCR ( bool bDark );
        static COLORREF GetFGDisabledCR ( bool bDark );
        static COLORREF GetFGMenuCR ( bool bDark );
        static COLORREF GetFGMenuSelectedCR ( bool bDark );
        static COLORREF GetFGHeaderCR ( bool bDark );

        //
        static COLORREF GetBKNormalCR ( bool bDark );
        static COLORREF GetBKSelectedCR ( bool bDark );
        static COLORREF GetBKDisabledCR ( bool bDark );
        static COLORREF GetBKMenuCR ( bool bDark );
        static COLORREF GetBKMenuSelectedCR ( bool bDark );
        static COLORREF GetBKHeaderCR ( bool bDark );

        //  CBrush
        static CBrush * GetFGNormalCBrush ( bool bDark );
        static CBrush * GetFGSelectedCBrush ( bool bDark );
        static CBrush * GetFGDisabledCBrush ( bool bDark );
        static CBrush * GetFGMenuCBrush ( bool bDark );
        static CBrush * GetFGMenuSelectedCBrush ( bool bDark );
        static CBrush * GetFGHeaderCBrush ( bool bDark );

        //
        static CBrush * GetBKNormalCBrush ( bool bDark );
        static CBrush * GetBKSelectedCBrush ( bool bDark );
        static CBrush * GetBKDisabledCBrush ( bool bDark );
        static CBrush * GetBKMenuCBrush ( bool bDark );
        static CBrush * GetBKMenuSelectedCBrush ( bool bDark );
        static CBrush * GetBKHeaderCBrush ( bool bDark );

        //  HBRUSH
        static HBRUSH GetFGNormalBrush ( bool bDark );
        static HBRUSH GetFGSelectedBrush ( bool bDark );
        static HBRUSH GetFGDisabledBrush ( bool bDark );
        static HBRUSH GetFGMenuBrush ( bool bDark );
        static HBRUSH GetFGMenuSelectedBrush ( bool bDark );
        static HBRUSH GetFGHeaderBrush ( bool bDark );

        //
        static HBRUSH GetBKNormalBrush ( bool bDark );
        static HBRUSH GetBKSelectedBrush ( bool bDark );
        static HBRUSH GetBKDisabledBrush ( bool bDark );
        static HBRUSH GetBKMenuBrush ( bool bDark );
        static HBRUSH GetBKMenuSelectedBrush ( bool bDark );
        static HBRUSH GetBKHeaderBrush ( bool bDark );

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

        static CBrush Black10Brush;
        static CBrush Black20Brush;
        static CBrush Black30Brush;
        static CBrush Black40Brush;
        static CBrush WhiteCFBrush;
        static CBrush WhiteDFBrush;
        static CBrush WhiteEFBrush;

    public :
        static HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        static BOOL OnEraseBkgnd(CDC* pDC, CWnd* pWnd);

        static COLORREF GetTextColor();
        static COLORREF GetBkColor();

        static void SetWindowTheme ( CWnd *pWnd );

};

