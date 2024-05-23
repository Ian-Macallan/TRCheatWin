#pragma once

#include <Windows.h>
#include <afx.h>
#include <afxwin.h>

#define ROOM_RGB_BLACK          RGB(0,0,0)
#define ROOM_RGB_WHITE          RGB(255,255,255)
#define ROOM_RGB_GREY           RGB(128,128,128)

#define ROOM_RGB_RED_DK         RGB(192,128,128)
#define ROOM_RGB_GREEN_DK       RGB(128,192,128)
#define ROOM_RGB_BLUE_DK        RGB(128,128,192)

#define ROOM_RGB_RED_BR         RGB(255,64,64)
#define ROOM_RGB_GREEN_BR       RGB(64,255,64)
#define ROOM_RGB_BLUE_BR        RGB(64,64,255)

#define ROOM_BLUE_WATER         RGB(42,157,244)
#define ROOM_QUICKSAND          RGB(194,161,149)
#define ROOM_STONE              RGB(192,192,192)
#define ROOM_NORMAL             RGB(204,255,153)

#define ROOM_RGB_RED            RGB(255,0,0)
#define ROOM_RGB_GREEN          RGB(0,255,0)
#define ROOM_RGB_BLUE           RGB(0,0,255)

#define ROOM_RGB_BROWN          RGB(153,76,0)
#define ROOM_RGB_OCEAN          RGB(0,157,196)
#define ROOM_RGB_CYAN           RGB(0,255,255)

#define ROOM_CR_RED_DK          COLORREF(ROOM_RGB_RED_DK)
#define ROOM_CR_GREEN_DK        COLORREF(ROOM_RGB_GREEN_DK)
#define ROOM_CR_BLUE_DK         COLORREF(ROOM_RGB_BLUE_DK)

#define ROOM_CR_RED_BR          COLORREF(ROOM_RGB_RED_BR)
#define ROOM_CR_GREEN_BR        COLORREF(ROOM_RGB_GREEN_BR)
#define ROOM_CR_BLUE_BR         COLORREF(ROOM_RGB_BLUE_BR)

#define ROOM_CR_RED             COLORREF(ROOM_RGB_RED)
#define ROOM_CR_GREEN           COLORREF(ROOM_RGB_GREEN)
#define ROOM_CR_BLUE            COLORREF(ROOM_RGB_BLUE)

#define ROOM_CR_BROWN           COLORREF(ROOM_RGB_BROWN)
#define ROOM_CR_OCEAN           COLORREF(ROOM_RGB_OCEAN)
#define ROOM_CR_CYAN            COLORREF(ROOM_RGB_CYAN)

#define ROOM_CR_BLUE_WATER      COLORREF(ROOM_BLUE_WATER)
#define ROOM_CR_QUICKSAND       COLORREF(ROOM_QUICKSAND)
#define ROOM_CR_STONE           COLORREF(ROOM_STONE)
#define ROOM_CR_NORMAL          COLORREF(ROOM_NORMAL)

//
/////////////////////////////////////////////////////////////////////////////
// CTRXRoomWall
//
/////////////////////////////////////////////////////////////////////////////
class CTRXRoomWall : public CStatic
{
        DECLARE_DYNAMIC(CTRXRoomWall)

    public:
        CTRXRoomWall();
        virtual ~CTRXRoomWall();

        //  Point in Client Coordinates
        void SetRoomPoint( CPoint pt, const double *pOrientation = NULL );

        //
        void SetOrientation ( double orientation );
        void SetSelected ( BOOL bSelected );
        void SetPointMode ( BOOL bPointMode );
        void SetClickable ( BOOL bClickable );
        void SetClickable ( BOOL bClickable, BOOL bMoveMouse, BOOL bSetPoint );

    protected :
        char        m_szText [ 256 ];
        BOOL        m_bSelected;
        BOOL        m_bPointMode;
        BOOL        m_bPointSet;

        BOOL        m_bClickable;
        BOOL        m_bMoveMouse;
        BOOL        m_bSetPoint;

        CPoint      m_Point;

        
        /////////////////////////////////
        //           180               //
        //          North              //
        //                             //
        //  270                  90    //
        //  West                East   //
        //                             //
        //           0                 //
        //         South               //
        /////////////////////////////////
        double      m_Orientation;
    protected:
        DECLARE_MESSAGE_MAP()
    public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );

        virtual BOOL PreTranslateMessage(MSG* pMsg);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnStnDblclick();
        afx_msg void OnStnClicked();
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


