// TRXRoomWall.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXRoomWall.h"
#include "TRXMapAreas.h"
#include "TRXPosition.h"
#include "TRXInfoPage.h"
#include "TRXRemastered.h"
#include "TRXVerticalMap.h"
#include "TRXGDI.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXRoomWall
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXRoomWall, CStatic)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRoomWall::CTRXRoomWall()
{
    m_bSelected     = FALSE;
    m_bPointMode    = FALSE;
    m_bPointSet     = FALSE;

    m_bClickable    = TRUE;
    m_bMoveMouse    = TRUE;
    m_bSetPoint     = TRUE;

    m_Point.x       = -1;
    m_Point.y       = -1;

    ZeroMemory ( m_szText, sizeof(m_szText) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXRoomWall::~CTRXRoomWall()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetRoomPoint( CPoint pt, const double *pOrientation )
{
    if ( pOrientation != NULL )
    {
        m_Orientation   = *pOrientation;
    }

    if ( pt.x >= 0 && pt.y >= 0 )
    {
        m_bPointSet     = TRUE;
        m_bPointMode    = TRUE;
        m_Point         = pt;
    }
    else
    {
        m_bPointSet     = FALSE;
        m_bPointMode    = FALSE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetOrientation ( double orientation )
{
    m_Orientation   = orientation;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetSelected ( BOOL bSelected )
{
    m_bSelected     = bSelected;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetPointMode ( BOOL bPointMode )
{
    m_bPointMode    = bPointMode;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetClickable ( BOOL bClickable )
{
    m_bClickable    = bClickable;
    m_bMoveMouse    = bClickable;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::SetClickable ( BOOL bClickable, BOOL bMoveMouse, BOOL bSetPoint )
{
    m_bClickable    = bClickable;
    m_bMoveMouse    = bMoveMouse;
    m_bSetPoint     = bSetPoint;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXRoomWall, CButton)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_CONTROL_REFLECT(STN_DBLCLK, &CTRXRoomWall::OnStnDblclick)
    ON_CONTROL_REFLECT(STN_CLICKED, &CTRXRoomWall::OnStnClicked)
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// Gestionnaires de messages de CTRXRoomWall


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if ( pDC )
    {

        // This code only works with buttons.
        ASSERT(lpDrawItemStruct->CtlType == ODT_STATIC);

        // If drawing selected, add the pushed style to DrawFrameControl.
        // Draw the button frame.
        CTRXPen penBlack;
        penBlack.CreatePen(PS_SOLID, 1, ROOM_RGB_BLACK);
        CPen* pOldPen = pDC->SelectObject(&penBlack);

        //  Rectangle
        pDC->Rectangle ( &lpDrawItemStruct->rcItem );
        
        //  Fill
        RECT rect = lpDrawItemStruct->rcItem;
        rect.left++;
        rect.right--;
        rect.top++;
        rect.bottom--;
        CTRXBrush brush;
        if ( m_bSelected )
        {
            brush.CreateSolidBrush ( ROOM_CR_RED );
        }
        else
        {
            brush.CreateSolidBrush ( ROOM_CR_GREEN );
        }
        pDC->FillRect ( &rect, &brush );
        brush.DeleteObject();

        pDC->SelectObject(pOldPen);
        penBlack.DeleteObject();

        // Get the button's text.
        GetWindowText ( m_szText, sizeof(m_szText) );

        // Draw the button text using the text color white.
        COLORREF crOldColor;
        if ( lpDrawItemStruct->itemState & ODS_DISABLED )
        {
            crOldColor = pDC->SetTextColor ( ROOM_RGB_GREY );
        }
        else
        {
            crOldColor = pDC->SetTextColor ( ROOM_RGB_BLACK );
        }

        pDC->SetBkMode( TRANSPARENT );
        pDC->DrawText ( m_szText, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
        pDC->SetTextColor( crOldColor);

        if ( m_bPointMode && m_bPointSet && m_Point.x >= 0 && m_Point.y >= 0 )
        {
            pDC->MoveTo ( m_Point.x - 2, m_Point.y );
            pDC->LineTo ( m_Point.x + 3, m_Point.y );
            pDC->MoveTo ( m_Point.x, m_Point.y - 2 );
            pDC->LineTo ( m_Point.x, m_Point.y + 3 );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXRoomWall::PreTranslateMessage(MSG* pMsg)
{
    // TODO
    return CStatic::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO
    if ( m_bMoveMouse )
    {
        if ( m_bSetPoint )
        {
            m_Point = point;
        }

        bool bContinue = true;

        if ( bContinue )
        {
            CTRXMapAreas *parent = dynamic_cast<CTRXMapAreas *>(GetParent());
            if ( parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnLButtonDown(nFlags, parentPoint );

                bContinue = false;
            }
        }

        if ( bContinue )
        {
            CTRXVerticalMap *parent = dynamic_cast<CTRXVerticalMap *>(GetParent());
            if ( parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnLButtonDown(nFlags, parentPoint );

                bContinue = false;
            }
        }
    }

    //
    CStatic::OnLButtonDown(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO
    if ( m_bMoveMouse )
    {
        if ( m_bSetPoint )
        {
            m_Point = point;
        }

        bool bContinue = true;

        if ( bContinue )
        {
            CTRXMapAreas *parent = dynamic_cast<CTRXMapAreas *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnLButtonUp(nFlags,parentPoint );
                bContinue   = false;
            }
        }

        if ( bContinue )
        {
            CTRXVerticalMap *parent = dynamic_cast<CTRXVerticalMap *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnLButtonUp(nFlags,parentPoint );
                bContinue   = false;
            }
        }

        if ( bContinue )
        {
            CTRXPosition *parent = dynamic_cast<CTRXPosition *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                RECT rect;
                GetClientRect ( &rect );
                InvalidateRect ( &rect );
                parent->ClickRoom(nFlags,point );
                bContinue   = false;
            }
        }

        if ( bContinue )
        {
            CTRXInfoPage *parent = dynamic_cast<CTRXInfoPage *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                RECT rect;
                GetClientRect ( &rect );
                InvalidateRect ( &rect );
                parent->ClickRoom(nFlags,point );
                bContinue   = false;
            }
        }
    }

    //
    CStatic::OnLButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO
    CStatic::OnMouseMove(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnStnDblclick()
{
    bool bContinue = true;

    if ( m_bClickable )
    {
        if ( bContinue )
        {
            CTRXMapAreas *parent = dynamic_cast<CTRXMapAreas *>(GetParent());
            if ( parent != NULL )
            {
                parent->SelectAreaAndExit( m_szText );
                bContinue   = false;
            }
        }

        if ( bContinue )
        {
            CTRXVerticalMap *parent = dynamic_cast<CTRXVerticalMap *>(GetParent());
            if ( parent != NULL )
            {
                parent->SelectAreaAndExit( m_szText );
                bContinue   = false;
            }
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnStnClicked()
{
    if ( m_bClickable )
    {
        CTRXRemastered *parent = dynamic_cast<CTRXRemastered *>(GetParent());
        if ( parent != NULL )
        {
            parent->SingleClick( );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO

    CStatic::OnRButtonDown(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXRoomWall::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO
    if ( m_bMoveMouse )
    {
        bool bContinue = true;

        if ( bContinue )
        {
            CTRXMapAreas *parent = dynamic_cast<CTRXMapAreas *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnRButtonUp(nFlags,parentPoint );
                bContinue   = false;
            }
        }

        if ( bContinue )
        {
            CTRXVerticalMap *parent = dynamic_cast<CTRXVerticalMap *>(GetParent());
            if ( bContinue && parent != NULL )
            {
                POINT pt = point;
                ClientToScreen ( &pt );
                parent->ScreenToClient ( &pt );
                CPoint parentPoint = pt;
                parent->OnRButtonUp(nFlags,parentPoint );
                bContinue   = false;
            }
        }
    }

    CStatic::OnRButtonUp(nFlags, point);
}
