#pragma once

#include <Windows.h>
#include <afx.h>
#include "TRXTools.h"
#include "TRSaveGame.h"

//  Full Lenbgth : 0x3DCA04
#define TR456LEVELSIZE      4049412
#define NB_SLOT_456         32

//
#define TR456_SLOT_LENGTH   0xA470


#define TR4_BLOCK_START     0x2000
#define TR4_BLOCK_LENGTH    TR456_SLOT_LENGTH
#define TR4_SLOT_LENGTH     TR4_BLOCK_LENGTH

#define TR5_BLOCK_START     (TR4_BLOCK_START+NB_SLOT_456*TR4_SLOT_LENGTH)
#define TR5_BLOCK_LENGTH    TR456_SLOT_LENGTH
#define TR5_SLOT_LENGTH     TR5_BLOCK_LENGTH

#define TR6_BLOCK_START     (TR5_BLOCK_START+NB_SLOT_456*TR5_SLOT_LENGTH)
#define TR6_BLOCK_LENGTH    TR456_SLOT_LENGTH
#define TR6_SLOT_LENGTH     TR6_BLOCK_LENGTH

//
//  00000001
#define TRR4_GUN_SET1       0x01
//  00000100
#define TRR4_GUN_SET4       0x04
//  00001000
#define TRR4_GUN_SET8       0x08
//  00001001
#define TRR4_GUN_SET        (TRR4_GUN_SET1|TRR4_GUN_SET8)
//  00001101
#define TRR4_GUN_MASK       (TRR4_GUN_SET1|TRR4_GUN_SET4|TRR4_GUN_SET8)

//    iMaskGun            = TRR4_GUN_SET1 | TRR4_GUN_SET8;        //  Pistol
//    iMaskUzi            = TRR4_GUN_SET1;
//    iMaskRiotGun        = TRR4_GUN_SET1 | TRR4_GUN_SET8;
//    iMaskCrossBow       = TRR4_GUN_SET1 | TRR4_GUN_SET8;        // Crossbow
//    iMaskGrenade        = TRR4_GUN_SET1 | TRR4_GUN_SET8;
//    iMaskRevolver       = TRR4_GUN_SET1;                        // Revolver
//    iMaskLaser          = TRR4_GUN_SET1;
//    iMaskBinocular      = TRR4_GUN_SET1;
//    iMaskCrowBar        = TRR4_GUN_SET1;

#define TRR4_MASK_PISTOL    TRR4_GUN_SET
#define TRR4_MASK_UZI       TRR4_GUN_SET1
#define TRR4_MASK_RIOTGUN   TRR4_GUN_SET
#define TRR4_MASK_CROSSBOW  TRR4_GUN_SET
#define TRR4_MASK_GRENADE   TRR4_GUN_SET
#define TRR4_MASK_MAGNUM    TRR4_GUN_SET
#define TRR4_MASK_LASER     TRR4_GUN_SET1
#define TRR4_MASK_BINOCULAR TRR4_GUN_SET1
#define TRR4_MASK_CROWBAR   TRR4_GUN_SET1


//  00000001
#define TRR5_GUN_SET1       0x01    
//  00000100
#define TRR5_GUN_SET4       0x04
//  00001000
#define TRR5_GUN_SET8       0x08
//  00001100
#define TRR5_GUN_SETC       0x0C
//  00001001
#define TRR5_GUN_SET        (TRR5_GUN_SET1|TRR5_GUN_SET8)
//  00001101
#define TRR5_GUN_MASK       (TRR5_GUN_SET1|TRR5_GUN_SET4|TRR5_GUN_SET8|TRR5_GUN_SETC)

//    iMaskPistol         = TRR5_GUN_SET1 | TRR5_GUN_SET8;        //  Pistol
//    iMaskUzi            = TRR5_GUN_SET1 | TRR5_GUN_SET8;
//    iMaskRiotGun        = TRR5_GUN_SET1 | TRR5_GUN_SET8;
//    iMaskHK             = TRR5_GUN_SET1 | TRR5_GUN_SET8;
//    iMaskDesertEagle    = TRR5_GUN_SET1 | TRR5_GUN_SETC;        // Revolver
//    iMaskLaser          = TRR5_GUN_SET1;
//    iMaskBinocular      = TRR5_GUN_SET1;
//    iMaskHeadSet        = TRR5_GUN_SET1;
//    iMaskRevolver       = TRR5_GUN_SET1 | TRR5_GUN_SETC;

#define TRR5_MASK_PISTOL    (TRR5_GUN_SET1|TRR5_GUN_SET8)
#define TRR5_MASK_UZI       (TRR5_GUN_SET1|TRR5_GUN_SET8)
#define TRR5_MASK_RIOTGUN   (TRR5_GUN_SET1|TRR5_GUN_SET8)
#define TRR5_MASK_HK        (TRR5_GUN_SET1|TRR5_GUN_SET8)
#define TRR5_MASK_GRAP      (TRR5_GUN_SET1|TRR5_GUN_SETC)
#define TRR5_MASK_LASER     TRR5_GUN_SET1
#define TRR5_MASK_BINOCULAR TRR5_GUN_SET1
#define TRR5_MASK_CROWBAR   TRR5_GUN_SET1
#define TRR5_MASK_REVOLVER  (TRR5_GUN_SET1|TRR5_GUN_SETC)


#define TRR4_STATE_OFFSET   (0x226C-0x2194)

#pragma pack(push, pack1, 1)

//  0000C605: 01 00
//  0000C607: 09 00
//  0000C609: 09 00
//  0000C60A: 01 00
struct GUN_TR4
{
    // 0x00c604 : 09
    BYTE        m_gunPistol;

    // 0x00c605 : 01
    BYTE        m_gunUzis;

    // 0x00c605 : 09
    BYTE        m_gunRiotGun;

    // 0x00c606 : 09
    BYTE        m_gunCrossBow;

    // 0x00c607 : 09
    BYTE        m_gunGrenadesLauncher;

    // 0x00c60A : 01
    BYTE        m_gunBinocular;

    // 0x00c608 : 09
    BYTE        m_gunRevolver;

    // 0x00c609 : 01
    BYTE        m_gunLaserLight;

    // 0x00c60B : 01
    BYTE        m_gunCrowBar;

    //
    BYTE        m_Unknown01;
    BYTE        m_Unknown02;
    BYTE        m_Unknown03;

    //  0x00c60C
    union
    {
        BYTE        m_Object [ 29 ];
        struct Detailed
        {
            //  Address : 1 - 3 : 3 Object
            BYTE    m_Objects [ 3 ];

            //  Address : 4 - 6 : 3 Examine
            BYTE    m_Examine [ 3 ];

            //  Address : 7 - 18 : 12 Puzzle
            BYTE    m_Puzzle [ 12 ];

            //  19 - 20 : 16  (8 Puzzle Combo * 2)
            BYTE    m_PuzzleCombo [ 2 ];    //  Four by byte

            //  Address : 21 - 22   : 12 Keys
            //  Byte 1 : Keys 1 to 8
            //  Byte 2 : Keys 9 to 12
            BYTE    m_Key [ 2 ];            //  Six by byte

            //  Address : 23 - 24 : 16 (8 Key Combo * 2)
            //  8 Key Combo3
            BYTE    m_KeyCombo [ 2 ];       //  Three by byte

            //  Address : 25 - 26 : 4 Pickup
            //  Seem to be 4 by byte only first byte used
            BYTE    m_Pickup [ 2 ];         //  Four by byte

            //  Address : 27 - 28  : 8 (4 Pickup Combo * 2)
            BYTE    m_PickupCombo [ 2 ];    //  Four by byte

            //  Address : 29
            //  Amulet
            BYTE    m_Amulet;
        };
    };
};

//  Second Slot at 0x00C470
//  Small Medipack at 0x00C62E
//  Level 0xC66C
struct TABLE_TR4
{
    //
    union
    {
        struct
        {
            //  0x00C470
            DWORD       unknown01 [ 2 ];

            //
            WORD        savenumber;
        };

        struct
        {
            //
            BYTE        unknown02 [ 0xc62e - 0xc470 ];

            //  0x00C62E
            WORD        m_iSmallMedipak;

            //  0x00C630
            WORD        m_iLargeMedipak;

            //  0x00C632
            WORD        m_iFlares;

            //  0x00C634
            //  Normally 0xFFFF
            WORD        m_iGunAmmos;

            //  0x00C636
            WORD        m_iUziAmmos;

            //  0x00C638
            WORD        m_iRevolverAmmos;

            //  0x00C63A
            WORD        m_iShotGunAmmo1;

            //  0x00C63C
            WORD        m_iShotGunAmmo2;

            //  0x00C63E
            WORD        m_iWhatAmmos;

            //  0x00C640
            WORD        m_iGrenade1;

            //  0x00C642
            WORD        m_iGrenade2;

            //  0x00C644
            WORD        m_iGrenade3;

            //  0x00C646
            WORD        m_iCrossbow1;

            //  0x00C648
            WORD        m_iCrossbow2;

            //  0x00C64A
            WORD        m_iCrossbow3;

        };

        struct
        {
            BYTE        unknown03 [  0x00C66C - 0x00c470 ];

            //      0x00C66C
#define     m_cLevelsLen        10
            BYTE        m_cLevels [ m_cLevelsLen ];
        };

        struct
        {
            BYTE        unknown04 [  0x00c604 - 0x00c470 ];
            GUN_TR4     m_GunTR4;
        };

        struct
        {
            BYTE        unknown05 [  0x00C4BA - 0x00c470 ];
            WORD        m_iAir;
        };

        struct
        {
            BYTE        unknown06 [  0x00C4E8 - 0x00c470 ];
            BYTE        m_iLara;
        };
    };

};

//
struct GUN_TR5
{
    //  0x16F : 09
    BYTE        m_gunPistol;        //  1

    //  0x170 : 09
    BYTE        m_gunUzis;          //  2

    //  0x171 : 09      // 0x661 0x20 to 0x00 make uzi appears at feet
    BYTE        m_gunRiotGun;       //  3

    //  0x172 : 09      Desert Eagle or Grappling
    BYTE        m_gunGrappling;       //  4

    //  0x173 : 09
    BYTE        m_gunHK;            //  5

    //  A Delta
    BYTE        m_Unknown01;

    //  0x174 : 01
    BYTE        m_gunRevolver;      //  6

    //  0x175 : 01
    BYTE        m_gunLaserLight;    //  7

    //  0x176 : 01
    BYTE        m_gunBinocular;     //  8

    //  0x177 : 01
    BYTE        m_gunHeadSet;       //  9

    BYTE        m_Unknown02;

    BYTE        m_Unknown03;

    //  0x178
    union
    {
        BYTE        m_Object [ 28 ];
        struct Detailed
        {
            //  Address : 1 - 3 : 3 Object
            BYTE    m_Objects [ 3 ];

            //  Address : 4 - 6 : 3 Examine
            BYTE    m_Examine [ 3 ];

            //  Address : 7 - 18 : 12 Puzzle
            BYTE    m_Puzzle [ 12 ];

            //  19 - 20 : 16  (8 Puzzle Combo * 2)
            BYTE    m_PuzzleCombo [ 2 ];    //  Four by byte

            //  Address : 21 - 22   : 12 Keys
            //  Byte 1 : Keys 1 to 8
            //  Byte 2 : Keys 9 to 12
            BYTE    m_Key [ 2 ];            //  Six by byte

            //  Address : 23 - 24 : 16 (8 Key Combo * 2)
            //  8 Key Combo3
            BYTE    m_KeyCombo [ 2 ];       //  Three by byte

            //  Address : 25 - 26 : 4 Pickup
            BYTE    m_Pickup [ 2 ];         //  Four by byte

            //  Address : 27 - 28  : 8 (4 Pickup Combo * 2)
            BYTE    m_PickupCombo [ 2 ];    //  Four by byte
        };
    };

};

//  14AE00
struct TABLE_TR5
{
    //
    union
    {
        struct
        {
            //  0x00C470
            DWORD       unknown01 [ 2 ];

            //
            WORD        savenumber;
        };

        struct
        {
            //
            BYTE        unknown02 [ 0x14AFBE - 0x14AE00 ];

            //  0x194
            WORD         m_iSmallMedipak;

            //  0x196
            WORD        m_iLargeMedipak;

            //  0x198 / 0x199 : 01
            WORD        m_iFlares;

            //  0x19A
            WORD        m_iGunAmmos;

            //  0x19C
            WORD        m_iUziAmmos;

            //  0x19E
            WORD        m_iRevolverAmmos;

            //  0x1A0
            WORD        m_iShotGunAmmo1;

            //  0x1A2
            WORD        m_iShotGunAmmo2;

            //  0x1A4
            WORD        m_iHKAmmos;

            //  0x1A6
            WORD        m_iGrapplingAmmos;
        };

        struct
        {
            BYTE        unknown03 [  0x14B06F - 0x14AE00 ];

            //      0x00C66C
            BYTE        m_cLevel;
            BYTE        m_cSubLevel;
        };

        struct
        {
            BYTE        unknown04 [  0x14AF94 - 0x14AE00 ];
            GUN_TR5     m_GunTR5;
        };

        struct
        {
            BYTE        unknown05 [  0x14AE4A - 0x14AE00 ];
            WORD        m_iAir;
        };

        struct
        {
            BYTE        unknown06 [  0x14AE78 - 0x14AE00 ];
            BYTE        m_iLara;
        };
    };
};

//
struct GUN_TR6
{

};

//
struct TABLE_TR6
{

};

//  Starts at 0x2000
union TRR4SaveGame
{
    union
    {
        BYTE full [ TR456_SLOT_LENGTH ];
        TABLE_TR4   table;
    };
};

//  Starts at 14AE00
struct TRR5SaveGame
{
    union
    {
        BYTE full [ TR456_SLOT_LENGTH ];
        TABLE_TR5   table;
    };

};

//  Start at 293C00 
struct TRR6SaveGame
{
    union
    {
        BYTE full [ TR456_SLOT_LENGTH ];
        TABLE_TR6   table;
    };
};


struct TR456Savegame
{
    struct Begining
    {
        BYTE    begining [ 0x2000 ];
    } beg;

    //  Start 002000 Size 148E00
    struct TRR4SaveGames
    {
        TRR4SaveGame trr4 [ NB_SLOT_456 ];
    } trr4s;

    //  Start 14AE00 Size 148E00
    struct TRR5SaveGames
    {
        TRR5SaveGame trr5 [ NB_SLOT_456 ];
    } trr5s;

    // Start 293C00 Size 148E00
    struct TRR6SaveGames
    {
        TRR6SaveGame trr6 [ NB_SLOT_456 ];
    } trr6s;

    DWORD   dwEnd;
};

//  Indicator Structure for TR 4 and 5
typedef struct indicatorTRR45Struct
{
    BOOL    bEnd;
    BYTE    b1;
    BYTE    b2;
    BYTE    b3;
    BYTE    b4;
    BOOL    useB3;
    //  0 for reliable
    //  1-8 for usable
    //  9 for unusable
    int     step;
    char    szLabel [ INDICATOR_LABEL_SIZE ];
} TRR45_INDICATORS;

struct TR8_POSITION
{
    BYTE    cMoving1;           //  0000 Change when moving
    BYTE    unknown1;           //  0001
    WORD    wWestToEast;        //  0002-0003 Change when moving East West
    short   wVertical;          //  0004-0005
    WORD    wSouthToNorth;      //  0006-0007 Change when moving South North
    BYTE    cRoom;              //  0008
    BYTE    unknown2;           //  0009
    BYTE    cOrientation;       //  000a
    BYTE    indicator1;         //  000b eg 0x02
    BYTE    indicator2;         //  000c eg 0x02
    BYTE    indicator3;         //  000d
    BYTE    indicator4;         //  000e eg 0x67
    BYTE    unknown3;           //  000f
    BYTE    cMoving2;           //  0010    Change when moving
    BYTE    unknown4;
    WORD    health;             //  0012
};

#pragma pack(pop, pack1)

//
/////////////////////////////////////////////////////////////////////////////
class CTR8SaveGame : public CObject
{
    DECLARE_DYNAMIC(CTR8SaveGame)

    protected :
        static CTR8SaveGame *m_pInstance;
    public :
        static CTR8SaveGame *GetInstance(BOOL bCreate = TRUE);
        static CTR8SaveGame *I(BOOL bCreate = TRUE);
        static BOOL IsBufferModified();
        static BOOL WriteFile(const char *pFilename);
        static void Discard();

    protected:
        CTR8SaveGame(void);

    public:
        virtual ~CTR8SaveGame(void);

        void Init( bool bFromContructor = false );

        //  Each block contains 31 structures for save position

    protected :
        static char *m_TR4_Start;
        static char *m_TR5_Start;
        static char *m_TR6_Start;

        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR4_Blocks [ NB_SLOT_456 ];
        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR5_Blocks [ NB_SLOT_456 ];
        //  Blocks are Save slot
        //  Inside a Blocks are there is a save list
        static char *m_TR6_Blocks [ NB_SLOT_456 ];

        //  Guns is the reference for gun and ammos
        static GUN_TR4 *m_TR4_Guns [ NB_SLOT_456 ];
        //  Guns is the reference for gun and ammos
        static GUN_TR5 *m_TR5_Guns [ NB_SLOT_456 ];
        //  Guns is the reference for gun and ammos
        static GUN_TR6 *m_TR6_Guns [ NB_SLOT_456 ];

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
        bool isKnown(int tombraider, const char *position, const char *pStart);
        const char *getPositionLabel(int tombraider, const char *position);
        const char *getIndicatorLabel()
        {
            return m_szIndicatorLabel;
        };

        WORD *GetRealHealthAddress ( int tombraider, int block );

        //
        TR8_POSITION *GetPositionAddress ( int tombraider, int block );

        WORD GetRealHealth ( int tombraider, int block );
        void SetRealHealth ( int tombraider, int block, WORD value );

        //  return the TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer
        void *SearchBlockEntry ( int tombraider, int block );
        //
        void *GetDefaultBlockEntry ( int tombraider, int block );

        //  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
        void *SearchGunEntry ( int tombraider, int block );
        void *GetDefaultGunEntry ( int tombraider, int block );

        //
        BYTE GetBlockIndicator ( int tombraider, int block );
        const char *GetBlockHexa ( int tombraider, int block, int iStart = 0, int iEnd = -1 );

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

        //  Only In Block : Object 
        BYTE *GetObjectBaseAddress ( int tombraider, int block );
        BYTE *GetBlockObjectAddress ( int tombraider, int block );

        //  Object 
        int GetBlockObject ( int tombraider, int block, int object );

        //  Object 
        void SetBlockObject ( int tombraider, int block, int object, int val );

        //  Addresses In Table
        ////////////////////////////////////////////////////
        BYTE *GetSlotMedipakUsedAddress(int tombraider, int block);
        BYTE *GetSlotPickupAddress(int tombraider, int block);
        WORD *GetSlotKillsAddress(int tombraider, int block);
        BYTE *GetSlotCrystalAddress ( int tombraider, int block );
        BYTE *GetSlotTreasureAddress ( int tombraider, int block );

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

        const char *GetInterest ( int tombraider, int block );

        void ReNumber ( );
        BOOL Delete ( int tombraider, int block );
        BOOL Export ( const char *pFilename, int tombraider, int block );
        BOOL Import( const char *pFilename, int tombraider, int block );

        BOOL BufferModified ();
        void discard();

        //  Set At STart Condition
        void SetLikePlus ( int tombraider, int block );
        void SetLikeGame ( int tombraider, int block );

        //
        void SetPistols ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos = 0xffff, bool bZeroWhenDisabled = false );
        void SetMagnum ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetUzi ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetRiotgun ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetM16 ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetHarpoon ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetGrenade ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );
        void SetRocket ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = false );

        //
        void SetPistols ( int tombraider, int block, BOOL bEnabled );
        void SetMagnum ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetUzi ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetRiotgun ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetM16 ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetHarpoon ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetGrenade ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );
        void SetRocket ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled = true );

        //
        void TraceBlock ( int tombraider, int block );
        void TraceGun ( int tombraider, int block );

        //
        static int GetBlockLength ( int tombraider );
        static int GetSlotLength ( int tombraider );
        static void *GetBlockStart ( int tombraider, int block, bool bForce = false );
        static void *GetSlotsEnd ( int tombraider, int block, bool bForce = false );
        //  Return directly m_TRx_Blocks [ b ] [ s ]
        static void *GetBlockSlot ( int tombraider, int block );

        //  return  m_TRX_Guns [ block ] [ slot ]
        static void *GetBlockGuns ( int tombraider, int block );
        //  Return m_TRx_Blocks [ b ] [ s ] but iif null can return the theorical address
        static void *GetBlockAddress ( int tombraider, int block, bool bForce = false );
        //  return  m_TRX_Guns [ block ] [ slot ]
        static void *GetGunAddress ( int tombraider, int block );

        //  Get Maximum Secrets for level Index from 0
        static BYTE GetSecretsForLevel ( int tombraider, int levelIndex );
        static WORD GetKillssForLevel ( int tombraider, int levelIndex );
        static WORD GetPickupsForLevel ( int tombraider, int levelIndex );
        static WORD GetCrystalsForLevel ( int tombraider, int levelIndex );

        //  From 1 to 32
        static const char *GetLevelName ( int tombraider, int level );
        static int GetLevelCount ( int tombraider );

        static const char *GetIndicatorFileType()
        {
            return ".trr.txt";
        }
        static int ReadIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static BOOL WriteIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static int CountIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MinIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MaxIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable );

        const TR_POSITION *GetPosition ( int tombraider, int block );
        BOOL SetPosition (  int tombraider, int block,
                            DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom );
};
