#pragma once

//
#include "ReadTRXBasicTypes.h"

#pragma pack(push, pack1, 1)

/**
Options is a set of bit flags with several global game settings (name of the settings directly borrowed from original text scripts distributed with TRLE):
    Bit 0 (0x01) — FlyCheat. Enables debug fly mode activated by typing DOZY ingame.
    Bit 1 (0x02) — LoadSave. When this bit is not set, load and save features are disabled. This option was used for demo versions.
    Bit 2 (0x04) — Title. Specifies if title screen should be displayed or not. If not set, game will start right away after user has launched an application.
    Bit 3 (0x08) — PlayAnyLevel. Gives an access to any level from the title screen.
    Bit 7 (0x80) — DemoDisc. Unknown feature, probably related to game versions deployed on promotional CDs.

 **/

//  Size : 7
struct tr4_script_header // 9 bytes
{
    xuint8_t        Options;
    xuint8_t        Filler[3];     // Unused
    xuint32_t       InputTimeout;
    xuint8_t        Security;
};

//  Size : 15
struct tr4_script_levelheader
{
    xuint8_t        NumTotalLevels;
    xuint16_t       NumUniqueLevelPaths;

    xuint16_t       LevelpathStringLen;
    xuint16_t       LevelBlockLen;

    xuint8_t        PSXLevelString [5];    //  typically ".PSX"
    xuint8_t        PSXFMVString   [5];    //  typically ".FMV"
    xuint8_t        PSXCutString [5];    //  typically ".CUT"
    xuint8_t        Filler [5];            //  Possibly for some additional extension type?

    xuint8_t        PCLevelString  [5];    //  typically ".TR4"
    xuint8_t        PCFMVString    [5];    //  typically ".BIK"
    xuint8_t        PCCutString    [5];    //  typically ".TR4"
    xuint8_t        Unused [5];
};

//
struct tr4_script_levellisting
{
    xuint16_t OffsetsToLevelpathString[1 /* NumTotalLevels */ ];
    xuint8_t LevelpathStringBlock [1 /* LevelpathStringLen */ ];

    xuint16_t OffsetsToLevelData [1 /* NumTotalLevels */ ];
};

//
struct tr4_script_leveldata
{
    xuint8_t LevelData [1 /*LevelDataLen */ ];
};

struct tr4_lang_header
{
    xuint16_t NumGenericStrings;
    xuint16_t NumPSXStrings;
    xuint16_t NumPCStrings;

    xuint16_t GenericStringsLen; //  including the null-terminator bytes
    xuint16_t PSXStringsLen;     //  including the null-terminator bytes
    xuint16_t PCStringsLen;      //  including the null-terminator bytes
};

#if 0
struct tr4_lang_stringdata
{
    string_entry Strings[1/* NumTotalStrings */ ];
};
#endif

//  FMV
struct LevelDatax80
{
    BYTE    opcode;
    xbitu8  val; // 4 least significant bits represent the FMV index; 4 most significant bits (y) represent the FMV trigger bitfield as in y=1<->bit 8 set
};

//  Level
struct LevelDatax81
{
    BYTE        opcode;
    xbitu8      stringIndex;
    xuint16_t   levelOptions;
    xbitu8      pathIndex;
    xbitu8      audio;
};

//  Title
struct LevelDatax82
{
    BYTE        opcode;
    xbitu8      pathIndex;      // Path Index seems to have another meaning in TR5
    xuint16_t   titleOptions;
    xbitu8      audio;
};

//   level-data-end  no arguments - this opcode appears at the end of every level (incl. title) block
struct LevelDatax83
{
    BYTE    opcode;
};

//  CUT
struct LevelDatax84
{
    BYTE    opcode;
    xbitu8  cutIndex;
};

//  ResidentCut
//  85 86 87 88
struct LevelDatax85
{
    BYTE    opcode;
    xbitu8  cutIndex;
};

//  Layer1
struct LevelDatax89
{
    BYTE    opcode;
    xbitu8  red; xbitu8 green; xbitu8 blue; xbiti8 speed;
};

//  Layer2
struct LevelDatax8A
{
    BYTE    opcode;
    xbitu8  red; xbitu8 green; xbitu8 blue; xbiti8 speed;
};

//  UVrotate
struct LevelDatax8B
{
    BYTE    opcode;
    xbiti8  speed;
};

//  Legend
struct LevelDatax8C
{
    BYTE    opcode;
    xbitu8  stringIndex;
};

//  LensFlare
struct LevelDatax8D
{
    BYTE    opcode;
    xuint16_t yClicks; xuint16_t zClicks; xuint16_t xClicks; xbitu8 red; xbitu8 green; xbitu8 blue;
};

//  Mirror
struct LevelDatax8E
{
    BYTE    opcode;
    xbitu8  room; xbit32 xAxis;
};

//  Fog
struct LevelDatax8F
{
    BYTE    opcode;
    xbitu8  red; xbitu8 green; xbitu8 blue;
};

//  AnimatingMIP
struct LevelDatax90
{
    BYTE    opcode;
    xbitu8  val; //: 4 least significant bits represent animatingObjectIndex - 1; 4 most significant bits represent the distance
};

//  LoadCamera
struct LevelDatax91
{
    BYTE    opcode;
    xbit32  srcX; xbit32 srcY; xbit32 srcZ; xbit32 targX; xbit32 targY; xbit32 targZ; xbitu8 room;
};

//  ResetHUB
struct LevelDatax92
{
    BYTE    opcode;
    xbitu8  levelIndex;
};

//  ResetHUB
struct LevelDatax92_5
{
    BYTE    opcode;
    xbitu8  levelIndex;
    xbitu8  other [ 13 ];
};

//  0x93 -> 0xD9
struct LevelDatax93
{
    BYTE        opcode;
    xuint16_t   stringIndex;
    xuint16_t   height;
    xuint16_t   size;
    xuint16_t   yAngle;
    xuint16_t   zAngle;
    xuint16_t   xAngle;
    xuint16_t   unknown;
};

//  LoseItemAtStartup
struct LevelDataxDA
{
    BYTE        opcode;
    xuint16_t   itemNumber;
};


//  TRX1 / TRX2 Structure
struct TRXStruct
{
    char    szSignature [ 4 ];
    WORD    wInitialVersion;
    WORD    wCurrentVersion;
    DWORD   dwCompressedSize;
    DWORD   dwUnCompressedSize;
};

#define TRX_STRING      0x02
#define TRX_DWORD1      0x03
#define TRX_ELEVEN      0x04
#define TRX_BOOLEAN     0x08
#define TRX_DWORD2      0x10

//
#pragma pack(pop, pack1)


//
//  For Adding Items Label
//  For Lavel What is 0
typedef void (*FCT_AddToItemsLabels)( int what, int level, int button, const char *pText, const char *pLevelName );

extern BOOL ReadTRXLanguage (   const char *pFilename, const char *pDirectory, int iLang, int version, bool bWrite = true );
extern BOOL ReadTRXScript (     const char *pathname, const char *pDirectory, int version = 4, bool bWrite = true,
                                FCT_AddToItemsLabels function = NULL );

extern BOOL UnZipTRXSavegame (     const char *pathname, const char *pDirectory, int version = 1 );

//  Pathname is the savegame name
extern BOOL IsScriptBlinded ( const char *pathname );
extern BOOL UnBlindTRXScript ( const char *pathname, const char *pDirectory );

//
extern BOOL UnSoftTRXScript ( const char *pathname, const char *pDirectory );
extern BOOL RemoveTRXScript ( const char *pathname, const char *pDirectory, const char *pArguments );

//
//  Alter Pure TR4 Script
extern BOOL BackupTRXScript ( const char *pathname );
extern BOOL AlterTRXScript ( const char *pathname, const char *pDirectory, bool bAnyLevel );

//
extern BOOL WriteTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version );
extern BOOL WriteTRXScript ( const char *pathname, const char *pDirectory, int version = 4 );

//
extern BOOL AnalyzeNGScript(char *pBYtes, long offset, FILE *hTxtFile );
extern BOOL WriteNGScript(char *pBYtes, long offset, FILE *hOutFile );
extern BOOL ReadTR4ForNG ( const char *pathname );

extern const char *GetTRXScriptVersion ();

extern void CloseOneFile ( FILE **phFile );