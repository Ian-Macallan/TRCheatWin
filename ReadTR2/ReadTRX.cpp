#include "stdafx.h"

#include "windows.h"
#include "io.h"

#include "../TR2LevelFormat.h"
#include "../TR_Areas.h"

#include "ReadTRX.h"
#include "ReadTRXScript.h"

#include "MCMemA.h"

#include "zlib.h"

//
/////////////////////////////////////////////////////////////////////////////
//  http://xproger.info/projects/OpenLara/trs.html
//
/////////////////////////////////////////////////////////////////////////////

// static BYTE  szBuffer [ 32 * 1024 * 1024 ];

// static BYTE  LevelDataCompressed [ 32 * 1024 * 1024 ];
// static BYTE  LevelDataUnCompressed [ 32 * 1024 * 1024 ];
static BYTE *pLevelData = NULL;

static  xint32_t Version; // version (4 bytes)
static  tr_colour Palette[256]; // 8-bit palette (768 bytes)
static  tr_colour4 Palette16[256]; //  (1024 bytes)
static  xuint32_t NumTextiles;
static tr_textile8 Textile8[ 1 ];
static tr_textile16 Textile16[ 1 ];
static xuint32_t Unused; // 32-bit unused value (4 bytes)
static xuint16_t NumRooms; // number of rooms (2 bytes)
static tr_room_info info;           // Where the room exists, in world coordinates
static xuint32_t NumDataWords;       // Number of data words (xuint16_t's)
static xuint16_t Data[1 /* NumDataWords */ ]; // The raw data from which the rest of this is derived

//  tr_room_data : In Data
static xint16_t NumVertices;                   // Number of vertices in the following list
static tr123_room_vertex Vertices[1 /* NumVertices */]; // List of vertices (relative coordinates)
static xint16_t NumRectangles;                 // Number of textured rectangles
static tr_face4 Rectangles[1 /*NumRectangles*/ ];    // List of textured rectangles
static xint16_t NumTriangles;                  // Number of textured triangles
static tr_face3 Triangles[1 /* NumTriangles */ ];      // List of textured triangles
static xint16_t NumSprites;                    // Number of sprites
static tr_room_sprite Sprites[1 /* NumSprites */ ];    // List of sprites

//
static xuint16_t NumPortals;                 // Number of visibility portals to other rooms
static tr_room_portal Portals[1 /* NumPortals*/ ];  // List of visibility portals

static xuint16_t NumZsectors;                                  // ``Width'' of sector list
static xuint16_t NumXsectors;                                  // ``Height'' of sector list
static tr_room_sector SectorList[1 /* NumXsectors * NumZsectors*/ ];  // List of sectors in this room

static xint16_t AmbientIntensity;
static xint16_t AmbientIntensity2;  // Usually the same as AmbientIntensity
static xint16_t LightMode;

static xuint16_t NumLights;                 // Number of point lights in this room
static tr_room_light Lights1[1 /* NumLights */ ];    // List of point lights
static tr3_room_light Lights3[1 /* NumLights */ ];    // List of point lights

static xuint16_t NumStaticMeshes;                            // Number of static meshes
static tr_room_staticmesh StaticMeshes1[1 /* NumStaticMeshes */ ];   // List of static meshes
static tr3_room_staticmesh StaticMeshes3[1 /* NumStaticMeshes */ ];   // List of static meshes

static xint16_t AlternateRoom;
static xint16_t Flags;

static xuint8_t WaterScheme;
static xuint16_t WaterScheme16;
static xuint8_t ReverbInfo;

static xuint8_t Filler;  // Unused.
static xuint16_t NumRoomTextiles; // number of non bumped room tiles (2 bytes)
static xuint16_t NumObjTextiles; // number of object tiles (2 bytes)
static xuint16_t NumBumpTextiles; // number of bumped room tiles (2 bytes)
static xuint32_t Textile32_UncompSize; // uncompressed size (in bytes) of the 32-bit textures chunk (4 bytes)
static xuint32_t Textile32_CompSize; // compressed size (in bytes) of the 32-bit textures chunk (4 bytes)
static xuint32_t Textile16_UncompSize; // uncompressed size (in bytes) of the 16-bit textures chunk (4 bytes)
static xuint32_t Textile16_CompSize; // compressed size (in bytes) of the 16-bit textures chunk (4 bytes)
static xuint32_t Textile32Misc_UncompSize; // uncompressed size (in bytes) of the 32-bit misc textures chunk (4 bytes), should always be 524288
static xuint32_t Textile32Misc_CompSize; // compressed size (in bytes) of the 32-bit misc textures chunk (4 bytes)
static xuint32_t LevelData_UncompSize; // uncompressed size (in bytes) of the level data chunk (4 bytes)
static xuint32_t LevelData_CompSize; // compressed size (in bytes) of the level data chunk (4 bytes)

static xuint8_t AlternateGroup;  // Replaces Filler from TR3
static xuint32_t RoomColour;        // In ARGB format!

static xuint16_t LaraType;
static xuint16_t WeatherType;
static xuint8_t Padding[28];

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
static xuint16_t Separator16;

static  tr_room_sector *AllSectors = NULL;

//  At the end of the TR4 file
#pragma pack(push, pack1, 1)
typedef struct StrEndNgHeader
{
	char EndCheck [ 4 ];    // it should be number 0x454C474E ( "NGLE")
	DWORD SizeNgHeader;     // size of whole extra ng header
} EndNgHeaderFields;
#pragma pack(pop, pack1)

//
/////////////////////////////////////////////////////////////////////////////
//  TRNG
/////////////////////////////////////////////////////////////////////////////
// tabella xor usata per cryptare/decryptare  tr4
static BYTE VetCryptTableXor[16][99] = {
        {56, 34, 233, 79, 32, 241, 55, 105, 21, 19, 211, 144,
        23, 132, 194, 9, 173, 4, 190, 210, 82, 24, 11, 236,
        71, 56, 168, 20, 141, 170, 151, 208, 223, 41, 146, 153,
        200, 248, 222, 112, 189, 26, 124, 219, 191, 139, 39, 83,
        86, 78, 103, 29, 250, 30, 179, 242, 239, 63, 81, 94,
        53, 176, 251, 214, 136, 47, 59, 160, 45, 67, 135, 186,
        34, 100, 37, 225, 104, 213, 207, 90, 254, 84, 15, 35,
        162, 36, 92, 62, 152, 137, 249, 101, 221, 183, 163, 221,
        49, 122, 227},
        {166, 212, 74, 48, 192, 202, 106, 205, 232, 16, 220, 120,
        164, 167, 159, 183, 42, 75, 111, 4, 68, 64, 143, 161,
        175, 134, 2, 76, 40, 217, 128, 140, 240, 125, 148, 216,
        199, 114, 180, 50, 51, 184, 226, 33, 171, 12, 165, 28,
        116, 243, 117, 172, 142, 232, 150, 7, 181, 234, 8, 244,
        46, 130, 202, 240, 247, 38, 154, 129, 18, 27, 187, 218,
        57, 97, 45, 201, 177, 180, 172, 196, 123, 156, 191, 17,
        149, 145, 224, 174, 188, 147, 15, 157, 53, 43, 209, 153,
        253, 206, 161},
        {229, 25, 195, 193, 131, 131, 10, 52, 113, 251, 93, 246,
        109, 197, 1, 198, 185, 155, 245, 231, 87, 7, 60, 89,
        70, 127, 210, 215, 65, 73, 119, 235, 142, 31, 108, 13,
        44, 138, 178, 126, 26, 3, 5, 252, 22, 203, 169, 204,
        98, 229, 158, 237, 255, 14, 228, 95, 238, 133, 56, 34,
        233, 79, 32, 241, 55, 105, 21, 19, 211, 144, 23, 132,
        194, 9, 173, 4, 190, 210, 82, 24, 11, 236, 71, 56,
        168, 20, 141, 170, 151, 208, 223, 41, 146, 153, 200, 248,
        222, 112, 189},
        {26, 124, 219, 191, 139, 39, 83, 86, 78, 103, 29, 250,
        30, 179, 242, 239, 63, 81, 94, 53, 176, 251, 214, 136,
        47, 59, 160, 45, 67, 135, 186, 34, 100, 37, 225, 104,
        213, 207, 90, 254, 84, 15, 35, 162, 36, 92, 62, 152,
        137, 249, 101, 221, 183, 163, 221, 49, 122, 227, 166, 212,
        74, 48, 192, 202, 106, 205, 232, 16, 220, 120, 164, 167,
        159, 183, 42, 75, 111, 4, 68, 64, 143, 161, 175, 134,
        2, 76, 40, 217, 128, 140, 240, 125, 148, 216, 199, 114,
        180, 50, 51},
        {184, 226, 33, 171, 12, 165, 28, 116, 243, 117, 172, 142,
        232, 150, 7, 181, 234, 8, 244, 46, 130, 202, 240, 247,
        38, 154, 129, 18, 27, 187, 218, 57, 97, 45, 201, 177,
        180, 172, 196, 123, 156, 191, 17, 149, 145, 224, 174, 188,
        147, 15, 157, 53, 43, 209, 153, 253, 206, 161, 229, 25,
        195, 193, 131, 131, 10, 52, 113, 251, 93, 246, 109, 197,
        1, 198, 185, 155, 245, 231, 87, 7, 60, 89, 70, 127,
        210, 215, 65, 73, 119, 235, 142, 31, 108, 13, 44, 138,
        178, 126, 26},
        {3, 5, 252, 22, 203, 169, 204, 98, 229, 158, 237, 255,
        14, 228, 95, 238, 133, 56, 34, 233, 79, 32, 241, 55,
        105, 21, 19, 211, 144, 23, 132, 194, 9, 173, 4, 190,
        210, 82, 24, 11, 236, 71, 56, 168, 20, 141, 170, 151,
        208, 223, 41, 146, 153, 200, 248, 222, 112, 189, 26, 124,
        219, 191, 139, 39, 83, 86, 78, 103, 29, 250, 30, 179,
        242, 239, 63, 81, 94, 53, 176, 251, 214, 136, 47, 59,
        160, 45, 67, 135, 186, 34, 100, 37, 225, 104, 213, 207,
        90, 254, 84},
        {15, 35, 162, 36, 92, 62, 152, 137, 249, 101, 221, 183,
        163, 221, 49, 122, 227, 166, 212, 74, 48, 192, 202, 106,
        205, 232, 16, 220, 120, 164, 167, 159, 183, 42, 75, 111,
        4, 68, 64, 143, 161, 175, 134, 2, 76, 40, 217, 128,
        140, 240, 125, 148, 216, 199, 114, 180, 50, 51, 184, 226,
        33, 171, 12, 165, 28, 116, 243, 117, 172, 142, 232, 150,
        7, 181, 234, 8, 244, 46, 130, 202, 240, 247, 38, 154,
        129, 18, 27, 187, 218, 57, 97, 45, 201, 177, 180, 172,
        196, 123, 156},
        {191, 17, 149, 145, 224, 174, 188, 147, 15, 157, 53, 43,
        209, 153, 253, 206, 161, 229, 25, 195, 193, 131, 131, 10,
        52, 113, 251, 93, 246, 109, 197, 1, 198, 185, 155, 245,
        231, 87, 7, 60, 89, 70, 127, 210, 215, 65, 73, 119,
        235, 142, 31, 108, 13, 44, 138, 178, 126, 26, 3, 5,
        252, 22, 203, 169, 204, 98, 229, 158, 237, 255, 14, 228,
        95, 238, 133, 56, 34, 233, 79, 32, 241, 55, 105, 21,
        19, 211, 144, 23, 132, 194, 9, 173, 4, 190, 210, 82,
        24, 11, 236},
        {71, 56, 168, 20, 141, 170, 151, 208, 223, 41, 146, 153,
        200, 248, 222, 112, 189, 26, 124, 219, 191, 139, 39, 83,
        86, 78, 103, 29, 250, 30, 179, 242, 239, 63, 81, 94,
        53, 176, 251, 214, 136, 47, 59, 160, 45, 67, 135, 186,
        34, 100, 37, 225, 104, 213, 207, 90, 254, 84, 15, 35,
        162, 36, 92, 62, 152, 137, 249, 101, 221, 183, 163, 221,
        49, 122, 227, 166, 212, 74, 48, 192, 202, 106, 205, 232,
        16, 220, 120, 164, 167, 159, 183, 42, 75, 111, 4, 188,
        184, 8, 26},
        {40, 254, 121, 196, 159, 81, 248, 4, 105, 245, 12, 80,
        63, 234, 45, 170, 170, 49, 91, 152, 35, 132, 30, 148,
        236, 108, 237, 37, 7, 97, 14, 126, 46, 99, 128, 109,
        166, 249, 67, 104, 111, 158, 19, 248, 138, 147, 51, 82,
        177, 217, 165, 65, 42, 44, 36, 61, 242, 21, 56, 137,
        13, 10, 89, 39, 53, 12, 134, 22, 172, 162, 73, 17,
        118, 71, 25, 93, 144, 60, 58, 250, 251, 129, 171, 233,
        116, 212, 110, 229, 61, 121, 62, 50, 20, 110, 95, 207,
        127, 180, 208},
        {189, 247, 75, 80, 185, 192, 238, 99, 6, 151, 228, 132,
        163, 2, 42, 246, 146, 122, 125, 117, 141, 68, 33, 69,
        218, 94, 23, 102, 120, 133, 92, 215, 102, 253, 175, 154,
        98, 199, 151, 106, 174, 225, 140, 139, 76, 9, 142, 252,
        59, 129, 38, 123, 54, 74, 201, 143, 130, 101, 190, 176,
        32, 140, 5, 34, 15, 72, 88, 160, 11, 18, 64, 112,
        87, 231, 53, 145, 244, 83, 55, 4, 159, 203, 206, 198,
        222, 149, 114, 150, 43, 107, 103, 182, 200, 214, 173, 41,
        115, 79, 1},
        {167, 178, 24, 164, 187, 255, 50, 153, 219, 157, 90, 223,
        78, 72, 209, 119, 204, 135, 155, 27, 156, 211, 181, 16,
        1, 113, 220, 86, 47, 28, 85, 169, 241, 91, 31, 77,
        193, 168, 57, 66, 226, 70, 96, 136, 84, 239, 29, 31,
        23, 48, 161, 195, 230, 124, 188, 184, 8, 26, 40, 254,
        121, 196, 159, 81, 248, 4, 105, 245, 12, 80, 63, 234,
        45, 170, 170, 49, 91, 152, 35, 132, 30, 148, 236, 108,
        237, 37, 7, 97, 14, 126, 46, 99, 128, 109, 166, 249,
        67, 104, 111},
        {158, 19, 248, 138, 147, 51, 82, 177, 217, 165, 65, 42,
        44, 36, 61, 242, 21, 56, 137, 13, 10, 89, 39, 53,
        12, 134, 22, 172, 162, 73, 17, 118, 71, 25, 93, 144,
        60, 58, 250, 251, 129, 171, 233, 116, 212, 110, 229, 61,
        121, 62, 50, 20, 110, 95, 207, 127, 180, 208, 189, 247,
        75, 80, 185, 192, 238, 99, 6, 151, 228, 132, 163, 2,
        42, 246, 146, 122, 125, 117, 141, 68, 33, 69, 218, 94,
        23, 102, 120, 133, 92, 215, 102, 253, 175, 154, 98, 199,
        151, 106, 174},
        {225, 140, 139, 76, 9, 142, 252, 59, 129, 38, 123, 54,
        74, 201, 143, 130, 101, 190, 176, 32, 140, 5, 34, 15,
        72, 88, 160, 11, 18, 64, 112, 87, 231, 53, 145, 244,
        83, 55, 4, 159, 203, 206, 198, 222, 149, 114, 150, 43,
        107, 103, 182, 200, 214, 173, 41, 115, 79, 1, 167, 178,
        24, 164, 187, 255, 50, 153, 219, 157, 90, 223, 78, 72,
        209, 119, 204, 135, 155, 27, 156, 211, 181, 16, 1, 113,
        220, 86, 47, 28, 85, 169, 241, 91, 31, 77, 193, 168,
        57, 66, 226},
        {70, 96, 136, 84, 239, 29, 31, 23, 48, 161, 195, 230,
        124, 188, 184, 8, 26, 40, 254, 121, 196, 159, 81, 248,
        4, 105, 245, 12, 80, 63, 234, 45, 170, 170, 49, 91,
        152, 35, 132, 30, 148, 236, 108, 237, 37, 7, 97, 14,
        126, 46, 99, 128, 109, 166, 249, 67, 104, 111, 158, 19,
        248, 138, 147, 51, 82, 177, 217, 165, 65, 42, 44, 36,
        61, 242, 21, 56, 137, 13, 10, 89, 39, 53, 12, 134,
        22, 172, 162, 73, 17, 118, 71, 25, 93, 144, 60, 58,
        250, 251, 129},
        {171, 233, 116, 212, 110, 229, 61, 121, 62, 50, 20, 110,
        95, 207, 127, 180, 208, 189, 247, 75, 80, 185, 192, 238,
        99, 6, 151, 228, 132, 163, 2, 42, 246, 146, 122, 125,
        117, 141, 68, 33, 69, 218, 94, 23, 102, 120, 133, 92,
        215, 102, 253, 175, 154, 98, 199, 151, 106, 174, 225, 140,
        139, 76, 9, 142, 252, 59, 129, 38, 123, 54, 74, 201,
        143, 130, 101, 190, 176, 32, 140, 5, 34, 15, 72, 88,
        160, 11, 18, 64, 112, 87, 231, 53, 145, 244, 83, 55,
        4, 159, 203}};

// tabella per cryptare e decriptare file tr4 e altra roba
// usata sia in tomb4 che in tom2pc
static BYTE VetCryptOrd[16][50] = {
        {0, 17, 7, 9, 6, 25, 1, 24, 48, 26, 11, 22,
        8, 19, 21, 18, 27, 44, 23, 10, 35, 43, 33, 29,
        30, 41, 3, 28, 12, 40, 38, 36, 39, 13, 14, 46,
        5, 45, 34, 49, 31, 42, 2, 16, 4, 20, 15, 47,
        32, 37},
        {18, 32, 15, 0, 7, 13, 14, 16, 48, 43, 8, 33,
        40, 22, 23, 41, 6, 44, 30, 49, 10, 29, 38, 24,
        35, 31, 46, 5, 36, 3, 19, 34, 21, 26, 12, 1,
        9, 37, 45, 42, 47, 20, 39, 4, 11, 17, 2, 25,
        28, 27},
        {38, 24, 12, 0, 39, 47, 7, 16, 23, 8, 25, 29,
        14, 26, 15, 44, 45, 37, 11, 9, 21, 5, 30, 42,
        33, 36, 20, 40, 41, 3, 6, 19, 1, 17, 22, 34,
        35, 2, 31, 48, 46, 18, 49, 32, 10, 43, 4, 27,
        28, 13},
        {45, 10, 38, 5, 23, 17, 19, 33, 4, 42, 29, 24,
        6, 18, 49, 30, 36, 0, 25, 14, 7, 28, 34, 40,
        16, 8, 32, 31, 11, 27, 20, 21, 35, 1, 9, 46,
        26, 41, 44, 15, 22, 43, 2, 47, 12, 48, 13, 37,
        39, 3},
        {27, 4, 10, 43, 12, 2, 22, 18, 23, 7, 30, 17,
        34, 1, 24, 28, 42, 11, 33, 32, 35, 31, 15, 44,
        20, 40, 36, 25, 9, 46, 16, 26, 0, 13, 41, 8,
        6, 3, 38, 37, 21, 49, 39, 19, 29, 45, 47, 14,
        48, 5},
        {6, 23, 21, 40, 8, 18, 3, 26, 39, 14, 43, 47,
        35, 22, 10, 7, 32, 45, 15, 44, 31, 29, 36, 16,
        33, 0, 28, 1, 41, 11, 2, 24, 5, 38, 37, 4,
        17, 25, 34, 42, 46, 19, 12, 49, 27, 48, 9, 30,
        20, 13},
        {6, 37, 16, 23, 11, 18, 39, 21, 40, 0, 33, 27,
        42, 4, 13, 36, 20, 48, 3, 25, 7, 41, 14, 8,
        10, 15, 45, 38, 43, 44, 34, 12, 2, 29, 47, 24,
        28, 46, 5, 1, 30, 22, 19, 26, 35, 9, 32, 17,
        49, 31},
        {42, 27, 15, 7, 33, 14, 20, 19, 12, 37, 17, 0,
        11, 25, 29, 10, 21, 8, 32, 5, 24, 30, 4, 28,
        34, 18, 45, 43, 36, 9, 40, 41, 44, 1, 23, 13,
        47, 39, 3, 38, 22, 48, 26, 35, 46, 16, 49, 6,
        31, 2},
        {27, 14, 12, 9, 40, 1, 2, 4, 41, 23, 19, 33,
        8, 22, 21, 24, 10, 47, 25, 30, 36, 29, 34, 43,
        6, 38, 5, 32, 35, 11, 20, 18, 48, 42, 7, 49,
        31, 15, 16, 26, 45, 3, 37, 44, 0, 13, 28, 46,
        39, 17},
        {29, 0, 11, 4, 16, 18, 13, 20, 25, 32, 44, 1,
        30, 37, 7, 6, 9, 26, 22, 3, 42, 33, 35, 36,
        31, 15, 2, 14, 34, 19, 39, 46, 45, 41, 8, 21,
        49, 10, 17, 38, 47, 28, 40, 43, 27, 5, 23, 48,
        24, 12},
        {7, 24, 10, 20, 6, 28, 9, 16, 41, 38, 43, 48,
        47, 35, 22, 8, 19, 32, 3, 27, 44, 30, 36, 33,
        34, 46, 29, 17, 39, 2, 5, 25, 4, 0, 37, 40,
        18, 26, 14, 42, 15, 11, 1, 49, 45, 13, 31, 21,
        12, 23},
        {6, 37, 16, 23, 11, 18, 39, 21, 40, 0, 33, 27,
        42, 4, 13, 36, 20, 48, 3, 25, 7, 41, 14, 8,
        10, 15, 45, 38, 43, 44, 34, 12, 2, 29, 47, 24,
        28, 46, 5, 1, 30, 22, 19, 26, 35, 9, 32, 17,
        49, 31},
        {42, 27, 15, 7, 33, 14, 20, 19, 12, 37, 17, 0,
        11, 25, 29, 10, 21, 8, 32, 5, 24, 30, 4, 28,
        34, 18, 45, 43, 36, 9, 40, 41, 44, 1, 23, 13,
        47, 39, 3, 38, 22, 48, 26, 35, 46, 16, 49, 6,
        31, 2},
        {27, 14, 12, 9, 40, 1, 2, 4, 41, 23, 19, 33,
        8, 22, 21, 24, 10, 47, 25, 30, 36, 29, 34, 43,
        6, 38, 5, 32, 35, 11, 20, 18, 48, 42, 7, 49,
        31, 15, 16, 26, 45, 3, 37, 44, 0, 13, 28, 46,
        39, 17},
        {29, 0, 11, 4, 16, 18, 13, 20, 25, 32, 44, 1,
        30, 37, 7, 6, 9, 26, 22, 3, 42, 33, 35, 36,
        31, 15, 2, 14, 34, 19, 39, 46, 45, 41, 8, 21,
        49, 10, 17, 38, 47, 28, 40, 43, 27, 5, 23, 48,
        24, 12},
        {7, 24, 10, 20, 6, 28, 9, 16, 41, 38, 43, 48,
        47, 35, 22, 8, 19, 32, 3, 27, 44, 30, 36, 33,
        34, 46, 29, 17, 39, 2, 5, 25, 4, 0, 37, 40,
        18, 26, 14, 42, 15, 11, 1, 49, 45, 13, 31, 21,
        12, 23}};

#define EXE_TOMB4  1

//  1 texture1 / 2 : texture2 / 3 : testure3  / 4 : Geometry (levelData)
static  int NumeroBloccoCrypt   = 4; // segnala se e' tex1,tex2,tex3 o geometry
static  int TipoExe             = 1;
static  int CalcoloCrypting     = 1;

// viene usato sia per cryptare che per decryptare (nota: se il programma
// attuale (testExe) == 1 (tomb4) allora decripta, se invece e' diverso
// come tom2pc allora cripta
// usare le due seguenti zone globali:
//  BYTE VetCryptTableXor[16][99]
// BYTE VetCryptOrd[16][50]
// nota: il valore SizeZona e' (se tr4) il valore CompressedSize
// viene usato per decidere quale serie di crittografazione usare
// usa valore globale NumeroBloccoCrypt per stabilire lo shift di
// valore dimensione size compressa sulla base del quale
// si sceglie la serie di ordinamento e di xor
void CriptaZona(BYTE *pZona, DWORD SizeZona)
{
    // nota: se si deve criptare bisogna PRIMA invertire l'ordine
    // e dopo fare lo xor
    // se invece si deve criptare bisogna PRIMA fare lo xsor e
    // e dopo cambiare l'ordine
    // per creare piu' confusione effetuare una intuile ripetezione per endere
    // tutto piu' complicato
    BYTE Buffer[234];
    int i;
    int j;
    // int CalcoloCrypting;
    BYTE *pBytes;


    // ottenere impostazione se crypting attivo
    // questo viene impostato sia da tom2pc che da tomb4
    // prima di chiamare questa procedura

    // se e' tom2pc fare prima xor
    // ossia prima fase di crypting
    // adesso in indice2 (bit bassi) c'e' 2
    // adesso fare prima fase di decrypt che e' ordinaemnto inverso
    if ( CalcoloCrypting && TipoExe == EXE_TOMB4 ) {
        // qui bisogna localizzare ogni indice nella tabella
        i = SizeZona >> (NumeroBloccoCrypt+4);
        pBytes = &VetCryptOrd[i & 0x0f][0];
        for (i=0;i<50;i++) {
            for (j=0;j<50;j++) {
                if (pBytes[j] == i) break;
            }
            Buffer[i] = pZona[j];
        }
        for (i=0;i<50;i++) {
            pZona[i] = Buffer[i];
        }
        // FaiCopiaStrana(&pZona[43], &Buffer[43]);
    }

    // adesso fare fase xor che agisce come seconda fase di decrypt
    if ( CalcoloCrypting && TipoExe == 1 ){
        // questa operazione potrebbe essere resa uguale a quella
        // di crypting
        i = SizeZona >> NumeroBloccoCrypt;
        pBytes = (BYTE *) &VetCryptTableXor[i & 0x0f][0];

        for (i=0;i<99;i++) {
            pZona[i] ^= pBytes[i];
        }
    }
}


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
BOOL ExtractData (  FILE *hOutputFile, int game,
                    const char *pPathname, const char *pName, TR_MODE TRMode, const char *pPrefix,
                    FCT_AddToCustomArea function )
{
    MCMemA memBuffer(64 * 1024 * 1024);
    MCMemA memLevelDataCompressed ( 64 * 1024 * 1024 );
    MCMemA memLevelDataUnCompressed ( 128 * 1024 * 1024 );

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
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        if ( TRMode == TRR2_MODE )
        {
            if ( Version != 0x002D )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        if ( TRMode == TRR3_MODE )
        {
            if ( ( Version & 0xffff ) != 0x0038 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        //
        if ( TRMode == TR4_MODE )
        {
            if ( memcmp ( &Version, "TR4", 3 ) != 0 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        if ( TRMode == TRR4_MODE )
        {
            if ( memcmp ( &Version, "TRX4", 4 ) != 0 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        //
        if ( TRMode == TR5_MODE )
        {
            if ( memcmp ( &Version, "TR4", 3 ) != 0 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        //
        if ( TRMode == TRR5_MODE  )
        {
            if ( memcmp ( &Version, "TRX5", 4 ) != 0 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );
                return bResult;
            }
        }

        //
        ZeroMemory ( memBuffer.ptr, memBuffer.len );

        //
        if ( TRMode == TRR2_MODE || TRMode == TRR3_MODE )
        {
            iRead = ReadChunk (  &Palette, sizeof(Palette), hFile );
            iRead = ReadChunk (  &Palette16, sizeof(Palette16), hFile );
        }

        //
        if ( TRMode == TR4_MODE || TRMode == TR5_MODE )
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

            ZeroMemory ( memLevelDataCompressed.ptr, memLevelDataCompressed.len );
            ZeroMemory ( memLevelDataUnCompressed.ptr, memLevelDataUnCompressed.len );

            long lDataPosition = ftell ( hFile );
            iRead = ReadChunk (  &LevelData_UncompSize, sizeof(LevelData_UncompSize), hFile );
            iRead = ReadChunk (  &LevelData_CompSize, sizeof(LevelData_CompSize), hFile );
            iRead = ReadChunk (  memLevelDataCompressed.ptr, LevelData_CompSize, hFile );

            long lUncompSize    = LevelData_UncompSize;
            long lCompSize      = LevelData_CompSize;

            //  Uncompress Level Data
            if ( LevelData_UncompSize != LevelData_CompSize )
            {
                int compressStatus = uncompress2( (Bytef *) memLevelDataUnCompressed.ptr, &LevelData_UncompSize,
                    (Bytef *) memLevelDataCompressed.ptr, &LevelData_CompSize);
                if ( compressStatus != Z_OK )
                {
                    //
                    //  Maybe crypted by TRNG
                    fseek ( hFile, lDataPosition, SEEK_SET );
                    iRead = ReadChunk (  &LevelData_UncompSize, sizeof(LevelData_UncompSize), hFile );
                    iRead = ReadChunk (  &LevelData_CompSize, sizeof(LevelData_CompSize), hFile );
                    iRead = ReadChunk (  memLevelDataCompressed.ptr, LevelData_CompSize, hFile );

                    CriptaZona ( (BYTE *) memLevelDataCompressed.ptr, LevelData_CompSize );
                    compressStatus = uncompress2( (Bytef *) memLevelDataUnCompressed.ptr, &LevelData_UncompSize,
                                                    (Bytef *) memLevelDataCompressed.ptr, &LevelData_CompSize);
                    if ( compressStatus != Z_OK )
                    {
                        PrintStdout ( "Error Uncompressing\r\n" );
                        bResult = FALSE;
                        CloseOneFile ( &hFile );

                        return bResult;
                    }
                }
            }
            else
            {
                memcpy ( memLevelDataUnCompressed.ptr, memLevelDataCompressed.ptr, LevelData_UncompSize );
            }
        }

        //
        if ( TRMode == TRR4_MODE || TRMode == TRR5_MODE )
        {
            iRead = ReadChunk (  &NumRoomTextiles, sizeof(NumRoomTextiles), hFile );
            iRead = ReadChunk (  &NumObjTextiles, sizeof(NumObjTextiles), hFile );
            iRead = ReadChunk (  &NumBumpTextiles, sizeof(NumBumpTextiles), hFile );

            iRead = ReadChunk (  &Textile32_UncompSize, sizeof(Textile32_UncompSize), hFile );
            Textile32_CompSize = Textile32_UncompSize;
            // iRead = ReadChunk (  &Textile32_CompSize, sizeof(Textile32_CompSize), hFile );
            iRead = ReadChunk (  memBuffer.ptr, Textile32_CompSize, hFile );

            // Now for ALEXHUB.TR4 we are at 0x44000A
            iRead = ReadChunk (  &Textile16_UncompSize, sizeof(Textile16_UncompSize), hFile );
            iRead = ReadChunk (  &Textile16_CompSize, sizeof(Textile16_CompSize), hFile );
            iRead = ReadChunk (  memBuffer.ptr, Textile16_CompSize, hFile );

            iRead = ReadChunk (  &Textile32Misc_UncompSize, sizeof(Textile32Misc_UncompSize), hFile );
            iRead = ReadChunk (  &Textile32Misc_CompSize, sizeof(Textile32Misc_CompSize), hFile );
            iRead = ReadChunk (  memBuffer.ptr, Textile32Misc_CompSize, hFile );

            if ( TRMode == TRR5_MODE )
            {
                iRead = ReadChunk (  &LaraType, sizeof(LaraType), hFile );
                iRead = ReadChunk (  &WeatherType, sizeof(WeatherType), hFile );
                iRead = ReadChunk (  &Padding, sizeof(Padding), hFile );
            }

            ZeroMemory ( memLevelDataCompressed.ptr, memLevelDataCompressed.len );
            ZeroMemory ( memLevelDataUnCompressed.ptr, memLevelDataUnCompressed.len );

            long lDataPosition = ftell ( hFile );
            iRead = ReadChunk (  &LevelData_UncompSize, sizeof(LevelData_UncompSize), hFile );
            iRead = ReadChunk (  &LevelData_CompSize, sizeof(LevelData_CompSize), hFile );
            iRead = ReadChunk (  memLevelDataCompressed.ptr, LevelData_CompSize, hFile );

            long lUncompSize    = LevelData_UncompSize;
            long lCompSize      = LevelData_CompSize;

            //  Uncompress Level Data
            if ( LevelData_UncompSize != LevelData_CompSize )
            {
                int compressStatus = uncompress2( (Bytef *) memLevelDataUnCompressed.ptr, &LevelData_UncompSize,
                    (Bytef *) memLevelDataCompressed.ptr, &LevelData_CompSize);
                if ( compressStatus != Z_OK )
                {
                    bResult = FALSE;
                    CloseOneFile ( &hFile );

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
        Print ( hOutputFile, "static TR_AREA %s%s [] =\n{\n", pPrefix, szName );

        //
        if ( TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE )
        {
            //
            if ( NumRooms <= 0 || NumRooms > 1024 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );

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

                //  Sectors
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
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                //
                AllSectors = ( tr_room_sector * ) memBuffer.ptr;

                xint16_t minFloor   = 0x7fff;
                xint16_t maxFloor   = (xint16_t)0x8000;
                xint16_t minCeiling = 0x7fff;
                xint16_t maxCeiling = (xint16_t)0x8000;

                xint16_t avgFloor   = 0;
                xint16_t avgCeiling = 0;

                int solidFloor      = 0;
                int solidCeiling    = 0;

                for ( int h = 0; h < NumXsectors; h++ )
                {
                    for ( int w = 0; w < NumZsectors; w++ )
                    {
                        xint8_t floor       = AllSectors [ h * NumZsectors + w ].Floor;
                        xint8_t ceiling     = AllSectors [ h * NumZsectors + w ].Ceiling;

                        //
                        if ( floor == -127 )
                        {
                            solidFloor++;
                        }
                        else
                        {
                            avgFloor        += floor;
                            if ( floor < minFloor )
                            {
                                minFloor    = floor;
                            }
                            if ( floor > maxFloor )
                            {
                                maxFloor    = floor;        // OK
                            }
                        }

                        //
                        if ( ceiling == -127 )
                        {
                            solidCeiling++;
                        }
                        else
                        {
                            avgCeiling      += ceiling;
                            if ( ceiling < minCeiling )
                            {
                                minCeiling  = ceiling;      //  OK
                            }
                            if ( ceiling > maxCeiling )
                            {
                                maxCeiling  = ceiling;
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
                    avgCeiling  = 0;
                }

                //
                long floorRatio     = ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
                long ceilingRatio   = ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

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
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                //
                Print ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
                    i, info.x, info.z, info.yTop, info.yBottom, NumXsectors, NumZsectors, Flags,
                    minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );

                //  Store In Memory
                if ( function != NULL )
                {
                    TR_AREA trArea;
                    ZeroMemory ( &trArea, sizeof(trArea) );

                    trArea.index                = i;
                    trArea.x                    = info.x;
                    trArea.z                    = info.z;
                    trArea.yTop                 = info.yTop;
                    trArea.yBottom              = info.yBottom;
                    trArea.pText                = "";
                    trArea.xSectors             = NumXsectors;
                    trArea.zSectors             = NumZsectors;
                    trArea.flags                = Flags;
                    trArea.minFloor             = minFloor;
                    trArea.maxFloor             = maxFloor;
                    trArea.minCeiling           = minCeiling;
                    trArea.maxCeiling           = maxCeiling;
                    trArea.solidFloorRatio      = floorRatio;
                    trArea.solidCeilingRatio    = ceilingRatio;
                    trArea.game                 = game;
                    (*function)( &trArea );
                }

            }   // End For

        }   // end TRMode == TRR1_MODE || TRMode == TRR2_MODE || TRMode == TRR3_MODE

        //
        if ( TRMode == TR4_MODE || TRMode == TRR4_MODE )
        {
            pLevelData  = (BYTE*) memLevelDataUnCompressed.ptr;
            pLevelData  += sizeof(xuint32_t);       //  Unused
            NumRooms    =   *( (xuint16_t *) pLevelData );
            pLevelData  += sizeof(NumRooms);
            BYTE *pCurrent = pLevelData;

            //
            if ( NumRooms <= 0 || NumRooms > 1024 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );

                return bResult;
            }

            for ( int i = 0; i < NumRooms; i++ )
            {
                tr4_room *pRoom         = ( tr4_room * ) pLevelData;
                tr_room_info *pInfo     = ( tr_room_info * ) pLevelData;
                pLevelData              += sizeof(tr_room_info);

                NumDataWords            = *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(NumDataWords);
                pLevelData              += NumDataWords * sizeof(xuint16_t);

                pCurrent                = pLevelData;

                NumPortals              = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumPortals);
                pLevelData              += NumPortals * sizeof(tr_room_portal);

                NumZsectors             = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumZsectors);

                NumXsectors             = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumXsectors);

                //
                if ( NumXsectors <= 0 || NumZsectors <= 0 )
                { 
                    bResult = FALSE;
                   CloseOneFile ( &hFile );

                   return bResult;
                }

                //
                if ( NumXsectors > 256 || NumZsectors > 256 )
                {
                    bResult = FALSE;
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                //
                AllSectors = ( tr_room_sector * ) pLevelData;

                xint16_t minFloor   = 0x7fff;
                xint16_t maxFloor   = (xint16_t)0x8000;
                xint16_t minCeiling = 0x7fff;
                xint16_t maxCeiling = (xint16_t)0x8000;

                xint16_t avgFloor   = 0;
                xint16_t avgCeiling = 0;

                int solidFloor      = 0;
                int solidCeiling    = 0;

                for ( int h = 0; h < NumXsectors; h++ )
                {
                    for ( int w = 0; w < NumZsectors; w++ )
                    {
                        xint8_t floor       = AllSectors [ h * NumZsectors + w ].Floor;
                        xint8_t ceiling     = AllSectors [ h * NumZsectors + w ].Ceiling;

                        //
                        if ( floor == -127 )
                        {
                            solidFloor++;
                        }
                        else
                        {
                            avgFloor        += floor;
                            if ( floor < minFloor )
                            {
                                minFloor    = floor;
                            }
                            if ( floor > maxFloor )
                            {
                                maxFloor    = floor;        // OK
                            }
                        }

                        //
                        if ( ceiling == -127 )
                        {
                            solidCeiling++;
                        }
                        else
                        {
                            avgCeiling      += ceiling;
                            if ( ceiling < minCeiling )
                            {
                                minCeiling  = ceiling;      //  OK
                            }
                            if ( ceiling > maxCeiling )
                            {
                                maxCeiling  = ceiling;
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
                    avgCeiling  = 0;
                }

                //
                long floorRatio     = ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
                long ceilingRatio   = ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

                //
                pLevelData              += NumZsectors * NumXsectors * sizeof(tr_room_sector);

                RoomColour              = *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(RoomColour);

                NumLights               = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumLights);

                pLevelData              += NumLights * sizeof(tr4_room_light);

                NumStaticMeshes         = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumStaticMeshes);

                pLevelData              += NumStaticMeshes * sizeof(tr3_room_staticmesh);

                AlternateRoom           = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(AlternateRoom);

                Flags                   = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(Flags);

                WaterScheme             = *( ( xuint8_t *) pLevelData );
                pLevelData              += sizeof(WaterScheme);

                ReverbInfo              = *( ( xuint8_t *) pLevelData );
                pLevelData              += sizeof(ReverbInfo);

                AlternateGroup          = *( ( xuint8_t *) pLevelData );
                pLevelData              += sizeof(AlternateGroup);

                //
                Print ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
                    i, pInfo->x, pInfo->z, pInfo->yTop, pInfo->yBottom, NumXsectors, NumZsectors, Flags,
                    minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );

                //
                if ( function != NULL )
                {
                    //  Store In Memory
                    TR_AREA trArea;
                    ZeroMemory ( &trArea, sizeof(trArea) );

                    trArea.index                = i;
                    trArea.x                    = pInfo->x;
                    trArea.z                    = pInfo->z;
                    trArea.yTop                 = pInfo->yTop;
                    trArea.yBottom              = pInfo->yBottom;
                    trArea.pText                = "";
                    trArea.xSectors             = NumXsectors;
                    trArea.zSectors             = NumZsectors;
                    trArea.flags                = Flags;
                    trArea.minFloor             = minFloor;
                    trArea.maxFloor             = maxFloor;
                    trArea.minCeiling           = minCeiling;
                    trArea.maxCeiling           = maxCeiling;
                    trArea.solidFloorRatio      = floorRatio;
                    trArea.solidCeilingRatio    = ceilingRatio;
                    trArea.game                 = game;

                    (*function)( &trArea );
                }
            }
        }


        //
        if ( TRMode == TR5_MODE || TRMode == TRR5_MODE )
        {
            pLevelData  = (BYTE*) memLevelDataUnCompressed.ptr;

            Unused      = * (xuint32_t * ) pLevelData;
            pLevelData += sizeof(xuint32_t);        //  Unused

            NumRooms    =   *( (xuint16_t *) pLevelData );
            pLevelData  += sizeof(NumRooms);

            xuint16_t unk = *( (xuint16_t *) pLevelData );
            pLevelData  += sizeof(NumRooms);

            //
            if ( NumRooms <= 0 || NumRooms > 1024 )
            {
                bResult = FALSE;
                CloseOneFile ( &hFile );

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
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                pLevelData              += sizeof(XELA);

                RoomDataSize            = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(RoomDataSize);

                pCurrent                = pLevelData;

                Seperator               = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(Seperator);

                EndSDOffset             = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(EndSDOffset);

                StartSDOffset           = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(StartSDOffset);

                Separator               = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(Separator);

                EndPortalOffset         = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(EndPortalOffset);

                tr5_room_info *pInfo    = ( tr5_room_info * ) pLevelData;
                pLevelData              += sizeof(tr5_room_info);

                NumZsectors             = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumZsectors);

                NumXsectors             = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumXsectors);

                //
                if ( NumXsectors > 256 || NumZsectors > 256 )
                {
                    bResult = FALSE;
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                //
                RoomColour              =  *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(RoomColour);

                NumLights               =  *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumLights);

                NumStaticMeshes         =  *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumStaticMeshes);

                ReverbInfo              =  *( ( xuint8_t *) pLevelData );
                pLevelData              += sizeof(ReverbInfo);

                AlternateGroup          =  *( ( xuint8_t *) pLevelData );
                pLevelData              += sizeof(AlternateGroup);

                WaterScheme16           =  *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(WaterScheme16);

                //      xuint32_t Filler[2];    // Both always 0x00007FFF
                pLevelData              += sizeof(xuint32_t) * 2;
                //      xuint32_t Separator[2]; // Both always 0xCDCDCDCD
                pLevelData              += sizeof(xuint32_t) * 2;
                //      xuint32_t Filler;       // Always 0xFFFFFFFF
                pLevelData              += sizeof(xuint32_t);

                AlternateRoom           = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(AlternateRoom);

                Flags                   = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(Flags);

                //  Unknown1
                pLevelData              += sizeof(xuint32_t) * 3;

                Separator               = *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(Separator);

                //  Unknown4
                pLevelData              += sizeof(xuint16_t ) * 2;

                float RoomX             = *( ( float *) pLevelData );
                pLevelData              += sizeof(RoomX );

                float RoomY             = *( ( float *) pLevelData );
                pLevelData              += sizeof(RoomY );

                float RoomZ             = *( ( float *) pLevelData );
                pLevelData              += sizeof(RoomZ );

                //    xuint32_t Separator[4]; // Always 0xCDCDCDCD
                //    xuint32_t Separator;    // 0 for normal rooms and 0xCDCDCDCD for null rooms
                //    xuint32_t Separator;    // Always 0xCDCDCDCD
                pLevelData              += sizeof(xuint32_t)*6;

                NumRoomTriangles        = *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(NumRoomTriangles );

                NumRoomRectangles       = *( ( xuint32_t *) pLevelData );
                pLevelData              += sizeof(NumRoomRectangles );

                Seperator               = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(Seperator);

                LightDataSize           = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(LightDataSize);

                NumLights2              = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(NumLights2);

                //          xuint32_t Unknown6;
                pLevelData              += sizeof(xuint32_t);

                RoomYTop                = *(xuint32_t *) pLevelData;
                float dfRoomYTop        = *(float *) pLevelData;
                pLevelData              += sizeof(RoomYTop);

                RoomYBottom             = *(xuint32_t *) pLevelData;
                float dfRoomYBottom     = *(float *) pLevelData;
                pLevelData              += sizeof(RoomYBottom);

                NumLayers               = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(NumLayers);

                LayerOffset             = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(LayerOffset);

                VerticesOffset          = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(VerticesOffset);

                PolyOffset              = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(PolyOffset);

                PolyOffset2             = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(PolyOffset2);

                NumVertices32           = *(xuint32_t *) pLevelData;
                pLevelData              += sizeof(NumVertices32);

                //   xuint32_t Separator[4];  // Always 0xCDCDCDCD
                pLevelData              += sizeof(xuint32_t) * 4;

                pLevelData              += sizeof(tr5_room_light ) * NumLights;

                //
                AllSectors = ( tr_room_sector * ) pLevelData;

                xint16_t minFloor   = 0x7fff;
                xint16_t maxFloor   = (xint16_t)0x8000;
                xint16_t minCeiling = 0x7fff;
                xint16_t maxCeiling = (xint16_t)0x8000;

                xint16_t avgFloor   = 0;
                xint16_t avgCeiling = 0;

                int solidFloor      = 0;
                int solidCeiling    = 0;

                for ( int h = 0; h < NumXsectors; h++ )
                {
                    for ( int w = 0; w < NumZsectors; w++ )
                    {
                        xint8_t floor       = AllSectors [ h * NumZsectors + w ].Floor;
                        xint8_t ceiling     = AllSectors [ h * NumZsectors + w ].Ceiling;

                        //
                        if ( floor == -127 )
                        {
                            solidFloor++;
                        }
                        else
                        {
                            avgFloor        += floor;
                            if ( floor < minFloor )
                            {
                                minFloor    = floor;
                            }
                            if ( floor > maxFloor )
                            {
                                maxFloor    = floor;        // OK
                            }
                        }

                        //
                        if ( ceiling == -127 )
                        {
                            solidCeiling++;
                        }
                        else
                        {
                            avgCeiling      += ceiling;
                            if ( ceiling < minCeiling )
                            {
                                minCeiling  = ceiling;      //  OK
                            }
                            if ( ceiling > maxCeiling )
                            {
                                maxCeiling  = ceiling;
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
                    avgCeiling  = 0;
                }

                //
                long floorRatio     = ( solidFloor * 100 ) / ( NumZsectors * NumXsectors  );
                long ceilingRatio   = ( solidCeiling * 100 ) / ( NumZsectors * NumXsectors  );

                //
                pLevelData              += NumZsectors * NumXsectors * sizeof(tr_room_sector);

                //
                NumPortals              = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(NumPortals);

                pLevelData              += sizeof(tr_room_portal) * NumPortals;

                //
                Separator16             = *( ( xuint16_t *) pLevelData );
                pLevelData              += sizeof(Separator16);

                //
                pLevelData              += sizeof( tr3_room_staticmesh ) * NumStaticMeshes;

                pLevelData              += sizeof( tr5_room_layer ) * NumLayers;

                //  xuint8_t Faces[(NumRoomRectangles * sizeof(tr_face4) + NumRoomTriangles * (tr_face3)];
                pLevelData              += NumRoomRectangles * sizeof(tr_face4) + NumRoomTriangles * sizeof(tr_face3);

                pLevelData              += sizeof(tr5_room_vertex) * NumVertices32;

                if ( NumXsectors <= 0 || NumZsectors <= 0 )
                {
                    bResult = FALSE;
                    CloseOneFile ( &hFile );

                    return bResult;
                }

                //
                Print ( hOutputFile, "\t//  Room X : %.3f - Y : %.3f - Z: %.3f - YTop : %.3f - YBottom : %.3f \n", RoomX, RoomY, RoomZ, dfRoomYTop, dfRoomYBottom );
                Print ( hOutputFile, "\t{ %d, %ld, %ld, %ld, %ld, \"\", %d, %d, 0x%X, %d, %d, %d, %d, %d, %d, %d, }, // 0x%02X\n",
                    i, pInfo->x, pInfo->z, pInfo->yTop, pInfo->yBottom, NumXsectors, NumZsectors, Flags,
                    minFloor, maxFloor, minCeiling, maxCeiling, floorRatio, ceilingRatio, game, i );

                //
                if ( function != NULL )
                {
                    //  Store In Memory
                    TR_AREA trArea;
                    ZeroMemory ( &trArea, sizeof(trArea) );

                    trArea.index                = i;
                    trArea.x                    = pInfo->x;
                    trArea.z                    = pInfo->z;
                    trArea.yTop                 = pInfo->yTop;
                    trArea.yBottom              = pInfo->yBottom;
                    trArea.pText                = "";
                    trArea.xSectors             = NumXsectors;
                    trArea.zSectors             = NumZsectors;
                    trArea.flags                = Flags;
                    trArea.minFloor             = minFloor;
                    trArea.maxFloor             = maxFloor;
                    trArea.minCeiling           = minCeiling;
                    trArea.maxCeiling           = maxCeiling;
                    trArea.solidFloorRatio      = floorRatio;
                    trArea.solidCeilingRatio    = ceilingRatio;
                    trArea.game                 = game;

                    (*function)( &trArea );
                }

                //
                pLevelData              = pCurrent + RoomDataSize;
            }

        }

        Print ( hOutputFile, "\t{ -1, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, -1, }, // End\n" );
        Print ( hOutputFile, "};\n\n" );

        bResult = TRUE;

        CloseOneFile ( &hFile );
    }

    CloseOneFile ( &hFile );

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
extern BOOL ReadTR4ForNG ( const char *pathname )
{
    BOOL bResult = FALSE;

    FILE *hFile = NULL;
    fopen_s ( &hFile, pathname, "rb" );
    if ( hFile == NULL )
    {
        return bResult;
    }

    //
    static char outFilename [ MAX_PATH ];
    strcpy_s ( outFilename, sizeof(outFilename), pathname );
    strcat_s ( outFilename, sizeof(outFilename), ".log" );

    FILE *hOutFile = NULL;
    fopen_s ( &hOutFile, outFilename, "w" );

    //
    StrEndNgHeader  endNGHeader;

    //
	fseek ( hFile, 0, SEEK_END );
	long fileSize = ftell(hFile);
	fseek ( hFile, 0, SEEK_SET );

	long indexBegin = fileSize - (long) sizeof(StrEndNgHeader);
	fseek ( hFile,indexBegin, SEEK_SET );
	fread(&endNGHeader, sizeof(StrEndNgHeader), 1, hFile );
    if ( memcmp ( endNGHeader.EndCheck, "NGLE", 4 ) != 0 )
    {
        CloseOneFile ( &hFile );
        CloseOneFile ( &hOutFile );
        return bResult;
    }

    MCMemA buffer ( endNGHeader.SizeNgHeader );
    indexBegin = fileSize - endNGHeader.SizeNgHeader;
	fseek ( hFile,indexBegin, SEEK_SET );

    //
	// 
    size_t toRead = endNGHeader.SizeNgHeader - sizeof(endNGHeader);

    //
	size_t iRead = fread ( buffer.ptr, 1, toRead, hFile );

    //
    //  Analyze Buffer
    bResult = AnalyzeNGScript(buffer.ptr, indexBegin, hOutFile );

    //
    CloseOneFile ( &hFile );

    //
    CloseOneFile ( &hOutFile );

    return bResult;
}