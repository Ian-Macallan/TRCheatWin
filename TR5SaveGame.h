#if !defined(AFX_TR5SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)
#define AFX_TR5SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_

#include "TRSaveGame.h"

#include "TRXTools.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
/////////////////////////////////////////////////////////////////////////////
// TR5SaveGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTR5SaveGame document
//
/////////////////////////////////////////////////////////////////////////////
/*
 *      ------------------------------------------------
 *      Definition.
 *      ------------------------------------------------
 */
#define TR5MAXLEVEL			35
#define TR5BUFFEREND		14014
#define	TR5LEVELSIZE		7743

//	00000001
#define	TR50_GUN_SET1		0x01	
//	00000100
#define	TR50_GUN_SET4		0x04
//	00001000
#define	TR50_GUN_SET8		0x08
//	00001100
#define	TR50_GUN_SETC		0x0C
//	00001001
#define	TR50_GUN_SET		(TR50_GUN_SET1|TR50_GUN_SET8)
//	00001101
#define	TR50_GUN_MASK		(TR50_GUN_SET1|TR50_GUN_SET4|TR50_GUN_SET8|TR50_GUN_SETC)

//
/*
 *      ------------------------------------------------
 *      Structures.
 *      ------------------------------------------------
 */
#pragma pack(push, pack1, 1)

//	Position is Variable
//	The room may be 82 (dec) before the 02 02 xx 67
//	This structure is 8 before the 02 02 xx  67
#define	TR5_POSITION_OFFSET		11
#define	TR5_LIFE_OFFSET			7
#define	TR5_FACTOR				2L

struct TR5_POSITION
{
	BYTE	cMoving1;			//	0000 Change when moving
	BYTE	unknown1;			//	0001
	WORD	wWestToEast;		//	0002-0003 Change when moving East West
	short	wVertical;			//	0004-0005 
	WORD	wSouthToNorth;		//	0006-0007 Change when moving South North
	BYTE	cRoom;				//	0008 
	BYTE	unknown2;			//	0009
	BYTE	cOrientation;		//	000a 
	BYTE	indicator1;			//	000b eg 0x02
	BYTE	indicator2;			//	000c eg 0x02
	BYTE	indicator3;			//	000d
	BYTE	indicator4;			//	000e eg 0x67
	BYTE	unknown3;			//	000f
	BYTE	cMoving2;			//	0010	Change when moving
	BYTE	unknown4;
	WORD	heath;				//	0012
};


struct          TR5Gun
{
	//	0x16F : 09
	BYTE		m_gunPistol;		//	1

	//	0x170 : 09
    BYTE		m_gunUzis;			//	2

	//	0x171 : 09		// 0x661 0x20 to 0x00 make uzi appears at feet
    BYTE		m_gunRiotGun;		//	3

	//	0x172 : 09		Desert Eagle or Grappling
    BYTE		m_gunDesertEagle;	//	4

	//	0x173 : 09
    BYTE		m_gunHK;			//	5

	//	0x174 : 01
	BYTE		m_gunRevolver;		//	6

	//	0x175 : 01
	BYTE		m_gunLaserLight;	//	7

	//	0x176 : 01
	BYTE		m_gunBinocular;		//	8

	//	0x177 : 01
	BYTE		m_gunHeadSet;		//	9

	//	0x178
	union
	{
		BYTE		m_Object [ 28 ];
		struct Detailed
		{
			//	Address : 1 - 3 : 3 Object
			BYTE	m_Objects [ 3 ];

			//	Address : 4 - 6 : 3 Examine
			BYTE	m_Examine [ 3 ];

			//	Address : 7 - 18 : 12 Puzzle
			BYTE	m_Puzzle [ 12 ];

			//	19 - 20	: 16  (8 Puzzle Combo * 2)
			BYTE	m_PuzzleCombo [ 2 ];	//	Four by byte

			//	Address : 21 - 22	: 12 Keys
			//	Byte 1 : Keys 1 to 8
			//	Byte 2 : Keys 9 to 12
			BYTE	m_Key [ 2 ];			//	Six by byte

			//	Address : 23 - 24 : 16 (8 Key Combo * 2)
			//	8 Key Combo3
			BYTE	m_KeyCombo [ 2 ];		//	Three by byte

			//	Address : 25 - 26 : 4 Pickup
			BYTE	m_Pickup [ 2 ];			//	Four by byte

			//	Address : 27 - 28  : 8 (4 Pickup Combo * 2)
			BYTE	m_PickupCombo [ 2 ];	//	Four by byte
		};
	};

};
typedef struct TR5Gun	TR5GUN;

struct          TR5Ammo
{
	//	0x194
	WORD		 m_iSmallMedipak;

	//	0x196
	WORD		m_iLargeMedipak;

	//	0x198 / 0x199 : 01
	WORD		m_iFlares;

	//	0x19A
	WORD		m_iGunAmmos;

	//	0x19C
	WORD		m_iUziAmmos;

	//	0x19E
	WORD		m_iRevolverAmmos;

	//	0x1A0
	WORD		m_iShotGunAmmo1;

	//	0x1A2
	WORD		m_iShotGunAmmo2;

	//	0x1A4
	WORD		m_iHKAmmos;

	//	0x1A6
	WORD		m_iGrapplingAmmos;

	//
	BYTE		m_Filler2 [ 0x1D7 - 0x1A6 - sizeof(short) ];

	//	0x1D7
	BYTE		m_iSecretCount;

};
typedef struct TR5Ammo	TR5AMMO;

struct          TR5Save
{
	union
	{
		struct
		{
			char                    szSavename [ 24 ];
			char                    cFiller1 [ 51 ];

			//	0x4b / 0xc4c
			WORD					iSaveNumber;
		};

		struct
		{
			char                    cFiller1A [ 0x9E ];

			//		0x9e
			//		0x04	= normal
			//		0x0c	= burning
			BYTE					laraState;
		};

		struct
		{
			char                    cFiller1B [ 0x70 ];

			WORD					iAir;
		};

		struct
		{
			char                    cFiller2 [ 0x16f ];

			//		0x16f
			TR5GUN					tagGuns;
		};

		struct 
		{
			char                    cFiller3 [ 0x194 ];

			//		0x194
			TR5AMMO					tagAmmo;
		};

		struct 
		{
			char                    cFiller3a [ 0x1ec];

			//		0x1ec
			BYTE					m_cLevel;
			BYTE					m_cSub;
		};

		//	Health is variable even inside a level
		struct
		{

			char                    cFiller3b [ 0x4f4 ];
			//		
			WORD					m_iHealth;
		};

		// No checksum for TR V
		struct
		{
			char                    cFiller4 [ 7742 ];
			BYTE					checkSum;
		};
	};
};

typedef struct TR5Save TR5SAVE;

#pragma pack(pop, pack1)

class CTR5SaveGame : public CTRSaveGame
{
	DECLARE_DYNAMIC(CTR5SaveGame)

	friend class CTRSaveGame;

	protected:
		CTR5SaveGame();

	public:
		virtual ~CTR5SaveGame();


	// Attributes
	protected :
		TR5SAVE					*m_pBuffer;
		TR5SAVE					*m_pBufferBackup;

	protected :


		//	Mask
		BYTE					iMaskPistol;
		BYTE					iMaskUzi;
		BYTE					iMaskRiotGun;
		BYTE					iMaskDesertEagle;
		BYTE					iMaskHK;
		BYTE					iMaskRevolver;
		BYTE					iMaskLaser;
		BYTE					iMaskHeadSet;
		BYTE					iMaskBinocular;
		BYTE					iRiotGunUnits;
	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTR5SaveGame)
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

		void *GetIndicatorAddress ();
		WORD *GetTR5LifeAddress ();
		int GetLife ();
		void SetLife ( const char *szLife );

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

		int	IsLaraBurning ();
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
		int	GetAir ( );
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

		//	No Compass
		BYTE GrabWeapon0 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon0 () {  return FALSE; };
		virtual BOOL HasAmmos0 () {  return FALSE; };

		//	Pistol
		BYTE GrabWeapon1 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon1 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunPistol & TR50_GUN_MASK ) )
			{
				return TRUE;
			}
			return CTRXTools::IsEnablePistol( getFullVersion(), getLevel() );
		};
		virtual BOOL HasAmmos1 () { return TRUE; };

		//	1 Pistol
		virtual const char *GetLabel1() {  return "Guns / Pistols"; };
		virtual UINT GetBitmap1 ( ) { return IDB_GUNS ; };

		//	2 Revolver
		BYTE GrabWeapon2 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon2 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunRevolver & TR50_GUN_MASK ) )
			{
				return TRUE;
			}
			return CTRXTools::IsEnableMagnum( getFullVersion(), getLevel() );
		};
		virtual BOOL HasAmmos2 () { return CTRXTools::IsAmmosMagnum( getFullVersion(), getLevel() ); };

		//	Revolver
		virtual const char *GetLabel2() {  return "Revolver"; };
		virtual UINT GetBitmap2 ( ) { return IDB_REVOLVER; };

		//	3 Uzi
		BYTE GrabWeapon3 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon3 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunUzis & TR50_GUN_MASK ) )
			{
				return true;
			}
			return CTRXTools::IsEnableUzi ( getFullVersion(), getLevel() );
		};
		virtual BOOL HasAmmos3 () { return CTRXTools::IsAmmosUzi( getFullVersion(), getLevel() ); };
		//	Uzi
		virtual const char *GetLabel3() {  return "Uzi"; };
		virtual UINT GetBitmap3 ( ) { return IDB_UZIS; };

		//	4 Riot Gun
		BYTE GrabWeapon4 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon4 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunRiotGun & TR50_GUN_MASK ) )
			{
				return true;
			}
			return CTRXTools::IsEnableRiotgun( getFullVersion(), getLevel() );
		};
		virtual BOOL HasAmmos4 () { return CTRXTools::IsAmmosRiotgun( getFullVersion(), getLevel() ); };

		//	Riot Gun
		virtual const char *GetLabel4() {  return "Riot Gun"; };
		virtual UINT GetBitmap4 ( ) { return IDB_RIOT_GUN; };

		//	5 HK
		BYTE GrabWeapon5 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon5 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunHK & TR50_GUN_MASK ) )
			{
				return true;
			}
			return CTRXTools::IsEnableM16( getFullVersion(), getLevel() );
		};
		virtual BOOL HasAmmos5 () { return CTRXTools::IsAmmosM16( getFullVersion(), getLevel() ); };

		//	HK
		virtual const char *GetLabel5() {  return "HK"; };
		virtual UINT GetBitmap5 ( ) { return IDB_HK; };

		//	Harpoon
		//////////////
		BYTE GrabWeapon6 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon6 () { return FALSE; /* CTRXTools::IsEnableHarpoon( getFullVersion(), getLevel() ); */   };
		virtual BOOL HasAmmos6 () { return FALSE; /* CTRXTools::IsAmmosHarpoon( getFullVersion(), getLevel() ); */ };
		virtual const char *GetLabel6() {  return "Harpoon"; };

		//	6 Revolver
		virtual UINT GetBitmap6 ( ) { return IDB_HARPOON; };

		//
		BYTE GrabWeapon7 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon7 () { return FALSE; /* CTRXTools::IsEnableRocket( getFullVersion(), getLevel() ); */ };
		virtual BOOL HasAmmos7 () { return FALSE; /* CTRXTools::IsAmmosRocket( getFullVersion(), getLevel() ); */ };
		virtual const char *GetLabel7() {  return "Rocket"; };
		virtual UINT GetBitmap7 ( ) { return IDB_ROCKET_LAUNCHER; };

		//
		//	Grappling Gun
		BYTE GrabWeapon8 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon8 ()
		{
			if ( m_pBuffer != NULL && ( m_pBuffer->tagGuns.m_gunDesertEagle & TR50_GUN_MASK ) )
			{
				return TRUE;
			}
			return CTRXTools::IsEnableHarpoon( getFullVersion(), getLevel() );
		};

		virtual BOOL HasAmmos8 () { return CTRXTools::IsAmmosHarpoon( getFullVersion(), getLevel() ); };
		virtual const char *GetLabel8() {  return "Grappling Gun"; };
		virtual UINT GetBitmap8 ( ) { return IDB_GRAPPLING; };

		//	Headset / Crowbar
		virtual BYTE GrabWeapon9 ( int iX, bool bAdd = true, bool bChange = true );
		virtual BOOL HasWeapon9 () {  return TRUE; };
		virtual BOOL HasAmmos9 () {  return FALSE; };

		//	Headset / Crowbar
		virtual const char *GetLabel9() {  return "Headset"; };
		virtual int CheckWeapon9 ( int iX );


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
		virtual UINT GetAmmoBitmap5 ( ) { return IDB_AMMOS_HK; };
		void SetAmmos5 ( const char *szString, int iX );

		//
		int GetAmmos6( int iX );
		void SetAmmos6 ( const char *szString, int iX );
		virtual UINT GetAmmoBitmap6 ( ) { return IDB_AMMOS_HARPOON; };

		//
		int GetAmmos7a( int iX );
		int GetAmmos7b( int iX );
		int GetAmmos7c( int iX );
		void SetAmmos7a ( const char *szString, int iX );
		void SetAmmos7b ( const char *szString, int iX );
		void SetAmmos7c ( const char *szString, int iX );
		virtual UINT GetAmmoBitmap7 ( ) { return IDB_AMMOS_ROCKET; };

		//	Grappling
		int GetAmmos8a( int iX );
		int GetAmmos8b( int iX );
		int GetAmmos8c( int iX );
		virtual UINT GetAmmoBitmap8 ( ) { return IDB_AMMOS_GRAP; };
		void SetAmmos8a ( const char *szString, int iX );
		void SetAmmos8b ( const char *szString, int iX );
		void SetAmmos8c ( const char *szString, int iX );

		//
		virtual BOOL HasItems ( ) { return TRUE; };
		virtual void SetItems ( int item, BYTE value );
		virtual BYTE GetItems ( int item );

		//	Simple Check for version TR5
		int	Valid ();
		void SetInvalid ();

		virtual int getLevelIndex();
		virtual int getLevel();
		const char *GetStatus ();

		void GetDetailedInfo (	char *szGame, size_t iSize, int *iGame, int *iLevel,
								char *szTitle, size_t iSizeTile );
		void writeSaveGame();
		int ReadSavegame( const char *pFilename );
		void RetrieveInformation( const char *pFilename );
		TR5GUN *SearchGunStructure ( WORD m_iHealth, int *iPos );
		int CheckIfAmmosMatch ( TR5GUN *pGun, WORD gunBitmap );
		void RetrieveHeader();
		void GetAmmosValues ( );
		int ConvertSecretBack ( int iSecret );
		int ConvertSecret ( int iSecret );

		TR5_POSITION *GetTR5Position ( );
		const TR_POSITION *GetPosition ( );
		BOOL SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom );

	// Generated message map functions
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
// The savegame object.


#endif // !defined(AFX_TR5SAVEGAME_H__F8F73C01_A0DB_11D2_8146_444553540000__INCLUDED_)

