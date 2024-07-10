#if !defined(AFX_TR3SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR3SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_

#include "TR123SaveGame.h"

#include "TRXTools.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
/////////////////////////////////////////////////////////////////////////////
// TR3SaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTR3SaveGame document
//
/////////////////////////////////////////////////////////////////////////////
/*
 *      ------------------------------------------------
 *      Definition.
 *      ------------------------------------------------
 */
#define TR3MAXLEVEL         23
#define TR3BUFFEREND        12102
#define TR3LEVELMINSIZE     13913
#define TR3LEVELMAXSIZE     13914

/*
 *      ------------------------------------------------
 *      Structures.
 *      ------------------------------------------------
 */
#pragma pack(push, pack1, 1)

struct          TR3Ammos
{
    WORD            iAir;

    BYTE            cFiller1 [ 28 ];

    //  Burning
    BYTE            cState;

    //
    char            cFiller2 [ 145 ];

    //  Health
    WORD  m_iGunAmmos;
    WORD  iFiller10;

    //  Desert Eagle
    WORD  m_iDesertEagle;
    WORD  iFiller11;

    //  Uzi
    WORD  m_iUzis;
    WORD  iFiller12;

    //  Riot Gun
    WORD  m_iRiotGun;
    WORD  iFiller13;

    //  Harpoon
    WORD  m_iHarpoon;
    WORD  iFiller14;

    //  Rocket
    WORD  m_iRockets;
    WORD  iFiller15;

    //  Greande
    WORD  m_iGrenades;
    WORD  iFiller16;

    //  MP5
    WORD  m_iMP5;
    WORD  iFiller17;

};
typedef struct TR3Ammos TR3AMMOS;

struct          TR30Struct
{
    WORD  m_iGunAmmos;

    //  Desert Eagle
    WORD  m_iDesertEagleAmmos;

    //  Uzi
    WORD  m_iUziAmmos;

    // Riot Gun
    WORD  m_iRiotGunAmmos;

    //  MP5
    WORD  m_iMP5Ammos;

    //  Rocket
    WORD  m_iRocketAmmos;

    //  Harpoo,s
    WORD  m_iHarpoonAmmos;

    //  Grenade
    WORD  m_iGrenadeAmmos;

    //  Small Pack
    BYTE   cSmallPack;

    //  Large Pack
    BYTE   cLargePack;

    BYTE   cUnknown;

    //  Flare
    BYTE   cFlare;
    BYTE   cFiller1;
    BYTE   cFiller2;
    BYTE   cFiller3;
    WORD  cGunBitmap;

    WORD  cFiller4;

    BYTE   cNbObject1;
    BYTE   cNbObject2;
    BYTE   cNbObject3;
    BYTE   cNbObject4;

    BYTE   cFillers [ 18 ];
    char   cSecrets;
    BYTE   cFiller99;

};

typedef struct TR30Struct TR30STRUCT;

struct          TR3Permanent
{
    TR30Struct      tagSummerize;
    WORD            iFiller1;
    char            cFiller2 [ 16 ];
    char            cSecrets;
    char            cFiller4 [ 21 ];
    BYTE            cLevel;
    BYTE            cSub;
    BYTE            cUnlimited;

    BYTE            cObject_P1;
    BYTE            cObject_P2;

    BYTE            cObjectP1;
    BYTE            cObjectP2;
    BYTE            cObjectP3;
    BYTE            cObjectP4;

    BYTE            cObjectK1;
    BYTE            cObjectK2;
    BYTE            cObjectK3;
    BYTE            cObjectK4;

    WORD            cFiller6;
    WORD            cFiller7;
    WORD            cFiller8 [ 3 ];

    BYTE            cObjectMask;

    char            cFillers [ 307 ];

    WORD            m_iHealth;
};

typedef struct TR3Permanent TR3PERMANENT;

struct          TR3Save
{
    union
    {
        struct
        {
            char            szSavename [ 23 ];
            BYTE            ind1;       // 0xFA on TR1
            char            cFiller1 [ 51 ];
            WORD            iSaveNumber;
            char            cFiller2 [ 91 ];

            char            cFiller3 [ 46 ];
            TR30Struct      trTable [ TR3MAXLEVEL ];
            TR3Permanent    trSingle;

            BYTE           szRemain [ TR3BUFFEREND ];
        };

        struct
        {
            char                    cFullBuffer [ TR3LEVELMAXSIZE ];
        };
    };
};

typedef struct TR3Save TR3SAVE;

#pragma pack(pop, pack1)

class CTR3SaveGame : public CTR123SaveGame
{
    DECLARE_DYNAMIC(CTR3SaveGame)

    friend class CTRSaveGame;

    protected:
        CTR3SaveGame();

    public:
        virtual ~CTR3SaveGame();

    // Attributes
    protected :
        TR3SAVE             *m_pBuffer;
        TR3SAVE             *m_pBufferBackup;

    protected :
        TR3AMMOS            *m_pGun;

        //  Mask
        WORD                iMaskCompass;
        WORD                iMaskPistol;
        WORD                iMaskShotGun;
        WORD                iMaskDesertEagle;
        WORD                iMaskUzi;
        WORD                iMaskMP5;
        WORD                iMaskHarpoon;
        WORD                iMaskCrossBow;
        WORD                iMaskGrenade;
        WORD                iMaskRocket;
    // Operations
    public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTR3SaveGame)
        public:
        //}}AFX_VIRTUAL

    // Implementation
    public:

        char *getBufferAddress ();
        char *getBufferBackupAddress ();
        size_t getBufferLength();

        BOOL BufferModified();
        void discard();

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
        int GetFlares ( int iX );
        int GetAir ( );

        void SetGunAmmos ( const char *szGunAmmos );

        void SetSmallMedipak ( const char *szString, int iX );
        void SetLargeMedipak ( const char *szString, int iX );
        void SetFlares ( const char *szString, int iX );
        void SetAir ( const char *szString );

        int CheckWeapon1 ( int iX );
        int CheckWeapon2 ( int iX );
        int CheckWeapon3 ( int iX );
        int CheckWeapon4 ( int iX );
        int CheckWeapon5 ( int iX );
        int CheckWeapon6 ( int iX );
        int CheckWeapon7 ( int iX );
        int CheckWeapon8 ( int iX );

        //  Compass
        BYTE GrabWeapon0 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon0 () {  return TRUE; };
        virtual BOOL HasAmmos0 () {  return FALSE; };

        //  Pistols
        BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon1 () {  return CTRXTools::IsEnablePistol(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos1 () {  return FALSE; };
        //  Pistols
        virtual const char *GetLabel1() {  return "Guns / Pistols"; };
        virtual UINT GetBitmap1 ( ) { return IDB_GUNS; };

        //  Desert Eagle
        BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon2 () {  return CTRXTools::IsEnableMagnum(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos2 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(), getLevel() ); };

        //  Uzi
        BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon3 () {  return CTRXTools::IsEnableUzi(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos3 () {  return CTRXTools::IsAmmosUzi(getFullVersion(), getLevel() ); };

        //  Shotgun
        BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon4 () {  return CTRXTools::IsEnableRiotgun(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos4 () {  return CTRXTools::IsAmmosRiotgun(getFullVersion(), getLevel() ); };
        
        //  MP5
        BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon5 () {  return CTRXTools::IsEnableM16(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos5 () {  return CTRXTools::IsAmmosM16(getFullVersion(), getLevel() ); };

        //  Rocket
        BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon6 () {  return CTRXTools::IsEnableRocket(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos6 () {  return CTRXTools::IsAmmosRocket(getFullVersion(), getLevel() ); };

        //  Grenade
        BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon7 () {  return CTRXTools::IsEnableGrenade(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos7 () {  return CTRXTools::IsAmmosGrenade(getFullVersion(), getLevel() ); };

        //  Harpoon
        BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon8 () {  return CTRXTools::IsEnableHarpoon(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos8 () {  return CTRXTools::IsAmmosHarpoon(getFullVersion(), getLevel() ); };

        virtual BOOL HasWeapon9 () {  return FALSE; };
        virtual BOOL HasAmmos9 () {  return FALSE; };

        //
        int GetAmmos1( int iX );

        //
        int GetAmmos2( int iX );
        void SetAmmos2 ( const char *szString, int iX );

        int GetAmmos3( int iX  );
        void SetAmmos3 ( const char *szString, int iX );

        int GetAmmos4a ( int iX );
        void SetAmmos4a ( const char *szString, int iX );

        int GetAmmos5 ( int iX );
        void SetAmmos5 ( const char *szString, int iX );

        int GetAmmos6( int iX );
        void SetAmmos6 ( const char *szString, int iX );

        int GetAmmos7a( int iX );
        void SetAmmos7a ( const char *szString, int iX );

        int GetAmmos8a( int iX );
        void SetAmmos8a ( const char *szString, int iX );

        //  For TR3 and below check pointer
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
        TR3AMMOS *SearchGunStructure ( WORD m_iHealth, WORD gunBitmap, int *iPos );
        int CheckIfAmmosMatch ( TR3AMMOS *pGun, WORD gunBitmap );
        void RetrieveHeader();
        void GetAmmosValues ( );
        int ConvertSecretBack ( int iSecret );
        int ConvertSecret ( int iSecret );

        const TR_POSITION *GetPosition ( );
        BOOL SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom );

    // Generated message map functions
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
// The savegame object.


#endif // !defined(AFX_TR3SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
