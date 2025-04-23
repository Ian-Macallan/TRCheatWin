#include "stdafx.h"

#include "TR9SaveGame.h"
#include "TRXGlobal.h"
#include "TRXTools.h"
#include "TR_Areas.h"

#include "TRXCHEATWIN.h"

extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
//  Indicator Table
//  The Table use only the first two words (3 bytes)
//  But anyway the position is checked too
/////////////////////////////////////////////////////////////////////////////
TRR_BYTES_INDICATORS IndicatorsTRRTableBytes [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x00,   0x02,   0,  "Standing", },          //  Normally 0x0002 0x0002 0xnnnn 0x0067

    {   FALSE,  0x03,   0x00,   0x03,   1,  "Sliding forward", },  
    {   FALSE,  0x0D,   0x00,   0x0D,   1,  "Underwater", },  
    {   FALSE,  0x0D,   0x00,   0x12,   1,  "Underwater", },
    {   FALSE,  0x12,   0x00,   0x12,   1,  "Other", },
    {   FALSE,  0x13,   0x00,   0x13,   1,  "Climbing", },
    {   FALSE,  0x17,   0x00,   0x02,   1,  "Rolling", },
    {   FALSE,  0x18,   0x00,   0x18,   1,  "Sliding", },
    {   FALSE,  0x20,   0x00,   0x20,   1,  "Sliding backward", },
    {   FALSE,  0x21,   0x00,   0x21,   1,  "On water", },
    {   FALSE,  0x41,   0x00,   0x02,   1,  "Walking in water", },
    {   FALSE,  0x41,   0x00,   0x41,   1,  "Walking in water", },
    {   FALSE,  0x47,   0x00,   0x47,   1,  "Duck", },
    {   FALSE,  0x57,   0x00,   0x57,   1,  "Climbing", },
    {   FALSE,  0x5b,   0x00,   0x5b,   1,  "Climbing", },

    {   TRUE,   0xff,   0xff,   0xff,   0,  "End", },

};
int IndicatorsTRRTableBytesCount = sizeof(IndicatorsTRRTableBytes)/sizeof(TRR_BYTES_INDICATORS);

//
//  Normally we could use a table like follows
TRR_WORDS_INDICATORS IndicatorsTRRTableWords [ MAX_INDICATORS ] =
{
    {   FALSE,  0x0002, 0x0002, 0x0000, 0x0067, FALSE,  0,  "Standing" },
    {   FALSE,  0x0008, 0x0008, 0x0000, 0x0112, FALSE,  1,  "Quad Bike" },
    {   FALSE,  0x000d, 0x000d, 0x0000, 0x006c, FALSE,  1,  "Swimming" },
    {   FALSE,  0x0012, 0x0012, 0x0000, 0x0057, FALSE,  1,  "Indicator 2" },
    {   FALSE,  0x0021, 0x0021, 0x0000, 0x006E, TRUE,   1,  "Swimming" },       //  Use W3
    {   FALSE,  0x0018, 0x0018, 0x0000, 0x0046, TRUE,   1,  "Sliding" },        //  Use W3
    {   FALSE,  0x000d, 0x0012, 0x0000, 0x006c, TRUE,   1,  "Underwater" },     //  Use W3
    {   FALSE,  0x0002, 0x0002, 0x0047, 0x00bd, TRUE,   1,  "Standing" },       //  Use W3

    {   FALSE,  0x000f, 0x000f, 0x0000, 0x0173, TRUE,   2,  "Quad Bike" },      //  Use W3
    {   FALSE,  0x0001, 0x0001, 0x0000, 0x0163, TRUE,   2,  "Quad Bike" },      //  Use W3

    {   FALSE,  0x0001, 0x0002, 0x0000, 0x000a, FALSE,  2,  "Indicator 10" },
    {   FALSE,  0x0001, 0x0002, 0x0000, 0x0008, FALSE,  2,  "Indicator 11" },

    {   FALSE,  0x0002, 0x0002, 0x0000, 0x000B, TRUE,   9,  "Standing" },       //  Use W3
    {   TRUE,   0xffff, 0xffff, 0xffff, 0xffff, TRUE,   0,  "End" },            //  End
};
int IndicatorsTRRTableWordsCount = sizeof(IndicatorsTRRTableWords)/sizeof(TRR_WORDS_INDICATORS);

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static const char *TR1LevelNames [] =
{
    "Caves",
    "City of Vilcabamba",
    "The Lost Valley",          // Shotgun
    "Tomb of Qualopec",         
    "St. Francis' Folly",       //  Magnum
    "Colosseum",                
    "Palace Midas",             
    "Cistern",
    "Tomb of Tihocan",
    "City of Khamoon",
    "Obelisk of Khamoon",
    "Sanctuary of the Scion",   //  Uzis
    "Natla's Mines",            
    "Atlantis",
    "The Great Pyramid",        // End if TR1

    "Return to Egypt",          // Start of TUB 
    "Temple of the Cat",        //  All Guns
    "Atlantean Stronghold",
    "The Hive",
    "20: ",
    "21: ",
    "22: ",
    "23: ",
    "24: ",
    "25: ",
    "26: ",
    "27: ",
    "28: ",
    "29: ",
    "30: ",
    "31: ",
    "32: ",
};

static BYTE TR1Secrets [] =
{
    3,  3,  5,  3,  4,
    3,  3,  3,  2,  3,
    3,  1,  3,  3,  3,
    3,  4,  2,  1,          //  Unfinished Business
};

static BYTE TR1Pickup [] =
{
    7,  13, 16, 8,  19, 14, 23, 28, 26, 24, 38, 29, 30, 50, 31,
    53, 63, 63, 60
};

static BYTE TR1Kills [] =
{
    14, 29, 13, 8,  23, 27, 43, 34, 17, 14, 16, 15, 3,  32, 6,
    41, 43, 31, 41
};

//
static WORD TR1_Secrets_Offsets[] =
{
    0,  1,  2,  3,  4,
    5,  6,  7,  8,  9,
    10, 11, 12, 13, 14,
    15, 16, 17, 18,
};

static TR_START_CONDITIONS TR1_Start_LikePlus [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },
};

static TR_START_CONDITIONS TR1_Start_LikeGame [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16
    {   true,   false,  false,  false,  false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,      // Lost Valley
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  true,   false,  false,  false,  false,      // St. Francis' Folly
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   false,  false,  false,  false,      // Sanctuary of the Scion
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,      //  The Great Pyramid
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   false,  false,  false,  false,
                50,     100,    6*10,   0,      0,      0,      0,
                0,      0,      2,      1000,   1800    },
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static const char *TR2LevelNames [] =
{
    "Great Wall",               //  Grenade

    "Venice",                   //  Magnum
    "Bartoli's Hideout",        //  Uzi
    "Opera House",

    "Offshore Rig",             // Shotgun Harpoon
    "Diving Area",              //  M16
    "40 Fathoms",

    "Wreck of the Maria Doria",
    "Living Quarters",
    "The Deck",
    
    "Tibetan Foothills",
    "Barkhang Monastery",
    "Catacombs of the Talion",
    "Ice Palace",

    "Temple of Xian",
    "Floating Islands",
    "The Dragon's Lair",

    "Home Sweet Home",

    "The Cold War",         //  Harpoon M16 Magnum Uzi
    "Fool's Gold",          //  Grenade
    "Furnace of the Gods",
    "Kingdom",
    "Nightmare in Vegas",
    "24: ",
    "25: ",
    "26: ",
    "27: ",
    "28: ",
    "29: ",
    "30: ",
    "31: ",
    "32: ",

};

static BYTE TR2Secrets [] =
{
    3,
    3,  3,  3,
    3,  3,  3,
    3,  3,  3,
    3,  3,  3,  3,
    3,  3,  0,
    0,  
    3,  3,  3,  3,  3,
};

//
static BYTE TR2Pickup [] =
{
    14,
    30, 28, 37,
    31, 39, 14,
    41, 16, 35,
    31, 49, 39, 33,
    40, 39, 24,
    45, 
    71, 69, 64, 52, 75, 
};

static BYTE TR2Kills [] =
{
    23,
    24, 37, 46,
    20, 34, 16,
    35, 21, 30,
    33, 30, 33, 22,
    37, 26, 11,
    16, 
    44, 62, 41, 29, 23, 
};

static BYTE TR2Crystals [] =
{
    0
};

//
static WORD TR2_Secrets_Offsets[] =
{
    //  China
    0,                  
    //  Venice
    7,  8,  9,          //  
    //  Offshore
    10, 11, 12,
    //  Sunken Ship
    13, 14, 15, 
    //  Tibet
    1,  2, 3, 4,    
    //  China & Dragon Lair
    5,  6,
    //  Tje Dragon Lair
    21,
    //  Manor
    23,
    //
    16, 17, 18, 19, 20              //  Offset in TR2G override TR2
};

static TR_START_CONDITIONS TR2_Start_LikePlus [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16
    {   true,   true,   true,   true,   true,   false,  true,   true,
                80,     160,    4*6,    3,      0,      4,      80,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },

};

static TR_START_CONDITIONS TR2_Start_LikeGame [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16
    {   true,   false,  false,  false,  false,  false,  false,  false,      //  0
                80,     160,    4*6,    3,      0,      4,      80,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },

    {   true,   false,  false,  false,  false,  false,  true,   false,      //  1 Venice
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  false,  false,  false,  true,   false,      //  2 Bartoldi
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   false,  false,  false,  true,   false,      //  3 Opera
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   false,  false,  false,  true,   false,      //  4 Offshore Rig
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   false,      //  5 Diving Area
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       //  6
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       //  7
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 8
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 9
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 10
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 11
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 12
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 13
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 14
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 15
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   false,  true,   true,       // 16
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   false,  false,  false,  false,  false,  false,  false,  false,      //  17 Home
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,      //  Cold War
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   true,   true,   false,  false,      //  Fool's Gold
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   true,   true,   false,  false,      //  Furnace of the Gods
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  true,   true,   true,   true,   false,      //  Kingdom
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
    {   false,  false,  false,  false,  false,  false,  false,  false,      //  Vegas
                80,     160,    4*6,    3,      0,      4,      80,
                0,      0,      2,      1000,   1800    },
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static const char *TR3LevelNames [] =
{
    //  India
    "Jungle",                   // Shotgun
    "Temple Ruins",
    "The River Ganges",
    "Caves of Kaliya",          // Grenade

    //  
    //  South Pacific
    "Coastal Village",          // Harpoon
    "Crash Site",               // M16
    "Madubu Gorge",             // Rocket
    "Temple of Puna",           // Magnum

    //  London
    "Thames Wharf",
    "Aldwych",                  //  Uzi M16 Rocket
    "Lud's Gate",               //  Rocket Harpoon
    "City",

    //  Nevada
    "Nevada Desert",            // Nevada
    "High Security Compound",   // Magnum Grenade
    "Area 51",                  // M16 Grenade

    //  Antartica
    "Antarctica",               //  Uzi
    "RX-Tech Mines",
    "Lost City of Tinnos",      //  Rocket
    "Meteorite Cavern",

    //
    "London - All Hallows",     //  Uzi

    //
    "Highland Fling",
    "Willard's Lair",
    "Shakespeare Cliff",
    "Sleeping with the Fishes",
    "It's a Madhouse!",
    "Reunion",
    "27: ",
    "28: ",
    "29: ",
    "30: ",
    "31: ",
    "32: ",
};

static TR_START_CONDITIONS TR3_Start_LikePlus [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16
    {   true,   true,   true,   true,   true,   true,   true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },
};

static TR_START_CONDITIONS TR3_Start_LikeGame [] =
{
    //  PIS     MAG     UZI     RIO     HAR     ROC     GRE     M16         
    {   true,   false,  false,  false,  false,  false,  false,  false,      //  India
                10,     40,     6*2,    3,      1,      2,      60,
    //          SMA     LAR     FLA     HEA     AIR
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   false,  false,  false,  false,  false,  true,   false,      // Pacific
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  true,   false,  true,   false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  true,   false,  true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  true,   true,   true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   false,  false,  false,  false,  false,  false,  false,      // London
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  false,  false,  false,  false,  false,  false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  true,   false,  false,  true,   false,  true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  true,   false,  true,   true,   false,  true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   false,  false,  false,  false,  false,  false,  false,      // Nevada
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   false,  false,  false,  false,  false,  false,  false,  false,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   false,  false,  false,  false,  true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   true,   false,  false,  false,  false,  true,   true,       // Antartica
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   false,  false,  false,  true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   false,  false,  false,  true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   false,  false,  true,   true,   true,
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   true,   true,   false,  false,  true,   true,   true,   // All Hallows
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },

    {   true,   false,  false,  false,  false,  false,  false,  false,  // Highland
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  true,   true,   true,   false,  false,  true,   
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  true,   true,   true,   false,  false,  true,   
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   false,  true,   true,   true,   true,   false,  true,   
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   true,   true,   true,   
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
    {   true,   true,   true,   true,   true,   true,   true,   true,   
                10,     40,     6*2,    3,      1,      2,      60,
                0,      0,      2,      1000,   1800    },
};

static BYTE TR3Secrets [] =
{
    //  India
    6,  4,  5, 0,
    //  Pacific
    4,  3,  3,  1,
    //  London
    5,  5,  6,  1,
    //  Nevada
    3,  2,  3,
    //  Antartic
    3,  3,  3,  0,
    //  All Hallows
    0,
    //  Last Artefact
    3,  3,  3,  3,  3,  0,  // Gold
};

//
//
static BYTE TR3Pickup [] =
{
    33, 43, 32, 10, 
    29, 26, 12, 11, 
    32, 50, 59, 7,  
    28, 34, 36, 
    34, 26, 33, 7,  
    15,
    47, 41, 39, 57, 49, 32, 
};

static BYTE TR3Kills [] =
{
    19, 30, 37, 15,
    26, 56, 16, 9,
    26, 29, 20, 0,
    29, 16, 40,
    30, 24, 90, 7,  
    2,
    24, 21, 30, 15, 22, 11, 
};

static BYTE TR3Crystals [] =
{
    6,  8,  7,  2,  5,  4,  8,  2,  8,  9,  11, 0,  3,  3,  4,  4,  5,  5,  0,
    0,  0,  0,  0,  0,  0,  
};

//
static WORD TR3_Secrets_Offsets[] =
{
    0x00,   //  0xe28e4 - 0xe28e4,          //  Jungle      0xe28e4
    0x01,   //  0xe28e5 - 0xe28e4,          //  Ruins       0xe28e5
    0x02,   //  0xe28e6 - 0xe28e4,          //  Gange       0xe2866
    0x03,   //  0xe28e7 - 0xe28e4,          //  Cave        0xe2867

    0x04,   //  0xe28e8 - 0xe28e4,          //  Costal      0xe28e8
    0x05,   //  0xe28e9 - 0xe28e4,          //  Crash       0xe28e9
    0x06,   //  0xe28ea - 0xe28e4,          //  Madubu      0xe28ea
    0x07,   //  0xe28eb - 0xe28e4,          //  Puna        0xe28eb

    //  Some Space Here between 0x08 and 0x6f (0x67 places)

    //
    0x72,   //  0xe2956 - 0xe28e4,          //  Thames      0xe2956
    0x73,   //  0xe2957 - 0xe28e4,          //  Aldwytch    0xe2957
    0x74,   //  0xe2958 - 0xe28e4,          //  Lud         0xe2958
    0x75,   //  0xe2959 - 0xe28e4,          //  City        0xe2959

    0x6f,   //  0xe2953 - 0xe28e4,          //  Nevada      0xe2953
    0x70,   //  0xe2954 - 0xe28e4,          //  Compound    0xe2954
    0x71,   //  0xe2955 - 0xe28e4,          //  Area51      0xe2955

    0x76,   //  0xe295a - 0xe28e4,          //  Antartic    0xe295a
    0x77,   //  0xe295b - 0xe28e4,          //  Rx Tech     0xe295b
    0x78,   //  0xe295c - 0xe28e4,          //  Lost City   0xe295c
    0x79,   //  0xe295d - 0xe28e4,          //  Meteore     0xe295d

    0x7a,   //  0xe295e - 0xe28e4,          //  All Hallows

    0x7b,   //  0xe295f - 0xe28e4,          //  Highland
    0x7c,   //  0xe2960 - 0xe28e4,          //  Willard
    0x7d,   //  0xe2961 - 0xe28e4,          //  Shakespeare
    0x7e,   //  0xe2962 - 0xe28e4,          //  Madhouse
    0x7f,   //  0xe2963 - 0xe28e4,          //  Sleeping
    0x80,   //  0xe2964 - 0xe28e4,          //  Reunion
};

//  Add 0x300 and add Start of Block
//  Health Will Be 10 bytes after
TR_HEALTH_RANGE TR1_HEALTH_RANGES[32] =
{
    { 0x825, 0 },   // Caves
    { 0x181D, 0 },  // City of Vilacamba
    { 0x82D, 0 },   // Lost Valley
    { 0xC41, 0 },   // Tomb of Qualopec
    { 0x1A39, 0 },  // St. Francis' Folly
    { 0xF4F, 0 },   // Colosseum
    { 0x82F, 0 },   // Palace Midas
    { 0x197B, 0 },  // The Cistern
    { 0xA29, 0 },   // Tomb of Tihocan
    { 0x827, 0 },   // City of Khamoon
    { 0xA8F, 0 },   // Obelisk of Khamoon
    { 0x114F, 0 },  // Sanctuary of the Scion
    { 0x12D3, 0 },  // Natla's Mines
    { 0xD0F, 0 },   // Atlantis
    { 0x10FD, 0 },  // The Great Pyramid
    { 0x8F3, 0 },   // Return to Egypt
    { 0xE1D, 0 },   // Temple of the Cat
    { 0xE35, 0 },   // Atlantean Stronghold
    { 0x10DF, 0 },  // The Hive
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },

};

//  Add 0x300 and add Start of Block
TR_HEALTH_RANGE TR2_HEALTH_RANGES[32] =
{
    {   0xB64, 0xB7C,   },  // The Great Wall
    {   0x7FA, 0x7FA, },    // Venice
    {   0x1734, 0x1758, },  // Bartoli's Hideout
    {   0x1E20, 0x1E38, },  // Opera House
    {   0xAC4,  0xADC,  },  // Offshore Rig
    {   0x12DE, 0x131A, },  // Diving Area
    {   0x7FC,  0x7FC,  },  // 40 Fathoms
    {   0x238E, 0x23EE, },  // Wreck of the Maria Doria
    {   0x90A,  0x90A,  },  // Living Quarters
    {   0xBAC,  0xBF4,  },  // The Deck
    {   0x12E4, 0x1314, },  // Tibetan Foothills
    {   0x2522, 0x2552+0x90,    },  // Barkhang Monastery
    {   0x7F8,  0x7F8,  },  // Catacombs of the Talion
    {   0xE2A,  0xE4E,  },  // Ice Palace
    {   0x2A7A, 0x2A9E+0x28,    },  // Temple of Xian
    {   0x9CC,  0x9D8,  },  // Floating Islands
    {   0xF78,  0xF90,  },  // The Dragon's Lair
    {   0xE86,  0xF2E,  },  // Home Sweet Home
    {   0x1626, 0x163E, },  // The Cold War
    {   0x1D80, 0x1D80+0x20,    },  // Fool's Gold
    {   0x1FD4, 0x1FE0+0x60,    },  // Furnace of the Gods
    {   0x91A,  0x91A+0x20, },  // Kingdom
    {   0xDDA,  0xDF2,  },  // Nightmare in Vegas
    //
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
};

//  Add 0x300 and add Start of Block
TR_HEALTH_RANGE TR3_HEALTH_RANGES [32] =
{
    {   0xB26,  0xB26,  },  // Jungle
    {   0xDCC,  0xE00,  },  // Temple Ruins
    {   0xAFC,  0xAFC,  },  // The River Ganges
    {   0x1038, 0x118A, },  // Caves of Kaliya
    {   0xCB8,  0xCB8   },  // Coastal Village
    {   0x2046, 0x2046  },  // Crash Site
    {   0x1250, 0x126A  },  // Madubu Gorge
    {   0xAD2,  0xAD2   },  // Temple of Puna
    {   0x10AA, 0x10AA  },  // Thames Wharf
    {   0x2C9A, 0x2D02  },  // Aldwych
    {   0xFF0,  0x100A+0x50 },  // Lud's Gate
    {   0xBB2,  0xBB2   },  // City
    {   0xAF8,  0xAF8   },  // Nevada Desert
    {   0xB4C,  0xB66   },  // High Security Compound
    {   0x11E4, 0x1218  },  // Area 51
    {   0xB12,  0xB12   },  // Antarctica
    {   0xFB0,  0xFB0+0x20  },  // RX-Tech Mines
    {   0xB7C,  0xB7C   },  // Lost City of Tinnos
    {   0xAD0,  0xAD0   },  // Meteorite Cavern
    {   0x1076, 0x10AA  },  // All Hallows
    {   0x1BF4, 0x1C0E+0x20 },  // Highland Fling
    {   0x15EE, 0x15EE  },  // Willard's Lair
    {   0x1338, 0x1352  },  // Shakespeare Cliff
    {   0xB5A,  0xB5A+0x10   },  // Sleeping with the Fishes
    {   0x1084, 0x1084  },  // It's a Madhouse!
    {   0x1810, 0x1844  },  // Reunion
    //
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
};

static const char *TR4LevelNames [] =
{
    "Angkor Wat",                   //  01      Index 0
    "Race for the Iris",            //  02
    "Tomb of Seth",                 //  03
    "Burial Chambers",              //  04
    "Valley of the Kings",          //  05
    "KV5",                          //  06
    "Temple of Karnak",             //  07
    "Great Hypostyle Hall",         //  08
    "Sacred Lake",                  //  09
    "Nothing ???",                  //  10      Index 9
    "Tomb of Semerkhet",            //  11      Index 10
    "Guardian of Semerkhet",        //  12
    "Desert Railroad",              //  13
    "Alexandria",                   //  14
    "Coastal Ruins",                //  15
    "Pharos, Temple of Isis",       //  16
    "Cleopatra's Palaces",          //  17
    "Catacombs",                    //  18
    "Temple of Poseidon",           //  19
    "The Lost Library",             //  20
    "Hall of Demetrius",            //  21      Index 20
    "City of the Dead",             //  22
    "Trenches",                     //  23
    "Chambers of Tulun",            //  24
    "Street Bazaar",                //  25
    "Citadel Gate",                 //  26
    "Citadel",                      //  27
    "Sphinx Complex",               //  28
    "Nothing ???",                  //  29      Index 28
    "Underneath the Sphinx",        //  30
    "Menkaure's Pyramid",           //  31      Index 30
    "Inside Menkaure's Pyramid",    //  32
    "The Mastabas",                 //  33
    "The Great Pyramid",            //  34
    "Khufu's Queen's Pyramids",     //  35
    "Inside the Great Pyramid",     //  36
    "Temple of Horus",              //  37
    "Temple of Horus",              //  38      Index 37

    "Office",                       //  39      Index 38
    "Times Exclusive",              //  40      Index 39
};

static const char *TR5LevelNames [] =
{
    "Streets of Rome",              //  01
    "Trajan's markets",             //  02
    "The Colosseum",                //  03
    "The Base",                     //  04
    "The Submarine",                //  05
    "Deepsea Dive",                 //  06
    "Sinking Submarine",            //  07
    "Gallows Tree",                 //  08
    "Labyrinth",                    //  09
    "Old Mill",                     //  10
    "The 13th Floor",               //  11
    "Escape with the Iris",         //  12
    "Security Breach",              //  13
    "Red Alert!",                   //  14
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::m_TR1_Start [ NB_TR1_BLOCKS ] = { NULL };
char *CTR9SaveGame::m_TR2_Start [ NB_TR2_BLOCKS ] = { NULL };
char *CTR9SaveGame::m_TR3_Start [ NB_TR3_BLOCKS ] = { NULL };

char *CTR9SaveGame::m_TR1_Blocks [ NB_TR1_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };
char *CTR9SaveGame::m_TR2_Blocks [ NB_TR2_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };
char *CTR9SaveGame::m_TR3_Blocks [ NB_TR3_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };

GUN_TR1 *CTR9SaveGame::m_TR1_Guns [ NB_TR1_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };
GUN_TR2 *CTR9SaveGame::m_TR2_Guns [ NB_TR2_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };
GUN_TR3 *CTR9SaveGame::m_TR3_Guns [ NB_TR3_BLOCKS ] [ NB_OF_SLOTS ] = { NULL };

char    *CTR9SaveGame::m_pBuffer            = NULL;
char    *CTR9SaveGame::m_pBufferBackup      = NULL;
int     CTR9SaveGame::m_iSaveLength         = 0;

//
BOOL TraceMode                              = FALSE;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR9SaveGame    *CTR9SaveGame::m_pInstance = NULL;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR9SaveGame, CObject)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR9SaveGame::CTR9SaveGame(void)
{
    Init(true);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR9SaveGame *CTR9SaveGame::GetInstance(BOOL bCreate)
{
    if ( bCreate && m_pInstance == NULL )
    {
        m_pInstance = new CTR9SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR9SaveGame *CTR9SaveGame::I(BOOL bCreate)
{
    return GetInstance(bCreate);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::IsBufferModified()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->BufferModified();
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::WriteFile(const char *oFilename)
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->writeFile(oFilename);
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::Discard()
{
    if ( m_pInstance != NULL )
    {
        m_pInstance->discard();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::Init(bool bFromContructor)
{
    m_hFile     = NULL;
    ZeroMemory ( m_szFilename, sizeof(m_szFilename) );
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    for ( int b = 0; b < NB_TR1_BLOCKS; b++ )
    {
        m_TR1_Start [ b ]   = NULL;
        m_TR2_Start [ b ]   = NULL;
        m_TR3_Start [ b ]   = NULL;

        for ( int l = 0; l < NB_OF_SLOTS; l++ )
        {
            m_TR1_Blocks [ b ] [ l ]    = NULL;
            m_TR2_Blocks [ b ] [ l ]    = NULL;
            m_TR3_Blocks [ b ] [ l ]    = NULL;

            m_TR1_Guns [ b ] [ l ]      = NULL;
            m_TR2_Guns [ b ] [ l ]      = NULL;
            m_TR3_Guns [ b ] [ l ]      = NULL;
        }
    }

    //
    m_iSaveLength   = 0;

    if ( bFromContructor || m_pBuffer == NULL )
    {
        m_pBuffer       = new char [ LEN_BUFFER ];
    }
    ZeroMemory ( m_pBuffer, LEN_BUFFER );

    if ( bFromContructor || m_pBufferBackup == NULL )
    {
        m_pBufferBackup = new char [ LEN_BUFFER ];
    }
    ZeroMemory ( m_pBufferBackup, LEN_BUFFER );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR9SaveGame::~CTR9SaveGame(void)
{
    m_pInstance = NULL;

    if ( m_pBuffer != NULL )
    {
        delete m_pBuffer;
        m_pBuffer = NULL;
    }

    if ( m_pBufferBackup != NULL )
    {
        delete m_pBufferBackup;
        m_pBufferBackup = NULL;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int TestGunMarker ( WORD pGunMarker, WORD marker, bool noGuns = false )
{
    if ( pGunMarker == marker )
    {
        return 1;
    }

    if ( pGunMarker - 1 == marker )
    {
        return 1;
    }

    if ( pGunMarker == marker - 1 )
    {
        return 1;
    }

    if ( pGunMarker + 1 == marker )
    {
        return 1;
    }

    if ( pGunMarker == marker + 1 )
    {
        return 1;
    }

    if ( noGuns )
    {
        // return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int TestAmmos ( WORD pGunAmmos, WORD ammosZero, WORD ammos, WORD unit, bool allowZero = false )
{
    if ( pGunAmmos == ammosZero || pGunAmmos == ammos )
    {
        return 1;
    }

    if ( ammosZero == 0 && ( pGunAmmos == ammos / unit ) )
    {
        // return 1;
    }

    if ( ammosZero == 0 && allowZero )
    {
        if ( CTRXGlobal::m_iSearchExt )
        {
            return 1;
        }
    }

    //
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns )
{
    char *pFound = NULL;

    //
    int magnumZero  = magnum;
    int uziZero     = uzi;
    int riotZero    = riot;

    if ( ( mask & TRR1_MASK_MAGNUM ) == 0 )
    {
        magnumZero = 0;
    }

    if ( ( mask & TRR1_MASK_UZI ) == 0 )
    {
        uziZero = 0;
    }

    if ( ( mask & TRR1_MASK_SHOTGUN ) == 0 )
    {
        riotZero = 0;
    }

    if ( TraceMode )
    {
        printf ( "\nSearching 3 : %d From 0x%06X to 0x%06X (allowZero:%d noGuns:%d): ",
            guns, RelativeAddress ( pFrom ), RelativeAddress ( pUntil ), allowZero, noGuns );
    }
    
    //
    int foundIndex = -1;
    for ( char *pCurrent = pFrom; pCurrent <= pUntil; pCurrent++ )
    {
        GUN_TR1 *pTag = (GUN_TR1 *) (pCurrent);
        if (    TestGunMarker ( pTag->guns, guns, noGuns ) &&
                TestAmmos ( pTag->magnum, magnumZero, magnum, 1, allowZero ) &&
                TestAmmos ( pTag->uzi, uziZero, uzi, UZI_CLIP, allowZero ) &&
                TestAmmos ( pTag->riotgun, riotZero, riot, 1, allowZero ) &&
                ( magnumZero != 0 || uziZero != 0 || riotZero != 0 || allowZero ) )
        {
            foundIndex++;
            countResult++;

            int newValue = 0;
            newValue += TestAmmos ( pTag->magnum, magnumZero, magnum, 1 );
            newValue += TestAmmos ( pTag->uzi, uziZero, uzi, UZI_CLIP );
            newValue += TestAmmos ( pTag->riotgun, riotZero, riot, 1 );

            WORD *pAir = ( WORD * ) ( ( BYTE * ) pTag - TR1_OFFSET_AIR );
            if ( allowZero && newValue < 3 && *pAir == 1800 )
            {
                newValue = 3;
            }

            if ( newValue >= valueResult )
            {
                valueResult = newValue;
                pFound      = pCurrent;
            }

            if ( pFound == NULL )
            {
                pFound = pCurrent;
            }
        }
    }

    //
    //  With Full Mask
    if ( pFound == NULL && mask != 0xffff )
    {
        // pFound = SearchGunStructure ( pFrom, pUntil, health, magnum, uzi, riot, countResult, valueResult, 0xffff, true, false );
    }

    //  Allow Zeroes
    if ( pFound == NULL && ! allowZero && ! noGuns )
    {
        pFound = SearchGunStructure ( pFrom, pUntil, guns, magnum, uzi, riot, countResult, valueResult, mask, true, false );
    }

    //  Without Health
    if ( pFound == NULL && ! allowZero && ! noGuns )
    {
        // pFound = SearchGunStructure ( pFrom, pUntil, health, magnum, uzi, riot, countResult, valueResult, mask, false, true );
    }

    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            WORD m16, WORD grenade, WORD harpoon,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns  )
{
    char *pFound = NULL;

    int magnumZero      = magnum;
    int uziZero         = uzi;
    int riotZero        = riot;
    int m16Zero         = m16;
    int grenadeZero     = grenade;
    int harpoonZero     = harpoon;

    //
    if ( ( mask & TRR2_MASK_DESERT ) == 0 )
    {
        magnumZero = 0;
    }

    if ( ( mask & TRR2_MASK_UZI ) == 0 )
    {
        uziZero = 0;
    }

    if ( ( mask & TRR2_MASK_SHOTGUN ) == 0 )
    {
        riotZero = 0;
    }

    if ( ( mask & TRR2_MASK_M16 ) == 0 )
    {
        m16Zero = 0;
    }

    if ( ( mask & TRR2_MASK_GRENADE ) == 0 )
    {
        grenadeZero = 0;
    }

    if ( ( mask & TRR2_MASK_HARPOON ) == 0 )
    {
        harpoonZero = 0;
    }

    if ( TraceMode )
    {
        printf ( "\nSearching 6 : %d From 0x%06X to 0x%06X (allowZero:%d noGuns:%d): ",
            guns, RelativeAddress ( pFrom ), RelativeAddress ( pUntil ), allowZero, noGuns );
    }

    int foundIndex = -1;
    for ( char *pCurrent = pFrom; pCurrent <= pUntil; pCurrent++ )
    {
        GUN_TR2 *pTag2 = (GUN_TR2 *) (pCurrent);
        if (    TestGunMarker ( pTag2->guns, guns, noGuns ) &&
                TestAmmos ( pTag2->magnum, magnumZero, magnum, 1, allowZero ) &&
                TestAmmos ( pTag2->uzi, uziZero, uzi, UZI_CLIP, allowZero ) &&
                TestAmmos ( pTag2->riotgun, riotZero, riot, 1, allowZero ) &&
                TestAmmos ( pTag2->m16, m16Zero, m16, M16_CLIP, allowZero ) &&
                TestAmmos ( pTag2->grenade, grenadeZero, grenade, GRENADE_CLIP, allowZero ) &&
                TestAmmos ( pTag2->harpoon, harpoonZero, harpoon, 1, allowZero ) &&
                ( magnumZero != 0 || uziZero != 0 || riotZero != 0 || m16Zero != 0 ||
                    grenadeZero != 0 || harpoonZero != 0 || allowZero ) )
        {
            countResult++;
            foundIndex++;

            int newValue = 0;
            newValue += TestAmmos ( pTag2->magnum, magnumZero, magnum, 1 );
            newValue += TestAmmos ( pTag2->uzi, uziZero, uzi, UZI_CLIP );
            newValue += TestAmmos ( pTag2->riotgun, riotZero, riot, 1 );
            newValue += TestAmmos ( pTag2->m16, m16Zero, m16, M16_CLIP );
            newValue += TestAmmos ( pTag2->grenade, grenadeZero, grenade, GRENADE_CLIP );
            newValue += TestAmmos ( pTag2->harpoon, harpoonZero, harpoon, 1 );

            WORD *pAir = ( WORD * ) ( ( BYTE * ) pTag2 - TR2_OFFSET_AIR );
            if ( allowZero && newValue < 6 && *pAir == 1800 )
            {
                newValue = 6;
            }

            if ( newValue >= valueResult )
            {
                valueResult = newValue;
                pFound      = pCurrent;
            }

            if ( pFound == NULL )
            {
                pFound = pCurrent;
            }
        }
    }

    //
    //  With Full Mask
    if ( pFound == NULL && mask != 0xffff )
    {
        // pFound = SearchGunStructure ( pFrom, pUntil, health, magnum, uzi, riot, m16, grenade, harpoon, countResult, valueResult, 0xffff, true, false );
    }

    //  Allow Zeroes
    if ( pFound == NULL && ! allowZero && ! noGuns )
    {
        pFound = SearchGunStructure ( pFrom, pUntil, guns, magnum, uzi, riot, m16, grenade, harpoon, countResult, valueResult, mask, true, false );
    }

    //  Without Health
    if ( pFound == NULL && ! allowZero && ! noGuns )
    {
        // pFound = SearchGunStructure ( pFrom, pUntil, health, magnum, uzi, riot, m16, grenade, harpoon, countResult, valueResult, false, index, true );
    }

    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            WORD m16, WORD grenade, WORD harpoon, WORD rocket,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns  )
{
    char *pFound = NULL;

    int magnumZero      = magnum;
    int uziZero         = uzi;
    int riotZero        = riot;
    int m16Zero         = m16;
    int grenadeZero     = grenade;
    int harpoonZero     = harpoon;
    int rocketZero      = rocket;

    //
    if ( ( mask & TRR3_MASK_DESERT ) == 0 )
    {
        magnumZero = 0;
    }

    if ( ( mask & TRR3_MASK_UZI ) == 0 )
    {
        uziZero = 0;
    }

    if ( ( mask & TRR3_MASK_SHOTGUN ) == 0 )
    {
        riotZero = 0;
    }

    if ( ( mask & TRR3_MASK_MP5 ) == 0 )
    {
        m16Zero = 0;
    }

    if ( ( mask & TRR3_MASK_GRENADE ) == 0 )
    {
        grenadeZero = 0;
    }

    if ( ( mask & TRR3_MASK_HARPOON ) == 0 )
    {
        harpoonZero = 0;
    }

    if ( ( mask & TRR3_MASK_ROCKET ) == 0 )
    {
        rocketZero = 0;
    }

    if ( TraceMode )
    {
        printf ( "\nSearching 7 : %d From 0x%06X to 0x%06X (allowZero:%d noGuns:%d): ",
            guns, RelativeAddress ( pFrom ), RelativeAddress ( pUntil ), allowZero, noGuns );
    }

    int foundIndex = -1;
    for ( char *pCurrent = pFrom; pCurrent <= pUntil; pCurrent++ )
    {
        GUN_TR3 *pTag3 =(GUN_TR3 *) (pCurrent);
        int iMatch =  0;
        if (    TestGunMarker ( pTag3->guns, guns, noGuns ) &&
                TestAmmos ( pTag3->magnum, magnumZero, magnum, 1, allowZero ) &&
                TestAmmos ( pTag3->uzi, uziZero, uzi, UZI_CLIP, allowZero ) &&
                TestAmmos ( pTag3->riotgun, riotZero, riot, 1, allowZero ) &&
                TestAmmos ( pTag3->m16, m16Zero, m16, M16_CLIP, allowZero ) &&  
                TestAmmos ( pTag3->grenade, grenadeZero, grenade, GRENADE_CLIP, allowZero ) &&
                TestAmmos ( pTag3->harpoon, harpoonZero, harpoon, 1, allowZero ) &&
                TestAmmos ( pTag3->rocket, rocketZero, rocket, 1, allowZero ) &&
             (  magnumZero != 0 || uziZero != 0 || riotZero != 0 || m16Zero != 0 ||
                grenadeZero != 0 || harpoonZero != 0 || rocketZero != 0 || allowZero ) )
        {
            countResult++;
            foundIndex++;
            int newValue = 0;
            newValue += TestAmmos ( pTag3->magnum, magnumZero, magnum, 1 );
            newValue += TestAmmos ( pTag3->uzi, uziZero, uzi, UZI_CLIP );
            newValue += TestAmmos ( pTag3->riotgun, riotZero, riot, 1 );
            newValue += TestAmmos ( pTag3->m16, m16Zero, m16, M16_CLIP );
            newValue += TestAmmos ( pTag3->grenade, grenadeZero, grenade, GRENADE_CLIP );
            newValue += TestAmmos ( pTag3->harpoon, harpoonZero, harpoon, 1 );
            newValue += TestAmmos ( pTag3->rocket, rocketZero, rocket, 1 );

            WORD *pAir = ( WORD * ) ( ( BYTE * ) pTag3 - TR3_OFFSET_AIR );
            if ( allowZero && newValue < 7 && *pAir == 1800 )
            {
                newValue = 7;
            }

            if ( newValue >= valueResult )
            {
                valueResult = newValue;
                pFound      = pCurrent;
            }

            if ( pFound == NULL )
            {
                pFound = pCurrent;
            }
        }
    }

    //
    //  With Full Mask
    if ( pFound == NULL && mask != 0xffff )
    {
        // pFound = SearchGunStructure ( pFrom, pUntil, health, magnum, uzi, riot, m16, grenade, harpoon, rocket, countResult, valueResult, 0xffff, true, false );
    }

    //  Allow Zeroes
    if ( pFound == NULL && ! allowZero && ! noGuns )
    {
        pFound = SearchGunStructure ( pFrom, pUntil, guns, magnum, uzi, riot, m16, grenade, harpoon, rocket, countResult, valueResult, mask, true, false );
    }

    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure (    int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = ( char *) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR2:
        {
            char *pBlockAddress = ( char *) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR2);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR3:
        {
            char *pBlockAddress = ( char *) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR3);
            char *pLastAddress  = pBlockAddress + ( NEXT_X40 - FIRST_X40 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure ( int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                WORD m16, WORD grenade, WORD harpoon,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR2:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR3:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR3);
            char *pLastAddress  = pBlockAddress + ( NEXT_X40 - FIRST_X40 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::SearchGunStructure ( int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                WORD m16, WORD grenade, WORD harpoon, WORD rocket,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR2:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
        case GAME_TRR3:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR3);
            char *pLastAddress  = pBlockAddress + ( NEXT_X40 - FIRST_X40 ) - 1;
            return SearchGunStructure ( pFirstAddress, pLastAddress,  guns, magnum, uzi, riot,
                m16, grenade, harpoon, rocket, countResult, valueResult, mask, allowZero, noGuns );
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTR9SaveGame::RelativeAddress ( const void *pAddress )
{
    if ( m_pBuffer != NULL )
    {
        return CTRXTools::RelativeAddress ( pAddress, m_pBuffer );
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::getBufferAddress()
{
    return m_pBuffer;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR9SaveGame::getBufferBackupAddress()
{
    return m_pBufferBackup;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR9SaveGame::getBufferLength()
{
    return m_iSaveLength;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::ReadFile ( const char *pFilename )
{
    Init();

    //
    CTRXGlobal::m_iUnchecked = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

    //
    fopen_s ( &m_hFile, pFilename, "rb" );
    if ( m_hFile != NULL )
    {
        m_iSaveLength = (int) fread ( m_pBuffer, 1, LEN_BUFFER, m_hFile );
        CloseOneFile ( &m_hFile );
        m_hFile = NULL;

        //
        if ( m_iSaveLength != TR123LEVELSIZE )
        {
            ZeroMemory ( m_pBuffer, LEN_BUFFER );
            m_iSaveLength = 0;
            return FALSE;
        }
        else
        {
            memcpy ( m_pBufferBackup, m_pBuffer, m_iSaveLength );
        }

        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::writeFile ( const char *pFilename )
{
    if ( m_iSaveLength > 0 && pFilename != NULL && strlen(pFilename) > 0 )
    {
        //  Just Rename Once
        static char szOriginal [ MAX_PATH ];
        static char szRenamed [ MAX_PATH ];

        //  32 Backup file
        for ( int i = CTRXGlobal::m_iTRRBackup - 1; i >= 1; i-- )
        {
            sprintf_s ( szOriginal, sizeof(szOriginal), "%s.bak.%d", pFilename, i - 1 );
            sprintf_s ( szRenamed, sizeof(szRenamed), "%s.bak.%d", pFilename, i );
            if ( PathFileExists ( szRenamed ) )
            {
                _unlink ( szRenamed );
            }

            if ( PathFileExists ( szOriginal ) )
            {
                rename ( szOriginal, szRenamed );
            }
        }

        //
        if( CTRXGlobal::m_iTRRBackup >= 1 )
        {
            sprintf_s ( szRenamed, sizeof(szRenamed), "%s.bak.%d", pFilename, 0 );
            if ( PathFileExists ( szRenamed ) )     //  Normally already renamed
            {
                _unlink ( szRenamed );
            }

            if ( PathFileExists ( pFilename ) )
            {
                rename ( pFilename, szRenamed );
            }
        }

        //
        fopen_s ( &m_hFile, pFilename, "wb" );
        if ( m_hFile != NULL )
        {
            size_t written = fwrite ( m_pBuffer, 1, m_iSaveLength, m_hFile );
            CloseOneFile ( &m_hFile );
            if ( written == m_iSaveLength )
            {
                memcpy ( m_pBufferBackup, m_pBuffer, m_iSaveLength );
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        return FALSE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::Load ( )
{
    int iCount      = 0;
    int tombraider  = 1;
    int block       = 0;
    for ( int i = FIRST_X30; i <= LAST_X30; i = i + LEN_31_X30 )
    {
        //
        int block = iCount;

        //
        if ( block >= NB_TR_BLOCKS )
        {
            block -= NB_TR_BLOCKS;
        }

        char *pBlockAddress = m_pBuffer + i;
        if ( iCount < NB_TR_BLOCKS )
        {
            m_TR1_Start [ block ]   = pBlockAddress;
            tombraider              = 1;
        }
        else
        {
            m_TR2_Start [ block ]   = pBlockAddress;
            tombraider              = 2;
        }

        //
        if ( TraceMode && iCount == 0 )
        {
            printf ( "TR1 -------------------\n" );
        }
        if ( TraceMode && iCount == NB_TR_BLOCKS )
        {
            printf ( "TR2 -------------------\n" );
        }

        //  OK
        WORD    *saveAddress = GetSaveAddress ( tombraider, block );
        BYTE    *levelAddress = GetBlockLevelAddress ( tombraider, block );
        WORD    save = GetSaveNumber ( tombraider, block );
        //  From 1 to n
        WORD    level = GetBlockLevelNumber ( tombraider, block );

        //
        WORD    health = * (WORD *) ( pBlockAddress );
        if  (  TraceMode )
        {
            const char *levelName;
            if ( iCount < NB_TR_BLOCKS )
            {
                levelName = GetLevelName ( tombraider, level );
            }
            else
            {
                levelName = GetLevelName ( tombraider, level );
            }

            printf ( "0x%06X = %d - Save (%lx): %d Level %d (%lx) (#%d) %s\n", i,
                health, RelativeAddress ( saveAddress ), save, level, (long) ( (char*)  levelAddress - m_pBuffer ), iCount, levelName );
            if ( GetRealHealthAddress(tombraider, block) != NULL )
            {
                printf ( "\tReal : %lx = %d\n", RelativeAddress ( GetRealHealthAddress(tombraider, block) ), GetRealHealth(tombraider, block) );
            }
        }

        if ( health != 0 )
        {
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR1);
            char *pLastAddress  = pBlockAddress + ( NEXT_X30 - FIRST_X30 ) - 1;

            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                char *pAddress = pBlockAddress + slot * sizeof(TABLE_TR1);

                //  TR1
                if ( iCount < NB_TR_BLOCKS )
                {
                    //  Store Address
                    m_TR1_Blocks [ block ] [ slot ] = pAddress;

                    TABLE_TR1 *pBlock = ( TABLE_TR1 *) ( GetBlockAddress ( tombraider, block, slot ) );
                    if  (  TraceMode )
                    {
                        TraceBlock(  tombraider, block, slot );
                    }

                    //
                    int countResult = 0;
                    int valueResult = 0;

                    //  Slot Uzed
                    if ( ( pBlock->gunmask & 1 ) == 1 )
                    {
                        int indexSearched = 0;
                        char *pFound = CTR9SaveGame::SearchGunStructure ( 1, iCount, pBlock->guns,
                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                            countResult, valueResult, pBlock->gunmask, false, false );
                        if ( TraceMode && pFound )
                        {
                            char *pStartBlock   = (char*) GetBlockStart ( tombraider, block );
                            char *pCurrentPos   = m_TR1_Blocks [ block ] [ slot ];
                            char *pHealth       = (char *) GetRealHealthAddress ( tombraider, block );
                            printf ( " F%d:0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X", valueResult,
                                        RelativeAddress ( pFound ),
                                        (long) ( pFound - m_TR1_Blocks [ block ] [ slot ] ),
                                        (long) ( pFound - (char*) GetBlockStart ( tombraider, block ) ),
                                        (long) ( pFound - m_pBuffer - TR1_OFFSET_AIR ),
                                        (long) ( pFound - m_pBuffer - TR1_OFFSET_STATE ),
                                        RelativeAddress ( pHealth ) );
                            if ( pHealth != NULL )
                            {
                                DWORD *pState = (DWORD *)(pHealth - 10 );
                                printf ( " - State %08lx", *pState );
                            }
                        }
                        if ( TraceMode ) printf ( "\n" );
                        if ( pFound && valueResult >= 1 )
                        {
                            GUN_TR1 *pTag1 = (GUN_TR1 *) pFound;
                            if ( valueResult >= 3 || ( valueResult >= 1 && ( pBlock->gunmask & 1 ) == 1 ) )
                            {
                                m_TR1_Guns [ block ] [ slot ] = pTag1;
                            }

                            if ( TraceMode && valueResult >= 1 )
                            {
                                TraceGun ( 1, iCount, slot );
                            }
                        }
                    }
                    else
                    {
                        if ( TraceMode ) printf ( "\n" );
                        // if ( slot != 0 )
                        {
                            m_TR1_Blocks [ block ] [ slot ] = NULL;
                        }
                    }
                }
                else
                {
                    //  Store Address
                    m_TR2_Blocks [ block ] [ slot ] = pAddress;

                    TABLE_TR2 *pBlock = ( TABLE_TR2 *) ( GetBlockAddress ( tombraider, block, slot ) );
                    if ( TraceMode )
                    {
                        TraceBlock (  tombraider, block, slot );
                    }

                    //
                    int countResult = 0;
                    int valueResult = 0;

                    if ( ( pBlock->gunmask & 1 ) == 1 )
                    {
                        int indexSearched = 0;
                        char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block,  pBlock->guns,
                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                            pBlock->m16, pBlock->grenade, pBlock->harpoon,
                            countResult, valueResult, pBlock->gunmask, false, false );
                        if ( TraceMode && pFound )
                        {
                            char *pStartBlock   = m_TR2_Start [ block ];
                            char *pCurrentPos   = m_TR2_Blocks [ block ] [ slot ];
                            char *pHealth       = (char *) GetRealHealthAddress ( tombraider, block );
                            printf ( " F%d:0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X", valueResult,
                                        RelativeAddress ( pFound ),
                                        (long) ( pFound - m_TR2_Blocks [ block ] [ slot ] ),
                                        (long) ( pFound - m_TR2_Start [ block ] ),
                                        (long) ( pFound - m_pBuffer - TR2_OFFSET_AIR ),
                                        (long) ( pFound - m_pBuffer - TR2_OFFSET_STATE ),
                                        RelativeAddress ( pHealth ) );
                            if ( pHealth != NULL )
                            {
                                DWORD *pState = (DWORD *)(pHealth - 10 );
                                printf ( " - State %08lx", *pState );
                            }
                        }
                        if ( TraceMode ) printf ( "\n" );

                        if ( pFound && valueResult >= 1 )
                        {
                            //  Store Tag
                            GUN_TR2 *pTag2 = (GUN_TR2 *) pFound;
                            if ( valueResult >= 6 || ( valueResult >= 1 && ( pBlock->gunmask & 1 ) == 1 ) )
                            {
                                m_TR2_Guns [ block ] [ slot ] = pTag2;
                            }

                            if ( TraceMode && valueResult >= 1 )
                            {
                                TraceGun ( tombraider, block, slot );
                            }
                        }
                    }
                    else
                    {
                        if ( TraceMode ) printf ( "\n" );
                        // if ( slot != 0 )
                        {
                            m_TR2_Blocks [ block ] [ slot ] = NULL;
                        }
                    }
                }
            }
        }
        iCount++;
    }

    if ( TraceMode ) printf ( "Count : %d\n", iCount );

    if ( TraceMode ) printf ( "TR3 -------------------\n" );

    iCount          = 0;
    block           = 0;
    tombraider      = 3;

    for ( int i = FIRST_X40; block < NB_TR3_BLOCKS; i = i + (NEXT_X40-FIRST_X40) )
    {
        char    *pBlockAddress          = m_pBuffer + i;
        m_TR3_Start [ block ]           = pBlockAddress;

        WORD    *saveAddress =  (WORD *) ( pBlockAddress - SAVE_X40 );
        WORD    *levelAddress =  (WORD *) ( pBlockAddress + (LEVEL_X40-FIRST_X40) );
        WORD    save = GetSaveNumber ( tombraider, block ); // *saveAddress;
        WORD    level = GetBlockLevelNumber ( tombraider, block ); // *levelAddress;
        WORD    value = * (WORD *) ( pBlockAddress );
        if ( TraceMode )
        {
            printf ( "0x%06X = %d - Save : %d Level %d (#%d) %s\n", i, value, save, level, block, GetLevelName ( tombraider, level ) );
            if ( GetRealHealthAddress(tombraider, block) != NULL )
            {
                printf ( "\tReal : %lx = %d\n", RelativeAddress ( GetRealHealthAddress(tombraider, block) ), GetRealHealth(tombraider, block) );
            }
        }

        if ( value != 0 )
        {
            char *pFirstAddress = pBlockAddress + (NB_OF_SLOTS + 1 ) * sizeof(TABLE_TR3);
            char *pLastAddress  = pBlockAddress + ( NEXT_X40 - FIRST_X40 ) - 1;

            int slot = 0;
            for ( int j = i; j <= i + LEN_31_X40; j = j + LEN_X40 )
            {
                char *pAddress = m_pBuffer + j;

                //  Store Address
                m_TR3_Blocks [ block ] [ slot ] = pAddress;

                TABLE_TR3 *pBlock = ( TABLE_TR3 * ) GetBlockAddress ( tombraider, block, slot );
                if ( TraceMode )
                {
                    TraceBlock ( tombraider, block, slot );
                }

                int countResult = 0;
                int valueResult = 0;
                if ( ( pBlock->gunmask & 1 ) == 1 )
                {
                    int indexSearched = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block, pBlock->guns,
                        pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                        pBlock->m16, pBlock->grenade, pBlock->harpoon, pBlock->rocket,
                        countResult, valueResult, pBlock->gunmask, false, false );
                    if ( TraceMode && pFound )
                    {
                        char *pStartBlock   = m_TR2_Start [ block ];
                        char *pCurrentPos   = m_TR2_Blocks [ block ] [ slot ];
                        char *pHealth       = (char *) GetRealHealthAddress ( tombraider, block );
                        printf ( " F%d:0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X - 0x%06X", valueResult,
                                    RelativeAddress ( pFound ),
                                    (long) ( pFound - m_TR3_Blocks [ block ] [ slot ] ),
                                    (long) ( pFound - m_TR3_Start [ block ] ),
                                    (long) ( pFound - m_pBuffer - TR3_OFFSET_AIR ),
                                    (long) ( pFound - m_pBuffer - TR2_OFFSET_STATE ),
                                    RelativeAddress ( pHealth ) );
                        if ( pHealth != NULL )
                        {
                            DWORD *pState = (DWORD *)(pHealth - 10 );
                            printf ( " - State %08lx", *pState );
                        }
                    }
                    if ( TraceMode ) printf ( "\n" );
                    if (  pFound && valueResult >= 1 )
                    {
                        GUN_TR3 *pTag3 = (GUN_TR3 *) pFound;
                        if ( valueResult >= 7 || ( valueResult >= 1 && ( pBlock->gunmask & 1 ) == 1 ) )
                        {
                            m_TR3_Guns [ block ] [ slot ] = pTag3;
                        }

                        if ( TraceMode && valueResult >= 1 )
                        {
                            TraceGun ( tombraider, block, slot );
                        }
                    }
                }
                else
                {
                    if ( TraceMode ) printf ( "\n" );
                    // if ( index != 0 )
                    {
                        m_TR3_Blocks [ block ] [ slot ] = NULL;
                    }
                }
                iCount++;
                slot++;
            }
        }
        block++;
    }

    //
    if ( TraceMode )
    {
        printf ( "Count : %d - Loop : %d\n", iCount, block );

        printf ( "Sizeof %d %d %d", (int) sizeof(TABLE_TR1), (int) sizeof(TABLE_TR2), (int) sizeof(TABLE_TR3) );
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetSaveNumber ( int tombraider, int block )
{
    WORD    *saveAddress = GetSaveAddress ( tombraider, block );
    if ( saveAddress != NULL )
    {
        return *saveAddress;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetSaveNumber ( int tombraider, int block, WORD saveNo )
{
    WORD    *saveAddress = GetSaveAddress ( tombraider, block );
    if ( saveAddress != NULL )
    {
        *saveAddress = saveNo;
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockLevelNumber ( int tombraider, int block )
{
    BYTE    *levelAddress = GetBlockLevelAddress ( tombraider, block );
    if ( levelAddress != NULL )
    {
        return *levelAddress;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::GetBlockElapsed ( int tombraider, int block )
{
    static char szElapsed [ MAX_PATH ];
    ZeroMemory ( szElapsed, sizeof(szElapsed) );
    DWORD *pElapsed = GetBlockElapsedAddress(tombraider,block);
    if ( pElapsed )
    {
        DWORD dwElapsed     = *pElapsed;
        DWORD dwSeconds     = dwElapsed / 30;
        DWORD dwMinutes     = dwSeconds / 60;
        DWORD dwHours       = dwMinutes / 60;
        dwSeconds           = dwSeconds - dwMinutes * 60;
        dwMinutes           = dwMinutes - dwHours * 60;
        sprintf_s ( szElapsed, sizeof(szElapsed), "%d:%02d:%02d", dwHours, dwMinutes, dwSeconds );
    }
    return szElapsed;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::GetBlockDistance ( int tombraider, int block )
{
    //  0000 0001   = 37.01 km      1 00 0000           0001 0000
    //  0000 4000   = 9.025 km      0 40 0000
    //  0000 2000   = 4.012 km      0 20 0000

    //  0000 1300   = 2.98 km       0 13 0000           0001 0011
    //  0000 1200   = 2.50 km       0 12 0000           0001 0010
    //  0000 1100   = 2.03 km       0 11 0000           0001 0001
    //  0000 1000   = 2.56 km       0 10 0000           0001 0000

    //  0000 0f00   = 2.09 km       0 0f 0000           0000 1111
    //  0000 0e00   = 2.61 km       0 0e 0000           0000 1110
    //  0000 0d00   = 1.14 km       0 0d 0000           0000 1101
    //  0000 0c00   = 1.67 km       0 0b 0000           0000 1100
    //  0000 0b00   = 1.19 km       0 0b 0000           0000 1011
    //  0000 0a00   = 1.72 km       0 0a 0000           0000 1010
    //  0000 0900   = 1.25 km       0 09 0000           0000 1001
    //  0000 0800   = 1.78 km       0 08 0000           0000 1000

    //  0000 0700   = 1.30 km       0 07 0000           0000 0111
    //  0000 0600   = 883 m         0 06 0000           0000 0110
    //  0000 0500   = 736 m         0 05 0000           0000 0101
    //  0000 0400   = 589 m         0 04 0000           0000 0100
    //  0000 0300   = 441 m         0 03 0000           0000 0011
    //  0000 0200   = 294 m         0 02 0000           0000 0010
    //  0000 0100   = 150 m         0 01 0000           0000 0001
    //  0010 0000   = 9 m           0 00 1000       
    //  0008 0000   = 4 m           0 00 0800       
    //  0002 0000   = 1 m           0 00 0200           0000 0000 0010

    static WORD meters0 [] =
    {
    //  0       1       2       3       4       5       6       7
        0,      0,      1,      1,      2,      2,      3,      3,
    //  8       9       a       b       c       d       e       f
        4,      4,      5,      5,      6,      6,      7,      8
    };

    static WORD meters1 [] =
    {
    //  0       1       2       3       4       5       6       7
        9,      150,    294,    441,    589,    736,    883,    1300,
    //  8       9       a       b       c       d       e       f
        1780,   1250,   1720,   1190,   1670,   1140,   2610,   2090
    };

    //
    static char szDistance [ MAX_PATH ];
    ZeroMemory ( szDistance, sizeof(szDistance) );
    DWORD *pDistance = GetBlockDistanceAddress(tombraider,block);
    if ( pDistance )
    {
        DWORD dwDistance    = *pDistance;

        //
        DWORD dwLow         = dwDistance & 0xffff;
        DWORD dwHigh        = ( dwDistance >> 16 ) & 0xffff;

        //
        DWORD dwByte0       = dwLow & 0xff;
        DWORD dwByte1       = ( dwLow >> 8 ) & 0xff;
        DWORD dwByte2       = dwHigh & 0xff;
        DWORD dwByte3       = ( dwHigh >> 8 ) & 0xff;

        //
#if 1
        long dwNormal       = ( dwDistance >> 9 ) & 0x0fffffff;
        DWORD dwMeter       = ( long ) ( ( double ) dwNormal * 1.155f );
        long dwKilo         = dwMeter / 1000;
        dwMeter             = dwMeter - 1000 * dwKilo;
#else
#endif
        //
        while ( dwMeter > 1000 )
        {
            dwKilo++;
            dwMeter -= 1000;
        }

        DWORD dwHecto       = dwMeter / 100;
        DWORD dwDeca        = dwMeter / 10;
        if ( dwKilo > 0 )
        {
            sprintf_s ( szDistance, sizeof(szDistance), "%d.%02d km", dwKilo, dwDeca );
        }
        else
        {
            sprintf_s ( szDistance, sizeof(szDistance), "%d m", dwMeter );
        }
    }
    return szDistance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
bool CTR9SaveGame::isKnown ( const char *position )
{
    TRR_HEALTH *pStruct = (TRR_HEALTH *) position;

#ifdef _DEBUG
    static char szDebugString [ MAX_PATH ];
    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( position, m_pBuffer );
    sprintf_s ( szDebugString, sizeof(szDebugString), 
        "Looking 0x%08x : 0x%04x 0x%04x 0x%04x 0x%04x ", 
        dwRelativeAddress, pStruct->words.w1, pStruct->words.w2, pStruct->words.w3, pStruct->words.w4 );
    OutputDebugString ( szDebugString );
#endif

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );
    for ( int i = 0; i < IndicatorsTRRTableBytesCount; i++ )
    {
        if ( IndicatorsTRRTableBytes [ i ].bEnd )
        {
            break;
        }

        if ( IndicatorsTRRTableBytes [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
        {
            continue;
        }

        if (    pStruct->bytes.b1 == IndicatorsTRRTableBytes [ i ].b1 &&
                pStruct->bytes.b2 == IndicatorsTRRTableBytes [ i ].b2 && 
                pStruct->bytes.b3 == IndicatorsTRRTableBytes [ i ].b3 )
        {
            strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRRTableBytes [ i ].szLabel );

#ifdef _DEBUG
            OutputDebugString ( "Match\n" );
#endif
            return true; 
        }
    }

    //
#ifdef _DEBUG
    OutputDebugString ( "\n" );
#endif
    return false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::getPositionLabel(const char *pHealth)
{
    int offset = offsetof(TRR_WORD_HEALTH,wRealHealth);

    const TRR_HEALTH *pStruct = (TRR_HEALTH *) ( pHealth - offset );

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    for ( int i = 0; i < IndicatorsTRRTableBytesCount; i++ )
    {
        if ( IndicatorsTRRTableBytes [ i ].bEnd )
        {
            break;
        }

        if ( IndicatorsTRRTableBytes [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
        {
            continue;
        }

        if (    pStruct->bytes.b1 == IndicatorsTRRTableBytes [ i ].b1 &&
                pStruct->bytes.b2 == IndicatorsTRRTableBytes [ i ].b2 &&
                pStruct->bytes.b3 == IndicatorsTRRTableBytes [ i ].b3 )
        {
            strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRRTableBytes [ i ].szLabel );
#ifdef _DEBUG
            static char szDebugString [ MAX_PATH ];
            DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pHealth, m_pBuffer );
            sprintf_s ( szDebugString, sizeof(szDebugString), 
                "Indicators 0x%08x : 0x%04x 0x%04x 0x%04x 0x%04x Found\n", 
                dwRelativeAddress, pStruct->words.w1 & 0xff, pStruct->words.w2 & 0xff, pStruct->words.w3, pStruct->words.w4 );
            OutputDebugString ( szDebugString );
#endif
            return m_szIndicatorLabel; 
        }
    }

//
#ifdef _DEBUG
    static char szDebugString [ MAX_PATH ];
    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pHealth, m_pBuffer );
    sprintf_s ( szDebugString, sizeof(szDebugString), 
        "Indicators 0x%08x : 0x%04x 0x%04x 0x%04x 0x%04x Not Found\n", 
        dwRelativeAddress, pStruct->words.w1 & 0xff, pStruct->words.w2 & 0xff, pStruct->words.w3, pStruct->words.w4 );
    OutputDebugString ( szDebugString );
#endif

    if ( *( (WORD *) pHealth ) == 1000 )
    {
        strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), "Full Health" );
    }

    if ( strlen(m_szIndicatorLabel) == 0 )
    {
        strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), "Range" );
    }

    return m_szIndicatorLabel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetRealHealthAddress ( int tombraider, int block )
{
#ifdef _DEBUG
    OutputDebugString ( "GetRealHealthAddress TR Remastered\n" );
#endif

    TR_HEALTH_RANGE *pRanges = NULL;
    char *pStart        = NULL;
    int iSubtract       = NULL;
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            pRanges     = TR1_HEALTH_RANGES;
            pStart      = (char*) GetBlockStart ( tombraider, block );
            iSubtract   = TR1_BLOCK_OFFSET;
            break;
        }
        case GAME_TRR2:
        {
            pRanges     = TR2_HEALTH_RANGES;
            pStart      = (char*) GetBlockStart ( tombraider, block );
            iSubtract   = TR2_BLOCK_OFFSET;
            break;
        }
        case GAME_TRR3:
        {
            pRanges     = TR3_HEALTH_RANGES;
            pStart      = (char*) GetBlockStart ( tombraider, block );
            iSubtract   = TR3_BLOCK_OFFSET;
            break;
        }

        default :
        {
            return NULL;
        }
    }

    //
    int iExtended = 0;
    if ( CTRXGlobal::m_iSearchExt )
    {
        iExtended   = 0x40;
    }

    //
    int levelNumber = GetBlockLevelNumber ( tombraider, block );
    if ( levelNumber < 1 || levelNumber > 32 )
    {
        return NULL;
    }

    //
    int levelIndex = levelNumber - 1;
    if ( pRanges [ levelIndex ].minOffset == 0 )
    {
        return NULL;
    }

    if ( pRanges [ levelIndex ].maxOffset == 0 )
    {
        pRanges [ levelIndex ].maxOffset = pRanges [ levelIndex ].minOffset;
    }
    
    //  Only One Position
    if ( pRanges [ levelIndex ].minOffset == ( pRanges [ levelIndex ].maxOffset + iExtended ) )
    {
        char *pHealth  = pStart - iSubtract + pRanges [ levelIndex ].minOffset;
        if ( *pHealth != 0 )
        {
            getPositionLabel ( pHealth );
            return ( WORD * ) pHealth;
        }
    }

    //
    //  Search Within Range
    for ( DWORD ir = pRanges [ levelIndex ].minOffset; ir <= pRanges [ levelIndex ].maxOffset + iExtended; ir++ )
    {
        //
        char *pCurrentHealth    = pStart - iSubtract + ir;
        int relative            = (int)( pCurrentHealth - m_pBuffer );

        //
        WORD *pHealth       = (WORD *) pCurrentHealth;
        BYTE *pFoundHealth  = NULL;
        //
        //  Chevk Indicators
        int offset          = offsetof(TRR_WORD_HEALTH,wRealHealth);
        char *pIndicators   = pCurrentHealth - offset;
        if ( isKnown ( pIndicators ) )
        {
            pFoundHealth = (BYTE *) pCurrentHealth;
        }
        //
        else if ( *pHealth == 1000 )
        {
            getPositionLabel ( pCurrentHealth );
            if ( strlen(m_szIndicatorLabel) == 0 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), "Full Health" );
            }
            pFoundHealth = (BYTE *) pHealth;
        }

        //
        //  Verify Position
        if ( pFoundHealth != NULL )
        {
            //  for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
            {
                TR9_POSITION *position = NULL;

                switch ( tombraider )
                {
                    case GAME_TRR1:
                    {
                        position = (TR9_POSITION *)(pFoundHealth - TR1_OFFSET_POS);
                        break;
                    }
                    case GAME_TRR2:
                    {
                        position = (TR9_POSITION *)(pFoundHealth - TR2_OFFSET_POS);
                        break;
                    }
                    case GAME_TRR3:
                    {
                        position = (TR9_POSITION *)(pFoundHealth - TR3_OFFSET_POS);
                        break;
                    }
                }

                DWORD dwSouthToNorth    = ( DWORD ) position->dwSouthToNorth;
                DWORD dwVertical        = ( DWORD ) position->dwVertical;
                DWORD dwWestToEast      = ( DWORD ) position->dwWestToEast;
                WORD wRoom              = position->wRoom;

                BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex,  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                if ( bCheck )
                {
                    //
                    return (WORD *) pFoundHealth;
                }
            }
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR9_POSITION *CTR9SaveGame::GetPositionAddress ( int tombraider, int block )
{
#ifdef _DEBUG
    OutputDebugString ( "GetPositionAddress TR Remastered\n" );
#endif

    BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
    if ( pHealth )
    {
        switch ( tombraider )
        {
            case GAME_TRR1:
            {
                return (TR9_POSITION *)(pHealth - TR1_OFFSET_POS);
                break;
            }
            case GAME_TRR2:
            {
                return (TR9_POSITION *)(pHealth - TR2_OFFSET_POS);
                break;
            }
            case GAME_TRR3:
            {
                return (TR9_POSITION *)(pHealth - TR3_OFFSET_POS);
                break;
            }
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTR9SaveGame::GetRealHealth ( int tombraider, int block )
{
    WORD *pHealth = GetRealHealthAddress ( tombraider, block );
    if ( pHealth != NULL )
    {
        return *pHealth;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetRealHealth ( int tombraider, int block, WORD value )
{
    WORD *pHealth = GetRealHealthAddress ( tombraider, block );
    if ( pHealth != NULL )
    {
        *pHealth = value;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::KillWillard ( int tombraider, int block )
{
    if ( tombraider == 3 && GetBlockLevelNumber (tombraider, block ) == 19 )
    {
        BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
        if ( pHealth != NULL )
        {
            BYTE *pWillard = (BYTE *)(pHealth+TR3_WILLARD_DEAD);
            if ( *pWillard == 0 )
            {
                *pWillard       = 1;
            }
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::KillTR1Boss ( int tombraider, int block )
{
    if ( tombraider == 1 && GetBlockLevelNumber (tombraider, block ) == 15 )
    {
        BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
        if ( pHealth != NULL )
        {
            WORD *pTr1Boss = (WORD *)(pHealth+TR1_BOSS);
            if ( *pTr1Boss > 0 && *pTr1Boss <= 500 )
            {
                *pTr1Boss       = 0;
            }
        }
    }
    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::IsKillEnabled ( int tombraider, int block )
{
    if ( tombraider == 1 && GetBlockLevelNumber (tombraider, block ) == 15 )
    {
        BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
        if ( pHealth != NULL )
        {
            WORD *pTr1Boss = (WORD *)(pHealth+TR1_BOSS);
            if ( *pTr1Boss > 0 && *pTr1Boss <= 500 )
            {
                return TRUE;
            }
        }
    }

    if ( tombraider == 3 && GetBlockLevelNumber (tombraider, block ) == 19 )
    {
        BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
        if ( pHealth != NULL )
        {
            BYTE *pWillard = (BYTE *)(pHealth+TR3_WILLARD_DEAD);
            if ( *pWillard == 0 )
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetSaveAddress ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    char *pStartBlock = (char*) GetBlockStart ( tombraider, block );

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            if ( pStartBlock != NULL )
            {
                WORD    *saveAddress = (WORD *) ( pStartBlock  - ( FIRST_X30 - SAVE_X30 ) );
                return  saveAddress;
            }
            break;
        }
        case GAME_TRR2:
        {
            if ( pStartBlock != NULL )
            {
                WORD    *saveAddress = (WORD *) ( pStartBlock - ( FIRST_X30 - SAVE_X30 ) );
                return  saveAddress;
            }
            break;
        }
        case GAME_TRR3:
        {
            if (  pStartBlock != NULL )
            {
                WORD    *saveAddress = (WORD *) ( pStartBlock - SAVE_X40 );
                return  saveAddress;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetBlockStart ( int tombraider, int block, bool bForce )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            void *pVoid = (void *) m_TR1_Start [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_pBuffer + TR1_BLOCK_START + TR1_BLOCK_OFFSET + block * GetBlockLength ( tombraider );
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            void *pVoid = (void *) m_TR2_Start [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_pBuffer + TR2_BLOCK_START + TR2_BLOCK_OFFSET + block * GetBlockLength ( tombraider );
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            void *pVoid = (void *) m_TR3_Start [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_pBuffer + TR3_BLOCK_START + TR3_BLOCK_OFFSET + block * GetBlockLength ( tombraider );
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetSlotsEnd ( int tombraider, int block, bool bForce )
{
    BYTE *pStart = ( BYTE* ) GetBlockStart ( tombraider, block, bForce );
    if ( pStart != NULL )
    {
        pStart = pStart + GetSlotLength(tombraider) * NB_OF_SLOTS;
    }

    return pStart;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return directly m_TRx_Blocks [ b ] [ s ]
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetBlockSlot ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot > NB_OF_SLOTS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case GAME_TRR1 : return  m_TR1_Blocks [ block ][ slot ];
        case GAME_TRR2 : return  m_TR2_Blocks [ block ][ slot ];
        case GAME_TRR3 : return  m_TR3_Blocks [ block ][ slot ];
    }
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return m_TRx_Blocks [ b ] [ s ] but iif null can return the theorical address
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetBlockAddress ( int tombraider, int block, int slot, bool bForce )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            void *pVoid = (void *) m_TR1_Blocks [ block ] [ slot ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_TR1_Start [ block ] + slot * GetSlotLength ( tombraider );
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            void *pVoid = (void *) m_TR2_Blocks [ block ] [ slot ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_TR2_Start [ block ] + slot * GetSlotLength ( tombraider );
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            void *pVoid = (void *) m_TR3_Blocks [ block ] [ slot ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }

            if ( bForce )
            {
                return m_TR3_Start [ block ] + slot * GetSlotLength ( tombraider );
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  return  m_TRX_Guns [ block ] [ slot ]
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetGunAddress ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            return (void *) m_TR1_Guns [ block ] [ slot ];
            break;
        }

        //
        case GAME_TRR2:
        {
            return (void *) m_TR2_Guns [ block ] [ slot ];
            break;
        }

        //
        case GAME_TRR3:
        {
            return (void *) m_TR3_Guns [ block ] [ slot ];
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  return  m_TRX_Guns [ block ] [ slot ]
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetBlockGuns ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot > NB_OF_SLOTS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case GAME_TRR1 : return  m_TR1_Guns [ block ][ slot ];
        case GAME_TRR2 : return  m_TR2_Guns [ block ][ slot ];
        case GAME_TRR3 : return  m_TR3_Guns [ block ][ slot ];
    }
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get Full Block Length
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetBlockLength ( int tombraider )
{
    switch ( tombraider )
    {
        case GAME_TRR1 : return TR1_BLOCK_LENGTH;
        case GAME_TRR2 : return TR2_BLOCK_LENGTH;
        case GAME_TRR3 : return TR3_BLOCK_LENGTH;
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get a Slot Length
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetSlotLength ( int tombraider )
{
    switch ( tombraider )
    {
        case GAME_TRR1 : return 0x30;
        case GAME_TRR2 : return 0x30;
        case GAME_TRR3 : return 0x40;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockIndicator ( int tombraider, int block )
{
    void *pStartBlock = GetBlockStart ( tombraider, block );
    if ( pStartBlock )
    {
        BYTE *pIndicator = (BYTE*) pStartBlock - TR_OFFSET_INDICATOR;
        return *pIndicator;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::GetBlockHexa ( int tombraider, int block, int slot, int iStart, int iEnd )
{
    static char szResult [ 0x40  * 4 ];
    ZeroMemory ( szResult, sizeof(szResult) );

    BYTE *pBlock = (BYTE * ) GetBlockAddress ( tombraider, block, slot );
    if ( pBlock )
    {
        int slotLength = GetSlotLength ( tombraider );
        if ( iStart < 0 )
        {
            iStart = 0;
        }
        if ( iEnd < 0 )
        {
            iEnd = slotLength;
        }
        for ( int i = iStart; i < iEnd; i++ )
        {
            size_t len = strlen(szResult);
            sprintf_s ( szResult + len, sizeof(szResult) - len, "%02X ", pBlock [ i ] );
        }
    }

    return szResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockLevelAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->level;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->level;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->level;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockMapAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->maps;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->maps;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->maps;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockObjectAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->objects;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->objects;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->objects;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockKeyAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->keys;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->keys;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->keys;
            }
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockSecretsBaseAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->tblsecrets;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->tblsecrets;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return pTable->tblsecrets;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockSecretsAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->secrets;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->secrets;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->secrets;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetBlockKillsAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->kills;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->kills;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->kills;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTR9SaveGame::GetBlockKills ( int tombraider, int block )
{
    //
    WORD    kills = 0;
    WORD *pKills = GetBlockKillsAddress ( tombraider, block );
    if ( pKills != NULL )
    {
        kills = *pKills;
    }
    return kills;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD *CTR9SaveGame::GetBlockDistanceAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->distance;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->distance;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->distance;
            }
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD *CTR9SaveGame::GetBlockElapsedAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->elapsed;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->elapsed;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->elapsed;
            }
            break;
        }
    }

    //
    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockCrystalAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            return NULL;
            break;
        }

        //
        case GAME_TRR2:
        {
            return NULL;
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->crystals;
            }
            break;
        }
    }

    //
    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockPickupAddress ( int tombraider, int block )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->pickups;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->pickups;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->pickups;
            }
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockPickup ( int tombraider, int block )
{
    //
    BYTE    pickups = 0;
    BYTE    *pPickup = GetBlockPickupAddress ( tombraider, block );
    if ( pPickup != NULL )
    {
        pickups = * pPickup;
    }

    //
    return pickups;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::countTag ( int tombraider, int block )
{
    int count = 0;

    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return count;
    }

    void * pAddress = NULL;
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            for ( int i = 0; i < NB_OF_SLOTS; i++ )
            {
                void *pCurrent = m_TR1_Guns [ block ] [ i ];
                if ( pCurrent  != NULL )
                {
                    if ( pAddress == NULL )
                    {
                        pAddress = pCurrent;
                        count++;
                    }

                    if ( pCurrent != pAddress )
                    {
                        count++;
                        pAddress = pCurrent;
                    }
                }
            }
            break;
        }
        case GAME_TRR2:
        {
            for ( int i = 0; i < NB_OF_SLOTS; i++ )
            {
                void *pCurrent = m_TR2_Guns [ block ] [ i ];
                if ( pCurrent  != NULL )
                {
                    if ( pAddress == NULL )
                    {
                        pAddress = pCurrent;
                        count++;
                    }

                    if ( pCurrent != pAddress )
                    {
                        count++;
                        pAddress = pCurrent;
                    }
                }
            }
            break;
        }
        case GAME_TRR3:
        {
            for ( int i = 0; i < NB_OF_SLOTS; i++ )
            {
                void *pCurrent = m_TR3_Guns [ block ] [ i ];
                if ( pCurrent  != NULL )
                {
                    if ( pAddress == NULL )
                    {
                        pAddress = pCurrent;
                        count++;
                    }

                    if ( pCurrent != pAddress )
                    {
                        count++;
                        pAddress = pCurrent;
                    }
                }
            }
            break;
        }
    }

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::GetLevelName ( int tombraider, int level )
{
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        case GAME_TR10:
        case GAME_TR15:
        {
            if ( level >= 1 && level <= sizeof(TR1LevelNames)/sizeof(const char*) )
            {
                return TR1LevelNames [ level - 1 ];
            }
            break;
        }
        //
        case GAME_TRR2:
        case GAME_TR20:
        case GAME_TR25:
        {
            if ( level >= 1 && level <= sizeof(TR2LevelNames)/sizeof(const char*) )
            {
                return TR2LevelNames [ level - 1 ];
            }
            break;
        }
        //
        case GAME_TRR3:
        case GAME_TR30:
        case GAME_TR35:
        {
            if ( level >= 1 && level <= sizeof(TR3LevelNames)/sizeof(const char*) )
            {
                return TR3LevelNames [ level - 1 ];
            }
            break;
        }
        //
        case GAME_TRR4:
        case GAME_TR40:
        case GAME_TR45:
        {
            if ( level >= 1 && level <= sizeof(TR4LevelNames)/sizeof(const char*) )
            {
                return TR4LevelNames [ level - 1 ];
            }
            break;
        }
        //
        case GAME_TRR5:
        case GAME_TR50:
        case GAME_TR55:
        {
            if ( level >= 1 && level <= sizeof(TR5LevelNames)/sizeof(const char*) )
            {
                return TR5LevelNames [ level - 1 ];
            }
            break;
        }

        //
        case GAME_TR19:
        case GAME_TR29:
        case GAME_TR39:
        case GAME_TR49:
        case GAME_TR59:
        case GAME_TR99:
        {
            return GetCustomLevelName();
            break;
        }
    }

    return "";
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetLevelCount ( int tombraider )
{
    switch ( tombraider )
    {
        case GAME_TRR1: return sizeof(TR1Secrets);
        case GAME_TRR2: return sizeof(TR2Secrets);
        case GAME_TRR3: return sizeof(TR3Secrets);
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTR9SaveGame::GetTRPlus ( int tombraider )
{
    if ( m_pBuffer == NULL )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            return * ( DWORD * ) ( m_pBuffer + TR1_GAME_PLUS );
            break;
        }
        case GAME_TRR2:
        {
            return * ( DWORD * ) ( m_pBuffer + TR2_GAME_PLUS );
            break;
        }
        case GAME_TRR3:
        {
            return * ( DWORD * ) ( m_pBuffer + TR3_GAME_PLUS );
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetTRPlus ( int tombraider, DWORD value )
{
    if ( m_pBuffer == NULL )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            DWORD current = * ( DWORD * ) ( m_pBuffer + TR1_GAME_PLUS );
            if ( current == 0 || current == 1 )
            {
                * ( DWORD * ) ( m_pBuffer + TR1_GAME_PLUS ) = value;
            }
            break;
        }
        case GAME_TRR2:
        {
            DWORD current = * ( DWORD * ) ( m_pBuffer + TR2_GAME_PLUS );
            if ( current == 0 || current == 1 )
            {
                * ( DWORD * ) ( m_pBuffer + TR2_GAME_PLUS ) = value;
            }
            break;
        }
        case GAME_TRR3:
        {
            DWORD current = * ( DWORD * ) ( m_pBuffer + TR3_GAME_PLUS );
            if ( current == 0 || current == 1 )
            {
                * ( DWORD * ) ( m_pBuffer + TR3_GAME_PLUS ) = value;
            }
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::IsGamePlus ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus = (BYTE*) ( pBlockAddress - TR1_SAVE_PLUS );
                return *plus;
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus      = (BYTE*) ( pBlockAddress - TR2_SAVE_PLUS );
                return *plus;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            char *pBlockAddress = (char*) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus = (BYTE*) ( pBlockAddress - TR3_SAVE_PLUS );
                return *plus;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetGamePlus ( int tombraider, int block, BYTE value )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus = (BYTE*) ( pBlockAddress - TR1_SAVE_PLUS );
                if ( value )
                {
                    *plus   = 1;
                }
                else
                {
                    *plus   = 0;
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus      = (BYTE*) ( pBlockAddress - TR2_SAVE_PLUS );
                if ( value )
                {
                    *plus   = 1;
                }
                else
                {
                    *plus   = 0;
                }
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockStart ( tombraider, block );
            if ( pBlockAddress )
            {
                BYTE *plus = (BYTE*) ( pBlockAddress - TR3_SAVE_PLUS );
                if ( value )
                {
                    *plus   = 1;
                }
                else
                {
                    *plus   = 0;
                }
            }
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//  return the first TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer that has a mask & 1
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetDefaultBlockEntry ( int tombraider, int block, int *piSlot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    int levelNumber = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //  Normally The Slot is always the 24
        case GAME_TRR1:
        {
            void *pReturned = NULL;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR1 *pBlock = ( TABLE_TR1 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) )
                {
                    pReturned   = pBlock;
                    *piSlot     = slot;
                }
            }
            return pReturned;
            break;
        }

        //
        case GAME_TRR2:
        {
            //  Normally The Slot is always the levelindex from 0
            void *pReturned = NULL;
            TABLE_TR2 *pBlockGood   = NULL;
            int iGoodGun        = -1;
            int iGoodResult     = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR2 *pBlock = ( TABLE_TR2 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) )
                {
                    pReturned   = pBlock;
                    *piSlot     = slot;
                }
            }
            return pReturned;
            break;
        }

        //
        case GAME_TRR3:
        {
            //  Normally The Slot is always the levelindex from 0
            void *pReturned = NULL;
            TABLE_TR3 *pBlockGood   = NULL;
            int iGoodGun        = -1;
            int iGoodResult     = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR3 *pBlock = ( TABLE_TR3 * ) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) )
                {
                    pReturned   = pBlock;
                    *piSlot     = slot;
                }
            }
            return pReturned;
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//  return the TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer
//  This will search the last occurrence
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::SearchBlockEntry ( int tombraider, int block, int *piSlot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlockGood   = NULL;
            int iGoodResult         = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR1 *pBlock = ( TABLE_TR1 *) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block, pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pBlockGood  = pBlock;
                        *piSlot     = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pBlockGood )
            {
                return pBlockGood;
            }

            //
            return GetDefaultBlockEntry ( tombraider, block, piSlot);
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlockGood   = NULL;
            int iGoodResult         = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR2 *pBlock = ( TABLE_TR2 *) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block,  pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            pBlock->m16, pBlock->grenade, pBlock->harpoon,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pBlockGood  = pBlock;
                        *piSlot     = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pBlockGood )
            {
                return pBlockGood;
            }

            //
            return GetDefaultBlockEntry ( tombraider, block, piSlot );
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlockGood   = NULL;
            int iGoodResult         = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR3 *pBlock = ( TABLE_TR3 * ) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block, pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            pBlock->m16, pBlock->grenade, pBlock->harpoon, pBlock->rocket,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pBlockGood  = pBlock;
                        *piSlot     = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pBlockGood )
            {
                return pBlockGood;
            }

            //
            return GetDefaultBlockEntry ( tombraider, block, piSlot );
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::GetDefaultGunEntry ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR1 *pBlock = ( TABLE_TR1 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) && GetGunAddress ( tombraider, block, slot ) != NULL )
                {
                    return GetGunAddress ( tombraider, block, slot );
                }
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR2 *pBlock = ( TABLE_TR2 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) && GetGunAddress ( tombraider, block, slot ) != NULL )
                {
                    return GetGunAddress ( tombraider, block, slot );
                }
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR3 *pBlock = ( TABLE_TR3 * ) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock && ( ( pBlock->gunmask & 1 ) == 1 ) && GetGunAddress ( tombraider, block, slot ) != NULL )
                {
                    return GetGunAddress ( tombraider, block, slot );
                }
            }
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
/////////////////////////////////////////////////////////////////////////////
void *CTR9SaveGame::SearchGunEntry ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider = 2;
        block       = block - NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pGood         = NULL;
            int iGoodSlot       = -1;
            int iGoodResult     = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR1 *pBlock = ( TABLE_TR1 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block, pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    //
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pGood       = pFound;
                        iGoodSlot   = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pGood )
            {
                return pGood;
            }

            //
            return GetDefaultGunEntry ( tombraider, block );
            break;
        }

        //
        case GAME_TRR2:
        {
            char *pGood         = NULL;
            int iGoodSlot       = -1;
            int iGoodResult     = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR2 *pBlock = ( TABLE_TR2 *) ( GetBlockAddress ( tombraider, block, slot ) );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block,  pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            pBlock->m16, pBlock->grenade, pBlock->harpoon,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    //
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pGood       = pFound;
                        iGoodSlot   = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pGood )
            {
                return pGood;
            }

            //
            return GetDefaultGunEntry ( tombraider, block );
            break;
        }

        //
        case GAME_TRR3:
        {
            char *pGood         = NULL;
            int iGoodSlot       = -1;
            int iGoodResult     = -1;
            for ( int slot = 0; slot < NB_OF_SLOTS; slot++ )
            {
                TABLE_TR3 *pBlock = ( TABLE_TR3 * ) GetBlockAddress ( tombraider, block, slot );
                if ( pBlock )
                {
                    int countResult     = 0;
                    int valueResult     = 0;
                    char *pFound = CTR9SaveGame::SearchGunStructure ( tombraider, block, pBlock->guns,
                                            pBlock->magnum, pBlock->uzi, pBlock->riotgun,
                                            pBlock->m16, pBlock->grenade, pBlock->harpoon,
                                            countResult, valueResult, pBlock->gunmask, false, false );
                    if ( pFound != NULL && valueResult >= 1 && valueResult >= iGoodResult )
                    {
                        pGood       = pFound;
                        iGoodSlot   = slot;
                        iGoodResult = valueResult;
                    }
                }
            }

            if ( pGood )
            {
                return pGood;
            }

            //
            return GetDefaultGunEntry ( tombraider, block );
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSlotMedipakUsedAddress(int tombraider, int block, int slot)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }
    
    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = (TABLE_TR1 *) ( m_TR1_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->mediused;
            }
            break;
        }

        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = (TABLE_TR2 *) ( m_TR2_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->mediused;
            }
            break;
        }

        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = (TABLE_TR3 *) ( m_TR3_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->mediused;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSlotPickupAddress(int tombraider, int block, int slot)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = (TABLE_TR1 *) ( m_TR1_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->pickups;
            }
            break;
        }

        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = (TABLE_TR2 *) ( m_TR2_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->pickups;
            }
            break;
        }

        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = (TABLE_TR3 *) ( m_TR3_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->pickups;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetSlotKillsAddress(int tombraider, int block, int slot)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = (TABLE_TR1 *) ( m_TR1_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->kills;
            }
            break;
        }

        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = (TABLE_TR2 *) ( m_TR2_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->kills;
            }
            break;
        }

        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = (TABLE_TR3 *) ( m_TR3_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->kills;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSlotCrystalAddress ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            break;
        }

        case GAME_TRR2:
        {
            break;
        }

        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = (TABLE_TR3 *) ( m_TR3_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return &pBlock->colCrystals;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSlotTreasureAddress ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            return NULL;
            break;
        }

        //
        case GAME_TRR2:
        {
            return NULL;
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = (TABLE_TR3 *) ( m_TR3_Blocks [ block ] [ slot ] );
            if ( pBlock != NULL )
            {
                return pBlock->treasure;
            }
            break;
        }
    }

    return NULL;

}
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetBlockObject ( int tombraider, int block, int object )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress = pBlockAddress + object;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress = pBlockAddress + object;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress = pBlockAddress + object;
                return *pAddress;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetKey ( int tombraider, int block, int key )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                return *pAddress;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Object / Items / Keys / Map / Treasures
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetObjectBaseAddress ( int tombraider, int block )
{
    return GetBlockMapAddress ( tombraider, block );
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetMap ( int tombraider, int block, int map )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                return *pAddress;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                return *pAddress;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::GetTreasure ( int tombraider, int block, int slot, int treasure )
{
    BYTE *pTreasure = GetSlotTreasureAddress ( tombraider, block, slot );
    if ( pTreasure != NULL && treasure >= 0 && treasure < 4 )
    {
        return pTreasure [ treasure ];
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockObject ( int tombraider, int block, int object, int val )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + object;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + object;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockObjectAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + object;
                *pAddress = val;
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetKey ( int tombraider, int block, int key, int val )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockKeyAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + key;
                *pAddress = val;
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetMap ( int tombraider, int block, int map, int val )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                *pAddress = val;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pBlockAddress = (char *) GetBlockMapAddress ( tombraider, block );
            if ( pBlockAddress )
            {
                char *pAddress  = pBlockAddress + map;
                *pAddress = val;
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetTreasure ( int tombraider, int block, int slot, int treasure, int val )
{
    BYTE *pTreasure = GetSlotTreasureAddress ( tombraider, block, slot );
    if ( pTreasure != NULL && treasure >= 0 && treasure < 4 )
    {
        pTreasure [ treasure ] = val;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetBlockAmmosHitsAddress(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosHits;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosHits;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosHits;
            }
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Ammos Used Follow by Hit and Kill
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetBlockAmmosUsedAddress(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosUsed;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosUsed;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->ammosUsed;
            }
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get Medipak address Used
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockMedipakUsedAddress(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1_STATS *pTable = (TABLE_TR1_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->mediused;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            TABLE_TR2_STATS *pTable = (TABLE_TR2_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->mediused;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            TABLE_TR3_STATS *pTable = (TABLE_TR3_STATS *) GetSlotsEnd ( tombraider, block, true );
            if ( pTable != NULL )
            {
                return &pTable->mediused;
            }
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockCrystal(int tombraider, int block)
{
    BYTE *pCrystal = GetBlockCrystalAddress ( tombraider, block );
    if ( pCrystal )
    {
        return *pCrystal;
    }
    return 0;
    
}

//
/////////////////////////////////////////////////////////////////////////////
//  Cnt Addr is relative for TR3 like stats and fixed otherwise
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetBlockSecretsCurrentAddress(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR1_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR1_Secrets_Offsets [ levelindex ];
            }

            BYTE *pBaseAddress  = GetBlockSecretsBaseAddress( tombraider,  block );
            if ( pBaseAddress )
            {
                BYTE *pStats            = ( BYTE * ) ( pBaseAddress + offset );
                return pStats;
            }
            break;
        }

        case GAME_TRR2:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR2_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR2_Secrets_Offsets [ levelindex ];
            }

            BYTE *pBaseAddress  = GetBlockSecretsBaseAddress( tombraider,  block );
            if ( pBaseAddress )
            {
                BYTE *pStats            = ( BYTE * ) ( pBaseAddress + offset );
                return pStats;
            }
            break;
        }

        case GAME_TRR3:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR3_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR3_Secrets_Offsets [ levelindex ];
            }

            BYTE *pBaseAddress  = GetBlockSecretsBaseAddress( tombraider,  block );
            if ( pBaseAddress )
            {
                BYTE *pStats            = ( BYTE * ) ( pBaseAddress + offset );
                return pStats;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::CompareSecretsCompleted ( int tombraider, int block, BYTE *pStartCompleted )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return -1;
    }

    if ( pStartCompleted == NULL )
    {
        return -1;
    }

    int countNotZero = 0;

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart && pStartCompleted )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR1Secrets); levelIndex++ )
                {
                    WORD offset             = TR1_Secrets_Offsets [ levelIndex ];
                    BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                    BYTE *pCompleted        = pStartCompleted + offset;
                    if ( *pStats != *pCompleted )
                    {
                        return -1;
                    }
                    if ( *pStats != 0 )
                    {
                        countNotZero++;
                    }
                }

                return countNotZero;
            }

            break;
        }
        //
        case GAME_TRR2:
        {
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart && pStartCompleted )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR2Secrets); levelIndex++ )
                {
                    WORD offset             = TR2_Secrets_Offsets [ levelIndex ];
                    BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                    BYTE *pCompleted        = pStartCompleted + offset;
                    if ( *pStats != *pCompleted )
                    {
                        return -1;
                    }
                    if ( *pStats != 0 )
                    {
                        countNotZero++;
                    }
                }

                return countNotZero;
            }

            break;
        }
        //
        case GAME_TRR3:
        {
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart && pStartCompleted )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR3Secrets); levelIndex++ )
                {
                    WORD offset             = TR3_Secrets_Offsets [ levelIndex ];
                    BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                    BYTE *pCompleted        = pStartCompleted + offset;
                    if ( *pStats != *pCompleted )
                    {
                        return -1;
                    }

                    if ( *pStats != 0 )
                    {
                        countNotZero++;
                    }
                }

                return countNotZero;
            }

            break;
        }
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return First Address of Secrets Completed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::SearchSecretsCompleted ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return NULL;
    }

    //
    int levelindex      = GetBlockLevelNumber ( tombraider, block ) - 1;

    //  Minimum non zero
    //  For 0,1 : 0
    //  For 2,3 : 1
    int MinimumNonZero  = levelindex / 2;

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            void *pGunAddress   = SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pCompleted        = ( BYTE * ) ( (BYTE * ) pGunAddress + TR1_OFFSET_SEC_GUN );
                if ( CompareSecretsCompleted ( tombraider, block, pCompleted  ) < 0 )
                {
                    for ( int i = 0; i < 48; i++ )
                    {
                        if ( CompareSecretsCompleted ( tombraider, block, pCompleted + i ) >= MinimumNonZero )
                        {
                            // return  pCompleted + i;
                        }
                    }
                    return NULL;
                }
                return pCompleted;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            void *pGunAddress   = SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pCompleted        = ( BYTE * ) ( (BYTE * ) pGunAddress + TR2_OFFSET_SEC_GUN );
                BYTE *pStatAddress      = GetBlockSecretsBaseAddress ( tombraider, block );
                if ( CompareSecretsCompleted ( tombraider, block, pCompleted  ) < 0 )
                {
                    for ( int i = 0; i < 48; i++ )
                    {
                        if ( CompareSecretsCompleted ( tombraider, block, pCompleted + i ) >= MinimumNonZero  )
                        {
                            return pCompleted + i;
                        }
                    }

                    return NULL;
                }
                return pCompleted;
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            void *pGunAddress   = SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pCompleted        = ( BYTE * ) ( (BYTE * ) pGunAddress + TR3_OFFSET_SEC_GUN );
                BYTE *pStatAddress      = GetBlockSecretsBaseAddress ( tombraider, block );
                if ( CompareSecretsCompleted ( tombraider, block, pCompleted  ) < 0 )
                {
                    for ( int i = 0; i < 48; i++ )
                    {
                        if ( CompareSecretsCompleted ( tombraider, block, pCompleted + i ) >=  MinimumNonZero )
                        {
                            // return  pCompleted + i;
                        }
                    }
                    return NULL;
                }
                return pCompleted;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Enable address is relative to Gun for TR3 other wise it is fixed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSecretsCompletedBaseAddress (int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            BYTE *pAddress  = SearchSecretsCompleted ( tombraider, block );
            if ( pAddress )
            {
                return pAddress;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            BYTE *pAddress  = SearchSecretsCompleted ( tombraider, block );
            if ( pAddress )
            {
                return pAddress;
            }
            break;
        }

        //  
        case GAME_TRR3:
        {
            BYTE *pAddress  = SearchSecretsCompleted ( tombraider, block );
            if ( pAddress )
            {
                return pAddress;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Enable address is relative to Gun for TR3 other wise it is fixed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR9SaveGame::GetSecretsCompletedAddress(int tombraider, int block, int iLevel)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    WORD    levelindex = 0;
    if ( iLevel < 0 )
    {
        levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;
    }
    else
    {
        levelindex = iLevel;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR1_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR1_Secrets_Offsets [ levelindex ];
            }

            BYTE *pAddress  = GetSecretsCompletedBaseAddress ( tombraider, block );
            if ( pAddress )
            {
                BYTE *pCompleted        = pAddress + offset;
                return pCompleted;
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR2_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR2_Secrets_Offsets [ levelindex ];
            }

            BYTE *pAddress  = GetSecretsCompletedBaseAddress ( tombraider, block );
            if ( pAddress )
            {
                BYTE *pCompleted        = pAddress + offset;
                return pCompleted;
            }
            break;
        }

        //  
        case GAME_TRR3:
        {
            WORD offset     =  0;
            int maxLevel    = sizeof(TR3_Secrets_Offsets)/sizeof(WORD);
            if ( levelindex >= 0 && levelindex < maxLevel )
            {
                offset  = TR3_Secrets_Offsets [ levelindex ];
            }

            BYTE *pAddress  = GetSecretsCompletedBaseAddress ( tombraider, block );
            if ( pAddress )
            {
                BYTE *pCompleted        = pAddress + offset;
                return pCompleted;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetSecretsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR1Secrets) )
            {
                return TR1Secrets[levelindex];
            }
            break;
        }
        case GAME_TRR2:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR2Secrets) )
            {
                return TR2Secrets[levelindex];
            }
            break;
        }
        case GAME_TRR3:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR3Secrets) )
            {
                return TR3Secrets[levelindex];
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR9SaveGame::GetKillsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR1Kills) )
            {
                return TR1Kills[levelindex];
            }
            break;
        }
        case GAME_TRR2:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR2Kills) )
            {
                return TR2Kills[levelindex];
            }
            break;
        }
        case GAME_TRR3:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR3Kills) )
            {
                return TR3Kills[levelindex];
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR9SaveGame::GetPickupsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR1Pickup) )
            {
                return TR1Pickup[levelindex];
            }
            break;
        }
        case GAME_TRR2:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR2Pickup) )
            {
                return TR2Pickup[levelindex];
            }
            break;
        }
        case GAME_TRR3:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR3Pickup) )
            {
                return TR3Pickup[levelindex];
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR9SaveGame::GetCrystalsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            break;
        }
        case GAME_TRR2:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR2Crystals) )
            {
                return TR2Crystals[levelindex];
            }
            break;
        }
        case GAME_TRR3:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR3Crystals) )
            {
                return TR3Crystals[levelindex];
            }
            break;
        }
    }

    return 0;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::GetBlocksSecrets ( int tombraider, int block, BYTE vector[32] )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            BYTE *pStatsStart           = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart )
            {
                int maxLevel    = sizeof(TR1_Secrets_Offsets)/sizeof(WORD);
                for ( int levelIndex = 0; levelIndex < sizeof(TR1Secrets); levelIndex++ )
                {
                    WORD offset     =  0;
                    if ( levelIndex >= 0 && levelIndex < maxLevel )
                    {
                        offset  = TR1_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats    = ( BYTE * ) ( pStatsStart + + offset );
                        vector [ levelIndex ]   = *pStats;
                    }
                }
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            BYTE *pStatsStart           = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart )
            {
                int maxLevel    = sizeof(TR2_Secrets_Offsets)/sizeof(WORD);
                for ( int levelIndex = 0; levelIndex < sizeof(TR2Secrets); levelIndex++ )
                {
                    WORD offset     =  0;
                    if ( levelIndex >= 0 && levelIndex < maxLevel )
                    {
                        offset  = TR2_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats    = ( BYTE * ) ( pStatsStart + + offset );
                        vector [ levelIndex ]   = *pStats;
                    }
                }
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            //
            BYTE *pStatsStart           = GetBlockSecretsBaseAddress ( tombraider, block );
            if ( pStatsStart )
            {
                int maxLevel    = sizeof(TR3_Secrets_Offsets)/sizeof(WORD);
                for ( int levelIndex = 0; levelIndex < sizeof(TR3Secrets); levelIndex++ )
                {
                    WORD offset     =  0;
                    if ( levelIndex >= 0 && levelIndex < maxLevel )
                    {
                        offset  = TR3_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats    = ( BYTE * ) ( pStatsStart + + offset );
                        vector [ levelIndex ]   = *pStats;
                    }
                }
            }
            break;
        }
    }
    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockSecretsAcquiredBits(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            if ( pAcquired )
            {
                return CTRXTools::CountBitsB ( *pAcquired );
            }
            break;
        }

        case GAME_TRR2:
        {
            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            if ( pAcquired )
            {
                return CTRXTools::CountBitsB ( *pAcquired );
            }
            break;
        }

        case GAME_TRR3:
        {
            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            if ( pAcquired )
            {
                return CTRXTools::CountBitsB ( *pAcquired );
            }
            break;
        }
    }

    return 0;
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockSecretsAcquired(int tombraider, int block)
{
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            return GetBlockSecretsAcquiredBits ( tombraider, block );
        }
        case GAME_TRR2:
        {
            return GetBlockSecretsAcquiredBits ( tombraider, block );
        }
        case GAME_TRR3:
        {
            return GetBlockSecretsAcquiredBits ( tombraider, block );
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetSecretsCompletedBits(int tombraider, int block, int iLevel)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    WORD    levelindex = 0;
    if ( iLevel < 0 )
    {
        levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;
    }
    else
    {
        levelindex = iLevel;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return CTRXTools::CountBitsB ( *pCompleted );
            }
            break;
        }

        case GAME_TRR2:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return CTRXTools::CountBitsB ( *pCompleted );
            }
            break;
        }

        case GAME_TRR3:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return CTRXTools::CountBitsB (  *pCompleted );
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetSecretsCompletedValue(int tombraider, int block, int iLevel)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    //
    WORD    levelindex = 0;
    if ( iLevel < 0 )
    {
        levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;
    }
    else
    {
        levelindex = iLevel;
    }

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return *pCompleted;
            }
            break;
        }

        case GAME_TRR2:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return *pCompleted;
            }
            break;
        }

        case GAME_TRR3:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return *pCompleted;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetSecretsCompleted(int tombraider, int block, int iLevel)
{
    //
    WORD    levelindex = 0;
    if ( iLevel < 0 )
    {
        levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;
    }
    else
    {
        levelindex = iLevel;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            return GetSecretsCompletedValue ( tombraider, block, levelindex );
        }

        case GAME_TRR2:
        {
            return GetSecretsCompletedValue ( tombraider, block, levelindex );
        }

        case GAME_TRR3:
        {
            return GetSecretsCompletedValue ( tombraider, block, levelindex );
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockPickup(int tombraider, int block, BYTE pickup)
{
    BYTE *pPickup = GetBlockPickupAddress ( tombraider, block );
    if ( pPickup )
    {
        *pPickup    = pickup;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockKills(int tombraider, int block, WORD kills)
{
    WORD *pKill = GetBlockKillsAddress ( tombraider, block );
    if ( pKill )
    {
        *pKill  = kills;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockCrystal(int tombraider, int block, BYTE crystals)
{
    BYTE *pCrystal = GetBlockCrystalAddress ( tombraider, block );
    if ( pCrystal )
    {
        *pCrystal   = crystals;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockSecretsCurrentValue(int tombraider, int block)
{
    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            BYTE *pCount                = GetBlockSecretsCurrentAddress ( tombraider, block );
            if ( pCount )
            {
                int bits                = *pCount;
                return bits;
            }
            break;
        }

        case GAME_TRR2:
        {
            BYTE *pCount                = GetBlockSecretsCurrentAddress ( tombraider, block );
            if ( pCount )
            {
                int bits                = *pCount;
                return bits;
            }
            break;
        }

        //  TR3
        case GAME_TRR3:
        {
            BYTE *pCount                = GetBlockSecretsCurrentAddress ( tombraider, block );
            if ( pCount )
            {
                int bits                = *pCount;
                return bits;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetBlockSecretsCurrent(int tombraider, int block)
{
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            return GetBlockSecretsCurrentValue ( tombraider, block );
        }
        case GAME_TRR2:
        {
            return GetBlockSecretsCurrentValue ( tombraider, block );
        }
        case GAME_TRR3:
        {
            return GetBlockSecretsCurrentValue ( tombraider, block );
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetSecretsMaximum(int tombraider, int block)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0 ;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR1Secrets) )
            {
                return TR1Secrets [ levelindex ];
            }
            break;
        }
        case GAME_TRR2:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR2Secrets) )
            {
                return TR2Secrets [ levelindex ];
            }
            break;
        }
        case GAME_TRR3:
        {
            if ( levelindex >= 0 && levelindex < sizeof(TR3Secrets) )
            {
                return TR3Secrets [ levelindex ];
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockSecretsAcquiredAll(int tombraider, int block, int excludeLevel )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    //  
    WORD    currentLevelIndex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            //
            int maxLevel    = sizeof(TR1_Secrets_Offsets)/sizeof(WORD);

            //  Stats
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            BYTE *pAddress      = SearchSecretsCompleted ( tombraider, block );
            if ( pStatsStart && pAddress )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR1Secrets) && levelIndex <= currentLevelIndex; levelIndex++ )
                {
                    if ( levelIndex < maxLevel && levelIndex != excludeLevel )
                    {
                        WORD offset             = TR1_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                        BYTE *pCompleted        = pAddress + offset;

                        if ( levelIndex == currentLevelIndex )
                        {
                            *pStats                 = 0;
                            *pCompleted             = 0;
                        }
                        else
                        {
                            *pStats                 = TR1Secrets [ levelIndex ];
                            *pCompleted             = TR1Secrets [ levelIndex ];
                        }
                    }
                }
            }
            break;
        }

        case GAME_TRR2:
        {
            //
            int maxLevel    = sizeof(TR2_Secrets_Offsets)/sizeof(WORD);

            //  Stats
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            BYTE *pAddress      = SearchSecretsCompleted ( tombraider, block );
            if ( pStatsStart && pAddress )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR2Secrets) && levelIndex <= currentLevelIndex; levelIndex++ )
                {
                    if ( levelIndex < maxLevel && levelIndex != excludeLevel )
                    {
                        WORD offset             = TR2_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                        BYTE *pCompleted        = pAddress + offset;

                        if ( levelIndex == currentLevelIndex )
                        {
                            *pStats                 = 0;
                            *pCompleted             = 0;
                        }
                        else
                        {
                            *pStats                 = TR2Secrets [ levelIndex ];
                            *pCompleted             = TR2Secrets [ levelIndex ];
                        }
                    }
                }
            }
            break;
        }

        //  Set Stats
        case GAME_TRR3:
        {
            //
            int maxLevel    = sizeof(TR3_Secrets_Offsets)/sizeof(WORD);

            //  Stats
            BYTE *pStatsStart   = GetBlockSecretsBaseAddress ( tombraider, block );
            BYTE *pAddress      = SearchSecretsCompleted ( tombraider, block );
            if ( pStatsStart && pAddress )
            {
                for ( int levelIndex = 0; levelIndex < sizeof(TR3Secrets) && levelIndex <= currentLevelIndex; levelIndex++ )
                {
                    if ( levelIndex < maxLevel && levelIndex != excludeLevel )
                    {
                        WORD offset             = TR3_Secrets_Offsets [ levelIndex ];
                        BYTE *pStats            = ( BYTE * ) ( pStatsStart + offset );
                        BYTE *pCompleted        = pAddress + offset;

                        if ( levelIndex == currentLevelIndex )
                        {
                            *pStats                 = 0;
                            *pCompleted             = 0;
                        }
                        else
                        {
                            *pStats                 = TR3Secrets [ levelIndex ];
                            *pCompleted             = TR3Secrets [ levelIndex ];
                        }
                    }
                }
            }
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetBlockSecretsAcquired(int tombraider, int block, BYTE secrets)
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( secrets == GetBlockSecretsAcquiredBits ( tombraider, block ) )
    {
        return;
    }

    //
    if ( secrets == 0xff )
    {
        secrets = GetSecretsMaximum ( tombraider, block );
    }

    //
    if ( secrets > 6 )
    {
        secrets = 6;
    }

    if ( secrets < 0 )
    {
        secrets = 0;
    }
    
    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            BYTE *pCount            = GetBlockSecretsCurrentAddress ( tombraider, block );
            BYTE *pCompleted        = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pAcquired && pCount && pCompleted )
            {
                *pAcquired              = CTRXTools::FillBitsB ( secrets );
                *pCount                 = secrets;
                *pCompleted             = secrets;
            }
            break;
        }

        case GAME_TRR2:
        {
            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            BYTE *pCount            = GetBlockSecretsCurrentAddress ( tombraider, block );
            BYTE *pCompleted        = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pAcquired && pCount && pCompleted )
            {
                *pAcquired              = CTRXTools::FillBitsB ( secrets );
                *pCount                 = secrets;
                *pCompleted             = secrets;
            }
            break;
        }

        //  For TR3 The Secrets are in Count
        case GAME_TRR3:
        {
            //  Guns            : E507A
            //  Real Health     : E2DE6
            //  000E28E4: 00 03 <-- Vector
            //  000E297C: 00 03
            //  000E4F66: 00 03
            //  000E514F: 00 03

            //  Jungle Guns: E3EEC          : E3FC1 - D5
            //  Ruins  Guns: E507A          : E514F - D5 after Guns

            //  pCompleted has a variable address
            //  000E28C0 000E28E4 000E3FC1  e3ed2 vs E3FC1 Jungle HEA:0E3ED2 <-- 0xEF vs health
            //  E3FC1 Jungle E514F for ruins
            //  e3eec vs E3FC1  Jungle :
            //  e41d4 vs E4318  Nevada
            //  0010C8C0 0010C953 0010DFC1 vs 0010E2CA vs 0010C8C0 HEA:10E186   <-- 0w144 vs health
            //  *pCompleted             = secrets;  //  CTRXTools::FillBitsB ( secrets );

            BYTE *pAcquired         = GetBlockSecretsAddress ( tombraider, block );
            BYTE *pCount            = GetBlockSecretsCurrentAddress ( tombraider, block );
            BYTE *pCompleted        = GetSecretsCompletedAddress ( tombraider, block );

            int iTotal = 0;
            for ( int iX = 0; iX < sizeof(TR3Secrets) && iX <= levelindex; iX++ )
            {
                iTotal += TR3Secrets [ iX ];
            }

            if ( pAcquired && pCount && pCompleted )
            {
                *pAcquired              = CTRXTools::FillBitsB ( secrets );
                *pCount                 = secrets;
                *pCompleted             = secrets;
            }

            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::ReNumber ( )
{
    //
    WORD saveNo [ NB_TR_BLOCKS ];
    WORD levelNo [ NB_TR_BLOCKS ];
    WORD blockNo [ NB_TR_BLOCKS ];

    //
    for ( int tombraider = 1; tombraider <= 3; tombraider++ )
    {
        ZeroMemory ( saveNo, sizeof(saveNo) );
        ZeroMemory ( levelNo, sizeof(levelNo) );
        ZeroMemory ( blockNo, sizeof(blockNo) );

        //  Get All
        for ( int block = 0; block < NB_TR_BLOCKS; block++ )
        {
            saveNo [ block ]    = GetSaveNumber ( tombraider, block );
            levelNo [ block ]   = GetBlockLevelNumber ( tombraider, block );
            blockNo [ block ]   = block;
        }

        //
        //  Sort by Level / SaveNo
        for ( int i = 0; i < NB_TR_BLOCKS; i++ )
        {
            for ( int j = i + 1; j < NB_TR_BLOCKS; j++ )
            {
                bool bSwap = false;
                if ( levelNo [ i ] > levelNo [ j ] )
                {
                    bSwap = true;
                }
                else if ( levelNo [ i ] == levelNo [ j ] && saveNo [ i ] > saveNo [ j ] )
                {
                    bSwap = true;
                }

                if ( bSwap )
                {
                    WORD temp       = levelNo [ i ];
                    levelNo [ i ]   = levelNo [ j ];
                    levelNo [ j ]   = temp;

                    temp            = saveNo [ i ];
                    saveNo [ i ]    = saveNo [ j ];
                    saveNo [ j ]    = temp;

                    temp            = blockNo [ i ];
                    blockNo [ i ]   = blockNo [ j ];
                    blockNo [ j ]   = temp;
                }
            }
        }
        
        //
        int count = 1;
        for ( int block = 0; block < NB_TR_BLOCKS; block++ )
        {
            WORD *pSaveAddress  = GetSaveAddress ( tombraider, blockNo [ block ] );
            int level           = GetBlockLevelNumber ( tombraider, blockNo [ block ] );
            if ( pSaveAddress != NULL && level != 0 )
            {
                saveNo [ block ]    = count;
                count++;
            }
        }

        //
        for ( int block = 0; block < NB_TR_BLOCKS; block++ )
        {
            WORD *pSaveAddress  = GetSaveAddress ( tombraider, blockNo [ block ] );
            int level           = GetBlockLevelNumber ( tombraider, blockNo [ block ] );
            if ( pSaveAddress != NULL && level != 0 )
            {
                *pSaveAddress   = saveNo [ block ];
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::Delete ( int tombraider, int block )
{
    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char    *pBuffer = m_pBuffer + TR1_BLOCK_START + GetBlockLength ( tombraider ) * block;
            memset ( pBuffer, 0, GetBlockLength ( tombraider ) );
            break;
        }
        case GAME_TRR2:
        {
            char    *pBuffer = m_pBuffer + TR2_BLOCK_START + GetBlockLength ( tombraider ) * block;
            memset ( pBuffer, 0, GetBlockLength ( tombraider ) );
            break;
        }
        case GAME_TRR3:
        {
            char    *pBuffer = m_pBuffer + TR3_BLOCK_START + GetBlockLength ( tombraider ) * block;
            memset ( pBuffer, 0, GetBlockLength ( tombraider ) );
            break;
        }
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::Export ( const char *pFilename, int tombraider, int block )
{
    BOOL    bResult = FALSE;

    //
    FILE *hFile = NULL;
    fopen_s ( & hFile, pFilename, "wb" );
    if ( hFile != NULL )
    {
        switch ( tombraider )
        {
            case GAME_TRR1:
            {
                char    *pBuffer = m_pBuffer + TR1_BLOCK_START + GetBlockLength ( tombraider ) * block;
                fwrite ( pBuffer, 1, GetBlockLength ( tombraider ), hFile );
                bResult = TRUE;
                break;
            }
            case GAME_TRR2:
            {
                char    *pBuffer = m_pBuffer + TR2_BLOCK_START + GetBlockLength ( tombraider ) * block;
                fwrite ( pBuffer, 1, GetBlockLength ( tombraider ), hFile );
                bResult = TRUE;
                break;
            }
            case GAME_TRR3:
            {
                char    *pBuffer = m_pBuffer + TR3_BLOCK_START + GetBlockLength ( tombraider ) * block;
                fwrite ( pBuffer, 1, GetBlockLength ( tombraider ), hFile );
                bResult = TRUE;
                break;
            }
        }
        CloseOneFile ( &hFile );

        return bResult;
    }
    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::Import( const char *pFilename, int tombraider, int block )
{
    BOOL    bResult = FALSE;

    //
    FILE *hFile = NULL;
    fopen_s ( & hFile, pFilename, "rb" );
    if ( hFile != NULL )
    {
        switch ( tombraider )
        {
            case GAME_TRR1:
            {
                char    *pSrcBuffer = (char *) malloc ( GetBlockLength ( tombraider ) + 1 );
                char    *pObjBuffer = m_pBuffer + TR1_BLOCK_START + GetBlockLength ( tombraider ) * block;
                size_t iRead = fread ( pSrcBuffer, 1, GetBlockLength ( tombraider ) + 1, hFile );
                if ( iRead == GetBlockLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetBlockLength ( tombraider ) );
                    bResult = TRUE;
                }
                free ( pSrcBuffer );
                break;
            }
            case GAME_TRR2:
            {
                char    *pSrcBuffer = (char *) malloc ( GetBlockLength ( tombraider ) + 1 );
                char    *pObjBuffer = m_pBuffer + TR2_BLOCK_START + GetBlockLength ( tombraider ) * block;
                size_t iRead = fread ( pSrcBuffer, 1, GetBlockLength ( tombraider ) + 1, hFile );
                if ( iRead == GetBlockLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetBlockLength ( tombraider ) );
                    bResult = TRUE;
                }
                free ( pSrcBuffer );
                break;
            }
            case GAME_TRR3:
            {
                char    *pSrcBuffer = (char *) malloc ( GetBlockLength ( tombraider ) + 1 );
                char    *pObjBuffer = m_pBuffer + TR3_BLOCK_START + GetBlockLength ( tombraider ) * block;
                int iRead = (int) fread ( pSrcBuffer, 1, GetBlockLength ( tombraider ) + 1, hFile );
                if ( iRead == GetBlockLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetBlockLength ( tombraider ) );
                    bResult = TRUE;
                }
                free ( pSrcBuffer );
                break;
            }
        }
        CloseOneFile ( &hFile );

        return bResult;
    }
    return bResult;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::TraceBlock ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock = (TABLE_TR1 *)GetBlockAddress(tombraider, block, slot);
            if ( pBlock )
            {
                printf ( "#%2d : 0x%08lx = MRK : %d - GUN : 0x%x - MAG : %d - UZI : %d - RIO : %d - ", slot,
                    RelativeAddress (  pBlock ), pBlock->guns, pBlock->gunmask, pBlock->magnum, pBlock->uzi, pBlock->riotgun / 6 );
                printf ( "HAR : %d - GRE : %d - M16 : %d - SMP : %u - LMP : %u - FLA : %u - flags : %u %u %u",
                            0, 0, 0, pBlock->smallMedipak, pBlock->largeMedipak, pBlock->scion,
                            pBlock->flag1, pBlock->flag2, pBlock->flag3  );
            }
            break;
        }
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock = (TABLE_TR2 *)GetBlockAddress(tombraider, block, slot);
            if ( pBlock )
            {
                printf ( "#%2d : 0x%08lx = MRK : %d - GUN : 0x%x - MAG : %d - UZI : %d - RIO : %d - ", slot,
                    RelativeAddress ( pBlock ), pBlock->guns, pBlock->gunmask, pBlock->magnum, pBlock->uzi, pBlock->riotgun / 6 );
                printf ( "HAR : %d - GRE : %d - M16 : %d - SMP : %u - LMP : %u - FLA : %u - flags : %u %u %u",
                    pBlock->harpoon, pBlock->grenade, pBlock->m16, pBlock->smallMedipak, pBlock->largeMedipak, pBlock->flare,
                    pBlock->flag1, pBlock->flag2, pBlock->flag3  );
            }
            break;
        }
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock = (TABLE_TR3 *)GetBlockAddress(tombraider, block, slot);
            if ( pBlock )
            {
                printf ( "#%2d : 0x%08lx = MRK : %d - GUN : 0x%x - MAG : %d - UZI : %d - RIO : %d - ", slot,
                    RelativeAddress (  pBlock ), pBlock->guns, pBlock->gunmask, pBlock->magnum, pBlock->uzi, pBlock->riotgun / 6 );
                printf ( "HAR : %d - GRE : %d - ROC : %d - M16 : %d - SMP : %u - LMP : %u - FLA : %u - flags : %u %u",
                    pBlock->harpoon, pBlock->grenade, pBlock->rocket, pBlock->m16, pBlock->smallMedipak, pBlock->largeMedipak,
                    pBlock->flare, pBlock->flag0, pBlock->flag1 );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::TraceGun ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            GUN_TR1 *pTag = (GUN_TR1*)GetGunAddress(tombraider, block, slot);
            if ( pTag )
            {
                printf ( "\tMAG : %d - UZI : %d - RIO : %d\n" , pTag->magnum, pTag->uzi, pTag->riotgun / 6 );
            }
            break;
        }
        case GAME_TRR2:
        {
            GUN_TR2 *pTag = (GUN_TR2 *)GetGunAddress(tombraider, block, slot);
            if ( pTag )
            {
                printf ( "\tMAG : %d - UZI : %d - RIO : %d - M16 : %d - HAR : %d - GRE : %d\n" ,
                    pTag->magnum, pTag->uzi, pTag->riotgun / 6,
                    pTag->m16, pTag->harpoon, pTag->grenade );
            }
            break;
        }
        case GAME_TRR3:
        {
            GUN_TR3 *pTag = (GUN_TR3 *)GetGunAddress(tombraider, block, slot);
            if ( pTag )
            {
                printf ( "\tMAG : %d - UZI : %d - RIO : %d  - M16 : %d - GRE : %d - ROC : %d - HAR : %d\n" ,
                    pTag->magnum, pTag->uzi, pTag->riotgun / 6,
                    pTag->m16, pTag->grenade, pTag->rocket, pTag->harpoon );
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR9SaveGame::GetAirAddress ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                WORD *pAddress  = (WORD *)( pGunAddress - TR1_OFFSET_AIR );
                return pAddress;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                WORD *pAddress  = (WORD *)( pGunAddress - TR2_OFFSET_AIR );
                return pAddress;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                WORD *pAddress  = (WORD *)( pGunAddress - TR3_OFFSET_AIR );
                return pAddress;
            }
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTR9SaveGame::GetAir ( int tombraider, int block )
{
    WORD *pAir = GetAirAddress ( tombraider, block );
    if ( pAir != NULL )
    {
        return *pAir;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetAir ( int tombraider, int block, WORD iAir )
{
    WORD *pAir = GetAirAddress ( tombraider, block );
    if ( pAir != NULL )
    {
        *pAir   = iAir;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR9SaveGame::GetState ( int tombraider, int block )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR1_OFFSET_STATE );
                return *pAddress;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR2_OFFSET_STATE );
                return *pAddress;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR3_OFFSET_STATE );
                return *pAddress;
            }
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetState ( int tombraider, int block, BYTE iAir )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    switch ( tombraider )
    {
        case GAME_TRR1:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR1_OFFSET_STATE );
                *pAddress = iAir;
            }
            break;
        }

        case GAME_TRR2:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR2_OFFSET_STATE );
                *pAddress = iAir;
            }
            break;
        }

        case GAME_TRR3:
        {
            char *pGunAddress   = (char *) SearchGunEntry ( tombraider, block );
            if ( pGunAddress )
            {
                BYTE *pAddress  = (BYTE *)( pGunAddress - TR3_OFFSET_STATE );
                *pAddress = iAir;
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR9SaveGame::GetInterest ( int tombraider, int block, int slot )
{
    static char szResponse [ 4096 ];
    ZeroMemory ( szResponse, sizeof(szResponse) );

    //
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return szResponse;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return szResponse;
    }

    {
        char *pAddress = NULL;
        pAddress = ( char *) GetSaveAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Save", RelativeAddress ( pAddress ), * (BYTE *) pAddress );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Save" );
        }

        pAddress = ( char *) GetBlockStart ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Start", RelativeAddress ( pAddress ) );
            pAddress = (char * ) pAddress + GetSlotLength ( tombraider ) * NB_OF_SLOTS;
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Last", RelativeAddress ( pAddress ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Start" );
        }

        pAddress    = ( char *) GetBlockAddress ( tombraider, block, slot );
        char *pSlot = pAddress;
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Block", RelativeAddress ( pAddress ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Block" );
        }

        pAddress = ( char *) GetBlockElapsedAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Elapsed", RelativeAddress ( pAddress ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Elapsed" );
        }

        pAddress = ( char *) GetBlockSecretsAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : 0x%02X (%d)\r\n", "Secrets", RelativeAddress ( pAddress ),
                        *((BYTE *)pAddress), CTRXTools::CountBitsB ( *((BYTE *)pAddress) ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Secrets" );
        }

        //
        pAddress = ( char *) GetBlockLevelAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Level", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Level" );
        }

        pAddress = ( char *) GetObjectBaseAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Objects", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Objects" );
        }

        pAddress = ( char *) GetBlockMapAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Map", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Map" );
        }

        pAddress = ( char *) GetBlockSecretsCurrentAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Stats", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Stats" );
        }

        pAddress = ( char *) GetBlockSecretsBaseAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Secrets", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Secrets" );
        }

        pAddress = ( char *) GetRealHealthAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Health", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Health", CTRXTools::RelativeAddress ( pAddress, pSlot ),
                        *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Health" );
        }

        pAddress = ( char *) GetAirAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Air", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Air" );
        }

        pAddress = ( char *) SearchGunEntry ( tombraider, block);
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Guns", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Guns" );
        }

        pAddress = ( char *) GetGunAddress ( tombraider, block, slot);
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Guns", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Guns" );
        }

        pAddress = ( char *) GetSecretsCompletedAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Completed", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Completed" );
        }

        pAddress = ( char *) GetSecretsCompletedBaseAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Completed Base", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Completed Base" );
        }

        pAddress = ( char *) GetBlockKillsAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Kill", RelativeAddress ( pAddress ),
                        *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Kill" );
        }

        pAddress = ( char *) GetBlockAmmosUsedAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Ammos Used", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Ammos Used" );
        }

        pAddress = ( char *) GetBlockAmmosHitsAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Ammos Hits", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Ammos Hits" );
        }

        pAddress = ( char *) GetBlockPickupAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Pickup", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Pickup" );
        }

        pAddress = ( char *) GetBlockMedipakUsedAddress ( tombraider, block );
        if ( pAddress )
        {
            BYTE mediUsed   = *((BYTE *) pAddress);
            BYTE decimal    = ( mediUsed % 2 ) * 5;
            mediUsed        = mediUsed / 2;
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d.%d\r\n", "Medipak", RelativeAddress ( pAddress ), mediUsed, decimal );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Medipak" );
        }

        pAddress = ( char *) GetBlockCrystalAddress ( tombraider, block );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %d\r\n", "Crystal", RelativeAddress ( pAddress ), *((BYTE *) pAddress) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Crystal" );
        }

        TR9_POSITION *pDwAddress = ( TR9_POSITION *) GetPositionAddress ( tombraider, block );
        if ( pDwAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %08lX %6ld ~ %.3f f\r\n", "West / East", RelativeAddress ( & pDwAddress->dwWestToEast ),
                        pDwAddress->dwWestToEast, pDwAddress->dwWestToEast, (double) (long) pDwAddress->dwWestToEast / POSITION_DIVIDER );
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %08lX %6ld ~ %.3f f\r\n", "Vertical", RelativeAddress ( & pDwAddress->dwVertical ),
                        pDwAddress->dwVertical, pDwAddress->dwVertical, (double) (long) pDwAddress->dwVertical / POSITION_DIVIDER );
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %08lX %6ld ~ %.3f f\r\n", "South / North", RelativeAddress ( & pDwAddress->dwSouthToNorth ),
                        pDwAddress->dwSouthToNorth, pDwAddress->dwSouthToNorth, (double) (long) pDwAddress->dwSouthToNorth / POSITION_DIVIDER );
            double dfOrientation = (double)pDwAddress->wOrientation;
            dfOrientation       *= 360.0;
            dfOrientation       /= 0x10000L;
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7.3f°\r\n", "Orientation", RelativeAddress ( & pDwAddress->wOrientation ),
                        pDwAddress->wOrientation, dfOrientation );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Room (Area)", RelativeAddress ( & pDwAddress->wRoom ),
                        pDwAddress->wRoom, pDwAddress->wRoom,
                        CTRXTools::ToBinary ( pDwAddress->wRoom ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wSomething0 ),
                        pDwAddress->wSomething0, pDwAddress->wSomething0,
                        CTRXTools::ToBinary ( pDwAddress->wSomething0 ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wSomething2 ),
                        pDwAddress->wSomething2, pDwAddress->wSomething2,
                        CTRXTools::ToBinary ( pDwAddress->wSomething2 ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wSomething3 ),
                        pDwAddress->wSomething3, pDwAddress->wSomething3,
                        CTRXTools::ToBinary ( pDwAddress->wSomething3 ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wSomething5 ),
                        pDwAddress->wSomething5, pDwAddress->wSomething5,
                        CTRXTools::ToBinary ( pDwAddress->wSomething5 ) );
        }

        sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "\r\n" );
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            //
            WORD *pRealHealth   = GetRealHealthAddress ( tombraider, block );
            if ( pRealHealth )
            {
                if ( GetBlockLevelNumber (tombraider, block ) == 15 )
                {
                    char *pAddress = (char * )( ( BYTE * ) pRealHealth ) + TR1_BOSS;
                    if ( pAddress )
                    {
                        sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                                    "TR1Boss\t\t: 0x%08lX : %d\r\n", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            void *pBlockAddress = GetBlockStart ( tombraider, block );
            WORD *pRealHealth   = GetRealHealthAddress ( tombraider, block );
            void *pStats        = GetBlockSecretsCurrentAddress ( tombraider, block );
            if ( pRealHealth )
            {
                if ( GetBlockLevelNumber (tombraider, block ) == 17 )
                {
                    char *pAddress = (char * )( ( BYTE * ) pRealHealth ) - TR2_DRAGON_BOSS1;
                    if ( pAddress )
                    {
                        sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                                    "TR2 - 1\t\t: 0x%08lX : %d\r\n", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
                    }
                    pAddress = (char * )( ( BYTE * ) pRealHealth ) - TR2_DRAGON_BOSS2;
                    if ( pAddress )
                    {
                        sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                                    "TR2 - 2\t\t: 0x%08lX : %d\r\n", RelativeAddress ( pAddress ), *((WORD *) pAddress) );
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            void *pBlockAddress = GetBlockStart ( tombraider, block );
            WORD *pRealHealth   = GetRealHealthAddress ( tombraider, block );
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, 0 );
            if ( pRealHealth )
            {
                if ( GetBlockLevelNumber (tombraider, block ) == 19 )
                {
                    BYTE *pAddress = (BYTE *)( pRealHealth ) + TR3_WILLARD_DEAD;
                    sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "Willard Dead\t: 0x%08lX : %d\r\n",
                        RelativeAddress (pAddress), *pAddress );
                }
            }

            break;
        }
    }

    return szResponse;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::BufferModified ()
{
    if ( m_pBuffer != NULL && m_pBufferBackup != NULL && m_iSaveLength > 0 )
    {
        if ( CTRXTools::MemoryCompare ( m_pBuffer, m_pBufferBackup, m_iSaveLength ) == 0 )
        {
            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::discard ()
{
    if ( m_pBuffer != NULL && m_pBufferBackup != NULL && m_iSaveLength > 0 )
    {
        memcpy ( m_pBuffer, m_pBufferBackup, m_iSaveLength );
    }

    return ;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetLikeGame ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    int level       = GetBlockLevelNumber( tombraider, block );
    int levelIndex = level - 1;
    if ( levelIndex < 0 )
    {
        return;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            int nbDefault = sizeof(TR1_Start_LikeGame)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            GUN_TR1 *pGun       = ( GUN_TR1 * ) m_TR1_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR1_Start_LikeGame [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR1_Start_LikeGame [ iDefault ].iLarge + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bMagnum,
                            TR1_Start_LikeGame [ iDefault ].iMagnum * levelIndex );
                SetUzi ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bUzi,
                            TR1_Start_LikeGame [ iDefault ].iUzi * levelIndex );
                SetRiotgun ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bRiotgun,
                            TR1_Start_LikeGame [ iDefault ].iRiotgun * levelIndex );
                SetM16 ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bM16,
                            TR1_Start_LikeGame [ iDefault ].iM16 * levelIndex );
                SetGrenade ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bGrenade,
                            TR1_Start_LikeGame [ iDefault ].iGrenade * levelIndex );
                SetHarpoon ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bHarpoon,
                            TR1_Start_LikeGame [ iDefault ].iHarpoon * levelIndex );
                SetRocket ( tombraider, block, slot, TR1_Start_LikeGame [ iDefault ].bRocket,
                            TR1_Start_LikeGame [ iDefault ].iRocket * levelIndex );


                //  Health / Air
                SetRealHealth ( tombraider, block, TR1_Start_LikeGame [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR1_Start_LikeGame [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );
                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            int nbDefault = sizeof(TR2_Start_LikeGame)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR2_Start_LikeGame [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR2_Start_LikeGame [ iDefault ].iLarge + levelIndex;
                pBlock->flare           = TR2_Start_LikeGame [ iDefault ].iFlare + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bMagnum,
                            TR2_Start_LikeGame [ iDefault ].iMagnum * levelIndex );
                SetUzi ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bUzi,
                            TR2_Start_LikeGame [ iDefault ].iUzi * levelIndex );
                SetRiotgun ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bRiotgun,
                            TR2_Start_LikeGame [ iDefault ].iRiotgun * levelIndex );
                SetM16 ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bM16,
                            TR2_Start_LikeGame [ iDefault ].iM16 * levelIndex );
                SetGrenade ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bGrenade,
                            TR2_Start_LikeGame [ iDefault ].iGrenade * levelIndex );
                SetHarpoon ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bHarpoon,
                            TR2_Start_LikeGame [ iDefault ].iHarpoon * levelIndex );
                SetRocket ( tombraider, block, slot, TR2_Start_LikeGame [ iDefault ].bRocket,
                            TR2_Start_LikeGame [ iDefault ].iRocket * levelIndex );

                //  Health / Air
                SetRealHealth ( tombraider, block, TR2_Start_LikeGame [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR2_Start_LikeGame [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );

                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            int nbDefault = sizeof(TR3_Start_LikeGame)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR3_Start_LikeGame [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR3_Start_LikeGame [ iDefault ].iLarge + levelIndex;
                pBlock->flare           = TR3_Start_LikeGame [ iDefault ].iFlare + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bMagnum,
                            TR3_Start_LikeGame [ iDefault ].iMagnum * levelIndex );
                SetUzi ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bUzi,
                            TR3_Start_LikeGame [ iDefault ].iUzi * levelIndex );
                SetRiotgun ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bRiotgun,
                            TR3_Start_LikeGame [ iDefault ].iRiotgun * levelIndex );
                SetM16 ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bM16,
                            TR3_Start_LikeGame [ iDefault ].iM16 * levelIndex );
                SetGrenade ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bGrenade,
                            TR3_Start_LikeGame [ iDefault ].iGrenade * levelIndex );
                SetHarpoon ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bHarpoon,
                            TR3_Start_LikeGame [ iDefault ].iHarpoon * levelIndex );
                SetRocket ( tombraider, block, slot, TR3_Start_LikeGame [ iDefault ].bRocket,
                            TR3_Start_LikeGame [ iDefault ].iRocket * levelIndex );

                //  Health / Air
                SetRealHealth ( tombraider, block, TR3_Start_LikeGame [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR3_Start_LikeGame [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );
                //
                pBlock->colCrystals     = 0;

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );

                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetLikePlus ( int tombraider, int block, int slot )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    int level       = GetBlockLevelNumber( tombraider, block );
    int levelIndex = level - 1;
    if ( levelIndex < 0 )
    {
        return;
    }

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            int nbDefault = sizeof(TR1_Start_LikePlus)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            GUN_TR1 *pGun       = ( GUN_TR1 * ) m_TR1_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR1_Start_LikePlus [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR1_Start_LikePlus [ iDefault ].iLarge + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bMagnum,
                            TR1_Start_LikePlus [ iDefault ].iMagnum * level );
                SetUzi ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bUzi,
                            TR1_Start_LikePlus [ iDefault ].iUzi * level );
                SetRiotgun ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bRiotgun,
                            TR1_Start_LikePlus [ iDefault ].iRiotgun * level );
                SetM16 ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bM16,
                            TR1_Start_LikePlus [ iDefault ].iM16 * level );
                SetGrenade ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bGrenade,
                            TR1_Start_LikePlus [ iDefault ].iGrenade * level );
                SetHarpoon ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bHarpoon,
                            TR1_Start_LikePlus [ iDefault ].iHarpoon * level );
                SetRocket ( tombraider, block, slot, TR1_Start_LikePlus [ iDefault ].bRocket,
                            TR1_Start_LikePlus [ iDefault ].iRocket * level );


                //  Health / Air
                SetRealHealth ( tombraider, block, TR1_Start_LikePlus [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR1_Start_LikePlus [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );
                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }

        //
        case GAME_TRR2:
        {
            int nbDefault = sizeof(TR2_Start_LikePlus)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR2_Start_LikePlus [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR2_Start_LikePlus [ iDefault ].iLarge + levelIndex;
                pBlock->flare           = TR2_Start_LikePlus [ iDefault ].iFlare + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bMagnum,
                            TR2_Start_LikePlus [ iDefault ].iMagnum * level );
                SetUzi ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bUzi,
                            TR2_Start_LikePlus [ iDefault ].iUzi * level );
                SetRiotgun ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bRiotgun,
                            TR2_Start_LikePlus [ iDefault ].iRiotgun * level );
                SetM16 ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bM16,
                            TR2_Start_LikePlus [ iDefault ].iM16 * level );
                SetGrenade ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bGrenade,
                            TR2_Start_LikePlus [ iDefault ].iGrenade * level );
                SetHarpoon ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bHarpoon,
                            TR2_Start_LikePlus [ iDefault ].iHarpoon * level );
                SetRocket ( tombraider, block, slot, TR2_Start_LikePlus [ iDefault ].bRocket,
                            TR2_Start_LikePlus [ iDefault ].iRocket * level );

                //  Health / Air
                SetRealHealth ( tombraider, block, TR2_Start_LikePlus [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR2_Start_LikePlus [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );

                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }

        //
        case GAME_TRR3:
        {
            int nbDefault = sizeof(TR3_Start_LikePlus)/sizeof(TR_START_CONDITIONS);
            int iDefault    = levelIndex % nbDefault;
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != 0 )
            {
                pBlock->smallMedipak    = TR3_Start_LikePlus [ iDefault ].iSmall + levelIndex;
                pBlock->largeMedipak    = TR3_Start_LikePlus [ iDefault ].iLarge + levelIndex;
                pBlock->flare           = TR3_Start_LikePlus [ iDefault ].iFlare + levelIndex;

                //  Set Guns and Ammos
                SetPistols ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bPistols );
                SetMagnum ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bMagnum,
                            TR3_Start_LikePlus [ iDefault ].iMagnum * level );
                SetUzi ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bUzi,
                            TR3_Start_LikePlus [ iDefault ].iUzi * level );
                SetRiotgun ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bRiotgun,
                            TR3_Start_LikePlus [ iDefault ].iRiotgun * level );
                SetM16 ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bM16,
                            TR3_Start_LikePlus [ iDefault ].iM16 * level );
                SetGrenade ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bGrenade,
                            TR3_Start_LikePlus [ iDefault ].iGrenade * level );
                SetHarpoon ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bHarpoon,
                            TR3_Start_LikePlus [ iDefault ].iHarpoon * level );
                SetRocket ( tombraider, block, slot, TR3_Start_LikePlus [ iDefault ].bRocket,
                            TR3_Start_LikePlus [ iDefault ].iRocket * level );

                //  Health / Air
                SetRealHealth ( tombraider, block, TR3_Start_LikePlus [ iDefault ].iHealth );
                SetAir ( tombraider, block, TR3_Start_LikePlus [ iDefault ].iAir );

                //  Crystals
                SetBlockCrystal ( tombraider, block, 0 );
                //
                pBlock->colCrystals     = 0;

                //  Set Secrets
                SetBlockSecretsAcquiredAll (  tombraider, block, levelIndex );
                SetBlockSecretsAcquired ( tombraider, block, 0 );

                //  Set Objects
                for ( int i = 0; i < 2; i++ )
                {
                    SetMap ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetBlockObject ( tombraider, block, i, 0 );
                }
                for ( int i = 0; i < 4; i++ )
                {
                    SetKey ( tombraider, block, i, 0 );
                }

                //
                DWORD *pElapsed     = GetBlockElapsedAddress ( tombraider, block );
                if ( pElapsed != NULL )
                {
                    *pElapsed   = 0;
                }
                DWORD *pDistance    = GetBlockDistanceAddress ( tombraider, block );
                if ( pDistance != NULL )
                {
                    *pDistance  = 0;
                }

                WORD *pKills    = GetBlockKillsAddress ( tombraider, block );
                if ( pKills != NULL )
                {
                    *pKills = 0;
                }
                BYTE *pPickup   = GetBlockPickupAddress ( tombraider, block );
                if ( pPickup != NULL )
                {
                    *pPickup    = 0;
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetPistols ( int tombraider, void *pBlocks, int level, BOOL bEnabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlocks;
            if ( pBlock != NULL )
            {
                BOOL bAuthorized = CTRXTools::IsEnablePistol ( tombraider, level );
                if ( bAuthorized && bEnabled )
                {
                    pBlock->gunmask     |= TRR1_MASK_PISTOL;    // Pistol
                }
                else
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR1_MASK_PISTOL ^ TRR1_MASK_ANY );
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            if ( pBlock != NULL )
            {
                BOOL bAuthorized = CTRXTools::IsEnablePistol ( tombraider, level );
                if ( bAuthorized && bEnabled )
                {
                    pBlock->gunmask     |= TRR2_MASK_PISTOL;    // Pistol
                }
                else
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_PISTOL ^ TRR2_MASK_ANY );
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            if ( pBlock != NULL )
            {
                BOOL bAuthorized = CTRXTools::IsEnablePistol ( tombraider, level );
                if ( bAuthorized && bEnabled )
                {
                    pBlock->gunmask     |= TRR3_MASK_PISTOL;    // Pistol
                }
                else
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_PISTOL ^ TRR3_MASK_ANY );
                }
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetPistols ( int tombraider, int block, int slot, BOOL bEnabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            if ( pBlock != NULL )
            {
                SetPistols ( tombraider, pBlock, level, bEnabled );
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            if ( pBlock != NULL )
            {
                SetPistols ( tombraider, pBlock, level, bEnabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            if ( pBlock != NULL )
            {
                SetPistols ( tombraider, pBlock, level, bEnabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetMagnum ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlocks;
            GUN_TR1 *pGun       = ( GUN_TR1 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableMagnum ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR1_MASK_MAGNUM; // Magnum
                    pBlock->magnum      = ammos;
                }

                if ( pGun )
                {
                    pGun->magnum        = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR1_MASK_MAGNUM ^ TRR1_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->magnum        = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = 0;
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableMagnum ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_DESERT; // Magnum
                    pBlock->magnum      = ammos;
                }
                if ( pGun )
                {
                    pGun->magnum        = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_DESERT ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->magnum        = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableMagnum ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_DESERT; // Magnum
                    pBlock->magnum      = ammos;
                }
                if ( pGun )
                {
                    pGun->magnum        = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_DESERT ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->magnum        = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->magnum      = ammos;
                    }
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetMagnum ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            GUN_TR1 *pGun       = ( GUN_TR1 * ) m_TR1_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetMagnum ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetMagnum ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetMagnum ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetUzi ( int tombraider, void *pBlocks, void *pGuns, int level,
                            BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlocks;
            GUN_TR1 *pGun       = ( GUN_TR1 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableUzi ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR1_MASK_UZI;    // Uzi
                    pBlock->uzi         = ammos;
                }

                if ( pGun )
                {
                    pGun->uzi           = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR1_MASK_UZI ^ TRR1_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->uzi       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableUzi ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_UZI;    // Uzi
                    pBlock->uzi         = ammos;
                }
                if ( pGun )
                {
                    pGun->uzi           = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_UZI ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->uzi       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableUzi ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_UZI;    // Uzi
                    pBlock->uzi         = ammos;
                }
                if ( pGun )
                {
                    pGun->uzi           = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_UZI ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->uzi       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->uzi     = ammos;
                    }
                }
            }
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetUzi ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );
    int levelIndex  = level - 1;

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            GUN_TR1 *pGun       = ( GUN_TR1 * ) m_TR1_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetUzi ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetUzi ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetUzi ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetRiotgun ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) pBlocks;
            GUN_TR1 *pGun       = ( GUN_TR1 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRiotgun ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR1_MASK_SHOTGUN;   // riotgun
                    pBlock->riotgun     = ammos;
                }
                if ( pGun )
                {
                    pGun->riotgun       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR1_MASK_SHOTGUN ^ TRR1_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->riotgun       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRiotgun ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_SHOTGUN;   // riotgun
                    pBlock->riotgun     = ammos;
                }
                if ( pGun )
                {
                    pGun->riotgun       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_SHOTGUN ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->riotgun       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRiotgun ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_SHOTGUN;   // riotgun
                    pBlock->riotgun     = ammos;
                }
                if ( pGun )
                {
                    pGun->riotgun       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_SHOTGUN ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->riotgun       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->riotgun     = ammos;
                    }
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetRiotgun ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {
            TABLE_TR1 *pBlock   = ( TABLE_TR1 *) m_TR1_Blocks [ block ][ slot ];
            GUN_TR1 *pGun       = ( GUN_TR1 * ) m_TR1_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRiotgun ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRiotgun ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRiotgun ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetHarpoon ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableHarpoon( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_HARPOON;    // harpoon
                    pBlock->harpoon     = ammos;
                }
                if ( pGun )
                {
                    pGun->harpoon       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_HARPOON ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->harpoon       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->harpoon     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->harpoon     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableHarpoon ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_HARPOON;    // harpoon
                    pBlock->harpoon     = ammos;
                }
                if ( pGun )
                {
                    pGun->harpoon       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_HARPOON ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->harpoon       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->harpoon     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->harpoon     = ammos;
                    }
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetHarpoon ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetHarpoon ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetHarpoon ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetM16 ( int tombraider, void *pBlocks, void *pGuns, int level,
                            BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableM16( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_M16;    // m16
                    pBlock->m16         = ammos;
                }
                if ( pGun )
                {
                    pGun->m16           = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_M16 ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->m16       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m16     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m16     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableM16 ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_MP5;    // m16
                    pBlock->m16         = ammos;
                }
                if ( pGun )
                {
                    pGun->m16           = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_MP5 ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->m16       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m16     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m16     = ammos;
                    }
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetM16 ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetM16 ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetM16 ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetGrenade ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) pBlocks;
            GUN_TR2 *pGun       = ( GUN_TR2 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableGrenade( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR2_MASK_GRENADE;    // grenade
                    pBlock->grenade     = ammos;
                }
                if ( pGun )
                {
                    pGun->grenade       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR2_MASK_GRENADE ^ TRR2_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->grenade       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->grenade     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->grenade     = ammos;
                    }
                }
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableGrenade ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock )
                {
                    pBlock->gunmask     |= TRR3_MASK_GRENADE;    // grenade
                    pBlock->grenade     = ammos;
                }
                if ( pGun )
                {
                    pGun->grenade       = ammos;
                }
            }
            else
            {
                if ( pBlock )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_GRENADE ^ TRR3_MASK_ANY );
                }

                if ( pGun )
                {
                    pGun->grenade       = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->grenade     = 0;
                    }
                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->grenade     = ammos;
                    }
                }
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetGrenade ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            TABLE_TR2 *pBlock   = ( TABLE_TR2 *) m_TR2_Blocks [ block ][ slot ];
            GUN_TR2 *pGun       = ( GUN_TR2 * ) m_TR2_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetGrenade ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetGrenade ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetRocket ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) pBlocks;
            GUN_TR3 *pGun       = ( GUN_TR3 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRocket ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pBlock != NULL )
                {
                    pBlock->gunmask     |= TRR3_MASK_ROCKET; // rocket
                    pBlock->rocket      = ammos;
                }

                if ( pGun != NULL )
                {
                    pGun->rocket        = ammos;
                }
            }
            else
            {
                if ( pBlock != NULL )
                {
                    pBlock->gunmask     = pBlock->gunmask & ( TRR3_MASK_ROCKET ^ TRR3_MASK_ANY );
                }

                if ( pGun != NULL )
                {
                    pGun->rocket        = 0;
                }

                if ( bZeroWhenDisabled )
                {
                    if ( pBlock != NULL )
                    {
                        pBlock->rocket      = 0;
                    }
                }
                else
                {
                    if ( pBlock != NULL )
                    {
                        pBlock->rocket      = ammos;
                    }
                }
            }
            break;
        }
    }
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR9SaveGame::SetRocket ( int tombraider, int block, int slot, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block >= NB_TR_BLOCKS )
    {
        tombraider  = 2;
        block       -= NB_TR_BLOCKS;
    }

    if ( block < 0 || block >= NB_TR_BLOCKS )
    {
        return;
    }

    if ( slot < 0 || slot >= NB_OF_SLOTS )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case GAME_TRR1:
        {

            break;
        }
        //
        case GAME_TRR2:
        {
            break;
        }
        //
        case GAME_TRR3:
        {
            TABLE_TR3 *pBlock   = ( TABLE_TR3 *) m_TR3_Blocks [ block ][ slot ];
            GUN_TR3 *pGun       = ( GUN_TR3 * ) m_TR3_Guns [ block ][ slot ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRocket ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  { FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE }
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::ReadIndicators(TRR_BYTES_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    if ( ! PathFileExists(pFilename) )
    {
        return 0;
    }

    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "r" );
    if ( hFile == NULL )
    {
        return 0;
    }

    //
    int line    = 0;
    static char szLine [ MAX_PATH ];

    //
    int count = 0;
    do
    {
        ZeroMemory ( szLine, sizeof(szLine) );
        char *pLine = fgets ( szLine, sizeof(szLine), hFile );
        if ( pLine != NULL )
        {
            //
            TRR_BYTES_INDICATORS indicators;
            ZeroMemory ( &indicators, sizeof(indicators) );

            //
            //  Get Label
            char *pLabel = strchr ( szLine, '"' );
            if ( pLabel )
            {
                //  Skip quote
                pLabel++;
                for ( int i = 0; i < sizeof(indicators.szLabel) - 1; i++ )
                {
                    if ( *pLabel == '"' )
                    {
                        break;
                    }
                    indicators.szLabel [ i ] = *pLabel;
                    pLabel++;
                }
            }

            //
            char    strDelimit[]    = " \t,{}";
            char    *strToken       = NULL;
            char    *context        = NULL;
            int     value           = 0;

            char *pAccolade         = strchr ( pLine, '{' );
            if ( pAccolade != NULL )
            {
                pLine   = pAccolade + 1;
            }

            //
            //      Treat Tokens
            int index = 0;
            strToken = strtok_s ( pLine, strDelimit, &context);
            while ( strToken != NULL && index <= 4 )
            {
                //
                bool bSkip = true;
                while ( bSkip )
                {
                    if ( *strToken == ' ' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '\t' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == ';' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '{' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '}' )
                    {
                        strToken++;
                    }
                    else
                    {
                        bSkip   = false;
                    }
                }

                //
                if ( _strnicmp ( strToken, "0x", 2 ) == 0 )
                {
                    sscanf_s ( strToken + 2, "%x", &value );
                }
                else if ( _strnicmp ( strToken, "true", strlen("true") ) == 0 )
                {
                    value   = TRUE;
                }
                else if ( _strnicmp ( strToken, "false", strlen("false") ) == 0 )
                {
                    value   = FALSE;
                }
                else
                {
                    value   = atoi(strToken);
                }

                //
                switch ( index )
                {
                    case 0 : indicators.bEnd    = (BOOL) value; break;
                    case 1 : indicators.b1      = (BYTE) value; break;
                    case 2 : indicators.b2      = (BYTE) value; break;
                    case 3 : indicators.b3      = (BYTE) value; break;
                    case 4 : indicators.step    = value; break;
                }

                //      Get next token:
                strToken = strtok_s( NULL, strDelimit, &context);
                index++;
            }

            //
            //  Add Entry
            if ( line < maxTable )
            {
                IndicatorsTRTable [ line ] = indicators;

                //
                line++;

                count++;
            }
        }
    }
    while ( ! feof ( hFile ) && ! ferror ( hFile ) );

    //
    //  Add End of entries
    //
    if ( line < maxTable )
    {
        TRR_BYTES_INDICATORS indicators;
        ZeroMemory ( &indicators, sizeof(indicators) );
        indicators.bEnd     = TRUE;
        indicators.b1       = 0xff;
        indicators.b2       = 0xff;
        indicators.b3       = 0xff;
        indicators.step     = 0;
        strcpy_s ( indicators.szLabel, sizeof(indicators.szLabel), "End" );
        IndicatorsTRTable [ line ] = indicators;
    }

    //
    CloseOneFile ( &hFile );

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR9SaveGame::WriteIndicators(TRR_BYTES_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    //
    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "w" );
    if ( hFile == NULL )
    {
        return FALSE;
    }

    //
    int index = 0;
    do
    {
        fprintf_s ( hFile, "{ " );
        TRR_BYTES_INDICATORS indicator = IndicatorsTRTable [ index ];
        if ( indicator.bEnd )
        {
            fprintf_s ( hFile, "TRUE, " );
        }
        else
        {
            fprintf_s ( hFile, "FALSE, " );
        }

        fprintf_s ( hFile, "0x%02x, ", indicator.b1 & 0xff );
        fprintf_s ( hFile, "0x%02x, ", indicator.b2 & 0xff );
        fprintf_s ( hFile, "0x%02x, ", indicator.b3 & 0xff );

        fprintf_s ( hFile, "%d, ", indicator.step );

        fprintf_s ( hFile, "\"%s\", ", indicator.szLabel );

        fprintf_s ( hFile, "}\n" );

        if ( indicator.bEnd )
        {
            break;
        }

        index++;
    }
    while ( index < maxTable );

    CloseOneFile ( &hFile );

    return TRUE;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::CountIndicators( TRR_BYTES_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count = 0;
    do
    {
        TRR_BYTES_INDICATORS indicator = IndicatorsTRTable [ count ];
        if ( indicator.bEnd )
        {
            return count;
        }
        count++;
    }
    while ( count < maxTable );

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::MinIndicators( TRR_BYTES_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int minimum     = -1;
    do
    {
        TRR_BYTES_INDICATORS indicator = IndicatorsTRTable [ count ];
        if ( indicator.bEnd )
        {
            return minimum;
        }

        if ( minimum == -1  || indicator.step < minimum )
        {
            minimum = indicator.step;
        }
        count++;
    }
    while ( count < maxTable );

    return minimum;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR9SaveGame::MaxIndicators( TRR_BYTES_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int maximum     = -1;
    do
    {
        TRR_BYTES_INDICATORS indicator = IndicatorsTRTable [ count ];
        if ( indicator.bEnd )
        {
            return maximum;
        }

        if ( maximum == -1  || indicator.step > maximum )
        {
            maximum = indicator.step;
        }
        count++;
    }
    while ( count < maxTable );

    return maximum;
}

