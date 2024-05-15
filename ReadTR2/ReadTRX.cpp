#include "stdafx.h"

#include "windows.h"
#include "io.h"

#include "../TR2LevelFormat.h"
#include "../TR_Areas.h"

#include "ReadTRX.h"
#include "MCMemA.h"

#include "zlib.h"

//
/////////////////////////////////////////////////////////////////////////////
//	http://xproger.info/projects/OpenLara/trs.html
//
/////////////////////////////////////////////////////////////////////////////

// static BYTE	szBuffer [ 32 * 1024 * 1024 ];

// static BYTE	LevelDataCompressed [ 32 * 1024 * 1024 ];
// static BYTE	LevelDataUnCompressed [ 32 * 1024 * 1024 ];
static BYTE *pLevelData = NULL;

static	xint32_t Version; // version (4 bytes)
static	tr_colour Palette[256]; // 8-bit palette (768 bytes)
static	tr_colour4 Palette16[256]; //  (1024 bytes)
static	xuint32_t NumTextiles; 
static tr_textile8 Textile8[ 1 ];
static tr_textile16 Textile16[ 1 ];
static xuint32_t Unused; // 32-bit unused value (4 bytes)
static xuxint16_t NumRooms; // number of rooms (2 bytes)
static tr_room_info info;           // Where the room exists, in world coordinates
static xuint32_t NumDataWords;       // Number of data words (xuxint16_t's)
static xuxint16_t Data[1 /* NumDataWords */ ]; // The raw data from which the rest of this is derived

//	tr_room_data : In Data
static xint16_t NumVertices;                   // Number of vertices in the following list
static tr123_room_vertex Vertices[1 /* NumVertices */]; // List of vertices (relative coordinates)
static xint16_t NumRectangles;                 // Number of textured rectangles
static tr_face4 Rectangles[1 /*NumRectangles*/ ];    // List of textured rectangles
static xint16_t NumTriangles;                  // Number of textured triangles
static tr_face3 Triangles[1 /* NumTriangles */ ];      // List of textured triangles
static xint16_t NumSprites;                    // Number of sprites
static tr_room_sprite Sprites[1 /* NumSprites */ ];    // List of sprites

//
static xuxint16_t NumPortals;                 // Number of visibility portals to other rooms
static tr_room_portal Portals[1 /* NumPortals*/ ];  // List of visibility portals

static xuxint16_t NumZsectors;                                  // ``Width'' of sector list
static xuxint16_t NumXsectors;                                  // ``Height'' of sector list
static tr_room_sector SectorList[1 /* NumXsectors * NumZsectors*/ ];  // List of sectors in this room

static xint16_t AmbientIntensity;
static xint16_t AmbientIntensity2;  // Usually the same as AmbientIntensity
static xint16_t LightMode;

static xuxint16_t NumLights;                 // Number of point lights in this room
static tr_room_light Lights1[1 /* NumLights */ ];    // List of point lights
static tr3_room_light Lights3[1 /* NumLights */ ];    // List of point lights

static xuxint16_t NumStaticMeshes;                            // Number of static meshes
static tr_room_staticmesh StaticMeshes1[1 /* NumStaticMeshes */ ];   // List of static meshes
static tr3_room_staticmesh StaticMeshes3[1 /* NumStaticMeshes */ ];   // List of static meshes

static xint16_t AlternateRoom;
static xint16_t Flags;

static xuxint8_t WaterScheme;
static xuxint16_t WaterScheme16;
static xuxint8_t ReverbInfo;

static xuxint8_t Filler;  // Unused.
static xuxint16_t NumRoomTextiles; // number of non bumped room tiles (2 bytes)
static xuxint16_t NumObjTextiles; // number of object tiles (2 bytes)
static xuxint16_t NumBumpTextiles; // number of bumped room tiles (2 bytes)
static xuint32_t Textile32_UncompSize; // uncompressed size (in bytes) of the 32-bit textures chunk (4 bytes)
static xuint32_t Textile32_CompSize; // compressed size (in bytes) of the 32-bit textures chunk (4 bytes)
static xuint32_t Textile16_UncompSize; // uncompressed size (in bytes) of the 16-bit textures chunk (4 bytes)
static xuint32_t Textile16_CompSize; // compressed size (in bytes) of the 16-bit textures chunk (4 bytes)
static xuint32_t Textile32Misc_UncompSize; // uncompressed size (in bytes) of the 32-bit misc textures chunk (4 bytes), should always be 524288
static xuint32_t Textile32Misc_CompSize; // compressed size (in bytes) of the 32-bit misc textures chunk (4 bytes)
static xuint32_t LevelData_UncompSize; // uncompressed size (in bytes) of the level data chunk (4 bytes)
static xuint32_t LevelData_CompSize; // compressed size (in bytes) of the level data chunk (4 bytes)

static xuxint8_t AlternateGroup;  // Replaces Filler from TR3
static xuint32_t RoomColour;        // In ARGB format!

static xuxint16_t LaraType;
static xuxint16_t WeatherType;
static xuxint8_t Padding[28];

static char XELA[4];           // So-called "XELA landmark"
static xuint32_t RoomDataSize;

static xuint32_t Seperator;     // 0xCDCDCDCD (4 bytes)

static xuint32_t EndSDOffset;
static xuint32_t StartSDOffset;

static xuint32_t Separator;     // Either 0 or 0xCDCDCDCD

static xuint32_t EndPortalOffset;
static xuint32_t NumRoomTriangles;
static xuint32_t NumRoomRectangles;

static xuint32_t LightDataSize;
static xuint32_t NumLights2;    // Always same as NumLights

static xint32_t RoomYTop;
static xint32_t RoomYBottom;

static xuint32_t NumLayers;

static xuint32_t LayerOffset;
static xuint32_t VerticesOffset;
static xuint32_t PolyOffset;
static xuint32_t PolyOffset2;   // Same as PolyOffset

static xuint32_t NumVertices32;
static xuxint16_t Separator16;

static 	tr_room_sector *AllSectors = NULL;


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int PrintStdout ( const char *format, va_list argptr )
{
	int result = 0;
#ifdef _DEBUG
	result = vfprintf ( stdout, format, argptr );
#endif
	return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int PrintStdout ( const char *format, ... )
{
	va_list argptr;
	va_start(argptr, format);
	int result = PrintStdout ( format, argptr );
	va_end(argptr);
	return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static size_t ReadChunk ( void *pBuffer, size_t iLen, FILE *hFile )
{
	if ( iLen >= 0 )
	{
		size_t iRead = fread (  pBuffer, 1, iLen, hFile );
		if ( iRead <= 0 )
		{
			PrintStdout ( "Error Reading\r\n" );
		}

		return iRead;
	}

	return iLen;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL ExtractData (	FILE *hOutputFile, int game,
					const char *pPathname, const char *pName, TR_MODE TRMode, const char *pPrefix,
					FCT_AddToCustomArea function )
{
	MCMemA memBuffer(32 * 1024 * 1024);
	MCMemA memLevelDataCompressed ( 32 * 1024 * 1024 );
	MCMemA memLevelDataUnCompressed ( 32 * 1024 * 1024 );

	BOOL  bResult = FALSE;

	static char szName [ MAX_PATH ];
	strcpy_s ( szName, sizeof(szName), pName );
	char *pDot = strchr(szName, '.' );
	if ( pDot != NULL )
	{
		*pDot = '\0';
	}

	FILE *hFile = NULL;
	size_t iRead = 0;

	fopen_s ( &hFile, pPathname, "rb" );
	if ( hFile )
	{
		iRead = ReadChunk (  &Version, sizeof(Version), hFile );
		PrintStdout ( "Version 0x%0X\n", Version );

		if ( TRMode == TRR1_MODE )
		{
			if ( Version != 0x20 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}
				return bResult;
			}
		}

		if ( TRMode == TRR2_MODE )
		{
			if ( Version != 0x002D )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}
				return bResult;
			}
		}

		if ( TRMode == TRR3_MODE )
		{
			if ( ( Version & 0xffff ) != 0x0038 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}
				return bResult;
			}
		}

		//
		if ( TRMode == TR4_MODE )
		{
			if ( memcmp ( &Version, "TR4", 3 ) != 0 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}
				return bResult;
			}
		}

		//
		if ( TRMode == TR5_MODE )
		{
			if ( memcmp ( &Version, "TR4", 3 ) != 0 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}
				return bResult;
			}
		}

		//
		ZeroMemory ( memBuffer.ptr, memBuffer.len );
		ZeroMemory ( memLevelDataCompressed.ptr, memLevelDataCompressed.len );
		ZeroMemory ( memLevelDataUnCompressed.ptr, memLevelDataUnCompressed.len );

		//
		if ( TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			iRead = ReadChunk (  &Palette, sizeof(Palette), hFile );
			iRead = ReadChunk (  &Palette16, sizeof(Palette16), hFile );
		}

		if ( TRMode == TR4_MODE || TRMode == TR5_MODE)
		{
			iRead = ReadChunk (  &NumRoomTextiles, sizeof(NumRoomTextiles), hFile );
			iRead = ReadChunk (  &NumObjTextiles, sizeof(NumObjTextiles), hFile );
			iRead = ReadChunk (  &NumBumpTextiles, sizeof(NumBumpTextiles), hFile );

			iRead = ReadChunk (  &Textile32_UncompSize, sizeof(Textile32_UncompSize), hFile );
			iRead = ReadChunk (  &Textile32_CompSize, sizeof(Textile32_CompSize), hFile );
			iRead = ReadChunk (  memBuffer.ptr, Textile32_CompSize, hFile );

			iRead = ReadChunk (  &Textile16_UncompSize, sizeof(Textile16_UncompSize), hFile );
			iRead = ReadChunk (  &Textile16_CompSize, sizeof(Textile16_CompSize), hFile );
			iRead = ReadChunk (  memBuffer.ptr, Textile16_CompSize, hFile );

			iRead = ReadChunk (  &Textile32Misc_UncompSize, sizeof(Textile32Misc_UncompSize), hFile );
			iRead = ReadChunk (  &Textile32Misc_CompSize, sizeof(Textile32Misc_CompSize), hFile );
			iRead = ReadChunk (  memBuffer.ptr, Textile32Misc_CompSize, hFile );

			if ( TRMode == TR5_MODE )
			{
				iRead = ReadChunk (  &LaraType, sizeof(LaraType), hFile );
				iRead = ReadChunk (  &WeatherType, sizeof(WeatherType), hFile );
				iRead = ReadChunk (  &Padding, sizeof(Padding), hFile );
			}

			iRead = ReadChunk (  &LevelData_UncompSize, sizeof(LevelData_UncompSize), hFile );
			iRead = ReadChunk (  &LevelData_CompSize, sizeof(LevelData_CompSize), hFile );
			iRead = ReadChunk (  memLevelDataCompressed.ptr, LevelData_CompSize, hFile );

			//	Uncompress Level Data
			if ( LevelData_UncompSize != LevelData_CompSize )
			{
				int compressStatus = uncompress2( (Bytef *) memLevelDataUnCompressed.ptr, &LevelData_UncompSize, (Bytef *) memLevelDataCompressed.ptr, &LevelData_CompSize);
				if ( compressStatus != Z_OK )
				{
					PrintStdout ( "Error Uncompressing\r\n" );
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}
			}
			else
			{
				memcpy ( memLevelDataUnCompressed.ptr, memLevelDataCompressed.ptr, LevelData_UncompSize );
			}
		}

		//
		if ( TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			iRead = ReadChunk (  &NumTextiles, sizeof(NumTextiles), hFile );
			PrintStdout ( "NumTextiles %ld 0x%lX\n", NumTextiles, NumTextiles );
			iRead = ReadChunk (  memBuffer.ptr, sizeof(tr_textile8) * NumTextiles, hFile );
		}

		if ( TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			iRead = ReadChunk (  memBuffer.ptr, sizeof(tr_textile16) * NumTextiles, hFile );
		}

		if ( TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			iRead = ReadChunk (  &Unused, sizeof(Unused), hFile );
			PrintStdout ( "Unused %ld 0x%lX\n", Unused, Unused );
		}

		//
		if ( TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			iRead = ReadChunk (  &NumRooms, sizeof(NumRooms), hFile );
			PrintStdout ( "NumRooms %d 0x%X\n", NumRooms, NumRooms );
		}

		//
		if ( hOutputFile != NULL )
		{
			fprintf ( hOutputFile, "static TR_AREA %s%s [] =\n{\n", pPrefix, szName );
		}

		//
		if ( TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE )
		{
			//
			if ( NumRooms <= 0 || NumRooms > 1024 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}

				return bResult;
			}

			//
			for ( int i = 0; i < NumRooms; i++ )
			{
				iRead = ReadChunk (  &info, sizeof(tr_room_info), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "info %d : x %ld z %ld ytop %ld ybottom %ld\n", i, info.x, info.z, info.yTop, info.yBottom );

				iRead = ReadChunk (  &NumDataWords, sizeof(NumDataWords), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "NumDataWords %d 0x%X\n", NumDataWords, NumDataWords );
				iRead = ReadChunk (  memBuffer.ptr, sizeof(Data) * NumDataWords, hFile );
				if ( iRead == 0 && NumDataWords != 0 )
				{
					break;
				}

				//
				iRead = ReadChunk (  &NumPortals, sizeof(NumPortals), hFile );
				PrintStdout ( "NumPortals %d 0x%X\n", NumPortals, NumPortals );
				iRead = ReadChunk (  memBuffer.ptr, sizeof(Portals) * NumPortals, hFile );
				if ( iRead == 0 && NumPortals != 0 )
				{
					break;
				}

				//	Sectors
				iRead = ReadChunk (  &NumZsectors, sizeof(NumZsectors), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "NumZsectors %d 0x%X\n", NumZsectors, NumZsectors );

				iRead = ReadChunk (  &NumXsectors, sizeof(NumXsectors), hFile );
				if ( iRead == 0 )
				{
					break;
				}

				//
				PrintStdout ( "NumXsectors %d 0x%X\n", NumXsectors, NumXsectors );

				//
				iRead = ReadChunk (  memBuffer.ptr, sizeof(SectorList) * NumZsectors * NumXsectors, hFile );
				if ( iRead == 0 && NumZsectors * NumXsectors != 0 )
				{
					break;
				}

				if ( NumXsectors > 256 || NumZsectors > 256 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				AllSectors = ( tr_room_sector * ) memBuffer.ptr;

				xint16_t minFloor	= 0x7fff;
				xint16_t maxFloor	= (xint16_t)0x8000;
				xint16_t minCeiling	= 0x7fff;
				xint16_t maxCeiling	= (xint16_t)0x8000;

				xint16_t avgFloor	= 0;
				xint16_t avgCeiling	= 0;

				int solidFloor		= 0;
				int solidCeiling	= 0;

				for ( int h = 0; h < NumXsectors; h++ )
				{
					for ( int w = 0; w < NumZsectors; w++ )
					{
						xint8_t floor		= AllSectors [ h * NumZsectors + w ].Floor;
						xint8_t ceiling		= AllSectors [ h * NumZsectors + w ].Ceiling;

						//
						if ( floor == -127 )
						{
							solidFloor++;
						}
						else
						{
							avgFloor		+= floor;
							if ( floor < minFloor )
							{
								minFloor	= floor;
							}
							if ( floor > maxFloor )
							{
								maxFloor	= floor;		// OK
							}
						}

						//
						if ( ceiling == -127 )
						{
							solidCeiling++;
						}
						else
						{
							avgCeiling		+= ceiling;
							if ( ceiling < minCeiling )
							{
								minCeiling	= ceiling;		//	OK
							}
							if ( ceiling > maxCeiling )
							{
								maxCeiling	= ceiling;
							}
						}
					}
				}

				if ( NumZsectors * NumXsectors > solidFloor )
				{
					avgFloor /= ( NumZsectors * NumXsectors - solidFloor );
				}
				else
				{
					avgFloor = 0;
				}

				if ( NumZsectors * NumXsectors > solidCeiling )
				{
					avgCeiling /= ( NumZsectors * NumXsectors - solidCeiling );
				}
				else
				{
					avgCeiling	= 0;
				}

				//
				long floorRatio		= ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
				long ceilingRatio	= ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

				//
				//
				iRead = ReadChunk (  &AmbientIntensity, sizeof(AmbientIntensity), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "AmbientIntensity %d 0x%X\n", AmbientIntensity, AmbientIntensity );

				if ( TRMode != TRR1_MODE )
				{
					iRead = ReadChunk (  &AmbientIntensity2, sizeof(AmbientIntensity2), hFile );
					PrintStdout ( "AmbientIntensity2 %d 0x%X\n", AmbientIntensity2, AmbientIntensity2 );
				}

				if ( TRMode != TRR1_MODE && TRMode != TRR3_MODE )
				{
					iRead = ReadChunk (  &LightMode, sizeof(LightMode), hFile );
					if ( iRead == 0 )
					{
						break;
					}
					PrintStdout ( "LightMode %d 0x%X\n", LightMode, LightMode );
				}

				iRead = ReadChunk (  &NumLights, sizeof(NumLights), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "NumLights %d 0x%X\n", NumLights, NumLights );
				if ( TRMode == TRR1_MODE )
				{
					iRead = ReadChunk (  memBuffer.ptr, sizeof(Lights1) * NumLights, hFile );
					if ( iRead == 0 && NumLights != 0 )
					{
						break;
					}
				}
				else
				{
					iRead = ReadChunk (  memBuffer.ptr, sizeof(Lights3) * NumLights, hFile );
					if ( iRead == 0 && NumLights != 0 )
					{
						break;
					}
				}

				iRead = ReadChunk (  &NumStaticMeshes, sizeof(NumStaticMeshes), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "NumStaticMeshes %d 0x%X\n", NumStaticMeshes, NumStaticMeshes );
				if ( TRMode == TRR1_MODE )
				{
					iRead = ReadChunk (  memBuffer.ptr, sizeof(StaticMeshes1) * NumStaticMeshes, hFile );
					if ( iRead == 0 && NumStaticMeshes != 0 )
					{
						break;
					}
				}
				else
				{
					iRead = ReadChunk (  memBuffer.ptr, sizeof(StaticMeshes3) * NumStaticMeshes, hFile );
				}

				iRead = ReadChunk (  &AlternateRoom, sizeof(AlternateRoom), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "AlternateRoom %d 0x%X\n", AlternateRoom, AlternateRoom );

				iRead = ReadChunk (  &Flags, sizeof(Flags), hFile );
				if ( iRead == 0 )
				{
					break;
				}
				PrintStdout ( "Flags %d 0x%X\n", Flags, Flags );

				if ( TRMode == TRR3_MODE )
				{
					iRead = ReadChunk (  &WaterScheme, sizeof(WaterScheme), hFile );
					if ( iRead == 0 )
					{
						break;
					}
					PrintStdout ( "WaterScheme %d 0x%X\n", WaterScheme, WaterScheme );

					iRead = ReadChunk (  &ReverbInfo, sizeof(ReverbInfo), hFile );
					if ( iRead == 0 )
					{
						break;
					}
					PrintStdout ( "ReverbInfo %d 0x%X\n", ReverbInfo, ReverbInfo );

					iRead = ReadChunk (  &Filler, sizeof(Filler), hFile );
					if ( iRead == 0 )
					{
						break;
					}
					PrintStdout ( "Filler %d 0x%X\n", Filler, Filler );
				}

				if ( NumXsectors <= 0 || NumZsectors <= 0 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				if ( hOutputFile != NULL )
				{
					fprintf ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
						i, info.x, info.z, info.yTop, info.yBottom, NumXsectors, NumZsectors, Flags,
						minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );
				}
				//	Store In Memory
				else if ( function != NULL )
				{
					TR_AREA trArea;
					ZeroMemory ( &trArea, sizeof(trArea) );

					trArea.index				= i;
					trArea.x					= info.x;
					trArea.z					= info.z;
					trArea.yTop					= info.yTop;
					trArea.yBottom				= info.yBottom;
					trArea.pText				= "";
					trArea.xSectors				= NumXsectors;
					trArea.zSectors				= NumZsectors;
					trArea.flags				= Flags;
					trArea.minFloor				= minFloor;
					trArea.maxFloor				= maxFloor;
					trArea.minCeiling			= minCeiling;
					trArea.maxCeiling			= maxCeiling;
					trArea.solidFloorRatio		= floorRatio;
					trArea.solidCeilingRatio	= ceilingRatio;
					trArea.game					= game;
					(*function)( &trArea );
				}

			}	// End For

		}	// end TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE

		//
		if ( TRMode == TR4_MODE )
		{
			pLevelData	= (BYTE*) memLevelDataUnCompressed.ptr;
			pLevelData	+= sizeof(xuint32_t);		//	Unused
			NumRooms	=	*( (xuxint16_t *) pLevelData );
			pLevelData	+= sizeof(NumRooms);
			BYTE *pCurrent = pLevelData;

			//
			if ( NumRooms <= 0 || NumRooms > 1024 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}

				return bResult;
			}

			for ( int i = 0; i < NumRooms; i++ )
			{
				tr4_room *pRoom			= ( tr4_room * ) pLevelData;
				tr_room_info *pInfo		= ( tr_room_info * ) pLevelData;
				pLevelData				+= sizeof(tr_room_info);

				NumDataWords			= *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(NumDataWords);
				pLevelData				+= NumDataWords * sizeof(xuxint16_t);

				pCurrent				= pLevelData;

				NumPortals				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumPortals);
				pLevelData				+= NumPortals * sizeof(tr_room_portal);

				NumZsectors				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumZsectors);

				NumXsectors				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumXsectors);

				//
				if ( NumXsectors > 256 || NumZsectors > 256 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				AllSectors = ( tr_room_sector * ) pLevelData;

				xint16_t minFloor	= 0x7fff;
				xint16_t maxFloor	= (xint16_t)0x8000;
				xint16_t minCeiling	= 0x7fff;
				xint16_t maxCeiling	= (xint16_t)0x8000;

				xint16_t avgFloor	= 0;
				xint16_t avgCeiling	= 0;

				int solidFloor		= 0;
				int solidCeiling	= 0;

				for ( int h = 0; h < NumXsectors; h++ )
				{
					for ( int w = 0; w < NumZsectors; w++ )
					{
						xint8_t floor		= AllSectors [ h * NumZsectors + w ].Floor;
						xint8_t ceiling		= AllSectors [ h * NumZsectors + w ].Ceiling;

						//
						if ( floor == -127 )
						{
							solidFloor++;
						}
						else
						{
							avgFloor		+= floor;
							if ( floor < minFloor )
							{
								minFloor	= floor;
							}
							if ( floor > maxFloor )
							{
								maxFloor	= floor;		// OK
							}
						}

						//
						if ( ceiling == -127 )
						{
							solidCeiling++;
						}
						else
						{
							avgCeiling		+= ceiling;
							if ( ceiling < minCeiling )
							{
								minCeiling	= ceiling;		//	OK
							}
							if ( ceiling > maxCeiling )
							{
								maxCeiling	= ceiling;
							}
						}
					}
				}

				if ( NumZsectors * NumXsectors > solidFloor )
				{
					avgFloor /= ( NumZsectors * NumXsectors - solidFloor );
				}
				else
				{
					avgFloor = 0;
				}

				if ( NumZsectors * NumXsectors > solidCeiling )
				{
					avgCeiling /= ( NumZsectors * NumXsectors - solidCeiling );
				}
				else
				{
					avgCeiling	= 0;
				}

				//
				long floorRatio		= ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
				long ceilingRatio	= ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

				//
				pLevelData				+= NumZsectors * NumXsectors * sizeof(tr_room_sector);

				RoomColour				= *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(RoomColour);

				NumLights				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumLights);

				pLevelData				+= NumLights * sizeof(tr4_room_light);

				NumStaticMeshes			= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumStaticMeshes);

				pLevelData				+= NumStaticMeshes * sizeof(tr3_room_staticmesh);

				AlternateRoom			= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(AlternateRoom);

				Flags					= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(Flags);

				WaterScheme				= *( ( xuxint8_t *) pLevelData );
				pLevelData				+= sizeof(WaterScheme);

				ReverbInfo				= *( ( xuxint8_t *) pLevelData );
				pLevelData				+= sizeof(ReverbInfo);

				AlternateGroup			= *( ( xuxint8_t *) pLevelData );
				pLevelData				+= sizeof(AlternateGroup);

				if ( NumXsectors <= 0 || NumZsectors <= 0 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				if ( hOutputFile != NULL )
				{
					fprintf ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
						i, pInfo->x, pInfo->z, pInfo->yTop, pInfo->yBottom, NumXsectors, NumZsectors, Flags,
						minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );
				}
				else if ( function != NULL )
				{
					//	Store In Memory
					TR_AREA trArea;
					ZeroMemory ( &trArea, sizeof(trArea) );

					trArea.index				= i;
					trArea.x					= pInfo->x;
					trArea.z					= pInfo->z;
					trArea.yTop					= pInfo->yTop;
					trArea.yBottom				= pInfo->yBottom;
					trArea.pText				= "";
					trArea.xSectors				= NumXsectors;
					trArea.zSectors				= NumZsectors;
					trArea.flags				= Flags;
					trArea.minFloor				= minFloor;
					trArea.maxFloor				= maxFloor;
					trArea.minCeiling			= minCeiling;
					trArea.maxCeiling			= maxCeiling;
					trArea.solidFloorRatio		= floorRatio;
					trArea.solidCeilingRatio	= ceilingRatio;
					trArea.game					= game;

					(*function)( &trArea );
				}
			}

		}


		//
		if ( TRMode == TR5_MODE )
		{
			pLevelData	= (BYTE*) memLevelDataUnCompressed.ptr;

			Unused		= * (xuint32_t * ) pLevelData;
			pLevelData += sizeof(xuint32_t);		//	Unused

			NumRooms	=	*( (xuxint16_t *) pLevelData );
			pLevelData	+= sizeof(NumRooms);

			xuxint16_t unk = *( (xuxint16_t *) pLevelData );
			pLevelData	+= sizeof(NumRooms);

			//
			if ( NumRooms <= 0 || NumRooms > 1024 )
			{
				bResult = FALSE;
				if ( hFile != NULL )
				{
					fclose ( hFile );
					hFile	= NULL;
				}

				return bResult;
			}

			BYTE *pCurrent = pLevelData;
			for ( int i = 0; i < NumRooms; i++ )
			{
				char szXELA [ 5 ] = "XELA";

				//
				if ( memcmp ( pLevelData, szXELA, 4 ) != 0 )
				{
					PrintStdout ( "No XELA\n" );
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				pLevelData				+= sizeof(XELA);

				RoomDataSize			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(RoomDataSize);

				pCurrent				= pLevelData;

				Seperator				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(Seperator);

				EndSDOffset				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(EndSDOffset);

				StartSDOffset			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(StartSDOffset);

				Separator				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(Separator);

				EndPortalOffset			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(EndPortalOffset);

				tr5_room_info *pInfo	= ( tr5_room_info * ) pLevelData;
				pLevelData				+= sizeof(tr5_room_info);

				NumZsectors				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumZsectors);

				NumXsectors				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumXsectors);

				//
				if ( NumXsectors > 256 || NumZsectors > 256 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				RoomColour				=  *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(RoomColour);

				NumLights				=  *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumLights);

				NumStaticMeshes			=  *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumStaticMeshes);

				ReverbInfo				=  *( ( xuxint8_t *) pLevelData );
				pLevelData				+= sizeof(ReverbInfo);

				AlternateGroup			=  *( ( xuxint8_t *) pLevelData );
				pLevelData				+= sizeof(AlternateGroup);

				WaterScheme16			=  *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(WaterScheme16);

				//		xuint32_t Filler[2];    // Both always 0x00007FFF
				pLevelData				+= sizeof(xuint32_t) * 2;
				//		xuint32_t Separator[2]; // Both always 0xCDCDCDCD
				pLevelData				+= sizeof(xuint32_t) * 2;
				//		xuint32_t Filler;       // Always 0xFFFFFFFF
				pLevelData				+= sizeof(xuint32_t);

				AlternateRoom			= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(AlternateRoom);

				Flags					= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(Flags);

				//	Unknown1
				pLevelData				+= sizeof(xuint32_t) * 3;

				Separator				= *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(Separator);

				//	Unknown4
				pLevelData				+= sizeof(xuxint16_t ) * 2;

				float RoomX				= *( ( float *) pLevelData );
				pLevelData				+= sizeof(RoomX );

				float RoomY				= *( ( float *) pLevelData );
				pLevelData				+= sizeof(RoomY );

				float RoomZ				= *( ( float *) pLevelData );
				pLevelData				+= sizeof(RoomZ );

				//    xuint32_t Separator[4]; // Always 0xCDCDCDCD
				//    xuint32_t Separator;    // 0 for normal rooms and 0xCDCDCDCD for null rooms
				//    xuint32_t Separator;    // Always 0xCDCDCDCD
				pLevelData				+= sizeof(xuint32_t)*6;

				NumRoomTriangles		= *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(NumRoomTriangles );

				NumRoomRectangles		= *( ( xuint32_t *) pLevelData );
				pLevelData				+= sizeof(NumRoomRectangles );

				Seperator				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(Seperator);

				LightDataSize			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(LightDataSize);

				NumLights2				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(NumLights2);

				//			xuint32_t Unknown6;
				pLevelData				+= sizeof(xuint32_t);

				RoomYTop				= *(xuint32_t *) pLevelData;
				float dfRoomYTop		= *(float *) pLevelData;
				pLevelData				+= sizeof(RoomYTop);

				RoomYBottom				= *(xuint32_t *) pLevelData;
				float dfRoomYBottom		= *(float *) pLevelData;
				pLevelData				+= sizeof(RoomYBottom);

				NumLayers				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(NumLayers);
    
				LayerOffset				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(LayerOffset);
    
				VerticesOffset			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(VerticesOffset);
    
				PolyOffset				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(PolyOffset);

				PolyOffset2				= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(PolyOffset2);

				NumVertices32			= *(xuint32_t *) pLevelData;
				pLevelData				+= sizeof(NumVertices32);

				//	 xuint32_t Separator[4];  // Always 0xCDCDCDCD
				pLevelData				+= sizeof(xuint32_t) * 4;

				pLevelData				+= sizeof(tr5_room_light ) * NumLights;

				//
				AllSectors = ( tr_room_sector * ) pLevelData;

				xint16_t minFloor	= 0x7fff;
				xint16_t maxFloor	= (xint16_t)0x8000;
				xint16_t minCeiling	= 0x7fff;
				xint16_t maxCeiling	= (xint16_t)0x8000;

				xint16_t avgFloor	= 0;
				xint16_t avgCeiling	= 0;

				int solidFloor		= 0;
				int solidCeiling	= 0;

				for ( int h = 0; h < NumXsectors; h++ )
				{
					for ( int w = 0; w < NumZsectors; w++ )
					{
						xint8_t floor		= AllSectors [ h * NumZsectors + w ].Floor;
						xint8_t ceiling		= AllSectors [ h * NumZsectors + w ].Ceiling;

						//
						if ( floor == -127 )
						{
							solidFloor++;
						}
						else
						{
							avgFloor		+= floor;
							if ( floor < minFloor )
							{
								minFloor	= floor;
							}
							if ( floor > maxFloor )
							{
								maxFloor	= floor;		// OK
							}
						}

						//
						if ( ceiling == -127 )
						{
							solidCeiling++;
						}
						else
						{
							avgCeiling		+= ceiling;
							if ( ceiling < minCeiling )
							{
								minCeiling	= ceiling;		//	OK
							}
							if ( ceiling > maxCeiling )
							{
								maxCeiling	= ceiling;
							}
						}
					}
				}

				if ( NumZsectors * NumXsectors > solidFloor )
				{
					avgFloor /= ( NumZsectors * NumXsectors - solidFloor );
				}
				else
				{
					avgFloor = 0;
				}

				if ( NumZsectors * NumXsectors > solidCeiling )
				{
					avgCeiling /= ( NumZsectors * NumXsectors - solidCeiling );
				}
				else
				{
					avgCeiling	= 0;
				}

				//
				long floorRatio		= ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
				long ceilingRatio	= ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

				//
				pLevelData				+= NumZsectors * NumXsectors * sizeof(tr_room_sector);

				//
				NumPortals				= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(NumPortals);

				pLevelData				+= sizeof(tr_room_portal) * NumPortals;

				//
				Separator16 			= *( ( xuxint16_t *) pLevelData );
				pLevelData				+= sizeof(Separator16);

				//
				pLevelData				+= sizeof( tr3_room_staticmesh ) * NumStaticMeshes;

				pLevelData				+= sizeof( tr5_room_layer ) * NumLayers;

				//	xuxint8_t Faces[(NumRoomRectangles * sizeof(tr_face4) + NumRoomTriangles * (tr_face3)];
				pLevelData				+= NumRoomRectangles * sizeof(tr_face4) + NumRoomTriangles * sizeof(tr_face3);

				pLevelData				+= sizeof(tr5_room_vertex) * NumVertices32;

				if ( NumXsectors <= 0 || NumZsectors <= 0 )
				{
					bResult = FALSE;
					if ( hFile != NULL )
					{
						fclose ( hFile );
						hFile	= NULL;
					}

					return bResult;
				}

				//
				if ( hOutputFile != NULL )
				{
					fprintf ( hOutputFile, "\t//	Room X : %.3f - Y : %.3f - Z: %.3f - YTop : %.3f - YBottom : %.3f \n", RoomX, RoomY, RoomZ, dfRoomYTop, dfRoomYBottom );
					fprintf ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
						i, pInfo->x, pInfo->z, pInfo->yTop, pInfo->yBottom, NumXsectors, NumZsectors, Flags,
						minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );
					fflush ( hOutputFile );
				}
				else if ( function != NULL )
				{
					//	Store In Memory
					TR_AREA trArea;
					ZeroMemory ( &trArea, sizeof(trArea) );

					trArea.index				= i;
					trArea.x					= pInfo->x;
					trArea.z					= pInfo->z;
					trArea.yTop					= pInfo->yTop;
					trArea.yBottom				= pInfo->yBottom;
					trArea.pText				= "";
					trArea.xSectors				= NumXsectors;
					trArea.zSectors				= NumZsectors;
					trArea.flags				= Flags;
					trArea.minFloor				= minFloor;
					trArea.maxFloor				= maxFloor;
					trArea.minCeiling			= minCeiling;
					trArea.maxCeiling			= maxCeiling;
					trArea.solidFloorRatio		= floorRatio;
					trArea.solidCeilingRatio	= ceilingRatio;
					trArea.game					= game;

					(*function)( &trArea );
				}

				//
				pLevelData				= pCurrent + RoomDataSize;
			}

		}

		if ( hOutputFile != NULL )
		{
			fprintf ( hOutputFile, "\t{ -1, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, -1, }, // End\n" );
			fprintf ( hOutputFile, "};\n\n" );
		}
		else
		{
			//	Store End In Memory
		}

		bResult = TRUE;

		fclose ( hFile );
		hFile	= NULL;
	}

	if ( hFile != NULL )
	{
		fclose ( hFile );
		hFile	= NULL;
	}

	return bResult;
}

