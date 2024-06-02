#pragma once

#include <Windows.h>
#include "TRXMenuBase.h"

#define ICON_NOT_SET    0xFFFF

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXNCColor
{
    public:
        CTRXNCColor(void);
        virtual ~CTRXNCColor(void);

    protected :
        //  Different Rect for handle mouse action
        CRect                   m_IconRect;
        CRect                   m_CloseRect;
        CRect                   m_MaximizeRect;
        CRect                   m_MinimizeRect;

        //
        CTRXMenuBase            *m_pContextMenu;
        UINT                    m_iHover;

    public :
        void DrawResizedIcon ( CDC *pDC, HICON hIcon, const RECT *pRect );
        BOOL ScreenPointOverRect ( CWnd *pWnd, CPoint pt, CRect cRect );

        //  To draw Background only set icon to zero
        void DrawIconFrame ( CDC *pDC, const CRect rect, bool bFillRect = true, CBrush *pbr = NULL );
        void DrawIcon ( CDC *pDC, UINT icon, const CRect rect, bool bFillRect = true, CBrush *pbr = NULL );
        void DrawAllIcons (  CWnd *pWnd, UINT iconOnly = 0 );

        //  Return TRUE if treated, FALSE if default must be done
        BOOL PaintCaption( CWnd *pWnd, BOOL bActive = FALSE );
        BOOL PaintWindow( CWnd *pWnd, BOOL bActive = FALSE );
        BOOL Activate( CWnd *pWnd, BOOL bActive = FALSE );

        //  Return TRUE if treated, FALSE if default must be done
        BOOL OnNcLButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point);
        BOOL OnNcLButtonUp(CWnd *pWnd, UINT nHitTest, CPoint point);
        BOOL OnNcRButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point);

        BOOL OnNcMouseMove(CWnd *pWnd, UINT nHitTest, CPoint point);

        BOOL OnNcMouseHover(CWnd *pWnd, UINT nFlags, CPoint point);
        BOOL OnNcMouseLeave(CWnd *pWnd );

        BOOL OnMouseMove(CWnd *pWnd, UINT nFlags, CPoint point);
        BOOL OnLButtonUp(CWnd *pWnd, UINT nFlags, CPoint point);

};

