#pragma once


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
struct tr4_script_header // 9 bytes
{
	xuxint8_t		Options;
	xuxint8_t		Filler[3];     // Unused
	xuint32_t	InputTimeout;
	xuxint8_t		Security;
};

//
struct tr4_script_levelheader
{
	xuxint8_t		NumTotalLevels;
	xuxint16_t	NumUniqueLevelPaths;

	xuxint16_t	LevelpathStringLen;
	xuxint16_t	LevelBlockLen;

	xuxint8_t		PSXLevelString [5];    //  typically ".PSX"
	xuxint8_t		PSXFMVString   [5];    //  typically ".FMV"
	xuxint8_t		PSXCutString [5];    //  typically ".CUT"
	xuxint8_t		Filler [5];            //  Possibly for some additional extension type?

	xuxint8_t		PCLevelString  [5];    //  typically ".TR4"
	xuxint8_t		PCFMVString    [5];    //  typically ".BIK"
	xuxint8_t		PCCutString    [5];    //  typically ".TR4"
	xuxint8_t		Unused [5];
};

//
struct tr4_script_levellisting
{
	xuxint16_t OffsetsToLevelpathString[1 /* NumTotalLevels */ ];
	xuxint8_t LevelpathStringBlock [1 /* LevelpathStringLen */ ];

	xuxint16_t OffsetsToLevelData [1 /* NumTotalLevels */ ];
};

//
struct tr4_script_leveldata
{
    xuxint8_t LevelData [1 /*LevelDataLen */ ];
};

struct tr4_lang_header
{
    xuxint16_t NumGenericStrings;
    xuxint16_t NumPSXStrings;
    xuxint16_t NumPCStrings;

    xuxint16_t GenericStringsLen; //  including the null-terminator bytes
    xuxint16_t PSXStringsLen;     //  including the null-terminator bytes
    xuxint16_t PCStringsLen;      //  including the null-terminator bytes
};

#if 0
struct tr4_lang_stringdata
{
    string_entry Strings[1/* NumTotalStrings */ ];
};
#endif

//	FMV
struct LevelDatax80
{
	BYTE	opcode;
	bitu8	val; // 4 least significant bits represent the FMV index; 4 most significant bits (y) represent the FMV trigger bitfield as in y=1<->bit 8 set
};

//	Level
struct LevelDatax81 
{
	BYTE		opcode;
	bitu8		stringIndex;
	xuxint16_t	levelOptions;
	bitu8		pathIndex;
	bitu8		audio;
};

//	Title
struct LevelDatax82
{
	BYTE		opcode;
	bitu8		pathIndex;		// Path Index seems to have another meaning in TR5
	xuxint16_t	titleOptions;
	bitu8		audio;
};

//	 level-data-end  no arguments - this opcode appears at the end of every level (incl. title) block
struct LevelDatax83
{
	BYTE	opcode;
};

//	CUT
struct LevelDatax84
{
	BYTE	opcode;
	bitu8	cutIndex;
};

//	ResidentCut
//	85 86 87 88
struct LevelDatax85
{
	BYTE	opcode;
	bitu8	cutIndex;
};

//	Layer1
struct LevelDatax89
{
	BYTE	opcode;
	bitu8	red; bitu8 green; bitu8 blue; bit8 speed;
};

//	Layer2
struct LevelDatax8A
{
	BYTE	opcode;
	bitu8	red; bitu8 green; bitu8 blue; bit8 speed;
};

//	UVrotate
struct LevelDatax8B
{
	BYTE	opcode;
	bit8	speed;
};

//	Legend
struct LevelDatax8C
{
	BYTE	opcode;
	bitu8	stringIndex;
};

//	LensFlare
struct LevelDatax8D
{
	BYTE	opcode;
	xuxint16_t yClicks; xuxint16_t zClicks; xuxint16_t xClicks; bitu8 red; bitu8 green; bitu8 blue;
};

//	Mirror
struct LevelDatax8E
{
	BYTE	opcode;
	bitu8	room; bit32 xAxis;
};

//	Fog
struct LevelDatax8F
{
	BYTE	opcode;
	bitu8	red; bitu8 green; bitu8 blue;
};

//	AnimatingMIP
struct LevelDatax90
{
	BYTE	opcode;
	bitu8	val; //: 4 least significant bits represent animatingObjectIndex - 1; 4 most significant bits represent the distance
};

//	LoadCamera
struct LevelDatax91
{
	BYTE	opcode;
	bit32	srcX; bit32 srcY; bit32 srcZ; bit32 targX; bit32 targY; bit32 targZ; bitu8 room;
};

//	ResetHUB
struct LevelDatax92
{
	BYTE	opcode;
	bitu8	levelIndex;
};

//	ResetHUB
struct LevelDatax92_5
{
	BYTE	opcode;
	bitu8	levelIndex;
	bitu8	other [ 13 ];
};

//	0x93 -> 0xD9
struct LevelDatax93
{
	BYTE		opcode;
	xuxint16_t	stringIndex; 
	xuxint16_t	height; 
	xuxint16_t	size; 
	xuxint16_t	yAngle; 
	xuxint16_t	zAngle; 
	xuxint16_t	xAngle; 
	xuxint16_t	unknown;
};

//	LoseItemAtStartup
struct LevelDataxDA
{
	BYTE		opcode;
	xuxint16_t	itemNumber;
};

//
#pragma pack(pop, pack1)

extern BOOL ReadTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version );
extern BOOL ReadTRXScript ( const char *pathname, const char *pDirectory, int version = 4 );

extern BOOL WriteTRXLanguage ( const char *pFilename, const char *pDirectory, int iLang, int version );
extern BOOL WriteTRXScript ( const char *pathname, const char *pDirectory, int version = 4 );
