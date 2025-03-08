#if !defined(AFX_TR2SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR2SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_

#include "TR123SaveGame.h"

#include "TRXTools.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
/////////////////////////////////////////////////////////////////////////////
// TR2SaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTR2SaveGame document
//
/////////////////////////////////////////////////////////////////////////////
/*
 *      ------------------------------------------------
 *      Definition.
 *      ------------------------------------------------
 */
#define TR2MAXLEVEL         23
#define TR2BUFFEREND        6030
#define TR2LEVELMINSIZE     7442
#define TR2LEVELMAXSIZE     7443
#define TR2LEVELBUFSIZE     80*1024

/*
 *      ------------------------------------------------
 *      Structures.
 *      ------------------------------------------------
 */
#pragma pack(push, pack1, 1)


struct          TR2Ammos
{
    //  Air
    WORD    iAir;

    //
    BYTE   cFiller1 [ 18 ];

    //  Burning
    BYTE   cState;

    //
    char            cFiller2 [ 145 ];

    //
    WORD    m_iGunAmmos;
    WORD    iFiller1;

    //
    WORD    m_iDesertEagle;
    WORD    iFiller2;

    WORD    m_iUzis;
    WORD    iFiller3;

    WORD    m_iRiotGun;
    WORD    iFiller4;

    WORD    m_iHarpoon;
    WORD    iFiller5;

    WORD    m_iGrenades;
    WORD    iFiller6;

    WORD    m_iM16Gun;
    WORD    iFiller7;

    WORD    m_iUnknownGun;
    WORD    iFiller8;

};
typedef struct TR2Ammos TR2AMMOS;

struct          TR20Struct
{
    WORD    m_iGunAmmos;

    //  Magnum
    WORD    m_iDesertEagleAmmos;

    //  Uzis
    WORD    m_iUziAmmos;

    //  Riot Gun
    WORD    m_iRiotGunAmmos;

    //  M16
    WORD    m_iM16GunAmmos;

    //  Greande
    WORD    m_iGrenadeAmmos;

    //  Harpoon
    WORD    m_iHarpoonAmmos;

    //  SDmall Pack
    BYTE    cSmallPack;

    /// Large Pack
    BYTE    cLargePack;

    //  
    BYTE    cUnknown;

    //  Flare
    BYTE    cFlare;
    BYTE    cFiller1;
    BYTE    cFiller3;

    //  Guns
    BYTE    cGunBitmap;
    BYTE    cFillers [ 21 ];

    //  Secrets
    BYTE    cSecrets;
    BYTE    cUnknownEnd;

};

typedef struct TR20Struct TR20STRUCT;

struct          TR2Save
{
    union
    {
        struct
        {
            char            szSavename [ 24 ];
            char            cFiller0 [ 1 ];
            BYTE            ind1;
            BYTE            ind2;
            char            cFiller1 [ 48 ];
            WORD            iSaveNumber;

            char            cFiller2 [ 46 ];
            TR20STRUCT      trTable [ TR2MAXLEVEL ];
            char            cFiller3 [ 18 ];
            BYTE            cSecrets;
            char            cFiller31;
            BYTE            cLevel;
            BYTE            cSub;

            BYTE           cUnlimited;

            BYTE           cObject_P1;
            BYTE           cObject_P2;

            BYTE           cObjectP1;
            BYTE           cObjectP2;
            BYTE           cObjectP3;
            BYTE           cObjectP4;

            BYTE           cObjectK1;
            BYTE           cObjectK2;
            BYTE           cObjectK3;
            BYTE           cObjectK4;
            char           cFiller6 [ 244 ];

            BYTE           szRemain [ TR2BUFFEREND ];
        };
        struct
        {
            char           cFullBuffer [ TR2LEVELBUFSIZE ];
        };
    };
};

typedef struct TR2Save TR2SAVE;

#pragma pack(pop, pack1)

class CTR2SaveGame : public CTR123SaveGame
{
    DECLARE_DYNAMIC(CTR2SaveGame)

    friend class CTRSaveGame;

    protected:
        CTR2SaveGame();

    public:
        virtual ~CTR2SaveGame();

    // Attributes
    protected :
        TR2SAVE             *m_pBuffer;
        TR2SAVE             *m_pBufferBackup;

    protected :
        TR2AMMOS            *m_pGun;

        //  Mask
        BYTE                iMaskCompass;
        BYTE                iMaskPistol;
        BYTE                iMaskDesertEagle;
        BYTE                iMaskUzi;
        BYTE                MaskShotGun;
        BYTE                iMaskHarpoon;
        BYTE                iMaskM16;
        BYTE                iMaskGrenade;

        // Operations
    public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTR2SaveGame)
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

        int GetLaraState ( );
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

        BYTE GrabWeapon0 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon0 () {  return TRUE; };
        virtual BOOL HasAmmmos0 () {  return FALSE; };

        BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon1 () {  return CTRXTools::IsEnablePistol(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos1 () {  return FALSE; };
        virtual const char *GetLabel1() {  return "Guns / Pistols"; };
        virtual UINT GetBitmap1 ( ) { return IDB_GUNS; };

        //  Desert Eagle
        BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon2 () {  return CTRXTools::IsEnableMagnum(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos2 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(),getLevel() ); };
        virtual const char *GetLabel2() {  return "Desert Eagle"; };
        virtual UINT GetBitmap2 ( ) { return IDB_DESERT_EAGLE; };

        //  Uzi
        BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon3 () {  return CTRXTools::IsEnableUzi(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos3 () {  return CTRXTools::IsAmmosUzi(getFullVersion(),getLevel() ); };

        //  Shotgun
        BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon4 () {  return CTRXTools::IsEnableRiotgun(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos4 () {  return CTRXTools::IsAmmosRiotgun(getFullVersion(),getLevel() ); };

        //  M16
        BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon5 () {  return CTRXTools::IsEnableM16(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos5 () {  return CTRXTools::IsAmmosM16(getFullVersion(),getLevel() ); };
        virtual const char *GetLabel5() {  return "M16"; };
        virtual UINT GetBitmap5 ( ) { return IDB_MP5; };

        //  Rocket Launcher
        BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon6 () {  return CTRXTools::IsEnableRocket(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos6 () {  return CTRXTools::IsAmmosRocket(getFullVersion(),getLevel() ); };

        //  Grenade Launcher
        BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon7 () {  return CTRXTools::IsEnableGrenade(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos7 () {  return CTRXTools::IsAmmosGrenade(getFullVersion(),getLevel() ); };

        //  Harpoon
        BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon8 () {  return CTRXTools::IsEnableHarpoon(getFullVersion(),getLevel() ); };
        virtual BOOL HasAmmos8 () {  return CTRXTools::IsAmmosHarpoon(getFullVersion(),getLevel() ); };

        //
        virtual BOOL HasWeapon9 () {  return FALSE; };
        virtual BOOL HaAmmos9 () {  return FALSE; };

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

        //  For TR2 and below check pointer
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
        TR2AMMOS *SearchGunStructure ( WORD m_iHealth, WORD gunBitmap, int *iPos );
        int CheckIfAmmosMatch ( TR2AMMOS *pGun, WORD gunBitmap );
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


#endif // !defined(AFX_TR2SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
