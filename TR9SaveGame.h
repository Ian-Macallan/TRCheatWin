#pragma once

#include <Windows.h>
#include <afx.h>

#include "TRSaveGame.h"
#include "TRXTools.h"

#define TR123LEVELSIZE      1384452

//  for TR1
//  First Address       0x003071
//  Second              0x003991
//  Third               0x00304f

//  TR1 Save level here
#define TR1_LEVEL_SAVE      24

//  TR2 Save level here
#define TR2_LEVEL_SAVE      18

//  TR3 Save Level Here
#define TR3_LEVEL_SAVE      20

//  Secrets
//  0x0DB040 = 1000 - Save (db00c): 39 Level 16 (db628) (#62) 16: Floating Islands
//  0x0DCD8E <- Guns
//  000DB00C: 26 27 <-- Save
//  000DB610: 8A 99
//  000DB611: 62 63
//  000DB620: F0 4E
//  000DB621: 5F 60
//  000DB624: 00 02
//  000DB626: 00 01 <-- Secrets ?
//  000DB63E: 00 01 <-- Secrets ?
//  000DB6BE: 22 2B
//  000DB9A8: 67 09
//  000DB9B0: 45 63
//  000DB9CA: 79 57
//  000DBBCC: 10 16
//  000DCC86: 0F 0B
//  000DCC88: 0F 17
//  000DCC96: 67 09
//  000DCC9E: 45 63
//  000DCE5C: 00 01 <-- Secrets ?



//  Burning : 0x1a instead of 0x0a
//  0x0D7840 = 1000 - Save (d780c): 40 Level 18 (d7e28) (#61) 18: Home Sweet Home
//  000D9128: 0A 1A
//  From Health Found
#define TR1_OFFSET_STATE    (0x0D922C-0x00D9128)    // 0x104 From Gun Found
#define TR2_OFFSET_STATE    (0x0D922C-0x00D9128)    // 0x104 From Gun Found
#define TR3_OFFSET_STATE    (0x0D922C-0x00D9128)    // 0x104 From Gun Found

//
#define NB_OF_SLOTS         31                      // 32 Positions for TR1 and TRUB, for TR2 and TR2G, for TR3 and TR3G

//  This should be the sabegame slots
#define NB_TR_BLOCKS        32

#define NB_TR1_BLOCKS       NB_TR_BLOCKS
#define NB_TR2_BLOCKS       NB_TR_BLOCKS
#define NB_TR3_BLOCKS       NB_TR_BLOCKS

//
#define TR1_BLOCK_START     0x002000
#define TR1_BLOCK_LENGTH    0x003800
#define TR1_SLOT_LENGTH     0x000030
#define TR1_BLOCK_OFFSET    0x000040
#define TR1_FIRST_BLOCK     (TR1_BLOCK_START+TR1_BLOCK_OFFSET)
// From Start of First Block
#define TR1_SAVE_PLUS       (TR1_FIRST_BLOCK - 0x02008)

#define TR2_BLOCK_START     0x072000
#define TR2_BLOCK_LENGTH    0x003800
#define TR2_SLOT_LENGTH     0x000030
#define TR2_BLOCK_OFFSET    0x000040
#define TR2_FIRST_BLOCK     (TR2_BLOCK_START+TR2_BLOCK_OFFSET)
// From Start of First Block
#define TR2_SAVE_PLUS       (TR2_FIRST_BLOCK - 0x72008)

#define TR3_BLOCK_START     0x0E2000
#define TR3_BLOCK_LENGTH    0x003800
#define TR3_SLOT_LENGTH     0x000040
#define TR3_BLOCK_OFFSET    0x0000A4
#define TR3_FIRST_BLOCK     (TR3_BLOCK_START+TR3_BLOCK_OFFSET)
// From Start of First Block
#define TR3_SAVE_PLUS       (TR3_FIRST_BLOCK - 0x0E2008)

//  00002626/2628 seems to be the bitmap (0x016 before count)
//  0000263C seems to be the count

// Secrets Completed from GunAddress found
#define TR1_OFFSET_SEC_GUN  0x000000BC

// Secrets Completed from GunAddress found
#define TR2_OFFSET_SEC_GUN  0x000000c8

// Secrets Completed from GunAddress found
#define TR3_OFFSET_SEC_GUN  0x000000D5

// Secrets Completed from GunAddress found
#define TR3_OFFSET_SEC_SUM  (0x0E514F-0x0E20A4)                     // Sum of secrets 0x30ab


//
//  Meteore 0x128B26
//  RH :    0x128AD0
#define TR3_WILLARD_DEAD    (0x000046)
#define TR2_DRAGON_BOSS1    (0x072F78-0x0727C3) //  From Real Health Minus
#define TR2_DRAGON_BOSS2    (0x072F78-0x07262E) //  From Real Health Minus
#define TR1_BOSS            (0x003637-0x0030FD) //  From Real Health

//  After Keys
//  One byte
//  One Byte : Secrets ?
//  Seven Bytes
//  One Byte : Kills ?
//  

//  TR1 / TR2
///////////////
#define FIRST_X30           TR1_FIRST_BLOCK                         // Start of x30 bytes structures
#define NEXT_X30            0x005840
#define LEN_31_X30          (NEXT_X30-FIRST_X30)                    //  14336

#define SAVE_X30            0x00200C

#define LEVEL1_X30          (0x00262C-FIRST_X30)

//
#define LEVEL2_X30          (0x072628-TR2_FIRST_BLOCK)
#define TR1_SAVE_OFFSET     (0x002E35-FIRST_X30)
#define TR1_SAVE_OFFSET_24  (0x003071-0x0024C0)

//  From Health 0x12A
#define TR1_OFFSET_AIR      (0x00D84F-0x00D725)                     // 0x12A From Health Found
#define TR2_OFFSET_AIR      (0x00D84F-0x00D725)                     // 0x12A From Health Found
#define TR3_OFFSET_AIR      (0x00D84F-0x00D725)                     // 0x12A From Health Found

#define LAST_X30            0xDE840                             //  Last Address of X30 area

//  TR3
///////////////
#define FIRST_X40           (TR3_BLOCK_START+TR3_BLOCK_OFFSET)      //  Start of x40 bytes structures
#define END_X40             0x0E2824                                //  End of one structure
#define LEN_31_X40          (END_X40-FIRST_X40)                     //  1920
#define NEXT_X40            0x0E58A4                                //  Next Structure for X40
#define LEN_X40             0x000040
#define SAVE_X40            0x000098
#define LEVEL_X40           0x0E28D6

#define TR_OFFSET_INDICATOR 0x000004

#define TR1_GAME_PLUS       0x00018C
#define TR2_GAME_PLUS       0x000190
#define TR3_GAME_PLUS       0x000194

#define TR_SOUND            0x10
#define TR_MUSIC            0x12

//
#define TR_WITDH            0x20
#define TR_HEIGHT           0x24

#define TR_KB_V             0x2A
#define TR_KB_PAD0          0x2F

#define TR_KB_UP            0x40
#define TR_KB_LOOK          0xA0
#define TR_KB_FLARER        0xB8
#define TR_KB_DUCK          0xC4
#define TR_KB_DASH          0xD0

//  From Start Of Block
#define TR1_INTEREST_1      (0x00002040-0x00002004)
#define TR2_INTEREST_1      0x000030                    // From Current Stats
#define TR3_INTEREST_1      (0x000E3FAF-0x000E3FA7)     // From Completed

//  From Real Health
#define TR3_INTEREST_2      (0x00E2B26-0x000E2AC1)

#define RIOT_CLIP           6
#define UZI_CLIP            20
#define M16_CLIP            30
#define GRENADE_CLIP        2

//  From Health 0x2825 And Guns 0x3071
//  0x2801/0x2802
//  Decrease when going to West
//  Caves End : 6B A7 00 00 / 00 1C 00 00 / E6 39 01 00 00 00
#define TR1_POS_WEST_EAST       0x00002801  //  0x2802  DWORD
//  Standing : 0xC00 (3072) - Jump 0x8AC (2220) : Seems to be inverted
#define TR1_POS_ALTITUDE        0x00002805  //  DWORD Less value even negative is higher
//  (34715 - 3584 = 31131 = 70m) (444 for 1 m)
//  Increase when going to north
#define TR1_POS_SOUTH_NORTH     0x00002809  //  0x280A  DWORD

//
#define TR1_OFFSET_POS      (0x00002825-TR1_POS_WEST_EAST)
#define TR2_OFFSET_POS      (0x00002825-TR1_POS_WEST_EAST)
#define TR3_OFFSET_POS      (0x00002825-TR1_POS_WEST_EAST)

#pragma pack(push, pack1, 1)

//  Divider to Get approximative Meters
#define POSITION_DIVIDER    1000L

struct TR9_POSITION
{
    DWORD   dwWestToEast;
    DWORD   dwVertical;
    DWORD   dwSouthToNorth;
    //  0 : ?
    //  1 : Orientation 0x0000 = facing white 0xffff facing red (North)
    WORD    wSomething0;
    WORD    wOrientation;
    WORD    wSomething2;
    WORD    wSomething3;
    WORD    wRoom;
    WORD    wSomething5;
};

//
struct TR_HEALTH_RANGE
{
    DWORD minOffset;
    DWORD maxOffset;
};

struct TABLE_TR1
{
    //  0x00
    WORD        guns;
    WORD        magnum;
    WORD        uzi;
    WORD        riotgun;    // Divide by 6

    //  0x08
    BYTE        smallMedipak;
    BYTE        largeMedipak;
    BYTE        scion;
    BYTE        flag1;
    BYTE        flag2;
    BYTE        flag3;
    WORD        m16none;

    //  0x10
    WORD        grenadenone;
    WORD        harpoonnone;
    WORD        unk14 [ 2 ];

    //  0x18
    WORD        ammosUsed;      //  On Slot 0
    WORD        unk19;
    WORD        ammoshits;      //  On Slot 0
    WORD        unk1b;

    //  0x20
    WORD        kills;
    WORD        unk22 [ 3 ];

    //  0x28
    WORD        unk28;
    BYTE        pickups;        //  On Slot 0
    BYTE        mediused;       //  * 2 On Slot 0
    BYTE        gunmask;
    BYTE        unk2d;
    WORD        unk2e;
};

//  Just after the last block
#define LEN_TR1_SECRETS     19
struct TABLE_TR1_STATS
{
    DWORD       unk0;
    DWORD       elapsed;        //  30ms units
    WORD        ammosUsed;
    WORD        unkc;

    WORD        ammosHits;
    WORD        unk10;
    WORD        kills;
    WORD        unk0e;

    //
    DWORD       distance;       // quater of a centimeter
    BYTE        secrets;        //  Acquired
    BYTE        unk13;
    BYTE        pickups;        //  0x2a
    BYTE        mediused;       //  0x2b * 2
    BYTE        level;          //  0x2c
    WORD        unk14;
    BYTE        maps[2];
    BYTE        objects[4];
    BYTE        keys[4];
    BYTE        unk30 [ 3 ];
    BYTE        tblsecrets [ LEN_TR1_SECRETS ];
};

struct TABLE_TR2
{
    //  0x00
    WORD        guns;
    WORD        magnum;
    WORD        uzi;
    WORD        riotgun;        // Divide by 6

    //  0x08
    WORD        m16;
    WORD        grenade;
    WORD        harpoon;
    BYTE        smallMedipak;
    BYTE        largeMedipak;

    //  0x10
    BYTE        flag1;
    BYTE        flare;
    BYTE        flag2;
    BYTE        flag3;
    WORD        unk14 [ 2 ];

    //  0x18
    WORD        ammosUsed;      //  No There See Structure Below
    WORD        unk19;
    WORD        ammoshits;      //  No There See Structure Below
    WORD        unk1b;

    //  0x20
    WORD        kills;
    WORD        unk22 [ 3 ];

    //  0x28
    WORD        unk28;
    BYTE        pickups;
    BYTE        mediused;
    BYTE        gunmask;
    BYTE        unk2d;
    WORD        unk2e;
};

//  Just after the last block
#define LEN_TR2_SECRETS     24
struct TABLE_TR2_STATS
{
    DWORD       elapsed;        // 30 ms units
    WORD        ammosUsed;
    WORD        unk2;

    WORD        ammosHits;
    WORD        unk4;
    WORD        kills;
    WORD        unk0e;

    //
    DWORD       distance;       // quarter of a centimeter
    BYTE        secrets;        // Acquired
    BYTE        unk12;
    BYTE        pickups;
    BYTE        mediused;       // * 2
    BYTE        level;
    WORD        unk14;
    BYTE        maps[2];
    BYTE        objects[4];
    BYTE        keys[4];
    BYTE        unk30 [ 3 ];
    BYTE        tblsecrets [ LEN_TR2_SECRETS ];
};

struct TABLE_TR3
{
    //  0x00
    WORD        guns;
    WORD        magnum;
    WORD        uzi;
    WORD        riotgun;    // Divide by 6

    //  0x08
    WORD        m16;
    WORD        rocket;
    WORD        harpoon;
    WORD        grenade;

    //  0x10
    BYTE        smallMedipak;
    BYTE        largeMedipak;
    BYTE        unk0;
    BYTE        flare;
    BYTE        colCrystals;
    BYTE        treasure [ 4 ];

    //  0x18
    BYTE        unk18;
    BYTE        flag0;
    BYTE        flag1;
    WORD        unk1c [ 2 ];

    //  0x20
    WORD        unk20 [ 4 ];

    //  0x28
    WORD        unk28 [ 4 ];

    //  0x30
    WORD        kills;
    WORD        unk32 [ 1 ];

    //  0x38
    WORD        unk38;
    WORD        unk3a [ 2 ];
    BYTE        pickups;
    BYTE        mediused;
    WORD        gunmask;
    WORD        unk3e;
};

//  After the last block
#define LEN_TR3_SECRETS     0x81
struct TABLE_TR3_STATS
{
    WORD        unk000 [ 32 ];
    BYTE        crystals;
    BYTE        unk1;
    WORD        unk2 [ 3 ];

    //
    DWORD       elapsed;        // 30 ms
    WORD        ammosUsed;
    WORD        unk8;

    //
    WORD        ammosHits;
    WORD        unk10;
    WORD        kills;
    WORD        unk0e;

    //
    DWORD       distance;       // quarter of a centimeter
    BYTE        secrets;        //  Acquired
    BYTE        unk18;
    BYTE        pickups;
    BYTE        mediused;       // * 2
    BYTE        unk14 [ 18 ];
    BYTE        level;
    WORD        unk19;
    BYTE        maps[2];
    BYTE        objects[4];
    BYTE        keys[4];
    BYTE        unk30 [ 1 ];
    BYTE        tblsecrets [ LEN_TR3_SECRETS ];
};

//  Here we will find the values from the table
struct GUN_TR1
{
    WORD        guns;
    WORD        unk1 [ 3 ];
    WORD        magnum;
    WORD        unk2 [ 3 ];
    WORD        uzi;
    WORD        unk3 [ 3 ];
    WORD        riotgun;
};

struct GUN_TR2
{
    WORD        guns;
    WORD        unk1 [ 3 ];
    WORD        magnum;
    WORD        unk2 [ 3 ];
    WORD        uzi;
    WORD        unk3 [ 3 ];
    WORD        riotgun;
    WORD        unk4 [ 3 ];
    WORD        harpoon;
    WORD        unk5 [ 3 ];
    WORD        grenade;
    WORD        unk6 [ 3 ];
    WORD        unka;
    WORD        unk7 [ 3 ];
    WORD        m16;
    WORD        unk8 [ 3 ];
};

struct GUN_TR3
{
    WORD        guns;
    WORD        unk1 [ 3 ];
    WORD        magnum;
    WORD        unk2 [ 3 ];
    WORD        uzi;
    WORD        unk3 [ 3 ];
    WORD        riotgun;
    WORD        unk4 [ 3 ];
    WORD        harpoon;
    WORD        unk7 [ 3 ];
    WORD        rocket;
    WORD        unk5 [ 3 ];
    WORD        grenade;
    WORD        unk6 [ 3 ];
    WORD        m16;
    WORD        unk8 [ 3 ];
};

struct  TR_START_CONDITIONS
{
    bool        bPistols;
    bool        bMagnum;
    bool        bUzi;
    bool        bRiotgun;
    bool        bHarpoon;
    bool        bRocket;
    bool        bGrenade;
    bool        bM16;

    WORD        iMagnum;
    WORD        iUzi;
    WORD        iRiotgun;
    WORD        iHarpoon;
    WORD        iRocket;
    WORD        iGrenade;
    WORD        iM16;

    BYTE        iSmall;
    BYTE        iLarge;
    BYTE        iFlare;
    WORD        iHealth;
    WORD        iAir;

};

#pragma pack(pop, pack1)

//  Indicator Structure for TR 4 and 5
#pragma pack(push, pack1, 1)
typedef struct indicatorTRRStruct
{
    BOOL    bEnd;
    BYTE    b1;
    BYTE    b2;
    BYTE    b3;
    int     step;
    char    szLabel [ 32 ];
} TRR_INDICATORS;
#pragma pack(pop, pack1)


//
/////////////////////////////////////////////////////////////////////////////
class CTR9SaveGame : public CObject
{
    DECLARE_DYNAMIC(CTR9SaveGame)

    protected :
        static CTR9SaveGame *m_pInstance;
    public :
        static CTR9SaveGame *GetInstance(BOOL bCreate = TRUE);
        static CTR9SaveGame *I(BOOL bCreate = TRUE);
        static BOOL IsBufferModified();
        static BOOL WriteFile(const char *pFilename);
        static void Discard();

    protected:
        CTR9SaveGame(void);

    public:
        virtual ~CTR9SaveGame(void);

        void Init( bool bFromContructor = false );

        //  Each block contains 31 structures for save position

    protected :
        static char *m_TR1_Start [ NB_TR1_BLOCKS ];
        static char *m_TR2_Start [ NB_TR2_BLOCKS ];
        static char *m_TR3_Start [ NB_TR3_BLOCKS ];

        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR1_Blocks [ NB_TR1_BLOCKS ] [ NB_OF_SLOTS ];
        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR2_Blocks [ NB_TR2_BLOCKS ] [ NB_OF_SLOTS ];
        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR3_Blocks [ NB_TR3_BLOCKS ] [ NB_OF_SLOTS ];

        //  Guns is the reference for gun and ammos
        static GUN_TR1 *m_TR1_Guns [ NB_TR1_BLOCKS ] [ NB_OF_SLOTS ];
        //  Guns is the reference for gun and ammos
        static GUN_TR2 *m_TR2_Guns [ NB_TR2_BLOCKS ] [ NB_OF_SLOTS ];
        //  Guns is the reference for gun and ammos
        static GUN_TR3 *m_TR3_Guns [ NB_TR3_BLOCKS ] [ NB_OF_SLOTS ];

        FILE    *m_hFile;
        char    m_szFilename [ MAX_PATH ];
        char    m_szIndicatorLabel [ 32 ];

    protected :
        static  char    *m_pBuffer;
        static  char    *m_pBufferBackup;
        static  int     m_iSaveLength;

    public :
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure (   char *pFrom, char *pUntil, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure ( char *pFrom, char *pUntil, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        WORD m16, WORD grenade, WORD harpoon,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure ( char *pFrom, char *pUntil, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        WORD m16, WORD grenade, WORD harpoon, WORD rocket,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );

        //
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure (   int tombraider, int block, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure ( int tombraider, int block, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        WORD m16, WORD grenade, WORD harpoon,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );
        //  Returns a pointer to a GUN_TR1 GUN_TR2 GUN_TR3 structure
        static char *SearchGunStructure ( int tombraider, int block, WORD guns,
                        WORD magnum, WORD uzi, WORD riot,
                        WORD m16, WORD grenade, WORD harpoon,  WORD rocket,
                        int &countResult, int &valueResult,
                        WORD mask,
                        bool allowZero, bool noGuns );
        //
        static DWORD RelativeAddress ( const void *pAddress );

        //
        BOOL ReadFile ( const char *pFilename );
        BOOL Load (  );
        BOOL writeFile ( const char *pFilename );

        //
        char *getBufferAddress();
        char *getBufferBackupAddress();
        size_t getBufferLength();

        //
        int GetSaveNumber ( int tombraider, int block );
        void SetSaveNumber ( int tombraider, int block, WORD saveNo );

        //
        WORD *GetSaveAddress ( int tombraider, int block );

        //
        bool isKnown(const char *position);
        const char *getPositionLabel(const char *position);
        const char *getIndicatorLabel()
        {
            return m_szIndicatorLabel;
        };

        WORD *GetRealHealthAddress ( int tombraider, int block );

        //
        TR9_POSITION *GetPositionAddress ( int tombraider, int block );

        WORD GetRealHealth ( int tombraider, int block );
        void SetRealHealth ( int tombraider, int block, WORD value );

        //  return the TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer
        void *SearchBlockEntry ( int tombraider, int block, int *piSlot );
        //  return the first TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer that has a mask & 1
        void *GetDefaultBlockEntry ( int tombraider, int block, int *piSlot );

        //  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
        void *SearchGunEntry ( int tombraider, int block );
        void *GetDefaultGunEntry ( int tombraider, int block );

        //
        BYTE GetBlockIndicator ( int tombraider, int block );
        const char *GetBlockHexa ( int tombraider, int block, int slot, int iStart = 0, int iEnd = -1 );

        WORD *GetAirAddress ( int tombraider, int block );
        WORD GetAir ( int tombraider, int block );
        void SetAir ( int tombraider, int block, WORD iAir );

        BYTE GetState ( int tombraider, int block );
        void SetState ( int tombraider, int block, BYTE iState );

        int countTag ( int tombraider, int block );

        //  Get Level Number from 1 to 25...
        BYTE GetBlockLevelNumber ( int tombraider, int block );
        const char *GetBlockElapsed ( int tombraider, int block );
        const char *GetBlockDistance ( int tombraider, int block );

        //  Only In Block : Object / Items / Keys / Map / Treasures
        BYTE *GetObjectBaseAddress ( int tombraider, int block );

        //  Object / Items / Keys / Map / Treasures
        int GetBlockObject ( int tombraider, int block, int object );
        int GetKey ( int tombraider, int block, int key );
        int GetMap ( int tombraider, int block, int map );
        int GetTreasure ( int tombraider, int block, int slot, int treasure );

        //  Object / Items / Keys / Map / Treasures
        void SetBlockObject ( int tombraider, int block, int object, int val );
        void SetKey ( int tombraider, int block, int key, int val );
        void SetMap ( int tombraider, int block, int map, int val );
        void SetTreasure ( int tombraider, int block, int slot, int treasure, int val );

        //  Addresses In Table
        ////////////////////////////////////////////////////
        BYTE *GetSlotMedipakUsedAddress(int tombraider, int block, int slot);
        BYTE *GetSlotPickupAddress(int tombraider, int block, int slot);
        WORD *GetSlotKillsAddress(int tombraider, int block, int slot);
        BYTE *GetSlotCrystalAddress ( int tombraider, int block, int slot );
        BYTE *GetSlotTreasureAddress ( int tombraider, int block, int slot );

        //  Addresses In Blocks
        ////////////////////////////////////////////////////
        DWORD *GetBlockElapsedAddress ( int tombraider, int block );
        DWORD *GetBlockDistanceAddress ( int tombraider, int block );

        BYTE *GetBlockCrystalAddress ( int tombraider, int block );
        //  Get Medipak address Used
        BYTE *GetBlockMedipakUsedAddress(int tombraider, int block);
        //  Pickup Address  follow by Meidpak used
        BYTE *GetBlockPickupAddress(int tombraider, int block);
        //  Get Kill Address
        WORD *GetBlockKillsAddress (int tombraider, int block);

        //
        //  Ammos Used Follow by Hit and Kill
        WORD *GetBlockAmmosUsedAddress(int tombraider, int block);
        WORD *GetBlockAmmosHitsAddress(int tombraider, int block);
        BYTE *GetBlockSecretsAddress ( int tombraider, int block );

        BYTE *GetBlockLevelAddress ( int tombraider, int block );
        BYTE *GetBlockSecretsBaseAddress ( int tombraider, int block );

        BYTE *GetBlockMapAddress ( int tombraider, int block );
        BYTE *GetBlockObjectAddress ( int tombraider, int block );
        BYTE *GetBlockKeyAddress ( int tombraider, int block );

        //
        WORD GetBlockKills ( int tombraider, int block );
        BYTE GetBlockPickup ( int tombraider, int block );
        BYTE GetBlockCrystal(int tombraider, int block);


        //  Get First Address of Stats
        // BYTE *GetBlocksSecretsBaseAddress(int tombraider, int block);

        void SetBlockPickup(int tombraider, int block, BYTE pickup);
        void SetBlockCrystal(int tombraider, int block, BYTE crystals);
        void SetBlockKills(int tombraider, int block, WORD kills);

        //  Get Maximum for current block
        BYTE GetSecretsMaximum ( int tombraider, int block );

        //
        void GetBlocksSecrets ( int tombraider, int block, BYTE vector[32] );

        //  Compare Secrets Completed with stats
        int CompareSecretsCompleted ( int tombraider, int block, BYTE *pStartCompleted );
        //  Return First Address of Secrets Completed
        BYTE *SearchSecretsCompleted ( int tombraider, int block );

        //BYTE *GetBlocksSecretsBaseAddress(int tombraider, int block);
        //  Cnt Addr is relative for TR3 like stats and fixed otherwise
        BYTE *GetBlockSecretsCurrentAddress(int tombraider, int block);

        //  Get Secrets Acquired
        BYTE GetBlockSecretsAcquiredBits(int tombraider, int block);
        BYTE GetBlockSecretsAcquired(int tombraider, int block);

        //  Get The number of secrets possible
        BYTE GetBlockSecretsCurrentValue(int tombraider, int block);
        BYTE GetBlockSecretsCurrent(int tombraider, int block);

        //
        BYTE *GetSecretsCompletedBaseAddress(int tombraider, int block );
        //  Enable address is relative to Gun for TR3 other wise it is fixed
        BYTE *GetSecretsCompletedAddress(int tombraider, int block, int iLevel = -1 );

        //  Get Secrets Enabled
        BYTE GetSecretsCompletedBits(int tombraider, int block, int iLevel = -1 );
        BYTE GetSecretsCompletedValue(int tombraider, int block, int iLevel = -1 );
        BYTE GetSecretsCompleted(int tombraider, int block, int iLevel = -1 );

        //
        void SetBlockSecretsAcquiredAll(int tombraider, int block, int excludeLevel = -1 );

        //  Set Secret acquired
        void SetBlockSecretsAcquired(int tombraider, int block, BYTE secrets = 0xff );

        DWORD GetTRPlus ( int tombraider );
        void SetTRPlus ( int tombraider, DWORD value );

        BYTE IsGamePlus ( int tombraider, int block );
        void SetGamePlus ( int tombraider, int block, BYTE value );

        //
        void KillWillard ( int tombraider, int block );
        void KillTR1Boss ( int tombraider, int block );
        BOOL IsKillEnabled ( int tombraider, int block );

        const char *GetInterest ( int tombraider, int block, int slot );

        void ReNumber ( );
        BOOL Delete ( int tombraider, int block );
        BOOL Export ( const char *pFilename, int tombraider, int block );
        BOOL Import( const char *pFilename, int tombraider, int block );

        BOOL BufferModified ();
        void discard();

        //  Set At STart Condition
        void SetLikePlus ( int tombraider, int block, int slot );
        void SetLikeGame ( int tombraider, int block, int slot );

        //
        void SetPistols ( int tombraider, void *pBlocks, int level, BOOL bEnabled );
        void SetMagnum ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetUzi ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetRiotgun ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetM16 ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetHarpoon ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetGrenade ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetRocket ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );

        //
        void SetPistols ( int tombraider, int block, int slot, BOOL bEnabled );
        void SetMagnum ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetUzi ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetRiotgun ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetM16 ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetHarpoon ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetGrenade ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetRocket ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );

        //
        void TraceBlock ( int tombraider, int block, int slot );
        void TraceGun ( int tombraider, int block, int slot );

        //
        static int GetBlockLength ( int tombraider );
        static int GetSlotLength ( int tombraider );
        static void *GetBlockStart ( int tombraider, int block, bool bForce = false );
        static void *GetSlotsEnd ( int tombraider, int block, bool bForce = false );
        //  Return directly m_TRx_Blocks [ b ] [ s ]
        static void *GetBlockSlot ( int tombraider, int block, int slot );

        //  return  m_TRX_Guns [ block ] [ slot ]
        static void *GetBlockGuns ( int tombraider, int block, int slot );
        //  Return m_TRx_Blocks [ b ] [ s ] but iif null can return the theorical address
        static void *GetBlockAddress ( int tombraider, int block, int slot, bool bForce = false );
        //  return  m_TRX_Guns [ block ] [ slot ]
        static void *GetGunAddress ( int tombraider, int block, int slot );

        //  Get Maximum Secrets for level Index from 0
        static BYTE GetSecretsForLevel ( int tombraider, int levelIndex );
        static WORD GetKillsForLevel ( int tombraider, int levelIndex );
        static WORD GetPickupsForLevel ( int tombraider, int levelIndex );
        static WORD GetCrystalsForLevel ( int tombraider, int levelIndex );

        //  From 1 to 32
        static const char *GetLevelName ( int tombraider, int level );
        static int GetLevelCount ( int tombraider );

        static const char *GetIndicatorFileType()
        {
            return ".trr.txt";
        }
        static int ReadIndicators( TRR_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static BOOL WriteIndicators( TRR_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static int CountIndicators( TRR_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MinIndicators( TRR_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MaxIndicators( TRR_INDICATORS *IndicatorsTRTable, const int maxTable );


};

//
extern  BOOL TraceMode;

extern TRR_INDICATORS IndicatorsTRRTable [ MAX_INDICATORS ];
extern int IndicatorsTRRTableCount;
