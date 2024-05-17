// TRXMapAreas.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXMapAreas.h"
#include "afxdialogex.h"
#include "TR9SaveGame.h"

#include "TR_Areas.h"
#include "TRXRoomPicture.h"
#include "TRXPosition.h"
#include "TRXInfoPage.h"
#include "TRXGlobal.h"

#include "resource.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int LevelAreasCount	= -1;
static const int maxArray = 512;
static TR_AREA_ROOM LevelAreas [ maxArray ];

static int LevelTopArrayCount = 0;
static long LevelTopArray [ maxArray ];

static int LevelBottomArrayCount = 0;
static long LevelBottomArray [ maxArray ];

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXMapAreas
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXMapAreas, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMapAreas::CTRXMapAreas(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXMapAreas::IDD, pParent)
{
	m_iMoving					= -1;

	LevelAreasCount				= 0;
	LevelTopArrayCount			= 0;
	LevelBottomArrayCount		= 0;

	ZeroMemory ( LevelAreas, sizeof(LevelAreas) );
	ZeroMemory ( LevelTopArray, sizeof(LevelTopArray) );
	ZeroMemory ( LevelBottomArray, sizeof(LevelBottomArray) );

	ZeroMemory ( &m_CurrentArea, sizeof(m_CurrentArea) );
	m_bCurrentArea				= false;
	m_bCurrentPosition			= false;

	//
	m_bClickable				= TRUE;

	m_iTombraider				= 0;
	m_iTombraiderFull			= 0;

	m_pVerticalMap				= NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXMapAreas::~CTRXMapAreas()
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
void CTRXMapAreas::DoDataExchange(CDataExchange* pDX)
{
	CTRXDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEVEL_TOP, m_LevelTop);
	DDX_Control(pDX, IDC_LEVEL_BOTTOM, m_LevelBottom);
	DDX_Control(pDX, IDC_REFRESH, m_Refresh);
	DDX_Control(pDX, IDC_AREA, m_Area);
	DDX_Control(pDX, IDC_EAST, m_East);
	DDX_Control(pDX, IDC_CURRENT, m_Current);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXMapAreas, CTRXDialogBase)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_REFRESH, &CTRXMapAreas::OnBnClickedRefresh)
	ON_EN_CHANGE(IDC_AREA, &CTRXMapAreas::OnChangeArea)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_CBN_SELCHANGE(IDC_LEVEL_TOP, &CTRXMapAreas::OnSelchangeLevelTop)
	ON_CBN_SELCHANGE(IDC_LEVEL_BOTTOM, &CTRXMapAreas::OnSelchangeLevelBottom)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//
//====================================================================================
//
//====================================================================================
static BOOL SearchInArrays ( long array [], int arrayCount, long value )
{
	for ( int i = 0; i < arrayCount; i++ )
	{
		if ( array [ i ] == value )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//
//====================================================================================
//
//====================================================================================
static BOOL AddToArrays ( long array [], int &arrayCount, int arrayMax, long value )
{
	if ( SearchInArrays ( array, arrayCount, value ) )
	{
		return TRUE;
	}

	if ( arrayCount < arrayMax )
	{
		array [ arrayCount ] = value;
		arrayCount++;
		return TRUE;
	}

	return FALSE;
}

//
//====================================================================================
//
//====================================================================================
static int compareLong ( const void *pVoid1, const void *pVoid2 )
{
	long *pArea1	= (long *) pVoid1;
	long *pArea2	= (long *) pVoid2;

	return *pArea1 - *pArea2;
}

//
//====================================================================================
//	Sort by descending yBottom, descending ytop, ascending x z
//====================================================================================
static int compareAreas ( const void *pVoid1, const void *pVoid2 )
{
	TR_AREA_ROOM *pArea1	= (TR_AREA_ROOM *) pVoid1;
	TR_AREA_ROOM *pArea2	= (TR_AREA_ROOM *) pVoid2;

	//
	if ( pArea1->info.yTop >  pArea2->info.yTop )
	{
		return -1;
	}
	else if ( pArea1->info.yTop < pArea2->info.yTop )
	{
		return 1;
	}

	//
	if ( pArea1->info.yBottom >  pArea2->info.yBottom )
	{
		return -1;
	}
	else if ( pArea1->info.yBottom <  pArea2->info.yBottom )
	{
		return 1;
	}

	//
	if ( pArea1->info.x >  pArea2->info.x )
	{
		return 1;
	}
	else if ( pArea1->info.x < pArea2->info.x )
	{
		return -1;
	}

	if ( pArea1->info.z >  pArea2->info.z )
	{
		return 1;
	}
	else if ( pArea1->info.z < pArea2->info.z )
	{
		return -1;
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXMapAreas
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMapAreas::OnInitDialog()
{
	CTRXDialogBase::OnInitDialog();

	//
	static char szText [ MAX_PATH ];

	m_East.SetWindowText ( "EAST" );

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

	// Load
	ZeroMemory ( LevelAreas, sizeof(LevelAreas) );
	LevelAreasCount = 0;
	TR_AREA *area = GetTRArea ( m_iTombraiderFull, m_iLevelIndex, LevelAreasCount );
	while ( area != NULL )
	{
		LevelAreas [ LevelAreasCount ].id		= 1000 + LevelAreasCount;
		LevelAreas [ LevelAreasCount ].info		= *area;

		AddToArrays ( LevelTopArray, LevelTopArrayCount, maxArray, LevelAreas [ LevelAreasCount ].info.yTop );
		AddToArrays ( LevelBottomArray, LevelBottomArrayCount, maxArray, LevelAreas [ LevelAreasCount ].info.yBottom );

		LevelAreasCount++;
		area = GetTRArea ( m_iTombraiderFull, m_iLevelIndex, LevelAreasCount );
	}

	//
	//	Sort
	qsort ( LevelAreas, LevelAreasCount, sizeof ( TR_AREA_ROOM ), compareAreas );
	qsort ( LevelTopArray, LevelTopArrayCount, sizeof ( long ), compareLong );
	qsort ( LevelBottomArray, LevelBottomArrayCount, sizeof ( long ), compareLong );

	//
	//	Fill Combos
	m_LevelTop.ResetContent();
	for ( int i = 0; i < LevelTopArrayCount; i++ )
	{
		sprintf_s ( szText, sizeof(szText), "%d", LevelTopArray [ i ] );
		m_LevelTop.AddString ( szText );
	}

	m_LevelBottom.ResetContent();
	for ( int i = 0; i < LevelBottomArrayCount; i++ )
	{
		sprintf_s ( szText, sizeof(szText), "%d", LevelBottomArray [ i ] );
		m_LevelBottom.AddString ( szText );
	}

	//	Show Current Area
	if ( m_bCurrentArea )
	{
		sprintf_s ( szText, sizeof(szText), "%d", m_CurrentArea.index );
		m_Current.SetWindowText ( szText );
	}

	//
	//	Find Lowest yTop and Greatest yBottom 
	//	yTop is allways < yBottom
	bool bMinTopSet		= false;
	m_yMinTop			= 0;

	bool bMaxTopSet		= false;
	m_yMaxTop			= 0;

	bool bMinBottomSet	= false;
	m_yMinBottom		 = 0;

	bool bMaxBottomSet	= false;
	m_yMaxBottom		 = 0;

	//	Get xMin and zMin Before Compute
	m_xMin				= 0;
	m_zMin				= 0;

	//	We could have also used the sort arrays
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		if ( ! bMinBottomSet || ( LevelAreas [ i ].info.yBottom < m_yMinBottom ) )
		{
			m_yMinBottom	= LevelAreas [ i ].info.yBottom;
			bMinBottomSet	= true;
		}

		if ( ! bMaxBottomSet || ( LevelAreas [ i ].info.yBottom > m_yMaxBottom ) )
		{
			m_yMaxBottom	= LevelAreas [ i ].info.yBottom;
			bMaxBottomSet	= true;
		}

		if ( ! bMinTopSet || ( LevelAreas [ i ].info.yTop < m_yMinTop ) )
		{
			m_yMinTop	= LevelAreas [ i ].info.yTop;
			bMinTopSet	= true;
		}

		if ( ! bMaxTopSet || ( LevelAreas [ i ].info.yTop > m_yMaxTop ) )
		{
			m_yMaxTop	= LevelAreas [ i ].info.yTop;
			bMaxTopSet	= true;
		}

		//
		if ( LevelAreas [ i ].info.x < m_xMin )
		{
			m_xMin	= LevelAreas [ i ].info.x;
		}

		if ( LevelAreas [ i ].info.z < m_zMin )
		{
			m_zMin	= LevelAreas [ i ].info.z;
		}

	}

	//
	m_yMin = min(m_yMinTop,m_yMinBottom);
	m_yMax = max(m_yMaxTop,m_yMaxBottom);

	//
	m_SetManualCombo	= false;
	SetCurSel ( &m_LevelTop, m_yMinTop );
	SetCurSel ( &m_LevelBottom, m_yMaxBottom );
	m_SetManualCombo	= true;

	//
	RECT levelRect;
	m_LevelTop.GetClientRect ( &levelRect );

	//
	int maxBottom	= -1;
	int maxRight	= -1;

	//
	CWnd *previousWIndow = NULL;

	//
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		LevelAreas [ i ].pWnd			= new CTRXRoomPicture();

		//
		if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
		{
			LevelAreas [ i ].pWnd->SetAreaAndPosition ( &LevelAreas [ i ].info, &m_CurrentPosition );
		}
		else
		{
			LevelAreas [ i ].pWnd->SetAreaAndPosition ( &LevelAreas [ i ].info, NULL);
		}

		//	Compute Place
		ComputeRoomRect ( &levelRect, &LevelAreas [ i ].rect, &LevelAreas [ i ].info );

		if ( LevelAreas [ i ].rect.bottom > maxBottom )
		{
			maxBottom	= LevelAreas [ i ].rect.bottom;
		}

		if ( LevelAreas [ i ].rect.right > maxRight )
		{
			maxRight	= LevelAreas [ i ].rect.right;
		}

		//
		if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
		{
			LevelAreas [ i ].pWnd->SetPointMode ( TRUE );
			ZeroMemory ( szText, sizeof(szText) );
		}
		else
		{
			//
			sprintf_s ( szText, sizeof(szText), "%d", LevelAreas [ i ].info.index );
		}

		CTRXRoomPicture::CreateInstanceInside ( this, LevelAreas [ i ].pWnd, LevelAreas [ i ].rect, szText, SS_NOTIFY );

		//
		int x = LevelAreas [ i ].rect.left;
		int y = LevelAreas [ i ].rect.top;
		int cx = LevelAreas [ i ].rect.right - LevelAreas [ i ].rect.left;
		int cy = LevelAreas [ i ].rect.bottom - LevelAreas [ i ].rect.top;
		LevelAreas [ i ].pWnd->SetWindowPos ( previousWIndow, x, y, cx, cy, (UINT) HWND_TOP );
		previousWIndow	= LevelAreas [ i ].pWnd;

		//
		LevelAreas [ i ].pWnd->SetClickable ( m_bClickable, TRUE, FALSE );

		if ( m_bCurrentArea && m_CurrentArea.index == LevelAreas [ i ].info.index )
		{
			CPoint pt = LevelAreas [ i ].pWnd->ComputeRoomPoint ( m_CurrentPosition.x, m_CurrentPosition.z );
			LevelAreas [ i ].pWnd->SetRoomPoint ( pt, &m_CurrentPosition.orientation );
		}

		LevelAreas [ i ].visible	= SW_NORMAL;
		BOOL bShow = LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );

		if ( m_bCurrentArea && LevelAreas [ i ].info.index == m_CurrentArea.index )
		{
			LevelAreas [ i ].pWnd->SetSelected ( TRUE );
		}

		if ( m_bToolTip )
		{
			m_ToolTip.AddTool ( LevelAreas [ i ].pWnd, LPSTR_TEXTCALLBACK );
		}
	}

	//
	//		Rectangle
	RECT	rect;
	rect.top	= 0;
	rect.bottom	= rect.top;
	rect.left	= 0;
	rect.right	= rect.left;

	DWORD dwStyle = SS_OWNERDRAW;
	BOOL bCreated = 
		m_Rectangle.Create ( 
		szText,							//	LPCTSTR lpszCaption,
		dwStyle,						//	DWORD dwStyle,
		rect,							//   const RECT& rect,
		this,							//	CWnd* pParentWnd,
		-1  // LevelAreas [ i ].id				//	UINT nID 
	);
	BOOL bShow = m_Rectangle.ShowWindow ( SW_HIDE );

	//
	//	Show every thing greater than yTop
	ShowLevelBetween ( m_yMinTop, m_yMaxBottom );

	//
	const int marginBorder = 10;


	RECT clientRect;
	GetClientRect ( &clientRect );
	int iHeight = 0;
	if ( clientRect.bottom -  clientRect.top < maxBottom )
	{
		iHeight	= maxBottom - clientRect.bottom + marginBorder;
	}
	int iWidth = 0;
	if ( clientRect.right -  clientRect.left < maxRight )
	{
		iWidth	= maxRight - clientRect.right + marginBorder;
	}

	//
	RECT wndRect;
	GetWindowRect ( &wndRect );
	wndRect.bottom	= wndRect.bottom + iHeight;
	wndRect.right	= wndRect.right + iWidth;

	MoveWindow ( &wndRect );

	m_iCx		= wndRect.right - wndRect.left;
	m_iCy		= wndRect.bottom - wndRect.top;

	ShowMap();

	if ( m_bToolTip )
	{
		m_ToolTip.Activate(TRUE);
		m_ToolTip.AddTool ( &m_LevelTop, "List of Levels for Top\r\nValues are inverted\r\n" );
		m_ToolTip.AddTool ( &m_LevelBottom, "List of Levels for Bottom\r\nValues are inverted\r\n" );
		m_ToolTip.AddTool ( &m_East, "     East     \r\nNorth     South\r\n     West     \r\n" );
		m_ToolTip.AddTool ( &m_Refresh, "Rearrange Map\r\nThe Top are the room that are at Top\r\nYou can also resize the window\r\n" );
		m_ToolTip.AddTool ( &m_Area, "Filter to show where is a room\r\nYou can also move room\r\n" );
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);

		CString strVerticalView;
		strVerticalView.LoadString(IDS_VERTICALVIEW);
		if (!strVerticalView.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_STRING, IDM_VERTICALVIEW, strVerticalView);
		}
	}

	//

	m_bInitDone	= true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::ComputeRoomRect ( const RECT *pBase, RECT *pRect, TR_AREA *pArea )
{

	//
	const int xzDivider			= 150;
	const int margin			= 10;

	const int yDividerTop		= 225;
	const int yDividerLeft		= 225;

	long deltaTop				= abs(pArea->yBottom - m_yMaxBottom) / yDividerTop;
	long deltaLeft				= abs(pArea->yBottom - m_yMaxBottom) / yDividerLeft;

	deltaTop					= 0;
	deltaLeft					= 0;

	//	We could also use negative .yBottom to shift rectangles
	//	The lowest is yBottom : the highest is the building
	pRect->top			= pBase->bottom + pArea->x / xzDivider + margin + deltaTop + abs(m_xMin) / xzDivider;
	pRect->left			= pArea->z / xzDivider + margin + deltaLeft  + abs(m_zMin) / xzDivider;
	pRect->bottom		= pRect->top + pArea->xSectors * TR_SECTOR_SIZE / xzDivider;
	pRect->right		= pRect->left + pArea->zSectors * TR_SECTOR_SIZE / xzDivider;
}

//
/////////////////////////////////////////////////////////////////////////////
//	Direct Select Area
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::SelectAreaAndExit ( int area )
{
	m_iSelectedArea	= area;
	CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//	Search Point : if not found use the text
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::SelectAreaAndExit ( const char *pText )
{
	//
	POINT point;
	BOOL bPosition = GetCursorPos ( &point );
	int index = FindWindowAtPoint ( point );
	if ( index >= 0 )
	{
		m_iSelectedArea = LevelAreas [ index ].info.index;
	}
	else
	{
		if ( strlen(pText) > 0 )
		{
			m_iSelectedArea = atoi ( pText );
		}
		else
		{
			m_iSelectedArea	= -1;
		}
	}

	CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnSize(UINT nType, int cx, int cy)
{
	CTRXDialogBase::OnSize(nType, cx, cy);

	if ( cx < 0 || cy < 0 )
	{
		return;
	}

	//
	if ( CTRXGlobal::m_iDarkTheme != 0 )
	{
	}

	// TODO: Add Code Here
	if ( m_bInitDone )
	{
		double factorX = (double) cx / (double) m_iCx;
		double factorY = (double) cy / (double) m_iCy;
		for ( int i = 0; i < LevelAreasCount; i++ )
		{
			RECT rect;
			rect = LevelAreas [ i ].rect;

			rect.left	= (long) ( (double ) rect.left * factorX );
			rect.right	= (long) ( (double ) rect.right * factorX );

			rect.top	= (long) ( (double ) rect.top * factorY );
			rect.bottom	= (long) ( (double ) rect.bottom * factorY );

			LevelAreas [ i ].pWnd->MoveWindow ( &rect, TRUE );
			InvalidateRect ( &rect, TRUE );
		}

		m_Rectangle.ShowWindow ( SW_HIDE );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnMove(int x, int y)
{
	CTRXDialogBase::OnMove(x, y);

	// TODO: Add Code Here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::ShowOne ( int index )
{
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		if ( LevelAreas [ i ].info.index == index )
		{
			LevelAreas [ i ].visible	= SW_NORMAL;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
		else
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//	Show Level where yTop > minLevel
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::ShowLevelBetween ( int yTop, int yBottom, int room )
{
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		long yTopTemp		= LevelAreas [ i ].info.yTop;
		long yBottomTemp	= LevelAreas [ i ].info.yBottom;
		int index			= LevelAreas [ i ].info.index;
		if ( yTopTemp >= yTop && yBottomTemp <= yBottom )
		{
			LevelAreas [ i ].visible	= SW_NORMAL;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
		else
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//	Show Level where yTop > minLevel
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::ShowLevelLess ( int yTop, int yBottom, int room )
{
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		long yTopTemp		= LevelAreas [ i ].info.yTop;
		long yBottomTemp	= LevelAreas [ i ].info.yBottom;
		int index			= LevelAreas [ i ].info.index;
		if ( yTopTemp <= yTop && yBottomTemp <= yBottom )
		{
			LevelAreas [ i ].visible	= SW_NORMAL;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
		else
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}

		if ( room != -1 && room != index && yTopTemp == yTop && yBottomTemp == yBottom )
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//	Show Level where yTop > minLevel
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::ShowLevelGreater ( int yTop, int yBottom, int room )
{
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		long yTopTemp		= LevelAreas [ i ].info.yTop;
		long yBottomTemp	= LevelAreas [ i ].info.yBottom;
		int index			= LevelAreas [ i ].info.index;
		if ( yTopTemp >= yTop && yBottomTemp >= yBottom )
		{
			LevelAreas [ i ].visible	= SW_NORMAL;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
		else
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}

		if ( room != -1 && room != index && yTopTemp == yTop && yBottomTemp == yBottom )
		{
			LevelAreas [ i ].visible	= SW_HIDE;
			LevelAreas [ i ].pWnd->ShowWindow ( LevelAreas [ i ].visible );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//	Point in Client Coordinates
/////////////////////////////////////////////////////////////////////////////
int CTRXMapAreas::FindWindowAtCPoint( CPoint point)
{
	POINT pt = point;
	ClientToScreen ( &pt );
	return FindWindowAtPoint ( pt );
}

//
/////////////////////////////////////////////////////////////////////////////
//	Point in Screen Coordinates
/////////////////////////////////////////////////////////////////////////////
int CTRXMapAreas::FindWindowAtPoint( POINT point)
{
	for ( int i = LevelAreasCount - 1; i >= 0; i-- )
	{
		if ( LevelAreas [ i ].pWnd != NULL && LevelAreas [ i ].visible == SW_NORMAL )
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
void CTRXMapAreas::OnLButtonDown(UINT nFlags, CPoint point)
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
void CTRXMapAreas::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here
	m_iMoving	= -1;

	CTRXDialogBase::OnLButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnMouseMove(UINT nFlags, CPoint point)
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
void CTRXMapAreas::ShowMap()
{
		RECT rect;
	GetClientRect ( &rect );
	int cx	= rect.right - rect.left;
	int cy	= rect.bottom - rect.top;

	double factorX = (double) cx / (double) m_iCx;
	double factorY = (double) cy / (double) m_iCy;

	//
	CWnd *previousWIndow = NULL;

	//
	for ( int i = 0; i < LevelAreasCount; i++ )
	{
		RECT rect;
		rect = LevelAreas [ i ].rect;

		rect.left	= (long) ( (double ) rect.left * factorX );
		rect.right	= (long) ( (double ) rect.right * factorX );

		rect.top	= (long) ( (double ) rect.top * factorY );
		rect.bottom	= (long) ( (double ) rect.bottom * factorY );

		LevelAreas [ i ].pWnd->MoveWindow ( &rect, TRUE );
		InvalidateRect ( &rect, TRUE );

		//
		int x = LevelAreas [ i ].rect.left;
		int y = LevelAreas [ i ].rect.top;
		int cx = LevelAreas [ i ].rect.right - LevelAreas [ i ].rect.left;
		int cy = LevelAreas [ i ].rect.bottom - LevelAreas [ i ].rect.top;
		LevelAreas [ i ].pWnd->SetWindowPos ( previousWIndow, x, y, cx, cy, (UINT) HWND_TOP );
		previousWIndow	= LevelAreas [ i ].pWnd;

	}

	//
	m_SetManualCombo	= false;
	SetCurSel ( &m_LevelTop, m_yMinTop );
	SetCurSel ( &m_LevelBottom, m_yMaxBottom );
	m_SetManualCombo	= true;

	ShowLevelBetween ( m_yMinTop, m_yMaxBottom );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnBnClickedRefresh()
{
	// TODO: Add Code Here
	ShowMap();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::SetCurSel ( CComboBox *pBox, long value )
{
	char szText [ 32 ];
	for ( int i = 0; i < pBox->GetCount(); i++ )
	{
		int iRes = pBox->GetLBText ( i, szText );
		if ( atoi(szText) == value )
		{
			m_SetManualCombo	= false;
			pBox->SetCurSel ( i );
			m_SetManualCombo	= true;
			return;
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnChangeArea()
{
	static char szText [ 256 ];
	ZeroMemory ( szText, sizeof(szText) );
	m_Area.GetWindowText ( szText, sizeof(szText) );
	int room					= atoi ( szText );
	long lTop					= m_yMinTop;
	long lBottom				= m_yMaxBottom;

	const int rectangleMargin	= 4;

	//
	if ( strlen(szText) > 0 && room >= 0 && room < LevelAreasCount )
	{
		for ( int i = 0; i < LevelAreasCount; i++ )
		{
			if ( LevelAreas [ i ].info.index == room )
			{
				LevelAreas [ i ].selected			= TRUE;
				LevelAreas [ i ].pWnd->SetSelected ( LevelAreas [ i ].selected );
				LevelAreas [ i ].pWnd->RedrawWindow();

				lTop		= LevelAreas [ i ].info.yTop;
				lBottom		= LevelAreas [ i ].info.yBottom;

				RECT rect;
				LevelAreas [ i ].pWnd->GetWindowRect ( &rect );
				ScreenToClient ( &rect );

				//
				rect.top	-= rectangleMargin;
				rect.left	-= rectangleMargin;
				rect.right	+= rectangleMargin + 1;
				rect.bottom	+= rectangleMargin + 1;

				m_Rectangle.ShowWindow ( SW_HIDE );
				m_Rectangle.MoveWindow ( &rect, TRUE );
				m_Rectangle.ShowWindow ( SW_NORMAL );
			}
			else
			{
				LevelAreas [ i ].selected			= FALSE;
				LevelAreas [ i ].pWnd->SetSelected ( LevelAreas [ i ].selected );
				LevelAreas [ i ].pWnd->RedrawWindow();
			}
		}

		//
		ShowLevelGreater (  lTop, lBottom );
	}
	else
	{
		m_SetManualCombo	= false;
		SetCurSel ( &m_LevelTop, m_yMinTop );
		SetCurSel ( &m_LevelBottom, m_yMaxBottom );
		m_SetManualCombo	= true;
		for ( int i = 0; i < LevelAreasCount; i++ )
		{
			if ( m_bCurrentArea && LevelAreas [ i ].info.index == m_CurrentArea.index )
			{
				LevelAreas [ i ].selected			= TRUE;
				LevelAreas [ i ].pWnd->SetSelected ( LevelAreas [ i ].selected );
				LevelAreas [ i ].pWnd->RedrawWindow();

				lTop		= LevelAreas [ i ].info.yTop;
				lBottom		= LevelAreas [ i ].info.yBottom;
			}
			else
			{
				LevelAreas [ i ].selected			= FALSE;
				LevelAreas [ i ].pWnd->SetSelected ( LevelAreas [ i ].selected );
				LevelAreas [ i ].pWnd->RedrawWindow();
			}

		}

		//
		m_Rectangle.ShowWindow ( SW_HIDE );

		//
		ShowLevelBetween ( m_yMinTop, m_yMaxBottom );
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMapAreas::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add Your Code Here
	m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add Code Here

	CTRXDialogBase::OnRButtonDown(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnRButtonUp(UINT nFlags, CPoint point)
{
	//
	int index = FindWindowAtCPoint(point);

	//
	if ( m_pVerticalMap == NULL )
	{
		m_pVerticalMap	= new CTRXVerticalMap();
		if (m_pVerticalMap != NULL)
		{
			m_pVerticalMap->SetParent ( this );
			m_pVerticalMap->SetClickable ( m_bClickable );
			m_pVerticalMap->SetTombraiderLevelIndex ( m_iTombraiderFull, m_iLevelIndex );
			if ( index != -1 )
			{
				m_pVerticalMap->SetCurrentArea ( &m_CurrentArea, &m_CurrentPosition );
			}
			BOOL bCreated = m_pVerticalMap->Create(IDD_TRXVERTICALMAP);
			if ( ! bCreated )   //Create failed.
			{
				AfxMessageBox( "Error creating Dialog" );
			}    
			m_pVerticalMap->ShowWindow(SW_SHOW);
		}
	}

	if ( index != -1 )
	{
		if ( m_pVerticalMap != NULL )
		{
			m_pVerticalMap->SetSelectedArea ( &LevelAreas [ index ].info );
			m_pVerticalMap->BringWindowToTop();
		}
	}

	CTRXDialogBase::OnRButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnSelchangeLevelTop()
{
	// TODO: Add Code Here
	if ( m_SetManualCombo )
	{
		char szText [ 32 ];
		int iCurSel1 = m_LevelTop.GetCurSel();
		int iCurSel2 = m_LevelBottom.GetCurSel();
		if ( iCurSel1 >= 0 && iCurSel2 >= 0 )
		{
			int iRes = m_LevelTop.GetLBText ( iCurSel1, szText );
			long yTop = atol ( szText );
			iRes = m_LevelBottom.GetLBText ( iCurSel2, szText );
			long yBottom = atol ( szText );
			ShowLevelBetween ( yTop, yBottom );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnSelchangeLevelBottom()
{
	OnSelchangeLevelTop();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnOK()
{
	// TODO: Add Code Here

	CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnCancel()
{
	// TODO: Add Code Here

	CTRXDialogBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXMapAreas::DoModal()
{
	// TODO: Add Code Here

	INT_PTR result = CTRXDialogBase::DoModal();
	if ( result == IDOK )
	{
		return m_iSelectedArea;
	}

	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXMapAreas::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
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
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), 
			"\r\nYou can double click to select a Room\r\nOr move the square to see layers under.\r\n" );
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), 
			"Right Mouse Button shows Verical Views.\r\n" );
		pText->lpszText = szText;

		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::SetCurrentArea ( const TR_AREA *pArea, const TR_CUR_POSITION *pCurrentPosition )
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
void CTRXMapAreas::SetClickable ( BOOL bClickable )
{
	m_bClickable	= bClickable;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::SetTombraiderLevelIndex ( int iTombraider, int iLevelIndex )
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
void CTRXMapAreas::SetVerticalMap ( CTRXVerticalMap *pVerticalMap )
{
	m_pVerticalMap	= pVerticalMap;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add Code Here
	// TODO: Add Code Here
	 if ((nID & 0xFFF0) == IDM_VERTICALVIEW)
	 {
		if ( m_pVerticalMap == NULL )
		{
			m_pVerticalMap	= new CTRXVerticalMap();
			if (m_pVerticalMap != NULL)
			{
				m_pVerticalMap->SetParent ( this );
				m_pVerticalMap->SetClickable ( m_bClickable );
				m_pVerticalMap->SetTombraiderLevelIndex ( m_iTombraiderFull, m_iLevelIndex );
				BOOL bCreated = m_pVerticalMap->Create(IDD_TRXVERTICALMAP);
				if ( ! bCreated )   //Create failed.
				{
					AfxMessageBox( "Error creating Dialog" );
				}    
				m_pVerticalMap->ShowWindow(SW_SHOW);
				m_pVerticalMap->BringWindowToTop();
			}
		}
	 }

	CTRXDialogBase::OnSysCommand(nID, lParam);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXMapAreas::OnClose()
{
	// TODO: Add Code Here
	if ( m_pVerticalMap != NULL )
	{
		m_pVerticalMap->SetParent ( NULL );
		m_pVerticalMap->DestroyWindow();

		//
		delete m_pVerticalMap;
		m_pVerticalMap	= NULL;
	}

	//
	CTRXDialogBase::OnClose();
}
