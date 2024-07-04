#include "StdAfx.h"

#include <windows.h>
#include <math.h>

#include "TR_Areas.h"

//
/////////////////////////////////////////////////////////////////////////////
//  Generated Areas
//
/////////////////////////////////////////////////////////////////////////////
#include "TR_Areas_inc.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

#define NAME_AND_AREA(lvl)      { #lvl, ##lvl,sizeof(lvl)/sizeof(TR_AREA) }
#define AREA_ONLY(lvl)          { ##lvl }
#define NAME_AREA(lvl)          NAME_AND_AREA(lvl)
//
/////////////////////////////////////////////////////////////////////////////
//  http://xproger.info/projects/OpenLara/trs.html
/////////////////////////////////////////////////////////////////////////////
static TRN_AREA TR1_AREAS [] =
{
    NAME_AREA(TR1_LEVEL1),          //  Caves
    NAME_AREA(TR1_LEVEL2),          //  City of Vilcabamba
    NAME_AREA(TR1_LEVEL3A),         //  The Lost Valley
    NAME_AREA(TR1_LEVEL3B),         //  Tomb of Qualopec
    NAME_AREA(TR1_LEVEL4),          //  St. Francis’ Folly
    NAME_AREA(TR1_LEVEL5),          //  Colosseum
    NAME_AREA(TR1_LEVEL6),          //  Palace Midas
    NAME_AREA(TR1_LEVEL7A),         //  Cistern
    NAME_AREA(TR1_LEVEL7B),         //  Tomb of Tihocan
    NAME_AREA(TR1_LEVEL8A),         //  City of Khamoon
    NAME_AREA(TR1_LEVEL8B),         //  Obelisk of Khamoon
    NAME_AREA(TR1_LEVEL8C),         //  Sanctuary of the Scion
    NAME_AREA(TR1_LEVEL10A),        //  Natla’s Mines
    NAME_AREA(TR1_LEVEL10B),        //  Atlantis
    NAME_AREA(TR1_LEVEL10C),        //  The Great Pyramid

    NAME_AREA(TR1G_EGYPT),          //  Return to Egypt
    NAME_AREA(TR1G_CAT),            //  Temple of the Cat
    NAME_AREA(TR1G_END),            //  Atlantean Stronghold
    NAME_AREA(TR1G_END2),           //  The Hive
    { NULL },

    NAME_AREA(TR1_TITLE),
    NAME_AREA(TR1_GYM),
    NAME_AREA(TR1_CUT1),
    NAME_AREA(TR1_CUT2),
    NAME_AREA(TR1_CUT3),
    NAME_AREA(TR1_CUT4),

    { NULL },
    { NULL },
};


static TRN_AREA TRR1_AREAS [] =
{
    NAME_AREA(TRR1_LEVEL1),         //  Caves
    NAME_AREA(TRR1_LEVEL2),         //  City of Vilcabamba
    NAME_AREA(TRR1_LEVEL3A),        //  The Lost Valley
    NAME_AREA(TRR1_LEVEL3B),        //  Tomb of Qualopec
    NAME_AREA(TRR1_LEVEL4),         //  St. Francis’ Folly
    NAME_AREA(TRR1_LEVEL5),         //  Colosseum
    NAME_AREA(TRR1_LEVEL6),         //  Palace Midas
    NAME_AREA(TRR1_LEVEL7A),        //  Cistern
    NAME_AREA(TRR1_LEVEL7B),        //  Tomb of Tihocan
    NAME_AREA(TRR1_LEVEL8A),        //  City of Khamoon
    NAME_AREA(TRR1_LEVEL8B),        //  Obelisk of Khamoon
    NAME_AREA(TRR1_LEVEL8C),        //  Sanctuary of the Scion
    NAME_AREA(TRR1_LEVEL10A),       //  Natla’s Mines
    NAME_AREA(TRR1_LEVEL10B),       //  Atlantis
    NAME_AREA(TRR1_LEVEL10C),       //  The Great Pyramid

    NAME_AREA(TRR1G_EGYPT),         //  Return to Egypt
    NAME_AREA(TRR1G_CAT),           //  Temple of the Cat
    NAME_AREA(TRR1G_END),           //  Atlantean Stronghold
    NAME_AREA(TRR1G_END2),          //  The Hive
    { NULL },

    NAME_AREA(TRR1_TITLE),
    NAME_AREA(TRR1_GYM),
    NAME_AREA(TRR1_CUT1),
    NAME_AREA(TRR1_CUT2),
    NAME_AREA(TRR1_CUT3),
    NAME_AREA(TRR1_CUT4),

    { NULL },
    { NULL },
};

//
/////////////////////////////////////////////////////////////////////////////
//  https://www.tombraidercie.com/jeux/tr2/niveaux/
//
/////////////////////////////////////////////////////////////////////////////
static TRN_AREA TR2_AREAS [] =
{
    NAME_AREA(TR2_WALL),            //  Great Wall

    NAME_AREA(TR2_BOAT),            //  Venice
    NAME_AREA(TR2_venice),          //  Bartoli's Hideout
    NAME_AREA(TR2_OPERA),           //  Opera House

    NAME_AREA(TR2_RIG),             // Offshore Rig
    NAME_AREA(TR2_platform),        // Diving Area
    NAME_AREA(TR2_unwater),         // 40 Fathoms
    NAME_AREA(TR2_keel),            // Wreck of the Maria Doria
    NAME_AREA(TR2_living),          // Living Quarters
    NAME_AREA(TR2_deck),            // The Deck

    NAME_AREA(TR2_SKIDOO),          // Tibetan Foothills
    NAME_AREA(TR2_MONASTRY),        // Barkhang Monastery
    NAME_AREA(TR2_catacomb),        // Catacombs of the Talion
    NAME_AREA(TR2_icecave),         // Ice Palace

    NAME_AREA(TR2_Emprtomb),        //  Temple of Xian
    NAME_AREA(TR2_FLOATING),        //  Floating Islands
    NAME_AREA(TR2_xian),            //  The Dragon's Lair

    NAME_AREA(TR2_house),           //  Home Sweet Home

    NAME_AREA(TR2G_level1),         //  The Cold War
    NAME_AREA(TR2G_level2),         //  Fool's Gold
    NAME_AREA(TR2G_level3),         //  Furnace of the Gods
    NAME_AREA(TR2G_level4),         //  Kingdom
    NAME_AREA(TR2G_level5),         //  Nightmare in Vegas
    { NULL },

    NAME_AREA(TR2_TITLE),
    NAME_AREA(TR2_ASSAULT),         //  Lara's Home
    NAME_AREA(TR2_CUT1),
    { NULL },
    { NULL },
};


static TRN_AREA TRR2_AREAS [] =
{
    NAME_AREA(TRR2_WALL),           //  Great Wall

    NAME_AREA(TRR2_BOAT),           //  Venice
    NAME_AREA(TRR2_VENICE),         //  Bartoli's Hideout
    NAME_AREA(TRR2_OPERA),          //  Opera House

    NAME_AREA(TRR2_RIG),            // Offshore Rig
    NAME_AREA(TRR2_PLATFORM),       // Diving Area
    NAME_AREA(TRR2_UNWATER),        // 40 Fathoms
    NAME_AREA(TRR2_KEEL),           // Wreck of the Maria Doria
    NAME_AREA(TRR2_LIVING),         // Living Quarters
    NAME_AREA(TRR2_DECK),           // The Deck

    NAME_AREA(TRR2_SKIDOO),         // Tibetan Foothills
    NAME_AREA(TRR2_MONASTRY),       // Barkhang Monastery
    NAME_AREA(TRR2_CATACOMB),       // Catacombs of the Talion
    NAME_AREA(TRR2_ICECAVE),        // Ice Palace

    NAME_AREA(TRR2_EMPRTOMB),       //  Temple of Xian
    NAME_AREA(TRR2_FLOATING),       //  Floating Islands
    NAME_AREA(TRR2_XIAN),           //  The Dragon's Lair

    NAME_AREA(TRR2_HOUSE),          //  Home Sweet Home

    NAME_AREA(TRR2G_LEVEL1),        //  The Cold War
    NAME_AREA(TRR2G_LEVEL2),        //  Fool's Gold
    NAME_AREA(TRR2G_LEVEL3),        //  Furnace of the Gods
    NAME_AREA(TRR2G_LEVEL4),        //  Kingdom
    NAME_AREA(TRR2G_LEVEL5),        //  Nightmare in Vegas
    { NULL },

    NAME_AREA(TRR2_TITLE),
    NAME_AREA(TRR2_ASSAULT),        //  Lara's Home
    NAME_AREA(TRR2_CUT1),
    NAME_AREA(TRR2_CUT2),
    NAME_AREA(TRR2_CUT3),
    NAME_AREA(TRR2_CUT4),
    { NULL },
    { NULL },
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static TRN_AREA TR3_AREAS [] =
{
    NAME_AREA(TR3_JUNGLE),              //  Jungle
    NAME_AREA(TR3_TEMPLE),              //  Temple Ruins
    NAME_AREA(TR3_QUADCHAS),            //  The River Ganges
    NAME_AREA(TR3_TONYBOSS),            //  Caves Of Kaliya

    NAME_AREA(TR3_SHORE),               //  Coastal Village
    NAME_AREA(TR3_CRASH),               //  Crash Site
    NAME_AREA(TR3_RAPIDS),              //  Madubu Gorge
    NAME_AREA(TR3_TRIBOSS),             //  Temple Of Puna

    NAME_AREA(TR3_ROOFS),               //  Thames Wharf
    NAME_AREA(TR3_SEWER),               //  Aldwych
    NAME_AREA(TR3_TOWER),               //  Lud's Gate
    NAME_AREA(TR3_OFFICE),              //  City

    NAME_AREA(TR3_NEVADA),              //  Nevada Desert
    NAME_AREA(TR3_COMPOUND),            //  High Security Compound
    NAME_AREA(TR3_AREA51),              //  Area 51

    NAME_AREA(TR3_ANTARC),              //  Antarctica
    NAME_AREA(TR3_MINES),               //  RX - Tech Mines
    NAME_AREA(TR3_CITY),                //  Lost City Of Tinnos
    NAME_AREA(TR3_CHAMBER),             //  Meteorite Cavern

    NAME_AREA(TR3_STPAUL),              //  All Hallows

    NAME_AREA(TR3G_scotland),           //  Highland-Fling
    NAME_AREA(TR3G_willsden),           //  Willards-Lair
    NAME_AREA(TR3G_chunnel),            //  Shakespeare-Cliff
    NAME_AREA(TR3G_undersea),           //  Sleeping-with-the-Fishes
    NAME_AREA(TR3G_zoo),                //  Its-A-Madhouse
    NAME_AREA(TR3G_slinc),              //  Reunion
    { NULL },

    NAME_AREA(TR3_TITLE),
    NAME_AREA(TR3_HOUSE),
    { NULL },
    { NULL },
};


static TRN_AREA TRR3_AREAS [] =
{
    NAME_AREA(TRR3_JUNGLE),             //  Jungle
    NAME_AREA(TRR3_TEMPLE),             //  Temple Ruins
    NAME_AREA(TRR3_QUADCHAS),           //  The River Ganges
    NAME_AREA(TRR3_TONYBOSS),           //  Caves Of Kaliya

    NAME_AREA(TRR3_SHORE),              //  Coastal Village
    NAME_AREA(TRR3_CRASH),              //  Crash Site
    NAME_AREA(TRR3_RAPIDS),             //  Madubu Gorge
    NAME_AREA(TRR3_TRIBOSS),            //  Temple Of Puna

    NAME_AREA(TRR3_ROOFS),              //  Thames Wharf
    NAME_AREA(TRR3_SEWER),              //  Aldwych
    NAME_AREA(TRR3_TOWER),              //  Lud's Gate
    NAME_AREA(TRR3_OFFICE),             //  City

    NAME_AREA(TRR3_NEVADA),             //  Nevada Desert
    NAME_AREA(TRR3_COMPOUND),           //  High Security Compound
    NAME_AREA(TRR3_AREA51),             //  Area 51

    NAME_AREA(TRR3_ANTARC),             //  Antarctica
    NAME_AREA(TRR3_MINES),              //  RX - Tech Mines
    NAME_AREA(TRR3_CITY),               //  Lost City Of Tinnos
    NAME_AREA(TRR3_CHAMBER),            //  Meteorite Cavern

    NAME_AREA(TRR3_STPAUL),             //  All Hallows

    NAME_AREA(TRR3G_SCOTLAND),          //  Highland-Fling
    NAME_AREA(TRR3G_WILLSDEN),          //  Willards-Lair
    NAME_AREA(TRR3G_CHUNNEL),           //  Shakespeare-Cliff
    NAME_AREA(TRR3G_UNDERSEA),          //  Sleeping-with-the-Fishes
    NAME_AREA(TRR3G_ZOO),               //  Its-A-Madhouse
    NAME_AREA(TRR3G_SLINC),             //  Reunion
    { NULL },

    NAME_AREA(TRR3_TITLE),
    NAME_AREA(TRR3_HOUSE),
    { NULL },
    { NULL },
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
/*
    alexhub         Alexandria
    alexhub2        Coastal Ruins
    angkor1         Angkor Wat
    ang_Race        Race for the Iris
    bikebit         City of the Dead
    citnew          Citadel
    cortyard        Chambers of Tulun
    csplit1         Catacombs
    csplit2         Temple of Poseidon
    hall            The Great Hypostyle Hall
    highstrt        Citadel Gate
    jeepchas        Valley of the Kings
    jeepchas2       Kv5
    joby 1a         The Sphinx Complex
    joby 1b         The Valley Temple
    joby 2          Underneath the Sphinx
    joby 3a         Menkaure's Pyramid
    joby 3b         Inside Menkaure's Pyramid.
    joby 4a         The Mastabas
    joby 4b         The Great Pyramid
    joby 4c         Khufu's Queens Pyramids
    joby 5a         Inside the Great Pyramid
    joby 5b & 5c    Temple of Horus
    karnak1         Temple of Karnak
    lake            The Sacred Lake
    libend          The Hall of Demetrius
    library         The Lost Library
    lowstrt         Street Bazar
    nutrench        New City*
    palaces2        Cleopatra's Palaces
    palaces         Pharos, Temple of Isis
    semer2          Guardian of Semerket
    semer           Tomb of Semerket
    settomb1        Tomb of Seth
    settomb2        Burial Chambers
    Title           Title
    Train           Desert Railroad
*/
static TRN_AREA TR4_AREAS [] =
{
    NAME_AREA(TR4_angkor1),             //  0
    NAME_AREA(TR4_ang_race),            //  1
    NAME_AREA(TR4_settomb1),            //  2
    NAME_AREA(TR4_settomb2),            //  3
    NAME_AREA(TR4_jeepchas),            //  4
    NAME_AREA(TR4_jeepchs2),            //  5
    NAME_AREA(TR4_karnak1),             //  6
    NAME_AREA(TR4_hall),                //  7
    NAME_AREA(TR4_lake),                //  8
    NAME_AREA(TR4_title),               //  9       // Nothing
    NAME_AREA(TR4_semer),               //  10  
    NAME_AREA(TR4_semer2),              //  11
    NAME_AREA(TR4_train),               //  12
    NAME_AREA(TR4_alexhub),             //  13
    NAME_AREA(TR4_alexhub2),            //  14
    NAME_AREA(TR4_palaces),             //  15
    NAME_AREA(TR4_palaces2),            //  16
    NAME_AREA(TR4_csplit1),             //  17
    NAME_AREA(TR4_csplit2),             //  18
    NAME_AREA(TR4_library),             //  19
    NAME_AREA(TR4_libend),              //  20
    NAME_AREA(TR4_bikebit),             //  21
    NAME_AREA(TR4_nutrench),            //  22
    NAME_AREA(TR4_cortyard),            //  23
    NAME_AREA(TR4_lowstrt),             //  24
    NAME_AREA(TR4_highstrt),            //  25
    NAME_AREA(TR4_citnew),              //  26
    NAME_AREA(TR4_joby1a),              //  27
    NAME_AREA(TR4_joby1b),              //  28      // None
    NAME_AREA(TR4_joby2),               //  29
    NAME_AREA(TR4_joby3a),              //  30
    NAME_AREA(TR4_joby3b),              //  31
    NAME_AREA(TR4_joby4a),              //  32
    NAME_AREA(TR4_joby4b),              //  33
    NAME_AREA(TR4_joby4c),              //  34
    NAME_AREA(TR4_joby5a),              //  35
    NAME_AREA(TR4_joby5b),              //  36
    NAME_AREA(TR4_joby5c),              //  37

    //
    NAME_AREA(TR4G_office),             //  38  Level Number 1
    NAME_AREA(TR4G_times),              //  39  Level Number 2 For the Game
    { NULL },

    //
    NAME_AREA(TR4_title),
    { NULL },
    { NULL },
};

static TRN_AREA TR5_AREAS [] =
{
    NAME_AREA(TR5_Andrea1),         //  Streets of Rome
    NAME_AREA(TR5_Andrea2),         //  Trajan's market
    NAME_AREA(TR5_Andrea3),         //  The Colosseum
    NAME_AREA(TR5_joby2),           //  The Base
    NAME_AREA(TR5_joby3),           //  The Submarine
    NAME_AREA(TR5_joby4),           //  Deepsea Dive
    NAME_AREA(TR5_joby5),           //  Sinking Submarine
    NAME_AREA(TR5_andy1),           //  Gallows Tree
    NAME_AREA(TR5_Andy2),           //  Labyrinth
    NAME_AREA(TR5_andy3),           //  Old Mill
    NAME_AREA(TR5_rich1),           //  The 13th Floor
    NAME_AREA(TR5_rich2),           //  Escape with the Iris
    NAME_AREA(TR5_richcut2),        //  Security Breach
    NAME_AREA(TR5_rich3),           //
    { NULL },
    NAME_AREA(TR5_Title),
    { NULL },
    { NULL },
};

//
/////////////////////////////////////////////////////////////////////////////
//  Custom Area
/////////////////////////////////////////////////////////////////////////////
static BOOL TRCCustomLevel                      = FALSE;
static int TRCCustomCount                       = 0;
static const int TRCCustomMax                   = 512;
static char CustomLevelName [ MAX_PATH ]        ="";
static TR_AREA TRC_Custom [ TRCCustomMax ] =
{
    { -1, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, },
};

static TRN_AREA TRC_AREAS [] =
{
    NAME_AREA(TRC_Custom),
    { NULL },
    { NULL },
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *GetCustomLevelName()
{
    return CustomLevelName;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void SetCustomLevelName( const char *pName )
{
    strcpy_s ( CustomLevelName, sizeof(CustomLevelName), pName );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void InitCustomArea ()
{
    TRCCustomCount  = 0;
    TRCCustomLevel  = FALSE;
    ZeroMemory ( TRC_Custom, sizeof(TRC_Custom) );
    for ( int i = 0; i < TRCCustomMax; i++ )
    {
        TRC_Custom [ i ].index  = -1;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void AddToCustomArea (TR_AREA *pArea)
{
    TRC_Custom [ TRCCustomCount ]   = *pArea;
    TRCCustomCount++;
    TRC_AREAS [ 0 ].count   = TRCCustomCount;

    TRCCustomLevel  = TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL IsCustomArea ( )
{
    return TRCCustomLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL InsideVertical ( long y, long yTop, long yBottom, DWORD extraVertical )
{
    int low     = (int) LOWORD(extraVertical);
    int high    = (int) HIWORD(extraVertical);

    if ( y >= yTop - low && y <= yBottom + high )
    {
        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get Table and alter level index
/////////////////////////////////////////////////////////////////////////////
static TRN_AREA *GetTable ( int tombraider, int &levelIndex, int &count )
{
    switch ( tombraider )
    {

        //
        case 1 :
        {
            count   = sizeof(TRR1_AREAS)/sizeof(TRN_AREA);
            return TRR1_AREAS;
            break;
        }

        //
        case 10 :
        {
            count   = sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            return TR1_AREAS;
            break;
        }

        //  UB
        case 15 :
        {
            if ( levelIndex < 4 )
            {
                levelIndex  = levelIndex + 15;
            }
            count   = sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            return TR1_AREAS;
            break;
        }

        //  UB
        case 19 :
        {
            if ( levelIndex < 4 )
            {
                levelIndex  = levelIndex + 15;
            }
            count   = sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            return TR1_AREAS;
            break;
        }

        //
        case 2 :
        {
            count   = sizeof(TRR2_AREAS)/sizeof(TRN_AREA);
            return TRR2_AREAS;
            break;
        }

        //
        case 20 :
        {
            count   = sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            return TR2_AREAS;
            break;
        }

        //  GOLD
        case 25 :
        {
            if ( levelIndex < 5 )
            {
                levelIndex  = levelIndex + 18;
            }
            count   = sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            return TR2_AREAS;
            break;
        }

        //  GOLD
        case 29 :
        {
            if ( levelIndex < 5 )
            {
                levelIndex  = levelIndex + 18;
            }
            count   = sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            return TR2_AREAS;
            break;
        }

        //
        case 3 :
        {
            count   = sizeof(TRR3_AREAS)/sizeof(TRN_AREA);
            return TRR3_AREAS;
            break;
        }

        //
        case 30 :
        {
            count   = sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            return TR3_AREAS;
            break;
        }

        //  The Lost Artifact
        case 35 :
        {
            if ( levelIndex < 6 )
            {
                levelIndex  = levelIndex + 20;
            }
            count   = sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            return TR3_AREAS;
            break;
        }

        //  The Lost Artifact
        case 39 :
        {
            if ( levelIndex < 6 )
            {
                levelIndex  = levelIndex + 20;
            }
            count   = sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            return TR3_AREAS;
            break;
        }

        //
        case 4 :
        case 40 :
        {
            count   = sizeof(TR4_AREAS)/sizeof(TRN_AREA);
            return TR4_AREAS;
            break;
        }

        //  Times Exclusive
        case 45 :
        {
            if ( levelIndex < 2 )
            {
                levelIndex  = levelIndex + 38;
            }
            count   = sizeof(TR4_AREAS)/sizeof(TRN_AREA);
            return TR4_AREAS;
            break;
        }

        //  Custom Levels
        case 49 :
        {
            if ( TRCCustomLevel )
            {
                count   = 1;
                return TRC_AREAS;
            }
            break;
        }

        //
        case 5 :
        case 50 :
        {
            count   = sizeof(TR5_AREAS)/sizeof(TRN_AREA);
            return TR5_AREAS;
            break;
        }

        //  Custom Level
        case 99 :
        {
            count = 1;
            return TRC_AREAS;
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int GetTRTableSize ( int tombraider )
{
    switch ( tombraider )
    {
        case 1 :
        {
            return sizeof(TRR1_AREAS)/sizeof(TRN_AREA);
            break;
        }
        case 10 :
        {
            return sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  UB
        case 15 :
        {
            return sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  UB
        case 19 :
        {
            return sizeof(TR1_AREAS)/sizeof(TRN_AREA);
            break;
        }

        case 2 :
        {
            return sizeof(TRR2_AREAS)/sizeof(TRN_AREA);
            break;
        }
        case 20 :
        {
            return sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  GOLD
        case 25 :
        {
            return sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  GOLD
        case 29 :
        {
            return sizeof(TR2_AREAS)/sizeof(TRN_AREA);
            break;
        }

        case 3 :
        {
            return sizeof(TRR3_AREAS)/sizeof(TRN_AREA);
            break;
        }
        case 30 :
        {
            return sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  GOLD
        case 35 :
        {
            return sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            break;
        }
        //  GOLD
        case 39 :
        {
            return sizeof(TR3_AREAS)/sizeof(TRN_AREA);
            break;
        }
        //
        case 4 :
        case 40 :
        {
            return sizeof(TR4_AREAS)/sizeof(TRN_AREA);
            break;
        }
        case 45 :
        {
            return sizeof(TR4_AREAS)/sizeof(TRN_AREA);
            break;
        }

        //  Custom Level
        case 49 :
        {
            if ( TRCCustomLevel )
            {
                return TRCCustomCount;
            }
            break;
        }

        case 5 :
        case 50 :
        {
            return sizeof(TR5_AREAS)/sizeof(TRN_AREA);
            break;
        }
        //
        case 99 :
        {
            return TRCCustomCount;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *GetAreaLevelName ( int tombraider, int levelIndex )
{
    //
    int count = 0;
    TRN_AREA *trLevelsAreas = GetTable ( tombraider, levelIndex, count);

    if ( trLevelsAreas == NULL )
    {
        return "";
    }

    if ( levelIndex < 0 || levelIndex >= count )
    {
        return NULL;
    }

    //
    TR_AREA *trLevelArea = NULL;
    if ( trLevelsAreas [ levelIndex ].pArea != NULL )
    {
        return trLevelsAreas [ levelIndex ].pName;
    }

    return "";
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
short FindAreaForCoordinates ( int tombraider, int levelIndex, long x, long y, long z )
{
    //
    int count = 0;
    TRN_AREA *trLevelsAreas = GetTable ( tombraider, levelIndex, count );

    if ( trLevelsAreas == NULL )
    {
        return -1;
    }

    if ( levelIndex < 0 || levelIndex >= count )
    {
        return NULL;
    }

    //
    TR_AREA *trLevelArea = NULL;
    if ( trLevelsAreas [ levelIndex ].pArea != NULL )
    {
        trLevelArea = trLevelsAreas [ levelIndex ].pArea;
    }

    if ( trLevelArea == NULL )
    {
        return -1;
    }

    //
    int i = 0;
    short areaIndex     = -1;

    //
    long deltaX     = 0x10000000;
    long deltaZ     = 0x10000000;
    long deltaY     = 0x10000000;

    while ( trLevelArea [ i ].pText != NULL )
    {
        //  Must Be within Vertical
        if ( InsideVertical ( y, trLevelArea [ i ].yTop, trLevelArea [ i ].yBottom ) )
        {
            if (    x >= trLevelArea [ i ].x && x < trLevelArea [ i ].x + trLevelArea [ i ].xSectors * TR_SECTOR_SIZE &&
                    z >= trLevelArea [ i ].z && z < trLevelArea [ i ].z + trLevelArea [ i ].zSectors * TR_SECTOR_SIZE        )
            {
                areaIndex   = i;
                return areaIndex;
            }
        }

        i++;
    }

    return areaIndex;
};

//
/////////////////////////////////////////////////////////////////////////////
//  x : East West
//  z : South North
/////////////////////////////////////////////////////////////////////////////
BOOL CheckAreaForCoordinates ( int tombraider, int levelIndex, int area, long x, long y, long z, DWORD extraVertical )
{
    TR_AREA *pArea = GetTRArea ( tombraider, levelIndex, area );
    if ( pArea )
    {
        if ( InsideVertical ( y, pArea->yTop, pArea->yBottom, extraVertical ) )
        {
            long xEnd = pArea->x + pArea->xSectors * TR_SECTOR_SIZE;
            long zEnd = pArea->z + pArea->zSectors * TR_SECTOR_SIZE;
            if ( x >= pArea->x && x < xEnd && z >= pArea->z && z < zEnd )
            {
                return TRUE;
            }
        }
    }

    if ( IsCustomArea() )
    {
        pArea = GetTRArea ( CUSTOM_GAME, CUSTOM_LEVEL_INDEX, area );
        if ( pArea )
        {
            if ( InsideVertical ( y, pArea->yTop, pArea->yBottom, extraVertical ) )
            {
                long xEnd = pArea->x + pArea->xSectors * TR_SECTOR_SIZE;
                long zEnd = pArea->z + pArea->zSectors * TR_SECTOR_SIZE;
                if ( x >= pArea->x && x < xEnd && z >= pArea->z && z < zEnd )
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL EnumAreaForCoordinates ( int tombraider, int levelIndex, int index, TR_AREA *pArea )
{
    //
    int count = 0;
    TRN_AREA *trLevelsAreas = GetTable ( tombraider, levelIndex, count );
    if ( trLevelsAreas == NULL )
    {
        return FALSE;
    }

    if ( levelIndex < 0 || levelIndex >= count )
    {
        return NULL;
    }

    //
    TR_AREA *trLevelArea = NULL;
    if ( trLevelsAreas [ levelIndex ].pArea != NULL )
    {
        trLevelArea = trLevelsAreas [ levelIndex ].pArea;
        count       = trLevelsAreas [ levelIndex ].count;
    }

    if ( trLevelArea == NULL )
    {
        return FALSE;
    }

    if ( index < 0 || index >= count )
    {
        return FALSE;
    }

    //
    if ( trLevelArea [ index ].pText != NULL && trLevelArea [ index ].index != -1 )
    {
        *pArea      = trLevelArea [ index ];
        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int GetTRAreaCount ( int tombraider, int levelIndex )
{
    //
    int count = 0;
    TRN_AREA *trLevelsAreas = GetTable ( tombraider, levelIndex, count );
    if ( trLevelsAreas == NULL )
    {
        return 0;
    }

    if ( levelIndex < 0 || levelIndex >= count )
    {
        return 0;
    }

    //
    TR_AREA *trLevelArea = NULL;
    if ( trLevelsAreas [ levelIndex ].pArea != NULL )
    {
        trLevelArea     = trLevelsAreas [ levelIndex ].pArea;
        count           = trLevelsAreas [ levelIndex ].count;
    }

    if ( trLevelArea == NULL )
    {
        return 0;
    }

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR_AREA *GetTRArea ( int tombraider, int levelIndex, int index )
{
    //
    int count = 0;
    TRN_AREA *trLevelsAreas = GetTable ( tombraider, levelIndex, count );
    if ( trLevelsAreas == NULL )
    {
        return NULL;
    }

    if ( levelIndex < 0 || levelIndex >= count )
    {
        return NULL;
    }

    //
    TR_AREA *trLevelArea = NULL;
    if ( trLevelsAreas [ levelIndex ].pArea != NULL )
    {
        trLevelArea     = trLevelsAreas [ levelIndex ].pArea;
        count           = trLevelsAreas [ levelIndex ].count;
    }

    if ( trLevelArea == NULL )
    {
        return NULL;
    }

    if ( index < 0 || index >= count )
    {
        return NULL;
    }

    //
    if ( trLevelArea [ index ].pText == NULL || trLevelArea [ index ].index == -1 )
    {
        return NULL;
    }

    return & ( trLevelArea [ index ] );
}
