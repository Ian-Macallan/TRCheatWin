// TRXVerticalMap.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXVerticalMap.h"
#include "afxdialogex.h"
#include "TRXMapAreas.h"
#include "TR9SaveGame.h"
#include "TRXRoomPicture.h"
#include "TRXWallPicture.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int LevelAreasCount				= -1;
static const int maxArray				= 512;
static TR_AREA_WALL LevelAreas [ maxArray ];

static WallDirectionEnum WallDirection	= WallNorth;

//
//====================================================================================
//	Sort by  descending x descending z ascending yBottom, ascending ytop, 
//	x is East West and positionned on y on screen
//	z is South North and positionned on x on screen
//====================================================================================
static int compareAreas ( const void *pVoid1, const void *pVoid2 )
{
	TR_AREA_WALL *pArea1	= (TR_AREA_WALL *) pVoid1;
	TR_AREA_WALL *pArea2	= (TR_AREA_WALL *) pVoid2;

	//
	long x1Min		= pArea1->info.x;
	long x1Max		= pArea1->info.x + pArea1->info.xSectors * TR_SECTOR_SIZE;
	long y1Top		= pArea1->info.yTop;
	long y1Bottom	= pArea1->info.yBottom;
	long z1Min		= pArea1->info.z;
	long z1Max		= pArea1->info.z + pArea1->info.zSectors * TR_SECTOR_SIZE;

	//
	long x2Min		= pArea2->info.x;
	long x2Max		= pArea2->info.x + pArea2->info.xSectors * TR_SECTOR_SIZE;
	long y2Top		= pArea2->info.yTop;
	long y2Bottom	= pArea2->info.yBottom;
	long z2Min		= pArea2->info.z;
	long z2Max		= pArea2->info.z + pArea2->info.zSectors * TR_SECTOR_SIZE;

	//
	bool useXmin	= false;
	bool useXmax	= false;
	bool useZmin	= false;
	bool useZmax	= false;
	int SortOrder	= -1;

	if ( WallDirection == WallSouth )
	{
		useZmax		= true;
		SortOrder	= 1;
	}
	else if ( WallDirection == WallNorth )
	{
		useZmin		= true;
		SortOrder	= -1;
	}
	else if ( WallDirection == WallEast )
	{
		useXmin		= true;
		SortOrder	= -1;
	}
	else if ( WallDirection == WallWest )
	{
		useXmax		= true;
		SortOrder	= 1;
	}

	//
	if ( useXmin )
	{
		//
		if ( x1Min >  x2Min )
		{
			return 1 * SortOrder;
		}
		else if ( x1Min < x2Min )
		{
			return -1 * SortOrder;
		}

		//
		if ( z1Min >  z2Min )
		{
			return 1 * SortOrder;
		}
		else if ( z1Min < z2Min )
		{
			return -1 * SortOrder;
		}
	}
	else if ( useXmax )
	{
		//
		if ( x1Max >  x2Max )
		{
			return 1 * SortOrder;
		}
		else if ( x1Max < x2Max )
		{
			return -1 * SortOrder;
		}

		//
		if ( z1Max >  z2Max )
		{
			return 1 * SortOrder;
		}
		else if ( z1Max < z2Max )
		{
			return -1 * SortOrder;
		}
	}
	else if ( useZmin )
	{
		//
		if ( z1Min >  z2Min )
		{
			return 1 * SortOrder;
		}
		else if ( z1Min < z2Min )
		{
			return -1 * SortOrder;
		}

		//
		if ( x1Min >  x2Min )
		{
			return 1 * SortOrder;
		}
		else if ( x1Min < x2Min )
		{
			return -1 * SortOrder;
		}
	}
	else if ( useZmax )
	{
		//
		if ( z1Max >  z2Max )
		{
			return 1 * SortOrder;
		}
		else if ( z1Max < z2Max )
		{
			return -1 * SortOrder;
		}

		//
		if ( x1Max >  x2Max )
		{
			return 1 * SortOrder;
		}
		else if ( x1Max < x2Max )
		{
			return -1 * SortOrder;
		}
	}

	// Always from bottom to top
	if ( y1Bottom >  y2Bottom )
	{
		return 1 * SortOrder;
	}
	else if ( y1Bottom <  y2Bottom )
	{
		return -1 * SortOrder;
	}

	//
	if ( y1Top >  y2Top )
	{
		return 1 * SortOrder;
	}
	else if ( y1Top < y2Top )
	{
		return -1 * SortOrder;
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXVerticalMap
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXVerticalMap, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXVerticalMap::CTRXVerticalMap(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXVerticalMap::IDD, pParent)
{
	m_Parent	= pParent;

	//
	ZeroMemory ( &m_SelectedArea, sizeof(m_SelectedArea) );
	ZeroMemory ( &m_CurrentArea, sizeof(m_CurrentArea) );

	m_SelectedArea.index		= -1;
	m_CurrentArea.index			= -1;
	m_iMoving					= -1;
	m_bClickable				= TRUE;

	m_bCurrentArea				= false;
	m_bSelectedArea				= false;
	m_bCurrentPosition			= false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::Free()
{
	//	Clean Rooms
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		if ( LevelAreas [ i ].pWnd != NULL )
		{
			delete LevelAreas [ i ].pWnd;
			LevelAreas [ i ].pWnd	= NULL;
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXVerticalMap::~CTRXVerticalMap()
{
	Free();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetParent ( CWnd *parent )
{
	m_Parent	= parent;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetSelectedArea ( const TR_AREA *pArea )
{
	if ( pArea != NULL  )
	{
		m_SelectedArea		= *pArea;
		m_bSelectedArea		= true;
	}

	//
	if ( m_bInitDone )
	{
		SetButtonFonts();

		//
		ShowWindows();
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetCurrentArea ( const TR_AREA *pArea, const TR_CUR_POSITION *pCurrentPosition )
{
	if ( pArea != NULL && pCurrentPosition != NULL )
	{
		m_CurrentArea		= *pArea;
		m_CurrentPosition	= *pCurrentPosition;
		m_bCurrentArea		= true;
		m_bCurrentPosition	= true;
	}
	else if ( pArea != NULL )
	{
		m_CurrentArea		= *pArea;
		ZeroMemory ( &m_CurrentPosition, sizeof(m_CurrentPosition) );
		m_bCurrentArea		= true;
		m_bCurrentPosition	= false;
	}
	else
	{
		ZeroMemory ( &m_CurrentArea, sizeof(m_CurrentArea) );
		ZeroMemory ( &m_CurrentPosition, sizeof(m_CurrentPosition) );
		m_bCurrentArea		= false;
		m_bCurrentPosition	= false;
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetClickable ( BOOL bClickable )
{
	m_bClickable = bClickable;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::DoDataExchange(CDataExchange* pDX)
{
	CTRXDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FROM_NORTH, m_North);
	DDX_Control(pDX, IDC_FROM_SOUTH, m_South);
	DDX_Control(pDX, IDC_FROM_WEST, m_West);
	DDX_Control(pDX, IDC_FROM_EAST, m_East);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXVerticalMap, CTRXDialogBase)
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_FROM_NORTH, &CTRXVerticalMap::OnBnClickedFromNorth)
	ON_BN_CLICKED(IDC_FROM_SOUTH, &CTRXVerticalMap::OnBnClickedFromSouth)
	ON_BN_CLICKED(IDC_FROM_WEST, &CTRXVerticalMap::OnBnClickedFromWest)
	ON_BN_CLICKED(IDC_FROM_EAST, &CTRXVerticalMap::OnBnClickedFromEast)
	ON_WM_CLOSE()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FROM_NORTH, &CTRXVerticalMap::OnBnClickedFromNorth)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXVerticalMap
//
/////////////////////////////////////////////////////////////////////////////
static BOOL pointInSquare ( long x, long z, long xL, long xH, long zL, long zH )
{
	if ( x >= xL && x < xH && z >= zL && z < zH )
	{
		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//	--->Z
//	|			x1z1------H1--------x1z2
//	v			|					|
//	X			|					|
//		xL,zL---|-------xLzH		|
//		|		|		|			|
//		|		|		|			|
//		|		V1		|			V2
//		|		|		|			|
//		xH,zL---|-------xHzH		|
//				|					|
//				|					|
//				x2z1------H2--------x2z2
//
//
/////////////////////////////////////////////////////////////////////////////
static BOOL linesInSquare ( long x1, long z1, long x2, long z2, long xL, long xH, long zL, long zH )
{
	//	Poiht in Square
	if ( pointInSquare ( x1, z1, xL, xH, zL, zH ) )
	{
		return TRUE;
	}

	//	Poiht in Square
	if ( pointInSquare ( x2, z1, xL, xH, zL, zH ) )
	{
		return TRUE;
	}

	//	Poiht in Square
	if ( pointInSquare ( x1, z2, xL, xH, zL, zH ) )
	{
		return TRUE;
	}

	//	Poiht in Square
	if ( pointInSquare ( x2, z2, xL, xH, zL, zH ) )
	{
		return TRUE;
	}

	//	Line V1
	//	z1 between x1 x2 outside
	if ( z1 >= zL && z1 <= zH  && x1 <= xL && x2 >= xH )
	{
		return TRUE;
	}

	//	Line V2 x1 x2 outside
	//	z2 between x1 x2 outside
	if ( z2 >= zL && z2 <= zH  && x1 <= xL && x2 >= xH )
	{
		return TRUE;
	}

	//	Line H1
	//	x1 between and z1 z2 outside
	if ( x1 >= xL && x1 <= xH && z1 <= zL && z2 >= zH )
	{
		return TRUE;
	}
	//	Line H2
	//	x2 between and z1 z2 outside
	if ( x2 >= xL && x2 <= xH && z1 <= zL && z2 >= zH )
	{
		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetWallOrientation()
{
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
		{
			if ( LevelAreas [ i ].pWnd != NULL )
			{
				LevelAreas [ i ].pWnd->SetWall ( WallDirection );
			}
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::ShowWindows()
{
	//
	static char szText [ 32 ];

	const float xzDivider			= 150.0;
	const float yDivider			= 100.0;
	const int minimumVertical		= 4;

	//
	//	Choose Area that Match
	bool bminXSet	= false;
	bool bmaxXSet	= false;
	bool bminZSet	= false;
	bool bmaxZSet	= false;
	bool bminYSet	= false;
	bool bmaxYSet	= false;

	m_minX			= 0;
	m_maxX			= 0;
	m_minZ			= 0;
	m_maxZ			= 0;

	m_minY			= 0;
	m_maxY			= 0;

	//	yBottom always greater than yTop
	//	So we search max yBottom and min Ytop
	m_minY			= 0;
	m_maxY			= 0;

	//
	//	Create Or Hide Windows
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		LevelAreas [ i ].visible		= SW_HIDE;

		if ( LevelAreas [ i ].pWnd != NULL )
		{
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
		else
		{
			ZeroMemory ( &LevelAreas [ i ].rect, sizeof(LevelAreas [ i ].rect) );

			//
			LevelAreas [ i ].pWnd = new CTRXWallPicture();

			LevelAreas [ i ].pWnd->SetClickable ( m_bClickable, TRUE, FALSE );

			//
			ZeroMemory ( szText, sizeof(szText) );

			//
			if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
			{
				LevelAreas [ i ].pWnd->SetAreaAndPosition ( &LevelAreas [ i ].info, &m_CurrentPosition );
				LevelAreas [ i ].pWnd->SetWall ( WallDirection );
			}
			else
			{
				LevelAreas [ i ].pWnd->SetAreaAndPosition ( &LevelAreas [ i ].info, NULL);
				LevelAreas [ i ].pWnd->SetWall ( Wall_None );
				sprintf_s ( szText, sizeof(szText), "%d", LevelAreas [ i ].info.index );
			}
			
			//
			CTRXWallPicture::CreateInstanceInside ( this, LevelAreas [ i ].pWnd, LevelAreas [ i ].rect, szText, SS_NOTIFY );
		}
	}

	//
	if ( m_SelectedArea.index == -1 )
	{
		return;
	}

	//
	//	Sort
	qsort ( LevelAreas, LevelAreasCount, sizeof ( TR_AREA_WALL ), compareAreas );

	//
	long xLow		= m_SelectedArea.x;
	long xHigh		= m_SelectedArea.x + m_SelectedArea.xSectors * TR_SECTOR_SIZE;
	long zLow		= m_SelectedArea.z;
	long zHigh		= m_SelectedArea.z + m_SelectedArea.zSectors * TR_SECTOR_SIZE;
	long yTop		= m_SelectedArea.yTop;
	long yBottom	= m_SelectedArea.yBottom;

	//
	//	Select
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		//
		TR_AREA *pArea = &LevelAreas [ i ].info;
		if ( pArea->index != -1 )
		{
			long xTmpLow	= pArea->x;
			long xTmpHigh	= pArea->x + pArea->xSectors * TR_SECTOR_SIZE;
			long zTmpLow	= pArea->z;
			long zTmpHigh	= pArea->z + pArea->zSectors * TR_SECTOR_SIZE;
			long yTmpTop	= pArea->yTop;
			long yTmpBottom	= pArea->yBottom;

			//
			//		A---B
			//		|	|
			//		C---D 
			bool bInside = false;

			//
			if ( m_SelectedArea.index == LevelAreas [ i ].info.index )
			{
				bInside = true;
			}

			//
			if ( linesInSquare ( xTmpLow, zTmpLow, xTmpHigh, zTmpHigh, xLow, xHigh, zLow, zHigh ) )
			{
				bInside = true;
			}

			if ( xTmpLow < m_minX || ! bminXSet )
			{
				m_minX		= xTmpLow;
				bminXSet	= true;
			}

			if ( xTmpHigh > m_maxX || ! bmaxXSet )
			{
				m_maxX		= xTmpHigh;
				bmaxXSet	= true;
			}

			if ( zTmpLow < m_minZ || ! bminZSet )
			{
				m_minZ		= zTmpLow;
				bminZSet	= true;
			}

			if ( zTmpHigh > m_maxZ || ! bmaxZSet )
			{
				m_maxZ		= zTmpHigh;
				bmaxZSet	= true;
			}

			if ( yTmpTop < m_minY || ! bminYSet )
			{
				m_minY		= yTmpTop;
				bminYSet	= true;
			}

			if ( yTmpBottom > m_maxY  || ! bmaxYSet )
			{
				m_maxY		= yTmpBottom;
				bmaxYSet	= true;
			}

			if ( bInside )
			{
				LevelAreas [ i ].visible		= SW_SHOW;
			}
		}
	}

	//
	RECT clientRect;
	GetClientRect ( &clientRect );

	//
	RECT northRect;
	m_North.GetClientRect ( &northRect );

	//
	//	Usable size
	const int margin	= 15;

	//
	RECT usabletRect = clientRect;
	usabletRect.top		= usabletRect.top + ( northRect.bottom - northRect.top ) + margin;
	int computedWidth	= 0;
	if ( WallDirection == WallNorth || WallDirection == WallSouth )
	{
		computedWidth	= (int) ( (float) ( m_maxX - m_minX ) / xzDivider ) + 2 * margin;
	}
	else
	{
		computedWidth	= (int) ( (float) ( m_maxZ - m_minZ ) / xzDivider ) + 2 * margin;
	}

	//
	long maxRight	= 0;
	long maxBottom	= 0;

	//
	CWnd *previousWIndow = NULL;

	//		Show and place on screen
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		if ( LevelAreas [ i ].visible == SW_SHOW )
		{
			long xRelLow	= LevelAreas [ i ].info.x;
			long xRelHigh	= LevelAreas [ i ].info.x + LevelAreas [ i ].info.xSectors * TR_SECTOR_SIZE;
			long zRelLow	= LevelAreas [ i ].info.z;
			long zRelHigh	= LevelAreas [ i ].info.z + LevelAreas [ i ].info.zSectors * TR_SECTOR_SIZE;
			long yRelTop	= LevelAreas [ i ].info.yTop;
			long yRelBottom	= LevelAreas [ i ].info.yBottom;

			//
			xRelLow			= xRelLow - m_minX;
			xRelHigh		= xRelHigh - m_minX;

			zRelLow			= zRelLow - m_minZ;
			zRelHigh		= zRelHigh - m_minZ;

			yRelTop			= yRelTop - m_minY;
			yRelBottom		= yRelBottom - m_minY;

			//
			xRelLow			= (long) ( (double) xRelLow / xzDivider );
			xRelHigh		= (long) ( (double) xRelHigh / xzDivider );

			zRelLow			= (long) ( (double) zRelLow / xzDivider );
			zRelHigh		= (long) ( (double) zRelHigh / xzDivider );

			yRelTop			= (long) ( (double) yRelTop / yDivider );
			yRelBottom		= (long) ( (double) yRelBottom / yDivider );

			//
			if ( WallDirection == WallNorth )
			{
				LevelAreas [ i ].rect.left		= xRelLow + margin;
				LevelAreas [ i ].rect.right		= xRelHigh + margin;
				LevelAreas [ i ].rect.top		= yRelTop + usabletRect.top;
				LevelAreas [ i ].rect.bottom	= yRelBottom + usabletRect.top;
			}
			else if ( WallDirection == WallSouth )
			{
				LevelAreas [ i ].rect.left		= computedWidth - margin - xRelHigh;
				LevelAreas [ i ].rect.right		= computedWidth - margin - xRelLow;
				LevelAreas [ i ].rect.top		= yRelTop + usabletRect.top;
				LevelAreas [ i ].rect.bottom	= yRelBottom + usabletRect.top;
			}
			else if ( WallDirection == WallWest ) 
			{
				LevelAreas [ i ].rect.left		= zRelLow + margin;
				LevelAreas [ i ].rect.right		= zRelHigh + margin;
				LevelAreas [ i ].rect.top		= yRelTop + usabletRect.top;
				LevelAreas [ i ].rect.bottom	= yRelBottom + usabletRect.top;
			}
			else // East
			{
				LevelAreas [ i ].rect.left		= computedWidth - margin - zRelHigh;
				LevelAreas [ i ].rect.right		= computedWidth - margin - zRelLow;
				LevelAreas [ i ].rect.top		= yRelTop + usabletRect.top;
				LevelAreas [ i ].rect.bottom	= yRelBottom + usabletRect.top;
			}

			//
			if ( LevelAreas [ i ].rect.bottom - LevelAreas [ i ].rect.top < minimumVertical )
			{
				LevelAreas [ i ].rect.bottom	= LevelAreas [ i ].rect.top + minimumVertical;
			}

			//
			LevelAreas [ i ].pWnd->MoveWindow ( &LevelAreas [ i ].rect, TRUE );
			int x = LevelAreas [ i ].rect.left;
			int y = LevelAreas [ i ].rect.top;
			int cx = LevelAreas [ i ].rect.right - LevelAreas [ i ].rect.left;
			int cy = LevelAreas [ i ].rect.bottom - LevelAreas [ i ].rect.top;
			LevelAreas [ i ].pWnd->SetWindowPos ( previousWIndow, x, y, cx, cy, (UINT)( HWND_TOP ) );
			LevelAreas [ i ].pWnd->ShowWindow ( SW_NORMAL );

			if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
			{
				CPoint pt = LevelAreas [ i ].pWnd->ComputeRoomPoint ( m_CurrentPosition.x, m_CurrentPosition.y, m_CurrentPosition.z );
				LevelAreas [ i ].pWnd->SetRoomPoint ( pt, &m_CurrentPosition.orientation );
			}

			previousWIndow	= LevelAreas [ i ].pWnd;

			if ( m_bToolTip )
			{
				m_ToolTip.AddTool ( LevelAreas [ i ].pWnd, LPSTR_TEXTCALLBACK );
			}

		}

		//
		if ( LevelAreas [ i ].rect.bottom > maxBottom )
		{
			maxBottom = LevelAreas [ i ].rect.bottom;
		}

		if ( LevelAreas [ i ].rect.right > maxRight )
		{
			maxRight = LevelAreas [ i ].rect.right;
		}
	}

	//
	RECT windowRect;
	GetWindowRect ( &windowRect );

	//
	GetClientRect ( &clientRect );
	int width	= clientRect.right - clientRect.left;
	int height	= clientRect.bottom - clientRect.top;

	//
	if ( maxBottom + margin > height || maxRight + margin > width )
	{
		long deltaBottom	= maxBottom - height + margin;
		long deltaRigth		= maxRight - width + margin;

		if ( deltaBottom > 0 )
		{
			windowRect.bottom	+= deltaBottom;
		}

		if ( deltaRigth > 0 )
		{
			windowRect.right	+= deltaRigth;
		}

		MoveWindow ( &windowRect, TRUE );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXVerticalMap::OnInitDialog()
{
	CTRXDialogBase::OnInitDialog();

	//
	static char szText [ MAX_PATH ];

	//
	const char *pAreaName	= GetAreaLevelName ( m_iTombraiderFull, m_iLevel - 1 );
	const char *pType = "Standard";
	if ( strncmp( pAreaName, "TRR", 3 ) == 0 )
	{
		pType = "Remastered";
	}
	if ( strchr (pAreaName, '_' ) != NULL )
	{
		pAreaName = strchr (pAreaName, '_' ) + 1;
	}
	const char *pLevelName = CTR9SaveGame::GetLevelName ( m_iTombraiderFull, m_iLevel );
	sprintf_s ( szText, sizeof(szText), "%s [%s] - %s", pType, pAreaName, pLevelName );
	SetWindowText ( szText );

	// Load All Areas
	ZeroMemory ( LevelAreas, sizeof(LevelAreas) );
	LevelAreasCount = 0;
	TR_AREA *area = GetTRArea ( m_iTombraiderFull, m_iLevelIndex, LevelAreasCount );
	while ( area != NULL )
	{
		LevelAreas [ LevelAreasCount ].id		= 1000 + LevelAreasCount;
		LevelAreas [ LevelAreasCount ].info		= *area;

		LevelAreasCount++;
		area = GetTRArea ( m_iTombraiderFull, m_iLevelIndex, LevelAreasCount );
	}

	ShowWindows();


	// TODO: Add Code Here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if ( m_bToolTip )
	{
		m_ToolTip.Activate(TRUE);
	}

	SetButtonFonts();

	m_bInitDone	= true;

	//
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//	Search Point if not found use Text and call parent SelectArea
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SelectAreaAndExit ( const char *pText )
{
	POINT point;
	BOOL bPosition	= GetCursorPos ( &point );
	int index		= FindWindowAtPoint ( point );

	int iSelectedArea = -1;
	if ( index >= 0 )
	{
		iSelectedArea = LevelAreas [ index ].info.index;
	}
	else
	{
		if ( strlen(pText) > 0 )
		{
			iSelectedArea = atoi ( pText );
		}
		else
		{
			iSelectedArea	= -1;
		}
	}

	if ( iSelectedArea != -1 )
	{
		CTRXMapAreas *pParent = dynamic_cast<CTRXMapAreas *>(m_Parent);
		if ( pParent != NULL )
		{
			pParent->SelectAreaAndExit ( iSelectedArea );
		}
	}

	CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnSysCommand(UINT nID, LPARAM lParam)
{
	CTRXDialogBase::OnSysCommand(nID, lParam);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetTombraiderLevelIndex ( int iTombraider, int iLevelIndex )
{
	switch ( iTombraider )
	{
		//
		case 1 :
		case 10 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 1;
			break;
		}

		//
		case 15 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 1;
			if ( iLevelIndex < 4 )
			{
				iLevelIndex = iLevelIndex + 15;
			}
			break;
		}

		//
		case 2 :
		case 20 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 2;
			break;
		}

		//
		case 25 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 2;
			if ( iLevelIndex < 5 )
			{
				iLevelIndex	= iLevelIndex + 18;
			}
			break;
		}

		//
		case 3:
		case 30 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 3;
			break;
		}

		//
		case 35 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 3;
			if ( iLevelIndex < 6 )
			{
				iLevelIndex	= iLevelIndex + 20;
			}
			break;
		}

		//
		case 4:
		case 40 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 4;
			break;
		}
		case 45 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 4;
			if ( iLevelIndex < 2 )
			{
				iLevelIndex	= iLevelIndex + 38;
			}
			break;
		}
		//
		case 49 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 4;
			break;
		}
		//
		case 5:
		case 50 :
		{
			m_iTombraiderFull	= iTombraider;
			m_iTombraider		= 5;
			break;
		}

		case 99 :
		{
			m_iTombraiderFull	= iTombraider;
			break;
		}
	}

	m_iLevelIndex	= iLevelIndex;
	m_iLevel		= m_iLevelIndex + 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXVerticalMap::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
	//
	static char szText [ MAX_PATH * 4 ];

	//
	if ( ! m_bToolTip )
	{
		return FALSE;
	}

	//
	TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMH;

	//
	POINT			ptAction;
	GetCursorPos ( &ptAction );
	ScreenToClient ( &ptAction );

	int index =	FindWindowAtCPoint( ptAction );
	if ( index >= 0 )
	{
		sprintf_s ( szText, sizeof(szText), "%d - yTop : %ld - yBottom : %ld\r\n",
			LevelAreas [ index ].info.index,
			LevelAreas [ index ].info.yTop,
			LevelAreas [ index ].info.yBottom );
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "x : %ld,%ld - z : %ld,%ld\r\n",
			LevelAreas [ index ].info.x, LevelAreas [ index ].info.x + LevelAreas [ index ].info.xSectors * TR_SECTOR_SIZE,
			LevelAreas [ index ].info.z, LevelAreas [ index ].info.z + LevelAreas [ index ].info.zSectors * TR_SECTOR_SIZE );
#ifdef _DEBUG
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Min Ceiling : %ld - Max Floor : %ld\r\n",
			LevelAreas [ index ].info.minCeiling * ROOM_FLOOR_UNITS, LevelAreas [ index ].info.maxFloor * ROOM_FLOOR_UNITS );
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Max Ceiling : %ld - Min Floor : %ld\r\n",
			LevelAreas [ index ].info.maxCeiling * ROOM_FLOOR_UNITS, LevelAreas [ index ].info.minFloor * ROOM_FLOOR_UNITS );
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Room Height : %ld\r\n",
			( LevelAreas [ index ].info.minCeiling - LevelAreas [ index ].info.maxFloor ) * ROOM_FLOOR_UNITS );
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Solid : %ld%% %ld%%\r\n",
			LevelAreas [ index ].info.solidFloorRatio, LevelAreas [ index ].info.solidCeilingRatio );
#endif
		pText->lpszText = szText;

		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//	Point in Client Coordinates
/////////////////////////////////////////////////////////////////////////////
int CTRXVerticalMap::FindWindowAtCPoint( CPoint point)
{
	POINT pt = point;
	ClientToScreen ( &pt );
	return FindWindowAtPoint ( pt );
}

//
/////////////////////////////////////////////////////////////////////////////
//	Point in Screen Coordinates
/////////////////////////////////////////////////////////////////////////////
int CTRXVerticalMap::FindWindowAtPoint( POINT point)
{
	for ( int i = LevelAreasCount - 1; i >= 0; i-- )
	{
		if ( LevelAreas [ i ].pWnd != NULL /* && LevelAreas [ i ].visible == SW_NORMAL */ )
		{
			RECT rect;
			LevelAreas [ i ].pWnd->GetWindowRect ( &rect );
			if ( point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom )
			{
				return i;
			}
		}
	}

	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	m_iMoving	= -1;
	CTRXDialogBase::OnLButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	int index = FindWindowAtCPoint(point);
	if ( index != -1 )
	{
		m_iMoving							= index;
		LevelAreas [ m_iMoving ].point.x	= point.x;
		LevelAreas [ m_iMoving ].point.y	= point.y;
	}

	CTRXDialogBase::OnLButtonDown(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	if ( m_iMoving != -1 )
	{
		RECT rect;
		LevelAreas [ m_iMoving ].pWnd->GetWindowRect ( &rect );
		ScreenToClient ( &rect );

		//
		RECT newRect = rect;
		newRect.left	= newRect.left + point.x - LevelAreas [ m_iMoving ].point.x;
		newRect.right	= newRect.right + point.x - LevelAreas [ m_iMoving ].point.x;
		newRect.top		= newRect.top + point.y - LevelAreas [ m_iMoving ].point.y;
		newRect.bottom	= newRect.bottom + point.y - LevelAreas [ m_iMoving ].point.y;
		LevelAreas [ m_iMoving ].pWnd->MoveWindow ( &newRect, TRUE );
		InvalidateRect ( &newRect, TRUE );

		LevelAreas [ m_iMoving ].point.x = point.x;
		LevelAreas [ m_iMoving ].point.y = point.y;

		//	Client Rect
		RECT rectWindow;
		GetClientRect ( &rectWindow );

		if (	newRect.left <= rectWindow.left || newRect.right >= rectWindow.right ||
				newRect.top <= rectWindow.top || newRect.bottom >= rectWindow.bottom )
		{
			m_iMoving = -1;
		}
	}

	CTRXDialogBase::OnMouseMove(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXVerticalMap::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add Code Here
	m_ToolTip.RelayEvent(pMsg);

	return CTRXDialogBase::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::SetButtonFonts()
{
	if ( WallDirection == WallNorth )
	{
		m_North.SetFont ( m_pBoldFont );
		m_South.SetFont ( m_pNormalFont );
		m_West.SetFont ( m_pNormalFont );
		m_East.SetFont ( m_pNormalFont );
	}
	else if ( WallDirection == WallSouth )
	{
		m_North.SetFont ( m_pNormalFont );
		m_South.SetFont ( m_pBoldFont );
		m_West.SetFont ( m_pNormalFont );
		m_East.SetFont ( m_pNormalFont );
	}
	else if ( WallDirection == WallWest )
	{
		m_North.SetFont ( m_pNormalFont );
		m_South.SetFont ( m_pNormalFont );
		m_West.SetFont ( m_pBoldFont );
		m_East.SetFont ( m_pNormalFont );
	}
	else
	{
		m_North.SetFont ( m_pNormalFont );
		m_South.SetFont ( m_pNormalFont );
		m_West.SetFont ( m_pNormalFont );
		m_East.SetFont ( m_pBoldFont );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnBnClickedFromNorth()
{
	WallDirection = WallNorth;
	SetButtonFonts();
	SetWallOrientation();
	ShowWindows();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnBnClickedFromSouth()
{
	WallDirection = WallSouth;
	SetButtonFonts();
	SetWallOrientation();
	ShowWindows();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnBnClickedFromWest()
{
	WallDirection = WallWest;
	SetButtonFonts();
	SetWallOrientation();
	ShowWindows();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnBnClickedFromEast()
{
	WallDirection = WallEast;
	SetButtonFonts();
	SetWallOrientation();
	ShowWindows();

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	//
	int index = FindWindowAtCPoint(point);
	if ( index != -1 )
	{
		SetSelectedArea ( &LevelAreas [ index ].info );
	}

	CTRXDialogBase::OnRButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnClose()
{
	// TODO: Add Code Here

	//	Clean Rooms
	Free();

	//
	if ( m_Parent != NULL )
	{
		CTRXMapAreas *pWnd = dynamic_cast<CTRXMapAreas *>(m_Parent );
		if ( pWnd != NULL )
		{
			pWnd->SetVerticalMap ( NULL );
		}
	}

	//
	CTRXDialogBase::OnClose();

	//
	DestroyWindow ();

	//
	delete this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXVerticalMap::OnDestroy()
{
	CTRXDialogBase::OnDestroy();

	// TODO: Add Code Here
}


