#pragma once

//
/////////////////////////////////////////////////////////////////////////////
// TRSaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <afx.h>

#include "resource.h"

#include "TRXTools.h"
#include "AutomaticVersionHeader.h"

//
// #pragma warning (disable : 4477)
// #pragma warning (disable : 4311)
// #pragma warning (disable : 4302)

//
/////////////////////////////////////////////////////////////////////////////
// CTRSaveGame document
//
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push, pack1, 1)

//  Seems to be 24 before TRLife found
//  0x22 (34) from iLife
//  Struct Size 3 * 4 + 4 * 2 = 20 bytes
#define TR_POSITION_OFFSET      24

//  Struct Size 20 in decimal
struct TR_POSITION
{
    DWORD   dwWestToEast;
    DWORD   dwVertical;
    DWORD   dwSouthToNorth;
    //  0 : ?
    //  1 : Orientation 0x0000 = facing white 0xffff facing red (North)
    //  2 : ??
    //  3 : Room
    WORD    wSomething1;
    WORD    wOrientation;
    WORD    wSomething2;
    WORD    wRoom;
};

//  Struct Size 12 in decimal
struct          TRLife
{
    WORD  iOne;     /* Must Be 0x0002 */
    WORD  iTwo;     /* Must Be 0x0002 */
    WORD  cFiller1;  
    WORD  iThree;   /* Must Be 0x0067 */
    WORD  cFiller2;
    WORD  iLife;    /* This is the Life */
};
typedef struct TRLife   TRLIFE;

#define MAX_SAVELENGTH  (2*1024*1024)

#pragma pack(pop, pack1)

//
//      Weapons Order
//      iMaskCompass            : Weapon0
//      iMaskPistol             : Weapon1
//      iMaskDesertEagle        : Weapon2
//      iMaskUzi                : Weapon3
//      iMaskShotGun            : Weapon4
//      iMaskMP5                : Weapon5
//      iMaskRocket             : Weapon6
//      iMaskGrenade            : Weapon7
//      iMaskHarpoon            : Weapon8

class CTRSaveGame  : public CObject
{
    DECLARE_DYNAMIC(CTRSaveGame)

    protected:
        CTRSaveGame();
        void Reset();
        static void StaticReset();

    public:
        virtual ~CTRSaveGame();

    // Attributes
    public:

    protected :
        char                m_Filename [ MAX_PATH ];
        char                m_FilenameBak [ MAX_PATH ];
        char                m_Status [ 256 ];

        static char         m_szBuffer [ MAX_SAVELENGTH ];
        static char         m_szBufferBackup [ MAX_SAVELENGTH ];

        //  Weapons
        WORD                m_iHarpoon;
        WORD                m_iCrossbow;
        WORD                m_iRockets;
        WORD                m_iGrenades;
        WORD                m_iHK;
        WORD                m_iMP5;
        WORD                m_iRiotGun;
        WORD                m_iShotgun;
        WORD                m_iUzis;
        WORD                m_iDesertEagle;
        WORD                m_iRevolver;
        WORD                m_iGunAmmos;

        int                 m_iLife;
        TRLIFE              *m_pLife;

        unsigned            m_iSaveLength;
        int                 m_iMaxLevel;

        //  Version 10, 15, 20, 30, 40, 49, 50
        int                 m_iVersion;
        //  Subversion 0, 5, 9
        int                 m_iSubVersion;

    // Operations
    public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTRSaveGame)
    public:
    //}}AFX_VIRTUAL

    // Implementation
    protected :
        static CTRSaveGame *m_pInstance;

    public:
        //
        static CTRSaveGame *GetInstance(BOOL bCreate = TRUE);
        static CTRSaveGame *I(BOOL bCreate = TRUE);
        static CTRSaveGame *GetTR1Instance();
        static CTRSaveGame *GetTUBInstance();
        static CTRSaveGame *GetTR2Instance();
        static CTRSaveGame *GetTR3Instance();
        static CTRSaveGame *GetTR4Instance();
        static CTRSaveGame *GetTR4NGInstance();
        static CTRSaveGame *GetTR5Instance();

        //
        static BOOL IsValid();
        static BOOL IsBufferModified();
        static int GetVersion ();
        static int GetFullVersion ();
        static int GetLevelIndex();
        static int GetLevel();
        static void WriteSaveGame();
        static void Discard();
        static char *GetBufferAddress();
        static char *GetBufferBackupAddress();
        static DWORD RelativeAddress( const void *pAddress );

        virtual const char *GetStatus() { return m_Status; };

        virtual int getFullVersion ()
        {
            if ( m_iVersion >= 10 )
            {
                return ( m_iVersion / 10 ) * 10 + m_iSubVersion;
            }
            else
            {
                return m_iVersion * 10 + m_iSubVersion;
            }
        };

        virtual char *getBufferAddress () = 0;
        virtual char *getBufferBackupAddress () = 0;
        virtual size_t getBufferLength() = 0;
        virtual void SetBufferLength(size_t len) { m_iSaveLength = (unsigned) len; };

        virtual TRLIFE *GetLifeAddress ();
        virtual int GetLife ();
        virtual void SetLife ( const char *szLife );
        virtual void SetLife ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetLife ( szText );
        }

        virtual int GetMap1 () = 0;
        virtual int GetMap2 () = 0;

        virtual int GetItem1 () = 0;
        virtual int GetItem2 () = 0;
        virtual int GetItem3 () = 0;
        virtual int GetItem4 () = 0;

        virtual int GetKey1 () = 0;
        virtual int GetKey2 () = 0;
        virtual int GetKey3 () = 0;
        virtual int GetKey4 () = 0;

        virtual int GetTreasure1 ( int iX ) = 0;
        virtual int GetTreasure2 ( int iX ) = 0;
        virtual int GetTreasure3 ( int iX ) = 0;
        virtual int GetTreasure4 ( int iX ) = 0;

        virtual void SetMap1 ( char *szString ) = 0;
        virtual void SetMap1 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetMap1 ( szText );
        }

        virtual void SetMap2 ( char *szString ) = 0;
        virtual void SetMap2 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetMap2 ( szText );
        }

        //  For TR4
        virtual BOOL HasItems ( ) { return FALSE; };
        virtual void SetItems ( int item, BYTE value ) { return; };
        virtual BYTE GetItems ( int item ) { return 0; };

        //
        virtual void SetItem1 ( char *szString ) = 0;
        virtual void SetItem1 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetItem1 ( szText );
        }

        virtual void SetItem2 ( char *szString ) = 0;
        virtual void SetItem2 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetItem2 ( szText );
        }

        virtual void SetItem3 ( char *szString ) = 0;
        virtual void SetItem3 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetItem3 ( szText );
        }

        virtual void SetItem4 ( char *szString ) = 0;
        virtual void SetItem4 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetItem4 ( szText );
        }

        virtual void SetKey1 ( char *szString ) = 0;
        virtual void SetKey1 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetKey1 ( szText );
        }

        virtual void SetKey2 ( char *szString ) = 0;
        virtual void SetKey2 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetKey2 ( szText );
        }

        virtual void SetKey3 ( char *szString ) = 0;
        virtual void SetKey3 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetKey3 ( szText );
        }

        virtual void SetKey4 ( char *szString ) = 0;
        virtual void SetKey4 ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetKey4 ( szText );
        }

        virtual void SetTreasure1 ( char *szString, int iX ) = 0;
        virtual void SetTreasure1 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetTreasure1 ( szText, iX );
        }

        virtual void SetTreasure2 ( char *szString, int iX ) = 0;
        virtual void SetTreasure2 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetTreasure2 ( szText, iX );
        }

        virtual void SetTreasure3 ( char *szString, int iX ) = 0;
        virtual void SetTreasure3 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetTreasure3 ( szText, iX );
        }

        virtual void SetTreasure4 ( char *szString, int iX ) = 0;
        virtual void SetTreasure4 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetTreasure4 ( szText, iX );
        }

        virtual void SetAllObjects () = 0;

        virtual int IsLaraBurning () = 0;
        virtual void SetLaraBurning ( int state = -1 ) = 0;
        virtual void SetLaraNoBurning ( int state = -1 ) = 0;

        virtual char *GetSaveName () = 0;
        virtual int GetSaveNumber () = 0;

        virtual int GetCurrentSecrets ( ) = 0;
        virtual void SetCurrentSecrets ( char *szString, int iX = -1 ) = 0;
        virtual void SetAllSecrets ( ) = 0;
        virtual int GetMaximumSecrets ( int iX ) = 0;

        virtual int GetUnlimitedAmmos() = 0;
        virtual void SetUnlimitedAmmos ( int iMask ) = 0;

        virtual int GetLaraState ( ) { return 0; };
        virtual void SetLaraState ( int state ) { };

        virtual int GetGunAmmos ( ) = 0;
        virtual int GetSmallMedipak ( int iX ) = 0;
        virtual int GetLargeMedipak ( int iX ) = 0;
        virtual int GetAir ( ) = 0;
        virtual int GetFlares ( int iX ) = 0;
        virtual int GetLaser ( int iX = 0 ) { return 0; };
        virtual int GetBinocular ( int iX = 0 ) { return 0; };

        //
        virtual void SetGunAmmos ( const char *szGunAmmos ) = 0;
        virtual void SetGunAmmos ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetGunAmmos ( szText );
        }

        virtual void SetSmallMedipak ( const char *szString, int iX ) = 0;
        virtual void SetSmallMedipak ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetSmallMedipak ( szText, iX );
        }

        virtual void SetLargeMedipak ( const char *szString, int iX ) = 0;
        virtual void SetLargeMedipak ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetLargeMedipak ( szText, iX );
        }

        virtual void SetAir ( const char *szString ) = 0;
        virtual void SetAir ( int val )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAir ( szText );
        }
        virtual void SetFlares ( const char *szString, int iX ) = 0;
        virtual void SetFlares ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetFlares ( szText, iX );
        }

        virtual void SetLaser ( char *szString, int iX = 0 ) { };
        virtual void SetLaser ( int val, int iX = 0 )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetLaser ( szText, iX );
        }

        virtual void SetBinocular ( char *szString, int iX = 0 ) { };
        virtual void SetBinocular ( int val, int iX = 0 )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetBinocular ( szText, iX );
        }

        //  Compass
        virtual BYTE GrabWeapon0 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon0 () {  return TRUE; };
        virtual BOOL HasAmmos0 () {  return FALSE; };

        //  Compass
        virtual const char *GetLabel0() {  return "Compass"; };
        virtual UINT GetBitmap0 ( ) { return IDB_CHRONO; };

        //  Pistols / Guns
        virtual BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon1 () {  return TRUE; };
        virtual BOOL HasAmmos1 () {  return TRUE; };

        //  Pistols / Guns
        virtual const char *GetLabel1() {  return "Guns / Pistols"; };
        virtual UINT GetBitmap1 ( ) { return IDB_GUNS; };
        virtual int CheckWeapon1 ( int iX ) = 0;

        //  Desert Eagle
        virtual BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon2 () {  return TRUE; };
        virtual BOOL HasAmmos2 () {  return TRUE; };
        //  Desert Eagle
        virtual const char *GetLabel2() {  return "Desert Eagle"; };
        virtual UINT GetBitmap2 ( ) { return IDB_DESERT_EAGLE; };
        virtual int CheckWeapon2 ( int iX ) = 0;

        //  Uzi
        virtual BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true ) = 0;
        virtual BOOL HasWeapon3 () {  return TRUE; };
        virtual BOOL HasAmmos3() {  return TRUE; };

        //  Uzi
        virtual const char *GetLabel3() {  return "Uzis"; };
        virtual UINT GetBitmap3 ( ) { return IDB_UZIS; };
        virtual int CheckWeapon3 ( int iX ) = 0;

        //  Shotgun
        virtual BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon4 () {  return TRUE; };
        virtual BOOL HasAmmos4 () {  return TRUE; };

        //  Shotgun
        virtual const char *GetLabel4() {  return "Shotgun"; };
        virtual UINT GetBitmap4 ( ) { return IDB_RIOT_GUN; };
        virtual int CheckWeapon4 ( int iX ) = 0;

        //  MP5
        virtual BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon5 () {  return TRUE; };
        virtual BOOL HasAmmos5 () {  return TRUE; };

        //  MP5
        virtual const char *GetLabel5() {  return "MP5"; };
        virtual UINT GetBitmap5 ( ) { return IDB_MP5; };
        virtual int CheckWeapon5 ( int iX ) = 0;

        //  Rocket
        virtual BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon6 () {  return TRUE; };
        virtual BOOL HasAmmos6 () {  return TRUE; };

        //  Rocket
        virtual const char *GetLabel6() {  return "Rocket Launcher"; };
        virtual UINT GetBitmap6 ( ) { return IDB_ROCKET_LAUNCHER; };
        virtual int CheckWeapon6 ( int iX ) = 0;

        //  Grenade
        virtual BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon7 () {  return TRUE; };
        virtual BOOL HasAmmos7 () {  return TRUE; };

        //  Grenade
        virtual const char *GetLabel7() {  return "Grenade Launcher"; };
        virtual UINT GetBitmap7 ( ) { return IDB_GRENADE_LAUNCHER; };
        virtual int CheckWeapon7 ( int iX ) = 0;

        //  Harpoon
        virtual BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true  ) = 0;
        virtual BOOL HasWeapon8 () {  return TRUE; };
        virtual BOOL HasAmmos8 () {  return TRUE; };

        //  Harpoon
        virtual const char *GetLabel8() {  return "Harpoon"; };
        virtual UINT GetBitmap8 ( ) { return IDB_HARPOON; };
        virtual int CheckWeapon8 ( int iX ) = 0;

        //  Headset / Crowbar
        virtual BYTE GrabWeapon9 ( int iX, bool bAdd = true, bool bChange = true  )  { return 0; };
        virtual BOOL HasWeapon9 () {  return FALSE; };
        virtual BOOL HasAmmos9 () {  return TRUE; };

        //  Headset / Crowbar
        virtual const char *GetLabel9() {  return "Headset / Crowbar"; };
        virtual UINT GetBitmap9 ( ) { return IDB_CROWBAR; };
        virtual int CheckWeapon9 ( int iX ) { return 0; };

        //  Guns
        virtual UINT GetAmmoBitmap1 ( ) { return IDB_AMMOS_GUNS; };
        virtual int GetAmmos1( int iX )
        {
            return 0;
        }

        virtual void SetAmmos1 ( const char *szString, int iX )
        {
            return;
        }

        virtual void SetAmmos1 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos1 ( szText, iX );
        }

        //  Desert Eagle
        virtual int GetAmmos2( int iX ) = 0;
        virtual UINT GetAmmoBitmap2 ( ) { return IDB_AMMOS_DESERT_EAGLE; };

        virtual void SetAmmos2 ( const char *szString, int iX ) = 0;
        virtual void SetAmmos2 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos2 ( szText, iX );
        }

        //  Uzis
        virtual int GetAmmos3( int iX ) = 0;
        virtual UINT GetAmmoBitmap3 ( ) { return IDB_AMMOS_UZIS; };

        virtual void SetAmmos3 ( const char *szString, int iX ) = 0;
        virtual void SetAmmos3 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos3 ( szText, iX );
        }

        //  Shotgun
        virtual int GetAmmos4a ( int iX ) = 0;
        virtual int GetAmmos4b ( int iX ) { return 0; };
        virtual UINT GetAmmoBitmap4 ( ) { return IDB_AMMOS_RIOTGUN; };

        //
        virtual void SetAmmos4a ( const char *szString, int iX ) = 0;
        virtual void SetAmmos4a ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos4a ( szText, iX );
        }

        virtual void SetAmmos4b ( const char *szString, int iX ) {};
        virtual void SetAmmos4b ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos4b ( szText, iX );
        }

        //  MP5
        virtual int GetAmmos5 ( int iX ) = 0;
        virtual UINT GetAmmoBitmap5 ( ) { return IDB_AMMOS_MP5; };

        virtual void SetAmmos5 ( const char *szString, int iX ) = 0;
        virtual void SetAmmos5 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos5 ( szText, iX );
        }

        //  Rocket
        virtual int GetAmmos6( int iX ) = 0;
        virtual UINT GetAmmoBitmap6 ( ) { return IDB_AMMOS_ROCKET; };

        virtual void SetAmmos6 ( const char *szString, int iX ) = 0;
        virtual void SetAmmos6 ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos6 ( szText, iX );
        }

        //  Grenades
        virtual int GetAmmos7a( int iX ) = 0;
        virtual int GetAmmos7b( int iX ) { return 0; };
        virtual int GetAmmos7c( int iX ) { return 0; };
        virtual UINT GetAmmoBitmap7 ( ) { return IDB_AMMOS_GRENADES; };

        virtual void SetAmmos7a ( const char *szString, int iX ) = 0;
        virtual void SetAmmos7a ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos7a ( szText, iX );
        }
        virtual void SetAmmos7b ( const char *szString, int iX ) {};
        virtual void SetAmmos7b ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos7b ( szText, iX );
        }
        virtual void SetAmmos7c ( const char *szString, int iX ) {};
        virtual void SetAmmos7c ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos7c ( szText, iX );
        }

        //  Harpoon
        virtual int GetAmmos8a( int iX ) = 0;
        virtual int GetAmmos8b( int iX ) { return 0; };
        virtual int GetAmmos8c( int iX ) { return 0; };
        virtual UINT GetAmmoBitmap8 ( ) { return IDB_AMMOS_HARPOON; };

        //
        virtual void SetAmmos8a ( const char *szString, int iX ) = 0;
        virtual void SetAmmos8a ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos8a ( szText, iX );
        }
        virtual void SetAmmos8b ( const char *szString, int iX ) {};
        virtual void SetAmmos8b ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos8b ( szText, iX );
        }
        virtual void SetAmmos8c ( const char *szString, int iX ) {};
        virtual void SetAmmos8c ( int val, int iX )
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "%d", val );
            SetAmmos8c ( szText, iX );
        }

        //
        virtual int Valid () = 0;
        virtual void SetInvalid () = 0;

        virtual BOOL BufferModified() = 0;
        virtual void discard() = 0;

        virtual int getLevelIndex() = 0;
        virtual int getLevel() = 0;
        virtual int GetSubVersion() { return m_iSubVersion; };
        virtual void GetDetailedInfo (  char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile ) = 0;
        virtual void writeSaveGame() = 0;
        virtual void Backup_Savegame();
        virtual int ReadSavegame( const char *pFilename ) = 0;
        virtual void RetrieveInformation( const char *pFilename ) = 0;
        virtual void RetrieveHeader() = 0;
        virtual void GetAmmosValues ( ) = 0;
        virtual int ConvertSecretBack ( int iSecret ) = 0;
        virtual int ConvertSecret ( int iSecret ) = 0;

        virtual int GetInfo ( char *szGame, size_t iSize, int *iGame, int *iLevel, char *szTitle, size_t iSizeTile );

        virtual const TR_POSITION *GetPosition ( ) = 0;
        virtual BOOL SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom ) = 0;

        static int InstanciateVersion ( const char *szFilename );


        // Generated message map functions
};

