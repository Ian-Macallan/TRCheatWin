// TRXWallPicture.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXWallPicture.h"
#include "TR_Areas.h"
#include "TR9SaveGame.h"
#include "TRXGDI.h"

static const int RoomDivider    = 150;
static const int RoomMargin     = 10;

//
/////////////////////////////////////////////////////////////////////////////
// CTRXWallPicture
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTRXWallPicture, CTRXRoomWall)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXWallPicture::CTRXWallPicture()
{
    ZeroMemory ( &m_Area, sizeof(m_Area) );
    m_bAreaSet      = false;
    m_bPositionSet  = false;
    m_Wall          = WallNorth;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXWallPicture::~CTRXWallPicture()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXWallPicture::CreateInstanceInside( CWnd *parent, CTRXWallPicture *pRoom, RECT &rect,
                                            const char *pText, DWORD dwStyle)
{
    if ( parent == NULL || pRoom == NULL )
    {
        return FALSE;
    }

    //
    dwStyle |= SS_OWNERDRAW;
    BOOL bCreated = pRoom->Create (
        pText,                          //  LPCTSTR lpszCaption,
        dwStyle,                        //  DWORD dwStyle,
        rect,                           //  const RECT& rect,
        parent,                         //  CWnd* pParentWnd,
        -1                              //  UINT nID
    );

    return bCreated;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXWallPicture::CreateInstanceInside( CWnd *parent, CTRXWallPicture *pRoom, CStatic *pFrame,
                                            const char *pText, DWORD dwStyle)
{
    if ( parent == NULL || pRoom == NULL || pFrame == NULL )
    {
        return FALSE;
    }

    RECT frameRect;
    pFrame->GetWindowRect ( &frameRect );
    parent->ScreenToClient( &frameRect );

    //
    BOOL bCreated = CreateInstanceInside ( parent, pRoom, frameRect, pText, dwStyle );

    return bCreated;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXWallPicture::CreateInstanceBelow ( CWnd *parent, CTRXWallPicture *pRoom, CWnd *pFrame,
                                            const char *pText, DWORD dwStyle )
{
    if ( parent == NULL || pRoom == NULL || pFrame == NULL )
    {
        return FALSE;
    }

    RECT frameRect;
    pFrame->GetWindowRect ( &frameRect );
    parent->ScreenToClient ( &frameRect );

    RECT roomRect;
    roomRect.top        = frameRect.bottom + RoomMargin;
    roomRect.left       = frameRect.left + RoomMargin;
    roomRect.bottom     = roomRect.top + pRoom->m_Area.xSectors * TR_SECTOR_SIZE / RoomDivider;
    roomRect.right      = roomRect.left + pRoom->m_Area.zSectors * TR_SECTOR_SIZE / RoomDivider;

    BOOL bCreated = CreateInstanceInside ( parent, pRoom, frameRect, pText, dwStyle );

    return bCreated;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXWallPicture, CTRXRoomWall)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXWallPicture
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXWallPicture::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if ( pDC )
    {

        // This code only works with buttons.
        ASSERT(lpDrawItemStruct->CtlType == ODT_STATIC);

        // If drawing selected, add the pushed style to DrawFrameControl.
        // Draw the button frame.
        CTRXPen pen;
        int penWidth    = 1;
        if ( m_bSelected )
        {
            penWidth    = 2;
            pen.CreatePen(PS_SOLID, penWidth, ROOM_RGB_RED);
        }
        else
        {
            pen.CreatePen(PS_SOLID, penWidth, ROOM_RGB_BLACK);
        }

        //
        CPen* pOldPen = pDC->SelectObject(&pen);

        //  Rectangle
        pDC->Rectangle ( &lpDrawItemStruct->rcItem );
        
        //  Fill
        RECT rect       = lpDrawItemStruct->rcItem;
        rect.left       += penWidth;
        rect.right      -= penWidth;
        rect.top        += penWidth;
        rect.bottom     -= penWidth;

        // Draw the button text using the text color white.
        COLORREF crOldColor;

        //
        CTRXBrush brush;
        COLORREF cr;
        DWORD   rgb;

        //
        if ( ( m_Area.flags & 0x01 ) == 0x01 )      // Bit 0 Full Of Water
        {
            cr  = ROOM_CR_BLUE_WATER;
            rgb = ROOM_RGB_WHITE;
        }
        else if ( ( m_Area.flags & 0x08) == 0x08 )  //  Bit 3 Open Area
        {
            cr  = ROOM_CR_GREEN;
            rgb = ROOM_RGB_BLACK;
        }
        else if ( ( m_Area.flags & 0x80 ) == 0x80 )     //  Bit 7 Quick Sand
        {
            cr  = ROOM_CR_QUICKSAND;
            rgb = ROOM_RGB_BLACK;
        }
        else if ( ( m_Area.flags & 0x100 ) == 0x100 )       //  Bit 8 Open Area
        {
            cr  = ROOM_CR_GREEN;
            rgb = ROOM_RGB_BLACK;
        }
        else if ( ( m_Area.flags & 0x200 ) == 0x200 )   //  Bit 9 Some Water
        {
            cr  = ROOM_CR_GREEN_BR;
            rgb = ROOM_RGB_BLACK;
        }
        else
        {
            cr  = ROOM_CR_NORMAL;
            rgb = ROOM_RGB_BLACK;
        }

        //
        if ( abs(m_Area.yBottom - m_Area.yTop) < LOW_CEILING )
        {
            BYTE r      = cr & 0xFF;
            BYTE g      = ( cr >> 8 ) & 0xFF;
            BYTE b      = ( cr >> 16 ) & 0xFF;
            r = ( BYTE ) ( (double) r * 0.8 );
            g = ( BYTE ) ( (double) g * 0.8 );
            b = ( BYTE ) ( (double) b * 0.8 );
            cr          = COLORREF(RGB(r,g,b));
        }
        else if ( abs(m_Area.yBottom - m_Area.yTop) > HIGH_CEILING )
        {
            BYTE r      = cr & 0xFF;
            BYTE g      = ( cr >> 8 ) & 0xFF;
            BYTE b      = ( cr >> 16 ) & 0xFF;
            if ( r < 0xf0 )
            {
                r = ( BYTE ) ( (double) r * 1.1 );
            }
            if ( g < 0xf0 )
            {
                g = ( BYTE ) ( (double) g * 1.1 );
            }
            if ( b < 0xf0 )
            {
                b = ( BYTE ) ( (double) b * 1.1 );
            }
            cr          = COLORREF(RGB(r,g,b));
        }

        //
        brush.CreateSolidBrush ( cr );
        crOldColor = pDC->SetTextColor ( rgb );

        //
        pDC->FillRect ( &rect, &brush );
        brush.DeleteObject();

        pDC->SelectObject(pOldPen);
        pen.DeleteObject();

        // Get the button's text.
        GetWindowText ( m_szText, sizeof(m_szText) );

        if ( lpDrawItemStruct->itemState & ODS_DISABLED )
        {
            //
        }

        //
        pDC->SetBkMode( TRANSPARENT );
        pDC->DrawText ( m_szText, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
        pDC->SetTextColor( crOldColor);

        //  Draw Orientation
        //  For Vertical View y is the base of the cross
        //  Center of the cross is x,y -3
        int xCenter = m_Point.x;
        int yCenter = m_Point.y - lenCross - 1;
        if ( m_bPointMode )
        {
            //
            int iOrientation    = (int) m_Orientation % 360;
            m_Orientation       = (float) ( iOrientation );

            //  Line
            //  Top
            DrawCross ( pDC, xCenter, yCenter, lenCross );

            float quarter = 90.0;

            //  Orientation From the center
            //  Looking West : 90
            if ( m_Orientation >= 0.5 * quarter  && m_Orientation < 1.5 * quarter )
            {
                //  Wiew from South
                if ( m_Wall == WallSouth )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  Wiew from North
                else if ( m_Wall == WallNorth )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Front
                else if ( m_Wall == WallWest )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Back
                else if ( m_Wall == WallEast )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                }
            }
            //  Facing North : 180
            else if ( m_Orientation >= 1.5 * quarter && m_Orientation < 2.5 * quarter )
            {
                //  View from East
                if ( m_Wall == WallEast )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View from West
                else if ( m_Wall == WallWest )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Back
                else if ( m_Wall == WallSouth )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Front
                else if ( m_Wall == WallNorth )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                }
            }
            //  Looking East : 270
            else if ( m_Orientation >= 2.5 * quarter && m_Orientation < 3.5 * quarter )
            {
                //  Wiew from South
                if ( m_Wall == WallSouth )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  Wiew from North
                else if ( m_Wall == WallNorth )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Front
                else if ( m_Wall == WallEast)
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Back
                else if ( m_Wall == WallWest )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                }
            }
            //  Facing South : 0
            else if ( m_Orientation >= 3.5 * quarter || m_Orientation < 0.5 * quarter )
            {
                //  View from East
                if ( m_Wall == WallEast )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View from West
                else if ( m_Wall == WallWest )
                {
                    if ( m_bTombraider123 )
                    {
                        DrawArrowRight ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowLeft ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Back
                else if ( m_Wall == WallNorth )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                }
                //  View From Front
                else if ( m_Wall == WallSouth )
                {
                    //  From Left Middle
                    if ( m_bTombraider123 )
                    {
                        DrawArrowDown ( pDC, xCenter, yCenter, lenCross );
                    }
                    else
                    {
                        DrawArrowUp ( pDC, xCenter, yCenter, lenCross );
                    }
                }
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXWallPicture::SetAreaAndPosition ( const TR_AREA *pArea, const TR_CUR_POSITION *position )
{
    if ( pArea != NULL && position != NULL )
    {
        m_Area              = *pArea;
        m_CurrentPosition   = *position;
        m_bAreaSet          = true;
        m_bPositionSet      = true;
    }
    else if ( pArea != NULL )
    {
        m_Area              = *pArea;
        ZeroMemory ( &m_CurrentPosition, sizeof(m_CurrentPosition) );
        m_bAreaSet          = true;
        m_bPositionSet      = false;
    }
    else
    {
        ZeroMemory ( &m_Area, sizeof(m_Area) );
        ZeroMemory ( &m_CurrentPosition, sizeof(m_CurrentPosition) );
        m_bAreaSet          = false;
        m_bPositionSet      = false;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXWallPicture::SetWall ( int iTombraider, WallDirectionEnum eWall )
{
    m_iTombraider   = iTombraider;
    m_Wall          = eWall;
    switch ( m_iTombraider )
    {
        //
        case GAME_TRR1 :
        case GAME_TR10 :
        case GAME_TR15 :
        case GAME_TRR2 :
        case GAME_TR20 :
        case GAME_TR25 :
        case GAME_TRR3 :
        case GAME_TR30 :
        case GAME_TR35 :
        {
            m_bTombraider123    = TRUE;
            break;
        }

        //
        default :
        {
            m_bTombraider123    = FALSE;
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const TR_AREA *CTRXWallPicture::GetArea()
{
    return &m_Area;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXWallPicture::ComputeRoomPoint ( double dfX, double dfY, double dfZ )
{
    CPoint point(0,0);

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 && m_Area.yTop != m_Area.yBottom )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        //  areaX West/East correspond to y
        long areaX =  ( long ) ( dfX * POSITION_DIVIDER ) - m_Area.x;
        //
        long areaY =  abs(m_Area.yTop - m_Area.yBottom) - abs ( ( long ) ( dfY * POSITION_DIVIDER ) - m_Area.yBottom );
        //  areaZ South/North correspond to x
        long areaZ =  ( long ) ( dfZ * POSITION_DIVIDER ) - m_Area.z;

        long maxX = m_Area.xSectors * TR_SECTOR_SIZE;
        long maxY = abs ( m_Area.yTop - m_Area.yBottom );
        long maxZ = m_Area.zSectors * TR_SECTOR_SIZE;

        //  North
        if ( m_Wall == WallNorth )
        {
            point.x = ( roomRect.right - roomRect.left ) - ( LONG ) ( ( areaX  * ( roomRect.right - roomRect.left ) ) / maxX );
        }
        //  South
        else if ( m_Wall == WallSouth )
        {
            point.x = ( LONG ) ( ( areaX  * ( roomRect.right - roomRect.left ) ) / maxX );
        }
        //  West
        else if ( m_Wall == WallWest )
        {
            point.x = ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        }
        //  East
        else
        {
            point.x = ( roomRect.right - roomRect.left ) - ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        }

        //  Y is not variable
        point.y = ( LONG ) ( ( areaY * ( roomRect.bottom - roomRect.top ) ) / maxY );
    }
    return point;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXWallPicture::ComputeRoomPoint ( DWORD dwX, DWORD dwY, DWORD dwZ )
{
    CPoint point(0,0);

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 && m_Area.yTop != m_Area.yBottom )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        //  areaX West/East correspond to y
        long areaX =  ( long ) ( dwX ) - m_Area.x;
        //
        long areaY = abs(m_Area.yTop - m_Area.yBottom) - abs ( ( long ) ( dwY - m_Area.yBottom ) );
        //  areaZ South/North correspond to x
        long areaZ =  ( long ) ( dwZ ) - m_Area.z;

        long maxX = m_Area.xSectors * TR_SECTOR_SIZE;
        long maxY = abs ( m_Area.yTop - m_Area.yBottom );
        long maxZ = m_Area.zSectors * TR_SECTOR_SIZE;

        //  North
        if ( m_Wall == WallNorth )
        {
            point.x = ( roomRect.right - roomRect.left ) - ( LONG ) ( ( areaX  * ( roomRect.right - roomRect.left ) ) / maxX );
        }
        //  South
        else if ( m_Wall == WallSouth )
        {
            point.x = ( LONG ) ( ( areaX  * ( roomRect.right - roomRect.left ) ) / maxX );
        }
        //  West
        else if ( m_Wall == WallWest )
        {
            point.x = ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        }
        //  East
        else
        {
            point.x = ( roomRect.right - roomRect.left ) - ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        }

        //  Y is not variable
        point.y = ( LONG ) ( ( areaY * ( roomRect.bottom - roomRect.top ) ) / maxY );
    }

    return point;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXWallPicture::ComputeRoomPoint ( long dwX, long dwY, long dwZ )
{
    return ComputeRoomPoint ( (DWORD) dwX, (DWORD) dwY, (DWORD) dwZ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXWallPicture::ComputeRoomPoint ( const char *pX, const char *pY, const char *pZ )
{
    if ( strchr(pX,'.' ) != NULL || strchr(pY,'.' ) != NULL || strchr(pZ,'.' ) != NULL )
    {
        double dfX = atof ( pX );
        double dfY = atof ( pY );
        double dfZ = atof ( pZ );

        return ComputeRoomPoint ( dfX, dfY, dfZ );
    }

    DWORD dwX = atol ( pX );
    DWORD dwY = atol ( pY );
    DWORD dwZ = atol ( pZ );

    return ComputeRoomPoint ( dwX, dwY, dwZ );
}

