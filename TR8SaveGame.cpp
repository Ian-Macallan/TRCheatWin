#include "stdafx.h"

#include "TR8SaveGame.h"
#include "TRXGlobal.h"
#include "TRXTools.h"
#include "TR_Areas.h"

#include "TRXCHEATWIN.h"

extern CTRXCHEATWINApp theApp;

static char    TRR4NBSecrets [ ] =
{
    /* Angkor Wat */                    8,  //  01
    /* Race for the Iris */             0,  //  02
    /* Tomb of Seth */                  5,  //  03
    /* Burial Chambers */               5,  //  04
    /* Valley of the Kings */           3,  //  05
    /* KV5 */                           4,  //  06
    /* Temple of Karnak */              4,  //  07
    /* Great Hypostyle Hall */          0,  //  08
    /* Sacred Lake */                   1,  //  09
    /* Nothing */                       0,  //  10
    /* Tomb of Semerkhet */             7,  //  11
    /* Guardian of Semerkhet */         3,  //  12
    /* Desert Railroad */               3,  //  13
    /* Alexandria */                    1,  //  14
    /* Coastal Ruins */                 0,  //  15
    /* Pharos, Temple of Isis */        0,  //  16
    /* Cleopatra's Palaces */           1,  //  17
    /* Catacombs */                     4,  //  18
    /* Temple of Poseidon */            1,  //  19
    /* The Lost Library */              3,  //  20
    /* Hall of Demetrius */             1,  //  21
    /* City of the Dead */              2,  //  22
    /* Trenches */                      0,  //  23
    /* Chambers of Tulun */             2,  //  24
    /* Street Bazaar */                 1,  //  25
    /* Citadel Gate */                  1,  //  26
    /* Citadel */                       2,  //  27
    /* Sphinx Complex */                1,  //  28
    /* Nothing */                       0,  //  29
    /* Underneath the Sphinx */         1,  //  30
    /* Menkaure's Pyramid */            1,  //  31
    /* Inside Menkaure's Pyramid */     1,  //  32
    /* The Mastabas */                  1,  //  33
    /* The Great Pyramid */             1,  //  34
    /* Khufu's Queen's Pyramids */      1,  //  35
    /* Inside the Great Pyramid */      1,  //  36
    /* Temple of Horus */               0,  //  37
    /* Temple of Horus */               0,  //  38  index 37
    /* Office */                        0,  //  39
    /* Times Exclusive */               0,  //  40  index 39
};

//
/////////////////////////////////////////////////////////////////////////////
//  Indicator Table
/////////////////////////////////////////////////////////////////////////////
TRR45_INDICATORS IndicatorsTRR4Table [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE,   0,  "Standing", },
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE,   0,  "Standing", },

    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   TRUE,   1,  "Indicator 1", },
    {   FALSE,  0x02,   0x02,   0x00,   0x0c,   TRUE,   1,  "Indicator 2", },
    {   FALSE,  0x02,   0x02,   0x00,   0x1f,   TRUE,   1,  "Indicator 3", },
    {   FALSE,  0x02,   0x02,   0x00,   0xbd,   TRUE,   1,  "Indicator 4", },
    {   FALSE,  0x02,   0x02,   0x00,   0xdd,   TRUE,   1,  "Indicator 5", },

    {   FALSE,  0x0d,   0x0d,   0x00,   0x6c,   TRUE,   1,  "Swimming", },
    {   FALSE,  0x0d,   0x0d,   0x47,   0x6c,   TRUE,   1,  "Swimming", },

    {   FALSE,  0x0f,   0x0f,   0x00,   0x1f,   TRUE,   1,  "Quad Bike", },
    {   FALSE,  0x12,   0x00,   0x00,   0x02,   TRUE,   1,  "Flare", },

    {   FALSE,  0x10,   0x00,   0x51,   0x51,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x21,   0x21,   0x00,   0x6e,   TRUE,   1,  "In Water", },
    {   FALSE,  0x47,   0x47,   0x47,   0xde,   TRUE,   1,  "Kneeling", },
    {   FALSE,  0x57,   0x57,   0x47,   0x1f,   TRUE,   1,  "Indicator 10", },

    //
    {   FALSE,  0x61,   0x61,   0x00,   0x44,   TRUE,   1,  "Indicator 11", },
    {   FALSE,  0x75,   0x10,   0x47,   0x9c,   TRUE,   1,  "Indicator 12", },
    {   FALSE,  0x21,   0x21,   0x47,   0x6e,   TRUE,   1,  "Indicator 13", },
    {   FALSE,  0x50,   0x50,   0x00,   0x07,   TRUE,   1,  "Indicator 14", },
    {   FALSE,  0x24,   0x24,   0x00,   0x7b,   TRUE,   1,  "Indicator 15", },
    {   FALSE,  0x24,   0x24,   0x47,   0x7b,   TRUE,   1,  "Indicator 16", },

    {   FALSE,  0x27,   0x10,   0x00,   0xa3,   TRUE,   1,  "Indicator 17", },
    {   FALSE,  0x27,   0x15,   0x00,   0xa3,   TRUE,   1,  "Indicator 18", },
    {   FALSE,  0x0f,   0x0f,   0x47,   0x1d,   TRUE,   1,  "Bike", },
    {   FALSE,  0x0f,   0x0f,   0x00,   0x1d,   TRUE,   1,  "Bike", },
    {   FALSE,  0x00,   0x00,   0x47,   0x23,   TRUE,   1,  "Jeep", },
    {   FALSE,  0x02,   0x02,   0x47,   0xdd,   TRUE,   1,  "Indicator 19", }, 
    {   FALSE,  0x02,   0x02,   0x47,   0x0b,   TRUE,   1,  "Indicator 20", }, 
    {   FALSE,  0x02,   0x02,   0x47,   0x1f,   TRUE,   1,  "Climbing", },

    {   FALSE,  0x13,   0x13,   0x47,   0x61,   TRUE,   1,  "Indicator 21", }, 
    {   FALSE,  0x13,   0x13,   0x00,   0x61,   TRUE,   1,  "Indicator 22", }, 

    {   FALSE,  0x19,   0x19,   0x00,   0x4b,   TRUE,   1,  "Indicator 23", }, 

    {   FALSE,  0x01,   0x02,   0x47,   0x08,   TRUE,   1,  "Indicator 24", }, 

    //
    {   TRUE,   0xff,   0xff,   0xff,   0xff,   TRUE,   0,  "End", },
};
int IndicatorsTRR4TableCount = sizeof(IndicatorsTRR4Table)/sizeof(TRR45_INDICATORS);

//
/////////////////////////////////////////////////////////////////////////////
//  Indicator Table
/////////////////////////////////////////////////////////////////////////////
TRR45_INDICATORS IndicatorsTRR5Table [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE,   0,  "Standing", },
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE,   0,  "Standing", },

    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   TRUE,   1,  "Indicator 10", },
    {   FALSE,  0x02,   0x02,   0x00,   0x1f,   TRUE,   1,  "Indicator 11", },
    {   FALSE,  0x0d,   0x0d,   0x00,   0x6c,   TRUE,   1,  "Indicator 12", },
    {   FALSE,  0x0d,   0x12,   0x00,   0x6c,   TRUE,   1,  "Indicator 13", },
    {   FALSE,  0x12,   0x12,   0x00,   0x57,   TRUE,   1,  "Indicator 14", },
    {   FALSE,  0x13,   0x13,   0x47,   0xae,   TRUE,   1,  "Indicator 15", },
    {   FALSE,  0x47,   0x47,   0x00,   0xde,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x50,   0x50,   0x00,   0x07,   TRUE,   1,  "Indicator 16", }, 
    {   FALSE,  0x47,   0x57,   0x00,   0xde,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   TRUE,   1,  "Jumping", },
    {   FALSE,  0x09,   0x09,   0x00,   0x17,   TRUE,   1,  "Falling", },
    {   FALSE,  0x01,   0x02,   0x00,   0x0a,   TRUE,   1,  "Running", },
    //
    {   FALSE,  0x27,   0x10,   0x47,   0x87,   TRUE,   1,  "Indicator 20", },
    {   FALSE,  0x59,   0x10,   0x47,   0xd2,   TRUE,   1,  "Indicator 21", },
    {   FALSE,  0x59,   0x15,   0x47,   0xd1,   TRUE,   1,  "Indicator 22", },
    {   FALSE,  0x28,   0x0b,   0x47,   0x9e,   TRUE,   1,  "Indicator 23", },
    {   FALSE,  0x02,   0x36,   0x00,   0x0b,   TRUE,   1,  "Indicator 24", },
    {   FALSE,  0x59,   0x16,   0x00,   0xd2,   TRUE,   1,  "Indicator 25", },
    {   FALSE,  0x47,   0x57,   0x47,   0xde,   TRUE,   1,  "Indicator 26", },

    {   FALSE,  0x18,   0x18,   0x00,   0x46,   TRUE,   1,  "Indicator 27", },
    {   FALSE,  0x27,   0x10,   0x47,   0xa9,   TRUE,   1,  "Indicator 28", },
    {   FALSE,  0x59,   0x10,   0x47,   0xd8,   TRUE,   1,  "Indicator 29", },

    //
    {   TRUE,   0xff,   0xff,   0xff,   0xff,   TRUE,   0,  "End", },

};
int IndicatorsTRR5TableCount = sizeof(IndicatorsTRR5Table)/sizeof(TRR45_INDICATORS);


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
static TR_POSITION_RANGE TRR4IndicatorRange [ TR4_LEVELS ] =
{
    {   0x0600,     0x0700  },      /* 1 Angkor Wat */ 
    {   0x0100,     0x4000  },      /* 2 Race for the Iris */
    {   0x0100,     0x3000  },      /* 3 Tomb of Seth */
    {   0x0100,     0x3000  },      /* 4 Burial Chambers */
    {   0x0100,     0x3000  },      /* 5 Valley of the Kings */
    {   0x0100,     0x3000  },      /* 6 KV5 */
    {   0x0100,     0x3000  },      /* 7 Temple of Karnak */
    {   0x0100,     0x3000  },      /* 8 Great Hypostyle Hall */
    {   0x0100,     0x3000  },      /* 9 Sacred Lake */
    {   0x0100,     0x3000  },      /* 10 Nothing */ 
    {   0x0100,     0x3000  },      /* 11 Tomb of Semerkhet */
    {   0x0100,     0x3000  },      /* 12 Guardian of Semerkhet */
    {   0x0100,     0x3000  },      /* 13 Desert Railroad */
    {   0x0100,     0x3000  },      /* 14 Alexandria */
    {   0x0100,     0x3000  },      /* 15 Coastal Ruins */
    {   0x0100,     0x3000  },      /* 16 Pharos, Temple of Isis */
    {   0x0100,     0x3000  },      /* 17 Cleopatra's Palaces */
    {   0x0100,     0x3000  },      /* 18 Catacombs */
    {   0x0100,     0x3000  },      /* 19 Temple of Poseidon */
    {   0x0100,     0x3000  },      /* 20 The Lost Library */
    {   0x0100,     0x4000  },      /* 21 Hall of Demetrius */
    {   0x0100,     0x3000  },      /* 22 City of the Dead */
    {   0x0100,     0x3000  },      /* 23 Trenches */
    {   0x0100,     0x3000  },      /* 24 Chambers of Tulun */
    {   0x280,      0x3000  },      /* 25 Street Bazaar */ 
    {   0x0100,     0x3000  },      /* 26 Citadel Gate */
    {   0x0100,     0x3000  },      /* 27 Citadel */
    {   0x0100,     0x3000  },      /* 28 Sphinx Complex */
    {   0x0100,     0x3000  },      /* 29 Nothing */
    {   0x0100,     0x3000  },      /* 30 Underneath the Sphinx */
    {   0x0100,     0x3000  },      /* 31 Menkaure's Pyramid */
    {   0x0100,     0x3000  },      /* 32 Inside Menkaure's Pyramid */
    {   0x0100,     0x3000  },      /* 33 The Mastabas */
    {   0x0100,     0x3000  },      /* 34 The Great Pyramid */ 
    {   0x0100,     0x3000  },      /* 35 Khufu's Queen's Pyramids */
    {   0x0100,     0x3000  },      /* 36 Inside the Great Pyramid */
    {   0x0100,     0x4000  },      /* 37 Temple of Horus */
    {   0x0100,     0x5000  },      /* 38 Temple of Horus */ 
    {   0x0100,     0x3000  },      /* 39 Office */
    {   0x0100,     0x3000  },      /* 40 Times Exclusive */
};

//
static TR_POSITION_RANGE TRR5IndicatorRange [ TR5_LEVELS ] =
{
    {   0x0100,     0x3000 },       //      For Level 1 Street
    {   0x0100,     0x3000 },       //      for Level 2 trajan
    {   0x0100,     0x3000 },       //      for Level 3 colise
    {   0x0100,     0x3000 },       //      for Level 4 the base
    {   0x0100,     0x3000 },       //      for Level 5 the submarine
    {   0x0100,     0x3000 },       //      for Level 6 deep sea
    {   0x0100,     0x3000 },       //      for Level 7 Sinking
    {   0x0100,     0x3000 },       //      for Level 8 Gallow
    {   0x0100,     0x3000 },       //      for Level 9 Labyrith
    {   0x0100,     0x3000 },       //      for Level 10 Old Mill
    {   0x0100,     0x3000 },       //      for Level 11 13th floor
    {   0x0100,     0x3000 },       //      for Level 12 Escape with iris
    {   0x0100,     0x3000 },       //      for Level 13 Security Breach
    {   0x0100,     0x3000 },       //      for Level 14 Red Alert
};


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::m_TR4_Start = { NULL };
char *CTR8SaveGame::m_TR5_Start = { NULL };
char *CTR8SaveGame::m_TR6_Start = { NULL };

char *CTR8SaveGame::m_TR4_Blocks [ NB_SLOT_456 ] = { NULL };
char *CTR8SaveGame::m_TR5_Blocks [ NB_SLOT_456 ] = { NULL };
char *CTR8SaveGame::m_TR6_Blocks [ NB_SLOT_456 ] = { NULL };

GUN_TR4 *CTR8SaveGame::m_TR4_Guns [ NB_SLOT_456 ] = { NULL };
GUN_TR5 *CTR8SaveGame::m_TR5_Guns [ NB_SLOT_456 ] = { NULL };
GUN_TR6 *CTR8SaveGame::m_TR6_Guns [ NB_SLOT_456 ] = { NULL };

char    *CTR8SaveGame::m_pBuffer            = NULL;
char    *CTR8SaveGame::m_pBufferBackup      = NULL;
int     CTR8SaveGame::m_iSaveLength         = 0;

//
BOOL TraceMode456                           = FALSE;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR8SaveGame    *CTR8SaveGame::m_pInstance = NULL;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR8SaveGame, CObject)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR8SaveGame::CTR8SaveGame(void)
{
    Init(true);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR8SaveGame *CTR8SaveGame::GetInstance(BOOL bCreate)
{
    if ( bCreate && m_pInstance == NULL )
    {
        m_pInstance = new CTR8SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR8SaveGame *CTR8SaveGame::I(BOOL bCreate)
{
    return GetInstance(bCreate);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::IsBufferModified()
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
BOOL CTR8SaveGame::WriteFile(const char *oFilename)
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
void CTR8SaveGame::Discard()
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
void CTR8SaveGame::Init(bool bFromContructor)
{
    m_hFile     = NULL;
    ZeroMemory ( m_szFilename, sizeof(m_szFilename) );
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );


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
CTR8SaveGame::~CTR8SaveGame(void)
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
char *CTR8SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns )
{
    char *pFound = NULL;

    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            WORD m16, WORD grenade, WORD harpoon,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns  )
{
    char *pFound = NULL;


    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::SearchGunStructure (    char *pFrom, char *pUntil, WORD guns,
                                            WORD magnum, WORD uzi, WORD riot,
                                            WORD m16, WORD grenade, WORD harpoon, WORD rocket,
                                            int &countResult, int &valueResult,
                                            WORD mask,
                                            bool allowZero, bool noGuns  )
{
    char *pFound = NULL;


    return pFound;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::SearchGunStructure (    int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::SearchGunStructure ( int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                WORD m16, WORD grenade, WORD harpoon,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::SearchGunStructure ( int tombraider, int block, WORD guns,
                WORD magnum, WORD uzi, WORD riot,
                WORD m16, WORD grenade, WORD harpoon, WORD rocket,
                int &countResult, int &valueResult,
                WORD mask,
                bool allowZero, bool noGuns )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTR8SaveGame::RelativeAddress ( const void *pAddress )
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
char *CTR8SaveGame::getBufferAddress()
{
    return m_pBuffer;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR8SaveGame::getBufferBackupAddress()
{
    return m_pBufferBackup;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR8SaveGame::getBufferLength()
{
    return m_iSaveLength;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::ReadFile ( const char *pFilename )
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
        if ( m_iSaveLength != TR456LEVELSIZE )
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
BOOL CTR8SaveGame::writeFile ( const char *pFilename )
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
//  Set  m_TRx_Start etc..
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::Load ( )
{
    int iCount      = 0;
    int tombraider  = 1;
    int block       = 0;
    if ( m_pBuffer != NULL )
    {
        TR456Savegame *pSaveGame = (TR456Savegame *) m_pBuffer;
        m_TR4_Start = (char *) & pSaveGame->trr4s;
        m_TR5_Start = (char *) & pSaveGame->trr5s;
        m_TR6_Start = (char *) & pSaveGame->trr6s;

        for ( int i = 0; i < NB_SLOT_456; i++ )
        {
            m_TR4_Blocks [ i ] = (char *) &pSaveGame->trr4s.trr4 [ i ];
            m_TR5_Blocks [ i ] = (char *) &pSaveGame->trr5s.trr5 [ i ];
            m_TR6_Blocks [ i ] = (char *) &pSaveGame->trr6s.trr6 [ i ];


            m_TR4_Guns [ i ] = &pSaveGame->trr4s.trr4 [ i ].table.m_GunTR4;
            m_TR5_Guns [ i ] = &pSaveGame->trr5s.trr5 [ i ].table.m_GunTR5;
            // m_TR6_Guns [ i ] = &pSaveGame->trr6s.trr6 [ i ].table.m_GunTR6;
        }
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::GetSaveNumber ( int tombraider, int block )
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
void CTR8SaveGame::SetSaveNumber ( int tombraider, int block, WORD saveNo )
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
BYTE CTR8SaveGame::GetBlockLevelNumber ( int tombraider, int block )
{
    BYTE    *levelAddress = GetBlockLevelAddress ( tombraider, block );
    if ( levelAddress != NULL )
    {
        //
        if ( tombraider == 4 )
        {
            int iLevel = 0;

            for ( int i = 0; i < m_cLevelsLen; i++ )
            {
                if ( levelAddress [ i ] < iLevel )
                {
                    // break;
                }

                if ( levelAddress [ i ] != 0 )
                {
                    iLevel = levelAddress [ i ];
                }
            }

            //  Some case where level not found
            if ( iLevel == 0 )
            {
                // iLevel = 1;
            }


            return iLevel;
        }

        //
        if ( tombraider == 5 )
        {
            return *levelAddress;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR8SaveGame::GetBlockElapsed ( int tombraider, int block )
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
const char *CTR8SaveGame::GetBlockDistance ( int tombraider, int block )
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
bool CTR8SaveGame::isKnown(int tombraider, const char *position, const char *pStart)
{
    BYTE byte1 = *( position - 7 );
    BYTE byte2 = *( position - 6 );
    BYTE byte3 = *( position - 5 );
    BYTE byte4 = *( position - 4 );

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );
    if ( tombraider == 4 )
    {
        for ( int i = 0; i < IndicatorsTRR4TableCount; i++ )
        {
            if ( IndicatorsTRR4Table [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTRR4Table [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
            {
                continue;
            }

            if ( IndicatorsTRR4Table [ i ].useB3 && IndicatorsTRR4Table [ i ].b3 != byte3 )
            {
                continue;
            }

            if ( byte1 == IndicatorsTRR4Table [ i ].b1 && byte2 == IndicatorsTRR4Table [ i ].b2 && byte4 == IndicatorsTRR4Table [ i ].b4 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRR4Table [ i ].szLabel );

#ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress1 = CTRXTools::RelativeAddress ( position, m_pBuffer );
                DWORD dwRelativeAddress2 = CTRXTools::RelativeAddress ( position, pStart );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Looking 0x%08x : 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x\n", 
                    dwRelativeAddress1, dwRelativeAddress2, byte1 & 0xff, byte2 & 0xff, byte3 & 0xff, byte4 & 0xff );
                OutputDebugString ( szDebugString );
#endif
                return true; 
            }
        }
    }

    if ( tombraider == 5 )
    {
        for ( int i = 0; i < IndicatorsTRR5TableCount; i++ )
        {
            if ( IndicatorsTRR5Table [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTRR5Table [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
            {
                continue;
            }

            if ( IndicatorsTRR5Table [ i ].useB3 && IndicatorsTRR5Table [ i ].b3 != byte3 )
            {
                continue;
            }

            if ( byte1 == IndicatorsTRR5Table [ i ].b1 && byte2 == IndicatorsTRR5Table [ i ].b2 && byte4 == IndicatorsTRR5Table [ i ].b4 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRR5Table [ i ].szLabel );

#ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress1 = CTRXTools::RelativeAddress ( position, m_pBuffer );
                DWORD dwRelativeAddress2 = CTRXTools::RelativeAddress ( position, pStart );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Looking 0x%08x : 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x\n", 
                    dwRelativeAddress1, dwRelativeAddress2, byte1 & 0xff, byte2 & 0xff, byte3 & 0xff, byte4 & 0xff );
                OutputDebugString ( szDebugString );
#endif
                return true; 
            }
        }
    }
    return false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR8SaveGame::getPositionLabel(int tombraider, const char *position)
{
    BYTE byte1 = *( position - 7 );
    BYTE byte2 = *( position - 6 );
    BYTE byte3 = *( position - 5 );
    BYTE byte4 = *( position - 4 );

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    if ( tombraider == 4 )
    {
        for ( int i = 0; i < IndicatorsTRR4TableCount; i++ )
        {
            if ( IndicatorsTRR4Table [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTRR4Table [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
            {
                continue;
            }

            if ( IndicatorsTRR4Table [ i ].useB3 && IndicatorsTRR4Table [ i ].b3 != byte3 )
            {
                continue;
            }

            if ( byte1 == IndicatorsTRR4Table [ i ].b1 && byte2 == IndicatorsTRR4Table [ i ].b2 && byte4 == IndicatorsTRR4Table [ i ].b4 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRR4Table [ i ].szLabel );
    #ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( position, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x Found\n", 
                    dwRelativeAddress, byte1 & 0xff, byte2 & 0xff, byte3 & 0xff, byte4 & 0xff );
                OutputDebugString ( szDebugString );
    #endif
                return m_szIndicatorLabel; 
            }
        }
    }

    if ( tombraider == 5 )
    {
        for ( int i = 0; i < IndicatorsTRR5TableCount; i++ )
        {
            if ( IndicatorsTRR5Table [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTRR5Table [ i ].step > CTRXGlobal::m_TRRIndexMaximum )
            {
                continue;
            }

            if ( IndicatorsTRR5Table [ i ].useB3 && IndicatorsTRR5Table [ i ].b3 != byte3 )
            {
                continue;
            }

            if ( byte1 == IndicatorsTRR5Table [ i ].b1 && byte2 == IndicatorsTRR5Table [ i ].b2 && byte4 == IndicatorsTRR5Table [ i ].b4 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTRR5Table [ i ].szLabel );
    #ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( position, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x Found\n", 
                    dwRelativeAddress, byte1 & 0xff, byte2 & 0xff, byte3 & 0xff, byte4 & 0xff );
                OutputDebugString ( szDebugString );
    #endif
                return m_szIndicatorLabel; 
            }
        }
    }
//
#ifdef _DEBUG
    static char szDebugString [ MAX_PATH ];
    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( position, m_pBuffer );
    sprintf_s ( szDebugString, sizeof(szDebugString), 
        "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x Not Found\n", 
        dwRelativeAddress, byte1 & 0xff, byte2 & 0xff, byte3 & 0xff, byte4 & 0xff );
    OutputDebugString ( szDebugString );
#endif

    return m_szIndicatorLabel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR8SaveGame::GetRealHealthAddress ( int tombraider, int block )
{
#ifdef _DEBUG
    OutputDebugString ( "GetRealHealthAddress TR Remastered\n" );
#endif

    char *pStart        = NULL;
    DWORD iStart        = 0x0100;
    DWORD iLength       = 0x3000;
    int level = GetBlockLevelNumber(tombraider, block );
    if ( level <= 0 )
    {
        return NULL;
    }

    //
    int levelIndex = level - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            pStart      = (char*) GetBlockSlot ( tombraider, block );
            iStart      = TRR4IndicatorRange [ levelIndex % TR4_LEVELS ].minOffset;
            iLength     = TRR4IndicatorRange [ levelIndex % TR4_LEVELS ].maxOffset;
            break;
        }

        case 5:
        {
            pStart      = (char*) GetBlockSlot ( tombraider, block );
            iStart      = TRR5IndicatorRange [ levelIndex % TR5_LEVELS ].minOffset;
            iLength     = TRR5IndicatorRange [ levelIndex % TR5_LEVELS ].maxOffset;
            break;
        }

        case 6:
        {
            return NULL;
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
    for ( DWORD i = iStart; i <= iLength; i++ )
    {
        char *position  = pStart + i;
        int relative    = (int)( position - m_pBuffer );

        //  Are we on full health
        WORD *pHealth = 0;
#if 0
        pHealth = (WORD *)position;
        if ( *pHealth == 1000 )
        {
            getPositionLabel ( tombraider, position );
            if ( strlen(m_szIndicatorLabel) == 0 )
            {
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), "Full Health" );
            }
            return pHealth;
        }
#endif
        // printf ( "%lx ", relative );
        if ( isKnown ( tombraider, position, pStart ) )
        {
            pHealth = ( WORD * ) position;
            if ( tombraider == 4 && ( *pHealth >= 0 && *pHealth <= 1000 ) )
            {
                //
                //  Verify Position
                for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
                {
                   TR8_POSITION *position = (TR8_POSITION *)( ( BYTE * ) pHealth - offsetof(TR8_POSITION,health) - i );
            
                    DWORD dwSouthToNorth    = ( DWORD) position->wSouthToNorth * TR4_FACTOR;
                    DWORD dwVertical        = ( DWORD ) position->wVertical * TR4_FACTOR;
                    DWORD dwWestToEast      = ( DWORD ) position->wWestToEast * TR4_FACTOR;
                    WORD wRoom              = position->cRoom;

                    BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex,  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                    if ( bCheck )
                    {
                        //
                        return pHealth;
                    }
                }
            }

            if ( tombraider == 5 && ( *pHealth > 0 && *pHealth <= 1000 ) )
            {
                //
                //  Verify Position
                for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
                {
                   TR8_POSITION *position = (TR8_POSITION *)( ( BYTE * ) pHealth - offsetof(TR8_POSITION,health) - i );
            
                    DWORD dwSouthToNorth    = ( DWORD) position->wSouthToNorth * TR5_FACTOR;
                    DWORD dwVertical        = ( DWORD ) position->wVertical * TR5_FACTOR;
                    DWORD dwWestToEast      = ( DWORD ) position->wWestToEast * TR5_FACTOR;
                    WORD wRoom              = position->cRoom;

                    BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex,  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                    if ( bCheck )
                    {
                        //
                        return pHealth;
                    }
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
TR8_POSITION *CTR8SaveGame::GetPositionAddress ( int tombraider, int block )
{
#ifdef _DEBUG
    OutputDebugString ( "GetPositionAddress TR Remastered\n" );
#endif

    int level = GetBlockLevelNumber(tombraider, block );
    if ( level <= 0 )
    {
        return NULL;
    }

    //
    int levelIndex = level - 1;

    //
    BYTE *pHealth = (BYTE *) GetRealHealthAddress ( tombraider, block );
    if ( pHealth )
    {
        switch ( tombraider )
        {
            case 4:
            {
                //
                //  Verify Position
                for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
                {
                   TR8_POSITION *position = (TR8_POSITION *)( ( BYTE * ) pHealth - offsetof(TR8_POSITION,health) - i );
            
                    DWORD dwSouthToNorth    = ( DWORD) position->wSouthToNorth * TR4_FACTOR;
                    DWORD dwVertical        = ( DWORD ) position->wVertical * TR4_FACTOR;
                    DWORD dwWestToEast      = ( DWORD ) position->wWestToEast * TR4_FACTOR;
                    WORD wRoom              = position->cRoom;

                    BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex,  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                    if ( bCheck )
                    {
                        //
                        return position;
                    }
                }
                break;
            }
            case 5:
            {
                //
                //  Verify Position
                for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
                {
                   TR8_POSITION *position = (TR8_POSITION *)( ( BYTE * ) pHealth - offsetof(TR8_POSITION,health) - i );
            
                    DWORD dwSouthToNorth    = ( DWORD) position->wSouthToNorth * TR5_FACTOR;
                    DWORD dwVertical        = ( DWORD ) position->wVertical * TR5_FACTOR;
                    DWORD dwWestToEast      = ( DWORD ) position->wWestToEast * TR5_FACTOR;
                    WORD wRoom              = position->cRoom;

                    BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex,  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                    if ( bCheck )
                    {
                        //
                        return position;
                    }
                }
                break;
            }
            case 6:
            {
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
WORD CTR8SaveGame::GetRealHealth ( int tombraider, int block )
{
    WORD *pAddress = GetRealHealthAddress ( tombraider, block );
    if ( pAddress != NULL )
    {
        int level = GetBlockLevelNumber(tombraider, block );

        //
        if ( tombraider == 4 && *pAddress == 0 )
        {
//            return 1000;
        }
        return *pAddress;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetRealHealth ( int tombraider, int block, WORD value )
{
    WORD *pAddress = GetRealHealthAddress ( tombraider, block );
    if ( pAddress != NULL )
    {
        int level = GetBlockLevelNumber(tombraider, block );

        //
        if ( tombraider == 4 && value == 1000 )
        {
//            value = 0;
        }
        *pAddress = value;
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::KillWillard ( int tombraider, int block )
{
    if ( tombraider == 3 && GetBlockLevelNumber (tombraider, block ) == 19 )
    {
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::KillTR1Boss ( int tombraider, int block )
{
    if ( tombraider == 1 && GetBlockLevelNumber (tombraider, block ) == 15 )
    {
    }
    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::IsKillEnabled ( int tombraider, int block )
{
    if ( tombraider == 1 && GetBlockLevelNumber (tombraider, block ) == 15 )
    {
    }

    if ( tombraider == 3 && GetBlockLevelNumber (tombraider, block ) == 19 )
    {
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR8SaveGame::GetSaveAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            TABLE_TR4 *pBlock = (TABLE_TR4 *)GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
                return &pBlock->savenumber;
            }
            break;
        }
        case 5:
        {
            TABLE_TR5 *pBlock = (TABLE_TR5 *)GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
                return &pBlock->savenumber;
            }
            break;
        }
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetBlockStart ( int tombraider )
{

    switch ( tombraider )
    {
        //
        case 4:
        {
            return m_TR4_Start;
            break;
        }

        //
        case 5:
        {
            return m_TR5_Start;
            break;
        }

        //
        case 6:
        {
            return m_TR6_Start;
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetBlockEnd ( int tombraider )
{
    BYTE *pStart = ( BYTE* ) GetBlockStart ( tombraider );
    if ( pStart != NULL )
    {
        pStart = pStart + GetBlockLength(tombraider);
    }

    return pStart;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return directly m_TRx_Blocks [ b ]
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetBlockSlot ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case 4 : return  m_TR4_Blocks [ block ];
        case 5 : return  m_TR5_Blocks [ block ];
        case 6 : return  m_TR6_Blocks [ block ];
    }
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return m_TRx_Blocks [ b ] [ s ] but iif null can return the theorical address
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetSlotAddress ( int tombraider, int block, bool bForce )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            void *pVoid = (void *) m_TR4_Blocks [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_TR4_Start + block * GetSlotLength ( tombraider );
            }
            break;
        }

        //
        case 5:
        {
            void *pVoid = (void *) m_TR5_Blocks [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }
            if ( bForce )
            {
                return m_TR5_Start + block * GetSlotLength ( tombraider );
            }
            break;
        }

        //
        case 6:
        {
            void *pVoid = (void *) m_TR6_Blocks [ block ];
            if ( pVoid != NULL )
            {
                return pVoid;
            }

            if ( bForce )
            {
                return m_TR6_Start + block * GetSlotLength ( tombraider );
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
void *CTR8SaveGame::GetGunAddress ( int tombraider, int block )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            return (void *) m_TR4_Guns [ block ];
            break;
        }

        //
        case 5:
        {
            return (void *) m_TR5_Guns [ block ];
            break;
        }

        //
        case 6:
        {
            return (void *) m_TR6_Guns [ block ];
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  return  m_TRX_Guns [ block ] [ slot ]
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetBlockGuns ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case 4 : return  m_TR4_Guns [ block ];
        case 5 : return  m_TR5_Guns [ block ];
        case 6 : return  m_TR6_Guns [ block ];
    }
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get Full Block Length
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::GetBlockLength ( int tombraider )
{
    switch ( tombraider )
    {
        case 4 : return TR456_SLOT_LENGTH * NB_SLOT_456;
        case 5 : return TR456_SLOT_LENGTH * NB_SLOT_456;
        case 6 : return TR456_SLOT_LENGTH * NB_SLOT_456;
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Get a Slot Length
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::GetSlotLength ( int tombraider )
{
    switch ( tombraider )
    {
        case 4 : return TR456_SLOT_LENGTH;
        case 5 : return TR456_SLOT_LENGTH;
        case 6 : return TR456_SLOT_LENGTH;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::GetBlockIndicator ( int tombraider, int block )
{
    void *pStartBlock = GetBlockStart ( tombraider );
    if ( pStartBlock )
    {
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR8SaveGame::GetBlockHexa ( int tombraider, int block, int iStart, int iEnd )
{
    static char szResult [ 0x40  * 4 ];
    ZeroMemory ( szResult, sizeof(szResult) );

    BYTE *pBlock = (BYTE * ) GetSlotAddress ( tombraider, block );
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
BYTE *CTR8SaveGame::GetBlockLevelAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock = (TABLE_TR4 *) GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
                return pBlock->m_cLevels;
            }
            break;
        }

        //
        case 5:
        {
            TABLE_TR5 *pBlock = (TABLE_TR5 *) GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
                return & pBlock->m_cLevel;
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetBlockObjectAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock = (TABLE_TR4 *) GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
#ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress1 = CTRXTools::RelativeAddress ( pBlock->m_GunTR4.m_Object, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Objects 0x%08x\n", dwRelativeAddress1 );
                OutputDebugString ( szDebugString );
#endif
                return pBlock->m_GunTR4.m_Object;
            }
            break;
        }

        //
        case 5:
        {
            TABLE_TR5 *pBlock = (TABLE_TR5 *) GetSlotAddress ( tombraider, block );
            if ( pBlock )
            {
#ifdef _DEBUG
                static char szDebugString [ MAX_PATH ];
                DWORD dwRelativeAddress1 = CTRXTools::RelativeAddress ( pBlock->m_GunTR5.m_Object, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), 
                    "Objects 0x%08x\n", dwRelativeAddress1 );
                OutputDebugString ( szDebugString );
#endif
                return pBlock->m_GunTR5.m_Object;
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetBlockSecretsBaseAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetBlockSecretsAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR8SaveGame::GetBlockKillsAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTR8SaveGame::GetBlockKills ( int tombraider, int block )
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
DWORD *CTR8SaveGame::GetBlockDistanceAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
DWORD *CTR8SaveGame::GetBlockElapsedAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
BYTE *CTR8SaveGame::GetBlockCrystalAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            return NULL;
            break;
        }

        //
        case 5:
        {
            return NULL;
            break;
        }

        //
        case 6:
        {
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
BYTE *CTR8SaveGame::GetBlockPickupAddress ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::GetBlockPickup ( int tombraider, int block )
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
int CTR8SaveGame::countTag ( int tombraider, int block )
{
    int count = 0;

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return count;
    }

    void * pAddress = NULL;
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR8SaveGame::GetLevelName ( int tombraider, int level )
{
    switch ( tombraider )
    {
        case 4:
        {
            if ( level >= 1 && level <= sizeof(TR4LevelNames)/sizeof(const char*) )
            {
                return TR4LevelNames [ level - 1 ];
            }
            break;
        }
        case 5:
        {
            if ( level >= 1 && level <= sizeof(TR5LevelNames)/sizeof(const char*) )
            {
                return TR5LevelNames [ level - 1 ];
            }
            break;
        }
        case 6:
        {
            break;
        }
    }

    return "";
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::GetLevelCount ( int tombraider )
{
    switch ( tombraider )
    {
        case 4: return sizeof(TR4LevelNames) / sizeof(char *);
        case 5: return sizeof(TR5LevelNames) / sizeof(char *);
        case 6: // return sizeof(TR6Secrets);
            break;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTR8SaveGame::GetTRPlus ( int tombraider )
{
    if ( m_pBuffer == NULL )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case 4:
        {
            return m_pBuffer [ TR4_GAME_PLUS ];
            break;
        }
        case 5:
        {
            return m_pBuffer [ TR5_GAME_PLUS ];
            break;
        }
        case 6:
        {
            return m_pBuffer [ TR6_GAME_PLUS ];
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetTRPlus ( int tombraider, DWORD value )
{
    if ( m_pBuffer == NULL )
    {
        return;
    }

    switch ( tombraider )
    {
        case 4:
        {
            m_pBuffer [ TR4_GAME_PLUS ] = (BYTE ) ( value & 0xFF );
            break;
        }
        case 5:
        {
            m_pBuffer [ TR5_GAME_PLUS ] = (BYTE ) ( value & 0xFF );
            break;
        }
        case 6:
        {
            m_pBuffer [ TR6_GAME_PLUS ] = (BYTE ) ( value & 0xFF );
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::IsGamePlus ( int tombraider, int block )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pTable      = (TABLE_TR4 *) GetBlockSlot ( tombraider, block );
            if ( pTable != NULL )
            {
                return pTable->m_iPius;
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pTable      = (TABLE_TR5 *) GetBlockSlot ( tombraider, block );
            if ( pTable != NULL )
            {
                return pTable->m_iPius;
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetGamePlus ( int tombraider, int block, BYTE value )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pTable      = (TABLE_TR4 *) GetBlockSlot ( tombraider, block );
            if ( pTable != NULL )
            {
                pTable->m_iPius = value;
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pTable      = (TABLE_TR5 *) GetBlockSlot ( tombraider, block );
            if ( pTable != NULL )
            {
                pTable->m_iPius = value;
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//  return the first TABLE_TR1 / TABLE_TR2 / TABLE_TR3 pointer that has a mask & 1
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetDefaultBlockEntry ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    int levelNumber = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //  Normally The Slot is always the 24
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
void *CTR8SaveGame::SearchBlockEntry ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::GetDefaultGunEntry ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//  return a pointer on a GUN_TR1 / GUN_TR2 / GUN_TR3 Structure
/////////////////////////////////////////////////////////////////////////////
void *CTR8SaveGame::SearchGunEntry ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSlotMedipakUsedAddress(int tombraider, int block)
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }
    
    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSlotPickupAddress(int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR8SaveGame::GetSlotKillsAddress(int tombraider, int block)
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSlotCrystalAddress ( int tombraider, int block )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSlotTreasureAddress ( int tombraider, int block )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return NULL;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            return NULL;
            break;
        }

        //
        case 5:
        {
            return NULL;
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;

}
//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::GetBlockObject ( int tombraider, int block, int object )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case 4:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress = pObjectAddress + object;
                return *pAddress;
            }
            break;
        }

        case 5:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress = pObjectAddress + object;
                return *pAddress;
            }
            break;
        }

        case 6:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress = pObjectAddress + object;
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
BYTE *CTR8SaveGame::GetObjectBaseAddress ( int tombraider, int block )
{
    return GetBlockObjectAddress ( tombraider, block );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetBlockObject ( int tombraider, int block, int object, int val )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        case 4:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress  = pObjectAddress + object;
                *pAddress = val;
            }
            break;
        }

        case 5:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress  = pObjectAddress + object;
                *pAddress = val;
            }
            break;
        }

        case 6:
        {
            BYTE *pObjectAddress = (BYTE *) GetBlockObjectAddress ( tombraider, block );
            if ( pObjectAddress )
            {
                BYTE *pAddress  = pObjectAddress + object;
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
WORD *CTR8SaveGame::GetBlockAmmosHitsAddress(int tombraider, int block)
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
WORD *CTR8SaveGame::GetBlockAmmosUsedAddress(int tombraider, int block)
{
    if ( block >= NB_SLOT_456 )
    {
        tombraider  = 2;
        block       -= NB_SLOT_456;
    }

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
BYTE *CTR8SaveGame::GetBlockMedipakUsedAddress(int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
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
BYTE CTR8SaveGame::GetBlockCrystal(int tombraider, int block)
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
BYTE *CTR8SaveGame::GetBlockSecretsCurrentAddress(int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::CompareSecretsCompleted ( int tombraider, int block, BYTE *pStartCompleted )
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return First Address of Secrets Completed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::SearchSecretsCompleted ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Enable address is relative to Gun for TR3 other wise it is fixed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSecretsCompletedBaseAddress (int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //  
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Enable address is relative to Gun for TR3 other wise it is fixed
/////////////////////////////////////////////////////////////////////////////
BYTE *CTR8SaveGame::GetSecretsCompletedAddress(int tombraider, int block, int iLevel)
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //  
        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::GetSecretsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR8SaveGame::GetKillssForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR8SaveGame::GetPickupsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD    CTR8SaveGame::GetCrystalsForLevel ( int tombraider, int levelindex )
{
    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return 0;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::GetBlocksSecrets ( int tombraider, int block, BYTE vector[32] )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }
    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::GetBlockSecretsAcquiredBits(int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }
    }

    return 0;
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR8SaveGame::GetBlockSecretsAcquired(int tombraider, int block)
{
    switch ( tombraider )
    {
        case 4:
        {
            return GetBlockSecretsAcquiredBits ( tombraider, block );
        }
        case 5:
        {
            return GetBlockSecretsAcquiredBits ( tombraider, block );
        }
        case 6:
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
BYTE CTR8SaveGame::GetSecretsCompletedBits(int tombraider, int block, int iLevel)
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return CTRXTools::CountBitsB ( *pCompleted );
            }
            break;
        }

        case 5:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return CTRXTools::CountBitsB ( *pCompleted );
            }
            break;
        }

        case 6:
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
BYTE CTR8SaveGame::GetSecretsCompletedValue(int tombraider, int block, int iLevel)
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return *pCompleted;
            }
            break;
        }

        case 5:
        {
            BYTE *pCompleted    = GetSecretsCompletedAddress ( tombraider, block, levelindex );
            if ( pCompleted )
            {
                return *pCompleted;
            }
            break;
        }

        case 6:
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
BYTE CTR8SaveGame::GetSecretsCompleted(int tombraider, int block, int iLevel)
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
        case 4:
        {
            return GetSecretsCompletedValue ( tombraider, block, levelindex );
        }

        case 5:
        {
            return GetSecretsCompletedValue ( tombraider, block, levelindex );
        }

        case 6:
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
void CTR8SaveGame::SetBlockPickup(int tombraider, int block, BYTE pickup)
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
void CTR8SaveGame::SetBlockKills(int tombraider, int block, WORD kills)
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
void CTR8SaveGame::SetBlockCrystal(int tombraider, int block, BYTE crystals)
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
BYTE CTR8SaveGame::GetBlockSecretsCurrentValue(int tombraider, int block)
{
    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            BYTE *pCount                = GetBlockSecretsCurrentAddress ( tombraider, block );
            if ( pCount )
            {
                int bits                = *pCount;
                return bits;
            }
            break;
        }

        case 5:
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
        case 6:
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
BYTE CTR8SaveGame::GetBlockSecretsCurrent(int tombraider, int block)
{
    switch ( tombraider )
    {
        case 4:
        {
            return GetBlockSecretsCurrentValue ( tombraider, block );
        }
        case 5:
        {
            return GetBlockSecretsCurrentValue ( tombraider, block );
        }
        case 6:
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
BYTE CTR8SaveGame::GetSecretsMaximum(int tombraider, int block)
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0 ;
    }

    //
    WORD    levelindex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetBlockSecretsAcquiredAll(int tombraider, int block, int excludeLevel )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //  
    WORD    currentLevelIndex = GetBlockLevelNumber ( tombraider, block ) - 1;

    //
    switch ( tombraider )
    {
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        //  Set Stats
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetBlockSecretsAcquired(int tombraider, int block, BYTE secrets)
{

    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        case 5:
        {
            break;
        }

        //  For TR3 The Secrets are in Count
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::ReNumber ( )
{
    //
    WORD saveNo [ NB_SLOT_456 ];
    WORD levelNo [ NB_SLOT_456 ];
    WORD blockNo [ NB_SLOT_456 ];

    //
    for ( int tombraider = 4; tombraider <= 5; tombraider++ )
    {
        ZeroMemory ( saveNo, sizeof(saveNo) );
        ZeroMemory ( levelNo, sizeof(levelNo) );
        ZeroMemory ( blockNo, sizeof(blockNo) );

        //  Get All
        for ( int block = 0; block < NB_SLOT_456; block++ )
        {
            saveNo [ block ]    = GetSaveNumber ( tombraider, block );
            levelNo [ block ]   = GetBlockLevelNumber ( tombraider, block );
            blockNo [ block ]   = block;
        }

        //
        //  Sort by Level / SaveNo
        for ( int i = 0; i < NB_SLOT_456; i++ )
        {
            //
            for ( int j = i + 1; j < NB_SLOT_456; j++ )
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
        for ( int block = 0; block < NB_SLOT_456; block++ )
        {
            WORD *pSaveAddress  = GetSaveAddress ( tombraider, blockNo [ block ] );
            int level           = GetBlockLevelNumber ( tombraider, blockNo [ block ] );
            int save            = GetSaveNumber ( tombraider, blockNo [ block ] );
            if ( pSaveAddress != NULL && save != 0 && level != 0 )
            {
                saveNo [ block ]    = count;
                count++;
            }
        }

        //
        for ( int block = 0; block < NB_SLOT_456; block++ )
        {
            WORD *pSaveAddress  = GetSaveAddress ( tombraider, blockNo [ block ] );
            int level           = GetBlockLevelNumber ( tombraider, blockNo [ block ] );
            int save            = GetSaveNumber ( tombraider, blockNo [ block ] );
            if ( pSaveAddress != NULL && save != 0 && level != 0 )
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
BOOL CTR8SaveGame::Delete ( int tombraider, int block )
{
    switch ( tombraider )
    {
        case 4:
        {
            char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
            memset ( pBuffer, 0, GetSlotLength ( tombraider ) );
            break;
        }
        case 5:
        {
            char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
            memset ( pBuffer, 0, GetSlotLength ( tombraider ) );
            break;
        }
        case 6:
        {
            char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
            memset ( pBuffer, 0, GetSlotLength ( tombraider ) );
            break;
        }
    }

    return TRUE;

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::Export ( const char *pFilename, int tombraider, int block )
{
    BOOL    bResult = FALSE;

    //
    FILE *hFile = NULL;
    fopen_s ( & hFile, pFilename, "wb" );
    if ( hFile != NULL )
    {
        switch ( tombraider )
        {
            case 4:
            {
                char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
                fwrite ( pBuffer, 1, GetSlotLength ( tombraider ), hFile );
                bResult = TRUE;
                break;
            }
            case 5:
            {
                char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
                fwrite ( pBuffer, 1, GetSlotLength ( tombraider ), hFile );
                bResult = TRUE;
                break;
            }
            case 6:
            {
                char    *pBuffer = ( char *) GetBlockSlot ( tombraider, block );
                fwrite ( pBuffer, 1, GetSlotLength ( tombraider ), hFile );
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
BOOL CTR8SaveGame::Import( const char *pFilename, int tombraider, int block )
{
    BOOL    bResult = FALSE;

    //
    FILE *hFile = NULL;
    fopen_s ( & hFile, pFilename, "rb" );
    if ( hFile != NULL )
    {
        switch ( tombraider )
        {
            case 4:
            {
                char    *pSrcBuffer = ( char *) malloc ( GetSlotLength ( tombraider ) + 1 );
                char    *pObjBuffer = ( char *) GetBlockSlot ( tombraider, block );
                size_t iRead = fread ( pSrcBuffer, 1, GetSlotLength ( tombraider ) + 1, hFile );
                if ( iRead == GetSlotLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetSlotLength ( tombraider ) );
                    bResult = TRUE;
                }
                free ( pSrcBuffer );
                break;
            }
            case 5:
            {
                char    *pSrcBuffer = ( char *) malloc ( GetSlotLength ( tombraider ) + 1 );
                char    *pObjBuffer = ( char *) GetBlockSlot ( tombraider, block );
                size_t iRead = fread ( pSrcBuffer, 1, GetSlotLength ( tombraider ) + 1, hFile );
                if ( iRead == GetSlotLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetSlotLength ( tombraider ) );
                    bResult = TRUE;
                }
                free ( pSrcBuffer );
                break;
            }
            case 6:
            {
                char    *pSrcBuffer = ( char *) malloc ( GetSlotLength ( tombraider ) + 1 );
                char    *pObjBuffer = ( char *) GetBlockSlot ( tombraider, block );
                int iRead = (int) fread ( pSrcBuffer, 1, GetSlotLength ( tombraider ) + 1, hFile );
                if ( iRead == GetSlotLength ( tombraider ) )
                {
                    memcpy ( pObjBuffer, pSrcBuffer, GetSlotLength ( tombraider ) );
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
void CTR8SaveGame::TraceBlock ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::TraceGun ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR8SaveGame::GetAirAddress ( int tombraider, int block )
{
    if ( block >= NB_SLOT_456 )
    {
        tombraider  = 2;
        block       -= NB_SLOT_456;
    }

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            if ( pBlock )
            {
                return & pBlock->m_iAir;
            }
            break;
        }

        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            if ( pBlock )
            {
                return & pBlock->m_iAir;
            }
            break;
        }

        case 6:
        {
            break;
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTR8SaveGame::GetAir ( int tombraider, int block )
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
void CTR8SaveGame::SetAir ( int tombraider, int block, WORD iAir )
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
BYTE CTR8SaveGame::GetState ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return 0;
    }

    switch ( tombraider )
    {
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            if ( pBlock )
            {
                return pBlock->m_iLara;
            }
            break;
        }

        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            if ( pBlock )
            {
                return pBlock->m_iLara;
            }
            break;
        }

        case 6:
        {
            break;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetState ( int tombraider, int block, BYTE iState )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            if ( pBlock )
            {
                pBlock->m_iLara = iState;
            }
            break;
        }

        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            if ( pBlock )
            {
                pBlock->m_iLara = iState;
            }
            break;
        }

        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR8SaveGame::GetInterest ( int tombraider, int block )
{
    static char szResponse [ 2048 ];
    ZeroMemory ( szResponse, sizeof(szResponse) );

    //
    if ( block < 0 || block >= NB_SLOT_456 )
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

        pAddress = ( char *) GetBlockStart ( tombraider );
        if ( pAddress )
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Start", RelativeAddress ( pAddress ) );
            pAddress = ( char * ) pAddress + GetBlockLength ( tombraider );
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX\r\n", "Last", RelativeAddress ( pAddress ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Start" );
        }

        pAddress = ( char *) GetSlotAddress ( tombraider, block );
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
                        "%16s: 0x%08lX\r\n", "Other", RelativeAddress ( pAddress ) );
        }
        else
        {
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "%16s: NULL\r\n", "Other" );
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

        pAddress = ( char *) GetGunAddress ( tombraider, block);
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

        TR8_POSITION *pDwAddress = ( TR8_POSITION *) GetPositionAddress ( tombraider, block );
        if ( pDwAddress )
        {
            double dfOrientation = (double)pDwAddress->cOrientation;
            dfOrientation       *= 360.0;
            dfOrientation       /= 0x10000L;
            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7.3f°\r\n", "Orientation", RelativeAddress ( & pDwAddress->cOrientation ),
                        pDwAddress->cOrientation, dfOrientation );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Room (Area)", RelativeAddress ( & pDwAddress->cRoom ),
                        pDwAddress->cRoom, pDwAddress->cRoom,
                        CTRXTools::ToBinary ( pDwAddress->cRoom ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wSouthToNorth ),
                        pDwAddress->wSouthToNorth, pDwAddress->wSouthToNorth,
                        CTRXTools::ToBinary ( pDwAddress->wSouthToNorth ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wWestToEast ),
                        pDwAddress->wWestToEast, pDwAddress->wWestToEast,
                        CTRXTools::ToBinary ( pDwAddress->wWestToEast ) );

            sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse),
                        "%16s: 0x%08lX : %04X %7d %s\r\n", "Word", RelativeAddress ( & pDwAddress->wVertical ),
                        pDwAddress->wVertical, pDwAddress->wVertical,
                        CTRXTools::ToBinary ( (WORD) pDwAddress->wVertical ) );
        }

        sprintf_s ( szResponse + strlen(szResponse), sizeof(szResponse) - strlen(szResponse), "\r\n" );
    }

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }
        //
        case 5:
        {
            break;
        }
        //
        case 6:
        {
            break;
        }
    }

    return szResponse;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::BufferModified ()
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
void CTR8SaveGame::discard ()
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
void CTR8SaveGame::SetLikeGame ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetLikePlus ( int tombraider, int block )
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            break;
        }

        //
        case 5:
        {
            break;
        }

        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetPistols ( int tombraider, void *pBlocks, void *pGuns, int level, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnablePistol ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunPistol       |= TRR4_MASK_PISTOL; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iGunAmmos     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunPistol       = pGun->m_gunPistol & ( TRR4_MASK_PISTOL ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iGunAmmos     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGunAmmos = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGunAmmos = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnablePistol ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunPistol       |= TRR5_MASK_PISTOL; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iGunAmmos     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunPistol       = pGun->m_gunPistol & ( TRR5_MASK_PISTOL ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iGunAmmos     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGunAmmos = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGunAmmos = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetPistols ( int tombraider, int block, BOOL bEnabled )
{
    if ( block >= NB_SLOT_456 )
    {
        tombraider  = 2;
        block       -= NB_SLOT_456;
    }

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetPistols ( tombraider, pBlock, pGun, level, bEnabled );
            }

            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetPistols ( tombraider, pBlock, pGun, level, bEnabled );
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetMagnum ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableMagnum ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunRevolver         |= TRR4_MASK_MAGNUM; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iRevolverAmmos    = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunRevolver         = pGun->m_gunRevolver & ( TRR4_MASK_MAGNUM ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iRevolverAmmos    = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iRevolverAmmos    = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iRevolverAmmos    = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableMagnum ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunRevolver         |= TRR5_MASK_REVOLVER; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iRevolverAmmos    = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunRevolver         = pGun->m_gunRevolver & ( TRR5_MASK_REVOLVER ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iRevolverAmmos    = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iRevolverAmmos    = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iRevolverAmmos    = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetMagnum ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetMagnum ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }

            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetMagnum ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetUzi ( int tombraider, void *pBlocks, void *pGuns, int level,
                            BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableUzi ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunUzis         |= TRR4_MASK_UZI; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iUziAmmos     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunUzis         = pGun->m_gunUzis & ( TRR4_MASK_UZI ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iUziAmmos     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iUziAmmos = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iUziAmmos = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableUzi ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunUzis         |= TRR5_MASK_UZI; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iUziAmmos     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunUzis         = pGun->m_gunUzis & ( TRR5_MASK_UZI ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iUziAmmos     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iUziAmmos = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iUziAmmos = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetUzi ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
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
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetUzi ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetUzi ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetRiotgun ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRiotgun ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunRiotGun          |= TRR4_MASK_RIOTGUN; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iShotGunAmmo1     = ammos;
                    pBlock->m_iShotGunAmmo2     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunRiotGun          = pGun->m_gunRiotGun & ( TRR4_MASK_RIOTGUN ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iShotGunAmmo1     = 0; 
                    pBlock->m_iShotGunAmmo2     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iShotGunAmmo1 = 0; 
                        pBlock->m_iShotGunAmmo2 = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iShotGunAmmo1 = ammos;
                        pBlock->m_iShotGunAmmo2 = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableRiotgun ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunRiotGun          |= TRR5_MASK_RIOTGUN; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iShotGunAmmo1     = ammos;
                    pBlock->m_iShotGunAmmo2     = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunRiotGun          = pGun->m_gunRiotGun & ( TRR5_MASK_RIOTGUN ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iShotGunAmmo1     = 0; 
                    pBlock->m_iShotGunAmmo2     = 0; 
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iShotGunAmmo1 = 0; 
                        pBlock->m_iShotGunAmmo2 = 0; 
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iShotGunAmmo1 = ammos;
                        pBlock->m_iShotGunAmmo2 = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetRiotgun ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRiotgun ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetRiotgun ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetHarpoon ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableHarpoon ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunCrossBow         |= TRR4_MASK_CROSSBOW; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iCrossbow1        = ammos;
                    pBlock->m_iCrossbow2        = ammos;
                    pBlock->m_iCrossbow3        = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunCrossBow         = pGun->m_gunCrossBow & ( TRR4_MASK_CROSSBOW ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iCrossbow1        = 0;
                    pBlock->m_iCrossbow2        = 0;
                    pBlock->m_iCrossbow3        = 0;
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iCrossbow1        = 0;
                        pBlock->m_iCrossbow2        = 0;
                        pBlock->m_iCrossbow3        = 0;
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iCrossbow1        = ammos;
                        pBlock->m_iCrossbow2        = ammos;
                        pBlock->m_iCrossbow3        = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetHarpoon ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {

            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetHarpoon ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 5:
        {
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetM16 ( int tombraider, void *pBlocks, void *pGuns, int level,
                            BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {

            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableM16 ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunHK               |= TRR5_MASK_HK; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iHKAmmos          = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunHK               = pGun->m_gunHK & ( TRR5_MASK_HK ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iHKAmmos          = 0;
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iHKAmmos      = 0;
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iHKAmmos      = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetM16 ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {

            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetM16 ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }

            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetGrenade ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) pBlocks;
            GUN_TR4 *pGun       = ( GUN_TR4 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableGrenade ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunGrenadesLauncher |= TRR4_MASK_GRENADE; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iGrenade1         = ammos;
                    pBlock->m_iGrenade2         = ammos;
                    pBlock->m_iGrenade3         = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunGrenadesLauncher = pGun->m_gunGrenadesLauncher & ( TRR4_MASK_GRENADE ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iGrenade1         = 0;
                    pBlock->m_iGrenade2         = 0;
                    pBlock->m_iGrenade3         = 0;
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGrenade1         = 0;
                        pBlock->m_iGrenade2         = 0;
                        pBlock->m_iGrenade3         = 0;
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                    pBlock->m_iGrenade1         = ammos;
                    pBlock->m_iGrenade2         = ammos;
                    pBlock->m_iGrenade3         = ammos;
                    }
                }
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) pBlocks;
            GUN_TR5 *pGun       = ( GUN_TR5 * ) pGuns;
            BOOL bAuthorized = CTRXTools::IsEnableHarpoon ( tombraider, level );
            if ( bAuthorized && bEnabled )
            {
                if ( pGun )
                {
                    pGun->m_gunGrappling        |= TRR5_MASK_GRAP; // Magnum
                }

                if ( pBlock )
                {
                    pBlock->m_iGrapplingAmmos   = ammos;
                }
            }
            else
            {
                if ( pGun )
                {
                    pGun->m_gunGrappling        = pGun->m_gunGrappling & ( TRR5_MASK_GRAP ^ 0xff );
                }

                if ( pBlock )
                {
                    pBlock->m_iGrapplingAmmos   = 0;
                }


                if ( bZeroWhenDisabled )
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGrapplingAmmos   = 0;
                    }

                }
                else
                {
                    if ( pBlock )
                    {
                        pBlock->m_iGrapplingAmmos   = ammos;
                    }
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetGrenade ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {
            TABLE_TR4 *pBlock   = ( TABLE_TR4 *) m_TR4_Blocks [ block ];
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetGrenade ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 5:
        {
            TABLE_TR5 *pBlock   = ( TABLE_TR5 *) m_TR5_Blocks [ block ];
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pBlock != NULL && pGun != NULL )
            {
                SetGrenade ( tombraider, pBlock, pGun, level, bEnabled, ammos, bZeroWhenDisabled );
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetRocket ( int tombraider, void *pBlocks, void *pGuns, int level,
                                BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{
    //
    switch ( tombraider )
    {
        //
        case 4:
        {

            break;
        }
        //
        case 5:
        {
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetRocket ( int tombraider, int block, BOOL bEnabled, WORD ammos, bool bZeroWhenDisabled )
{

    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    //
    int level       = GetBlockLevelNumber ( tombraider, block );

    //
    switch ( tombraider )
    {
        //
        case 4:
        {

            break;
        }
        //
        case 5:
        {
            break;
        }
        //
        case 6:
        {
            break;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  { FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE }
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::ReadIndicators(TRR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::WriteIndicators(TRR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    return FALSE;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR8SaveGame::CountIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count = 0;
    do
    {
        TRR45_INDICATORS indicator = IndicatorsTRTable [ count ];
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
int CTR8SaveGame::MinIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int minimum     = -1;
    do
    {
        TRR45_INDICATORS indicator = IndicatorsTRTable [ count ];
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
int CTR8SaveGame::MaxIndicators( TRR45_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int maximum     = -1;
    do
    {
        TRR45_INDICATORS indicator = IndicatorsTRTable [ count ];
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

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const TR_POSITION *CTR8SaveGame::GetPosition ( int tombraider, int block )
{
    static TR_POSITION localTRPosition;
    ZeroMemory ( &localTRPosition, sizeof(localTRPosition)  );

    const TR8_POSITION *pTR8Position = GetPositionAddress ( tombraider, block );
    if ( pTR8Position != NULL )
    {
        localTRPosition.dwSouthToNorth      = pTR8Position->wSouthToNorth * TR4_FACTOR;
        localTRPosition.dwVertical          = pTR8Position->wVertical * TR4_FACTOR;
        localTRPosition.dwWestToEast        = pTR8Position->wWestToEast * TR4_FACTOR;
        localTRPosition.wOrientation        = pTR8Position->cOrientation << 8;
        localTRPosition.wRoom               = pTR8Position->cRoom;

        return &localTRPosition;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR8SaveGame::SetPosition (    int tombraider, int block,
                                    DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
    //
    //  Check If position was good
    TR8_POSITION *pTR8Position = GetPositionAddress ( tombraider, block );
    if ( pTR8Position != NULL )
    {
        int levelIndex = GetBlockLevelNumber(tombraider, block) - 1;
        BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex, wRoom, dwWestToEast, dwVertical , dwSouthToNorth);
        if ( ! bCheck )
        {
            return FALSE;
        }

        if ( pTR8Position != NULL )
        {
            if ( abs ( pTR8Position->wSouthToNorth - (WORD) ( dwSouthToNorth / TR4_FACTOR ) ) > 2 )
            {
                pTR8Position->wSouthToNorth = (WORD) ( dwSouthToNorth / TR4_FACTOR );
            }

            if ( abs ( pTR8Position->wVertical - (WORD) ( dwVertical / TR4_FACTOR ) ) > 2 )
            {
                pTR8Position->wVertical     = (WORD) ( dwVertical / TR4_FACTOR );
            }

            if ( abs ( pTR8Position->wWestToEast - (WORD) ( dwWestToEast / TR4_FACTOR ) ) > 2 )
            {
                pTR8Position->wWestToEast   = (WORD) ( dwWestToEast / TR4_FACTOR );
            }

            if ( abs ( pTR8Position->cOrientation - CTRXTools::MakeOrientationToOneByte ( wDirection ) ) > 2 )
            {
                pTR8Position->cOrientation  = CTRXTools::MakeOrientationToOneByte ( wDirection );
            }

            pTR8Position->cRoom         = (BYTE) wRoom;

            return TRUE;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetBinocular ( int tombraider, int block, BYTE value )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunBinocular    = TRR4_MASK_LASER;
                }
                else
                {
                    pGun->m_gunBinocular    = pGun->m_gunBinocular & ( TRR4_MASK_LASER ^ 0xff );
                }
            }
            break;
        }
        //
        case 5:
        {
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunBinocular    = TRR5_MASK_LASER;
                }
                else
                {
                    pGun->m_gunBinocular    = pGun->m_gunBinocular & ( TRR5_MASK_LASER ^ 0xff );
                }
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetLaser ( int tombraider, int block, BYTE value )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunLaserLight    = TRR4_GUN_SET1;
                }
                else
                {
                    pGun->m_gunLaserLight    = pGun->m_gunLaserLight & ( TRR4_GUN_SET1 ^ 0xff );
                }
            }
            break;
        }
        //
        case 5:
        {
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunLaserLight    = TRR5_GUN_SET1;
                }
                else
                {
                    pGun->m_gunLaserLight    = pGun->m_gunLaserLight & ( TRR5_GUN_SET1 ^ 0xff );
                }
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetCrowbar ( int tombraider, int block, BYTE value )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            GUN_TR4 *pGun       = ( GUN_TR4 * ) m_TR4_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunCrowBar    = TRR4_GUN_SET1;
                }
                else
                {
                    pGun->m_gunCrowBar    = pGun->m_gunCrowBar & ( TRR4_GUN_SET1 ^ 0xff );
                }
            }
            break;
        }
        //
        case 5:
        {
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunCrowBar    = TRR5_GUN_SET1;
                }
                else
                {
                    pGun->m_gunCrowBar    = pGun->m_gunCrowBar & ( TRR5_GUN_SET1 ^ 0xff );
                }
            }
            break;
        }

        //
        case 6:
        {
            break;
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR8SaveGame::SetHeadSet ( int tombraider, int block, BYTE value )
{
    if ( block < 0 || block >= NB_SLOT_456 )
    {
        return;
    }

    switch ( tombraider )
    {
        //
        case 4:
        {
            break;
        }
        //
        case 5:
        {
            GUN_TR5 *pGun       = ( GUN_TR5 * ) m_TR5_Guns [ block ];
            if ( pGun )
            {
                if ( value )
                {
                    pGun->m_gunHeadSet    = TRR5_GUN_SET1;
                }
                else
                {
                    pGun->m_gunHeadSet    = pGun->m_gunHeadSet & ( TRR5_GUN_SET1 ^ 0xff );
                }
            }
            break;
        }
        //
        case 6:
        {
            break;
        }
    }

    return;
}

