#pragma once

#include "TRXRoomWall.h"
#include "TR_Areas.h"

//
/////////////////////////////////////////////////////////////////////////////
// CTRXRoomPicture
//
/////////////////////////////////////////////////////////////////////////////
class CTRXRoomPicture : public CTRXRoomWall
{
		DECLARE_DYNAMIC(CTRXRoomPicture)

	public:
		CTRXRoomPicture();
		virtual ~CTRXRoomPicture();

		//
		static BOOL CreateInstanceInside(CWnd *parent, CTRXRoomPicture *pRoom, RECT &rect, 
												const char *pText = "", DWORD dwStyle = SS_NOTIFY);

		static BOOL CreateInstanceInside(CWnd *parent, CTRXRoomPicture *pRoom, CStatic *pFrame, 
												const char *pText = "", DWORD dwStyle = SS_NOTIFY);

		//	Create Below the frame with area value
		static BOOL CreateInstanceBelow(CWnd *parent, CTRXRoomPicture *pRoom, CWnd *pFrame, 
												const char *pText = "", DWORD dwStyle = SS_NOTIFY);

		//	Values are divided by POSITION_DIVDER (1000)
		CPoint ComputeRoomPoint ( double dfX, double dfZ );
		CPoint ComputeRoomPoint ( DWORD dwX, DWORD dwZ );
		CPoint ComputeRoomPoint ( long dwX, long dwZ );
		CPoint ComputeRoomPoint ( const char *pX, const char *pZ );

		//	CPoint in Client Rect
		BOOL GetXZ(CPoint point, double &dfX, double &dfZ);
		BOOL GetXZ(CPoint point, DWORD &dwX, DWORD &dwZ);
		void SetAreaAndPosition ( const TR_AREA *pArea, const TR_CUR_POSITION *position );

		const TR_AREA *GetArea();

	protected  :
		TR_AREA				m_Area;
		TR_CUR_POSITION		m_CurrentPosition;

		bool				m_bAreaSet;
		bool				m_bPositionSet;

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


