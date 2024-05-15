#pragma once

#include <Windows.h>

#include "ReadTRXBasicTypes.h"

#pragma pack(push, pack1, 1)

#
struct tr_colour   // 3 bytes
{
    uint8_t Red;        // Red component (0 -- darkest, 255 -- brightest)
    uint8_t Green;      // Green component (0 -- darkest, 255 -- brightest)
    uint8_t Blue;       // Blue component (0 -- darkest, 255 -- brightest)
};

struct tr_colour4  // 4 bytes
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Unused;
};

struct tr_textile8    // 65536 bytes
{
    uint8_t Tile[256 * 256];
};

struct tr_textile16    // 131072 bytes
{
    WORD Tile[256 * 256];
};

struct tr_textile32    // 256144 bytes
{
    DWORD Tile[256 * 256];
};

struct tr_room_info    // 16 bytes
{
    int32_t x;             // X-offset of room (world coordinates)
    int32_t z;             // Z-offset of room (world coordinates)
    int32_t yBottom;
    int32_t yTop;
};

struct tr5_room_info    // 20 bytes
{
	int32_t x;             // X-offset of room (world coordinates)
	int32_t y;             // Y-offset of room (world coordinates) - only in TR5
	int32_t z;             // Z-offset of room (world coordinates)
	int32_t yBottom;
	int32_t yTop;
};

struct tr_vertex   // 6 bytes
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct tr5_vertex   // 12 bytes
{
    float x;
    float y;
    float z;
};


struct tr_room_vertex  // 8 bytes
{
	tr_vertex Vertex;
	int16_t Lighting;
};

struct tr2_room_vertex  // 12 bytes
{
	tr_vertex Vertex;
	int16_t Lighting;
	uint16_t Attributes; // A set of flags for special rendering effects
	int16_t Lighting2;  // Almost always equal to Lighting1
};

struct tr3_room_vertex  // 12 bytes
{
	tr_vertex Vertex;
	int16_t Lighting;   // Value is ignored!
	uint16_t Attributes; // A set of flags for special rendering effects
	uint16_t Colour;     // 15-bit colour
};

struct tr123_room_vertex  // 12 bytes
{
	tr_vertex Vertex;
	int16_t Lighting;
	uint16_t Attributes; // A set of flags for special rendering effects
	int16_t Lighting2;  // Almost always equal to Lighting1
};

struct tr5_room_vertex  // 28 bytes
{
	tr5_vertex Vertex;     // Vertex is now floating-point
	tr5_vertex Normal;
	uint32_t Colour;     // 32-bit colour
};

struct tr_face4    // 12 bytes
{
    uint16_t Vertices[4];
    uint16_t Texture;
};

struct tr_face3    // 8 bytes
{
    uint16_t Vertices[3];
    uint16_t Texture;
};

struct tr_room_sprite  // 4 bytes
{
    int16_t Vertex;       // Offset into vertex list
    int16_t Texture;      // Offset into sprite texture list
};

struct tr_room_data    // (variable length)
{
    int16_t NumVertices;                   // Number of vertices in the following list
    tr2_room_vertex Vertices[1 /* NumVertices */]; // List of vertices (relative coordinates)

    int16_t NumRectangles;                 // Number of textured rectangles
    tr_face4 Rectangles[1 /*NumRectangles*/ ];    // List of textured rectangles

    int16_t NumTriangles;                  // Number of textured triangles
    tr_face3 Triangles[1 /* NumTriangles */ ];      // List of textured triangles

    int16_t NumSprites;                    // Number of sprites
    tr_room_sprite Sprites[1 /* NumSprites */ ];    // List of sprites
};

struct tr_room_portal  // 32 bytes
{
    uint16_t  AdjoiningRoom; // Which room this portal leads to
    tr_vertex Normal;
    tr_vertex Vertices[4];
};

struct tr_room_sector // 8 bytes
{
    uint16_t FDindex;    // Index into FloorData[]
    uint16_t BoxIndex;   // Index into Boxes[] (-1 if none)
    uint8_t  RoomBelow;  // 255 is none
    int8_t   Floor;      // Absolute height of floor
    uint8_t  RoomAbove;  // 255 if none
    int8_t   Ceiling;    // Absolute height of ceiling
};

struct tr_room_light   // 18 bytes
{
	int32_t x, y, z;       // Position of light, in world coordinates
	uint16_t Intensity1;    // Light intensity
	uint32_t Fade1;         // Falloff value
};

struct tr2_room_light   // 24 bytes
{
	int32_t x, y, z;       // Position of light, in world coordinates
	uint16_t Intensity1;    // Light intensity
	uint16_t Intensity2;    // Only in TR2
	uint32_t Fade1;         // Falloff value
	uint32_t Fade2;         // Only in TR2
};

struct tr3_room_light   // 24 bytes
{
	int32_t x, y, z;       // Position of light, in world coordinates
	tr_colour4 Colour;        // Colour of the light
	uint32_t Intensity;
	uint32_t Fade;          // Falloff value
};

struct tr_room_staticmesh  // 18 bytes
{
    uint32_t x, y, z;    // Absolute position in world coordinates
    uint16_t Rotation;
    uint16_t Intensity1;
    uint16_t MeshID;     // Which StaticMesh item to draw
};

struct tr2_room_staticmesh  // 20 bytes
{
    uint32_t x, y, z;    // Absolute position in world coordinates
    uint16_t Rotation;
    uint16_t Intensity1;
    uint16_t Intensity2; // Absent in TR1
    uint16_t MeshID;     // Which StaticMesh item to draw
};

struct tr3_room_staticmesh  // 20 bytes
{
    uint32_t x, y, z;    // Absolute position in world coordinates
    uint16_t Rotation;
    uint16_t Colour;     // 15-bit colour
    uint16_t Unused;     // Not used!
    uint16_t MeshID;     // Which StaticMesh item to draw
};

struct tr_room  // (variable length)
{
    tr_room_info info;           // Where the room exists, in world coordinates

    uint32_t NumDataWords;       // Number of data words (uint16_t's)
    uint16_t Data[1 /* NumDataWords */ ]; // The raw data from which the rest of this is derived

#if 0
    tr_room_data RoomData;       // The room mesh
#endif

    uint16_t NumPortals;                 // Number of visibility portals to other rooms
    tr_room_portal Portals[1 /* NumPortals */];  // List of visibility portals

    uint16_t NumZsectors;                                  // ``Width'' of sector list
    uint16_t NumXsectors;                                  // ``Height'' of sector list
    tr_room_sector SectorList[ 1 /* NumXsectors * NumZsectors */ ];  // List of sectors in this room

    int16_t AmbientIntensity;

    uint16_t NumLights;                 // Number of lights in this room
    tr_room_light Lights[1 /* NumLights */ ];    // List of lights

    uint16_t NumStaticMeshes;                            // Number of static meshes
    tr2_room_staticmesh StaticMeshes[1 /* NumStaticMeshes */];   // List of static meshes

    int16_t AlternateRoom;
    int16_t Flags;
};

struct tr2_room  // (variable length)
{
    tr_room_info info;           // Where the room exists, in world coordinates

    uint32_t NumDataWords;       // Number of data words (uint16_t's)
    uint16_t Data[1 /* NumDataWords */ ]; // The raw data from which the rest of this is derived

#if 0
    tr_room_data RoomData;       // The room mesh
#endif

    uint16_t NumPortals;                 // Number of visibility portals to other rooms
    tr_room_portal Portals[1 /* NumPortals*/ ];  // List of visibility portals

    uint16_t NumZsectors;                                  // ``Width'' of sector list
    uint16_t NumXsectors;                                  // ``Height'' of sector list
    tr_room_sector SectorList[1 /* NumXsectors * NumZsectors*/ ];  // List of sectors in this room

    int16_t AmbientIntensity;
    int16_t AmbientIntensity2;  // Usually the same as AmbientIntensity
    int16_t LightMode;

    uint16_t NumLights;                 // Number of point lights in this room
    tr_room_light Lights[1 /* NumLights */ ];    // List of point lights

    uint16_t NumStaticMeshes;                            // Number of static meshes
    tr_room_staticmesh StaticMeshes[1 /* NumStaticMeshes */ ];   // List of static meshes

    int16_t AlternateRoom;
    int16_t Flags;
};

struct tr3_room  // (variable length)
{
    tr_room_info info;           // Where the room exists, in world coordinates

    uint32_t NumDataWords;       // Number of data words (uint16_t's)
    uint16_t Data[1 /* NumDataWords */]; // The raw data from which the rest of this is derived
#if 0
    tr_room_data RoomData;       // The room mesh
#endif
    uint16_t NumPortals;                 // Number of visibility portals to other rooms
    tr_room_portal Portals[1 /* NumPortals */ ];  // List of visibility portals

    uint16_t NumZsectors;                                  // ``Width'' of sector list
    uint16_t NumXsectors;                                  // ``Height'' of sector list
    tr_room_sector SectorList[1 /* NumXsectors * NumZsectors */ ];  // List of sectors in this room

    int16_t AmbientIntensity1;  // This and the next one only affect externally-lit objects
    int16_t AmbientIntensity2;  // Usually the same as AmbientIntensity1

    uint16_t NumLights;                 // Number of point lights in this room
    tr3_room_light Lights[ 1 /* NumLights */ ];   // List of point lights

    uint16_t NumStaticMeshes;                            // Number of static meshes
    tr_room_staticmesh StaticMeshes[1 /* NumStaticMeshes */ ];    // List of static meshes

    int16_t AlternateRoom;
    int16_t Flags;

    uint8_t WaterScheme;
    uint8_t ReverbInfo;

    uint8_t Filler;  // Unused.
};

struct tr4_room_light   // 46 bytes
{
	int32_t x, y, z;       // Position of light, in world coordinates
	tr_colour Colour;        // Colour of the light

	uint8_t LightType;
	uint8_t Unknown;       // Always 0xFF?
	uint8_t Intensity;

	float In;            // Also called hotspot in TRLE manual
	float Out;           // Also called falloff in TRLE manual
	float Length;
	float CutOff;

	float dx, dy, dz;    // Direction - used only by sun and spot lights
};

struct tr4_room  // (variable length)
{
    tr_room_info info;           // Where the room exists, in world coordinates

    uint32_t NumDataWords;       // Number of data words (uint16_t's)
    uint16_t Data[1 /*NumDataWords*/]; // The raw data from which the rest of this is derived
#if 0
    tr_room_data RoomData;       // The room mesh
#endif
    uint16_t NumPortals;                 // Number of visibility portals to other rooms
    tr_room_portal Portals[1/*NumPortals*/];  // List of visibility portals

    uint16_t NumZsectors;                                  // ``Width'' of sector list
    uint16_t NumXsectors;                                  // ``Height'' of sector list
    tr_room_sector SectorList[1/*NumXsectors * NumZsectors*/]; // List of sectors in this room

    uint32_t RoomColour;        // In ARGB format!

    uint16_t NumLights;                 // Number of point lights in this room
    tr4_room_light Lights[1/*NumLights*/];   // List of point lights

    uint16_t NumStaticMeshes;                           // Number of static meshes
    tr_room_staticmesh StaticMeshes[1/*NumStaticMeshes*/];   // List of static meshes

    int16_t AlternateRoom;
    int16_t Flags;

    uint8_t WaterScheme;
    uint8_t ReverbInfo;

    uint8_t AlternateGroup;  // Replaces Filler from TR3
};

struct tr5_room_light   // 88 bytes
{
	float x, y, z;       // Position of light, in world coordinates
	float r, g, b;       // Colour of the light

	uint32_t Separator;    // Dummy value = 0xCDCDCDCD

	float In;            // Cosine of the IN value for light / size of IN value
	float Out;           // Cosine of the OUT value for light / size of OUT value
	float RadIn;         // (IN radians) * 2
	float RadOut;        // (OUT radians) * 2
	float Range;         // Range of light

	float dx, dy, dz;    // Direction - used only by sun and spot lights
	int32_t x2, y2, z2;    // Same as position, only in integer.
	int32_t dx2, dy2, dz2; // Same as direction, only in integer.

	uint8_t LightType;

	uint8_t Filler[3];     // Dummy values = 3 x 0xCD
};

struct tr5_room_layer   // 56 bytes
{
    uint32_t NumLayerVertices;   // Number of vertices in this layer (4 bytes)
    uint16_t UnknownL1;
    uint16_t NumLayerRectangles; // Number of rectangles in this layer (2 bytes)
    uint16_t NumLayerTriangles;  // Number of triangles in this layer (2 bytes)
    uint16_t UnknownL2;

    uint16_t Filler;             // Always 0
    uint16_t Filler2;            // Always 0

    // The following 6 floats define the bounding box for the layer

    float    LayerBoundingBoxX1;
    float    LayerBoundingBoxY1;
    float    LayerBoundingBoxZ1;
    float    LayerBoundingBoxX2;
    float    LayerBoundingBoxY2;
    float    LayerBoundingBoxZ2;

    uint32_t Filler3;     // Always 0 (4 bytes)
    uint32_t UnknownL6;   // Unknown
    uint32_t UnknownL7;   // Unknown
    uint32_t UnknownL8;   // Always the same throughout the level.
};

struct tr5_room // (variable length)
{
    char XELA[4];           // So-called "XELA landmark"

    uint32_t RoomDataSize;

    uint32_t Seperator;     // 0xCDCDCDCD (4 bytes)

    uint32_t EndSDOffset;
    uint32_t StartSDOffset;

    uint32_t Separator;     // Either 0 or 0xCDCDCDCD

    uint32_t EndPortalOffset;

    tr_room_info info;

    uint16_t NumZSectors;
    uint16_t NumXSectors;

    uint32_t RoomColour;   // In ARGB format!

    uint16_t NumLights;
    uint16_t NumStaticMeshes;

    uint8_t  ReverbInfo;
    uint8_t  AlternateGroup;
    uint16_t WaterScheme;

    uint32_t Filler[2];    // Both always 0x00007FFF
    uint32_t Separator2[2]; // Both always 0xCDCDCDCD
    uint32_t Filler2;       // Always 0xFFFFFFFF

    uint16_t AlternateRoom;
    uint16_t Flags;

    uint32_t Unknown1;
    uint32_t Unknown2;     // Always 0
    uint32_t Unknown3;     // Always 0

    uint32_t Separator3;    // 0xCDCDCDCD

    uint16_t Unknown4;
    uint16_t Unknown5;

    float RoomX;
    float RoomY;
    float RoomZ;

    uint32_t Separator4[4]; // Always 0xCDCDCDCD
    uint32_t Separator5;    // 0 for normal rooms and 0xCDCDCDCD for null rooms
    uint32_t Separator6;    // Always 0xCDCDCDCD

    uint32_t NumRoomTriangles;
    uint32_t NumRoomRectangles;

    uint32_t Separator7;     // Always 0

    uint32_t LightDataSize;
    uint32_t NumLights2;    // Always same as NumLights

    uint32_t Unknown6;

    int32_t RoomYTop;
    int32_t RoomYBottom;

    uint32_t NumLayers;

    uint32_t LayerOffset;
    uint32_t VerticesOffset;
    uint32_t PolyOffset;
    uint32_t PolyOffset2;   // Same as PolyOffset

    uint32_t NumVertices;

    uint32_t Separator8[4];  // Always 0xCDCDCDCD

    tr5_room_light Lights[1/*NumLights*/];    // Data for the lights (88 bytes * NumRoomLights)
    tr_room_sector SectorList[1/*NumXSectors * NumZSectors*/]; // List of sectors in this room

    uint16_t NumPortals;                 // Number of visibility portals to other rooms
    tr_room_portal Portals[1/*NumPortals*/];  // List of visibility portals

    uint16_t Separator9;  // Always 0xCDCD

    tr3_room_staticmesh StaticMeshes[1/*NumStaticMeshes*/];   // List of static meshes

    tr5_room_layer rl[1/*NumLayers*/]; // Data for the room layers (volumes) (56 bytes * NumLayers)

    uint8_t Faces[1/*(NumRoomRectangles * sizeof(tr_face4) + NumRoomTriangles * (tr_face3)*/];

    tr5_room_vertex Vertices[1/*NumVertices*/];
};

struct tr_mesh_N // (variable length)
{
	tr_vertex Centre;
	int32_t CollRadius;

	int16_t NumVertices;           // Number of vertices in this mesh
	tr_vertex Vertices[1 /* NumVertices */ ]; // List of vertices (relative coordinates)

	int16_t NumNormals;

	// if(NumNormals > 0)
	tr_vertex Normals[1 /* umNormals */];
	//else
	//   int16_t Lights[abs(NumNormals)];

     int16_t NumTexturedRectangles; // number of textured rectangles in this mesh
    tr_face4 TexturedRectangles[1 /* NumTexturedRectangles */ ]; // list of textured rectangles

     int16_t NumTexturedTriangles;  // number of textured triangles in this mesh
    tr_face3 TexturedTriangles[1 /* NumTexturedTriangles */ ]; // list of textured triangles

     int16_t NumColouredRectangles; // number of coloured rectangles in this mesh
    tr_face4 ColouredRectangles[1 /* NumColouredRectangles */]; // list of coloured rectangles

     int16_t NumColouredTriangles; // number of coloured triangles in this mesh
    tr_face3 ColouredTriangles[1 /* NumColouredTriangles */ ]; // list of coloured triangles
};

struct tr_mesh_L // (variable length)
{
	tr_vertex Centre;
	int32_t CollRadius;

	int16_t NumVertices;           // Number of vertices in this mesh
	tr_vertex Vertices[1 /* NumVertices */ ]; // List of vertices (relative coordinates)

	int16_t NumNormals;

	//  if(NumNormals > 0)
	//    tr_vertex Normals[NumNormals];
	//  else
	int16_t Lights[1 /* abs(NumNormals )*/];

     int16_t NumTexturedRectangles; // number of textured rectangles in this mesh
    tr_face4 TexturedRectangles[1 /* NumTexturedRectangles */ ]; // list of textured rectangles

     int16_t NumTexturedTriangles;  // number of textured triangles in this mesh
    tr_face3 TexturedTriangles[1 /* NumTexturedTriangles */ ]; // list of textured triangles

     int16_t NumColouredRectangles; // number of coloured rectangles in this mesh
    tr_face4 ColouredRectangles[1 /* NumColouredRectangles */]; // list of coloured rectangles

     int16_t NumColouredTriangles; // number of coloured triangles in this mesh
    tr_face3 ColouredTriangles[1 /* NumColouredTriangles */ ]; // list of coloured triangles
};

//	PHD File
struct TR1Level
{
	uint32_t Version; // version (4 bytes)
	uint32_t NumTextiles; // number of texture tiles (4 bytes)
	tr_textile8 Textile8[1 /*NumTextiles*/ ]; // 8-bit (palettized) textiles (NumTextiles * 65536 bytes)
	uint32_t Unused; // 32-bit unused value (4 bytes)
	uint16_t NumRooms; // number of rooms (2 bytes)
	tr_room Rooms[1 /* NumRooms */ ]; // room list (variable length)
#if 0
	uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
	uint16_t FloorData[NumFloorData]; // floor data (NumFloorData * 2 bytes)
	uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
	tr_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
	uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
	uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
	uint32_t NumAnimations; // number of animations to follow (4 bytes)
	tr_animation Animations[NumAnimations]; // animation list (NumAnimations * 32 bytes)
	uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
	tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
	uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
	tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
	uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
	tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
	uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
	tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
	uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
	uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
	uint32_t NumModels; // number of models to follow (4 bytes)
	tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
	uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
	tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
	uint32_t NumObjectTextures; // number of object textures to follow (4 bytes) (after AnimatedTextures in TR3)
	tr_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 20 bytes) (after AnimatedTextures in TR3)
	uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
	tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
	uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
	tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
	uint32_t NumCameras; // number of camera data records to follow (4 bytes)
	tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
	uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
	tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
	uint32_t NumBoxes; // number of box data records to follow (4 bytes)
	tr_box Boxes[NumBoxes]; // box data (NumBoxes * 20 bytes [TR1 version])
	uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
	uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
	uint16_t GroundZone[2*NumBoxes]; // ground zone data
	uint16_t GroundZone2[2*NumBoxes]; // ground zone 2 data
	uint16_t FlyZone[2*NumBoxes]; // fly zone data
	uint16_t GroundZoneAlt[2*NumBoxes]; // ground zone data (alternate rooms?)
	uint16_t GroundZoneAlt2[2*NumBoxes]; // ground zone 2 data (alternate rooms?)
	uint16_t FlyZoneAlt[2*NumBoxes]; // fly zone data (alternate rooms?)
	uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
	uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
	uint32_t NumEntities; // number of entities to follow (4 bytes)
	tr_entity Entities[NumEntities]; // entity list (NumEntities * 22 bytes [TR1 version])
	uint8_t LightMap[32 * 256]; // light map (8192 bytes)
	tr_colour Palette[256]; // 8-bit palette (768 bytes)
	uint16_t NumCinematicFrames; // number of cinematic frame records to follow (2 bytes)
	tr_cinematic_frame CinematicFrames[NumCinematicFrames]; // (NumCinematicFrames * 16 bytes)
	uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
	uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
	int16_t SoundMap[256]; // sound map (512 bytes)
	uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
	tr_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
	uint32_t NumSamples; // number of uint8_t's in Samples (4 bytes)
	uint8_t Samples[NumSamples]; // array of uint8_t's -- embedded sound samples in Microsoft WAVE format (NumSamples bytes)
	uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)
	uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
#endif
};

struct TR2Level
{
	int32_t Version; // version (4 bytes)
	tr_colour Palette[256]; // 8-bit palette (768 bytes)
	tr_colour4 Palette16[256]; //  (1024 bytes)
	uint32_t NumTextiles; // number of texture tiles (4 bytes)
	tr_textile8 Textile8[1 /* NumTextiles */ ]; // 8-bit (palettized) textiles (NumTextiles * 65536 bytes)
	tr_textile16 Textile16[1 /* NumTextiles */ ]; // 16-bit (ARGB) textiles (NumTextiles * 131072 bytes)
	uint32_t Unused; // 32-bit unused value (4 bytes)
	uint16_t NumRooms; // number of rooms (2 bytes)
	tr2_room Rooms[1 /* NumRooms */ ]; // room list (variable length)
	uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
	uint16_t FloorData[1 /* NumFloorData*/ ]; // floor data (NumFloorData * 2 bytes)
#if 0
	uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
	tr_mesh_N Meshes[1 /* NumMeshPointers */ ]; // note that NumMeshPointers comes AFTER Meshes[]
	uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
	uint32_t MeshPointers[1 /* NumMeshPointers */ ]; // mesh pointer list (NumMeshPointers * 4 bytes)
	uint32_t NumAnimations; // number of animations to follow (4 bytes)
	tr_animation Animations[NumAnimations]; // animation list (NumAnimations * 32 bytes)
	uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
	tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
	uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
	tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
	uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
	tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
	uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
	tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
	uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
	uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
	uint32_t NumModels; // number of models to follow (4 bytes)
	tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
	uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
	tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
	uint32_t NumObjectTextures; // number of object textures to follow (4 bytes)
	tr_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 20 bytes) (after AnimatedTextures in TR3)
	uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
	tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
	uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
	tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
	uint32_t NumCameras; // number of camera data records to follow (4 bytes)
	tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
	uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
	tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
	uint32_t NumBoxes; // number of box data records to follow (4 bytes)
	tr2_box Boxes[NumBoxes]; // box data (NumBoxes * 8 bytes)
	uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
	uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
	int16_t Zones[10*NumBoxes]; // zone data (NumBoxes * 20 bytes)
	uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
	uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
	uint32_t NumEntities; // number of entities to follow (4 bytes)
	tr2_entity Entities[NumEntities]; // entity list (NumEntities * 24 bytes)
	uint8_t LightMap[32 * 256]; // light map (8192 bytes)
	uint16_t NumCinematicFrames; // number of cinematic frame records to follow (2 bytes)
	tr_cinematic_frame CinematicFrames[NumCinematicFrames]; // (NumCinematicFrames * 16 bytes)
	uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
	uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
	int16_t SoundMap[370]; // sound map (740 bytes)
	uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
	tr_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
	uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)
	uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
#endif
};

struct TR3Level
{
	uint32_t Version; // version (4 bytes)
	tr_colour Palette[256]; // 8-bit palette (768 bytes)
	tr_colour4 Palette16[256]; //  (1024 bytes)
	uint32_t NumTextiles; // number of texture tiles (4 bytes)
	tr_textile8 Textile8[1 /* NumTextiles */]; // 8-bit (palettized) textiles (NumTextiles * 65536 bytes)
	tr_textile16 Textile16[ 1 /* NumTextiles */ ]; // 16-bit (ARGB) textiles (NumTextiles * 131072 bytes) (absent from TR1)
	uint32_t Unused; // 32-bit unused value (4 bytes)
	uint16_t NumRooms; // number of rooms (2 bytes)
	tr3_room Rooms[1 /* NumRooms */ ]; // room list (variable length)
#if 0
	uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
	uint16_t FloorData[NumFloorData]; // floor data (NumFloorData * 2 bytes)
	uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
	tr_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
	uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
	uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
	uint32_t NumAnimations; // number of animations to follow (4 bytes)
	tr_animation Animations[NumAnimations]; // animation list (NumAnimations * 32 bytes)
	uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
	tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
	uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
	tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
	uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
	tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
	uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
	tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
	uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
	uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
	uint32_t NumModels; // number of models to follow (4 bytes)
	tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
	uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
	tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
	uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
	tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
	uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
	tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
	uint32_t NumCameras; // number of camera data records to follow (4 bytes)
	tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
	uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
	tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
	uint32_t NumBoxes; // number of box data records to follow (4 bytes)
	tr2_box Boxes[NumBoxes]; // box data (NumBoxes * 8 bytes)
	uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
	uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
	int16_t Zones[10*NumBoxes]; // zone data (NumBoxes * 20 bytes)
	uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
	uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
	uint32_t NumObjectTextures; // number of object textures to follow (4 bytes) (after AnimatedTextures in TR3)
	tr_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 20 bytes)
	uint32_t NumEntities; // number of entities to follow (4 bytes)
	tr2_entity Entities[NumEntities]; // entity list (NumEntities * 24 bytes)
	uint8_t LightMap[32 * 256]; // light map (8192 bytes)
	uint16_t NumCinematicFrames; // number of cinematic frame records to follow (2 bytes)
	tr_cinematic_frame CinematicFrames[NumCinematicFrames]; // (NumCinematicFrames * 16 bytes)
	uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
	uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
	int16_t SoundMap[370]; // sound map (740 bytes)
	uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
	tr3_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
	uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)  +
	uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
#endif
};

struct TR4Level
{
	uint32_t Version; // version (4 bytes)
	uint16_t NumRoomTextiles; // number of non bumped room tiles (2 bytes)
	uint16_t NumObjTextiles; // number of object tiles (2 bytes)
	uint16_t NumBumpTextiles; // number of bumped room tiles (2 bytes)
	uint32_t Textile32_UncompSize; // uncompressed size (in bytes) of the 32-bit textures chunk (4 bytes)
	uint32_t Textile32_CompSize; // compressed size (in bytes) of the 32-bit textures chunk (4 bytes)
	uint8_t Textile32_Compressed[1/*Textile32_CompSize*/]; // zlib-compressed 32-bit textures chunk (Textile32_CompSize bytes)
	//	{
	//	    tr4_textile32 Textile32[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
	//	}
	uint32_t Textile16_UncompSize; // uncompressed size (in bytes) of the 16-bit textures chunk (4 bytes)
	uint32_t Textile16_CompSize; // compressed size (in bytes) of the 16-bit textures chunk (4 bytes)
	uint8_t Textile16_Compressed[1/*Textile32_CompSize*/]; // zlib-compressed 16-bit textures chunk (Textile16_CompSize bytes)
	//	{
	//	    tr_textile16 Textile16[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
	//	}
	uint32_t Textile32Misc_UncompSize; // uncompressed size (in bytes) of the 32-bit misc textures chunk (4 bytes), should always be 524288
	uint32_t Textile32Misc_CompSize; // compressed size (in bytes) of the 32-bit misc textures chunk (4 bytes)
	uint8_t Textile32Misc_Compressed[1/*Textile32Misc_CompSize*/]; // zlib-compressed 32-bit misc textures chunk (Textile32Misc_CompSize bytes)
	//	{
	//	    tr4_textile32 Textile32Misc[2];
	//	}
	uint32_t LevelData_UncompSize; // uncompressed size (in bytes) of the level data chunk (4 bytes)
	uint32_t LevelData_CompSize; // compressed size (in bytes) of the level data chunk (4 bytes)
	uint8_t LevelData_Compressed[1/*LevelData_CompSize*/]; // zlib-compressed level data chunk (LevelData_CompSize bytes)
	//	{
	//	    uint32_t Unused; // 32-bit unused value, always 0 (4 bytes)
	//	    uint16_t NumRooms; // number of rooms (2 bytes)
	//	    tr4_room Rooms[NumRooms]; // room list (variable length)
#if 0
		uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
		uint16_t FloorData[NumFloorData]; // floor data (NumFloorData * 2 bytes)
		uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
		tr4_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
		uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
		uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
		uint32_t NumAnimations; // number of animations to follow (4 bytes)
		tr4_animation Animations[NumAnimations]; // animation list (NumAnimations * 40 bytes)
		uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
		tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
		uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
		tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
		uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
		tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
		uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
		tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
		uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
		uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
		uint32_t NumModels; // number of models to follow (4 bytes)
		tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
		uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
		tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
		uint8_t SPR[3]; // S P R (0x53, 0x50, 0x52)
		uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
		tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
		uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
		tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
		uint32_t NumCameras; // number of camera data records to follow (4 bytes)
		tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
		uint32_t NumFlybyCameras; // number of flyby camera data records to follow (4 bytes)
		tr4_flyby_camera FlybyCameras[NumFlybyCameras]; // flyby camera data (NumFlybyCameras * 40 bytes)
		uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
		tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
		uint32_t NumBoxes; // number of box data records to follow (4 bytes)
		tr2_box Boxes[NumBoxes]; // box data (NumBoxes * 8 bytes)
		uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
		uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
		int16_t Zones[10*NumBoxes]; // zone data (NumBoxes * 20 bytes)
		uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
		uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
		uint8_t AnimatedTexturesUVCount;
		uint32_t NumObjectTextures; // number of object textures to follow (4 bytes) (after AnimatedTextures in TR3)
		tr4_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 38 bytes)
		uint32_t NumEntities; // number of entities to follow (4 bytes)
		tr4_entity Entities[NumEntities]; // entity list (NumEntities * 24 bytes)
		uint32_t NumAIObjects; // number of AI objects to follow (4 bytes)
		tr4_ai_object AIObjects[NumAIObjects]; // AI objects list (NumAIObjects * 24 bytes)
		uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
		uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
		int16_t SoundMap[370]; // sound map (740 bytes)
		uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
		tr3_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
		uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)  +
		uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
	}
	uint32_t NumSamples; // number of sound samples (4 bytes)
	tr4_sample Samples[NumSamples]; // sound samples (this is the last part, so you can simply read until EOF)
#endif
};

struct TR5Level
{
uint32_t Version; // version (4 bytes)
uint16_t NumRoomTextiles; // number of non bumped room tiles (2 bytes)
uint16_t NumObjTextiles; // number of object tiles (2 bytes)
uint16_t NumBumpTextiles; // number of bumped room tiles (2 bytes)
uint32_t Textile32_UncompSize; // uncompressed size (in bytes) of the 32-bit textures chunk (4 bytes)
uint32_t Textile32_CompSize; // compressed size (in bytes) of the 32-bit textures chunk (4 bytes)
uint8_t Textile32_Compressed[1 /* Textile32_CompSize */ ]; // zlib-compressed 32-bit textures chunk (Textile32_CompSize bytes)
//	{
//	    tr4_textile32 Textile32[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
//	}
uint32_t Textile16_UncompSize; // uncompressed size (in bytes) of the 16-bit textures chunk (4 bytes)
uint32_t Textile16_CompSize; // compressed size (in bytes) of the 16-bit textures chunk (4 bytes)
uint8_t Textile16_Compressed[ 1 /* Textile32_CompSize */ ]; // zlib-compressed 16-bit textures chunk (Textile16_CompSize bytes)
//	{
//	    tr_textile16 Textile16[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
//	}
uint32_t Textile32Misc_UncompSize; // uncompressed size (in bytes) of the 32-bit misc textures chunk (4 bytes), should always be 786432
uint32_t Textile32Misc_CompSize; // compressed size (in bytes) of the 32-bit misc textures chunk (4 bytes)
uint8_t Textile32Misc_Compressed[1 /* Textile32Misc_CompSize */]; // zlib-compressed 32-bit misc textures chunk (Textile32Misc_CompSize bytes)
//	{
//	    tr4_textile32 Textile32Misc[3];
//	}
uint16_t LaraType;
uint16_t WeatherType;
uint8_t Padding[28];
uint32_t LevelData_UncompSize; // uncompressed size (in bytes) of the level data chunk (4 bytes)
uint32_t LevelData_CompSize; // compressed size (in bytes) of the level data chunk, equal to LevelData_UncompSize (4 bytes)
// NOT COMPRESSED
uint32_t Unused; // 32-bit unused value, always 0 (4 bytes)
uint16_t NumRooms; // number of rooms (2 bytes)
tr5_room Rooms[1 /* NumRooms */]; // room list (variable length)
#if 0
uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
uint16_t FloorData[NumFloorData]; // floor data (NumFloorData * 2 bytes)
uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
tr4_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
uint32_t NumAnimations; // number of animations to follow (4 bytes)
tr4_animation Animations[NumAnimations]; // animation list (NumAnimations * 40 bytes)
uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
uint32_t NumModels; // number of models to follow (4 bytes)
tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
uint8_t SPR[4]; // S P R \0 (0x53, 0x50, 0x52, 0x00)
uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
uint32_t NumCameras; // number of camera data records to follow (4 bytes)
tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
uint32_t NumFlybyCameras; // number of flyby camera data records to follow (4 bytes)
tr4_flyby_camera FlybyCameras[NumFlybyCameras]; // flyby camera data (NumFlybyCameras * 40 bytes)
uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
uint32_t NumBoxes; // number of box data records to follow (4 bytes)
tr2_box Boxes[NumBoxes]; // box data (NumBoxes * 8 bytes)
uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
int16_t Zones[10*NumBoxes]; // zone data (NumBoxes * 20 bytes)
uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
uint8_t AnimatedTexturesUVCount;
uint8_t TEX[4]; // T E X \0 (0x54, 0x45, 0x58, 0x00)
uint32_t NumObjectTextures; // number of object textures to follow (4 bytes) (after AnimatedTextures in TR3)
tr4_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 38 bytes)
uint32_t NumEntities; // number of entities to follow (4 bytes)
tr4_entity Entities[NumEntities]; // entity list (NumEntities * 24 bytes)
uint32_t NumAIObjects; // number of AI objects to follow (4 bytes)
tr4_ai_object AIObjects[NumAIObjects]; // AI objects list (NumAIObjects * 24 bytes)
uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
int16_t SoundMap[450]; // sound map (740 bytes)
uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
tr3_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)  +
uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
uint8_t Separator[6]; // 6 0xCD bytes
uint32_t NumSamples; // number of sound samples (4 bytes)
tr4_sample Samples[NumSamples]; // sound samples (this is the last part, so you can simply read until EOF)
#endif
};

#pragma pack(pop, pack1)


