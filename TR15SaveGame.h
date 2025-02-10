#if !defined(AFX_TUBSAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
#define AFX_TUBSAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_

#include "TR123SaveGame.h"

#include "TRXTools.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
/////////////////////////////////////////////////////////////////////////////
// TR15SaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTUBSaveGame document
//
/////////////////////////////////////////////////////////////////////////////
/*
 *      ------------------------------------------------
 *      Definition.
 *      ------------------------------------------------
 */
#define TUBMAXLEVEL         4   
#define TUBBUFFEREND        0x270F
#define TUBLEVELMINSIZE     10434
#define TUBLEVELMAXSIZE     10435
#define TUBLEVELBUFSIZE     80*1024

/*
 *      ------------------------------------------------
 *      Structures.
 *      ------------------------------------------------
 */
#pragma pack(push, pack1, 1)

struct          TUBAmmos
{
    WORD            iAir;

    BYTE            cFiller1 [ 31 ];

    BYTE            cFiller2;

    char            cFiller3 [ 100 ];

    WORD            m_iGunAmmos;
    char            cFiller01 [ 0x0A ];
    WORD            m_iDesertEagle;
    char            cFiller02 [ 0x0A ];
    WORD            m_iUzis;
    char            cFiller03 [ 0x0A ];
    WORD            m_iRiotGun;
};
typedef struct TUBAmmos TUBAMMOS;

struct          TUBStruct
{
    WORD            m_iGunAmmos;
    WORD            m_iDesertEagle;
    WORD            m_iUzis;
    WORD            m_iRiotGun;
    BYTE            cSmallPack;
    BYTE            cLargePack;

    BYTE            cNbObject1;

    BYTE            cFiller20 [ 2 ];
    BYTE            cGunBitmap;
    BYTE            cFiller30;
};

typedef struct TUBStruct TUBSTRUCT;

struct          TUBSave
{
    union
    {
        struct
        {
            char            szSavename [ 0x18 ];
            char            cFiller01 [ 0x33 ];
            WORD            iSaveNumber;

            char            cFiller02 [ 0x11 ];
            TUBSTRUCT       trTable [ TUBMAXLEVEL ];
            TUBSTRUCT       trSingle;
            char            cFiller30 [ 0x0A ];
            BYTE            cLevel;
            BYTE            cSub;
            char            cFiller31 [ 1 ];

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

            char           cFiller40 [ 243 ];

            BYTE           szRemain [ TUBBUFFEREND ];
        };

        struct
        {
            char           cFullBuffer [ TUBLEVELBUFSIZE ];
        };
    };

};

typedef struct TUBSave TUBSAVE;

#pragma pack(pop, pack1)

class CTUBSaveGame : public CTR123SaveGame
{
    DECLARE_DYNAMIC(CTUBSaveGame)

    friend class CTRSaveGame;

    protected:
        CTUBSaveGame();

    public :
        virtual ~CTUBSaveGame();

    // Attributes
    protected :
        TUBSAVE             *m_pBuffer;
        TUBSAVE             *m_pBufferBackup;

    protected :
        TUBAMMOS            *m_pGun;

        //  Mask
        BYTE                iMaskCompass;
        BYTE                iMaskPistol;
        BYTE                iMaskMagnum;
        BYTE                iMaskUzi;
        BYTE                iMaskShotGun;
        BYTE                iMaskHarpoon;
        BYTE                iMaskMP5;
        BYTE                iMaskGrenade;


    // Operations
    public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTUBSaveGame)
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
        virtual const char *GetLabel0() {  return "Compass"; };

        //  Pistols
        BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon1 () {  return CTRXTools::IsEnablePistol(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos1 () {  return FALSE; };
        //  Pistols
        virtual const char *GetLabel1() {  return "Guns / Pistols"; };
        virtual UINT GetBitmap1 ( ) { return IDB_GUNS; };

        //  Magnum
        BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon2 () {  return CTRXTools::IsEnableMagnum(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos2 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(), getLevel() ); };
        //  Magnum
        virtual const char *GetLabel2() {  return "Magnum"; };
        virtual UINT GetBitmap2 ( ) { return IDB_DESERT_EAGLE; };

        //  Uzi
        BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon3 () {  return CTRXTools::IsEnableMagnum(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos3 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(), getLevel() ); };
        //  Uzi
        virtual const char *GetLabel3() {  return "Uzi"; };
        virtual UINT GetBitmap3 ( ) { return IDB_UZIS; };

        //  Shotgun
        BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon4 () {  return CTRXTools::IsEnableMagnum(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos4 () {  return CTRXTools::IsAmmosMagnum(getFullVersion(), getLevel() ); };
        //  Shotgun
        virtual const char *GetLabel4() {  return "Shotgun"; };
        virtual UINT GetBitmap4 ( ) { return IDB_RIOT_GUN; };

        //  MP5
        BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon5 () {  return CTRXTools::IsEnableM16(getFullVersion(), getLevel() ); };
        virtual BOOL HaAmmos5 () {  return CTRXTools::IsAmmosM16(getFullVersion(), getLevel() ); };
        //  MP5
        virtual const char *GetLabel5() {  return "MP5"; };
        virtual UINT GetBitmap5 ( ) { return IDB_MP5; };

        //  Grenade Launcher
        BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon6 () {  return CTRXTools::IsEnableGrenade(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos6 () {  return CTRXTools::IsAmmosGrenade(getFullVersion(), getLevel() ); };
        //  Grenade Launcher
        virtual const char *GetLabel6() {  return "Rocket Launcher"; };
        virtual UINT GetBitmap6 ( ) { return IDB_ROCKET_LAUNCHER; };

        //  Grenade Launcher : Not in TR1
        BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon7 () {  return CTRXTools::IsEnableRocket(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos7 () {  return CTRXTools::IsAmmosRocket(getFullVersion(), getLevel() ); };
        //  Grenade Launcher : Not in TR1
        virtual const char *GetLabel7() {  return "Grenade Launcher"; };
        virtual UINT GetBitmap7 ( ) { return IDB_GRENADE_LAUNCHER; };

        //  Harpoon : Not in TR1
        BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true );
        virtual BOOL HasWeapon8 () {  return CTRXTools::IsEnableHarpoon(getFullVersion(), getLevel() ); };
        virtual BOOL HasAmmos8 () {  return CTRXTools::IsAmmosHarpoon(getFullVersion(), getLevel() ); };
        //  Harpoon : Not in TR1
        virtual const char *GetLabel8() {  return "Harpoon"; };
        virtual UINT GetBitmap8( ) { return IDB_HARPOON; };

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

        //  For TRUB and below check pointer
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
        TUBAMMOS *SearchGunStructure ( WORD m_iHealth, WORD gunBitmap, int *iPos );
        int CheckIfAmmosMatch ( TUBAMMOS *pGun, WORD gunBitmap );
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


#endif // !defined(AFX_TUBSAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
