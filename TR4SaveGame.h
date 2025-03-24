#pragma once

#include "TRXTools.h"
#include "TR45SaveGame.h"

//
/////////////////////////////////////////////////////////////////////////////
// TR4SaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
//

//  00000001
#define TR40_GUN_SET1       0x01
//  00000100
#define TR40_GUN_SET4       0x04
//  00001000
#define TR40_GUN_SET8       0x08
//  00001001
#define TR40_GUN_SET        (TR40_GUN_SET1|TR40_GUN_SET8)
//  00001101
#define TR40_GUN_MASK       (TR40_GUN_SET1|TR40_GUN_SET4|TR40_GUN_SET8)

//
/////////////////////////////////////////////////////////////////////////////
// CTR4SaveGame document
//
/////////////////////////////////////////////////////////////////////////////
/*
 *      ------------------------------------------------
 *      Definition.
 *      ------------------------------------------------
 */
#define TR4MAXLEVEL         38
#define TR4BUFFEREND        14014
#define TR4LEVELMINSIZE     15954
#define TR4LEVELMAXSIZE     15955
#define TR4LEVELALTSIZE     66330
#define TR4LEVELBUFSIZE     80*1024

//
//  0x3ed :
//  0x3ef :
//  0x3f0 0x3f1
//  East West : 0x3f0 0x3f1 0x3f7
//  Height : 0x3f2 0x3f3 and 0x3f7
//  North South : 0x3f4 0x3f5 0x3f7
//  0x3f6 : room
//  0x3f6 - 0x3f7 : Orientation

/*
 *      ------------------------------------------------
 *      Structures.
 *      ------------------------------------------------
 */
#pragma pack(push, pack1, 1)

//  Position is Variable
//  The room may be 82 (dec) before the 02 02 xx 67
//  This structure is 8 before the 02 02 xx  67
#define TR4_POSITION_OFFSET         11
#define TR4_FACTOR                  2L
struct TR4_POSITION
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
#define TR4_REALHEALTH_OFFSET   (offsetof(TR4_POSITION,health)-offsetof(TR4_POSITION,indicator1))

struct          TR4Gun
{
    //  0x169 : 09
    BYTE        m_gunPistol;

    //  0x16a : 01
    BYTE        m_gunUzis;

    //  0x16b : 09
    BYTE        m_gunRiotGun;

    //  0x16c : 09
    BYTE        m_gunCrossBow;

    //  0x16d : 09
    BYTE        m_gunGrenadesLauncher;

    //  0x16e : 01
    BYTE        m_gunRevolver;

    //  0x16f : 01
    BYTE        m_gunLaserLight;

    //  0x170 : 01
    BYTE        m_gunBinocular;

    //  0x171 : 01
    BYTE        m_gunCrowBar;

    //  0x172-0x18e : Objects
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
typedef struct TR4Gun   TR4GUN;

struct          TR4Ammo
{
    //  0x190
    WORD    m_iSmallMedipak;

    //  0x192
    WORD    m_iLargeMedipak;

    //  0x194 / 0x195 : 01
    WORD    m_iFlares;

    //  0x196
    //  Normally 0xFFFF
    WORD    m_iGunAmmos;

    //  0x198
    WORD    m_iUziAmmos;

    //  0x19a
    WORD    m_iRevolverAmmos;

    //  0x19c/ 0x19d
    WORD    m_iShotGunAmmo1;

    //  0x19e/ 0x19f
    WORD    m_iShotGunAmmo2;

    //  0x1A0
    WORD    m_iGrenade1;

    //  0x1A2
    WORD    m_iGrenade2;

    //  0x1A4
    WORD    m_iGrenade3;

    //  0x1A6
    WORD    m_iCrossbow1;

    //  0x1A8
    WORD    m_iCrossbow2;

    //  0x1AA
    WORD    m_iCrossbow3;

    //
    BYTE    m_Filler2 [ 0x1fb - 0x1AA - sizeof(short) ];

    //  0x1fb
    BYTE    m_iSecretCount;

};
typedef struct TR4Ammo  TR4AMMO;

struct          TR4Save
{
    union
    {
        struct
        {
            char                    szSavename [ 24 ];
            char                    cFiller1 [ 51 ];

            //  0x4b / 0x4c
            WORD                    iSaveNumber;
        };

        struct
        {
            char                    cFiller1A [ 0x9B ];

            //      0x9b
            //      0x04    = normal
            //      0x0c    = burning
            BYTE                    laraState;
        };

        struct
        {
            char                    cFiller1B [ 0x6d ];

            WORD                    iAir;
        };

        struct
        {
            char                    cFiller2 [ 0x169 ];

            //      0x169
            TR4GUN                  tagGuns;
        };

        struct
        {
            char                    cFiller3 [ 0x190 ];

            //      0x194
            TR4AMMO                 tagAmmo;
        };

        struct
        {
            char                    cFiller3b [ 0x1b5];

            //      0x1b5
            BYTE                    m_cLevels [ 10 ];
        };

        //   0x1e8  : statistics timing ...
        //  000001ED distance


        // 0x3e51 une sorte de checksum
        struct
        {
            char                    cFiller4 [ 0x3e51 ];
            BYTE                    checkSum;
        };

        struct
        {
            char                    cFullBuffer [ TR4LEVELBUFSIZE ];
        };
    };
};

typedef struct TR4Save TR4SAVE;

#pragma pack(pop, pack1)

class CTR4SaveGame : public CTR45SaveGame
{
    DECLARE_DYNAMIC(CTR4SaveGame)

    friend class CTRSaveGame;

    protected:
        CTR4SaveGame();

    public:
        virtual ~CTR4SaveGame();

    // Attributes
    protected :
        TR4SAVE             *m_pBuffer;
        TR4SAVE             *m_pBufferBackup;

    protected :
        //  Mask
        BYTE                iMaskCompass;
        BYTE                iMaskGun;
        BYTE                iMaskPistol;
        BYTE                iMaskRevolver;
        BYTE                iMaskUzi;
        BYTE                iMaskRiotGun;
        BYTE                iMaskCrossBow;
        BYTE                iMaskGrenade;
        BYTE                iMaskCrowBar;
        BYTE                iMaskHeadSet;
        BYTE                iMaskLaser;
        BYTE                iMaskBinocular;
        BYTE                iRiotGunUnits;
    // Operations
    public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTR4SaveGame)
        public:
        //}}AFX_VIRTUAL

    // Implementation
    public:

        char *getBufferAddress ();
        char *getBufferBackupAddress ();
        size_t getBufferLength();

        BOOL BufferModified();
        void discard();

        int GetLaraState ( );
        void SetLaraState ( int state );

        WORD *GetTR4RealHealthAddress();
        int GetRealHealth ();
        void SetRealHealth ( const char *szRealHealth );
        void *GetIndicatorAddress ( int index = 0 );

        int GetMap1 ();
        int GetMap2 ();

        int GetItem1 ();
        int GetItem2 ();
        int GetItem3 ();
        int GetItem4 ();

        int GetKey1 ();
        int GetKey2 ();
        int GetKey3 ();
        int GetKey4 ();

        int GetTreasure1 ( int iX );
        int GetTreasure2 ( int iX );
        int GetTreasure3 ( int iX );
        int GetTreasure4 ( int iX );

        void SetMap1 ( char *szString );
        void SetMap2 ( char *szString );

        void SetItem1 ( char *szString );
        void SetItem2 ( char *szString );
        void SetItem3 ( char *szString );
        void SetItem4 ( char *szString );

        void SetKey1 ( char *szString );
        void SetKey2 ( char *szString );
        void SetKey3 ( char *szString );
        void SetKey4 ( char *szString );

        void SetTreasure1 ( char *szString, int iX );
        void SetTreasure2 ( char *szString, int iX );
        void SetTreasure3 ( char *szString, int iX );
        void SetTreasure4 ( char *szString, int iX );

        void SetAllObjects ();

        int IsLaraBurning ();
        void SetLaraBurning (int state = -1);
        void SetLaraNoBurning (int state = -1);

        char *GetSaveName ();
        int GetSaveNumber ();

        int GetCurrentSecrets ( );
        void SetCurrentSecrets ( char *szString, int iX = -1 );
        void SetAllSecrets ( );
        int GetMaximumSecrets ( int iX );

        int GetUnlimitedAmmos();
        void SetUnlimitedAmmos ( int iMask );

        int GetGunAmmos ( );
        int GetSmallMedipak ( int iX );
        int GetLargeMedipak ( int iX );
        int GetAir ( );
        int GetFlares ( int iX );
        int GetLaser ( int iX = 0 );
        int GetBinocular ( int iX = 0 );

        void SetGunAmmos ( const char *szGunAmmos );

        void SetSmallMedipak ( const char *szString, int iX );
        void SetLargeMedipak ( const char *szString, int iX );
        void SetAir ( const char *szString );
        void SetFlares ( const char *szString, int iX );
        void SetLaser ( char *szString, int iX );
        void SetBinocular ( char *szString, int iX );

        int CheckWeapon1 ( int iX );
        int CheckWeapon2 ( int iX );
        int CheckWeapon3 ( int iX );
        int CheckWeapon4 ( int iX );
        int CheckWeapon5 ( int iX );
        int CheckWeapon6 ( int iX );
        int CheckWeapon7 ( int iX );
        int CheckWeapon8 ( int iX );
        int CheckWeapon9 ( int iX );

        // No Compass
        BYTE GrabWeapon0 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon0 () {  return TRUE; };
        virtual BOOL HasAmmos0 () {  return FALSE; };

        //  Pistols
        BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon1 () {  return CTRXTools::IsEnablePistol(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos1 () {  return TRUE; };
        //  Pistols
        virtual const char *GetLabel1() {  return "Guns / Pistols"; };
        virtual UINT GetBitmap1 ( ) { return IDB_GUNS; };

        //  Revolver
        BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon2 () {  return CTRXTools::IsEnableMagnum(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos2 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(), getLevel() ); };
        //  Revolver
        virtual const char *GetLabel2() {  return "Revolver"; };
        virtual UINT GetBitmap2 ( ) { return IDB_REVOLVER; };

        //  Uzi
        BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon3 () {  return CTRXTools::IsEnableUzi(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos3 () {  return CTRXTools::IsAmmosUzi(getFullVersion(), getLevel() ); };
        //  Uzi
        virtual const char *GetLabel3() {  return "Uzi"; };
        virtual UINT GetBitmap3 ( ) { return IDB_UZIS; };

        //  Riot Gun
        BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon4 () {  return CTRXTools::IsEnableRiotgun(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos4 () {  return CTRXTools::IsAmmosRiotgun(getFullVersion(), getLevel() ); };
        //  Riot Gun
        virtual const char *GetLabel4() {  return "Riot Gun"; };
        virtual UINT GetBitmap4 ( ) { return IDB_RIOT_GUN; };

        //
        BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon5 () {  return CTRXTools::IsEnableM16(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos5 () {  return CTRXTools::IsAmmosM16(getFullVersion(), getLevel() ); };

        //
        BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon6 () {  return CTRXTools::IsEnableRocket(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos6 () {  return CTRXTools::IsAmmosRocket(getFullVersion(), getLevel() ); };

        //  Grenade Launcher
        BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon7 () {  return  CTRXTools::IsEnableGrenade(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos7 () {  return  CTRXTools::IsAmmosGrenade(getFullVersion(), getLevel() ); };
        //  Grenade Launcher
        virtual const char *GetLabel7() {  return "Grenade Launcher"; };
        virtual UINT GetBitmap7 ( ) { return IDB_GRENADE_LAUNCHER; };

        //  Crossbow
        BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon8 () {  return  CTRXTools::IsEnableHarpoon(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos8 () {  return  CTRXTools::IsAmmosHarpoon(getFullVersion(), getLevel() ); };
        //  Crossbow
        virtual const char *GetLabel8() {  return "Crossbow"; };
        virtual UINT GetBitmap8 ( ) { return IDB_CROSSBOW; };

        //  Crossbar
        BYTE GrabWeapon9 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon9 () {  return  CTRXTools::IsEnableCrowbar(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos9 () {  return  FALSE; };
        //  Crossbow
        virtual const char *GetLabel9() {  return "Crowbar"; };
        virtual UINT GetBitmap9 ( ) { return IDB_CROWBAR; };

        //
        virtual UINT GetAmmoBitmap1 ( ) { return IDB_AMMOS_GUNS; };

        //
        int GetAmmos1( int iX );
        void SetAmmos1 ( const char *szString, int iX );

        //
        int GetAmmos2( int iX );
        virtual UINT GetAmmoBitmap2 ( ) { return IDB_AMMOS_REVOLVER; };
        void SetAmmos2 ( const char *szString, int iX );

        //
        int GetAmmos3( int iX );
        virtual UINT GetAmmoBitmap3 ( ) { return IDB_AMMOS_UZIS; };
        void SetAmmos3 ( const char *szString, int iX );

        //
        int GetAmmos4a ( int iX );
        int GetAmmos4b ( int iX );
        virtual UINT GetAmmoBitmap4 ( ) { return IDB_AMMOS_RIOTGUN; };
        void SetAmmos4a ( const char *szString, int iX );
        void SetAmmos4b ( const char *szString, int iX );

        //
        int GetAmmos5 ( int iX );
        virtual UINT GetAmmoBitmap5 ( ) { return IDB_AMMOS_GUNS; };
        void SetAmmos5 ( const char *szString, int iX );

        //
        int GetAmmos6( int iX );
        virtual UINT GetAmmoBitmap6 ( ) { return IDB_AMMOS_GUNS; };
        void SetAmmos6 ( const char *szString, int iX );

        int GetAmmos7a( int iX );
        int GetAmmos7b( int iX );
        int GetAmmos7c( int iX );
        virtual UINT GetAmmoBitmap7 ( ) { return IDB_AMMOS_GRENADES; };
        void SetAmmos7a ( const char *szString, int iX );
        void SetAmmos7b ( const char *szString, int iX );
        void SetAmmos7c ( const char *szString, int iX );

        //
        int GetAmmos8a( int iX );
        int GetAmmos8b( int iX );
        int GetAmmos8c( int iX );
        virtual UINT GetAmmoBitmap8 ( ) { return IDB_AMMOS_ARROW; };
        void SetAmmos8a ( const char *szString, int iX );
        void SetAmmos8b ( const char *szString, int iX );
        void SetAmmos8c ( const char *szString, int iX );

        //
        virtual BOOL HasItems ( ) { return TRUE; };
        virtual void SetItems ( int item, BYTE value );
        virtual BYTE GetItems ( int item );

        //  Simple Check for version TR4
        int Valid ();
        void SetInvalid ();

        virtual int getLevelIndex();
        virtual int getLevel();
        const char *GetStatus ();

        void GetDetailedInfo (  char *szGame, size_t iSize, int *iGame, int *iLevel,
                                char *szTitle, size_t iSizeTile );
        void writeSaveGame();
        int ReadSavegame( const char *pFilename );
        void RetrieveInformation( const char *pFilename );
        TR4GUN *SearchGunStructure ( WORD m_iHealth, int *iPos );
        int CheckIfAmmosMatch ( TR4GUN *pGun, WORD gunBitmap );
        void RetrieveHeader();
        void GetAmmosValues ( );
        int ConvertSecretBack ( int iSecret );
        int ConvertSecret ( int iSecret );

        const TR_POSITION *GetPosition ( );
        TR4_POSITION *GetTR4Position ( );
        BOOL SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom );

        static const char *GetIndicatorFileType()
        {
            return ".tr4.txt";
        }

    // Generated message map functions
};

//
extern TR45_INDICATORS IndicatorsTR4Table [ MAX_INDICATORS ];
extern int IndicatorsTR4TableCount;
