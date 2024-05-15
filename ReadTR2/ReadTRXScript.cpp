#include "stdafx.h"
#include <Windows.h>

#include "ReadTRXScript.h"
#include "MCMemA.h"


//
/////////////////////////////////////////////////////////////////////////////
//	-script "G:\Program Files (x86)\Core Design\trle\Script\SCRIPT.DAT"
//
/////////////////////////////////////////////////////////////////////////////

static struct tr4_script_header scriptHeader;
static struct tr4_script_levelheader scriptLevelHeader;
static struct tr4_lang_header langHeader;

//
static const int	maxLevels		= 64;

//	Length is scriptLevelHeader.NumTotalLevels * sizeof(xuxint16_t)
static xuxint16_t LevelpathStringOffsets [ maxLevels ];
//	Length is scriptLevelHeader.NumTotalLevels * sizeof(xuxint16_t)
static xuxint16_t LevelBlockDataOffsets [ maxLevels ];

//	Length is in scriptLevelHeader.LevelpathStringLen
static char LevelpathStringBlockData [ 0x7fff ];

//	Length is in scriptLevelHeader.LevelBlockLen
static BYTE LevelBlockData [ 0x7fff ];

//
xuxint16_t	LanguageBlockLen;
//	The Size will be LanguageBlockLen
//	String are zero terminated. The end will be an other zero
static BYTE LanguageBlockData [ 0x7fff ];

static char LanguageFilename [ MAX_PATH ];

//	From Languages
//	Lengtth will be langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings
static xuxint16_t StringOffsetTable[8192];

//	This will be used to search string in Level Data and setn the index
static const int StringTableMax	= 4096;
static char *StringTable [ StringTableMax ];

//
static 	FILE *hLogFile = NULL;

//
/**
0x81  Level           bitu8 stringIndex, xuxint16_t levelOptions, bitu8 pathIndex, bitu8 audio
0x82  [Title] Level   bitu8 pathIndex, xuxint16_t titleOptions, bitu8 audio
0x8C  Legend          bitu8 stringIndex
0x91  LoadCamera      bit32 srcX, bit32 srcY, bit32 srcZ, bit32 targX, bit32 targY, bit32 targZ, bitu8 room
0x89  Layer1          bitu8 red, bitu8 green, bitu8 blue, bit8 speed
0x8A  Layer2          bitu8 red, bitu8 green, bitu8 blue, bit8 speed
0x8E  Mirror          bitu8 room, bit32 xAxis
0x8F  Fog             bitu8 red, bitu8 green, bitu8 blue
0x84  Cut             bitu8 cutIndex
0x8B  UVrotate        bit8 speed
0x85  ResidentCut1    bitu8 cutIndex
0x86  ResidentCut2    bitu8 cutIndex
0x87  ResidentCut3    bitu8 cutIndex
0x88  ResidentCut4    bitu8 cutIndex
0x80  FMV             bitu8: 4 least significant bits represent the FMV index; 4 most significant bits (y) represent the FMV trigger bitfield as in y=1<->bit 8 set
0x92  ResetHUB        bitu8 levelIndex
0x90  AnimatingMIP    bitu8: 4 least significant bits represent animatingObjectIndex - 1; 4 most significant bits represent the distance
0x8D  LensFlare       xuxint16_t yClicks, bit16 zClicks, xuxint16_t xClicks, bitu8 red, bitu8 green, bitu8 blue
0x93  KEY_ITEM1       xuxint16_t stringIndex, xuxint16_t height, xuxint16_t size, xuxint16_t yAngle, xuxint16_t zAngle, xuxint16_t xAngle, xuxint16_t unknown
0x94  KEY_ITEM2   -=-  (All the same)
0x95  KEY_ITEM3   -=-
0x96  KEY_ITEM4   -=-
0x97  KEY_ITEM5   -=-
0x98  KEY_ITEM6   -=-
0x99  KEY_ITEM7   -=-
0x9A  KEY_ITEM8   -=-
0x9B  KEY_ITEM9   -=-
0x9C  KEY_ITEM10  -=-
0x9D  KEY_ITEM11  -=-
0x9E  KEY_ITEM12  -=-
0x9F  PUZZLE_ITEM1  -=-
0xA0  PUZZLE_ITEM2  -=-
0xA1  PUZZLE_ITEM3  -=-
0xA2  PUZZLE_ITEM4  -=-
0xA3  PUZZLE_ITEM5  -=-
0xA4  PUZZLE_ITEM6  -=-
0xA5  PUZZLE_ITEM7  -=-
0xA6  PUZZLE_ITEM8  -=-
0xA7  PUZZLE_ITEM9  -=-
0xA8  PUZZLE_ITEM10  -=-
0xA9  PUZZLE_ITEM11  -=-
0xAA  PUZZLE_ITEM12  -=-

0xAB  PICKUP_ITEM1  -=-
0xAC  PICKUP_ITEM2  -=-
0xAD  PICKUP_ITEM3  -=-
0xAE  PICKUP_ITEM4  -=-

0xAF  EXAMINE1 -=-
0xB0  EXAMINE2 -=-
0xB1  EXAMINE3 -=-

0xB2  KEY_ITEM1_COMBO1 -=-
0xB3  KEY_ITEM1_COMBO2 -=-
0xB4  KEY_ITEM2_COMBO1 -=-
0xB5  KEY_ITEM2_COMBO2 -=-
0xB6  KEY_ITEM3_COMBO1 -=-
0xB7  KEY_ITEM3_COMBO2 -=-
0xB8  KEY_ITEM4_COMBO1 -=-
0xB9  KEY_ITEM4_COMBO2 -=-
0xBA  KEY_ITEM5_COMBO1 -=-
0xBB  KEY_ITEM5_COMBO2 -=-
0xBC  KEY_ITEM6_COMBO1 -=-
0xBD  KEY_ITEM6_COMBO2 -=-
0xBE  KEY_ITEM7_COMBO1 -=-
0xBF  KEY_ITEM7_COMBO2 -=-
0xC0  KEY_ITEM8_COMBO1 -=-
0xC1  KEY_ITEM8_COMBO2 -=-

0xC2  PUZZLE_ITEM1_COMBO1 -=-
0xC3  PUZZLE_ITEM1_COMBO2  -=-
0xC4  PUZZLE_ITEM2_COMBO1  -=-
0xC5  PUZZLE_ITEM2_COMBO2  -=-
0xC6  PUZZLE_ITEM3_COMBO1  -=-
0xC7  PUZZLE_ITEM3_COMBO2  -=-
0xC8  PUZZLE_ITEM4_COMBO1  -=-
0xC9  PUZZLE_ITEM4_COMBO2  -=-
0xCA  PUZZLE_ITEM5_COMBO1  -=-
0xCB  PUZZLE_ITEM5_COMBO2  -=-
0xCC  PUZZLE_ITEM6_COMBO1  -=-
0xCD  PUZZLE_ITEM6_COMBO2 -=-
0xCE  PUZZLE_ITEM7_COMBO1 -=-
0xCF  PUZZLE_ITEM7_COMBO2 -=-
0xD0  PUZZLE_ITEM8_COMBO1 -=-
0xD1  PUZZLE_ITEM8_COMBO2 -=-

0xD2  PICKUP_ITEM1_COMBO1 -=-
0xD3  PICKUP_ITEM1_COMBO2 -=-
0xD4  PICKUP_ITEM2_COMBO1 -=-
0xD5  PICKUP_ITEM2_COMBO2 -=-
0xD6  PICKUP_ITEM3_COMBO1 -=-
0xD7  PICKUP_ITEM3_COMBO2 -=-
0xD8  PICKUP_ITEM4_COMBO1 -=-
0xD9  PICKUP_ITEM4_COMBO2 -=-

0x83  level-data-end  no arguments - this opcode appears at the end of every level (incl. title) block
 **/

/**
The xuxint16_t values levelOptions and titleOptions are actually bit fields containing several boolean options, and are laid out as follows (per-bit description):

    Bit 0 (0x0001) — YoungLara
    Bit 1 (0x0002) — Weather
    Bit 2 (0x0004) — Horizon
    Bit 4 (0x0010) — Layer2 used (?)
    Bit 3 (0x0008) — Horizon (has to be paired with 3)
    Bit 5 (0x0020) — Starfield
    Bit 6 (0x0040) — Lightning
    Bit 7 (0x0080) — Train
    Bit 8 (0x0100) — Pulse
    Bit 9 (0x0200) — ColAddHorizon
    Bit 10 (0x0400) — ResetHUB used
    Bit 11 (0x0800) — ColAddHorizon (has to be paired with 10)
    Bit 12 (0x1000) — Timer
    Bit 13 (0x2000) — Mirror used
    Bit 14 (0x4000) — RemoveAmulet
    Bit 15 (0x8000) — NoLevel
 **/

 /**
[Level]
Name=		Playable Tutorial Level
Legend=		The year 2000, somewhere in Egypt...
Horizon=	ENABLED
Layer1=		160,160,192,7
PuzzleCombo=	3,1,Cartouche Piece 1,	$0000,$0400,$0000,$0000,$0000,$0002
PuzzleCombo=	3,2,Cartouche Piece 2,	$0000,$0400,$0000,$0000,$0000,$0002
Puzzle=		3,Ba Cartouche,		$0000,$0400,$0000,$0000,$0000,$0002
Puzzle=		5,Eye Of Horus,		$0017,$0500,$0000,$0000,$0000,$0002
PuzzleCombo=	5,1,Eye Piece,		$0017,$0500,$0000,$0000,$0000,$0002
PuzzleCombo=	5,2,Eye Piece,		$0017,$0500,$0000,$0000,$0000,$0002
Puzzle=		6,The Hand Of Orion,	$0000,$0400,$8000,$c000,$0000,$0002
Puzzle=		8,The Hand Of Sirius,	$0000,$0400,$8000,$c000,$0000,$0002
LoadCamera=	0,0,0,0,0,0,255
LoadCamera=	11088,-1100,28896,11119,-1399,31486,0
Level=		DATA\TUT1,107

  **/

//	from 0x93
static char *labelData  [] =
{
	"Key=\t1",
	"Key=\t2",
	"Key=\t3",
	"Key=\t4",
	"Key=\t5",
	"Key=\t6",
	"Key=\t7",
	"Key=\t8",
	"Key=\t9",
	"Key=\t10",
	"Key=\t11",
	"Key=\t12",
	"Puzzle=\t1",
	"Puzzle=\t2",
	"Puzzle=\t3",
	"Puzzle=\t4",
	"Puzzle=\t5",
	"Puzzle=\t6",
	"Puzzle=\t7",
	"Puzzle=\t8",
	"Puzzle=\t9",
	"Puzzle=\t10",
	"Puzzle=\t11",
	"Puzzle=\t12",
	"Pickup=\t1",
	"Pickup=\t2",
	"Pickup=\t3",
	"Pickup=\t4",
	"Examine=\t1",
	"Examine=\t2",
	"Examine=\t3",
	"KeyCombo=\t1,1",
	"KeyCombo=\t1,2",
	"KeyCombo=\t2,1",
	"KeyCombo=\t2,2",
	"KeyCombo=\t3,1",
	"KeyCombo=\t3,2",
	"KeyCombo=\t4,1",
	"KeyCombo=\t4,2",
	"KeyCombo=\t5,1",
	"KeyCombo=\t5,2",
	"KeyCombo=\t6,1",
	"KeyCombo=\t6,2",
	"KeyCombo=\t7,1",
	"KeyCombo=\t7,2",
	"KeyCombo=\t8,1",
	"KeyCombo=\t8,2",
	"PuzzleCombo=\t1,1",
	"PuzzleCombo=\t1,2",
	"PuzzleCombo=\t2,1",
	"PuzzleCombo=\t2,2",
	"PuzzleCombo=\t3,1",
	"PuzzleCombo=\t3,2",
	"PuzzleCombo=\t4,1",
	"PuzzleCombo=\t4,2",
	"PuzzleCombo=\t5,1",
	"PuzzleCombo=\t5,2",
	"PuzzleCombo=\t6,1",
	"PuzzleCombo=\t6,2",
	"PuzzleCombo=\t7,1",
	"PuzzleCombo=\t7,2",
	"PuzzleCombo=\t8,1",
	"PuzzleCombo=\t8,2",
	"PickupCombo=\t1,1",
	"PickupCombo=\t1,2",
	"PickupCombo=\t2,1",
	"PickupCombo=\t2,2",
	"PickupCombo=\t3,1",
	"PickupCombo=\t3,2",
	"PickupCombo=\t4,1",
	"PickupCombo=\t4,2",
};

//
enum SectionEnum
{
	Section_None,
	Section_Option,
	Section_PSXExtensions,
	Section_PCExtensions,
	Section_Language,
	Section_Title,
	Section_Level,
};

//
enum StringsEnum
{
	StringsNone,
	StringsGeneric,
	StringsPSX,
	StringsPC,
};

//
//====================================================================================
//
//====================================================================================
static const char *__strstri ( const char *pString, const char *pSearched )
{
	if ( ( pString == NULL ) || ( pSearched == NULL ) )
	{
		return NULL;
	}

	if ( ( strlen ( pString ) == 0 ) || ( strlen ( pSearched ) == 0 ) )
	{
		return NULL;
	}

	size_t	iLen = strlen ( pSearched );
	if ( iLen == 0 )
	{
		return NULL;
	}

	//
	while ( *pString != L'\0' )
	{
		if ( _strnicmp ( pString, pSearched, iLen ) == 0 )
		{
			return pString;
		}

		pString++;
	}

	return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *TraceOperation ( void *block, size_t len )
{
	static char szText [ MAX_PATH ];
	ZeroMemory ( szText, sizeof(szText) );

	BYTE *pBlock = (BYTE *) block;

	for ( size_t i = 0; i < len; i++ )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "%02X ", pBlock [ i ] );
	}

	return szText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int strIcmpL ( const char *pString, const char *part )
{
	return _strnicmp( pString, part, strlen(part) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *StringToText ( char *pText )
{
	static char szTempString [ 0x7fff ];
	ZeroMemory ( szTempString, sizeof(szTempString) );

	//
	size_t index = 0;
	while ( *pText != '\0' )
	{
		if ( *pText < ' ' || *pText >= 0x7f )
		{
			switch ( *pText )
			{
				case '\n' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\\n" );
					index = strlen(szTempString);
					break;
				}
				case '\r' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\\r" );
					index = strlen(szTempString);
					break;
				}
				case '\t' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\\t" );
					index = strlen(szTempString);
					break;
				}
				default :
				{
					sprintf_s ( szTempString + strlen(szTempString), sizeof(szTempString) - strlen(szTempString), "\\x%02X", (BYTE) (*pText) );
					index = strlen(szTempString);
					break;
				}
			}

		}
		else
		{
			szTempString [ index ] = *pText;
			szTempString [ index + 1 ] = '\0';
			index++;
		}

		//
		pText++;
	}

	//
	return szTempString;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *RemoveFileType ( char *pText )
{
	for ( int i = (int) strlen(pText) - 1; i >= 0 ; i-- )
	{
		if ( pText [ i ] == '.' )
		{
			pText [ i ] = '\0';
			return pText;
		}

		if ( pText [ i ] == '\\' || pText [ i ] == ':' )
		{
			return pText;
		}
	}

	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL IsHexa ( BYTE parm )
{
	if ( parm >= '0' && parm <= '9' )
	{
		return TRUE;
	}
	if ( parm >= 'A' && parm <= 'F' )
	{
		return TRUE;
	}
	if ( parm >= 'a' && parm <= 'f' )
	{
		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL IsNumeric ( char *parms )
{
	if ( strlen(parms) > 0 )
	{
		while ( *parms != '\0' )
		{
			if ( *parms < '0' || *parms > '9' )
			{
				return FALSE;
			}
			parms++;
		}
		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL EndsWithI ( const char *pText, const char *pEnd )
{
	if ( strlen(pText) >= strlen(pEnd) )
	{
		if ( _stricmp ( pText + strlen(pText) - strlen(pEnd), pEnd ) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL EndsWith ( const char *pText, const char *pEnd )
{
	if ( strlen(pText) >= strlen(pEnd) )
	{
		if ( strcmp ( pText + strlen(pText) - strlen(pEnd), pEnd ) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL HasBooleanValue ( const char *pText )
{
	if ( EndsWithI ( pText, "ENABLED" ) )
	{
		return TRUE;
	}

	if ( EndsWithI ( pText, "DISABLED" ) )
	{
		return TRUE;
	}

	if ( EndsWithI ( pText, "ENABLE" ) )
	{
		return TRUE;
	}

	if ( EndsWithI ( pText, "DISABLE" ) )
	{
		return TRUE;
	}

	if ( EndsWithI ( pText, "TRUE" ) )
	{
		return TRUE;
	}

	if ( EndsWithI ( pText, "FALSE" ) )
	{
		return TRUE;
	}

	return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL IsOptionEnabled ( const char *pBuffer )
{
	if ( _stricmp ( pBuffer, "TRUE" ) == 0 )
	{
		return TRUE;
	}

	if ( _stricmp ( pBuffer, "ENABLED" ) == 0 )
	{
		return TRUE;
	}

	if ( _stricmp ( pBuffer, "ENABLE" ) == 0 )
	{
		return TRUE;
	}

	if ( _stricmp ( pBuffer, "FALSE" ) == 0 )
	{
		return FALSE;
	}

	if ( _stricmp ( pBuffer, "DISABLED" ) == 0 )
	{
		return FALSE;
	}

	if ( _stricmp ( pBuffer, "DISABLE" ) == 0 )
	{
		return FALSE;
	}
	return FALSE;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BYTE HexaToBYTE ( BYTE parm )
{
	BYTE value = 0;
	if ( parm >= '0' && parm <= '9' )
	{
		value = parm - '0';
	}
	else if ( parm >= 'A' && parm <= 'F' )
	{
		value = parm - 'A' + 10;
	}	
	else if ( parm >= 'a' && parm <= 'f' )
	{
		value = parm - 'a' + 10;
	}
	return value;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BYTE HexaToBYTE ( BYTE *parms, int &count )
{
	BYTE value = 0;
	count	= 0;
	for ( int i = 0; i < sizeof(value) * 2; i++ )
	{
		value	= value << 4;
		if ( ! IsHexa ( parms [ i ] ) )
		{
			return value;
		}

		value	= value | HexaToBYTE ( parms [ i ] );
		count++;
	}
	return value;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static WORD HexaToWORD ( BYTE *parms, int &count )
{
	WORD value = 0;
	count	= 0;
	for ( int i = 0; i < sizeof(value) * 2; i++ )
	{
		value	= value << 4;
		if ( ! IsHexa ( parms [ i ] ) )
		{
			return value;
		}

		value	= value | HexaToBYTE ( parms [ i ] );
		count++;
	}
	return value;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static DWORD HexaToDWORD ( BYTE *parms, int &count )
{
	DWORD value = 0;
	count	= 0;
	for ( int i = 0; i < sizeof(value) * 2; i++ )
	{
		if ( ! IsHexa ( parms [ i ] ) )
		{
			return value;
		}

		value	= value << 4;
		value	= value | HexaToBYTE ( parms [ i ] );
		count++;
	}
	return value;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BYTE GetBYTEValue ( char *parms )
{
	BYTE result;

	int count = 0;
	if ( *parms == '$' )
	{
		result = HexaToBYTE ( (BYTE*) parms + 1, count );
	}
	else
	{
		result = atoi ( parms );
	}

	return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static WORD GetWORDValue ( char *parms )
{
	WORD result;

	int count = 0;
	if ( *parms == '$' )
	{
		result = HexaToWORD ( (BYTE*) parms + 1, count );
	}
	else
	{
		result = atoi ( parms );
	}

	return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static DWORD GetDWORDValue ( char *parms )
{
	DWORD result;
	int count = 0;

	if ( *parms == '$' )
	{
		result = HexaToDWORD ( (BYTE*) parms + 1, count );
	}
	else
	{
		result = atol ( parms );
	}

	return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static xuxint16_t SearchStringIndex ( const char *pText, const char delimiter = '\0' )
{
	static char szSearched [ MAX_PATH ];
	strcpy_s ( szSearched, sizeof(szSearched), pText );
	if ( delimiter != '\0' )
	{
		char *pDelimiter = strchr ( szSearched, delimiter );
		if ( pDelimiter != NULL )
		{
			*pDelimiter	= '\0';
		}
	}

	for ( xuxint16_t i = 0; i < StringTableMax; i++ )
	{
		if ( StringTable [ i ] != NULL )
		{
			if ( _stricmp ( szSearched, StringTable [ i ] ) == 0 )
			{
				return i;
			}
		}
	}

	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Error String %s not found\n", pText );

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *TextToString ( char *pText )
{
	static char szTempString [ 0x7fff ];
	ZeroMemory ( szTempString, sizeof(szTempString) );

	//
	size_t index = 0;
	while ( *pText != '\0' )
	{
		if ( *pText == '\\' )
		{
			pText++;
			switch  ( *pText )
			{
				case 'n' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\n" );
					index	= strlen(szTempString);
					pText++;
					break;
				}
				case 'r' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\r" );
					index	= strlen(szTempString);
					pText++;
					break;
				}
				case 't' :
				{
					strcat_s ( szTempString, sizeof(szTempString), "\t" );
					index	= strlen(szTempString);
					pText++;
					break;
				}
				case 'x' :
				{
					pText++;
					int count = 0;
					BYTE val = HexaToBYTE((BYTE*) pText, count );
					szTempString [ index ] = val;
					szTempString [ index + 1 ] = '\0';
					index	= strlen(szTempString);
					pText	+= count;
					break;
				}
				default:
				{
					pText++;
					break;
				}
			}
		}
		else
		{
			szTempString [ index ] = *pText;
			szTempString [ index + 1 ] = '\0';
			index++;
			pText++;
		}
		//
	}

	return szTempString;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *RemoveCRLF( char *pText )
{
	if ( pText != NULL && strlen(pText) > 0 )
	{
		for ( int i = (int) strlen(pText) - 1; i >= 0; i++ )
		{
			if ( pText [ i ] == '\r' || pText [ i ] == '\n' )
			{
				pText [ i ] = '\0';
			}
			else
			{
				return pText;
			}
		}
	}
	return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static void Cleanup()
{
	for ( int i = 0; i < StringTableMax; i++ )
	{
		if ( StringTable [ i ] != NULL )
		{
			free (  StringTable [ i ] );
			 StringTable [ i ] = NULL;
		}
	}
}

/**
The xuxint16_t values levelOptions and titleOptions are actually bit fields containing several boolean options, and are laid out as follows (per-bit description):

    Bit 0 (0x0001) — YoungLara
    Bit 1 (0x0002) — Weather
    Bit 2 (0x0004) — Horizon
    Bit 4 (0x0010) — Layer2 used (?)
    Bit 3 (0x0008) — Horizon (has to be paired with 3)
    Bit 5 (0x0020) — Starfield
    Bit 6 (0x0040) — Lightning
    Bit 7 (0x0080) — Train
    Bit 8 (0x0100) — Pulse
    Bit 9 (0x0200) — ColAddHorizon
    Bit 10 (0x0400) — ResetHUB used
    Bit 11 (0x0800) — ColAddHorizon (has to be paired with 10)
    Bit 12 (0x1000) — Timer
    Bit 13 (0x2000) — Mirror used
    Bit 14 (0x4000) — RemoveAmulet
    Bit 15 (0x8000) — NoLevel
 **/
static const char *OptionLabel ( xuxint16_t option )
{
	static char szText [ MAX_PATH * 4 ];
	ZeroMemory ( szText, sizeof(szText) );

	if ( option & 0x01 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "YoungLara=\tTRUE\n" );
	}

	if ( option & 0x02 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Weather=\tTRUE\n" );
	}

	if ( option & 0x04 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Horizon=\tTRUE\n" );
	}

	if ( option & 0x08 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Horizon2=\tTRUE\n" );
	}

	if ( option & 0x10 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Layer2=\tTRUE\n" );
	}

	if ( option & 0x20 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Starfield=\tTRUE\n" );
	}

	if ( option & 0x40 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Lightning=\tTRUE\n" );
	}

	if ( option & 0x80 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Train=\tTRUE\n" );
	}

	if ( option & 0x100 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Pulse=\tTRUE\n" );
	}

	if ( option & 0x200 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "ColAddHorizon=\tTRUE\n" );
	}

	if ( option & 0400 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "ResetHUB=\tTRUE\n" );
	}

	if ( option & 0x800 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "ColAddHorizon2=\tTRUE\n" );
	}

	if ( option & 0x1000 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Timer=\tTRUE\n" );
	}

	if ( option & 0x2000 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "Mirror=\tTRUE\n" );
	}

	if ( option & 0x4000 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "RemoveAmulet=\tTRUE\n" );
	}

	if ( option & 0x8000 )
	{
		sprintf_s ( szText + strlen(szText), sizeof(szText) - strlen(szText), "NoLevel=\tTRUE\n" );
	}

	return szText;
}

//
/////////////////////////////////////////////////////////////////////////////
//	http://xproger.info/projects/OpenLara/trs.html
//
/////////////////////////////////////////////////////////////////////////////
BOOL ReadTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version )
{
	//	For First Language
	if ( iLang == 0 )
	{
		ZeroMemory ( StringTable, sizeof(StringTable) );
	}

	MCMemA memLanguageStrings(0x1000 * 0x40);
	MCMemA memString ( 0x7fff );

	static char	szLanguage [ MAX_PATH ];
	if ( strlen(pDirectory) > 0 )
	{
		strcpy_s ( szLanguage, sizeof(szLanguage), pDirectory );
		strcat_s ( szLanguage, sizeof(szLanguage), "\\" );
		strcat_s ( szLanguage, sizeof(szLanguage), pFilename );
	}
	else
	{
		strcpy_s ( szLanguage, sizeof(szLanguage), pFilename );
	}

	//
	static char szOutputFilename [ MAX_PATH ];

	//
	//	Treat Language
	BOOL bResult	= FALSE;

	//
	FILE *hInpFile = NULL;
	FILE *hOutFile = NULL;

	//
	fopen_s ( &hInpFile, szLanguage, "rb" );
	if ( hInpFile == NULL )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szLanguage );
		return bResult;
	}

	//
	strcpy_s ( szOutputFilename, sizeof(szOutputFilename), szLanguage );
	strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".TXT" );
	fopen_s ( &hOutFile, szOutputFilename, "w" );
	if ( hOutFile == NULL )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szOutputFilename );
		fclose ( hInpFile );

		hInpFile	= NULL;

		Cleanup();

		return bResult;
	}

	//
	//
	size_t uRead = fread ( (char*) &langHeader, 1, sizeof(langHeader), hInpFile );
	if ( uRead != sizeof(langHeader) )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();
			
		return bResult;
	}

	//
	//	All the strings defined within {LANGUAGE}.DAT files are ASCII null-terminated strings.
	//	Every character (byte) contained in such a string is XOR-ed with byte 0xA5 (as mentioned above, it is done 
	fprintf ( hOutFile, "; NumGenericStrings : %d\n", langHeader.NumGenericStrings );
	fprintf ( hOutFile, "; NumPSXStrings : %d\n", langHeader.NumPSXStrings );
	fprintf ( hOutFile, "; NumPCStrings : %d\n", langHeader.NumPCStrings );

	fprintf ( hOutFile, "; GenericStringsLen : %d\n", langHeader.GenericStringsLen );
	fprintf ( hOutFile, "; PSXStringsLen : %d\n", langHeader.PSXStringsLen );
	fprintf ( hOutFile, "; PCStringsLen : %d\n", langHeader.PCStringsLen );

	int countStrings = langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;

	uRead = fread ( (char*) &StringOffsetTable, 1, sizeof(xuxint16_t)*countStrings, hInpFile );
	if ( uRead != sizeof(xuxint16_t)*countStrings )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();
			
		return bResult;
	}

	ZeroMemory ( memLanguageStrings.ptr, memLanguageStrings.len );
	uRead = fread ( (char*) memLanguageStrings.ptr, 1, memLanguageStrings.len, hInpFile );
	for ( size_t i = 0; i < uRead; i++ )
	{
		if ( memLanguageStrings.ptr [ i ] != 0x00 )
		{
			memLanguageStrings.ptr [ i ] = memLanguageStrings.ptr [ i ] ^ 0xA5;
		}
	}

	//	Generic
	fprintf ( hOutFile, "[Strings]\n" );
	int iBegining = 0;
	for ( int i = 0; i < langHeader.NumGenericStrings; i++ )
	{
		int len = langHeader.GenericStringsLen - StringOffsetTable [ iBegining + i ];
		if ( i + 1 < langHeader.NumGenericStrings )
		{
			len = StringOffsetTable [ iBegining + i + 1 ] - StringOffsetTable [ iBegining + i ];
		}
		ZeroMemory ( memString.ptr, memString.len );
		memcpy_s ( memString.ptr, memString.len, memLanguageStrings.ptr + StringOffsetTable [ iBegining + i ], len );
		fprintf ( hOutFile, "; Generic String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
		const char *pText = StringToText ( memString.ptr );
		if ( StringTable [ iBegining + i ] == NULL )
		{
			StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
			strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
		}

		fprintf ( hOutFile, "%s\n", pText );
	}
	fprintf ( hOutFile, "\n" );

	//	PSX
	fprintf ( hOutFile, "[PSXStrings]\n" );
	iBegining = langHeader.NumGenericStrings;
	for ( int i = 0; i < langHeader.NumPSXStrings; i++ )
	{
		int len = langHeader.GenericStringsLen + langHeader.PSXStringsLen - StringOffsetTable [ iBegining + i ];
		if ( i + 1 < langHeader.NumPSXStrings )
		{
			len = StringOffsetTable [ iBegining + i + 1 ] - StringOffsetTable [ iBegining + i ];
		}
		ZeroMemory ( memString.ptr, memString.len );
		memcpy_s ( memString.ptr, memString.len, memLanguageStrings.ptr + StringOffsetTable [ iBegining + i ], len );
		fprintf ( hOutFile, "; PSX String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
		const char *pText = StringToText ( memString.ptr );
		if ( StringTable [ iBegining + i ] == NULL )
		{
			StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
			strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
		}
		fprintf ( hOutFile, "%s\n", pText );
	}
	fprintf ( hOutFile, "\n" );

	//
	fprintf ( hOutFile, "[PCStrings]\n" );
	iBegining = langHeader.NumGenericStrings + langHeader.NumPSXStrings;
	for ( int i = 0; i < langHeader.NumPCStrings; i++ )
	{
		int len = langHeader.GenericStringsLen + langHeader.PSXStringsLen + langHeader.PCStringsLen - StringOffsetTable [ iBegining + i ];
		if ( i + 1 < langHeader.NumPCStrings )
		{
			len = StringOffsetTable [ iBegining + i + 1 ] - StringOffsetTable [ iBegining + i ];
		}
		ZeroMemory ( memString.ptr, memString.len );
		memcpy_s ( memString.ptr, memString.len, memLanguageStrings.ptr + StringOffsetTable [ iBegining + i ], len );
		fprintf ( hOutFile, "; PC String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
		const char *pText = StringToText ( memString.ptr );
		if ( StringTable [ iBegining + i ] == NULL )
		{
			StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
			strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
		}
		fprintf ( hOutFile, "%s\n", pText );
	}
	fprintf ( hOutFile, "\n" );

	//
	fclose ( hOutFile );
	fclose ( hInpFile );

	hOutFile	= NULL;
	hInpFile	= NULL;

	bResult = TRUE;

	return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL TreatLevelData ( FILE *hOutFile, xuxint16_t offset, int len, int version )
{
	//
	bool bTitle = false;

	//
	//	Search if Title
	for ( int step = 1; step <= 2; step++ )
	{

		//
		int x = offset;

		//
		if ( step == 2 )
		{
			if ( bTitle )
			{
				fprintf ( hOutFile, "[Title]\n" );
			}
			else
			{
				fprintf ( hOutFile, "[Level]\n" );
			}
		}

		//
		while (  x < len )
		{
			switch ( LevelBlockData [ x ] )
			{
				//	FMV
				//	bitu8 val
				case 0x80 :
				{
					LevelDatax80 *pArguments = ( LevelDatax80 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "FMV=\t$%02X\n", pArguments->val );
					x += sizeof(LevelDatax80);
					break;
				}

				//	Level
				//	bitu8 stringIndex; xuxint16_t levelOptions; bitu8 pathIndex; bitu8 audio;
				case 0x81 :
				{
					LevelDatax81 *pArguments = ( LevelDatax81 * ) ( & LevelBlockData [ x ] );
					const char *pString = StringTable [ pArguments->stringIndex ];
					if ( step == 2 ) fprintf ( hOutFile, "Name=\t%s\n", pString );
					const char *pathString = "";
					if ( pArguments->pathIndex >= 0 && pArguments->pathIndex < maxLevels )
					{
						pathString = LevelpathStringBlockData + LevelpathStringOffsets [ pArguments->pathIndex ];
					}
					if ( step == 2 ) fprintf ( hOutFile, "; Level 0x81 args : %d,$%04X,%d,%d\n",
							pArguments->stringIndex, pArguments->levelOptions, pArguments->pathIndex, pArguments->audio );
					if ( step == 2 ) fprintf ( hOutFile, "Level=\t%s,%d\n", pathString, pArguments->audio );

					const char *pLabelOption = OptionLabel ( pArguments->levelOptions );
					if ( step == 2 ) fprintf ( hOutFile, "%s", pLabelOption );

					x += sizeof(LevelDatax81);
					break;
				}
				//	Title
				//	bitu8 pathIndex; xuxint16_t titleOptions; bitu8 audio;
				case 0x82 :
				{
					bTitle = true;

					//
					LevelDatax82 *pArguments = ( LevelDatax82 * ) ( & LevelBlockData [ x ] );
					const char *pathString = "";
					if ( pArguments->pathIndex >= 0 && pArguments->pathIndex < maxLevels )
					{
						pathString = LevelpathStringBlockData + LevelpathStringOffsets [ pArguments->pathIndex ];
						if ( step == 2 ) fprintf ( hOutFile, "; Level 0x82 args : %d,$%04X,%d\n", 
							pArguments->pathIndex, pArguments->titleOptions, pArguments->audio );
						if ( step == 2 ) fprintf ( hOutFile, "Level=\t%s,%d\n", pathString, pArguments->audio );
					}
					else
					{
						if ( step == 2 ) fprintf ( hOutFile, "; Level 0x82 args : %d,$%04X,%d\n", 
							pArguments->pathIndex, pArguments->titleOptions, pArguments->audio );
						if ( step == 2 ) fprintf ( hOutFile, "Level=\t%d,%d\n", pArguments->pathIndex, pArguments->audio );
					}

					const char *pLabelOption = OptionLabel ( pArguments->titleOptions );
					if ( step == 2 ) fprintf ( hOutFile, "%s", pLabelOption );

					x += sizeof(LevelDatax82);
					break;
				}
				case 0x83 :
				{
					LevelDatax83 *pArguments = ( LevelDatax83 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "; Level-Data-End Opcode $%02X\n", pArguments->opcode );
					x += sizeof(LevelDatax83);
					break;
				}
				//	bitu8 cutIndex;
				case 0x84 :
				{
					LevelDatax84 *pArguments = ( LevelDatax84 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "Cut=\t$%02X\n", pArguments->cutIndex );
					x += sizeof(LevelDatax84);
					break;
				}
				//	ResidentCut1
				case 0x85 :
				{
					LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "ResidentCut=\t1,$%02X\n", pArguments->cutIndex );
					x += sizeof(LevelDatax85);
					break;
				}
				case 0x86 :
				{
					LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "ResidentCut=\t2,$%02X\n", pArguments->cutIndex );
					x += sizeof(LevelDatax85);
					break;
				}
				case 0x87 :
				{
					LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "ResidentCut=\t3,$%02X\n", pArguments->cutIndex );
					x += sizeof(LevelDatax85);
					break;
				}
				case 0x88 :
				{
					LevelDatax84 *pArguments = ( LevelDatax84 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "ResidentCut=\t4,$%02X\n", pArguments->cutIndex );
					x += sizeof(LevelDatax84);
					break;
				}

				//	bitu8 red; bitu8 green; bitu8 blue; bit8 speed;
				case 0x89 :
				{
					LevelDatax89 *pArguments = ( LevelDatax89 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "Layer1=\t%d,%d,%d,%d\n",
										pArguments->red, pArguments->green, pArguments->blue, pArguments->speed  );
					x += sizeof(LevelDatax89);
					break;
				}

				case 0x8A :
				{
					LevelDatax89 *pArguments = ( LevelDatax89 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "Layer2=\t%d,%d,%d,%d\n",
										pArguments->red, pArguments->green, pArguments->blue, pArguments->speed  );
					x += sizeof(LevelDatax89);
					break;
				}

				//	speed
				case 0x8B :
				{
					LevelDatax8B *pArguments = ( LevelDatax8B * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "UVrotate=\t%d\n", pArguments->speed  );
					x += sizeof(LevelDatax8B);
					break;
				}
				//	bitu8 stringIndex;
				case 0x8C :
				{
					LevelDatax8C *pArguments = ( LevelDatax8C * ) ( & LevelBlockData [ x ] );
					const char *pString = StringTable [ pArguments->stringIndex ];
					if ( step == 2 ) fprintf ( hOutFile, "Legend=\t%s\n", pString  );
					x += sizeof(LevelDatax8C);
					break;
				}
				//	xuxint16_t yClicks; bit16 zClicks; xuxint16_t xClicks; bitu8 red; bitu8 green; bitu8 blue;
				case 0x8D :
				{
					LevelDatax8D *pArguments = ( LevelDatax8D * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "LensFlare=\t%d,%d,%d,$%02X,$%02X,$%02X\n", 
										pArguments->yClicks, pArguments->zClicks, pArguments->xClicks,
										pArguments->red, pArguments->green, pArguments->blue  );
					x += sizeof(LevelDatax8D);
					break;
				}
				//	bitu8 room; bit32 xAxis;
				case 0x8E :
				{
					LevelDatax8E *pArguments = ( LevelDatax8E * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "Mirror=\t%d,$%04lX\n", pArguments->room, pArguments->xAxis  );
					x += sizeof(LevelDatax8E);
					break;
				}
				//	bitu8 red; bitu8 green; bitu8 blue;
				case 0x8F :
				{
					LevelDatax8F *pArguments = ( LevelDatax8F * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "Fog=\t$%02X,$%02X,$%02X\n", pArguments->red, pArguments->green, pArguments->blue  );
					x += sizeof(LevelDatax8F);
					break;
				}
				//	bitu8 val;
				case 0x90 :
				{
					LevelDatax90 *pArguments = ( LevelDatax90 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "AnimatingMIP=\t$%02X\n", pArguments->val );
					x += sizeof(LevelDatax90);
					break;
				}
				//	bit32 srcX; bit32 srcY; bit32 srcZ; bit32 targX; bit32 targY; bit32 targZ; bitu8 room;
				case 0x91 :
				{
					LevelDatax91 *pArguments = ( LevelDatax91 * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "LoadCamera=\t%ld,%ld,%ld,%ld,%ld,%ld,%d\n", 
										pArguments->srcX, pArguments->srcY, pArguments->srcZ,
										pArguments->targX, pArguments->targY, pArguments->targZ,
										pArguments->room );
					x += sizeof(LevelDatax91);
					break;
				}
				//	bitu8 levelIndex;
				case 0x92 :
				{
					if ( version == 4 )
					{
						LevelDatax92 *pArguments = ( LevelDatax92 * ) ( & LevelBlockData [ x ] );
						if ( step == 2 ) fprintf ( hOutFile, "ResetHUB=\t$%02X\n", pArguments->levelIndex );
						x += sizeof(LevelDatax92);
					}
					else if ( version == 5 )
					{
						LevelDatax92_5 *pArguments = ( LevelDatax92_5 * ) ( & LevelBlockData [ x ] );
						if ( step == 2 )
						{
							fprintf ( hOutFile, "ResetHUB=\t$%02X,", pArguments->levelIndex );
							for ( int iX = 0; iX < sizeof(pArguments->other); iX++ )
							{
								if ( iX == sizeof(pArguments->other) - 1 )
								{
									fprintf ( hOutFile, "$%02X\n", pArguments->other[iX] );
								}
								else
								{
									fprintf ( hOutFile, "$%02X,", pArguments->other[iX] );
								}
							}
						}
						x += sizeof(LevelDatax92_5);
					}
					break;
				}
				//
				case 0xDA :
				{
					LevelDataxDA *pArguments = ( LevelDataxDA * ) ( & LevelBlockData [ x ] );
					if ( step == 2 ) fprintf ( hOutFile, "LoseItemAtStartup=\t%d\n", pArguments->itemNumber );
					x += sizeof(LevelDataxDA);
					break;
				}
				//
				default :
				{
					if ( LevelBlockData [ x ]  >= 0x93 && LevelBlockData [ x ] <= 0xd9 )
					{
						int index = (unsigned char ) ( LevelBlockData [ x ] - 0x93 );

						LevelDatax93 *pArguments = ( LevelDatax93 * ) ( & LevelBlockData [ x ] );
						const char *pString = StringTable [ pArguments->stringIndex ];
						if ( step == 2 ) fprintf ( hOutFile, "%s,%s,$%04X,$%04X,$%04X,$%04X,$%04X,$%04X\n", labelData[index],
											pString, 
											pArguments->height, pArguments->size,
											pArguments->yAngle, pArguments->zAngle, pArguments->xAngle,
											pArguments->unknown );
						x += sizeof(LevelDatax93);
					}
					else
					{
						if ( step == 2 ) fprintf ( hOutFile, "; OpcodeError $%02X\n", LevelBlockData [ x ] );
						x++;
					}
					break;
				}
			}
		}
	}

	//
	fprintf ( hOutFile, "\n" );

	return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//	Full Pathname
//
/////////////////////////////////////////////////////////////////////////////
BOOL ReadTRXScript ( const char *pathname, const char *pDirectory, int version )
{
	//
	ZeroMemory ( StringTable, sizeof(StringTable) );

	//
	BOOL bResult = FALSE;

	//
	static char szOutputFilename [ MAX_PATH ];

	//
	static char szLogFilename [ MAX_PATH ];
	strcpy_s ( szLogFilename, sizeof(szLogFilename), pathname );
	strcat_s ( szLogFilename, sizeof(szLogFilename), ".LOG" );

	//
	fopen_s ( &hLogFile, szLogFilename, "w" );

	//
	FILE *hInpFile = NULL;
	FILE *hOutFile = NULL;
	fopen_s ( &hInpFile, pathname, "rb" );
	if ( hInpFile == NULL )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", pathname );
		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;
		return bResult;
	}

	//
	strcpy_s ( szOutputFilename, sizeof(szOutputFilename), pathname );
	strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".TXT" );
	fopen_s ( &hOutFile, szOutputFilename, "w" );
	if ( hOutFile == NULL )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szOutputFilename );
		fclose ( hInpFile );

		hInpFile	= NULL;

		Cleanup();

		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;

		return bResult;
	}

	//
	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading scriptHeader %d\n", sizeof(scriptHeader) );
	size_t uRead = fread ( (char*) &scriptHeader, 1, sizeof(scriptHeader), hInpFile );
	if ( uRead != sizeof(scriptHeader) )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();

		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;

		return bResult;
	}

	//
	fprintf ( hOutFile, "[Options]\n" );
	fprintf ( hOutFile, "; Options : 0x%x\n", scriptHeader.Options );
	if ( scriptHeader.Options & 0x01 )
	{
		fprintf ( hOutFile, "FlyCheat=\tENABLED\n" );
	}
	else
	{
		fprintf ( hOutFile, "FlyCheat=\tDISABLED\n" );
	}
	if ( scriptHeader.Options & 0x02 )
	{
		fprintf ( hOutFile, "LoadSave=\tENABLED\n" );
	}
	else
	{
		fprintf ( hOutFile, "LoadSave=\tDISABLED\n" );
	}
	if ( scriptHeader.Options & 0x04 )
	{
		fprintf ( hOutFile, "Title=\tENABLED\n" );
	}
	else
	{
		fprintf ( hOutFile, "Title=\tDISABLED\n" );
	}
	if ( scriptHeader.Options & 0x08 )
	{
		fprintf ( hOutFile, "PlayAnyLevel=\tENABLED\n" );
	}
	else
	{
		fprintf ( hOutFile, "PlayAnyLevel=\tDISABLED\n" );
	}
	if ( scriptHeader.Options & 0x80 )
	{
		fprintf ( hOutFile, "DemoDisc=\tENABLED\n" );
	}
	else
	{
		fprintf ( hOutFile, "DemoDisc=\tDISABLED\n" );
	}

	fprintf ( hOutFile, "InputTimeout=\t%d\n", scriptHeader.InputTimeout );
	fprintf ( hOutFile, "Security=\t$%02X\n", scriptHeader.Security );
	fprintf ( hOutFile, "\n" );

	//
	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading scriptHeader %d\n", sizeof(scriptLevelHeader) );
	uRead = fread ( (char*) &scriptLevelHeader, 1, sizeof(scriptLevelHeader), hInpFile );
	if ( uRead != sizeof(scriptLevelHeader) )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();

		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;

		return bResult;
	}

	//
	fprintf ( hOutFile, "; NumTotalLevels : %d\n", scriptLevelHeader.NumTotalLevels );
	fprintf ( hOutFile, "; NumUniqueLevelPaths : %d\n", scriptLevelHeader.NumUniqueLevelPaths );
	fprintf ( hOutFile, "; LevelpathStringLen : %d\n", scriptLevelHeader.LevelpathStringLen );
	fprintf ( hOutFile, "; LevelBlockLen : %d\n", scriptLevelHeader.LevelBlockLen );

	fprintf ( hOutFile, "[PSXExtensions]\n" );
	fprintf ( hOutFile, "Level=\t%s\n", scriptLevelHeader.PSXLevelString );
	fprintf ( hOutFile, "Cut=\t%s\n", scriptLevelHeader.PSXCutString );
	fprintf ( hOutFile, "FMV=\t%s\n", scriptLevelHeader.PSXFMVString );
	fprintf ( hOutFile, "\n" );

	fprintf ( hOutFile, "[PCExtensions]\n" );
	fprintf ( hOutFile, "Level=\t%s\n", scriptLevelHeader.PCLevelString );
	fprintf ( hOutFile, "Cut=\t%s\n", scriptLevelHeader.PCCutString );
	fprintf ( hOutFile, "FMV=\t%s\n", scriptLevelHeader.PCFMVString );
	fprintf ( hOutFile, "\n" );

	//
	int nbLevels = scriptLevelHeader.NumTotalLevels;

	//
	//	Strings
	ZeroMemory ( &LevelpathStringOffsets, sizeof(LevelpathStringOffsets) );
	if ( nbLevels > 0 )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading LevelpathStringOffsets %d\n", sizeof(xuxint16_t)*nbLevels );
		uRead = fread ( (char*) &LevelpathStringOffsets, 1, sizeof(xuxint16_t)*nbLevels, hInpFile );
		if ( uRead != sizeof(xuxint16_t)*nbLevels )
		{
			fclose ( hOutFile );
			fclose ( hInpFile );

			hOutFile	= NULL;
			hInpFile	= NULL;

			Cleanup();

			if ( hLogFile != NULL ) fclose ( hLogFile );
			hLogFile	= NULL;

			return bResult;
		}
	}

	//
	ZeroMemory ( LevelpathStringBlockData, sizeof(LevelpathStringBlockData) );
	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading LevelpathStringBlockData %d\n", scriptLevelHeader.LevelpathStringLen );
	uRead = fread ( LevelpathStringBlockData, 1, scriptLevelHeader.LevelpathStringLen, hInpFile );
	if ( uRead != scriptLevelHeader.LevelpathStringLen )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();

		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;

		return bResult;
	}
		
	for ( int i = 0; i < nbLevels; i++ )
	{
		fprintf ( hOutFile, "; LevelpathStringOffsets %d : %d %s\n", i, LevelpathStringOffsets [ i ], LevelpathStringBlockData + LevelpathStringOffsets [ i ] );
	}

	//
	//
	ZeroMemory ( &LevelBlockDataOffsets, sizeof(LevelBlockDataOffsets) );
	if ( nbLevels > 0 )
	{
		if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading LevelBlockDataOffsets %d\n", sizeof(xuxint16_t)*nbLevels );
		uRead = fread ( (char*) &LevelBlockDataOffsets, 1, sizeof(xuxint16_t)*nbLevels, hInpFile );
		if ( uRead != sizeof(xuxint16_t)*nbLevels )
		{
			fclose ( hOutFile );
			fclose ( hInpFile );

			hOutFile	= NULL;
			hInpFile	= NULL;

			Cleanup();

			if ( hLogFile != NULL ) fclose ( hLogFile );
			hLogFile	= NULL;

			return bResult;
		}
	}

	//
	ZeroMemory ( LevelBlockData, sizeof(LevelBlockData) );
	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading LevelBlockData %d\n", scriptLevelHeader.LevelBlockLen );
	uRead = fread ( LevelBlockData, 1, scriptLevelHeader.LevelBlockLen, hInpFile );
	if ( uRead != scriptLevelHeader.LevelBlockLen )
	{
		fclose ( hOutFile );
		fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		Cleanup();

		if ( hLogFile != NULL ) fclose ( hLogFile );
		hLogFile	= NULL;

		return bResult;
	}


	//
	//	Finally Languages

	//
	fprintf ( hOutFile, "\n" );
	fprintf ( hOutFile, "; LANGUAGE.DAT will be treated as LANGUAGE.DAT.TXT by the write procedure\n" );
	fprintf ( hOutFile, "; So leave this section as it is\n" );
	fprintf ( hOutFile, "; The Index will not be treated : so order them correctly\n" );
	fprintf ( hOutFile, "[Language]\n" );

	ZeroMemory ( LanguageBlockData, sizeof(LanguageBlockData) );
	LanguageBlockLen = (xuxint16_t) fread ( LanguageBlockData, 1, sizeof(LanguageBlockData), hInpFile );
	if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Reading LanguageBlockData %d\n", LanguageBlockLen );
	char *pLang = (char*) LanguageBlockData;
	int iLang = 0;
	while ( *pLang != '\0' )
	{
		//	Next Generation
		if ( memcmp ( pLang, "NG", 2 ) == 0 )
		{
			fprintf ( hOutFile, "; Next Generation File.\n" );
			break;
		}
		fprintf ( hOutFile, "File=\t%d,%s\n", iLang, pLang );
		ReadTRXLanguage ( pLang, pDirectory, iLang, version );
		pLang += strlen ( pLang ) + 1;
		iLang ++;
	}
	fprintf ( hOutFile, "\n" );

	//	Treat After Language has been read
	for ( int i = 0; i < nbLevels; i++ )
	{
		int len = scriptLevelHeader.LevelBlockLen;
		if ( i + 1 < nbLevels )
		{
			len = LevelBlockDataOffsets [ i + 1 ];
		}
		fprintf ( hOutFile, "; LevelBlockDataOffsets %d : %d 0x%x (len=%d)\n", i, LevelBlockDataOffsets [ i ], LevelBlockDataOffsets [ i ], len - LevelBlockDataOffsets [ i ] );

		//
		//	Treat Level Data
		TreatLevelData ( hOutFile, LevelBlockDataOffsets [ i ],  len, version );
	}

	//
	fclose ( hOutFile );
	fclose ( hInpFile );

	hOutFile	= NULL;
	hInpFile	= NULL;

	bResult		= TRUE;

	//
	Cleanup();

	if ( hLogFile != NULL ) fclose ( hLogFile );
	hLogFile	= NULL;

	//
	return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//	File will be LANG.DAT : we will search with .TXT extension replacing or added to filename
/////////////////////////////////////////////////////////////////////////////
BOOL WriteTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version )
{
	//
	BOOL bResult = FALSE;

	MCMemA memLine(4096);

	//
	ZeroMemory ( StringOffsetTable, sizeof(StringOffsetTable) );

	//
	static char	szLanguage [ MAX_PATH ];
	if ( strlen(pDirectory) > 0 )
	{
		strcpy_s ( szLanguage, sizeof(szLanguage), pDirectory );
		strcat_s ( szLanguage, sizeof(szLanguage), "\\" );
		strcat_s ( szLanguage, sizeof(szLanguage), pFilename );
	}
	else
	{
		strcpy_s ( szLanguage, sizeof(szLanguage), pFilename );
	}

	//
	if ( __strstri ( szLanguage, ".TXT" ) == NULL )
	{
		strcat_s ( szLanguage, sizeof(szLanguage), ".TXT" );
	}

	//
	static char szOutputFilename [ MAX_PATH ];
	
	for ( int step = 1; step <= 2; step++ )
	{
		//
		FILE *hInpFile = NULL;
		FILE *hOutFile = NULL;

		//
		fopen_s ( &hInpFile, szLanguage, "r" );
		if ( hInpFile == NULL )
		{
			if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szLanguage );
			return bResult;
		}

		//
		//	Read All Strings
		if ( step == 1 )
		{
			ZeroMemory ( &langHeader, sizeof(langHeader) );
			StringsEnum stringsType = StringsNone;
			do
			{
				char *pBuffer = fgets ( memLine.ptr, memLine.len, hInpFile );
				if ( pBuffer != NULL )
				{
					RemoveCRLF ( pBuffer );

					//
					if ( *pBuffer == ';' )
					{
						continue;
					}
					else if ( *pBuffer == '\0' )
					{
						continue;
					}
					else if ( strIcmpL ( pBuffer, "[Strings]" ) == 0 )
					{
						stringsType	= StringsGeneric;
					}
					else if ( strIcmpL ( pBuffer, "[PSXStrings]" ) == 0 )
					{
						stringsType	= StringsPSX;
					}
					else if ( strIcmpL ( pBuffer, "[PCStrings]" ) == 0 )
					{
						stringsType	= StringsPC;
					}
					else
					{
						//	We Have a string
						int stringsCount	= langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;
						StringOffsetTable [ stringsCount ] = langHeader.GenericStringsLen + langHeader.PSXStringsLen + langHeader.PCStringsLen;

						//
						if ( StringTable [ stringsCount ] == NULL )
						{
							StringTable [ stringsCount ] = ( char * ) malloc ( strlen(pBuffer) + 1 );
							strcpy_s ( StringTable [ stringsCount ], strlen(pBuffer) + 1, pBuffer );
						}

						//
						char *pText = TextToString ( pBuffer );

						//
						switch ( stringsType )
						{
							case StringsGeneric :
							{
								langHeader.NumGenericStrings++;
								langHeader.GenericStringsLen += (xuxint16_t) strlen(pText) + 1;
								break;
							}
							case StringsPSX :
							{
								langHeader.NumPSXStrings++;
								langHeader.PSXStringsLen += (xuxint16_t) strlen(pText) + 1;
								break;
							}
							case StringsPC :
							{
								langHeader.NumPCStrings++;
								langHeader.PCStringsLen += (xuxint16_t) strlen(pText) + 1;
								break;
							}
						}
					}
				}
			}
			while ( ! feof(hInpFile) && ! ferror( hInpFile) );

		}
		//	Write All
		if ( step == 2 )
		{
			strcpy_s ( szOutputFilename, sizeof(szOutputFilename), szLanguage );
			strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".DAT" );
			fopen_s ( &hOutFile, szOutputFilename, "wb" );
			if ( hOutFile == NULL )
			{
				if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szOutputFilename );
				fclose ( hInpFile );

				hInpFile	= NULL;

				Cleanup();

				return bResult;
			}

			//	Write Language Header
			fwrite ( &langHeader, 1, sizeof(langHeader), hOutFile );

			//
			//	Write Offset Table
			int stringsCount	= langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;
			fwrite ( StringOffsetTable, 1, sizeof(xuxint16_t)*stringsCount, hOutFile );

			//
			do
			{
				char *pBuffer = fgets ( memLine.ptr, memLine.len, hInpFile );
				if ( pBuffer != NULL )
				{
					RemoveCRLF ( pBuffer );

					//
					if ( *pBuffer == ';' )
					{
						continue;
					}
					else if ( *pBuffer == '\0' )
					{
						continue;
					}
					else if ( strIcmpL ( pBuffer, "[Strings]" ) == 0 )
					{
						continue;
					}
					else if ( strIcmpL ( pBuffer, "[PSXStrings]" ) == 0 )
					{
						continue;
					}
					else if ( strIcmpL ( pBuffer, "[PCStrings]" ) == 0 )
					{
						continue;
					}

					//
					char *pText = TextToString ( pBuffer );

					//	XOR with 0xA5
					for ( size_t i = 0; i < strlen(pText); i++ )
					{
						pText [ i ] = pText [ i ] ^ 0xA5;
					}

					//	Write
					fwrite ( pText, 1, strlen(pText) + 1, hOutFile );
				}
			}
			while ( ! feof(hInpFile) && ! ferror( hInpFile) );
		}

		if ( hInpFile != NULL ) fclose ( hInpFile );
		if ( hOutFile != NULL ) fclose ( hOutFile );

		hInpFile = NULL;
		hOutFile = NULL;

	}

	return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *SkipSpaceOrTab ( char *pBuffer )
{
	char *pTemp = pBuffer;

	//
	while ( *pTemp!= '\0' )
	{
		if ( *pTemp != ' ' && *pTemp != '\t'  )
		{
			return pTemp;
		}
		pTemp++;
	}

	return pTemp;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *SkipToValue ( char *pBuffer )
{
	char *pTemp = pBuffer;

	pTemp = strchr(pBuffer,'=' );
	if ( pTemp == NULL )
	{
		pTemp = pBuffer;
	}
	else
	{
		pTemp++;
	}

	return SkipSpaceOrTab ( pTemp );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static char *SkipToNext ( char *pBuffer )
{
	char *pTemp = pBuffer;

	//
	while ( *pTemp != '\0' )
	{
		if ( *pTemp == ',' )
		{
			return SkipSpaceOrTab ( pTemp + 1 );
		}
		pTemp++;
	}

	return pTemp;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL WriteTRXScript ( const char *pathname, const char *pDirectory, int version )
{
	BOOL bResult = FALSE;

	MCMemA memLine(4096);

	//
	SectionEnum	currentSection = Section_None;

	//
	ZeroMemory ( StringTable, sizeof(StringTable) );

	//
	static char szOutputFilename [ MAX_PATH ];

	//
	ZeroMemory ( &scriptHeader, sizeof(scriptHeader) );

	//	Extension
	ZeroMemory ( &scriptLevelHeader, sizeof(scriptLevelHeader) );

	//
	ZeroMemory ( LevelpathStringOffsets, sizeof(LevelpathStringOffsets) );
	ZeroMemory ( LevelpathStringBlockData, sizeof(LevelpathStringBlockData) );

	ZeroMemory ( LevelBlockData, sizeof(LanguageBlockData) );
	ZeroMemory ( LevelBlockDataOffsets, sizeof(LevelBlockDataOffsets) );

	//
	ZeroMemory ( LanguageBlockData, sizeof(LanguageBlockData) );
	LanguageBlockLen	= 0;

	//
	scriptLevelHeader.NumTotalLevels		= 0;
	scriptLevelHeader.NumUniqueLevelPaths	= 0;
	scriptLevelHeader.LevelBlockLen			= 0;
	scriptLevelHeader.LevelpathStringLen	= 0;

	//
	int	LevelOffset = -1;
	int	TitleOffset = -1;
	int	nameIndex	= -1;

	//
	static char szLogFilename [ MAX_PATH ];
	strcpy_s ( szLogFilename, sizeof(szLogFilename), pathname );
	strcat_s ( szLogFilename, sizeof(szLogFilename), ".LOG" );

	//
	fopen_s ( &hLogFile, szLogFilename, "w" );

	//
	for ( int step = 1; step <= 2; step++ )
	{

		FILE *hInpFile = NULL;
		FILE *hOutFile = NULL;

		fopen_s ( &hInpFile, pathname, "r" );
		if ( hInpFile == NULL )
		{
			if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", pathname );
			if ( hLogFile != NULL ) fclose ( hLogFile );
			hLogFile	= NULL;
			return FALSE;
		}

		//
		if ( step == 2 )
		{
			strcpy_s ( szOutputFilename, sizeof(szOutputFilename), pathname );
			strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".DAT" );
			fopen_s ( &hOutFile, szOutputFilename, "wb" );
			if ( hOutFile == NULL )
			{
				if ( hLogFile != NULL ) fprintf_s ( hLogFile, "File Open Error %s\n", szOutputFilename );
				fclose ( hInpFile );

				hInpFile	= NULL;

				Cleanup();

				if ( hLogFile != NULL ) fclose ( hLogFile );
				hLogFile	= NULL;
				return bResult;
			}
		}

		//
		//	Read Line
		bool bOptionsDone		= false;

		//
		bool bNeedEndOfLevel	= false;

		//
		do
		{
			char *pBuffer = fgets ( memLine.ptr, memLine.len,  hInpFile );
			if ( pBuffer != NULL )
			{
				RemoveCRLF ( pBuffer );

				//
				//	Detect Sections
				/////////////////////////////////////////
				if ( *pBuffer == ';' )
				{
					continue;
				}

				else if ( *pBuffer == '\0' )
				{
					continue;
				}

				else if ( strIcmpL(pBuffer,"[Options]" ) == 0 )
				{
					//
					//	No Previous : THis is the first Block

					//
					currentSection = Section_Option;

					//
					continue;
				}
				else if ( strIcmpL(pBuffer,"[PSXExtensions]" ) == 0 )
				{
					//
					//	Write Prevoious : Options
					if ( ! bOptionsDone )
					{ 
						if ( step == 2 )
						{
							if ( hLogFile ) fprintf_s ( hLogFile, "Writing scriptHeader %d\n", sizeof(scriptHeader) );
							fwrite ( &scriptHeader, 1, sizeof(scriptHeader), hOutFile );
						}
						bOptionsDone	= true;
					}

					//
					currentSection = Section_PSXExtensions;

					//
					continue;
				}
				else if ( strIcmpL(pBuffer,"[PCExtensions]" ) == 0 )
				{
					//
					//	Write Prevoious : Options
					if ( ! bOptionsDone )
					{ 
						if ( step == 2 )
						{
							if ( hLogFile ) fprintf_s ( hLogFile, "Writing scriptHeader %d\n", sizeof(scriptHeader) );
							fwrite ( &scriptHeader, 1, sizeof(scriptHeader), hOutFile );
						}
						bOptionsDone	= true;
					}

					//
					currentSection = Section_PCExtensions;

					//
					continue;
				}
				else if ( strIcmpL(pBuffer,"[Language]" ) == 0 )
				{
					//
					//	Write Prevoious : Extensions
					if ( step == 2 )
					{
						//
						//	Add 0x83 For The Last Level
						if ( bNeedEndOfLevel )
						{
							//
							LevelDatax83	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	=	0x83;
							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

							bNeedEndOfLevel	= false;
						}

						if ( hLogFile ) fprintf_s ( hLogFile, "Writing scriptLevelHeader %d\n", sizeof(scriptLevelHeader) );
						fwrite ( &scriptLevelHeader, 1, sizeof(scriptLevelHeader), hOutFile );

						//
						//	Write Prevoious : LevelpathStringBlockData
						if ( hLogFile ) fprintf_s ( hLogFile, "Writing LevelpathStringOffsets %d\n", sizeof(xuxint16_t)*scriptLevelHeader.NumTotalLevels );
						fwrite ( LevelpathStringOffsets, 1, sizeof(xuxint16_t)*scriptLevelHeader.NumTotalLevels, hOutFile );
						
						//
						if ( hLogFile ) fprintf_s ( hLogFile, "Writing LevelpathStringBlockData %d\n", scriptLevelHeader.LevelpathStringLen );
						fwrite ( LevelpathStringBlockData, 1, scriptLevelHeader.LevelpathStringLen, hOutFile );

						//	Write Previous : LevelBlockDataOffsets
						if ( hLogFile ) fprintf_s ( hLogFile, "Writing LevelBlockDataOffsets %d\n", sizeof(xuxint16_t)*scriptLevelHeader.NumTotalLevels );
						fwrite ( (char*) &LevelBlockDataOffsets, 1, sizeof(xuxint16_t)*scriptLevelHeader.NumTotalLevels, hOutFile );

						//	Then Data
						if ( hLogFile ) fprintf_s ( hLogFile, "Writing LevelBlockData %d\n", scriptLevelHeader.LevelBlockLen );
						fwrite ( LevelBlockData, 1, scriptLevelHeader.LevelBlockLen, hOutFile );
						//	Finally Languages
						//	Write One byte more
						if ( hLogFile ) fprintf_s ( hLogFile, "Writing LanguageBlockData %d\n", LanguageBlockLen );
						fwrite ( LanguageBlockData, 1, LanguageBlockLen, hOutFile );
					}

					//
					currentSection = Section_Language;

					//
					continue;
				}
				else if ( strIcmpL(pBuffer,"[Title]" ) == 0 )
				{
					//
					currentSection = Section_Title;
					if ( step == 1 )
					{
						//
						//	Add 0x83
						if ( bNeedEndOfLevel )
						{
							//
							LevelDatax83	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	=	0x83;
							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

							bNeedEndOfLevel	= false;
						}

						//
						LevelBlockDataOffsets [ scriptLevelHeader.NumTotalLevels ]  = scriptLevelHeader.LevelBlockLen;

						//
						bNeedEndOfLevel	= true;
						scriptLevelHeader.NumTotalLevels++;
						scriptLevelHeader.NumUniqueLevelPaths	= scriptLevelHeader.NumTotalLevels;
					}

					//
					continue;
				}
				else if ( strIcmpL(pBuffer,"[Level]" ) == 0 )
				{
					//
					currentSection = Section_Level;
					if ( step == 1 )
					{
						//
						//	Add 0x83
						if ( bNeedEndOfLevel )
						{
							//
							LevelDatax83	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	=	0x83;
							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

							bNeedEndOfLevel	= false;
						}

						//
						LevelBlockDataOffsets [ scriptLevelHeader.NumTotalLevels ]  = scriptLevelHeader.LevelBlockLen;

						//
						bNeedEndOfLevel	= true;

						//
						scriptLevelHeader.NumTotalLevels++;
						scriptLevelHeader.NumUniqueLevelPaths	= scriptLevelHeader.NumTotalLevels;
					}

					//
					continue;
				}

				//
				//	First Step Is Detailed Analysis
				/////////////////////////////////////////
				if ( step == 1 )
				{
					int count = 0;

					//
					//	Inside The [Option]
					if ( currentSection == Section_Option )
					{
						if ( strIcmpL ( pBuffer, "FlyCheat=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								scriptHeader.Options	|= 0x01;
							}
						}
						else if ( strIcmpL ( pBuffer, "LoadSave=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								scriptHeader.Options	|= 0x02;
							}
						}
						else if ( strIcmpL ( pBuffer, "Title=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								scriptHeader.Options	|= 0x04;
							}
						}
						else if ( strIcmpL ( pBuffer, "PlayAnyLevel=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								scriptHeader.Options	|= 0x08;
							}
						}
						else if ( strIcmpL ( pBuffer, "DemoDisc=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								scriptHeader.Options	|= 0x80;
							}
						}
						else if ( strIcmpL ( pBuffer, "InputTimeout=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							scriptHeader.InputTimeout	= GetDWORDValue ( pBuffer );
						}
						else if ( strIcmpL ( pBuffer, "Security=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							scriptHeader.Security	= GetBYTEValue ( pBuffer );
						}
						else
						{
							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Not Recognized %s\n", pBuffer );
						}
					}
					//
					//	Inside The [Level] or [Title]
					else if ( currentSection == Section_Level || currentSection == Section_Title )
					{
						/** 
							Bit 0 (0x0001) — YoungLara
							Bit 1 (0x0002) — Weather
							Bit 2 (0x0004) — Horizon
							Bit 4 (0x0010) — Layer2 used (?)
							Bit 3 (0x0008) — Horizon (has to be paired with 3)
							Bit 5 (0x0020) — Starfield
							Bit 6 (0x0040) — Lightning
							Bit 7 (0x0080) — Train
							Bit 8 (0x0100) — Pulse
							Bit 9 (0x0200) — ColAddHorizon
							Bit 10 (0x0400) — ResetHUB used
							Bit 11 (0x0800) — ColAddHorizon (has to be paired with 10)
							Bit 12 (0x1000) — Timer
							Bit 13 (0x2000) — Mirror used
							Bit 14 (0x4000) — RemoveAmulet
							Bit 15 (0x8000) — NoLevel
						 **/
						//	Bit 0 (0x0001) — YoungLara
						if ( strIcmpL ( pBuffer, "YoungLara=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0001;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0001;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Weather=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0002;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0002;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Horizon=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0004;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0004;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Horizon2=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0008;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0008;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Layer2=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0010;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0010;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Starfield=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0020;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0020;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Lightning=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0040;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0040;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Train=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0080;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0080;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						//	Bit 8 (0x0100) — Pulse
						else if ( strIcmpL ( pBuffer, "Pulse=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0100;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0100;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						//	Bit 9 (0x0200) — ColAddHorizon
						else if ( strIcmpL ( pBuffer, "ColAddHorizon=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0200;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0200;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						//	Bit 10 (0x0400) — ResetHUB used
						else if ( strIcmpL ( pBuffer, "ResetHUB=" ) == 0 && HasBooleanValue ( pBuffer )  )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0400;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0400;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "ColAddHorizon2=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x0800;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x0800;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						//	Bit 12 (0x1000) — Timer
						else if ( strIcmpL ( pBuffer, "Timer=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x1000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x1000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "Mirror=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x2000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x2000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "RemoveAmulet=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x4000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x4000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						else if ( strIcmpL ( pBuffer, "NoLevel=" ) == 0 && HasBooleanValue ( pBuffer ) )
						{
							pBuffer = SkipToValue ( pBuffer );
							if ( IsOptionEnabled ( pBuffer ) )
							{
								if ( LevelOffset >= 0 )
								{
									LevelDatax81	*pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
									pData->levelOptions	|= 0x8000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
								}
								else if ( TitleOffset >= 0 )
								{
									LevelDatax82	*pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
									pData->titleOptions	|= 0x8000;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
								}
								else
								{
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "No Level\n" );
								}
							}
						}
						//	0x80
						else if ( strIcmpL ( pBuffer, "FMV=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax80	data;
							ZeroMemory ( &data, sizeof(data) );

							data.opcode	=	0x80;
							data.val	=	GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);
							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding FMV %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//
						//	Name
						else if ( strIcmpL ( pBuffer, "Name=" ) == 0 )
						{
							pBuffer			= SkipToValue ( pBuffer );
							nameIndex		= SearchStringIndex ( pBuffer );
						}
						//	0x81 for Level / 0x82 for Title
						//	We Are Inside Level or Title
						//
						//	https://trwiki.earvillage.net/doku.php?id=trs:scripting_tr4_tr5
						else if ( strIcmpL ( pBuffer, "Level=" ) == 0 )
						{
							pBuffer			= SkipToValue ( pBuffer );
							char *pAudio	= strchr ( pBuffer, ',' );
							if ( *pAudio != NULL )
							{
								*pAudio	= '\0';
								pAudio++;
							}

							//
							//	Ok Store in LevelpathStringOffsets and 
							BOOL bNumericPath	= FALSE;
							bit8 iNumericPath	= atoi(pBuffer);
							if ( IsNumeric(pBuffer) )
							{
								//
								bNumericPath = TRUE;
							}
							else
							{
								LevelpathStringOffsets [ scriptLevelHeader.NumTotalLevels - 1 ] = scriptLevelHeader.LevelpathStringLen;
								memcpy_s ( LevelpathStringBlockData + scriptLevelHeader.LevelpathStringLen,
											sizeof(LevelpathStringBlockData) - scriptLevelHeader.LevelpathStringLen, pBuffer, strlen(pBuffer) + 1 );
								scriptLevelHeader.LevelpathStringLen	+= (xuxint16_t)strlen(pBuffer) + 1;
							}

							//
							BYTE audio = 0;
							if ( pAudio != NULL )
							{
								audio = GetBYTEValue ( pAudio );
							}

							//
							//	bitu8 stringIndex; xuxint16_t levelOptions; bitu8 pathIndex; bitu8 audio;
							if ( currentSection == Section_Level )
							{
								//
								LevelDatax81	data;
								ZeroMemory ( &data, sizeof(data) );
								data.opcode			= 0x81;
								if ( bNumericPath )
								{
									data.pathIndex		= iNumericPath;
								}
								else
								{
									data.pathIndex		= scriptLevelHeader.NumTotalLevels - 1;
								}
								data.audio			= audio;
								if ( nameIndex != - 1)
								{
									data.stringIndex	= nameIndex;
									if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Level Name %s\n", TraceOperation ( &data, sizeof(data) ) );
									nameIndex			= -1;
								}

								//
								LevelOffset = scriptLevelHeader.LevelBlockLen;	//
								TitleOffset = -1;

								//
								memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
											sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
								scriptLevelHeader.LevelBlockLen += sizeof(data);

								if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Level %s\n", TraceOperation ( &data, sizeof(data) ) );
							}
							//	
							else
							{
								//	bitu8 pathIndex; xuxint16_t titleOptions; bitu8 audio;
								LevelDatax82	data;
								ZeroMemory ( &data, sizeof(data) );
								data.opcode			= 0x82;
								if ( bNumericPath )
								{
									data.pathIndex		= iNumericPath;
								}
								else
								{
									data.pathIndex		= scriptLevelHeader.NumTotalLevels - 1;
								}
								data.audio			= audio;

								//
								LevelOffset = -1;
								TitleOffset = scriptLevelHeader.LevelBlockLen;	//

								//
								memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
											sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
								scriptLevelHeader.LevelBlockLen += sizeof(data);

								if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Title %s\n", TraceOperation ( &data, sizeof(data) ) );
							}
						}
						//	0x84 
						else if ( strIcmpL ( pBuffer, "Cut=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax84	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode		= 0x84;
							data.cutIndex	= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Cut %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x85 0x88
						else if ( strIcmpL ( pBuffer, "ResidentCut=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax85	data;
							ZeroMemory ( &data, sizeof(data) );
							int resident	= GetBYTEValue ( pBuffer );
							if ( resident >= 1 && resident <= 4 )
							{
								data.opcode		= 0x85 + resident - 1;
							}
							else
							{
								data.opcode		= 0x85;
							}
							pBuffer			= SkipToNext ( pBuffer );
							data.cutIndex	= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding ResidentCut %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x89
						else if ( strIcmpL ( pBuffer, "Layer1=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax89	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x89;
							data.red	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.green	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.blue	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.speed	= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Layer1 %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8a
						else if ( strIcmpL ( pBuffer, "Layer2=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8A	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8a;
							data.red	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.green	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.blue	= GetBYTEValue ( pBuffer );
							pBuffer		= SkipToNext ( pBuffer );
							data.speed	= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Layer2 %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8b
						else if ( strIcmpL ( pBuffer, "UVrotate=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8B	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8B;
							data.speed	= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding UVrotate %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8c
						else if ( strIcmpL ( pBuffer, "Legend=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8C	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8C;
							//
							//	Search String
							data.stringIndex = (bitu8) SearchStringIndex ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Legend %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8d
						else if ( strIcmpL ( pBuffer, "LensFlare=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8D	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8D;
							//
							data.yClicks		= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zClicks		= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xClicks		= GetWORDValue ( pBuffer );

							pBuffer				= SkipToNext ( pBuffer );
							data.red			= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.green			= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.blue			= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding LensFlare %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8E
						else if ( strIcmpL ( pBuffer, "Mirror=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8E	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8E;

							//
							data.room			= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAxis			= GetDWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Mirror %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x8F
						else if ( strIcmpL ( pBuffer, "Fog=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax8F	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x8F;
							//
							data.red		= GetBYTEValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.green		= GetBYTEValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.blue		= GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Fog %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x90
						else if ( strIcmpL ( pBuffer, "AnimatingMIP=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax90	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x90;
							//
							data.val	 = GetBYTEValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding AnimatingMIP %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x91
						else if ( strIcmpL ( pBuffer, "LoadCamera=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax91	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x91;
							//
							//	Search String
							data.srcX		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.srcY		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.srcZ		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.targX		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.targY		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.targZ		= GetDWORDValue ( pBuffer );
							pBuffer			= SkipToNext ( pBuffer );
							data.room		= GetBYTEValue ( pBuffer );
							
							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding LoadCamera %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x92
						else if ( strIcmpL ( pBuffer, "ResetHUB=" ) == 0 )
						{
							//
							pBuffer = SkipToValue ( pBuffer );

							//
							if ( version == 4 )
							{
								LevelDatax92	data;
								ZeroMemory ( &data, sizeof(data) );
								data.opcode	= 0x92;
								//
								//	Search String
								data.levelIndex			= GetBYTEValue ( pBuffer );

								//
								memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
											sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
								scriptLevelHeader.LevelBlockLen += sizeof(data);

								if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding ResetHUB Version 4 %s\n", TraceOperation ( &data, sizeof(data) ) );
							}
							else if ( version == 5 )
							{
								LevelDatax92_5	data;
								ZeroMemory ( &data, sizeof(data) );
								data.opcode	= 0x92;
								//
								//	Search String
								data.levelIndex			= GetBYTEValue ( pBuffer );
								for ( int iX = 0; iX < sizeof(data.other); iX++ )
								{
									pBuffer					= SkipToNext ( pBuffer );
									data.other [ iX ]		= GetBYTEValue ( pBuffer );
								}

								//
								memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
											sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
								scriptLevelHeader.LevelBlockLen += sizeof(data);

								if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding ResetHUB Version 5 %s\n", TraceOperation ( &data, sizeof(data) ) );
							}
						}
						else if ( strIcmpL ( pBuffer, "LoseItemAtStartup=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDataxDA	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xDA;
							//
							//	Search String
							data.itemNumber			= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding LoseItemAtStartup %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						
						//	0x93 -- 0x9E
						else if ( strIcmpL ( pBuffer, "Key=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x93;

							int key = GetBYTEValue ( pBuffer );
							if ( key >= 1 && key <= 12 )
							{
								data.opcode	= 0x93 + key - 1;
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Key %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0x9F -- 0xAA
						else if ( strIcmpL ( pBuffer, "Puzzle=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0x9F;

							int puzzle = GetBYTEValue ( pBuffer );
							if ( puzzle >= 1 && puzzle <= 12 )
							{
								data.opcode	= 0x9F + puzzle - 1;
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Puzzle %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0xAB -- 0xAE
						else if ( strIcmpL ( pBuffer, "Pickup=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xAB;

							int pickup = GetBYTEValue ( pBuffer );
							if ( pickup >= 1 && pickup <= 4 )
							{
								data.opcode	= 0xAB + pickup - 1;
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Pickup %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0xAF -- 0xB1
						else if ( strIcmpL ( pBuffer, "Examine=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xAF;

							int examine = GetBYTEValue ( pBuffer );
							if ( examine >= 1 && examine <= 3 )
							{
								data.opcode	= 0xAF + examine - 1;
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding Examine %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0xB2 -- 0xC1
						/**
						0xB2  KEY_ITEM1_COMBO1 -=-
						0xB3  KEY_ITEM1_COMBO2 -=-
						0xB4  KEY_ITEM2_COMBO1 -=-
						0xB5  KEY_ITEM2_COMBO2 -=-
						0xB6  KEY_ITEM3_COMBO1 -=-
						0xB7  KEY_ITEM3_COMBO2 -=-
						0xB8  KEY_ITEM4_COMBO1 -=-
						0xB9  KEY_ITEM4_COMBO2 -=-
						0xBA  KEY_ITEM5_COMBO1 -=-
						0xBB  KEY_ITEM5_COMBO2 -=-
						0xBC  KEY_ITEM6_COMBO1 -=-
						0xBD  KEY_ITEM6_COMBO2 -=-
						0xBE  KEY_ITEM7_COMBO1 -=-
						0xBF  KEY_ITEM7_COMBO2 -=-
						0xC0  KEY_ITEM8_COMBO1 -=-
						0xC1  KEY_ITEM8_COMBO2 -=-
						 **/
						else if ( strIcmpL ( pBuffer, "KeyCombo=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xB2;

							//	8 Keys
							int key				= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							//	2 Combos
							int combo			= GetBYTEValue ( pBuffer );
							if ( key >= 1 && key <= 8 && combo >= 1 && combo <= 2 )
							{
								data.opcode	= 0xB2 + ( key - 1 ) * 2 + ( combo - 1 );
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding KeyCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0xC2 -- 0xD1
						else if ( strIcmpL ( pBuffer, "PuzzleCombo=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xC2;

							//	8 Puzzle
							int puzzle				= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							//	2 Combos
							int combo			= GetBYTEValue ( pBuffer );
							if ( puzzle >= 1 && puzzle <= 8 && combo >= 1 && combo <= 2 )
							{
								data.opcode	= 0xC2 + ( puzzle - 1 ) * 2 + ( combo - 1 );
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding PuzzleCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						//	0xD2 -- 0xD9
						else if ( strIcmpL ( pBuffer, "PickupCombo=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );

							//
							LevelDatax93	data;
							ZeroMemory ( &data, sizeof(data) );
							data.opcode	= 0xD2;

							//	4 Pickup
							int pickup			= GetBYTEValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							//	2 Combos
							int combo			= GetBYTEValue ( pBuffer );
							if ( pickup >= 1 && pickup <= 4 && combo >= 1 && combo <= 2 )
							{
								data.opcode	= 0xD2 + ( pickup - 1 ) * 2 + ( combo - 1 );
							}
							pBuffer = SkipToNext ( pBuffer );

							//
							//	Search String
							data.stringIndex	= SearchStringIndex ( pBuffer, ',' );
							pBuffer				= SkipToNext ( pBuffer );
							data.height			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.size			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.yAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.zAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.xAngle			= GetWORDValue ( pBuffer );
							pBuffer				= SkipToNext ( pBuffer );
							data.unknown		= GetWORDValue ( pBuffer );

							//
							memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
										sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
							scriptLevelHeader.LevelBlockLen += sizeof(data);

							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding PickupCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
						}
						else
						{
							if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Not Recognized %s\n", pBuffer );
						}

					}
					//
					//	Language Treated
					else if ( currentSection == Section_Language )
					{
						if ( strIcmpL ( pBuffer, "File=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							int iLang = atoi(pBuffer);
							char *pComma = strchr ( pBuffer, ',' );
							if ( *pComma != NULL )
							{
								pBuffer = pComma + 1;
							}

							//
							strcpy_s ( LanguageFilename, sizeof(LanguageFilename), pBuffer );
							if ( EndsWithI ( LanguageFilename, ".TXT" ) )
							{
								if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Changing Language File Type for %s\n", LanguageFilename );
								RemoveFileType ( LanguageFilename );
								strcat_s ( LanguageFilename, sizeof(LanguageFilename), ".DAT" );
							}
							//	Write Language has two step inside
							WriteTRXLanguage ( LanguageFilename, pDirectory, iLang, version );

							//
							memcpy_s ( LanguageBlockData + LanguageBlockLen, sizeof(LanguageBlockData) - LanguageBlockLen, LanguageFilename, strlen ( LanguageFilename ) + 1 );
							LanguageBlockLen += (xuxint16_t) strlen ( LanguageFilename ) + 1;
						}
					}
					else if ( currentSection == Section_PSXExtensions )
					{
						if ( strIcmpL ( pBuffer, "Level=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PSXLevelString, sizeof(scriptLevelHeader.PSXLevelString), pBuffer );
						}
						else if ( strIcmpL ( pBuffer, "Cut=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PSXCutString, sizeof(scriptLevelHeader.PSXCutString), pBuffer );
						}
						else if ( strIcmpL ( pBuffer, "FMV=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PSXFMVString, sizeof(scriptLevelHeader.PSXFMVString), pBuffer );
						}
					}
					else if ( currentSection == Section_PCExtensions )
					{
						if ( strIcmpL ( pBuffer, "Level=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PCLevelString, sizeof(scriptLevelHeader.PCLevelString), pBuffer );
						}
						else if ( strIcmpL ( pBuffer, "Cut=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PCCutString, sizeof(scriptLevelHeader.PCCutString), pBuffer );
						}
						else if ( strIcmpL ( pBuffer, "FMV=" ) == 0 )
						{
							pBuffer = SkipToValue ( pBuffer );
							strcpy_s ( (char*) scriptLevelHeader.PCFMVString, sizeof(scriptLevelHeader.PCFMVString), pBuffer );
						}
					}
					else
					{
						if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Not Recognized %s\n", pBuffer );
					}
				}
			}
			else
			{
				//
				//	Add 0x83 For The Last Level
				if ( step == 1 && bNeedEndOfLevel )
				{
					//
					LevelDatax83	data;
					ZeroMemory ( &data, sizeof(data) );
					data.opcode	=	0x83;
					//
					memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen, 
								sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
					scriptLevelHeader.LevelBlockLen += sizeof(data);

					if ( hLogFile != NULL ) fprintf_s ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

					bNeedEndOfLevel	= false;
				}

			}
		}
		while ( ! feof(hInpFile) && ! ferror(hInpFile) );

		//
		//	Write Language at End

		//
		//
		if ( hOutFile != NULL ) fclose ( hOutFile );
		if ( hInpFile != NULL ) fclose ( hInpFile );

		hOutFile	= NULL;
		hInpFile	= NULL;

		bResult		= TRUE;
	}

	Cleanup();

	if ( hLogFile != NULL ) fclose ( hLogFile );
	hLogFile	= NULL;

	return bResult;
}
