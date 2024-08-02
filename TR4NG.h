#pragma once


//
/////////////////////////////////////////////////////////////////////////////
//  TRNG Specific Structure
//  Start at 0x8000
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push, pack1, 1)

#define NGTAG_END_SEQUENCE              0x0000

#define NGTAG_NON_USATO                 0x8001
#define NGTAG_ANIMATED_TEXTURES         0x8002
#define NGTAG_OLD_EFFECTS               0x8003
#define NGTAG_OLD_FMV                   0x8004
#define NGTAG_REMAP_OBJ                 0x8005
#define NGTAG_SALVA_COORDINATE          0x8006
#define NGTAG_PROGR_ACTIONS             0x8007
#define NGTAG_OLD_ACTIONS               0x8008
#define NGTAG_EXTRA_INFO_ROOMS          0x8009
#define NGTAG_LANGUAGE_STRINGS          0x800A
#define NGTAG_SCRIPT_OPTIONS            0x800B
#define NGTAG_SCRIPT_LEVEL              0x800C
#define NGTAG_FLAG_LEVEL_TR4            0x800D
#define NGTAG_OLD_CONDITION             0x800E
#define NGTAG_VARIABLE_DATA             0x800F
#define NGTAG_PUSH_CLIMB                0x8010
#define NGTAG_PRINT_STRING              0x8011
#define NGTAG_ELEVATORS                 0x8012
#define NGTAG_MIRRORS                   0x8013
#define NGTAG_BLIND_SAVE                0x8014
#define NGTAG_CASUALE                   0x8015
#define NGTAG_CONTROLLO_OPTIONS         0x8016
#define NGTAG_TEX_PARZIALE              0x8017
#define NGTAG_REMAP_TAILS               0x8018
#define NGTAG_SWAP_MESH                 0x8019
#define NGTAG_STATUS_GTRIGGERS          0x801A
#define NGTAG_ROOM_FLAGS                0x801C
#define NGTAG_WEATHER_INTENSITY         0x801D
#define NGTAG_STATUS_ORGANIZER          0x801E
#define NGTAG_IMPORT_FILE               0x801F
#define NGTAG_INDICI_PFRAME             0x8020
#define NGTAG_REMAP_STATICS             0x8021
#define NGTAG_SALVA_STATICS             0x8022
#define NGTAG_SALVA_TIMER_OGGETTI       0x8023
#define NGTAG_VERSION_HEADER            0x8024
#define NGTAG_TOM_VERSION               0x8025
#define NGTAG_STATUS_ANIM_RANGES        0x8030
#define NGTAG_SAVEGAME_INFOS            0x8031
#define NGTAG_MINI_SHOT                 0x8032
#define NGTAG_LEVEL_NAMES               0x8033
#define NGTAG_NG_HUB_HEADERS            0x8035
#define NGTAG_VAR_DATA_LARA             0x8036
#define NGTAG_VET_REMAP_ROOMS           0x8037
#define NGTAG_VAR_GLOBAL_TRNG           0x8038
#define NGTAG_VAR_LOCAL_TRNG            0x8039
#define NGTAG_FROZEN_ITEMS              0x803A
#define NGTAG_SAVE_LOCUST               0x803B
#define NGTAG_NO_COLL_ITEMS             0x803C
#define NGTAG_SLOT_NAMES                0x803D
#define NGTAG_FISH                      0x803E
#define NGTAG_STATUS_TRIGGER_GROUP      0x803F
#define NGTAG_ADAPTIVE_FARVIEW          0x8040
#define NGTAG_KAYAK_EXTRA_DATA          0x8041
#define NGTAG_ASSIGNED_SLOT             0x8042
#define NGTAG_DIARY_DATA                0x8043
#define NGTAG_ANIM_SWAPPING             0x8044
#define NGTAG_FLIP_MESH                 0x8045
#define NGTAG_PLUGIN_DATA               0x8046
#define NGTAG_PLUGIN_NAMES_NGLE         0x8047
#define NGTAG_PLUGIN_ID_FLOOR_TABLE     0x8048
#define NGTAG_EXTRA_AI_RECORDS          0x8049
#define NGTAG_CUTSCENE_CAMERA           0x804A
#define NGTAG_ACTORS_INDICES            0x804B
#define NGTAG_SLOT_FLAGS_ARRAY          0x804C
#define NGTAG_OCB_ITEMS                 0x804D
#define NGTAG_REMAP_PLUGIN_IDS          0x804E
#define NGTAG_SWAP_SINGLE_MESH          0x804F
#define NGTAG_NEW_CAMERAS               0x8050
#define NGTAG_SLOT_INFO                 0x8051

//  We Will ignore large structure
//  That are DWORD length
typedef struct  TRNGIteration
{
    WORD    length;         //  Length of TR4NGRepeatable in words;
    WORD    codeOp;         //  0x80xx
    WORD    values[1];      //  values [ length - 2 * sizeof(WORD) ];
} TRNGITERATION;

typedef struct TRNGSpecific
{
    BYTE            signature [2];  //  NG
    TRNGITERATION   iteration;
} TRNGSPECIFIC;

//  NGTAG_SAVEGAME_INFOS 0x8031 // savegame
typedef struct TRNGSaveInfos {
	WORD    Flags; // FSAV_ ... flag per savegame
	WORD    ShatterInizio; // primo slot statico per shatter
	WORD    ShatterFine;  // ultimo slot statico per shatter
	WORD    LaraStateId; // state id di lara
	short   LaraVitality; // vitalita' di lara
	char    Tr4Name[32]; // solonome di file .tr4 a cui si lega questo savegame
	short   Veicolo; // VEC_ ... oppure -1 se non e' su un veicolo
	BYTE    Copy_057_0F3[0x9d]; // copia prima parte savegame
	BYTE    Copy_169_1AC[0x44]; // copia secondo blocco savegame
	WORD    FlagsRoom; // flag di stanza dove si trova lara
	DWORD   OffsetLara; // offset in inizio savegae di dove parte strutturalara
	BYTE    SegretiTrovati_1FB; // bytre di segreti 
	BYTE    NonUsato;
	DWORD   Distanza;
	BYTE    ByteAlign;
	WORD    Reserved[114]; // ulteriori sviluppi futuri
} TRNGSAVEGAMEINFOS;


// struttura Item per tomb4 and for lara
typedef struct TRNGSaveCoord {
	WORD    OrientingH;
	WORD    OrientingV;
	WORD    FlagInvisibile;
	DWORD   CordX;
	int     CordY;
	DWORD   CordZ;
	WORD    Room;
} TRNGSAVECOORD;

typedef struct TRNGBaseSalvaCords {
	WORD            TotSalvati;
	WORD            VetIndici[1];
	TRNGSaveCoord   VetSalvati[1];
} TRNGBASESAVECOORD;

typedef struct TRNGInventoryItems {
	BYTE WeaponPistols;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponUZI;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponShotGun;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponCrossBow;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponGrenadeGun;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponRevolver;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE LaserSight;		//  0 / 1
	BYTE Binoculars;		//  0 / 1
	BYTE Crowbar;		//  0 / 1
	BYTE ClockWorkBeetle;		//  CBEET_ values to test with & operator (bit flags)
	BYTE WaterSkinSmall;		//  WSKIN_ values to test in mixed way, sometimes with == and other flag with &
	BYTE WaterSkinLarge;		//  WSKIN_ values to test in mixed way, sometimes with == and other flag with &
	BYTE ExamineItem1;		//  0 / 1
	BYTE ExamineItem2;		//  0 / 1
	BYTE ExamineItem3;		//  0 / 1
	BYTE PuzzleItem1;		//  quantity
	BYTE PuzzleItem2;		//  quantity
	BYTE PuzzleItem3;		//  quantity
	BYTE PuzzleItem4;		//  quantity
	BYTE PuzzleItem5;		//  quantity
	BYTE PuzzleItem6;		//  quantity
	BYTE PuzzleItem7;		//  quantity
	BYTE PuzzleItem8;		//  quantity
	BYTE PuzzleItem9;		//  quantity
	BYTE PuzzleItem10;		//  quantity
	BYTE PuzzleItem11;		//  quantity
	BYTE PuzzleItem12;		//  quantity
	WORD ComboItems;		//  COMBO_ values to test with "&" operator (bit flags)
	WORD Keys;				//  EX16_ values to test with "&" operator (bit flags) 
	WORD KeyCombo;			//  KCOMBO_ values to test with "&" operator (bit flags)
	short PickupItems;		//  EX16_ values to test with "&" operator (bit flags) 
	short PickupCombo;		//  PCOMBO_ values to test with "&" operator (bit flags) 
	short QuestItems;		//  EX16_ values to test with "&" operator (bit flags) 
	short MediPackSmall;		//  quantity (-1 = unlimited)
	short MediPackLarge;		// quantity (-1 = unlimited)
	short Flares;		// quantity (-1 = unlimited)
	short AmmoPistols;		// quantity (-1 = unlimited)
	short AmmoUZI;		//  quantity (-1 = unlimited)
	short AmmoRevolver;		// quantity (-1 = unlimited)
	short AmmoShotgunNormals;		//  quantity multiple by 6 (-1 = unlimited)
	short AmmoShotgunWideShot;		//  quantity multiple by 6 (-1 = unlimited)
	short AmmoGrenadeNormals;		// quantity (-1 = unlimited)
	short AmmoGrenadeSuper;		//  quantity (-1 = unlimited)
	short AmmoGrenadeFlash;		// quantity (-1 = unlimited)
	short AmmoCrossBowNormals;		// quantity (-1 = unlimited)
	short AmmoCrossBowPoison;		// quantity (-1 = unlimited)
	short AmmoCrossBowExplosive;		// quantity (-1 = unlimited)
	BYTE ClockWorkBeetleAvailUsage;		// available times to use it. When it becomes 0 it will explode but it is 0 also when beetle has not yet been picked up

} TRNGINVENTORYITEMS;

//
typedef struct TRNGItemTr4 {
	int HeightFloor;		// 00
	int ContactFlags;		// 04  (oggetto toccato lara (darts))
	DWORD MeshVisibilityMask; // 08
	WORD SlotID;			// 0C
	WORD StateIdCurrent;	    // 0E
	WORD StateIdNext;			// 10
	WORD StateIdAI;				// 12
	WORD AnimationNow;		    // 14
	WORD FrameNow;			    // 16
	short Room;					// 18
	short ItemIndexNext;	    // 1A
	short ItemIndexPrevious;     // 1C
	short SpeedH;			// 1E
	short SpeedV;			// 20
	short Health;				// 22
	WORD BoxZoneIndex;			// 24
	short ObjectTimer;			// 26
	WORD Objectbuttons;	// 28
	WORD Intensity1;			// 2a
	WORD OcbCode;				// 2c
	BYTE IdSprite1;				// 2e
	BYTE IdSprite2;				// 2f
	WORD Trasparency;			// 30   trasparenza
	WORD Mistery1;				// 32
	short Reserved_34;		    // 34
	short Reserved_36;				// 36
	short Reserved_38;				// 38
	short Reserved_3A;				// 3A
	void *pZonaSavegame;		// 3C	or pCreatureInfo structure for enemies				
	DWORD  CordX;					// 40
	int  CordY;					// 44
	DWORD  CordZ;					// 48
	short OrientationV;  // 4c
	short OrientationH;  // 4e
	short OrientationT;		// 50
	DWORD LightRed;		//52
	DWORD LightGreen;		// 56
	DWORD LightBlue;		// 5A
	DWORD LightRGB;		// 5E
	DWORD LightDifferenceRed;		// 62
	DWORD LightDifferenceGreen;		// 66
	DWORD LightDifferenceBlue;		// 6A
	DWORD LightChangeDegrees;		// 6E
	BYTE Zone1[2730];		// 72
	BYTE Zone2[2758];		// B1C
	BYTE *pZone1;		// 15e2
	BYTE *pZone2;		// 15e6
	DWORD FlagsMain;		// 15ea (FITEM_.. flags)
	DWORD FlagsSwapMesh;	// 15ee
	DWORD Mistery5;		// 15f2
} TRNGITEMS4;   // size 0x15f6

typedef struct TRNGCercaStatic {
	WORD    IndiceRoom;
	short   IndiceStatic; // interno a stanza
} TRNGCERCASTATIC;

typedef struct TRNGSalvaStatic {
	TRNGCercaStatic Indici;
	WORD            Flags;
	int             OrgX;
	int             OrgY;
	int             OrgZ;
	WORD            Orient;
	WORD            Colore;
} TRNGSALVASTATIC;

typedef struct TRNGText80 {
	char Text[80];
} TRNGTEXT80;

typedef struct TRNGTwoShort {
	short s1;
	short s2;
} TRNGTWOSHORT;

typedef struct TRNGFourBytes {
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE b4;
} TRNGFOURBYTES;

typedef struct TRNGGroupLongVar {
	union {
		int Long;
		union {
			short Short1;
			short Short2;
		};
		union {
			BYTE Byte1;
			BYTE Byte2;
			BYTE Byte3;
			BYTE Byte4;
		};
	};
} TRNGGROUPLONGVAR;

typedef struct TRNGVariablesNames {
	TRNGGroupLongVar Alfa;
	TRNGGroupLongVar Beta;
	TRNGGroupLongVar Delta;
	int Timer;

} TRNGVARIABLESNAMES;

typedef struct TRNGBlockNum {
	union {
		int VetNumeriLong[4];
		short VetNumeriShort[8];
		BYTE VetNumeriByte[16];
		TRNGVariablesNames Name;
	};
} TRNGBLOCKNUM;

typedef struct TRNGVariabiliGlob {
	// variabili numeriche

	TRNGBlockNum NumWar;
	// variabili di testo
	TRNGText80 VetTextVar[4];


	// variabili store per savegame
	union {
		int VetStoreLong[16];
		short VetStoreShort[32];
		BYTE VetStoreByte[64];
	};

	// variabili speciali
	char LastInputText[80];
	int LastInputNumber;
	int CurrentValue;
	char TextBig[320];
	// valori per usi futuri
	DWORD VetExtra[20];

} TRNGGLOBALVARIABLES;

//
typedef struct TRNGVersionHeader {
	WORD VetVersione[4];
	WORD Flags;	// VER_... flags

} TRNGVERSIONHEADER;

#pragma pack(pop, pack1)
