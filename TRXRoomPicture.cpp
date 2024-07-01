// TRXRoomPicture.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXRoomPicture.h"
#include "TR_Areas.h"
#include "TR9SaveGame.h"
#include "TRXColors.h"
#include "TRXGDI.h"

static const int RoomDivider    = 150;
static const int RoomMargin     = 10;

//
/////////////////////////////////////////////////////////////////////////////
// CTRXRoomPicture
//
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTRXRoomPicture, CTRXRoomWall)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRoomPicture::CTRXRoomPicture()
{
    ZeroMemory ( &m_Area, sizeof(m_Area) );
    m_bAreaSet      = false;
    m_bPositionSet  = false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRoomPicture::~CTRXRoomPicture()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRoomPicture::CreateInstanceInside( CWnd *parent, CTRXRoomPicture *pRoom, RECT &rect,
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
BOOL CTRXRoomPicture::CreateInstanceInside( CWnd *parent, CTRXRoomPicture *pRoom, CStatic *pFrame,
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
BOOL CTRXRoomPicture::CreateInstanceBelow ( CWnd *parent, CTRXRoomPicture *pRoom, CWnd *pFrame,
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
BEGIN_MESSAGE_MAP(CTRXRoomPicture, CTRXRoomWall)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXRoomPicture
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomPicture::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
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
        //  Center of the cross is x,y
        if ( m_bPointMode )
        {
            //
            int iOrientation    = (int) m_Orientation % 360;
            m_Orientation       = (float) ( iOrientation );

            //  Cross
            DrawCross ( pDC, m_Point.x, m_Point.y, lenCross );

            float quarter = 90.0;

            //  Facing West : 90
            if ( m_Orientation >= 0.5 * quarter /* 45.0 */ && m_Orientation < 1.5 * quarter /* 135.0 */ )
            {
                DrawArrowDown ( pDC, m_Point.x, m_Point.y, lenCross );
            }
            //  Facing North : 180
            else if ( m_Orientation >= 1.5 * quarter /* 135.0 */ && m_Orientation < 2.5 * quarter /* 225.0 */ )
            {
                DrawArrowLeft ( pDC, m_Point.x, m_Point.y, lenCross );
            }
            //  East : 270
            else if ( m_Orientation >= 2.5 * quarter /* 225.0 */ && m_Orientation < 3.5 * quarter /* 315.0 */ )
            {
                DrawArrowUp ( pDC, m_Point.x, m_Point.y, lenCross );
            }
            //  Facing South : 0 
            else if ( m_Orientation >= 3.5 * quarter /* 315.0 */ || m_Orientation < 0.5 * quarter /* 45.0 */ )
            {
                DrawArrowRight ( pDC, m_Point.x, m_Point.y, lenCross );
            }
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomPicture::SetAreaAndPosition ( const TR_AREA *pArea, const TR_CUR_POSITION *position )
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
const TR_AREA *CTRXRoomPicture::GetArea()
{
    return &m_Area;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXRoomPicture::ComputeRoomPoint ( double dfX, double dfZ )
{
    CPoint point(0,0);

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        //  areaX West/East correspond to y
        long areaX =  ( long ) ( dfX * POSITION_DIVIDER ) - m_Area.x;
        //  areaZ South/North correspond to x
        long areaZ =  ( long ) ( dfZ * POSITION_DIVIDER ) - m_Area.z;

        long maxX = m_Area.xSectors * TR_SECTOR_SIZE;
        long maxZ = m_Area.zSectors * TR_SECTOR_SIZE;

        point.x = ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        point.y = ( LONG ) ( ( areaX * ( roomRect.bottom - roomRect.top ) ) / maxX );
    }
    return point;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXRoomPicture::ComputeRoomPoint ( DWORD dwX, DWORD dwZ )
{
    CPoint point(0,0);

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        //  areaX West/East correspond to y
        long areaX =  ( long ) ( dwX ) - m_Area.x;
        //  areaZ South/North correspond to x
        long areaZ =  ( long ) ( dwZ ) - m_Area.z;

        long maxX = m_Area.xSectors * TR_SECTOR_SIZE;
        long maxZ = m_Area.zSectors * TR_SECTOR_SIZE;

        point.x = ( LONG ) ( ( areaZ  * ( roomRect.right - roomRect.left ) ) / maxZ );
        point.y = ( LONG ) ( ( areaX * ( roomRect.bottom - roomRect.top ) ) / maxX );
    }

    return point;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXRoomPicture::ComputeRoomPoint ( long dwX, long dwZ )
{
    return ComputeRoomPoint ( (DWORD) dwX, (DWORD) dwZ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXRoomPicture::ComputeRoomPoint ( const char *pX, const char *pZ )
{
    if ( strchr(pX,'.' ) != NULL || strchr(pZ,'.' ) != NULL )
    {
        double dfX = atof ( pX );
        double dfZ = atof ( pZ );

        return ComputeRoomPoint ( dfX, dfZ );
    }

    DWORD dwX = atol ( pX );
    DWORD dwZ = atol ( pZ );

    return ComputeRoomPoint ( dwX, dwZ );
}

//
/////////////////////////////////////////////////////////////////////////////
//  CPoint in Client Rect
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRoomPicture::GetXZ(CPoint point, double &dfX, double &dfZ)
{
    dfX = 0;
    dfZ = 0;

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        double xRatio = ( double ) point.y / ( double ) roomRect.bottom;
        double zRatio = ( double ) point.x / ( double ) roomRect.right;

        dfX = ( ( double ) m_Area.x + ( double ) m_Area.xSectors * TR_SECTOR_SIZE * xRatio ) / POSITION_DIVIDER;
        dfZ = ( ( double ) m_Area.z + ( double ) m_Area.zSectors * TR_SECTOR_SIZE * zRatio ) / POSITION_DIVIDER;

        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  CPoint in Client Rect
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRoomPicture::GetXZ(CPoint point, DWORD &dwX, DWORD &dwZ)
{
    dwX = 0;
    dwZ = 0;

    if ( m_bAreaSet && m_Area.xSectors != 0 && m_Area.zSectors != 0 )
    {
        RECT roomRect;
        GetClientRect ( &roomRect );

        double xRatio = ( double ) point.y / ( double ) roomRect.bottom;
        double zRatio = ( double ) point.x / ( double ) roomRect.right;

        dwX = (DWORD) ( ( double ) m_Area.x + ( double ) m_Area.xSectors * TR_SECTOR_SIZE * xRatio );
        dwZ = (DWORD) ( ( double ) m_Area.z + ( double ) m_Area.zSectors * TR_SECTOR_SIZE * zRatio );

        return TRUE;
    }

    return FALSE;
}
