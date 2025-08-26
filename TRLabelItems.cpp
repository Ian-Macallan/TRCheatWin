#include "stdafx.h"

#include "TRLabelItems.h"

//
/////////////////////////////////////////////////////////////////////////////
//  TR1
/////////////////////////////////////////////////////////////////////////////
FourLabels TR1_Key_Label [TR1_LEVELS] =
{
    { "",   "", "", "" },
    { "Sllver Key", "", "", "" },
    { "Machine Cog",    "", "", "" },
    { "Machine Cog",    "", "", "" },
    { "Neptune Key",    "Atlas Key",    "Damocles Key", "Thor Key" },

    { "Rusty Key",  "", "", "" },
    { "",   "", "", "" },
    { "Gold Key",   "Silver Key",   "Rusty Key",    "" },
    { "Gold Key",   "", "Rusty Key",    "" },
    { "Saphyre Key",    "", "", "" },

    { "Saphyre Key",    "", "", "" },
    { "Gold Key",   "", "", "" },
    { "Key",    "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },

    // UB
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "Gold Key",   "", "", "" },
    { "Ornate Key", "", "", "" },
};

//
FourLabels TR1_Object_Label [TR1_LEVELS] =
{
    { "",   "", "", "" },
    { "Ghost Idol", "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },

    { "",   "", "", "" },
    { "Gold Bar",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },

    { "Eye Of Horus",   "Scarab",   "Seal Of Anubis",   "Ankh" },
    { "Ankh",   "Scarab",   "", "" },
    { "Fuse",   "Pyramid Key",  "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },

    //  UB
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
};

TwoLabels TR1_Map_Label [TR1_LEVELS] =
{
    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },

    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },

    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },

    // UB
    { "",   "" },
    { "",   "" },
    { "",   "" },
    { "",   "" },

};

//
/////////////////////////////////////////////////////////////////////////////
//  TR2 Extracted
/////////////////////////////////////////////////////////////////////////////
FourLabels TR2_Key_Label [TR2_LEVELS] =
{
    {     "Guardhouse Key",     "Rusty Key",     "",     "",     }, /* 1 */

    {     "Boathouse Key",     "Steel Key",     "Iron Key",     "",     }, /* 2 */
    {     "Library Key",     "Detonator Key",     "",     "",     }, /* 3 */
    {     "Ornate Key",     "",     "",     "",     }, /* 4 */

    {     "Red Pass Card",     "Yellow Pass Card",     "Green Pass Card",     "",     }, /* 5 */
    {     "Red Pass Card",     "",     "",     "Blue Pass Card",     }, /* 6 */
    {     "",     "",     "",     "",     }, /* 7 */
    {     "Rest Room Key",     "Rusty Key",     "Cabin Key",     "",     }, /* 8 */
    {     "Theatre Key",     "Rusty Key",     "",     "",     }, /* 9 */
    {     "",     "Stern Key",     "Storage Key",     "Cabin Key",     }, /* 10 */

    {     "Drawbridge Key",     "Hut Key",     "",     "",     }, /* 11 */
    {     "Strongroom Key",     "Trapdoor Key",     "Rooftops Key",     "Main Hall Key",     }, /* 12 */
    {     "",     "",     "",     "",     }, /* 13 */
    {     "",     "Gong Hammer",     "",     "",     }, /* 14 */

    {     "",     "Gold Key",     "Silver Key",     "Main Chamber Key",     }, /* 15 */

    {     "",     "",     "",     "",     }, /* 16 */
    {     "",     "",     "",     "",     }, /* 17 */
    {     "Gun Cupboard Key",     "",     "",     "",     }, /* 18 */

    // The Cold War
    {     "Guardroom Key",     "Shaft 'B' Key",     "",     "",     }, /* 1 */
    {     "CardKey 1",     "",     "",     "CardKey 2",     }, /* 2 */
    {     "",     "",     "",     "",     }, /* 3 */
    {     "",     "",     "",     "",     }, /* 4 */
    {     "Hotel Key",     "",     "",     "",     }, /* 5 */

};

//  TR2 Extracted
FourLabels TR2_Object_Label [TR2_LEVELS] =
{
    {     "",     "",     "",     "",     }, /* 1 */

    {     "",     "",     "",     "",     }, /* 2 */
    {     "",     "",     "",     "",     }, /* 3 */
    {     "Relay Box",     "Circuit Board",     "",     "",     }, /* 4 */

    {     "",     "",     "",     "",     }, /* 5 */
    {     "Machine Chip",     "",     "",     "",     }, /* 6 */
    {     "",     "",     "",     "",     }, /* 7 */
    {     "Circuit Breaker",     "",     "",     "",     }, /* 8 */
    {     "",     "",     "",     "",     }, /* 9 */
    {     "",     "",     "",     "The Seraph",     }, /* 10 */

    {     "",     "",     "",     "The Seraph",     }, /* 11 */
    {     "Prayer Wheels",     "Gemstones",     "",     "The Seraph",     }, /* 12 */
    {     "Tibetan Mask",     "",     "",     "",     }, /* 13 */
    {     "Tibetan Mask",     "",     "",     "",     }, /* 14 */

    {     "The Dragon Seal",     "",     "",     "",     }, /* 15 */
    {     "Mystic Plaque",     "Mystic Plaque",     "",     "",     }, /* 16 */
    {     "Mystic Plaque",     "Dagger of Xian",     "",     "",     }, /* 17 */

    {     "Dagger of Xian",     "",     "",     "",     }, /* 18 */

    //  The Cold War
    {     "",     "",     "",     "",     }, /* 1 */
    {     "Circuit Board",     "",     "",     "",     }, /* 2 */
    {     "Mask Of Tornarsuk",     "Gold Nugget",     "",     "",     }, /* 3 */
    {     "Mask Of Tornarsuk",     "",     "",     "",     }, /* 4 */
    {     "Elevator Junction",     "Door Circuit",     "",     "",     }, /* 5 */

};

//  TR2 Extracted
TwoLabels TR2_Map_Label [TR2_LEVELS] =
{
    {     "",     "",     }, /* 1 */

    {     "",     "",     }, /* 2 */
    {     "",     "",     }, /* 3 */
    {     "",     "",     }, /* 4 */

    {     "",     "",     }, /* 5 */
    {     "",     "",     }, /* 6 */
    {     "",     "",     }, /* 7 */
    {     "",     "",     }, /* 8 */
    {     "",     "",     }, /* 9 */
    {     "",     "",     }, /* 10 */

    {     "",     "",     }, /* 11 */
    {     "",     "",     }, /* 12 */
    {     "Gong Hammer",     "",     }, /* 13 */
    {     "",     "Talion",     }, /* 14 */

    {     "",     "",     }, /* 15 */
    {     "",     "",     }, /* 16 */
    {     "",     "",     }, /* 17 */

    {     "",     "",     }, /* 18 */

    //  The Cold War
    {     "",     "",     }, /* 1 */
    {     "",     "",     }, /* 2 */
    {     "",     "",     }, /* 3 */
    {     "",     "",     }, /* 4 */
    {     "",     "",     }, /* 5 */
};

//
/////////////////////////////////////////////////////////////////////////////
//  TR3 Extracted
/////////////////////////////////////////////////////////////////////////////
FourLabels TR3_Key_Label [TR3_LEVELS] =
{
    //  India
    {     "",     "",     "",     "Indra Key",     }, /* 1 */
    {     "Key Of Ganesha",     "Key Of Ganesha",     "Key Of Ganesha",     "Key Of Ganesha",     }, /* 2 */
    {     "Gate Key",     "",     "",     "",     }, /* 3 */
    {     "",     "",     "",     "",     }, /* 4 */

    // Pacific
    {     "Smuggler's Key",     "",     "",     "",     }, /* 5 */
    {     "Commander Bishop's Key",     "Lt. Tuckerman's Key",     "",     "",     }, /* 6 */
    {     "",     "",     "",     "",     }, /* 7 */
    {     "",     "",     "",     "",     }, /* 8 */

    //  London
    {     "Flue Room Key",     "Cathedral Key",     "",     "",     }, /* 9 */
    {     "Maintenance Key",     "Solomon's Key",     "Solomon's Key",     "Solomon's Key",     }, /* 10 */
    {     "Boiler Room Key",     "",     "",     "",     }, /* 11 */
    {     "",     "",     "",     "",     }, /* 12 */

    //  Nevada
    {     "Generator Access",     "Detonator Switch",     "",     "",     }, /* 13 */
    {     "Keycard Type A",     "Keycard Type B",     "",     "",     }, /* 14 */
    {     "Launch Code Pass",     "",     "",     "",     }, /* 15 */

    //  Antartica
    {     "Hut Key",     "",     "",     "",     }, /* 16 */
    {     "",     "",     "",     "",     }, /* 17 */
    {     "Uli Key",     "",     "",     "",     }, /* 18 */
    {     "",     "",     "",     "",     }, /* 19 */

    //  All Hallows
    {     "Vault Key",     "",     "",     "",     }, /* 20 */

    //  Gold
    {     "",     "",     "",     "",     }, /* 1 */
    {     "Cairn Key",     "",     "",     "",     }, /* 2 */
    {     "Drill Activator Card",     "",     "",     "",     }, /* 3 */
    {     "",     "",     "",     "",     }, /* 4 */
    {     "Zoo Key",     "",     "",     "Aviary Key",     }, /* 5 */
    {     "",     "",     "",     "",     }, /* 6 */
};

//  TR3 Extracted
FourLabels TR3_Object_Label [TR3_LEVELS] =
{
    {     "",     "",     "",     "",     }, /* 1 */
    {     "Scimitar",     "Scimitar",     "",     "",     }, /* 2 */
    {     "",     "",     "",     "",     }, /* 3 */
    {     "",     "",     "",     "",     }, /* 4 */

    {     "Serpent Stone",     "",     "",     "",     }, /* 5 */
    {     "",     "",     "",     "",     }, /* 6 */
    {     "",     "",     "",     "",     }, /* 7 */
    {     "",     "",     "",     "",     }, /* 8 */

    {     "",     "",     "",     "",     }, /* 9 */
    {     "Old Penny",     "Ticket",     "Masonic Mallet",     "Ornate Star",     }, /* 10 */
    {     "Embalming Fluid",     "",     "",     "",     }, /* 11 */
    {     "",     "",     "",     "",     }, /* 12 */

    {     "",     "",     "",     "",     }, /* 13 */
    {     "Blue Security Pass",     "Yellow Security Pass",     "",     "",     }, /* 14 */
    {     "Tower Access Key",     "Code Clearance Disk",     "Code Clearance Disk",     "Hanger Access Key",     }, /* 15 */

    {     "Crowbar",     "Gate Control Key",     "",     "",     }, /* 16 */
    {     "Crowbar",     "Lead Acid Battery",     "Winch Starter",     "",     }, /* 17 */
    {     "Oceanic Mask",     "",     "",     "",     }, /* 18 */
    {     "",     "",     "",     "",     }, /* 19 */

    {     "",     "",     "",     "",     }, /* 20 */


    //  Gold
    {     "Crowbar",     "Thistle Stone",     "",     "",     }, /* 1 */
    {     "Crowbar",     "",     "",     "",     }, /* 2 */
    {     "Pump Access Disk",     "",     "",     "",     }, /* 3 */
    {     "Circuit Bulb",     "Mutant Sample",     "Mutant Sample",     "Circuit Bulb",     }, /* 4 */
    {     "The Hand Of Rathmore",     "",     "",     "",     }, /* 5 */
    {     "The Hand Of Rathmore",     "",     "",     "",     }, /* 6 */
};

//  TR3 Extracted
TwoLabels TR3_Map_Label [TR3_LEVELS] =
{
    {     "",     "",     }, /* 1 */
    {     "",     "",     }, /* 2 */
    {     "",     "",     }, /* 3 */
    {     "",     "",     }, /* 4 */

    {     "",     "",     }, /* 5 */
    {     "Swamp Map",     "",     }, /* 6 */
    {     "",     "",     }, /* 7 */
    {     "",     "",     }, /* 8 */

    {     "",     "",     }, /* 9 */
    {     "",     "",     }, /* 10 */
    {     "",     "",     }, /* 11 */
    {     "",     "",     }, /* 12 */

    {     "",     "",     }, /* 13 */
    {     "",     "",     }, /* 14 */
    {     "",     "",     }, /* 15 */

    {     "",     "",     }, /* 16 */
    {     "",     "",     }, /* 17 */
    {     "",     "",     }, /* 18 */
    {     "",     "",     }, /* 19 */

    {     "",     "",     }, /* 20 */

    //

    //  Gold
    {     "",     "",     }, /* 1 */
    {     "",     "",     }, /* 2 */
    {     "",     "",     }, /* 3 */
    {     "The Hand Of Rathmore",     "",     }, /* 4 */
    {     "",     "",     }, /* 5 */
    {     "",     "",     }, /* 6 */

};

FourLabels TR3_Treasure_Label [TR3_LEVELS] =
{
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },

    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },

    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },

    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },

    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },

    //
    { "Infada Stone",   "Element 115",  "The Eye Of Iris",  "Ora Dagger" },

    //  Gold
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
    { "",   "", "", "" },
};

//
/////////////////////////////////////////////////////////////////////////////
//  TR4
/////////////////////////////////////////////////////////////////////////////
char *TR4ItemsName [NB_BUTTONS] =
{
    "Winding Key (2)... \r\nMechanical Scarab (4) \r\nBoth (6)\r\nCombined (1) \r\n",       // 1
    "Small Waterskin (1)\r\n3 Liters (4)\r\n",      // 2
    "Large Waterskin (1)\r\n1 or 2 Liters (2)\r\n3 Liters (4)\r\n",     // 3
    "Scrap Of Paper (1) \r\n",  // 4
    "The Rules of Senet (1) \r\n",  // 5
    "Fire Circle Scroll (1) \r\n",  // 6
    "Golden Skull (1) \r\n"
        "Eye Of Horus (1) \r\n"
        "The Hand Of Orion (1) \r\n"
        "Ignition Key (1) \r\n"
        "Sun Talisman (1) \r\n"
        "Trident (1) \r\n"
        "Nitrous Oxide Feeder (1) \r\n"
        "Shovel (1) \r\n"
        "Stone of Maat (1) \r\n",   // 7
    "The Timeless Sands (1) \r\n"
        "The Hand Of Sirius (1) \r\n"
        "Canopic Jar 1 (1) \r\n"
        "Music Scroll (1) \r\n"
        "Car-Jack (1) \r\n"
        "Stone of Khepri (1) \r\n", // 8
    "Scarab Talisman (1) \r\n"
        "Canopic Jar 2 (1) \r\n"
        "Golden Star (1) \r\n"
        "Stone of Atum (1) \r\n",   // 9
    "The Golden Serpent (1) \r\n"
        "Ba Cartouche (1) \r\n"
        "Hook and Pole (1) \r\n"
        "Roof Key (1) \r\n"
        "Stone of Re (1) \r\n", // 10
    "Ra Cartouche (1) \r\n"
        "Portal Guardian (1) \r\n"
        "Weapon Code Key (1) \r\n"
        "Holy Scripture (1) \r\n",  // 11
    "Golden Vraeus (1) \r\n"
        "Horseman's Gem (1) \r\n"
        "Western Shaft Key (1) \r\n",   // 12
    "Guardian Key (1) \r\n"
        "Northern Shaft Key (1) \r\n",  // 13
    "Mine Detonator (1) \r\n"
        "Southern Shaft Key (1) \r\n",  // 14
    "Eastern Shaft Key (1) \r\n",   // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "Eye Piece (1) \r\n"
        "Eye Piece (2) \r\n"
        "Sun Goddess (2) \r\n"
        "Sun Disk (1) \r\n"
        "Cartouche Piece 1 (64) \r\n"
        "Cartouche Piece 2 (128) \r\n"
        "Wall Hook (64) \r\n"
        "Broken Handle (128) \r\n"
        "Valve Pipe (1) \r\n"
        "Nitrous Oxide Cannister (2) \r\n"
        "Car-Jack Body (4) \r\n"
        "Handle (8) \r\n"
        "Metal Blade (1) \r\n"
        "Wooden Handle (2) \r\n",   // 19
    "Ornate Handle (1) \r\n"
        "Hathor Effigy (2) \r\n"
        "Mine Detonator Body (64) \r\n"
        "Mine Position Data (128) \r\n",    // 20
    "Hypostyle Key (2) \r\n"
        "Temple Key (4) \r\n"
        "Token (1) \r\n"
        "Silver Key (1) \r\n",  // 21
    "Gate Key (2) \r\n"
        "Armoury Key (4) \r\n"
        "The Guards Keys (8) \r\n", // 22
    "", // 23
    "", // 24
    "Broken Beetle (1) \r\n"
        "Broken Glasses (2) \r\n"
        "Bag of Sand (1) \r\n"
        "Jerrycan (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Amulet of Horus (1)... \r\nBreast Plate (2) \r\nLeft Greave (4) \r\nRight Greave (8) \r\nLeft Gauntlet (16) \r\nRight Gauntlet (32) \r\nAll (63) \r\n",        // 29
};

//  Angkor Wat
char *TR4ItemsName01 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Golden Skull (1) \r\n",    // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  The Tomb of Seth
char *TR4ItemsName03 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Eye Of Horus (1) \r\n",    // 7
    "The Timeless Sands (1) \r\n",  // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Eye Piece (1) \r\nEye Piece (2) \r\n", // 19
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Burial Chambers
char *TR4ItemsName04 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "The Hand Of Orion (1) \r\n",   // 7
    "The Hand Of Sirius (1) \r\n",  // 8
    "Scarab Talisman (1) \r\n", // 9
    "The Golden Serpent (1) \r\n",  // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Amulet of Horus (1) \r\n",     // 29
};

//  Valley of the Kings
char *TR4ItemsName05 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Ignition Key (1) \r\n",    // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  KV5
char *TR4ItemsName06 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Ignition Key (1) \r\n",    // 7
    "Canopic Jar 1 \r\n",   // 8
    "Canopic Jar 2 \r\n",   // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Temple of Karnak (first visit)
char *TR4ItemsName07 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Sun Talisman (1) \r\n",    // 7
    "Canopic Jar 1 (1) \r\n",   // 8
    "Canopic Jar 2 (1) \r\n",   // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Sun Goddess (2) \r\n", // 19
    "", // 20
    "Hypostyle Key (2) \r\n",   // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Great Hypostyle Hall (first visit)
char *TR4ItemsName08 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Sun Talisman (1) \r\n",    // 7
    "Canopic Jar 1 (1) \r\n",   // 8
    "Canopic Jar 2 (1) \r\n",   // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Sun Disk (1) \r\nSun Goddess (2) \r\n",    // 19
    "", // 20
    "Hypostyle Key (2) \r\nTemple Key (4) \r\n",    // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Sacred Lake (first visit)
char *TR4ItemsName09 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Sun Talisman (1) \r\n",    // 7
    "Canopic Jar 1 (1) \r\n",   // 8
    "Canopic Jar 2 (1) \r\n",   // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Sun Disk (1) \r\nSun Goddess (2) \r\n",    // 19
    "", // 20
    "Hypostyle Key (2) \r\nTemple Key (4) \r\n",    // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  None or Tomb of Semerkhet
char *TR4ItemsName10 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "The Rules of Senet (1) \r\n",  // 5
    "", // 6
    "", // 7
    "", // 8
    "", // 9
    "Ba Cartouche (1) \r\n",    // 10
    "Ra Cartouche (1) \r\n",    // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Cartouche Piece 1 (64) \r\nCartouche Piece 2 (128) \r\n",  // 19
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Tomb of Semerkhet
char *TR4ItemsName11 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "The Rules of Senet (1) \r\n",  // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "Ba Cartouche (1) \r\n",    // 10
    "Ra Cartouche (1) \r\n",    // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Cartouche Piece 1 (64) \r\nCartouche Piece 2 (128) \r\n",  // 19
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Guardian of Semerkhet
char *TR4ItemsName12 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "Golden Vraeus (1) \r\n",   // 12
    "Guardian Key (1) \r\n",    // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Desert Railroad
char *TR4ItemsName13 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Alexandria
char *TR4ItemsName14 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Trident (1) \r\n", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "Hook and Pole (1) \r\n",   // 10
    "Portal Guardian (1) \r\n", // 11
    "Horseman's Gem (1) \r\n",  // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "Wall Hook (64) \r\nBroken Handle (128) \r\n",  // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "Token (1) \r\n",   // 21
    "Gate Key (2) \r\n",    // 22
    "",     // 23
    "",     // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Coastal Ruins (first visit)
char *TR4ItemsName15 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Trident (1) \r\n", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "Hook and Pole (1) \r\n",   // 10
    "Portal Guardian (1) \r\n", // 11
    "Horseman's Gem (1) \r\n",  // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "Wall Hook (64) \r\nBroken Handle (128) \r\n",  // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "Token (1) \r\n",   // 21
    "Gate Key (2) \r\n",    // 22
    "",     // 23
    "",     // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Pharos, Temple of Isis
char *TR4ItemsName16 [NB_BUTTONS] =
{
    "Winding Key (2)... \r\nMechanical Scarab (4) \r\nBoth (6)\r\nCombined (1) \r\n",       // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Trident (1) \r\n", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "Hook and Pole (1) \r\n",   // 10
    "Portal Guardian (1) \r\n", // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "Wall Hook (64) \r\nBroken Handle (128) \r\n",  // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "Token (1) \r\n",   // 21
    "Gate Key (2) \r\n",    // 22
    "",     // 23
    "",     // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Cleopatra's Palaces
char *TR4ItemsName17 [NB_BUTTONS] =
{
    "Winding Key (2)... \r\nMechanical Scarab (4) \r\nBoth (6)\r\nCombined (1) \r\n",       // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Portal Guardian (1) \r\n", // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar \r\n",       // 17
    "Black Beetle (1) \r\n",    // 18
    "",     // 19   
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Catacombs
char *TR4ItemsName18 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Trident (1) \r\n", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "", // 10
    "Portal Guardian (1) \r\n", // 11
    "Horseman's Gem (1) \r\n",  // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "", // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "Token (1) \r\n",   // 21
    "Gate Key (2) \r\n",    // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Temple of Poseidon
char *TR4ItemsName19 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Trident (1) \r\n", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "", // 10
    "Portal Guardian (1) \r\n", // 11
    "Horseman's Gem (1) \r\n",  // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "", // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Gantlet(s) \r\n",      // 29
};

//  Lost Library
char *TR4ItemsName20 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "Fire Circle Scroll (1) \r\n",  // 6
    "", // 7
    "Music Scroll (1) \r\n",    // 8
    "Golden Star (1) \r\n", // 9
    "", // 10
    "Portal Guardian (1) \r\n", // 11
    "Horseman's Gem (1) \r\n",  // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "", // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Left Gauntlet (16) \r\n",      // 29
};

//  Hall of Demetrius
char *TR4ItemsName21 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Portal Guardian (1) \r\n", // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "Pharos Knot (1) \r\n", // 16
    "Pharos Pillar (1) \r\n",   // 17
    "Black Beetle (1) \r\n",    // 18
    "", // 19
    "Ornate Handle (1) \r\nHathor Effigy (2) \r\n", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "Broken Beetle (1) \r\nBroken Glasses (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  City of the Dead (first visit)
char *TR4ItemsName22 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Trenches (first visit)
char *TR4ItemsName23 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Chambers Of Tulun
char *TR4ItemsName24 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Street Bazaar
char *TR4ItemsName25 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Citadel Gate
char *TR4ItemsName26 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Citadel
char *TR4ItemsName27 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Nitrous Oxide Feeder (1) \r\n",    // 7
    "Car-Jack (1) \r\n",    // 8
    "", // 9
    "Roof Key (1) \r\n",    // 10
    "Weapon Code Key (1) \r\n", // 11
    "", // 12
    "", // 13
    "Mine Detonator (1) \r\n",  // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Valve Pipe (1) \r\nNitrous Oxide Cannister (2) \r\nCar-Jack Body (4) \r\nHandle (8) \r\n", // 19
    "Mine Detonator Body (64) \r\nMine Position Data (128) \r\n",   // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Sphinx Complex
char *TR4ItemsName28 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "Shovel (1) \r\n",  // 7
    "", // 8
    "", // 9
    "", // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "Metal Blade (1) \r\nWooden Handle (2) \r\n",   // 19
    "", // 20
    "Silver Key (1) \r\n",  // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  
char *TR4ItemsName29 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "",     // 11
    "",     // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Underneath the Sphinx
char *TR4ItemsName30 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "Scrap Of Paper (1) \r\n",  // 4
    "", // 5
    "", // 6
    "Stone of Maat (1) \r\n",   // 7
    "Stone of Khepri (1) \r\n", // 8
    "Stone of Atum (1) \r\n",   // 9
    "Stone of Re (1) \r\n", // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "",     // 13
    "",     // 14
    "",     // 15
    "",     // 16
    "",     // 17
    "",     // 18
    "",     // 19   
    "",     // 20
    "",     // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Menkaure's Pyramid
char *TR4ItemsName31 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Inside Menkaure's Pyramid
char *TR4ItemsName32 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  The Mastabas
char *TR4ItemsName33 [NB_BUTTONS] =
{
    "",     // 1
    "Small Waterskin (1)\r\n3 Liters (4)\r\n",     // 2
    "Large Waterskin (1)\r\n1 or 2 Liters (2)\r\n3 Liters (4)\r\n",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "Northern Shaft Key (1) \r\n",  // 13
    "Southern Shaft Key (1) \r\n",  // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "Jerrycan (2) \r\nBag of Sand (1) \r\nBoth (3) \r\n",   // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Nothing
char *TR4ItemsName34 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "Northern Shaft Key (1) \r\n",  // 13
    "Southern Shaft Key (1) \r\n",  // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Khufu's Queen's Pyramids
char *TR4ItemsName35 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "Northern Shaft Key (1) \r\n",  // 13
    "Southern Shaft Key (1) \r\n",  // 14
    "Eastern Shaft Key (1) \r\n",   // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Inside the Great Pyramid
char *TR4ItemsName36 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "Western Shaft Key (1) \r\n",   // 12
    "Northern Shaft Key (1) \r\n",  // 13
    "Southern Shaft Key (1) \r\n",  // 14
    "Eastern Shaft Key (1) \r\n",   // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  Temple of Horus
char *TR4ItemsName37 [NB_BUTTONS] =
{
    "",     // 1
    "Small Waterskin (1)\r\n3 Liters (4)\r\n",     // 2
    "Large Waterskin (1)\r\n1 or 2 Liters (2)\r\n3 Liters (4)\r\n",     // 3
    "",     // 4
    "",     // 5
    "",     // 6
    "",     // 7
    "",     // 8
    "",     // 9
    "",     // 10
    "Holy Scripture (1) \r\n",  // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "Armoury Key (4) \r\nThe Guards Keys (8) \r\n", // 22
    "",     // 23
    "",     // 24
    "",     // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "Armor \r\n",       // 29
};

//  The Tomb of Seth
#define TR4ItemsName38 TR4ItemsName37


//
/////////////////////////////////////////////////////////////////////////////
//  TR5
/////////////////////////////////////////////////////////////////////////////

char *TR5ItemsName [NB_BUTTONS] =
{
    "Crowbar \r\n",         // 1
    "", // 2
    "", // 3
    "", // 4
    "Cloth \r\n",                   // 5
    "Vials of chloroform \r\n",     // 6
    "Garden Key (1) \r\n"
        "Valve wheel (1) \r\n"
        "A Fuse (1) \r\n"
        "Chaff flares (1) \r\n"
        "Nitrogen canister (1) \r\n"
        "Heart (1) \r\n"
        "Bone dust (1) \r\n"
        "Chalk (1) \r\n"
        "Helipad access key (1) \r\n",  // 7
    "Oxygen canister (1) \r\n"
        "Catapult (1) \r\n"
        "Silver coin (1) \r\n"
        "Iris artifact (1) \r\n",   // 8
    "Golden coin (1) \r\n"
        "Suit console (1) \r\n"
        "Iron clapper (1) \r\n"
        "Access code disc (1) \r\n"
        "Teleporter disc (1) \r\n", // 9
    "Mars symbol (1) \r\n"
        "Gemstone (1) \r\n"
        "Suit battery (1) \r\n"
        "Iris lab access (1) \r\n", // 10
    "Venus symbol (1) \r\n",    // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Gemstone piece (64) \r\n"
        "Gemstone piece (128) \r\n"
        "Aqualung (16) \r\n"
        "Suit console (32) \r\n"
        "Battery (+) (64) \r\n"
        "Battery (-) (128) \r\n"
        "Rubber tube (4) \r\n"
        "Pitchfork (8) \r\n"
        "Key bit (left) (1) \r\n"
        "Key bit (right) (2) \r\n", // 19
    "", // 20
    "Golden Key 2 (2) \r\n"
        "Cloth (1) \r\n"
        "Colosseum key 2 (2) \r\n"
        "Silver key (64) \r\n"
        "Swipe Card (128) \r\n"
        "Bronze key (32) \r\n"
        "Hammer (64) \r\n"
        "High level access card (128) \r\n"
        "Red alert! (1) \r\n"
        "Escape with the iris (1) \r\n",    // 21
    "", // 22
    "", // 23
    "", // 24
    "Mercury stone (1) \r\n"
        "Saturn Symbol (2) \r\n"
        "Philosophers stone (1) \r\n"
        "Spear Of Destiny (1) \r\n"
        "Bestiary (1) \r\n"
        "ID access card (1) \r\n"
        "Iris artifact (2) \r\n"
        "Low level access card (1) \r\n"
        "Restroom access card (2) \r\n",    // 25
    "", // 26
    "", // 27
    "", // 28
    "", // 29
};

//  Streets of Rome
char *TR5ItemsName01 [NB_BUTTONS] =
{
    "Crowbar \r\n",         // 1
    "",         // 2
    "",         // 3
    "",         // 4
    "", // 5
    "", // 6
    "Garden Key (1) \r\n",  // 7
    "", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "Golden Key 2 (2) \r\nCloth (1) \r\n",  // 21
    "", // 22
    "", // 23
    "", // 24
    "Mercury stone (1) \r\nSaturn Symbol (2) \r\n", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Trajan
char *TR5ItemsName02 [NB_BUTTONS] =
{
    "Crowbar \r\n",         // 1
    "",         // 2
    "",         // 3
    "",         // 4
    "", // 5
    "", // 6
    "Valve wheel (1) \r\n", // 7
    "", // 8
    "Golden coin (1) \r\n", // 9
    "Mars symbol (1) \r\n", // 10
    "Venus symbol (1) \r\n",    // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Coliseum
char *TR5ItemsName03 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "", // 7
    "", // 8
    "", // 9
    "Gemstone (1) \r\n",    // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Gemstone piece (64) \r\nGemstone piece (128) \r\n",    // 19
    "", // 20
    "Colosseum key 2 (2) \r\n", // 21
    "", // 22
    "", // 23
    "", // 24
    "Philosophers stone (1) \r\n",  // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

// The base
char *TR5ItemsName04 [NB_BUTTONS] =
{
    "",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "A Fuse (1) \r\n",  // 7
    "", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "Silver key (64) \r\nSwipe Card (128) \r\n",    // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  The Submarine
char *TR5ItemsName05 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "", // 7
    "", // 8
    "Suit console (1) \r\n",    // 9
    "Suit battery (1) \r\n",    // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Aqualung (16) \r\nSuit console (32) \r\nBattery (+) (64) \r\nBattery (-) (128) \r\n",  // 19
    "", // 20
    "Bronze key (32) \r\nSilver key (64) \r\n", // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Deepsea dive
char *TR5ItemsName06 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "Chaff flares (1) \r\n",    // 7
    "", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "Spear Of Destiny (1) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Sinking sub
char *TR5ItemsName07 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "Nitrogen canister (1) \r\n",   // 7
    "Oxygen canister (1) \r\n", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "Bronze key (32) \r\nSilver key (64) \r\nSwipe Card (128) \r\n",    // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Gallow Tree
char *TR5ItemsName08 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "Heart (1) \r\n",   // 7
    "Catapult (1) \r\n",    // 8
    "Iron clapper (1) \r\n",    // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Rubber tube (4) \r\nPitchfork (8) \r\n",   // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Labyrinth
char *TR5ItemsName09 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "Bone dust (1) \r\n",   // 7
    "", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "Bestiary (1) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Old mill
char *TR5ItemsName10 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "", // 5
    "", // 6
    "Chalk (1) \r\n",   // 7
    "Silver coin (1) \r\n", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "Bestiary (1) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  13 floor
char *TR5ItemsName11 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "Cloth \r\n",                   // 5
    "Vials of chloroform \r\n",     // 6
    "", // 7
    "", // 8
    "Access code disc (1) \r\n",    // 9
    "Iris lab access (1) \r\n", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "Hammer (64) \r\nHigh level access card (128) \r\nRed alert! (1) \r\n", // 21
    "", // 22
    "", // 23
    "", // 24
    "ID access card (1) \r\nIris artifact (2) \r\n",    // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Escape
char *TR5ItemsName12 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "Cloth \r\n",                   // 5
    "Vials of chloroform \r\n",     // 6
    "", // 7
    "Iris artifact (1) \r\n",   // 8
    "Teleporter disc (1) \r\n", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "Red alert! (1) \r\n",  // 21
    "", // 22
    "", // 23
    "", // 24
    "Low level access card (1) \r\nRestroom access card (2) \r\n",  // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//  Red Alert
char *TR5ItemsName14 [NB_BUTTONS] =
{
    "Crowbar \r\n",     // 1
    "",     // 2
    "",     // 3
    "",     // 4
    "Cloth \r\n",                   // 5
    "Vials of chloroform \r\n",     // 6
    "Helipad access key (1) \r\n",  // 7
    "Iris artifact (1) \r\n",   // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "Key bit (left) (1) \r\nKey bit (right) (2) \r\n",  // 19
    "",     // 20
    "Gold Key \r\n",        // 21
    "",     // 22
    "",     // 23
    "",     // 24
    "Grappling Gun (8) \r\n",       // 25
    "",     // 26
    "",     // 27
    "",     // 28
    "",     // 29
};

//
/////////////////////////////////////////////////////////////////////////////
//  TR4 Custom Levels
/////////////////////////////////////////////////////////////////////////////
char *TR49ItemsNameInd [ TR4NGMAXLEVEL ] [ NB_BUTTONS ];
char *TR49ItemsNameGen [ NB_BUTTONS ];

//
/////////////////////////////////////////////////////////////////////////////
//  TR2 andf TR3 Custom Levels
/////////////////////////////////////////////////////////////////////////////
FourLabels  TR39PuzzleName [ TR4NGMAXLEVEL ];
TwoLabels   TR39PickupName [ TR4NGMAXLEVEL ];
FourLabels  TR39KeyName [ TR4NGMAXLEVEL ];

