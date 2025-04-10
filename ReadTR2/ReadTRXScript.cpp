#include "stdafx.h"
#include <Windows.h>

#include <Shlwapi.h>

#include "ReadTRXScript.h"
#include "MCMemA.h"
#include "TR4NG.h"
#include "TRXTools.h"

#ifndef     NB_BUTTONS
#define     NB_BUTTONS      29
#endif

//
#define     LEN_SCRIPT_BUFFER   (8*1024*1024)
#define     LEN_SCRIPT_LINE     4096

//  Space to store temporarly Items strings
#define     MAX_LABEL_STRING    1024
typedef struct LabelStringStruct
{
    char    text [MAX_LABEL_STRING];
} LabelStringType;

static LabelStringType          ItemsTable [ NB_BUTTONS ];
static LabelStringType          GlobalItemsTable [ NB_BUTTONS ];

typedef struct CorrespondanceStruct
{
    int     offset;
    BYTE    value;
} CorrespondanceType;

//  Correspondance between objects and TR4 Object Structure
static CorrespondanceType Correspondance [ 0xD9 - 0x93 + 1 ] =
{
    //  12 Keys
    { 20,   1 },
    { 20,   2 },
    { 20,   4 },
    { 20,   8 },
    { 20,   16 },
    { 20,   32 },
    { 20,   64 },
    { 20,   128 },
    { 21,   1 },
    { 21,   2 },
    { 21,   4 },
    { 21,   8 },
    //  12 Puzzle
    { 6,    1 },
    { 7,    1 },
    { 8,    1 },
    { 9,    1 },
    { 10,   1 },
    { 11,   1 },
    { 12,   1 },
    { 13,   1 },
    { 14,   1 },
    { 15,   1 },
    { 16,   1 },
    { 17,   1 },
    //  4 Pickup
    { 24,   1 },
    { 24,   2 },
    { 24,   4 },
    { 24,   8 },
    //  3 Examine
    { 3,    1 },
    { 4,    1 },
    { 5,    1 },
    //  16 KeyCombo
    { 22,   1 },
    { 22,   2 },
    { 22,   4 },
    { 22,   8 },
    { 22,   16 },
    { 22,   32 },
    { 22,   64 },
    { 22,   128 },
    { 23,   1 },
    { 23,   2 },
    { 23,   4 },
    { 23,   8 },
    { 23,   16 },
    { 23,   32 },
    { 23,   64 },
    { 23,   128 },
    //  16 PuzzleCombo
    { 18,   1 },
    { 18,   2 },
    { 18,   4 },
    { 18,   8 },
    { 18,   16 },
    { 18,   32 },
    { 18,   64 },
    { 18,   128 },
    { 19,   1 },
    { 19,   2 },
    { 19,   4 },
    { 19,   8 },
    { 19,   16 },
    { 19,   32 },
    { 19,   64 },
    { 19,   128 },
    //  8 PickupCombo
    { 26,   1 },
    { 26,   2 },
    { 26,   4 },
    { 26,   8 },
    { 27,   1 },
    { 27,   2 },
    { 27,   4 },
    { 27,   8 },

};

//  WORD Settings 
//  BYTE Checksum
//  BYTE Settings
static WORD BlindValues [ 3 ];
static DWORD BlindOffset [ 3 ];
static BYTE BlindBuffer [ LEN_SCRIPT_BUFFER ];

//
static WORD SoftValues [ 3 ];
static DWORD SoftOffset [ 3 ];
static BYTE SoftBuffer [ LEN_SCRIPT_BUFFER ];

//
static char TRXScriptVersion [ 32 ] = "";

//
static BYTE AlteredBuffer [ LEN_SCRIPT_BUFFER ];

//  Start Of NG Script
//  NG
//  The length is at END with NGLE + length
static DWORD TR4NGOffset;
static DWORD TR4NGSize;

static char  szArguments [ MAX_PATH ];

#define MAX_ARGS        256
static int  ArgumentsCount = 0;
static DWORD  ArgumentsBeg [ MAX_ARGS ];
static DWORD  ArgumentsEnd [ MAX_ARGS ];

//
#define MAX_ARGS_TAG        32
static int  ArgumentsTagCount = 0;
static WORD  ArgumentsTag [ MAX_ARGS_TAG ];

//
/////////////////////////////////////////////////////////////////////////////
//  -script "G:\Program Files (x86)\Core Design\trle\Script\SCRIPT.DAT"
//
/////////////////////////////////////////////////////////////////////////////

static struct tr4_script_header scriptHeader;
static struct tr4_script_levelheader scriptLevelHeader;
static struct tr4_lang_header langHeader;

//
static const int    maxLevels       = 64;

//  Length is scriptLevelHeader.NumTotalLevels * sizeof(xuint16_t)
static xuint16_t LevelpathStringOffsets [ maxLevels ];
//  Length is scriptLevelHeader.NumTotalLevels * sizeof(xuint16_t)
static xuint16_t LevelBlockDataOffsets [ maxLevels ];

//  Length is in scriptLevelHeader.LevelpathStringLen
static char LevelpathStringBlockData [ LEN_SCRIPT_BUFFER ];

//  Length is in scriptLevelHeader.LevelBlockLen
static BYTE LevelBlockData [ LEN_SCRIPT_BUFFER ];

//
xuint16_t   LanguageBlockLen;
//  The Size will be LanguageBlockLen
//  String are zero terminated. The end will be an other zero
static BYTE LanguageBlockData [ LEN_SCRIPT_BUFFER ];

static char LanguageFilename [ MAX_PATH ];

//  From Languages
//  Lengtth will be langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings
static xuint16_t StringOffsetTable[8192];

//  This will be used to search string in Level Data and setn the index
static const int StringTableMax = 4096;
static char *StringTable [ StringTableMax ];

//
static  FILE *hLogFile      = NULL;
static  FILE *hHeaFile  = NULL;

//
/**
0x81  Level           xbitu8 stringIndex, xuint16_t levelOptions, xbitu8 pathIndex, xbitu8 audio
0x82  [Title] Level   xbitu8 pathIndex, xuint16_t titleOptions, xbitu8 audio
0x8C  Legend          xbitu8 stringIndex
0x91  LoadCamera      xbit32 srcX, xbit32 srcY, xbit32 srcZ, xbit32 targX, xbit32 targY, xbit32 targZ, xbitu8 room
0x89  Layer1          xbitu8 red, xbitu8 green, xbitu8 blue, xbiti8 speed
0x8A  Layer2          xbitu8 red, xbitu8 green, xbitu8 blue, xbiti8 speed
0x8E  Mirror          xbitu8 room, xbit32 xAxis
0x8F  Fog             xbitu8 red, xbitu8 green, xbitu8 blue
0x84  Cut             xbitu8 cutIndex
0x8B  UVrotate        xbiti8 speed
0x85  ResidentCut1    xbitu8 cutIndex
0x86  ResidentCut2    xbitu8 cutIndex
0x87  ResidentCut3    xbitu8 cutIndex
0x88  ResidentCut4    xbitu8 cutIndex
0x80  FMV             xbitu8: 4 least significant bits represent the FMV index; 4 most significant bits (y) represent the FMV trigger bitfield as in y=1<->bit 8 set
0x92  ResetHUB        xbitu8 levelIndex
0x90  AnimatingMIP    xbitu8: 4 least significant bits represent animatingObjectIndex - 1; 4 most significant bits represent the distance
0x8D  LensFlare       xuint16_t yClicks, xbit16 zClicks, xuint16_t xClicks, xbitu8 red, xbitu8 green, xbitu8 blue
0x93  KEY_ITEM1       xuint16_t stringIndex, xuint16_t height, xuint16_t size, xuint16_t yAngle, xuint16_t zAngle, xuint16_t xAngle, xuint16_t unknown
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
The xuint16_t values levelOptions and titleOptions are actually bit fields containing several boolean options, and are laid out as follows (per-bit description):

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
Name=       Playable Tutorial Level
Legend=     The year 2000, somewhere in Egypt...
Horizon=    ENABLED
Layer1=     160,160,192,7
PuzzleCombo=    3,1,Cartouche Piece 1,  $0000,$0400,$0000,$0000,$0000,$0002
PuzzleCombo=    3,2,Cartouche Piece 2,  $0000,$0400,$0000,$0000,$0000,$0002
Puzzle=     3,Ba Cartouche,     $0000,$0400,$0000,$0000,$0000,$0002
Puzzle=     5,Eye Of Horus,     $0017,$0500,$0000,$0000,$0000,$0002
PuzzleCombo=    5,1,Eye Piece,      $0017,$0500,$0000,$0000,$0000,$0002
PuzzleCombo=    5,2,Eye Piece,      $0017,$0500,$0000,$0000,$0000,$0002
Puzzle=     6,The Hand Of Orion,    $0000,$0400,$8000,$c000,$0000,$0002
Puzzle=     8,The Hand Of Sirius,   $0000,$0400,$8000,$c000,$0000,$0002
LoadCamera= 0,0,0,0,0,0,255
LoadCamera= 11088,-1100,28896,11119,-1399,31486,0
Level=      DATA\TUT1,107

  **/

//  from 0x93
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
const char *GetTRXScriptVersion ()
{
    return TRXScriptVersion;
}

//
//====================================================================================
//  TRNG
// decriptare zona dei primi 64 bytes di script dat puntati da pScript
//====================================================================================
void DecriptaScript(BYTE *pScript)
{
    int i;
    int Indice;
    BYTE VetOut[64];
    int TotXor;
    int j;
    int TotOrd;

    static BYTE VetCryptTableXor[17] = {
        239, 85, 225, 248, 61, 111, 214, 25, 218, 151, 29, 139,
        133, 15, 180, 10, 196}; 

    TotXor = 8;
    TotOrd = 20;

    static BYTE VetCryptOrd[64] =
        {57, 49, 1, 7, 36, 37, 0, 17, 45, 13, 40, 44,
        46, 33, 30, 34, 20, 41, 26, 19, 59, 53, 43, 2,
        22, 6, 23, 9, 31, 10, 21, 15, 5, 8, 42, 24,
        55, 14, 48, 56, 47, 60, 12, 39, 28, 32, 16, 27,
        52, 35, 62, 58, 63, 11, 18, 38, 4, 54, 50, 61,
        51, 25, 29, 3};

    // fare in modo che il valore dei totali delle due serie
    // costanti sia corretto
    TotXor = TotXor << 1;       //  16
    TotOrd *=3;                 //  60
    TotXor++;                   //  17
    TotOrd +=4;                 //  64
    // adesso totXor = 17
    // e TotOrd = 64
    // prima usare lo xor per togliere la mascheratura
    Indice=0;
    for (i=0;i<TotOrd;i++) {
        if (Indice >= TotXor) Indice=0;

        pScript[i] ^= VetCryptTableXor[Indice];
        Indice++;
    }

    // adesso rimettere a posto l'ordine
    for (i=0;i<TotOrd;i++) {
        for (j=0;j<TotOrd;j++) {
            if (VetCryptOrd[j] == i) break;
        }
        VetOut[i] = pScript[j];
    }

    for (i=0;i<TotOrd;i++) {
        pScript[i] = VetOut[i];
    }


}

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

    size_t  iLen = strlen ( pSearched );
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

        if ( pText [ i ] == '\\' || pText [ i ] == '/' || pText [ i ] == ':' )
        {
            return pText;
        }
    }

    return pText;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Remove filename including trailing \ or /
/////////////////////////////////////////////////////////////////////////////
static char *RemoveFilename ( char *pText )
{
    //
    bool    bFound = false;

    //
    for ( int i = (int) strlen(pText) - 1; i > 0; i-- )
    {
        if ( pText [ i ] == '\\' || pText [ i ] == '/' )
        {
            pText [ i ] = '\0';
            bFound = true;
            break;
        }
        if ( pText [ i ] == ':' )
        {
            pText [ i + 1 ] = '\0';
            bFound = true;
            break;
        }
    }

    if ( ! bFound )
    {
        *pText = '\0';
    }

    //
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
    count   = 0;
    for ( int i = 0; i < sizeof(value) * 2; i++ )
    {
        value   = value << 4;
        if ( ! IsHexa ( parms [ i ] ) )
        {
            return value;
        }

        value   = value | HexaToBYTE ( parms [ i ] );
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
    count   = 0;
    for ( int i = 0; i < sizeof(value) * 2; i++ )
    {
        value   = value << 4;
        if ( ! IsHexa ( parms [ i ] ) )
        {
            return value;
        }

        value   = value | HexaToBYTE ( parms [ i ] );
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
    count   = 0;
    for ( int i = 0; i < sizeof(value) * 2; i++ )
    {
        if ( ! IsHexa ( parms [ i ] ) )
        {
            return value;
        }

        value   = value << 4;
        value   = value | HexaToBYTE ( parms [ i ] );
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
//====================================================================================
//  Print if FILE is Not NULL
//====================================================================================
static int Print ( FILE *hFile, const char *format, ... )
{
    if ( hFile == NULL )
    {
        return -1;
    }

    va_list argptr;
    va_start(argptr, format);
    int result = vfprintf_s ( hFile, format, argptr );
    va_end(argptr);
    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static xuint16_t SearchStringIndex ( const char *pText, const char delimiter = '\0' )
{
    static char szSearched [ MAX_PATH ];
    strcpy_s ( szSearched, sizeof(szSearched), pText );
    if ( delimiter != '\0' )
    {
        char *pDelimiter = strchr ( szSearched, delimiter );
        if ( pDelimiter != NULL )
        {
            *pDelimiter = '\0';
        }
    }

    for ( xuint16_t i = 0; i < StringTableMax; i++ )
    {
        if ( StringTable [ i ] != NULL )
        {
            if ( _stricmp ( szSearched, StringTable [ i ] ) == 0 )
            {
                return i;
            }
        }
    }

    Print ( hLogFile, "Error String %s not found\n", pText );

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
                    index   = strlen(szTempString);
                    pText++;
                    break;
                }
                case 'r' :
                {
                    strcat_s ( szTempString, sizeof(szTempString), "\r" );
                    index   = strlen(szTempString);
                    pText++;
                    break;
                }
                case 't' :
                {
                    strcat_s ( szTempString, sizeof(szTempString), "\t" );
                    index   = strlen(szTempString);
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
                    index   = strlen(szTempString);
                    pText   += count;
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
The xuint16_t values levelOptions and titleOptions are actually bit fields containing several boolean options, and are laid out as follows (per-bit description):

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
static const char *OptionLabel ( xuint16_t option )
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
//
/////////////////////////////////////////////////////////////////////////////
void CloseOneFile ( FILE **phFile )
{
    if ( phFile == NULL || *phFile == NULL )
    {
        return;
    }
    fclose ( *phFile );
    *phFile = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void XorWith ( BYTE *pBuffer, size_t len, BYTE security )
{
    for ( size_t i = 0; i < len; i++ )
    {
        pBuffer [ i ] = pBuffer [ i ] ^ security;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  http://xproger.info/projects/OpenLara/trs.html
//
/////////////////////////////////////////////////////////////////////////////
BOOL ReadTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version, bool bWrite )
{
    //  For First Language
    if ( iLang == 0 )
    {
        ZeroMemory ( StringTable, sizeof(StringTable) );
    }

    MCMemA memLanguageStrings(LEN_SCRIPT_BUFFER);
    MCMemA memString ( LEN_SCRIPT_BUFFER );

    static char szLanguage [ MAX_PATH ];
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
    //  Treat Language
    BOOL bResult    = FALSE;

    //
    FILE *hInpFile = NULL;
    FILE *hOutFile = NULL;

    //
    fopen_s ( &hInpFile, szLanguage, "rb" );
    if ( hInpFile == NULL )
    {
        Print ( hLogFile, "File Open Error %s\n", szLanguage );
        return bResult;
    }

    //
    if ( bWrite )
    {
        strcpy_s ( szOutputFilename, sizeof(szOutputFilename), szLanguage );
        strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".TXT" );
        fopen_s ( &hOutFile, szOutputFilename, "w" );
        if ( hOutFile == NULL )
        {
            Print ( hLogFile, "File Open Error %s\n", szOutputFilename );
            CloseOneFile ( &hInpFile );

            Cleanup();

            return bResult;
        }
    }

    //
    ZeroMemory ( &langHeader, sizeof(langHeader) );
    size_t uRead = fread ( (char*) &langHeader, 1, sizeof(langHeader), hInpFile );
    if ( uRead != sizeof(langHeader) )
    {
        CloseOneFile ( &hOutFile );
        CloseOneFile ( &hInpFile );

        Cleanup();
            
        return bResult;
    }

    //
    //  All the strings defined within {LANGUAGE}.DAT files are ASCII null-terminated strings.
    //  Every character (byte) contained in such a string is XOR-ed with byte 0xA5 (as mentioned above, it is done
    if ( bWrite )
    {
        Print ( hOutFile, "; NumGenericStrings : %d\n", langHeader.NumGenericStrings );
        Print ( hOutFile, "; NumPSXStrings : %d\n", langHeader.NumPSXStrings );
        Print ( hOutFile, "; NumPCStrings : %d\n", langHeader.NumPCStrings );

        Print ( hOutFile, "; GenericStringsLen : %d\n", langHeader.GenericStringsLen );
        Print ( hOutFile, "; PSXStringsLen : %d\n", langHeader.PSXStringsLen );
        Print ( hOutFile, "; PCStringsLen : %d\n", langHeader.PCStringsLen );
    }

    int countStrings = langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;

    ZeroMemory ( &StringOffsetTable, sizeof(StringOffsetTable) );
    uRead = fread ( (char*) &StringOffsetTable, 1, sizeof(xuint16_t)*countStrings, hInpFile );
    if ( uRead != sizeof(xuint16_t)*countStrings )
    {
        CloseOneFile ( &hOutFile );
        CloseOneFile ( &hInpFile );

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

    //  Generic
    Print ( hOutFile, "[Strings]\n" );

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
        Print ( hOutFile, "; Generic String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
        const char *pText = StringToText ( memString.ptr );
        if ( StringTable [ iBegining + i ] == NULL )
        {
            StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
            strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
        }

        Print ( hOutFile, "%s\n", pText );
    }

    Print ( hOutFile, "\n" );

    //  PSX
    Print ( hOutFile, "[PSXStrings]\n" );

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
        Print ( hOutFile, "; PSX String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
        const char *pText = StringToText ( memString.ptr );
        if ( StringTable [ iBegining + i ] == NULL )
        {
            StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
            strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
        }
        Print ( hOutFile, "%s\n", pText );
    }
    Print ( hOutFile, "\n" );

    //
    Print ( hOutFile, "[PCStrings]\n" );
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
        Print ( hOutFile, "; PC String %d (Offset %d : %d)\n", i, StringOffsetTable [ iBegining + i ], len );
        const char *pText = StringToText ( memString.ptr );
        if ( StringTable [ iBegining + i ] == NULL )
        {
            StringTable [ iBegining + i ] = (char*) malloc ( strlen(pText) + 1 );
            strcpy_s ( StringTable [ iBegining + i ], strlen(pText) + 1, pText );
        }
        Print ( hOutFile, "%s\n", pText );
    }
    Print ( hOutFile, "\n" );

    //
    CloseOneFile ( &hOutFile );
    CloseOneFile ( &hInpFile );

    bResult = TRUE;

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL TreatLevelData (    const char *pDirectory, const char *pDataFileType,
                                FILE *hOutFile, xuint16_t offset, int len,
                                int levelIndex,
                                int version, FCT_AddToItemsLabels function )
{
    //
    bool bTitle = false;

    static char szTRDataPath [ MAX_PATH ];

    //
    //  Search if Title
    for ( int step = 1; step <= 2; step++ )
    {

        //
        int x = offset;

        //
        if ( step == 2 )
        {
            if ( bTitle )
            {
                Print ( hOutFile, "[Title]\n" );
            }
            else
            {
                Print ( hOutFile, "[Level]\n" );
            }
        }

        //
        while (  x < len )
        {
            switch ( LevelBlockData [ x ] )
            {
                //  FMV
                //  xbitu8 val
                case 0x80 :
                {
                    LevelDatax80 *pArguments = ( LevelDatax80 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "FMV=\t$%02X\n", pArguments->val );
                    x += sizeof(LevelDatax80);
                    break;
                }

                //  Level
                //  xbitu8 stringIndex; xuint16_t levelOptions; xbitu8 pathIndex; xbitu8 audio;
                case 0x81 :
                {
                    LevelDatax81 *pArguments = ( LevelDatax81 * ) ( & LevelBlockData [ x ] );
                    const char *pString = StringTable [ pArguments->stringIndex ];
                    if ( step == 2 ) Print ( hOutFile, "Name=\t%s\n", pString );
                    const char *pathString = "";
                    if ( pArguments->pathIndex >= 0 && pArguments->pathIndex < maxLevels )
                    {
                        pathString = LevelpathStringBlockData + LevelpathStringOffsets [ pArguments->pathIndex ];
                    }

                    if ( step == 2 )
                    {
                        Print ( hOutFile, "; Level 0x81 args : %d,$%04X,%d,%d\n",
                            pArguments->stringIndex, pArguments->levelOptions, pArguments->pathIndex, pArguments->audio );
                        // A good place to test if path  exist
                        strcpy_s ( szTRDataPath, sizeof(szTRDataPath), pDirectory );
                        strcat_s ( szTRDataPath, sizeof(szTRDataPath), "\\" );
                        strcat_s ( szTRDataPath, sizeof(szTRDataPath), pathString );
                        strcat_s ( szTRDataPath, sizeof(szTRDataPath), pDataFileType );
                        if ( ! PathFileExists ( szTRDataPath ) )
                        {
                             Print ( hOutFile, "; '%s' Does not exist\n", szTRDataPath );
                        }
                        Print ( hOutFile, "Level=\t%s,%d\n", pathString, pArguments->audio );

                        if ( function != NULL )
                        {
                            MCMemA memCoded ( ( int ) strlen(pString) + 2 );
                            strcpy_s ( memCoded.ptr, memCoded.len, pString );
                            strcpy_s ( memCoded.ptr, memCoded.len, TextToString ( memCoded.ptr ) );
                            (*function)(1, levelIndex, -1, pathString, memCoded.ptr );
                        }
                    }

                    const char *pLabelOption = OptionLabel ( pArguments->levelOptions );
                    if ( step == 2 ) Print ( hOutFile, "%s", pLabelOption );

                    x += sizeof(LevelDatax81);
                    break;
                }
                //  Title
                //  xbitu8 pathIndex; xuint16_t titleOptions; xbitu8 audio;
                case 0x82 :
                {
                    bTitle = true;

                    //
                    LevelDatax82 *pArguments = ( LevelDatax82 * ) ( & LevelBlockData [ x ] );
                    const char *pathString = "";
                    if ( pArguments->pathIndex >= 0 && pArguments->pathIndex < maxLevels )
                    {
                        pathString = LevelpathStringBlockData + LevelpathStringOffsets [ pArguments->pathIndex ];
                        if ( step == 2 )
                        {
                            Print ( hOutFile, "; Level 0x82 args : %d,$%04X,%d\n",
                                pArguments->pathIndex, pArguments->titleOptions, pArguments->audio );
                            // A good place to test if path  exist
                            strcpy_s ( szTRDataPath, sizeof(szTRDataPath), pDirectory );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), "\\" );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), pathString );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), pDataFileType );
                            if ( ! PathFileExists ( szTRDataPath ) )
                            {
                                 Print ( hOutFile, "; '%s' Does not exist\n", szTRDataPath );
                            }

                            Print ( hOutFile, "Level=\t%s,%d\n", pathString, pArguments->audio );

                            if ( function != NULL )
                            {
                                (*function)(1, levelIndex, 1, pathString, "[Title]" );
                            }
                        }
                    }
                    else
                    {
                        if ( step == 2 )
                        {
                            Print ( hOutFile, "; Level 0x82 args : %d,$%04X,%d\n",
                                pArguments->pathIndex, pArguments->titleOptions, pArguments->audio );
                            // A good place to test if path  exist
                            strcpy_s ( szTRDataPath, sizeof(szTRDataPath), pDirectory );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), "\\" );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), pathString );
                            strcat_s ( szTRDataPath, sizeof(szTRDataPath), pDataFileType );
                            if ( ! PathFileExists ( szTRDataPath ) )
                            {
                                 Print ( hOutFile, "; '%s' Does not exist\n", szTRDataPath );
                            }

                            Print ( hOutFile, "Level=\t%d,%d\n", pArguments->pathIndex, pArguments->audio );

                            if ( function != NULL )
                            {
                                (*function)(1, levelIndex, 1, pathString, "[Title]" );
                            }
                        }
                    }

                    const char *pLabelOption = OptionLabel ( pArguments->titleOptions );
                    if ( step == 2 ) Print ( hOutFile, "%s", pLabelOption );

                    x += sizeof(LevelDatax82);
                    break;
                }

                //  End Of Level
                case 0x83 :
                {
                    LevelDatax83 *pArguments = ( LevelDatax83 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "; Level-Data-End Opcode $%02X\n", pArguments->opcode );
                    x += sizeof(LevelDatax83);
                    //
                    break;
                }
                //  xbitu8 cutIndex;
                case 0x84 :
                {
                    LevelDatax84 *pArguments = ( LevelDatax84 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "Cut=\t$%02X\n", pArguments->cutIndex );
                    x += sizeof(LevelDatax84);
                    break;
                }
                //  ResidentCut1
                case 0x85 :
                {
                    LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "ResidentCut=\t1,$%02X\n", pArguments->cutIndex );
                    x += sizeof(LevelDatax85);
                    break;
                }
                case 0x86 :
                {
                    LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "ResidentCut=\t2,$%02X\n", pArguments->cutIndex );
                    x += sizeof(LevelDatax85);
                    break;
                }
                case 0x87 :
                {
                    LevelDatax85 *pArguments = ( LevelDatax85 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "ResidentCut=\t3,$%02X\n", pArguments->cutIndex );
                    x += sizeof(LevelDatax85);
                    break;
                }
                case 0x88 :
                {
                    LevelDatax84 *pArguments = ( LevelDatax84 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "ResidentCut=\t4,$%02X\n", pArguments->cutIndex );
                    x += sizeof(LevelDatax84);
                    break;
                }

                //  xbitu8 red; xbitu8 green; xbitu8 blue; xbiti8 speed;
                case 0x89 :
                {
                    LevelDatax89 *pArguments = ( LevelDatax89 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "Layer1=\t%d,%d,%d,%d\n",
                                        pArguments->red, pArguments->green, pArguments->blue, pArguments->speed  );
                    x += sizeof(LevelDatax89);
                    break;
                }

                case 0x8A :
                {
                    LevelDatax89 *pArguments = ( LevelDatax89 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "Layer2=\t%d,%d,%d,%d\n",
                                        pArguments->red, pArguments->green, pArguments->blue, pArguments->speed  );
                    x += sizeof(LevelDatax89);
                    break;
                }

                //  speed
                case 0x8B :
                {
                    LevelDatax8B *pArguments = ( LevelDatax8B * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "UVrotate=\t%d\n", pArguments->speed  );
                    x += sizeof(LevelDatax8B);
                    break;
                }
                //  xbitu8 stringIndex;
                case 0x8C :
                {
                    LevelDatax8C *pArguments = ( LevelDatax8C * ) ( & LevelBlockData [ x ] );
                    const char *pString = StringTable [ pArguments->stringIndex ];
                    if ( step == 2 ) Print ( hOutFile, "Legend=\t%s\n", pString  );
                    x += sizeof(LevelDatax8C);
                    break;
                }
                //  xuint16_t yClicks; xbit16 zClicks; xuint16_t xClicks; xbitu8 red; xbitu8 green; xbitu8 blue;
                case 0x8D :
                {
                    LevelDatax8D *pArguments = ( LevelDatax8D * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "LensFlare=\t%d,%d,%d,$%02X,$%02X,$%02X\n",
                                        pArguments->yClicks, pArguments->zClicks, pArguments->xClicks,
                                        pArguments->red, pArguments->green, pArguments->blue  );
                    x += sizeof(LevelDatax8D);
                    break;
                }
                //  xbitu8 room; xbit32 xAxis;
                case 0x8E :
                {
                    LevelDatax8E *pArguments = ( LevelDatax8E * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "Mirror=\t%d,$%04lX\n", pArguments->room, pArguments->xAxis  );
                    x += sizeof(LevelDatax8E);
                    break;
                }
                //  xbitu8 red; xbitu8 green; xbitu8 blue;
                case 0x8F :
                {
                    LevelDatax8F *pArguments = ( LevelDatax8F * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "Fog=\t$%02X,$%02X,$%02X\n", pArguments->red, pArguments->green, pArguments->blue  );
                    x += sizeof(LevelDatax8F);
                    break;
                }
                //  xbitu8 val;
                case 0x90 :
                {
                    LevelDatax90 *pArguments = ( LevelDatax90 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "AnimatingMIP=\t$%02X\n", pArguments->val );
                    x += sizeof(LevelDatax90);
                    break;
                }
                //  xbit32 srcX; xbit32 srcY; xbit32 srcZ; xbit32 targX; xbit32 targY; xbit32 targZ; xbitu8 room;
                case 0x91 :
                {
                    LevelDatax91 *pArguments = ( LevelDatax91 * ) ( & LevelBlockData [ x ] );
                    if ( step == 2 ) Print ( hOutFile, "LoadCamera=\t%ld,%ld,%ld,%ld,%ld,%ld,%d\n",
                                        pArguments->srcX, pArguments->srcY, pArguments->srcZ,
                                        pArguments->targX, pArguments->targY, pArguments->targZ,
                                        pArguments->room );
                    x += sizeof(LevelDatax91);
                    break;
                }
                //  xbitu8 levelIndex;
                case 0x92 :
                {
                    if ( version == 4 )
                    {
                        LevelDatax92 *pArguments = ( LevelDatax92 * ) ( & LevelBlockData [ x ] );
                        if ( step == 2 ) Print ( hOutFile, "ResetHUB=\t$%02X\n", pArguments->levelIndex );
                        x += sizeof(LevelDatax92);
                    }
                    else if ( version == 5 )
                    {
                        LevelDatax92_5 *pArguments = ( LevelDatax92_5 * ) ( & LevelBlockData [ x ] );
                        if ( step == 2 )
                        {
                            Print ( hOutFile, "ResetHUB=\t$%02X,", pArguments->levelIndex );
                            for ( int iX = 0; iX < sizeof(pArguments->other); iX++ )
                            {
                                if ( iX == sizeof(pArguments->other) - 1 )
                                {
                                    Print ( hOutFile, "$%02X\n", pArguments->other[iX] );
                                }
                                else
                                {
                                    Print ( hOutFile, "$%02X,", pArguments->other[iX] );
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
                    if ( step == 2 ) Print ( hOutFile, "LoseItemAtStartup=\t%d\n", pArguments->itemNumber );
                    x += sizeof(LevelDataxDA);
                    break;
                }
                //
                default :
                {
                    if ( LevelBlockData [ x ]  >= 0x93 && LevelBlockData [ x ] <= 0xd9 )
                    {
                        int index = (unsigned char ) ( LevelBlockData [ x ] - 0x93 );

                        //  On TR5 seems to be shifted
                        if ( version == 5 )
                        {
                            index = ( index + 1 ) % ( sizeof(labelData) / sizeof(char *) );
                        }

                        LevelDatax93 *pArguments = ( LevelDatax93 * ) ( & LevelBlockData [ x ] );
                        const char *pString = StringTable [ pArguments->stringIndex ];
                        if ( step == 2 ) Print ( hOutFile, "%s,%s,$%04X,$%04X,$%04X,$%04X,$%04X,$%04X\n", labelData[index],
                                            pString,
                                            pArguments->height, pArguments->size,
                                            pArguments->yAngle, pArguments->zAngle, pArguments->xAngle,
                                            pArguments->unknown );
                        x += sizeof(LevelDatax93);

                        //
                        if ( step == 2 )
                        {
                            CorrespondanceType corr = Correspondance [ index ];
                            static char szItemName [ MAX_PATH ];
                            sprintf_s ( szItemName, sizeof(szItemName), "%s (%d)", pString, corr.value );

                            if ( hOutFile != NULL )
                            {
                                sprintf_s ( ItemsTable [ corr.offset ].text + strlen(ItemsTable [ corr.offset ].text),
                                            sizeof(ItemsTable [ corr.offset ].text) - strlen(ItemsTable [ corr.offset ].text),
                                            "%s \\r\\n", szItemName );
                            }
                            else
                            {
                                sprintf_s ( ItemsTable [ corr.offset ].text + strlen(ItemsTable [ corr.offset ].text),
                                            sizeof(ItemsTable [ corr.offset ].text) - strlen(ItemsTable [ corr.offset ].text),
                                            "%s \r\n", szItemName );
                            }

                            //
                            if ( strstr ( GlobalItemsTable [ corr.offset ].text, szItemName ) == NULL )
                            {
                                //
                                if ( hOutFile != NULL )
                                {
                                    if ( strlen(GlobalItemsTable [ corr.offset ].text) != 0 )
                                    {
                                        sprintf_s ( GlobalItemsTable [ corr.offset ].text + strlen(GlobalItemsTable [ corr.offset ].text),
                                                    sizeof(GlobalItemsTable [ corr.offset ].text) - strlen(GlobalItemsTable [ corr.offset ].text),
                                                    "\n\t\t" );
                                    }

                                    sprintf_s ( GlobalItemsTable [ corr.offset ].text + strlen(GlobalItemsTable [ corr.offset ].text),
                                            sizeof(GlobalItemsTable [ corr.offset ].text) - strlen(GlobalItemsTable [ corr.offset ].text),
                                            "\"%s \\r\\n\"", szItemName );
                                }
                                else
                                {
                                    sprintf_s ( GlobalItemsTable [ corr.offset ].text + strlen(GlobalItemsTable [ corr.offset ].text),
                                            sizeof(GlobalItemsTable [ corr.offset ].text) - strlen(GlobalItemsTable [ corr.offset ].text),
                                            "%s \r\n", szItemName );
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( step == 2 ) Print ( hOutFile, "; OpcodeError $%02X\n", LevelBlockData [ x ] );
                        x++;
                    }
                    break;
                }
            }
        }
    }

    //
    Print ( hOutFile, "\n" );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL WriteHeader ( int version, int level, FCT_AddToItemsLabels function = NULL )
{
    //
    if ( level < 0 )
    {
        if ( hHeaFile == NULL )
        {
            int b = 0;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "Crowbar" );
            }

            //
            b = 1;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "Small Waterskin (1)\r\n3 Liters (4)\r\n" );
            }

            //
            b = 2;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "Large Waterskin (1)\r\n2 Liters (2)\r\n3 Liters (4)\r\n" );
            }

            //
            b = 5;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "Cloth" );
            }

            //
            b = 6;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "Vials of chloroform" );
            }
        }
        else
        {
            int b = 0;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "\"Crowbar\"" );
            }

            //
            b = 1;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "\"Small Waterskin (1)\r\n3 Liters (4)\r\n\"" );
            }

            //
            b = 2;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "\"Large Waterskin (1)\r\n2 Liters (2)\r\n3 Liters (4)\r\n\"" );
            }

            //
            b = 5;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "\"Cloth\"" );
            }

            //
            b = 6;
            if ( strlen(GlobalItemsTable [ b ].text) == 0 )
            {
                strcpy_s ( GlobalItemsTable [ b ].text, sizeof(GlobalItemsTable [ b ].text), "\"Vials of chloroform\"" );
            }
        }
    }

    if ( level >= 0 )
    {
        Print (  hHeaFile, "char *TR%dItemsName%02d [NB_BUTTONS] =\n", version, level );
        Print (  hHeaFile, "{\n" );

        for ( int b = 0; b < NB_BUTTONS; b++ )
        {
            Print ( hHeaFile, "\t\"%s\",\t// %d \n", ItemsTable [ b ].text, b + 1 );
            if  ( function != NULL )
            {
                MCMemA memCoded ( ( int ) strlen(ItemsTable [ b ].text) + 2 );
                strcpy_s ( memCoded.ptr, memCoded.len, ItemsTable [ b ].text );
                strcpy_s ( memCoded.ptr, memCoded.len, TextToString ( memCoded.ptr ) );
                ( *function ) ( 0, level, b, memCoded.ptr, "" );
            }
        }
        Print (  hHeaFile, "};\n" );
        Print (  hHeaFile, "\n" );
    }
    else
    {
        Print (  hHeaFile, "char *TR%dItemsName [NB_BUTTONS] =\n", version );
        Print (  hHeaFile, "{\n" );

        for ( int b = 0; b < NB_BUTTONS; b++ )
        {
            if ( strlen(GlobalItemsTable [ b ].text) > 0 )
            {
                Print ( hHeaFile, "\t%s,\t// %d \n", GlobalItemsTable [ b ].text, b + 1 );
            }
            else
            {
                Print ( hHeaFile, "\t\"\",\t// %d \n", b + 1 );
            }

            //
            if  ( function != NULL )
            {
                MCMemA memCoded ( ( int ) strlen ( GlobalItemsTable [ b ].text ) + 2 );
                strcpy_s ( memCoded.ptr, memCoded.len, GlobalItemsTable [ b ].text );
                strcpy_s ( memCoded.ptr, memCoded.len, TextToString ( memCoded.ptr ) );
                ( *function ) ( 0, level, b, memCoded.ptr, "" );
            }
        }
        Print (  hHeaFile, "};\n" );
        Print (  hHeaFile, "\n" );
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
// decripta controllo script dat e salva i dati in altro vettore
//
/////////////////////////////////////////////////////////////////////////////
void DecriptaControlloScriptDat(const BYTE *pSource, int size, BYTE *pTarget )
{
	int i;
	int Indice;

    //  13 Bytes
	BYTE VetCrypt[] = {27, 48, 38, 153, 58, 77, 42, 58, 66, 45, 55, 22, 55};
	int TotCrypt;

	TotCrypt= 10;
	TotCrypt +=3;
	Indice=0;

	for (i=1; i<size; i++)
    {
		if (Indice == TotCrypt) Indice=0;
		pTarget[i] = pSource[i] ^ VetCrypt[Indice++];
	}

    //  Check Sum
	pTarget[0] = pSource[0];
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void OutputTRNGSaveString ( const char *pText, FILE *hOutFile )
{
#ifdef _DEBUG
#if TRACE_TRNG_SAVE
    OutputDebugString ( pText );
#endif
#endif

    //
    if ( hOutFile != NULL )
    {
        fprintf ( hOutFile, "%s", pText );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void OutputTRNGScriptString ( const char *pText, FILE *hOutFile )
{
#ifdef _DEBUG
#if TRACE_TRNG_SCRIPT
    OutputDebugString ( pText );
#endif
#endif

    //
    if ( hOutFile != NULL )
    {
        fprintf_s ( hOutFile, "%s", pText );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static void DumpControl ( BYTE *pAddress, const char *pTitle, const char *pText, int size, FILE *hOutFile, int enclose = -1 )
{
    static char szDebugString [ MAX_PATH ];

    sprintf_s ( szDebugString, sizeof(szDebugString), "; %s - %-10s : ", pTitle, pText );
    OutputTRNGScriptString( szDebugString, hOutFile );
    for ( int index = 0; index < size; index++ )
    {
        if ( ( index + 1 ) % 64 == 0 )
        {
            // OutputTRNGScriptString( "\n; ", hOutFile );
        }

        if ( index == enclose )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "[%02x] ", *pAddress & 0xff );
        }
        else
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "%02x ", *pAddress & 0xff );
        }
        OutputTRNGScriptString( szDebugString, hOutFile );
        pAddress++;
    }
    sprintf_s ( szDebugString, sizeof(szDebugString), "\n" );
    OutputTRNGScriptString( szDebugString, hOutFile );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL AnalyzeNGScript(char *pBYtes, long offset, FILE *hTxtFile )
{
    static char szDebugString [ MAX_PATH ];

    //
    ZeroMemory ( BlindValues, sizeof(BlindValues) );
    ZeroMemory ( BlindOffset, sizeof(BlindOffset) );

    //
    ZeroMemory ( SoftValues, sizeof(SoftValues) );
    ZeroMemory ( SoftOffset, sizeof(SoftOffset) );

    //
    ZeroMemory ( TRXScriptVersion, sizeof(TRXScriptVersion) );

    //
    TR4NGOffset = NULL;
    TR4NGSize   = NULL;

    //
    TRNGSPECIFIC *pTRNG = (TRNGSPECIFIC *) pBYtes;
    if ( memcmp ( pTRNG->signature, "NG", sizeof(pTRNG->signature) ) != 0 )
    {
        return FALSE;
    }

    TR4NGOffset = offset;

    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : Start at 0x%08lx\n", TR4NGOffset );
    OutputTRNGScriptString( szDebugString, hTxtFile );

    //
    TRNGITERATION *pIteration = & pTRNG->iteration;

    //
    BOOL bContinue              = TRUE;
    DWORD length                = 0;
    WORD *pCodeOp               = NULL;
    DWORD ExtraWords            = 0;
    WORD *pValues               = NULL;

    WORD ctnSettings            = 0;
    DWORD ctnSettingsAddress    = 0;

    //
    while ( bContinue )
    {
        length  = pIteration->w2.length;
        pCodeOp = &pIteration->w2.codeOp;
        pValues = pIteration->w2.values;

        //
        if ( pIteration->w2.length & 0x8000)
        {
            // size e' DWORD
            length      = ( ( (DWORD) (pIteration->w3.lengthHi & 0x7fff) ) << 16 ) + (DWORD) pIteration->w3.lengthLo;
            pCodeOp     = &pIteration->w3.codeOp;
            pValues     = pIteration->w3.values;
            ExtraWords  = 3;
        }
        else
        {
            // size e' WORD
            length      = pIteration->w2.length;
            ExtraWords  = 2;
        }

        DWORD relativeAddress = NULL;

        //
        if ( length == 0 )
        {
            relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;
            sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx Length zero - Code is 0x%04x\n",
                relativeAddress, *pCodeOp );
            OutputTRNGScriptString( szDebugString, hTxtFile );

            //
            //  Normally followed by NGLME + length (DWORD)
            char *pEnd = ( char * ) pValues;
            if ( memcmp ( pEnd, "NGLE", strlen("NGLE") ) == 0 )
            {
                relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset + sizeof(WORD);
                TR4NGSize = *( (DWORD *) ( pEnd + strlen("NGLE") ) );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx NGLE size is 0x%04x\n", 
                    relativeAddress, TR4NGSize );
                OutputTRNGScriptString( szDebugString, hTxtFile );
            }

            //
            bContinue = FALSE;
            break;
        }

        if ( *pCodeOp < 0x8000 || *pCodeOp > 0x80ff )
        {
            relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;
            sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx Code is 0x%04x - Length : %ld\n",  
                relativeAddress, *pCodeOp, length );
            OutputTRNGScriptString( szDebugString, hTxtFile );
            bContinue = FALSE;
            break;
        }

        //
        relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;
        sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx Code is 0x%04x (%s) (%ld words %ld bytes)\n", 
            relativeAddress, *pCodeOp, GetTRNGTagLabel(*pCodeOp), length, (long) sizeof(WORD) * ( length - ExtraWords ) );
        OutputTRNGScriptString( szDebugString, hTxtFile );

        //
        //  Trace items
		switch (*pCodeOp)
        {
            //
		    case NGTAG_SCRIPT_OPTIONS:
            {
                int indice = 0;
                while ( ( pValues[indice] & 0xff ) != 0 )
                {
                    relativeAddress = CTRXTools::RelativeAddress ( &pValues[indice], pBYtes ) + (DWORD) offset;
                    WORD TotWords   = pValues[indice] & 0xff;
                    WORD TagScript  = pValues[indice]  >> 8;
                    indice++;
                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx : TotWords : %3u - TagScript : %3u (0x%02x) %s = 0x%04x\n", 
                        relativeAddress, TotWords, TagScript, TagScript, GetTRNGCntLabel(TagScript), pValues[indice] );
                    OutputTRNGScriptString( szDebugString, hTxtFile );
                    if ( TagScript == ctn_Settings )
                    {
                        ctnSettings         = pValues[indice];
                        ctnSettingsAddress  = CTRXTools::RelativeAddress ( &pValues[indice], pBYtes ) + (DWORD) offset;
                    }
                    indice  += TotWords;
                }
                break;
            }

            //
		    case NGTAG_CONTROLLO_OPTIONS:
            {
                int nb = (int) sizeof(WORD) * ( length - ExtraWords );
                DumpControl ( (BYTE *) pValues, "TRNGSCRIPT", "Crypted", nb, hTxtFile, 19 );

                //
                MCMemA memUncrypted ( nb );

                //
                DecriptaControlloScriptDat ( (BYTE * ) pValues, nb, (BYTE * ) memUncrypted.ptr );

                //
                DumpControl ( (BYTE *) memUncrypted.ptr, "TRNGSCRIPT", "Decrypted", nb, hTxtFile, 19 );

                //
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx : ctn_Settings=%04x versus [19]=%02x\n", 
                    ctnSettingsAddress, ctnSettings, memUncrypted.ptr [ 19 ] );
                OutputTRNGScriptString( szDebugString, hTxtFile );

                //  Blind Save
                if ( ( ctnSettings & SET_BLIND_SAVEGAMES ) != 0 && ( memUncrypted.ptr [ 19 ] & SET_BLIND_SAVEGAMES ) != 0 )
                {
                    WORD mask               = 0xffff ^ SET_BLIND_SAVEGAMES;
                    ctnSettings             &= mask;
                    memUncrypted.ptr [ 19 ] &= mask;

                    //  Compute Checksum
                    int checkSum = 0;
                    BYTE *pAddress = (BYTE * ) memUncrypted.ptr;
                    for ( int i = 1; i < nb; i++ )
                    {
                        checkSum += pAddress [ i ];
                    }
                    checkSum &= 0xff;

                    memUncrypted.ptr [ 0 ] = checkSum & 0xff;

                    //
                    pAddress = (BYTE * ) pValues;
                    DWORD checkSumAddress = CTRXTools::RelativeAddress ( pAddress, pBYtes ) + (DWORD) offset;
                    DWORD settingAddress = CTRXTools::RelativeAddress ( pAddress + 19, pBYtes ) + (DWORD) offset;

                    //
                    MCMemA memCrypted ( nb );
                    DecriptaControlloScriptDat ( (BYTE * ) memUncrypted.ptr, nb, (BYTE * ) memCrypted.ptr );

                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : To Remove SET_BLIND_SAVEGAMES\n" );
                    OutputTRNGScriptString( szDebugString, hTxtFile );

                    //  Save Blind Values and Offset
                    BlindValues [ 0 ] = ctnSettings;
                    BlindValues [ 1 ] = checkSum;
                    BlindValues [ 2 ] = memCrypted.ptr [ 19 ];

                    BlindOffset [ 0 ] = ctnSettingsAddress;
                    BlindOffset [ 1 ] = checkSumAddress;
                    BlindOffset [ 2 ] = settingAddress;

                    //
                    sprintf_s ( szDebugString, sizeof(szDebugString), 
                        "; TRNGSCRIPT : ctn_Settings %08lx New=%04x versus [19] %08lx Old=%02x New=%02x CheckSum %08lx Old=%02x New=%02x\n", 
                        BlindOffset [ 0 ], BlindValues [ 0 ],
                        BlindOffset [ 2 ], pAddress [ 19 ], BlindValues [ 2 ] & 0xff,
                        BlindOffset [ 1 ], pAddress [ 0 ], BlindValues [ 1 ] & 0xff );
                    OutputTRNGScriptString( szDebugString, hTxtFile );

                    //
                    DumpControl ( (BYTE *) memCrypted.ptr, "TRNGSCRIPT", "ReCrypted", nb, hTxtFile, 19 );
                }
                else
                {
                    OutputTRNGScriptString( "; TRNGSCRIPT : Script file is not SET_BLIND_SAVEGAMES\n", hTxtFile );
                }

                //  Soft Settings
                if ( ( ctnSettings & SET_FORCE_SOFT_FULL_SCREEN ) != 0 && ( memUncrypted.ptr [ 19 ] & SET_FORCE_SOFT_FULL_SCREEN ) != 0 )
                {
                    WORD mask               = 0xffff ^ SET_FORCE_SOFT_FULL_SCREEN;
                    ctnSettings             &= mask;
                    memUncrypted.ptr [ 19 ] &= mask;

                    //  Compute Checksum
                    int checkSum = 0;
                    BYTE *pAddress = (BYTE * ) memUncrypted.ptr;
                    for ( int i = 1; i < nb; i++ )
                    {
                        checkSum += pAddress [ i ];
                    }
                    checkSum &= 0xff;

                    memUncrypted.ptr [ 0 ] = checkSum & 0xff;

                    //
                    pAddress = (BYTE * ) pValues;
                    DWORD checkSumAddress = CTRXTools::RelativeAddress ( pAddress, pBYtes ) + (DWORD) offset;
                    DWORD settingAddress = CTRXTools::RelativeAddress ( pAddress + 19, pBYtes ) + (DWORD) offset;

                    //
                    MCMemA memCrypted ( nb );
                    DecriptaControlloScriptDat ( (BYTE * ) memUncrypted.ptr, nb, (BYTE * ) memCrypted.ptr );

                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : To Remove SET_FORCE_SOFT_FULL_SCREEN\n" );
                    OutputTRNGScriptString( szDebugString, hTxtFile );

                    //  Save Soft Values and Offset
                    SoftValues [ 0 ] = ctnSettings;
                    SoftValues [ 1 ] = checkSum;
                    SoftValues [ 2 ] = memCrypted.ptr [ 19 ];

                    SoftOffset [ 0 ] = ctnSettingsAddress;
                    SoftOffset [ 1 ] = checkSumAddress;
                    SoftOffset [ 2 ] = settingAddress;

                    //
                    sprintf_s ( szDebugString, sizeof(szDebugString), 
                        "; TRNGSCRIPT : ctn_Settings %08lx New=%04x versus [19] %08lx Old=%02x New=%02x CheckSum %08lx Old=%02x New=%02x\n", 
                        SoftOffset [ 0 ], SoftValues [ 0 ],
                        SoftOffset [ 2 ], pAddress [ 19 ], SoftValues [ 2 ] & 0xff,
                        SoftOffset [ 1 ], pAddress [ 0 ], SoftValues [ 1 ] & 0xff );
                    OutputTRNGScriptString( szDebugString, hTxtFile );

                    //
                    DumpControl ( (BYTE *) memCrypted.ptr, "TRNGSCRIPT", "ReCrypted", nb, hTxtFile, 19 );
                }
                else
                {
                    OutputTRNGScriptString( "; TRNGSCRIPT : Script file is not SET_FORCE_SOFT_FULL_SCREEN\n", hTxtFile );
                }

                break;
            }

            //
		    case NGTAG_LEVEL_NAMES:
            {
                int indice = 0;
                break;
            }

            //
            case NGTAG_FLAG_LEVEL_TR4 :
            {
                int indice = 0;
                relativeAddress = CTRXTools::RelativeAddress ( &pValues[indice], pBYtes ) + (DWORD) offset;
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx = 0x%04x 0x%04x\n", 
                    relativeAddress, pValues[indice], pValues[indice + 1] );
                OutputTRNGScriptString( szDebugString, hTxtFile );
                break;
            }

            //
            case NGTAG_TOM_VERSION:
            {
                TRNGVERSIONHEADER *pSave = (TRNGVERSIONHEADER *) pValues;
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : %d\tVersion : %u.%u.%u.%u - Flag : 0x%04x\n", 
                    (int) sizeof(TRNGVERSIONHEADER),
                    pSave->VetVersione [ 0 ], pSave->VetVersione [ 1 ],
                    pSave->VetVersione [ 2 ], pSave->VetVersione [ 3 ],
                    pSave->Flags );
                OutputTRNGScriptString( szDebugString, hTxtFile );
                break;
            }

            //
            case NGTAG_VERSION_HEADER :
            {
                TRNGVERSIONHEADER *pSave = (TRNGVERSIONHEADER *) pValues;
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : %d\tVersion : %u.%u.%u.%u - Flag : 0x%04x\n", 
                    (int) sizeof(TRNGVERSIONHEADER),
                    pSave->VetVersione [ 0 ], pSave->VetVersione [ 1 ],
                    pSave->VetVersione [ 2 ], pSave->VetVersione [ 3 ],
                    pSave->Flags );
                OutputTRNGScriptString( szDebugString, hTxtFile );
                sprintf_s ( TRXScriptVersion, sizeof(TRXScriptVersion), "%u.%u.%u.%u", 
                    pSave->VetVersione [ 0 ], pSave->VetVersione [ 1 ],
                    pSave->VetVersione [ 2 ], pSave->VetVersione [ 3 ] );
                break;
            }

            //
		    case NGTAG_SCRIPT_LEVEL:
            {
                int indice = 0;
                while ( ( pValues[indice] & 0xff ) != 0 )
                {
                    relativeAddress = CTRXTools::RelativeAddress ( &pValues[indice], pBYtes ) + (DWORD) offset;
                    WORD TotWords   = pValues[indice] & 0xff;
                    WORD TagScript  = pValues[indice]  >> 8;
                    indice++;
                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSCRIPT : 0x%08lx : TotWords : %3u - TagScript : %3u (0x%02x) %s = ", 
                        relativeAddress, TotWords, TagScript, TagScript, GetTRNGCntLabel(TagScript) );
                    OutputTRNGScriptString( szDebugString, hTxtFile );

                    for ( int i = 0; i < TotWords; i++ )
                    {
                        sprintf_s ( szDebugString, sizeof(szDebugString), "0x%04x, ", 
                            pValues[indice + i] );
                        OutputTRNGScriptString( szDebugString, hTxtFile );
                    }

                    OutputTRNGScriptString( "\n", hTxtFile );
                    indice  += TotWords;
                }
                break;
            }
        }

        //
        pIteration = ( TRNGITERATION * ) ( ( WORD * ) pIteration + length );
    };

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  TOMBPC.DAT
/////////////////////////////////////////////////////////////////////////////
BOOL ReadTombPC (    const char *pathname, const char *pDirectory, int version, bool bWrite,
                        FCT_AddToItemsLabels function )
{

    static xuint32_t Version;             // The Script Version (Always 3 for TR2/3)
    static xuint8_t Description[256];     // Null-terminated string describing the script copyright info etc. Not encrypted.
    static xuint16_t GameflowSize;        // Size in bytes of the game flow data, always 128 bytes
    static xint32_t FirstOption;          // What to do when the game starts
    static xint32_t TitleReplace;         // What to do when EXIT_TO_TITLE is requested
    static xint32_t OnDeathDemoMode;      // What to do when Lara dies during the demo mode
    static xint32_t OnDeathInGame;        // What to do when Lara dies during the game
    static xuint32_t DemoTime;            // Time in game ticks (1/30th of a second) to wait before starting a demo
    static xint32_t OnDemoInterrupt;      // What to do when the demo mode is interrupted
    static xint32_t OnDemoEnd;            // What to do when the demo mode ends
    static xuint8_t Unknown1[36];         // Filler
    static xuint16_t NumLevels;           // Number of levels in the game, including the training level, not including the title level.
    static xuint16_t NumChapterScreens;   // Chapter screens (Present in TR2, first used in TR3)
    static xuint16_t NumTitles;           // Number of title elements (TITLE.TR2 level + the legal/title pictures in *.PCX format)
    static xuint16_t NumFMVs;             // Number of FMV cutscenes PC - (*.RPL), PSX - (*STR)
    static xuint16_t NumCutscenes;        // Number of in-game (engine-rendered) cutscenes (CUT*.TR2)
    static xuint16_t NumDemoLevels;       // Number of demo levels
    static xuint16_t TitleSoundID;        // ID of title soundtrack
    static xuint16_t SingleLevel;         // If doing only a single level, the level ID (starting at 1). -1 means disabled.
    static xuint8_t Unknown2[32];         // Filler
    static xuint16_t Flags;               // Various flags, see below
    static xuint8_t Unknown3[6];          // Filler
    static xuint8_t XORKey;               // Key used to encrypt/decrypt strings
    static xuint8_t LanguageID;           // Script Language ID, see below
    static xuint16_t SecretSoundID;       // ID of soundtrack to play when a secret is found
    static xuint8_t Unknown4[4];          // Filler



//     Bit 0 (0x01) — DemoVersion. If set, it indicates that the game is a demo distribution.
//     Bit 1 (0x02) — TitleDisabled. If set, it indicates that the game has no Title Screen.
//     Bit 2 (0x04) — CheatModeCheckDisabled. If set, it indicates that the game does not look for the cheat sequence keystrokes and events.
//     Bit 3 (0x08) — NoInputTimeout. If set, it indicates that the game waits forever if there is no input (won’t enter demo mode).
//     Bit 4 (0x10) — LoadSaveDisabled. If set, it indicates that the game does not allow save games.
//     Bit 5 (0x20) — ScreenSizingDisabled. If set, it indicates that the game does not allow screen resizing (with the function keys).
//     Bit 6 (0x40) — LockoutOptionRing. If set, it indicates that the user has no access to the Option Ring while playing the game.
//     Bit 7 (0x80) — DozyCheatEnabled. If set, it indicates that the game has the DOZY cheat enabled (only present in the final build of TR2 on PSX).
//     Bit 8 (0x100) — UseXor. If set, it indicates that a cypher byte was used to encrypt the strings in the script file, and is stored in the XorKey field.
//     Bit 9 (0x200) — GymEnabled. Is Gym available on title screen.
//     Bit 10 (0x400) — SelectAnyLevel. If set, it enables level select when New Game is selected.
//     Bit 11 (0x800) — EnableCheatCode. It apparently has no effect on the PC game.

//      If Flags & UseXor true each character (except null-terminator) must be ^ XorKey to decrypt the string.

//  TPCStringArray[NumLevels] LevelStrings;                  // level name strings
//  TPCStringArray[NumChapterScreens] ChapterScreenStrings;  // chapter screen strings
//  TPCStringArray[NumTitles] TitleStrings;                  // title strings
//  TPCStringArray[NumFMVs] FMVStrings;                      // FMV path strings
//  TPCStringArray[NumLevels] LevelPathStrings;              // level path strings
//  TPCStringArray[NumCutscenes] CutscenePathStrings;        // cutscene path strings

//  uint16_t SequenceOffsets[NumLevels + 1];                 // Relative offset to sequence info (the +1 is because the first one is the FrontEnd sequence, for when the game starts)
//  uint16_t SequenceNumBytes;                               // Size of SequenceOffsets in bytes
//  uint16_t[] Sequences[NumLevels + 1];    

//  dtruct TPCStringArray // (variable length)
//  {
//      uint16_t Offsets[Count]; // List containing for each string an offset in the Data block (Count * 2 bytes)
//      uint16_t TotalSize; // Total size, in bytes (2 bytes)
//      uint8_t Data[TotalSize]; // Strings block, usually encrypted (XOR-ed with XORKey, see above)
//  }

    //
    BOOL bResult = FALSE;

    //
    if ( ! PathFileExists ( pathname ) )
    {
        return FALSE;
    }

    //
    static char szOutputFilename [ MAX_PATH ];

    //
    static char szLogFilename [ MAX_PATH ];
    strcpy_s ( szLogFilename, sizeof(szLogFilename), pathname );
    strcat_s ( szLogFilename, sizeof(szLogFilename), ".LOG" );

    //
    if ( bWrite ) fopen_s ( &hLogFile, szLogFilename, "w" );

    //
    FILE *hInpFile = NULL;
    FILE *hTxtFile = NULL;

    fopen_s ( &hInpFile, pathname, "rb" );
    if ( hInpFile == NULL )
    {
        Print ( hLogFile, "File Open Error %s\n", pathname );
        CloseOneFile ( &hLogFile );
        return bResult;
    }

    //
    if ( bWrite )
    {
        strcpy_s ( szOutputFilename, sizeof(szOutputFilename), pathname );
        strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".TXT" );
        fopen_s ( &hTxtFile, szOutputFilename, "w" );
        if ( hTxtFile == NULL )
        {
            Print ( hLogFile, "File Open Error %s\n", szOutputFilename );
            CloseOneFile ( &hInpFile );

            Cleanup();

            CloseOneFile ( &hLogFile );

            return bResult;
        }
    }

    //
    size_t iRead = 0;
    iRead = fread ( &Version, sizeof(Version), 1, hInpFile );
    iRead = fread ( &Description, sizeof(Description), 1, hInpFile );
    iRead = fread ( &GameflowSize, sizeof(GameflowSize), 1, hInpFile );
    iRead = fread ( &FirstOption, sizeof(FirstOption), 1, hInpFile );
    iRead = fread ( &TitleReplace, sizeof(TitleReplace), 1, hInpFile );
    iRead = fread ( &OnDeathDemoMode, sizeof(OnDeathDemoMode), 1, hInpFile );
    iRead = fread ( &OnDeathInGame, sizeof(OnDeathInGame), 1, hInpFile );
    iRead = fread ( &DemoTime, sizeof(DemoTime), 1, hInpFile );
    iRead = fread ( &OnDemoInterrupt, sizeof(OnDemoInterrupt), 1, hInpFile );
    iRead = fread ( &OnDemoEnd, sizeof(OnDemoEnd), 1, hInpFile );
    iRead = fread ( &Unknown1, sizeof(Unknown1), 1, hInpFile );
    iRead = fread ( &NumLevels, sizeof(NumLevels), 1, hInpFile );
    iRead = fread ( &NumChapterScreens, sizeof(NumChapterScreens), 1, hInpFile );
    iRead = fread ( &NumTitles, sizeof(NumTitles), 1, hInpFile );
    iRead = fread ( &NumFMVs, sizeof(NumFMVs), 1, hInpFile );
    iRead = fread ( &NumCutscenes, sizeof(NumCutscenes), 1, hInpFile );
    iRead = fread ( &NumDemoLevels, sizeof(NumDemoLevels), 1, hInpFile );
    iRead = fread ( &TitleSoundID, sizeof(TitleSoundID), 1, hInpFile );
    iRead = fread ( &SingleLevel, sizeof(SingleLevel), 1, hInpFile );
    iRead = fread ( &Unknown2, sizeof(Unknown2), 1, hInpFile );
    iRead = fread ( &Flags, sizeof(Flags), 1, hInpFile );
    iRead = fread ( &Unknown3, sizeof(Unknown3), 1, hInpFile );
    iRead = fread ( &XORKey, sizeof(XORKey), 1, hInpFile );
    iRead = fread ( &LanguageID, sizeof(LanguageID), 1, hInpFile );
    iRead = fread ( &SecretSoundID, sizeof(SecretSoundID), 1, hInpFile );
    iRead = fread ( &Unknown4, sizeof(Unknown4), 1, hInpFile );

    Print ( hLogFile, "Version %d\n", Version );
    Print ( hLogFile, "NumLevels %d\n", NumLevels );
    Print ( hLogFile, "NumChapterScreens %d\n", NumChapterScreens );
    Print ( hLogFile, "NumTitles %d\n", NumTitles );
    Print ( hLogFile, "NumFMVs %d\n", NumFMVs );
    Print ( hLogFile, "NumDemoLevels %d\n", NumDemoLevels );
    Print ( hLogFile, "LanguageID %d\n", LanguageID );

    if ( Flags & 0x100 )
    {
        Print ( hLogFile, "XOR will be used %02x\n", XORKey );
    }

    //
    //  Read Level Strings
//  dtruct TPCStringArray // (variable length)
//  {
//      uint16_t Offsets[Count]; // List containing for each string an offset in the Data block (Count * 2 bytes)
//      uint16_t TotalSize; // Total size, in bytes (2 bytes)
//      uint8_t Data[TotalSize]; // Strings block, usually encrypted (XOR-ed with XORKey, see above)
//  }


    //  LevelStrings
    MCMemA LevelStrings;
    static xuint16_t OffsetsLevelStrings [ 1024 ];

    {
        ZeroMemory ( OffsetsLevelStrings, sizeof(OffsetsLevelStrings) );
        iRead = fread ( &OffsetsLevelStrings, NumLevels * sizeof(xuint16_t), 1, hInpFile );

        xuint16_t TotalSize = 0;
        iRead = fread ( &TotalSize, sizeof(TotalSize), 1, hInpFile );

        //
        LevelStrings.AllocateA(TotalSize + 1);
        iRead = fread ( LevelStrings.ptr, TotalSize, 1, hInpFile );

        //
        //  Decrypt
        if ( Flags & 0x100 )
        {
            for ( unsigned short i = 0; i < TotalSize; i++ )
            {
                if ( LevelStrings.ptr [ i ] != '\0' )
                {
                    LevelStrings.ptr [ i ] = LevelStrings.ptr [ i ] ^ XORKey;
                }
            }
        }

        //
        //  List
        for ( unsigned short l = 0; l < NumLevels; l++ )
        {
            Print ( hLogFile, "LevelStrings %d : %s\n", l, LevelStrings.ptr + OffsetsLevelStrings [ l ] );
        }
    }

    //  ChapterScreenStrings
    MCMemA ChapterScreenStrings;
    static xuint16_t OffsetsChapterScreenStrings [ 1024 ];
    {
        ZeroMemory ( OffsetsChapterScreenStrings, sizeof(OffsetsChapterScreenStrings) );
        iRead = fread ( &OffsetsChapterScreenStrings, NumChapterScreens * sizeof(xuint16_t), 1, hInpFile );

        xuint16_t TotalSize = 0;
        iRead = fread ( &TotalSize, sizeof(TotalSize), 1, hInpFile );

        //
        ChapterScreenStrings.AllocateA ( TotalSize + 1 );
        iRead = fread ( ChapterScreenStrings.ptr, TotalSize, 1, hInpFile );

        //
        //  Decrypt
        if ( Flags & 0x100 )
        {
            for ( unsigned short i = 0; i < TotalSize; i++ )
            {
                if ( ChapterScreenStrings.ptr [ i ] != '\0' )
                {
                    ChapterScreenStrings.ptr [ i ] = ChapterScreenStrings.ptr [ i ] ^ XORKey;
                }
            }
        }

        //
        //  List
        for ( unsigned short l = 0; l < NumChapterScreens; l++ )
        {
            Print ( hLogFile, "ChapterScreenStrings %d : %s\n", l, ChapterScreenStrings.ptr + OffsetsChapterScreenStrings [ l ] );
        }
    }

    //  TitleStrings
    MCMemA TitleStrings;
    static xuint16_t OffsetsTitleStrings [ 1024 ];
    {
        ZeroMemory ( OffsetsTitleStrings, sizeof(OffsetsTitleStrings) );
        iRead = fread ( &OffsetsTitleStrings, NumTitles * sizeof(xuint16_t), 1, hInpFile );

        xuint16_t TotalSize = 0;
        iRead = fread ( &TotalSize, sizeof(TotalSize), 1, hInpFile );

        //
        TitleStrings.AllocateA ( TotalSize + 1 );
        iRead = fread ( TitleStrings.ptr, TotalSize, 1, hInpFile );

        //
        //  Decrypt
        if ( Flags & 0x100 )
        {
            for ( unsigned short i = 0; i < TotalSize; i++ )
            {
                if ( TitleStrings.ptr [ i ] != '\0' )
                {
                    TitleStrings.ptr [ i ] = TitleStrings.ptr [ i ] ^ XORKey;
                }
            }
        }

        //
        //  List
        for ( unsigned short l = 0; l < NumTitles; l++ )
        {
            Print ( hLogFile, "TitleStrings %d : %s\n", l, TitleStrings.ptr + OffsetsTitleStrings [ l ] );
        }
    }

    //  FMVStrings
    MCMemA FMVStrings;
    static xuint16_t OffsetsFMVStrings [ 1024 ];
    {
        ZeroMemory ( OffsetsFMVStrings, sizeof(OffsetsFMVStrings) );
        iRead = fread ( &OffsetsFMVStrings, NumFMVs * sizeof(xuint16_t), 1, hInpFile );

        xuint16_t TotalSize = 0;
        iRead = fread ( &TotalSize, sizeof(TotalSize), 1, hInpFile );

        //
        FMVStrings.AllocateA ( TotalSize + 1 );
        iRead = fread ( FMVStrings.ptr, TotalSize, 1, hInpFile );

        //
        //  Decrypt
        if ( Flags & 0x100 )
        {
            for ( unsigned short i = 0; i < TotalSize; i++ )
            {
                if ( FMVStrings.ptr [ i ] != '\0' )
                {
                    FMVStrings.ptr [ i ] = FMVStrings.ptr [ i ] ^ XORKey;
                }
            }
        }

        //
        //  List
        for ( unsigned short l = 0; l < NumFMVs; l++ )
        {
            Print ( hLogFile, "FMVStrings %d : %s\n", l, FMVStrings.ptr + OffsetsFMVStrings [ l ] );
        }
    }

    //  LevelPathStrings
    MCMemA LevelPathStrings;
    static xuint16_t OffsetsLevelPathStrings [ 1024 ];
    {
        ZeroMemory ( OffsetsLevelPathStrings, sizeof(OffsetsLevelPathStrings) );
        iRead = fread ( &OffsetsLevelPathStrings, NumLevels * sizeof(xuint16_t), 1, hInpFile );

        xuint16_t TotalSize = 0;
        iRead = fread ( &TotalSize, sizeof(TotalSize), 1, hInpFile );

        //
        LevelPathStrings.AllocateA ( TotalSize + 1 );
        iRead = fread ( LevelPathStrings.ptr, TotalSize, 1, hInpFile );

        //
        //  Decrypt
        if ( Flags & 0x100 )
        {
            for ( unsigned short i = 0; i < TotalSize; i++ )
            {
                if ( LevelPathStrings.ptr [ i ] != '\0' )
                {
                    LevelPathStrings.ptr [ i ] = LevelPathStrings.ptr [ i ] ^ XORKey;
                }
            }
        }

        //
        //  List
        for ( unsigned short l = 0; l < NumLevels; l++ )
        {
            char *pString       = LevelStrings.ptr + OffsetsLevelStrings [ l ];
            char *pathString    = LevelPathStrings.ptr + OffsetsLevelPathStrings [ l ];

            Print ( hLogFile, "LevelPathStrings %d : %s (%s)\n", l, pathString, pString );

            if ( function != NULL )
            {
                MCMemA memCoded ( ( int ) strlen(pString) + 2 );
                strcpy_s ( memCoded.ptr, memCoded.len, pString );
                strcpy_s ( memCoded.ptr, memCoded.len, TextToString ( memCoded.ptr ) );
                (*function)(1, l, -1, pathString, memCoded.ptr );
            }

        }
    }

    //
    CloseOneFile ( &hTxtFile );
    CloseOneFile ( &hInpFile );

    CloseOneFile ( &hLogFile );

    bResult     = TRUE;

    //
    Cleanup();

    return bResult;   // Then use bResult
}

//
/////////////////////////////////////////////////////////////////////////////
//  Full Pathname
//
/////////////////////////////////////////////////////////////////////////////
BOOL ReadTRXScript (    const char *pathname, const char *pDirectory, int version, bool bWrite,
                        FCT_AddToItemsLabels function )
{
    if ( version == 2 || version == 3 )
    {
        return ReadTombPC ( pathname, pDirectory, version,  bWrite, function );
    }

    if ( version != 4 && version != 5 )
    {
        return FALSE;
    }

    //
    if ( ! PathFileExists ( pathname ) )
    {
        return FALSE;
    }

    //
    static char szDataFileType [ 32 ];
    strcpy_s ( szDataFileType, sizeof(szDataFileType), ".TR4" );
    if ( version == 4 )
    {
        strcpy_s ( szDataFileType, sizeof(szDataFileType), ".TR4" );
    }
    else if ( version == 5 )
    {
        strcpy_s ( szDataFileType, sizeof(szDataFileType), ".TRC" );
    }

    //
    ZeroMemory ( StringTable, sizeof(StringTable) );

    ZeroMemory ( GlobalItemsTable, sizeof(GlobalItemsTable) );

    //
    ZeroMemory ( BlindValues, sizeof(BlindValues) );
    ZeroMemory ( BlindOffset, sizeof(BlindOffset) );

    TR4NGOffset = NULL;
    TR4NGSize   = NULL;

    //
    BOOL bResult = FALSE;

    //
    static char szOutputFilename [ MAX_PATH ];

    //
    static char szLogFilename [ MAX_PATH ];
    strcpy_s ( szLogFilename, sizeof(szLogFilename), pathname );
    strcat_s ( szLogFilename, sizeof(szLogFilename), ".LOG" );

    //
    static char szHeaFilename [ MAX_PATH ];
    strcpy_s ( szHeaFilename, sizeof(szHeaFilename), pathname );
    strcat_s ( szHeaFilename, sizeof(szHeaFilename), ".h" );

    //
    if ( bWrite ) fopen_s ( &hLogFile, szLogFilename, "w" );
    if ( bWrite ) fopen_s ( &hHeaFile, szHeaFilename, "w" );

    //
    FILE *hInpFile = NULL;
    FILE *hTxtFile = NULL;

    fopen_s ( &hInpFile, pathname, "rb" );
    if ( hInpFile == NULL )
    {
        Print ( hLogFile, "File Open Error %s\n", pathname );
        CloseOneFile ( &hLogFile );
        CloseOneFile ( &hHeaFile );
        return bResult;
    }

    //
    if ( bWrite )
    {
        strcpy_s ( szOutputFilename, sizeof(szOutputFilename), pathname );
        strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".TXT" );
        fopen_s ( &hTxtFile, szOutputFilename, "w" );
        if ( hTxtFile == NULL )
        {
            Print ( hLogFile, "File Open Error %s\n", szOutputFilename );
            CloseOneFile ( &hInpFile );

            Cleanup();

            CloseOneFile ( &hLogFile );
            CloseOneFile ( &hHeaFile );

            return bResult;
        }
    }

    size_t uRead = 0;

    //  Look If Encryot
    ////////////////////////////////////
    BOOL fileIsCrypted = FALSE;
    BYTE cryptHeader [ 64 ];
    ZeroMemory ( cryptHeader, sizeof(cryptHeader) );
    uRead = fread ( cryptHeader, 1, sizeof(cryptHeader), hInpFile );
    if ( uRead != sizeof(cryptHeader) )
    {
        CloseOneFile ( &hTxtFile );
        CloseOneFile ( &hInpFile );
        CloseOneFile ( &hLogFile );
        CloseOneFile ( &hHeaFile );

        Cleanup();

        return bResult;
    }

    //  Preserve Buffer
    //  The crypted part covers the two headers and it will remain 8 bytes.
    DecriptaScript ( cryptHeader );
    tr4_script_header       *pScriptHeader      = (tr4_script_header *) cryptHeader;
    tr4_script_levelheader  *pScriptLevelHeader = (tr4_script_levelheader *)( cryptHeader + sizeof(tr4_script_header) );
    BYTE *pCryptHeader                          = cryptHeader;

    //
    //  Check
    bool bGood = false;
    if (    _strnicmp ( (char *) pScriptLevelHeader->PCLevelString, ".TR4", strlen(".TR4") ) == 0  &&
            _strnicmp ( (char *) pScriptLevelHeader->PCCutString, ".TR4", strlen(".TR4") ) == 0    &&
            _strnicmp ( (char *) pScriptLevelHeader->PCFMVString, ".", strlen(".") ) == 0               )
    {
        fileIsCrypted   = TRUE;
        Print ( hTxtFile, "; File is Crypted TRNG\n" );
        bGood = true;
    }

    //  File not Crypted : Rewind and Read Normally
    if ( ! fileIsCrypted )
    {
        fseek ( hInpFile, SEEK_SET, 0 );
        Print ( hLogFile, "Reading scriptHeader %d\n", sizeof(scriptHeader) );
        uRead = fread ( (char*) &scriptHeader, 1, sizeof(scriptHeader), hInpFile );
        if ( uRead != sizeof(scriptHeader) )
        {
            CloseOneFile ( &hTxtFile );
            CloseOneFile ( &hInpFile );
            CloseOneFile ( &hLogFile );
            CloseOneFile ( &hHeaFile );

            Cleanup();

            return bResult;
        }
    }
    //  Get The Script Header from Buffer
    else
    {
        memcpy_s ( &scriptHeader, sizeof(scriptHeader), pCryptHeader, sizeof(scriptHeader) );
        pCryptHeader += sizeof(scriptHeader);
    }

    //
    Print ( hTxtFile, "[Options]\n" );
    Print ( hTxtFile, "; Options : 0x%x\n", scriptHeader.Options );

    //
    if ( scriptHeader.Options & 0x01 )
    {
        Print ( hTxtFile, "FlyCheat=\tENABLED\n" );
    }
    else
    {
        Print ( hTxtFile, "FlyCheat=\tDISABLED\n" );
    }

    if ( scriptHeader.Options & 0x02 )
    {
        Print ( hTxtFile, "LoadSave=\tENABLED\n" );
    }
    else
    {
        Print ( hTxtFile, "LoadSave=\tDISABLED\n" );
    }

    if ( scriptHeader.Options & 0x04 )
    {
        Print ( hTxtFile, "Title=\tENABLED\n" );
    }
    else
    {
        Print ( hTxtFile, "Title=\tDISABLED\n" );
    }

    if ( scriptHeader.Options & 0x08 )
    {
        Print ( hTxtFile, "PlayAnyLevel=\tENABLED\n" );
    }
    else
    {
        Print ( hTxtFile, "PlayAnyLevel=\tDISABLED\n" );
    }

    if ( scriptHeader.Options & 0x80 )
    {
        Print ( hTxtFile, "DemoDisc=\tENABLED\n" );
    }
    else
    {
        Print ( hTxtFile, "DemoDisc=\tDISABLED\n" );
    }

    //
    Print ( hTxtFile, "InputTimeout=\t%d\n", scriptHeader.InputTimeout );
    Print ( hTxtFile, "Security=\t$%02X\n", scriptHeader.Security );
    Print ( hTxtFile, "\n" );

    //
    if ( ! fileIsCrypted )
    {
        Print ( hLogFile, "Reading scriptHeader %d\n", sizeof(scriptLevelHeader) );
        uRead = fread ( (char*) &scriptLevelHeader, 1, sizeof(scriptLevelHeader), hInpFile );
        if ( uRead != sizeof(scriptLevelHeader) )
        {
            CloseOneFile ( &hTxtFile );
            CloseOneFile ( &hInpFile );

            CloseOneFile ( &hLogFile );
            CloseOneFile ( &hHeaFile );

            Cleanup();

            return bResult;
        }
    }
    //  Get The Script Level Header from Buffer
    else
    {
        memcpy_s ( &scriptLevelHeader, sizeof(scriptLevelHeader), pCryptHeader, sizeof(scriptLevelHeader) );
        pCryptHeader += sizeof(scriptLevelHeader);
    }

    //
    Print ( hTxtFile, "; NumTotalLevels : %d\n", scriptLevelHeader.NumTotalLevels );
    Print ( hTxtFile, "; NumUniqueLevelPaths : %d\n", scriptLevelHeader.NumUniqueLevelPaths );
    Print ( hTxtFile, "; LevelpathStringLen : %d\n", scriptLevelHeader.LevelpathStringLen );
    Print ( hTxtFile, "; LevelBlockLen : %d\n", scriptLevelHeader.LevelBlockLen );

    Print ( hTxtFile, "[PSXExtensions]\n" );
    Print ( hTxtFile, "Level=\t%s\n", scriptLevelHeader.PSXLevelString );
    Print ( hTxtFile, "Cut=\t%s\n", scriptLevelHeader.PSXCutString );
    Print ( hTxtFile, "FMV=\t%s\n", scriptLevelHeader.PSXFMVString );
    Print ( hTxtFile, "\n" );

    Print ( hTxtFile, "[PCExtensions]\n" );
    Print ( hTxtFile, "Level=\t%s\n", scriptLevelHeader.PCLevelString );
    Print ( hTxtFile, "Cut=\t%s\n", scriptLevelHeader.PCCutString );
    Print ( hTxtFile, "FMV=\t%s\n", scriptLevelHeader.PCFMVString );
    Print ( hTxtFile, "\n" );

    if (    strlen ( (char *) scriptLevelHeader.PSXLevelString ) > 5    ||
            strlen ( (char *) scriptLevelHeader.PSXCutString ) > 5      ||
            strlen ( (char *) scriptLevelHeader.PSXLevelString ) > 5    ||
            strlen ( (char *) scriptLevelHeader.PCLevelString ) > 5     ||
            strlen ( (char *) scriptLevelHeader.PCCutString ) > 5       ||
            strlen ( (char *) scriptLevelHeader.PCFMVString ) > 5           )
    {
        CloseOneFile ( &hTxtFile );
        CloseOneFile ( &hInpFile );

        CloseOneFile ( &hLogFile );
        CloseOneFile ( &hHeaFile );

        Cleanup();

        return FALSE;
    }

    //  Normally it remains 8 bytes only
    int remaining = sizeof(cryptHeader) - sizeof(scriptHeader) - sizeof(scriptLevelHeader);

    //
    //  Strings
    int nbLevelPath = scriptLevelHeader.NumUniqueLevelPaths;
    ZeroMemory ( &LevelpathStringOffsets, sizeof(LevelpathStringOffsets) );
    if ( ! fileIsCrypted )
    {
        if ( nbLevelPath > 0 )
        {
            Print ( hLogFile, "Reading LevelpathStringOffsets %d\n", sizeof(xuint16_t)*nbLevelPath );
            uRead = fread ( (char*) &LevelpathStringOffsets, 1, sizeof(xuint16_t)*nbLevelPath, hInpFile );
            if ( uRead != sizeof(xuint16_t)*nbLevelPath )
            {
                CloseOneFile ( &hTxtFile );
                CloseOneFile ( &hInpFile );

                CloseOneFile ( &hLogFile );
                CloseOneFile ( &hHeaFile );

                Cleanup();

                return bResult;
            }
        }
    }
    else
    {
        //  Look if there is enough and Read if not in the file.
        //  We have already read the first 64 bytes
        //  So the reading continues from here
        //  We could have to read if there is more than 4 levels ( 2 bytes * 4 = 8)
        int toCopy = min ( remaining, (int) sizeof(xuint16_t)*nbLevelPath );
        if ( toCopy > 0 )
        {
            memcpy_s ( (BYTE*) LevelpathStringOffsets, toCopy, pCryptHeader, toCopy );
        }
        pCryptHeader += toCopy;
        remaining = remaining - toCopy;
        int toRead = sizeof(xuint16_t)*nbLevelPath - toCopy;
        if ( toRead > 0 )
        {
            uRead = fread ( ( (char*) &LevelpathStringOffsets ) + toCopy, 1, toRead, hInpFile );
            if ( uRead != toRead )
            {
                CloseOneFile ( &hTxtFile );
                CloseOneFile ( &hInpFile );

                CloseOneFile ( &hLogFile );
                CloseOneFile ( &hHeaFile );

                Cleanup();

                return bResult;
            }
        }
    }

    //
    ZeroMemory ( LevelpathStringBlockData, sizeof(LevelpathStringBlockData) );
    Print ( hLogFile, "Reading LevelpathStringBlockData %d\n", scriptLevelHeader.LevelpathStringLen );
    if ( ! fileIsCrypted )
    {
        uRead = fread ( LevelpathStringBlockData, 1, scriptLevelHeader.LevelpathStringLen, hInpFile );
        if ( uRead != scriptLevelHeader.LevelpathStringLen )
        {
            CloseOneFile ( &hTxtFile );
            CloseOneFile ( &hInpFile );

            CloseOneFile ( &hLogFile );
            CloseOneFile ( &hHeaFile );

            Cleanup();

            return bResult;
        }
    }
    else
    {
        //  Look if there is enough and Read if not in the file.
        //  We have already read the first 64 bytes
        //  So the reading continues from here
        //  We will have to read even if there is only one level ( 2 bytes * 4 = 8)
        //  because levelpath is DATA\L<null> (so a minimum of 7 bytes)
        int toCopy = min ( remaining, scriptLevelHeader.LevelpathStringLen );
        if ( toCopy > 0 )
        {
            memcpy_s ( (BYTE*) LevelpathStringBlockData, toCopy, pCryptHeader, toCopy );
        }
        remaining = remaining - toCopy;
        int toRead = scriptLevelHeader.LevelpathStringLen - toCopy;
        if ( toRead > 0 )
        {
            uRead = fread ( LevelpathStringBlockData + toCopy, 1, toRead, hInpFile );
            if ( uRead != toRead )
            {
                CloseOneFile ( &hTxtFile );
                CloseOneFile ( &hInpFile );

                CloseOneFile ( &hLogFile );
                CloseOneFile ( &hHeaFile );

                Cleanup();

                return bResult;
            }
        }
    }

    //
    for ( int i = 0; i < nbLevelPath; i++ )
    {
        Print ( hTxtFile, "; LevelpathStringOffsets %d : %d %s\n", 
                    i, LevelpathStringOffsets [ i ], LevelpathStringBlockData + LevelpathStringOffsets [ i ] );
    }

    //
    //
    int nbLevels = scriptLevelHeader.NumTotalLevels;
    ZeroMemory ( &LevelBlockDataOffsets, sizeof(LevelBlockDataOffsets) );
    if ( nbLevels > 0 )
    {
        Print ( hLogFile, "Reading LevelBlockDataOffsets %d\n", sizeof(xuint16_t)*nbLevels );
        uRead = fread ( (char*) &LevelBlockDataOffsets, 1, sizeof(xuint16_t)*nbLevels, hInpFile );
        if ( uRead != sizeof(xuint16_t)*nbLevels )
        {
            CloseOneFile ( &hTxtFile );
            CloseOneFile ( &hInpFile );

            CloseOneFile ( &hLogFile );
            CloseOneFile ( &hHeaFile );

            Cleanup();

            return bResult;
        }
    }

    //
    ZeroMemory ( LevelBlockData, sizeof(LevelBlockData) );
    Print ( hLogFile, "Reading LevelBlockData %d\n", scriptLevelHeader.LevelBlockLen );
    uRead = fread ( LevelBlockData, 1, scriptLevelHeader.LevelBlockLen, hInpFile );
    if ( uRead != scriptLevelHeader.LevelBlockLen )
    {
        CloseOneFile ( &hTxtFile );
        CloseOneFile ( &hInpFile );

        CloseOneFile ( &hLogFile );
        CloseOneFile ( &hHeaFile );

        Cleanup();

        return bResult;
    }


    //
    //  Finally Languages

    //
    Print ( hTxtFile, "\n" );
    Print ( hTxtFile, "; LANGUAGE.DAT will be treated as LANGUAGE.DAT.TXT by the write procedure\n" );
    Print ( hTxtFile, "; So leave this section as it is\n" );
    Print ( hTxtFile, "; The Index will not be treated : so order them correctly\n" );
    Print ( hTxtFile, "[Language]\n" );

    ZeroMemory ( LanguageBlockData, sizeof(LanguageBlockData) );
    long lPos = ftell ( hInpFile );
    LanguageBlockLen = (xuint16_t) fread ( LanguageBlockData, 1, sizeof(LanguageBlockData), hInpFile );
    Print ( hLogFile, "Reading LanguageBlockData %d\n", LanguageBlockLen );
    char *pLang = (char*) LanguageBlockData;
    int iLang = 0;
    while ( *pLang != '\0' )
    {
        //  Next Generation
        if ( memcmp ( pLang, "NG", 2 ) == 0 )
        {
            //  Offset of pLang from LanguageBlockData
            long offset = (long) ( (BYTE*) pLang - (BYTE*)LanguageBlockData );

            //  Offset of pLang from start of file
            offset += lPos;
            Print ( hTxtFile, "; Next Generation File.\n" );

            //
            //  Overrided in AnalyzeNGScript
            TR4NGOffset = offset;

            //
            AnalyzeNGScript(pLang, offset, hTxtFile);

            break;
        }
        Print ( hTxtFile, "File=\t%d,%s\n", iLang, pLang );
        ReadTRXLanguage ( pLang, pDirectory, iLang, version, bWrite );
        pLang += strlen ( pLang ) + 1;
        iLang ++;
    }
    Print ( hTxtFile, "\n" );

    //  Treat After Language has been read
    for ( int i = 0; i < nbLevels; i++ )
    {
        ZeroMemory ( ItemsTable, sizeof(ItemsTable) );

        //
        int len = scriptLevelHeader.LevelBlockLen;
        if ( i + 1 < nbLevels )
        {
            len = LevelBlockDataOffsets [ i + 1 ];
        }

        Print ( hTxtFile, "; LevelBlockDataOffsets %d : %d 0x%x (len=%d)\n", i,
            LevelBlockDataOffsets [ i ], LevelBlockDataOffsets [ i ], len - LevelBlockDataOffsets [ i ] );

        //
        //  Treat Level Data
        TreatLevelData ( pDirectory, szDataFileType, hTxtFile, LevelBlockDataOffsets [ i ],  len, i, version, function );

        //
        WriteHeader ( version, i, function );
    }

    //
    WriteHeader ( version, -1, function );

    //
    CloseOneFile ( &hTxtFile );
    CloseOneFile ( &hInpFile );

    CloseOneFile ( &hLogFile );
    CloseOneFile ( &hHeaFile );

    bResult     = TRUE;

    //
    Cleanup();

    //
    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Pathname is the savegame name
/////////////////////////////////////////////////////////////////////////////
BOOL IsScriptBlinded ( const char *pathname )
{
    //
    static char szScript [ MAX_PATH ];
    static char szDirectory [ MAX_PATH ];

    strcpy_s ( szScript, sizeof(szScript), pathname );
    RemoveFilename ( szScript );
    strcpy_s ( szDirectory, sizeof(szDirectory), szScript );
    strcat_s ( szScript, sizeof(szScript), "\\SCRIPT.DAT" );

    //
    BOOL bRead = ReadTRXScript ( szScript, szDirectory, 4, false, NULL );
    if ( ! bRead )
    {
        return FALSE;
    }

    if ( BlindOffset [ 0 ] == 0 || BlindOffset [ 1 ] == 0 || BlindOffset [ 2 ] == 0 )
    {
        return FALSE;
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Unblind Savegame
/////////////////////////////////////////////////////////////////////////////
BOOL UnBlindTRXScript ( const char *pathname, const char *pDirectory )
{
    //
    static char szUnBlindedScript [ MAX_PATH ];

    BOOL bRead = ReadTRXScript ( pathname, pDirectory, 4, false, NULL );
    if ( ! bRead )
    {
        return FALSE;
    }

    if ( BlindOffset [ 0 ] == 0 || BlindOffset [ 1 ] == 0 || BlindOffset [ 2 ] == 0 )
    {
        return FALSE;
    }

    //
    BackupTRXScript ( pathname );

    //
    FILE *hInpFile = NULL;
    FILE *hOutFile = NULL;

    //
    fopen_s ( &hInpFile, pathname, "rb" );
    if ( hInpFile == NULL )
    {
        return FALSE;
    }

    //
    strcpy_s ( szUnBlindedScript, sizeof(szUnBlindedScript), pathname );
    RemoveFileType ( szUnBlindedScript );
    strcat_s ( szUnBlindedScript, sizeof(szUnBlindedScript), ".unblinded.dat" );

    //
    fopen_s ( &hOutFile, szUnBlindedScript, "wb" );
    if ( hOutFile == NULL )
    {
        CloseOneFile ( &hInpFile );
        return FALSE;
    }

    //  First Copy All
    BOOL bContinue = TRUE;
    do
    {
        ZeroMemory ( BlindBuffer, sizeof(BlindBuffer) );
        size_t iRead = fread ( BlindBuffer, 1, sizeof(BlindBuffer), hInpFile );
        if ( iRead > 0 )
        {
            fwrite ( BlindBuffer, 1, iRead, hOutFile );
        }
        else
        {
            bContinue = FALSE;
        }

    } while ( bContinue );

    //  Then Write Altered Bytes
    fseek ( hOutFile, (long) BlindOffset [ 0 ], SEEK_SET );
    fwrite ( &BlindValues [ 0 ], 1, sizeof(BYTE), hOutFile );

    fseek ( hOutFile, (long) BlindOffset [ 1 ], SEEK_SET );
    fwrite ( &BlindValues [ 1 ], 1, sizeof(BYTE), hOutFile );

    fseek ( hOutFile, (long) BlindOffset [ 2 ], SEEK_SET );
    fwrite ( &BlindValues [ 2 ], 1, sizeof(BYTE), hOutFile );

    //
    CloseOneFile ( &hInpFile );
    CloseOneFile ( &hOutFile );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Unblind Savegame
/////////////////////////////////////////////////////////////////////////////
BOOL UnSoftTRXScript ( const char *pathname, const char *pDirectory )
{
    //
    static char szUnSoftScript [ MAX_PATH ];

    BOOL bRead = ReadTRXScript ( pathname, pDirectory, 4, false, NULL );
    if ( ! bRead )
    {
        return FALSE;
    }

    if ( SoftOffset [ 0 ] == 0 || SoftOffset [ 1 ] == 0 || SoftOffset [ 2 ] == 0 )
    {
        return FALSE;
    }

    //
    BackupTRXScript ( pathname );

    //
    FILE *hInpFile = NULL;
    FILE *hOutFile = NULL;

    //
    fopen_s ( &hInpFile, pathname, "rb" );
    if ( hInpFile == NULL )
    {
        return FALSE;
    }

    //
    strcpy_s ( szUnSoftScript, sizeof(szUnSoftScript), pathname );
    RemoveFileType ( szUnSoftScript );
    strcat_s ( szUnSoftScript, sizeof(szUnSoftScript), ".unsoft.dat" );

    //
    fopen_s ( &hOutFile, szUnSoftScript, "wb" );
    if ( hOutFile == NULL )
    {
        CloseOneFile ( &hInpFile );
        return FALSE;
    }

    //  First Copy All
    BOOL bContinue = TRUE;
    do
    {
        ZeroMemory ( SoftBuffer, sizeof(SoftBuffer) );
        size_t iRead = fread ( SoftBuffer, 1, sizeof(SoftBuffer), hInpFile );
        if ( iRead > 0 )
        {
            fwrite ( SoftBuffer, 1, iRead, hOutFile );
        }
        else
        {
            bContinue = FALSE;
        }

    } while ( bContinue );

    //  Then Write Altered Bytes
    fseek ( hOutFile, (long) SoftOffset [ 0 ], SEEK_SET );
    fwrite ( &SoftValues [ 0 ], 1, sizeof(BYTE), hOutFile );

    fseek ( hOutFile, (long) SoftOffset [ 1 ], SEEK_SET );
    fwrite ( &SoftValues [ 1 ], 1, sizeof(BYTE), hOutFile );

    fseek ( hOutFile, (long) SoftOffset [ 2 ], SEEK_SET );
    fwrite ( &SoftValues [ 2 ], 1, sizeof(BYTE), hOutFile );

    //
    CloseOneFile ( &hInpFile );
    CloseOneFile ( &hOutFile );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL WriteNGScript(char *pBYtes, long offset, FILE *hOutFile )
{
    //
    TRNGSPECIFIC *pTRNG = (TRNGSPECIFIC *) pBYtes;
    if ( memcmp ( pTRNG->signature, "NG", sizeof(pTRNG->signature) ) != 0 )
    {
        return FALSE;
    }

    //
    DWORD dwTotalLength         = 0;

    //
    fwrite ( "NG", 1, strlen("NG"), hOutFile );
    dwTotalLength   += (DWORD) strlen("NG");

    //
    TRNGITERATION *pIteration = & pTRNG->iteration;

    //
    BOOL bContinue              = TRUE;
    DWORD length                = 0;
    WORD *pCodeOp               = NULL;
    DWORD ExtraWords            = 0;
    WORD *pValues               = NULL;

    WORD ctnSettings            = 0;
    DWORD ctnSettingsAddress    = 0;

    //
    while ( bContinue )
    {
        length  = pIteration->w2.length;
        pCodeOp = &pIteration->w2.codeOp;
        pValues = pIteration->w2.values;

        //
        if ( pIteration->w2.length & 0x8000 )
        {
            // size e' DWORD
            length      = ( ( (DWORD) (pIteration->w3.lengthHi & 0x7fff) ) << 16 ) + (DWORD) pIteration->w3.lengthLo;
            pCodeOp     = &pIteration->w3.codeOp;
            pValues     = pIteration->w3.values;
            ExtraWords  = 3;
        }
        else
        {
            // size e' WORD
            length      = pIteration->w2.length;
            ExtraWords  = 2;
        }

        DWORD relativeAddress = NULL;

        //
        if ( length == 0 )
        {
            relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;

            //
            //  Normally followed by NGLME + length (DWORD)
            char *pEnd = ( char * ) pValues;
            if ( memcmp ( pEnd, "NGLE", strlen("NGLE") ) == 0 )
            {
                relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset + sizeof(WORD);
            }

            //
            bContinue = FALSE;

            //
            fwrite ( pIteration, 1, 2 * sizeof(WORD), hOutFile );
            dwTotalLength   += 2 * sizeof(WORD);
            break;
        }

        if ( *pCodeOp < 0x8000 || *pCodeOp > 0x80ff )
        {
            relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;
            bContinue = FALSE;

            //
            fwrite ( pIteration, 1, 2 * sizeof(WORD), hOutFile );
            dwTotalLength   += 2 * sizeof(WORD);

            break;
        }

        //
        //
        relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, pBYtes ) + (DWORD) offset;

        //
        //  Trace items
		switch (*pCodeOp)
        {
            //
		    case NGTAG_SCRIPT_OPTIONS:
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
                break;
            }

            //
		    case NGTAG_CONTROLLO_OPTIONS:
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
                break;
            }

            //
		    case NGTAG_LEVEL_NAMES:
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
                break;
            }

            //
            case NGTAG_FLAG_LEVEL_TR4 :
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
                break;
            }

            //
            case NGTAG_TOM_VERSION:
            case NGTAG_VERSION_HEADER :
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
                break;
            }

            //
		    case NGTAG_SCRIPT_LEVEL:
            {
                //
                long IterationPosition = ftell ( hOutFile );

                //  Write Header
                fwrite ( pIteration, 1, ExtraWords * sizeof(WORD), hOutFile );
                dwTotalLength   += ExtraWords * sizeof(WORD);

                //
                DWORD toSubtract = 0;

                //
                int indice      = 0;
                while ( ( pValues[indice] & 0xff ) != 0 )
                {
                    relativeAddress = CTRXTools::RelativeAddress ( &pValues[indice], pBYtes ) + (DWORD) offset;
                    BOOL bRemoved = FALSE;

                    //
                    //  For Address
                    for ( int i = 0; i < ArgumentsCount; i++ )
                    {
                        if ( relativeAddress >= ArgumentsBeg [ i ] && relativeAddress <= ArgumentsEnd [ i ] )
                        {
                            bRemoved = TRUE;
                            break;
                        }
                    }

                    WORD TotWords   = pValues[indice] & 0xff;
                    WORD TagScript  = pValues[indice]  >> 8;

                    //
                    //  For TAG
                    for ( int i = 0; i < ArgumentsTagCount; i++ )
                    {
                        if ( TagScript == ArgumentsTag [ i ] )
                        {
                            bRemoved = TRUE;
                            break;
                        }
                    }

                    //  Write Data
                    if ( ! bRemoved )
                    {

                        fwrite ( &pValues[indice], 1, ( TotWords + 1 ) * sizeof(WORD), hOutFile );
                        dwTotalLength   += ( TotWords + 1 ) * sizeof(WORD);
                    }
                    else
                    {
                        toSubtract  += ( TotWords + 1 );
                    }

                    indice  += ( TotWords + 1 );
                }

                //  Write Last Word
                fwrite ( &pValues[indice], 1, sizeof(WORD), hOutFile );
                dwTotalLength   += sizeof(WORD);

                //  Alter Tag length
                if ( toSubtract > 0 )
                {
                    long CurrentPosition = ftell ( hOutFile );
                    if ( ExtraWords == 3 )
                    {
                        pIteration->w3.lengthLo     = (length - toSubtract) & 0xffff;
                        pIteration->w3.lengthHi     = ( (length - toSubtract) >> 16 ) | 0x8000;
                    }
                    else // ExtraWords == 2
                    {
                        pIteration->w2.length   -= (WORD) toSubtract;
                    }

                    //  Write Back
                    fseek ( hOutFile, IterationPosition, SEEK_SET );
                    fwrite ( pIteration, 1, ExtraWords * sizeof(WORD), hOutFile );

                    //  Re Position
                    fseek ( hOutFile, CurrentPosition, SEEK_SET );
                }

                //
                break;
            }

            default :
            {
                fwrite ( pIteration, 1, length * sizeof(WORD), hOutFile );
                dwTotalLength   += length * sizeof(WORD);
            }
        }

        //
        pIteration = ( TRNGITERATION * ) ( ( WORD * ) pIteration + length );
    };

    //
    dwTotalLength   += (DWORD)strlen("NGLE");
    dwTotalLength   += sizeof(DWORD);

    fwrite ( "NGLE", 1, strlen("NGLE"), hOutFile );

    fwrite ( &dwTotalLength, 1, sizeof(DWORD), hOutFile );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL RemoveTRXScript ( const char *pathname, const char *pDirectory, const char *pArguments )
{
    static char szAlteredScript [ MAX_PATH ];

    BOOL bRead = ReadTRXScript ( pathname, pDirectory, 4, false, NULL );
    if ( ! bRead )
    {
        return FALSE;
    }

    if ( TR4NGOffset == NULL || TR4NGSize == 0 )
    {
        return FALSE;
    }

    //
    BackupTRXScript ( pathname );

    //
    char *token         =   NULL;
    char *next_token    =   NULL;

    //
    ArgumentsCount      = 0;
    ArgumentsTagCount   = 0;

    ZeroMemory ( szArguments, sizeof(szArguments) );
    strcpy_s ( szArguments, sizeof(szArguments), pArguments );
    token = strtok_s( szArguments, ",", &next_token);
    while ( token != NULL )
    {
        //  Treat One

        //  Tag
        if ( _strnicmp ( token, "t:", strlen("t:") ) == 0 )
        {
            token   =   token + strlen("t:");

            int number;
            if ( strncmp ( token, "0x", 2 ) == 0 )
            {
                sscanf_s ( token + strlen("0x"), "%x", &number );
            }
            else if ( strncmp ( token, "x", 1 ) == 0 )
            {
                sscanf_s ( token + strlen("x"), "%x", &number );
            }
            else
            {
                sscanf_s ( token, "%x", &number );
            }

            ArgumentsTag [ ArgumentsTagCount ] = number;

            ArgumentsTagCount++;
        }
        //  Or Address
        else
        {
            int number;
            if ( strncmp ( token, "0x", 2 ) == 0 )
            {
                sscanf_s ( token + strlen("0x"), "%x", &number );
            }
            else if ( strncmp ( token, "x", 1 ) == 0 )
            {
                sscanf_s ( token + strlen("x"), "%x", &number );
            }
            else
            {
                sscanf_s ( token, "%x", &number );
            }

            //
            ArgumentsBeg [ ArgumentsCount ] = number;
            ArgumentsEnd [ ArgumentsCount ] = number;

            char *pSecond = strchr ( token, '-' );
            if ( pSecond != NULL )
            {
                pSecond++;

                if ( strncmp ( pSecond, "0x", 2 ) == 0 )
                {
                    sscanf_s ( pSecond + strlen("0x"), "%x", &number );
                }
                else if ( strncmp ( pSecond, "x", 1 ) == 0 )
                {
                    sscanf_s ( pSecond + strlen("x"), "%x", &number );
                }
                else
                {
                    sscanf_s ( pSecond, "%x", &number );
                }

                ArgumentsEnd [ ArgumentsCount ] = number;
            }

            ArgumentsCount++;
        }

        //  Next
        token = strtok_s( NULL, ",", &next_token);
    }

    //
    FILE *hInpFile = NULL;
    FILE *hOutFile = NULL;

    //
    fopen_s ( &hInpFile, pathname, "rb" );
    if ( hInpFile == NULL )
    {
        return FALSE;
    }

    //
    strcpy_s ( szAlteredScript, sizeof(szAlteredScript), pathname );
    RemoveFileType ( szAlteredScript );
    strcat_s ( szAlteredScript, sizeof(szAlteredScript), ".altered.dat" );

    //
    fopen_s ( &hOutFile, szAlteredScript, "wb" );
    if ( hOutFile == NULL )
    {
        CloseOneFile ( &hInpFile );
        return FALSE;
    }

    //  First Copy All until offset
    BOOL bContinue          = TRUE;
    size_t remainToRead     = TR4NGOffset;
    do
    {
        ZeroMemory ( AlteredBuffer, sizeof(AlteredBuffer) );
        size_t sizeToRead   = min ( sizeof(AlteredBuffer), remainToRead );
        size_t iRead = fread ( AlteredBuffer, 1, sizeToRead, hInpFile );
        if ( iRead > 0 )
        {
            fwrite ( AlteredBuffer, 1, iRead, hOutFile );
            remainToRead -= iRead;
        }
        else
        {
            bContinue = FALSE;
        }

        if ( remainToRead == 0 )
        {
            bContinue = FALSE;
        }
    } while ( bContinue );

    //
    //  TRNG Pos is in TR4NGOffset

    //
    //  Copy TRNG Script

    //  Position at 
    fseek ( hInpFile, 0, SEEK_SET );
    ZeroMemory ( AlteredBuffer, sizeof(AlteredBuffer) );
    size_t iRead = fread ( AlteredBuffer, 1, sizeof(AlteredBuffer), hInpFile );

    //
    //  Currrent Pointer is at AlteredBuffer + TR4NGOffset
    char *pBuffer = (char*) AlteredBuffer;
    WriteNGScript( pBuffer + TR4NGOffset, TR4NGOffset, hOutFile );

    //
    CloseOneFile ( &hInpFile );
    CloseOneFile ( &hOutFile );

    //
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL BackupTRXScript ( const char *pathname )
{
    if ( ! PathFileExists ( pathname ) )
    {
        return FALSE;
    }

    //
    static char BackupBuffer [ 4096 ];

    //
    static char szBackupFilename [ MAX_PATH ];
    ZeroMemory ( szBackupFilename, sizeof(szBackupFilename) );
    strcpy_s ( szBackupFilename, sizeof(szBackupFilename), pathname );
    RemoveFileType ( szBackupFilename );
    strcat_s ( szBackupFilename, sizeof(szBackupFilename), ".original.dat" );

    //
    //  Backup script file
    FILE *hInpFile = NULL;
    FILE *hOutFile = NULL;

    //
    if ( ! PathFileExists ( szBackupFilename ) )
    {
        //
        fopen_s ( &hInpFile, pathname, "rb" );
        if ( hInpFile == NULL )
        {
            return FALSE;
        }

        fopen_s ( &hOutFile, szBackupFilename, "wb" );
        if ( hOutFile == NULL )
        {
            CloseOneFile ( &hInpFile );
            return FALSE;
        }

        //
        BOOL bContinue          = TRUE;
        do
        {
            size_t iRead = fread ( BackupBuffer, 1, sizeof(BackupBuffer), hInpFile );
            if ( iRead > 0 )
            {
                size_t iWrite = fwrite ( BackupBuffer, 1, iRead, hOutFile );
            }
            else
            {
                bContinue = FALSE;
            }
        }
        while ( bContinue );

        //
        CloseOneFile ( &hInpFile );
        CloseOneFile ( &hOutFile );

        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL AlterTRXScript ( const char *pathname, const char *pDirectory, bool bAnyLevel )
{
    if ( ! PathFileExists ( pathname ) )
    {
        return FALSE;
    }

    //
    BOOL bRead = ReadTRXScript ( pathname, pDirectory );
    if ( ! bRead )
    {
        return FALSE;
    }

    //  Do Not ALter TRNG Script
    if ( TR4NGOffset != NULL )
    {
        return FALSE;
    }
    
    //
    BackupTRXScript ( pathname );

    //
    DWORD dwAttributes = GetFileAttributes ( pathname );
    //  Remove Read Only
    if ( ( dwAttributes & FILE_ATTRIBUTE_READONLY ) == FILE_ATTRIBUTE_READONLY )
    {
        dwAttributes ^= FILE_ATTRIBUTE_READONLY;
        SetFileAttributes ( pathname, dwAttributes );
    }

    //
    //  Backup script file
    FILE *hInpFile = NULL;

    //
    //  Alter script file : first byte
    fopen_s ( &hInpFile, pathname, "rb+" );
    if ( hInpFile == NULL )
    {
        return FALSE;
    }
    size_t iRead = fread ( AlteredBuffer, 1, sizeof(AlteredBuffer), hInpFile );
    if ( bAnyLevel )
    {
        AlteredBuffer [ 0 ] |= 0x08;
    }
    else
    {
        AlteredBuffer [ 0 ] &= ( 0xff ^ 0x08 );
    }

    //  Enable Load / Save
    AlteredBuffer [ 0 ] |= 0x02;

    //
    fseek ( hInpFile, 0, SEEK_SET );
    size_t iWrite = fwrite ( AlteredBuffer, 1, iRead, hInpFile );
    CloseOneFile ( &hInpFile );

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  File will be LANG.DAT : we will search with .TXT extension replacing or added to filename
/////////////////////////////////////////////////////////////////////////////
BOOL WriteTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version )
{
    //
    BOOL bResult = FALSE;

    MCMemA memLine(LEN_SCRIPT_LINE);

    //
    ZeroMemory ( StringOffsetTable, sizeof(StringOffsetTable) );

    //
    static char szLanguage [ MAX_PATH ];
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
            Print ( hLogFile, "File Open Error %s\n", szLanguage );
            return bResult;
        }

        //
        //  Read All Strings
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
                        stringsType = StringsGeneric;
                    }
                    else if ( strIcmpL ( pBuffer, "[PSXStrings]" ) == 0 )
                    {
                        stringsType = StringsPSX;
                    }
                    else if ( strIcmpL ( pBuffer, "[PCStrings]" ) == 0 )
                    {
                        stringsType = StringsPC;
                    }
                    else
                    {
                        //  We Have a string
                        int stringsCount    = langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;
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
                                langHeader.GenericStringsLen += (xuint16_t) strlen(pText) + 1;
                                break;
                            }
                            case StringsPSX :
                            {
                                langHeader.NumPSXStrings++;
                                langHeader.PSXStringsLen += (xuint16_t) strlen(pText) + 1;
                                break;
                            }
                            case StringsPC :
                            {
                                langHeader.NumPCStrings++;
                                langHeader.PCStringsLen += (xuint16_t) strlen(pText) + 1;
                                break;
                            }
                        }
                    }
                }
            }
            while ( ! feof(hInpFile) && ! ferror( hInpFile) );

        }
        //  Write All
        if ( step == 2 )
        {
            strcpy_s ( szOutputFilename, sizeof(szOutputFilename), szLanguage );
            strcat_s ( szOutputFilename, sizeof(szOutputFilename), ".DAT" );
            fopen_s ( &hOutFile, szOutputFilename, "wb" );
            if ( hOutFile == NULL )
            {
                Print ( hLogFile, "File Open Error %s\n", szOutputFilename );
                CloseOneFile ( &hInpFile );

                Cleanup();

                return bResult;
            }

            //  Write Language Header
            fwrite ( &langHeader, 1, sizeof(langHeader), hOutFile );

            //
            //  Write Offset Table
            int stringsCount    = langHeader.NumGenericStrings + langHeader.NumPSXStrings + langHeader.NumPCStrings;
            fwrite ( StringOffsetTable, 1, sizeof(xuint16_t)*stringsCount, hOutFile );

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

                    //  XOR with 0xA5
                    for ( size_t i = 0; i < strlen(pText); i++ )
                    {
                        pText [ i ] = pText [ i ] ^ 0xA5;
                    }

                    //  Write
                    fwrite ( pText, 1, strlen(pText) + 1, hOutFile );
                }
            }
            while ( ! feof(hInpFile) && ! ferror( hInpFile) );
        }

        CloseOneFile ( &hInpFile );
        CloseOneFile ( &hOutFile );

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

    MCMemA memLine(LEN_SCRIPT_LINE);

    //
    SectionEnum currentSection = Section_None;

    //
    ZeroMemory ( StringTable, sizeof(StringTable) );

    //
    static char szOutputFilename [ MAX_PATH ];

    //
    ZeroMemory ( &scriptHeader, sizeof(scriptHeader) );

    //  Extension
    ZeroMemory ( &scriptLevelHeader, sizeof(scriptLevelHeader) );

    //
    ZeroMemory ( LevelpathStringOffsets, sizeof(LevelpathStringOffsets) );
    ZeroMemory ( LevelpathStringBlockData, sizeof(LevelpathStringBlockData) );

    ZeroMemory ( LevelBlockData, sizeof(LanguageBlockData) );
    ZeroMemory ( LevelBlockDataOffsets, sizeof(LevelBlockDataOffsets) );

    //
    ZeroMemory ( LanguageBlockData, sizeof(LanguageBlockData) );
    LanguageBlockLen    = 0;

    //
    scriptLevelHeader.NumTotalLevels        = 0;
    scriptLevelHeader.NumUniqueLevelPaths   = 0;
    scriptLevelHeader.LevelBlockLen         = 0;
    scriptLevelHeader.LevelpathStringLen    = 0;

    //
    int LevelOffset = -1;
    int TitleOffset = -1;
    int nameIndex   = -1;

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
            Print ( hLogFile, "File Open Error %s\n", pathname );
            CloseOneFile ( &hLogFile );
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
                Print ( hLogFile, "File Open Error %s\n", szOutputFilename );
                CloseOneFile ( &hInpFile );

                CloseOneFile ( &hLogFile );

                Cleanup();

                return bResult;
            }
        }

        //
        //  Read Line
        bool bOptionsDone       = false;

        //
        bool bNeedEndOfLevel    = false;

        //
        do
        {
            char *pBuffer = fgets ( memLine.ptr, memLine.len,  hInpFile );
            if ( pBuffer != NULL )
            {
                RemoveCRLF ( pBuffer );

                //
                //  Detect Sections
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
                    //  No Previous : THis is the first Block

                    //
                    currentSection = Section_Option;

                    //
                    continue;
                }
                else if ( strIcmpL(pBuffer,"[PSXExtensions]" ) == 0 )
                {
                    //
                    //  Write Prevoious : Options
                    if ( ! bOptionsDone )
                    {
                        if ( step == 2 )
                        {
                            Print ( hLogFile, "Writing scriptHeader %d\n", sizeof(scriptHeader) );
                            fwrite ( &scriptHeader, 1, sizeof(scriptHeader), hOutFile );
                        }
                        bOptionsDone    = true;
                    }

                    //
                    currentSection = Section_PSXExtensions;

                    //
                    continue;
                }
                else if ( strIcmpL(pBuffer,"[PCExtensions]" ) == 0 )
                {
                    //
                    //  Write Prevoious : Options
                    if ( ! bOptionsDone )
                    {
                        if ( step == 2 )
                        {
                            Print ( hLogFile, "Writing scriptHeader %d\n", sizeof(scriptHeader) );
                            fwrite ( &scriptHeader, 1, sizeof(scriptHeader), hOutFile );
                        }
                        bOptionsDone    = true;
                    }

                    //
                    currentSection = Section_PCExtensions;

                    //
                    continue;
                }
                else if ( strIcmpL(pBuffer,"[Language]" ) == 0 )
                {
                    //
                    //  Write Prevoious : Extensions
                    if ( step == 2 )
                    {
                        //
                        //  Add 0x83 For The Last Level
                        if ( bNeedEndOfLevel )
                        {
                            //
                            LevelDatax83    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode =   0x83;
                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

                            bNeedEndOfLevel = false;
                        }

                        Print ( hLogFile, "Writing scriptLevelHeader %d\n", sizeof(scriptLevelHeader) );
                        fwrite ( &scriptLevelHeader, 1, sizeof(scriptLevelHeader), hOutFile );

                        //
                        //  Write Prevoious : LevelpathStringBlockData
                        Print ( hLogFile, "Writing LevelpathStringOffsets %d\n", sizeof(xuint16_t)*scriptLevelHeader.NumUniqueLevelPaths );
                        fwrite ( LevelpathStringOffsets, 1, sizeof(xuint16_t)*scriptLevelHeader.NumUniqueLevelPaths, hOutFile );
                        
                        //
                        Print ( hLogFile, "Writing LevelpathStringBlockData %d\n", scriptLevelHeader.LevelpathStringLen );
                        fwrite ( LevelpathStringBlockData, 1, scriptLevelHeader.LevelpathStringLen, hOutFile );

                        //  Write Previous : LevelBlockDataOffsets
                        Print ( hLogFile, "Writing LevelBlockDataOffsets %d\n", sizeof(xuint16_t)*scriptLevelHeader.NumTotalLevels );
                        fwrite ( (char*) &LevelBlockDataOffsets, 1, sizeof(xuint16_t)*scriptLevelHeader.NumTotalLevels, hOutFile );

                        //  Then Data
                        Print ( hLogFile, "Writing LevelBlockData %d\n", scriptLevelHeader.LevelBlockLen );
                        fwrite ( LevelBlockData, 1, scriptLevelHeader.LevelBlockLen, hOutFile );
                        //  Finally Languages
                        //  Write One byte more
                        Print ( hLogFile, "Writing LanguageBlockData %d\n", LanguageBlockLen );
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
                        //  Add 0x83
                        if ( bNeedEndOfLevel )
                        {
                            //
                            LevelDatax83    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode =   0x83;
                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

                            bNeedEndOfLevel = false;
                        }

                        //
                        LevelBlockDataOffsets [ scriptLevelHeader.NumTotalLevels ]  = scriptLevelHeader.LevelBlockLen;

                        //
                        bNeedEndOfLevel = true;
                        scriptLevelHeader.NumTotalLevels++;
                        scriptLevelHeader.NumUniqueLevelPaths   = scriptLevelHeader.NumTotalLevels;
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
                        //  Add 0x83
                        if ( bNeedEndOfLevel )
                        {
                            //
                            LevelDatax83    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode =   0x83;
                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

                            bNeedEndOfLevel = false;
                        }

                        //
                        LevelBlockDataOffsets [ scriptLevelHeader.NumTotalLevels ]  = scriptLevelHeader.LevelBlockLen;

                        //
                        bNeedEndOfLevel = true;

                        //
                        scriptLevelHeader.NumTotalLevels++;
                        scriptLevelHeader.NumUniqueLevelPaths   = scriptLevelHeader.NumTotalLevels;
                    }

                    //
                    continue;
                }

                //
                //  First Step Is Detailed Analysis
                /////////////////////////////////////////
                if ( step == 1 )
                {
                    int count = 0;

                    //
                    //  Inside The [Option]
                    if ( currentSection == Section_Option )
                    {
                        if ( strIcmpL ( pBuffer, "FlyCheat=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                scriptHeader.Options    |= 0x01;
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "LoadSave=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                scriptHeader.Options    |= 0x02;
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "Title=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                scriptHeader.Options    |= 0x04;
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "PlayAnyLevel=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                scriptHeader.Options    |= 0x08;
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "DemoDisc=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                scriptHeader.Options    |= 0x80;
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "InputTimeout=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            scriptHeader.InputTimeout   = GetDWORDValue ( pBuffer );
                        }
                        else if ( strIcmpL ( pBuffer, "Security=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            scriptHeader.Security   = GetBYTEValue ( pBuffer );
                        }
                        else
                        {
                            Print ( hLogFile, "Not Recognized %s\n", pBuffer );
                        }
                    }
                    //
                    //  Inside The [Level] or [Title]
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
                        //  Bit 0 (0x0001) — YoungLara
                        if ( strIcmpL ( pBuffer, "YoungLara=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                if ( LevelOffset >= 0 )
                                {
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0001;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0001;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0002;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0002;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0004;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0004;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0008;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0008;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0010;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0010;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0020;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0020;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0040;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0040;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0080;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0080;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
                                }
                            }
                        }
                        //  Bit 8 (0x0100) — Pulse
                        else if ( strIcmpL ( pBuffer, "Pulse=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                if ( LevelOffset >= 0 )
                                {
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0100;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0100;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
                                }
                            }
                        }
                        //  Bit 9 (0x0200) — ColAddHorizon
                        else if ( strIcmpL ( pBuffer, "ColAddHorizon=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                if ( LevelOffset >= 0 )
                                {
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0200;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0200;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
                                }
                            }
                        }
                        //  Bit 10 (0x0400) — ResetHUB used
                        else if ( strIcmpL ( pBuffer, "ResetHUB=" ) == 0 && HasBooleanValue ( pBuffer )  )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                if ( LevelOffset >= 0 )
                                {
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0400;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0400;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x0800;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x0800;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
                                }
                            }
                        }
                        //  Bit 12 (0x1000) — Timer
                        else if ( strIcmpL ( pBuffer, "Timer=" ) == 0 && HasBooleanValue ( pBuffer ) )
                        {
                            pBuffer = SkipToValue ( pBuffer );
                            if ( IsOptionEnabled ( pBuffer ) )
                            {
                                if ( LevelOffset >= 0 )
                                {
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x1000;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x1000;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x2000;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x2000;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x4000;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x4000;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
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
                                    LevelDatax81    *pData = (LevelDatax81 *)( LevelBlockData + LevelOffset );
                                    pData->levelOptions |= 0x8000;
                                    Print ( hLogFile, "Changing Level %s\n", TraceOperation ( pData, sizeof(LevelDatax81) ) );
                                }
                                else if ( TitleOffset >= 0 )
                                {
                                    LevelDatax82    *pData = (LevelDatax82 *)( LevelBlockData + LevelOffset );
                                    pData->titleOptions |= 0x8000;
                                    Print ( hLogFile, "Changing Title %s\n", TraceOperation ( pData, sizeof(LevelDatax82) ) );
                                }
                                else
                                {
                                    Print ( hLogFile, "No Level\n" );
                                }
                            }
                        }
                        //  0x80
                        else if ( strIcmpL ( pBuffer, "FMV=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax80    data;
                            ZeroMemory ( &data, sizeof(data) );

                            data.opcode =   0x80;
                            data.val    =   GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);
                            Print ( hLogFile, "Adding FMV %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //
                        //  Name
                        else if ( strIcmpL ( pBuffer, "Name=" ) == 0 )
                        {
                            pBuffer         = SkipToValue ( pBuffer );
                            nameIndex       = SearchStringIndex ( pBuffer );
                        }
                        //  0x81 for Level / 0x82 for Title
                        //  We Are Inside Level or Title
                        //
                        //  https://trwiki.earvillage.net/doku.php?id=trs:scripting_tr4_tr5
                        else if ( strIcmpL ( pBuffer, "Level=" ) == 0 )
                        {
                            pBuffer         = SkipToValue ( pBuffer );
                            char *pAudio    = strchr ( pBuffer, ',' );
                            if ( *pAudio != NULL )
                            {
                                *pAudio = '\0';
                                pAudio++;
                            }

                            //
                            //  Ok Store in LevelpathStringOffsets and
                            BOOL bNumericPath   = FALSE;
                            xbiti8 iNumericPath = atoi(pBuffer);
                            if ( IsNumeric(pBuffer) )
                            {
                                //
                                bNumericPath = TRUE;
                            }
                            else
                            {
                                //  We Will Dupplicate PATH
                                LevelpathStringOffsets [ scriptLevelHeader.NumUniqueLevelPaths - 1 ] = scriptLevelHeader.LevelpathStringLen;
                                memcpy_s ( LevelpathStringBlockData + scriptLevelHeader.LevelpathStringLen,
                                            sizeof(LevelpathStringBlockData) - scriptLevelHeader.LevelpathStringLen, pBuffer, strlen(pBuffer) + 1 );
                                scriptLevelHeader.LevelpathStringLen    += (xuint16_t)strlen(pBuffer) + 1;
                            }

                            //
                            BYTE audio = 0;
                            if ( pAudio != NULL )
                            {
                                audio = GetBYTEValue ( pAudio );
                            }

                            //
                            //  xbitu8 stringIndex; xuint16_t levelOptions; xbitu8 pathIndex; xbitu8 audio;
                            if ( currentSection == Section_Level )
                            {
                                //
                                LevelDatax81    data;
                                ZeroMemory ( &data, sizeof(data) );
                                data.opcode         = 0x81;
                                if ( bNumericPath )
                                {
                                    data.pathIndex      = iNumericPath;
                                }
                                else
                                {
                                    data.pathIndex      = scriptLevelHeader.NumTotalLevels - 1;
                                }
                                data.audio          = audio;
                                if ( nameIndex != - 1)
                                {
                                    data.stringIndex    = nameIndex;
                                    Print ( hLogFile, "Level Name %s\n", TraceOperation ( &data, sizeof(data) ) );
                                    nameIndex           = -1;
                                }

                                //
                                LevelOffset = scriptLevelHeader.LevelBlockLen;  //
                                TitleOffset = -1;

                                //
                                memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                            sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                                scriptLevelHeader.LevelBlockLen += sizeof(data);

                                Print ( hLogFile, "Adding Level %s\n", TraceOperation ( &data, sizeof(data) ) );
                            }
                            //  
                            else
                            {
                                //  xbitu8 pathIndex; xuint16_t titleOptions; xbitu8 audio;
                                LevelDatax82    data;
                                ZeroMemory ( &data, sizeof(data) );
                                data.opcode         = 0x82;
                                if ( bNumericPath )
                                {
                                    data.pathIndex      = iNumericPath;
                                }
                                else
                                {
                                    data.pathIndex      = scriptLevelHeader.NumTotalLevels - 1;
                                }
                                data.audio          = audio;

                                //
                                LevelOffset = -1;
                                TitleOffset = scriptLevelHeader.LevelBlockLen;  //

                                //
                                memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                            sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                                scriptLevelHeader.LevelBlockLen += sizeof(data);

                                Print ( hLogFile, "Adding Title %s\n", TraceOperation ( &data, sizeof(data) ) );
                            }
                        }
                        //  0x84
                        else if ( strIcmpL ( pBuffer, "Cut=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax84    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode     = 0x84;
                            data.cutIndex   = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Cut %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x85 0x88
                        else if ( strIcmpL ( pBuffer, "ResidentCut=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax85    data;
                            ZeroMemory ( &data, sizeof(data) );
                            int resident    = GetBYTEValue ( pBuffer );
                            if ( resident >= 1 && resident <= 4 )
                            {
                                data.opcode     = 0x85 + resident - 1;
                            }
                            else
                            {
                                data.opcode     = 0x85;
                            }
                            pBuffer         = SkipToNext ( pBuffer );
                            data.cutIndex   = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding ResidentCut %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x89
                        else if ( strIcmpL ( pBuffer, "Layer1=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax89    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x89;
                            data.red    = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.green  = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.blue   = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.speed  = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Layer1 %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8a
                        else if ( strIcmpL ( pBuffer, "Layer2=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8A    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8a;
                            data.red    = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.green  = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.blue   = GetBYTEValue ( pBuffer );
                            pBuffer     = SkipToNext ( pBuffer );
                            data.speed  = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Layer2 %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8b
                        else if ( strIcmpL ( pBuffer, "UVrotate=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8B    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8B;
                            data.speed  = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding UVrotate %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8c
                        else if ( strIcmpL ( pBuffer, "Legend=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8C    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8C;
                            //
                            //  Search String
                            data.stringIndex = (xbitu8) SearchStringIndex ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Legend %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8d
                        else if ( strIcmpL ( pBuffer, "LensFlare=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8D    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8D;
                            //
                            data.yClicks        = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zClicks        = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xClicks        = GetWORDValue ( pBuffer );

                            pBuffer             = SkipToNext ( pBuffer );
                            data.red            = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.green          = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.blue           = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding LensFlare %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8E
                        else if ( strIcmpL ( pBuffer, "Mirror=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8E    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8E;

                            //
                            data.room           = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAxis          = GetDWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Mirror %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x8F
                        else if ( strIcmpL ( pBuffer, "Fog=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax8F    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x8F;
                            //
                            data.red        = GetBYTEValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.green      = GetBYTEValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.blue       = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Fog %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x90
                        else if ( strIcmpL ( pBuffer, "AnimatingMIP=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax90    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x90;
                            //
                            data.val     = GetBYTEValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding AnimatingMIP %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x91
                        else if ( strIcmpL ( pBuffer, "LoadCamera=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax91    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x91;
                            //
                            //  Search String
                            data.srcX       = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.srcY       = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.srcZ       = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.targX      = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.targY      = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.targZ      = GetDWORDValue ( pBuffer );
                            pBuffer         = SkipToNext ( pBuffer );
                            data.room       = GetBYTEValue ( pBuffer );
                            
                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding LoadCamera %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x92
                        else if ( strIcmpL ( pBuffer, "ResetHUB=" ) == 0 )
                        {
                            //
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            if ( version == 4 )
                            {
                                LevelDatax92    data;
                                ZeroMemory ( &data, sizeof(data) );
                                data.opcode = 0x92;
                                //
                                //  Search String
                                data.levelIndex         = GetBYTEValue ( pBuffer );

                                //
                                memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                            sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                                scriptLevelHeader.LevelBlockLen += sizeof(data);

                                Print ( hLogFile, "Adding ResetHUB Version 4 %s\n", TraceOperation ( &data, sizeof(data) ) );
                            }
                            else if ( version == 5 )
                            {
                                LevelDatax92_5  data;
                                ZeroMemory ( &data, sizeof(data) );
                                data.opcode = 0x92;
                                //
                                //  Search String
                                data.levelIndex         = GetBYTEValue ( pBuffer );
                                for ( int iX = 0; iX < sizeof(data.other); iX++ )
                                {
                                    pBuffer                 = SkipToNext ( pBuffer );
                                    data.other [ iX ]       = GetBYTEValue ( pBuffer );
                                }

                                //
                                memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                            sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                                scriptLevelHeader.LevelBlockLen += sizeof(data);

                                Print ( hLogFile, "Adding ResetHUB Version 5 %s\n", TraceOperation ( &data, sizeof(data) ) );
                            }
                        }
                        else if ( strIcmpL ( pBuffer, "LoseItemAtStartup=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDataxDA    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xDA;
                            //
                            //  Search String
                            data.itemNumber         = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding LoseItemAtStartup %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        
                        //  0x93 -- 0x9E
                        else if ( strIcmpL ( pBuffer, "Key=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x93;

                            int key = GetBYTEValue ( pBuffer );
                            if ( key >= 1 && key <= 12 )
                            {
                                data.opcode = 0x93 + key - 1;
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Key %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0x9F -- 0xAA
                        else if ( strIcmpL ( pBuffer, "Puzzle=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0x9F;

                            int puzzle = GetBYTEValue ( pBuffer );
                            if ( puzzle >= 1 && puzzle <= 12 )
                            {
                                data.opcode = 0x9F + puzzle - 1;
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Puzzle %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0xAB -- 0xAE
                        else if ( strIcmpL ( pBuffer, "Pickup=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xAB;

                            int pickup = GetBYTEValue ( pBuffer );
                            if ( pickup >= 1 && pickup <= 4 )
                            {
                                data.opcode = 0xAB + pickup - 1;
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Pickup %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0xAF -- 0xB1
                        else if ( strIcmpL ( pBuffer, "Examine=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xAF;

                            int examine = GetBYTEValue ( pBuffer );
                            if ( examine >= 1 && examine <= 3 )
                            {
                                data.opcode = 0xAF + examine - 1;
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding Examine %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0xB2 -- 0xC1
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
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xB2;

                            //  8 Keys
                            int key             = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            //  2 Combos
                            int combo           = GetBYTEValue ( pBuffer );
                            if ( key >= 1 && key <= 8 && combo >= 1 && combo <= 2 )
                            {
                                data.opcode = 0xB2 + ( key - 1 ) * 2 + ( combo - 1 );
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding KeyCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0xC2 -- 0xD1
                        else if ( strIcmpL ( pBuffer, "PuzzleCombo=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xC2;

                            //  8 Puzzle
                            int puzzle              = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            //  2 Combos
                            int combo           = GetBYTEValue ( pBuffer );
                            if ( puzzle >= 1 && puzzle <= 8 && combo >= 1 && combo <= 2 )
                            {
                                data.opcode = 0xC2 + ( puzzle - 1 ) * 2 + ( combo - 1 );
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding PuzzleCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        //  0xD2 -- 0xD9
                        else if ( strIcmpL ( pBuffer, "PickupCombo=" ) == 0 )
                        {
                            pBuffer = SkipToValue ( pBuffer );

                            //
                            LevelDatax93    data;
                            ZeroMemory ( &data, sizeof(data) );
                            data.opcode = 0xD2;

                            //  4 Pickup
                            int pickup          = GetBYTEValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            //  2 Combos
                            int combo           = GetBYTEValue ( pBuffer );
                            if ( pickup >= 1 && pickup <= 4 && combo >= 1 && combo <= 2 )
                            {
                                data.opcode = 0xD2 + ( pickup - 1 ) * 2 + ( combo - 1 );
                            }
                            if ( version == 5 && data.opcode > 0x93 )
                            {
                                data.opcode = data.opcode - 1;
                            }
                            pBuffer = SkipToNext ( pBuffer );

                            //
                            //  Search String
                            data.stringIndex    = SearchStringIndex ( pBuffer, ',' );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.height         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.size           = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.yAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.zAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.xAngle         = GetWORDValue ( pBuffer );
                            pBuffer             = SkipToNext ( pBuffer );
                            data.unknown        = GetWORDValue ( pBuffer );

                            //
                            memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                        sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                            scriptLevelHeader.LevelBlockLen += sizeof(data);

                            Print ( hLogFile, "Adding PickupCombo %s\n", TraceOperation ( &data, sizeof(data) ) );
                        }
                        else
                        {
                            Print ( hLogFile, "Not Recognized %s\n", pBuffer );
                        }

                    }
                    //
                    //  Language Treated
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
                                Print ( hLogFile, "Changing Language File Type for %s\n", LanguageFilename );
                                RemoveFileType ( LanguageFilename );
                                strcat_s ( LanguageFilename, sizeof(LanguageFilename), ".DAT" );
                            }
                            //  Write Language has two step inside
                            WriteTRXLanguage ( LanguageFilename, pDirectory, iLang, version );

                            //
                            memcpy_s ( LanguageBlockData + LanguageBlockLen, sizeof(LanguageBlockData) - LanguageBlockLen, LanguageFilename, strlen ( LanguageFilename ) + 1 );
                            LanguageBlockLen += (xuint16_t) strlen ( LanguageFilename ) + 1;
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
                        Print ( hLogFile, "Not Recognized %s\n", pBuffer );
                    }
                }
            }
            else
            {
                //
                //  Add 0x83 For The Last Level
                if ( step == 1 && bNeedEndOfLevel )
                {
                    //
                    LevelDatax83    data;
                    ZeroMemory ( &data, sizeof(data) );
                    data.opcode =   0x83;
                    //
                    memcpy_s ( LevelBlockData + scriptLevelHeader.LevelBlockLen,
                                sizeof(LevelBlockData) - scriptLevelHeader.LevelBlockLen, &data, sizeof(data) );
                    scriptLevelHeader.LevelBlockLen += sizeof(data);

                    Print ( hLogFile, "Adding EndOfLevel %s\n", TraceOperation ( &data, sizeof(data) ) );

                    bNeedEndOfLevel = false;
                }

            }
        }
        while ( ! feof(hInpFile) && ! ferror(hInpFile) );

        //
        //  Write Language at End

        //
        //
        CloseOneFile ( &hOutFile );
        CloseOneFile ( &hInpFile );

        bResult     = TRUE;
    }

    Cleanup();

    CloseOneFile ( &hLogFile );

    return bResult;
}
