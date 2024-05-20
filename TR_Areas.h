#pragma once

#include <Windows.h>

//
#pragma warning (disable : 4477)
#pragma warning (disable : 4311)
#pragma warning (disable : 4302)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
#define	TR_SECTOR_SIZE		1024
#define	LOW_CEILING			1024
#define	HIGH_CEILING		4096
#define	ROOM_FLOOR_UNITS	256
#define	FROM_THE_GROUND		0x100

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
struct TR_AREA
{
	//	Index
	int			index;
	//	West To East
	long		x;
	//	South To North
	long		z;

	//	The Top of a building
	//	The sign is reverted
	long		yTop;
	//	The Ground of a building
	//	Normally Lara is at yBottom - v where v is between 0 and 1024 (0x400)
	//	W>e use 0x100
	//	The sign is reverted
	long		yBottom;			
	//
	const char	*pText;		//	Text

	//	Sectore to have real size we must multiply by TR_SECTOR_SIZE
	long		xSectors;			//	X Sectors
	//	Sectore to have real size we must multiply by TR_SECTOR_SIZE
	long		zSectors;			//	Y Sectors

	//	Flags
	WORD		flags;

	//	Max Floor : 256 units of height 
	//	4 is 1024 real world unit
	long		minFloor;
	long		maxFloor;

	//	Min Ceiling : 256 units of height 
	//	4 is 1024 real world unit
	long		minCeiling;
	long		maxCeiling;

	//	Solid Floor Ratio
	long		solidFloorRatio;

	//	Solid Ceiling Ratio
	long		solidCeilingRatio;

	//	Game number
	int			game;
};

//
//		Name And Structure
struct TRN_AREA
{
	const char	*pName;
	TR_AREA		*pArea;
	int			count;
};

//
/////////////////////////////////////////////////////////////////////////////
//	Extended for MAP and sort
/////////////////////////////////////////////////////////////////////////////
class CTRXRoomPicture;
class CTRXWallPicture;

struct TR_AREA_ROOM
{
	CTRXRoomPicture		*pWnd;
	UINT				id;
	RECT				rect;
	UINT				visible;
	POINT				point;
	BOOL				selected;
	TR_AREA				info;
};

struct TR_AREA_WALL
{
	CTRXWallPicture		*pWnd;
	UINT				id;
	RECT				rect;
	UINT				visible;
	POINT				point;
	BOOL				selected;
	TR_AREA				info;
};

//
/////////////////////////////////////////////////////////////////////////////
//	Current Position
/////////////////////////////////////////////////////////////////////////////
struct TR_CUR_POSITION
{
	long				x;
	long				y;
	long				z;
	double				orientation;
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
extern short FindAreaForCoordinates ( int tombraider, int levelIndex, long x, long y, long z );
extern BOOL EnumAreaForCoordinates (	int tombraider, int levelIndex, int index, TR_AREA *pArea );
extern BOOL InsideVertical ( long y, long yTop, long yBottom, bool bSpecial = false );

//	x : East West
//	z : South North
extern BOOL CheckAreaForCoordinates ( int tombraider, int levelIndex, int area, long x, long y, long z, bool bSpecial = false );

//	Get Table :! m_iTombraoder may be the full version 1 2 3 4 10 15 20...
extern TR_AREA *GetTRArea ( int tombraider, int levelIndex, int index );
extern int GetTRAreaCount ( int tombraider, int levelIndex );
extern const char *GetAreaLevelName ( int tombraider, int levelIndex );
extern int GetTRTableSize ( int tombraider );

//	Custom
#define	CUSTOM_GAME				99
#define	CUSTOM_LEVEL			1
#define	CUSTOM_LEVEL_INDEX		0

extern void InitCustomArea ();
extern void AddToCustomArea (TR_AREA *pArea);
extern BOOL IsCustomArea ( );
extern char *GetCustomLevelName();
extern void SetCustomLevelName( const char *pName );
typedef void (*FCT_AddToCustomArea)(TR_AREA *pArea);
