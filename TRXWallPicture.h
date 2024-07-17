#pragma once

#include "TRXRoomWall.h"
#include "TR_Areas.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
enum WallDirectionEnum
{
    WallNorth,
    WallSouth,
    WallWest,
    WallEast,
    Wall_None,
};

//
/////////////////////////////////////////////////////////////////////////////
// CTRXWallPicture
//
/////////////////////////////////////////////////////////////////////////////
class CTRXWallPicture : public CTRXRoomWall
{
        DECLARE_DYNAMIC(CTRXWallPicture)

    public:
        CTRXWallPicture();
        virtual ~CTRXWallPicture();

        //
        static BOOL CreateInstanceInside(CWnd *parent, CTRXWallPicture *pRoom, RECT &rect,
                                                const char *pText = "", DWORD dwStyle = SS_NOTIFY);

        static BOOL CreateInstanceInside(CWnd *parent, CTRXWallPicture *pRoom, CStatic *pFrame,
                                                const char *pText = "", DWORD dwStyle = SS_NOTIFY);

        //  Create Below the frame with area value
        static BOOL CreateInstanceBelow(CWnd *parent, CTRXWallPicture *pRoom, CWnd *pFrame,
                                                const char *pText = "", DWORD dwStyle = SS_NOTIFY);

        //  Values are divided by POSITION_DIVDER (1000)
        CPoint ComputeRoomPoint ( double dfX, double dfY, double dfZ );
        CPoint ComputeRoomPoint ( DWORD dwX, DWORD dwY, DWORD dwZ );
        CPoint ComputeRoomPoint ( long dwX, long dwY, long dwZ );
        CPoint ComputeRoomPoint ( const char *pX, const char *pY, const char *pZ );

        //  CPoint in Client Rect
        // BOOL GetXZ(CPoint point, double &dfX, double &dfZ);
        // BOOL GetXZ(CPoint point, DWORD &dwX, DWORD &dwZ);
        void SetAreaAndPosition ( const TR_AREA *pArea, const TR_CUR_POSITION *position );
        void SetWall ( int iTombraider, WallDirectionEnum eWall );
        const TR_AREA *GetArea();

    protected  :
        int                 m_iTombraider;
        BOOL                m_bTombraider123;

        TR_AREA             m_Area;
        TR_CUR_POSITION     m_CurrentPosition;

        bool                m_bAreaSet;
        bool                m_bPositionSet;

        WallDirectionEnum   m_Wall;

    protected:
        DECLARE_MESSAGE_MAP()

    public:
        virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


