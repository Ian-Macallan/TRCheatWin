// TR4SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR4SaveGame.h"
#include "TRXTools.h"
#include "TR_Areas.h"
#include "TRXGlobal.h"
#include "GunGrids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTRXCHEATWINApp theApp;

/*
 *      ------------------------------------------------
 *      Data.
 *      ------------------------------------------------
 */
static const int Many32K            = 0x7fff;

static char    TR4NBSecrets [ ] =
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
TR45_INDICATORS IndicatorsTR4Table [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE,   0,  "Standing", },
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE,   0,  "Standing", },

    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   TRUE,   1,  "", },
    {   FALSE,  0x02,   0x02,   0x00,   0x0c,   TRUE,   1,  "", },
    {   FALSE,  0x02,   0x02,   0x00,   0x1f,   TRUE,   1,  "", },
    {   FALSE,  0x02,   0x02,   0x00,   0xbd,   TRUE,   1,  "", },
    {   FALSE,  0x02,   0x02,   0x00,   0xdd,   TRUE,   1,  "", },

    {   FALSE,  0x0d,   0x0d,   0x00,   0x6c,   TRUE,   1,  "Swimming", },
    {   FALSE,  0x0d,   0x0d,   0x47,   0x6c,   TRUE,   1,  "Swimming", },

    {   FALSE,  0x0f,   0x0f,   0x00,   0x1f,   TRUE,   1,  "Quad", },
    {   FALSE,  0x12,   0x00,   0x00,   0x02,   TRUE,   1,  "Flare", },

    {   FALSE,  0x10,   0x00,   0x51,   0x51,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x21,   0x21,   0x00,   0x6e,   TRUE,   1,  "In Water", },
    {   FALSE,  0x47,   0x47,   0x47,   0xde,   TRUE,   1,  "Kneeling", },
    {   FALSE,  0x57,   0x57,   0x47,   0x1f,   TRUE,   1,  "", },

    //
    {   FALSE,  0x61,   0x61,   0x00,   0x44,   TRUE,   1,  "", },
    {   FALSE,  0x75,   0x10,   0x47,   0x9c,   TRUE,   1,  "", },
    {   FALSE,  0x21,   0x21,   0x47,   0x6e,   TRUE,   1,  "", },
    {   FALSE,  0x50,   0x50,   0x00,   0x07,   TRUE,   1,  "", },
    {   FALSE,  0x24,   0x24,   0x00,   0x7b,   TRUE,   1,  "", },
    {   FALSE,  0x24,   0x24,   0x47,   0x7b,   TRUE,   1,  "", },

    {   FALSE,  0x27,   0x10,   0x00,   0xa3,   TRUE,   1,  "", },
    {   FALSE,  0x27,   0x15,   0x00,   0xa3,   TRUE,   1,  "", },
    {   FALSE,  0x0f,   0x0f,   0x47,   0x1d,   TRUE,   1,  "Bike", },
    {   FALSE,  0x0f,   0x0f,   0x00,   0x1d,   TRUE,   1,  "Bike", },
    {   FALSE,  0x00,   0x00,   0x47,   0x23,   TRUE,   1,  "Jeep", },
    {   FALSE,  0x02,   0x02,   0x47,   0xdd,   TRUE,   1,  "", }, 
    {   FALSE,  0x02,   0x02,   0x47,   0x0b,   TRUE,   1,  "", }, 
    {   FALSE,  0x02,   0x02,   0x47,   0x1f,   TRUE,   1,  "Climbing", },

    {   FALSE,  0x13,   0x13,   0x47,   0x61,   TRUE,   1,  "", }, 
    {   FALSE,  0x13,   0x13,   0x00,   0x61,   TRUE,   1,  "", }, 

    {   FALSE,  0x19,   0x19,   0x00,   0x4b,   TRUE,   1,  "", }, 

    {   FALSE,  0x01,   0x02,   0x47,   0x08,   TRUE,   1,  "", }, 

    //
    {   TRUE,   0xff,   0xff,   0xff,   0xff,   TRUE,   0,  "End", },
};
int IndicatorsTR4TableCount = sizeof(IndicatorsTR4Table)/sizeof(TR45_INDICATORS);

//
static int positionCount = 0;
static TR4_POSITION *positionTable [ MAX_POSITION ];

//
static TR_POSITION_RANGE TR4IndicatorRange [ TR4_LEVELS ] =
{
    {   0x340,      0x360   },      /* 1 Angkor Wat */ 
    {   0x280,      0x3000  },      /* 2 Race for the Iris */
    {   0x3e0,      0x440   },      /* 3 Tomb of Seth */
    {   0x3b0,      0x600   },      /* 4 Burial Chambers */
    {   0x2f0,      0x340   },      /* 5 Valley of the Kings */
    {   0x4b0,      0x630   },      /* 6 KV5 */
    {   0x310,      0x720   },      /* 7 Temple of Karnak */
    {   0x620,      0x950   },      /* 8 Great Hypostyle Hall */
    {   0x7e0,      0x980   },      /* 9 Sacred Lake */
    {   0x280,      0x3000  },      /* 10 Nothing */ 
    {   0xd00,      0xfff   },      /* 11 Tomb of Semerkhet */
    {   0x380,      0x4ff   },      /* 12 Guardian of Semerkhet */
    {   0x340,      0x39f   },      /* 13 Desert Railroad */
    {   0x2e0,      0x2ff   },      /* 14 Alexandria */
    {   0x4f0,      0x1400  },      /* 15 Coastal Ruins */
    {   0x1500,     0x18ff  },      /* 16 Pharos, Temple of Isis */
    {   0x1900,     0x19ff  },      /* 17 Cleopatra's Palaces */
    {   0x780,      0x7ff   },      /* 18 Catacombs */
    {   0xc60,      0xdff   },      /* 19 Temple of Poseidon */
    {   0x280,      0x3000  },      /* 20 The Lost Library */
    {   0x1400,     0x14ff  },      /* 21 Hall of Demetrius */
    {   0x310,      0x3000  },      /* 22 City of the Dead */
    {   0xc00,      0xfff   },      /* 23 Trenches */
    {   0x650,      0xdff   },      /* 24 Chambers of Tulun */
    {   0x280,      0x3000  },      /* 25 Street Bazaar */ 
    {   0x900,      0xfff   },      /* 26 Citadel Gate */
    {   0x400,      0x6ff   },      /* 27 Citadel */
    {   0x300,      0x1200  },      /* 28 Sphinx Complex */
    {   0x280,      0x3000  },      /* 29 Nothing */
    {   0x500,      0x5ff   },      /* 30 Underneath the Sphinx */
    {   0xb00,      0xcff   },      /* 31 Menkaure's Pyramid */
    {   0xe00,      0xeff   },      /* 32 Inside Menkaure's Pyramid */
    {   0x1400,     0x16ff  },      /* 33 The Mastabas */
    {   0x280,      0x3000  },      /* 34 The Great Pyramid */ 
    {   0x1b00,     0x1bff  },      /* 35 Khufu's Queen's Pyramids */
    {   0x1e00,     0x1eff  },      /* 36 Inside the Great Pyramid */
    {   0x2200,     0x28ff  },      /* 37 Temple of Horus */
    {   0x2200,     0x28ff  },      /* 38 Temple of Horus */ 
    {   0x280,      0x3000  },      /* 39 Office */
    {   0x500,      0x5ff   },      /* 40 Times Exclusive */
};

//
/////////////////////////////////////////////////////////////////////////////
// CTR4SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR4SaveGame, CTR45SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR4SaveGame::CTR4SaveGame()
{

    m_iVersion          = 40;

    m_iSaveLength       = TR4LEVELMINSIZE;
    m_iMaxLevel         = TR4MAXLEVEL;

    iMaskGun            = TR40_GUN_SET1 | TR40_GUN_SET8;        //  Pistol
    iMaskUzi            = TR40_GUN_SET1;
    iMaskRiotGun        = TR40_GUN_SET1 | TR40_GUN_SET8;
    iMaskCrossBow       = TR40_GUN_SET1 | TR40_GUN_SET8;        // Crossbow
    iMaskGrenade        = TR40_GUN_SET1 | TR40_GUN_SET8;
    iMaskRevolver       = TR40_GUN_SET1;                        // Revolver
    iMaskLaser          = TR40_GUN_SET1;
    iMaskBinocular      = TR40_GUN_SET1;
    iMaskCrowBar        = TR40_GUN_SET1;

    iRiotGunUnits       = 6;

    m_pLife             = NULL;

    m_pBuffer           = new ( TR4SAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR4SAVE) );

    m_pBufferBackup     = new ( TR4SAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR4SAVE) );

    m_iSubVersion       = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR4SaveGame::~CTR4SaveGame()
{
    if ( m_pBuffer )
    {
        delete m_pBuffer;
        m_pBuffer = NULL;
    }

    if ( m_pBufferBackup )
    {
        delete m_pBufferBackup;
        m_pBufferBackup = NULL;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
// CTR4SaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR4SaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::ReadSavegame ( const char *pFilename )
{

        FILE                    *hFile;
        size_t                uLenBuffer;

        char                    szEmpty [ 1 ];

        strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
        strcpy_s ( m_Status, sizeof(m_Status), "" );

        memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4SAVE ) );

        /*
         *      Read file.
         */
        hFile = NULL;
        fopen_s ( &hFile,  m_Filename, "rb" );
        if ( hFile == NULL )
        {
            strcpy_s ( m_Status, sizeof(m_Status), "Unable to read file." );
            return 0;
        }

        /*
         *      Get Buffer.
         */
        if ( m_iSaveLength < TR4LEVELMINSIZE || m_iSaveLength > TR4LEVELMAXSIZE )
        {
            sprintf_s ( m_Status, sizeof(m_Status), "Internal error in length %d versus %d = %d.",
                (int) sizeof ( TR4SAVE ), m_iSaveLength,
                m_iSaveLength - (int) sizeof ( TR4SAVE ) );
            fclose ( hFile );
            return 0;
        }

        memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4SAVE ) );
        uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
        if ( uLenBuffer != m_iSaveLength )
        {
            strcpy_s ( m_Status, sizeof(m_Status), "File size is not correct." );
            fclose ( hFile );
            return 0;
        }

        long lPos = ftell ( hFile );
        if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
        {
            fseek ( hFile, 0, SEEK_END );
            long lEnd = ftell ( hFile );
            sprintf_s ( m_Status, sizeof(m_Status), "File size is too large %ld til %ld = %ld.", lPos, lEnd, lEnd - lPos );
            fclose ( hFile );
            return 0;
        }

        memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR4SAVE) );

        /*
         *      Close file.
         */
        fclose ( hFile );

        return 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::writeSaveGame()
{
    FILE                    *hFile;
    size_t                  uLenBuffer;

    strcpy_s ( m_Status, sizeof(m_Status), "" );

    /*
     *  Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( ! ( m_pBuffer->tagGuns.m_gunRevolver & iMaskRevolver ) )
    {
        m_pBuffer->tagGuns.m_gunRevolver  = 0 ;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunUzis & iMaskUzi ) )
    {
        m_pBuffer->tagGuns.m_gunUzis = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunRiotGun & iMaskRiotGun ) )
    {
        m_pBuffer->tagGuns.m_gunRiotGun = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & iMaskGrenade ) )
    {
        m_pBuffer->tagGuns.m_gunGrenadesLauncher = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunCrossBow & iMaskCrossBow ) )
    {
        m_pBuffer->tagGuns.m_gunCrossBow = 0;
    }


    //  Compute CheckSum
    unsigned char *pBackup = (unsigned char *)m_pBufferBackup;
    unsigned char *pBuffer = (unsigned char *)m_pBuffer;

    unsigned checkSum = m_pBufferBackup->checkSum;
    for ( int i = 0; i < sizeof(TR4SAVE) - 1; i++ )
    {
        if ( pBackup [ i ] != pBuffer [ i ] )
        {
            unsigned delta = pBuffer [ i ] - pBackup [ i ];
            checkSum -= delta;
        }
    }

    //
    checkSum = checkSum & 0xff;
    m_pBuffer->checkSum = checkSum;

    //
    Backup_Savegame();

    /*
     *      Write file.
     */
    hFile = NULL;
    fopen_s ( &hFile,  m_Filename, "wb" );
    if ( hFile == NULL )
    {
        strcpy_s ( m_Status, sizeof(m_Status), "Unable to write the file." );
        return;
    }

    /*
     *      Get Buffer.
     */
    if ( m_iSaveLength < TR4LEVELMINSIZE || m_iSaveLength > TR4LEVELMAXSIZE )
    {
        sprintf_s ( m_Status, sizeof(m_Status), "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR4SAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR4SAVE ) );
        fclose ( hFile );
        return;
    }

    uLenBuffer = fwrite ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        strcpy_s ( m_Status, sizeof(m_Status), "File size is not correct." );
        fclose ( hFile );
        return;
    }

    /*
     *      Close file.
     */
    fclose ( hFile );

    //
    memcpy ( m_pBufferBackup,  m_pBuffer, m_iSaveLength );

    strcpy_s ( m_Status, sizeof(m_Status), "File successfully updated." );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::RetrieveInformation( const char *pFilename )
{
    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );

        if ( strstr ( (char *) m_pBuffer, "Times" ) != NULL )
        {
            m_iSubVersion   = 5;
        }
        else
        {
            m_iSubVersion   = 0;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR4GUN *CTR4SaveGame::SearchGunStructure ( unsigned short m_iHealth, int *iPos )
{
        return ( &m_pBuffer->tagGuns );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckIfAmmosMatch ( TR4GUN *pGun, WORD gunBitmap )
{
    int             iX;

    iX = getLevelIndex ();

    return Valid();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::RetrieveHeader()
{
        /*
         *      Objects mask.
         */
//      m_pBuffer->cObjectMask = 0xff;

        /*
         *      Infinite ammos.
         */
        GetAmmosValues ( );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::GetAmmosValues()
{
    int     iX;

    /*
     *      Get gun state.
     */
    iX = getLevelIndex ();

    /*
     *      Get current values for Guns.
     */
    m_iDesertEagle      = m_pBuffer->tagGuns.m_gunRevolver;
    m_iRiotGun          = m_pBuffer->tagGuns.m_gunRiotGun;
    m_iUzis             = m_pBuffer->tagGuns.m_gunUzis;
    m_iGrenades         = m_pBuffer->tagGuns.m_gunGrenadesLauncher;
    m_iHarpoon          = m_pBuffer->tagGuns.m_gunCrossBow;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::ConvertSecret(int iSecret)
{
    int     iResult;

    iResult = 0;

    while ( iSecret )
    {
        if ( ( iSecret & 1 ) != 0 )
        {
            iResult++;
        }

        iSecret /= 2;
    }

    return iResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::ConvertSecretBack(int iSecret)
{
    int     iResult;

    iResult = 0;

    while ( iSecret )
    {
        iResult *= 2;
        iResult += 1;
        iSecret--;
    }

    return iResult;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::GetDetailedInfo (    char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR4 Last Revelation" );
    strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
    *iGame  = m_pBuffer->iSaveNumber;
    *iLevel = getLevel();
    if ( strstr ( (char*) m_pBuffer, "Times" ) != NULL )
    {
        strcpy_s ( szGame, iSize, "TR4 Times Exclusive" );
        m_iSubVersion   = 5;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::getLevel()
{
    int iLevel = 0;
    for ( int i = 0; i < sizeof(m_pBuffer->m_cLevels); i++ )
    {
        if ( m_pBuffer->m_cLevels [ i ] < iLevel )
        {
            // break;
        }
        if ( m_pBuffer->m_cLevels [ i ] != 0 )
        {
            iLevel = m_pBuffer->m_cLevels [ i ];
        }
    }
    return iLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTR4SaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::getLevelIndex()
{
    return getLevel() - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos1(int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGunAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos2(int iX )
{
    return ( m_pBuffer->tagAmmo.m_iRevolverAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos3( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iUziAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos4a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iShotGunAmmo1 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos4b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iShotGunAmmo2 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos5 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos6( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->tagGuns.m_iRockets );
    }

//  return ( m_pGun->m_iRockets );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos7a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos7b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade2 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos7c( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade3 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos8a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos8b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAmmos8c( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos1 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGunAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos2 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iRevolverAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos3 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iUziAmmos = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos4a ( const char *szString, int iX )
{
    if ( atoi ( szString ) < 0 )
    {
        m_pBuffer->tagAmmo.m_iShotGunAmmo1 = -1;
    }
    else
    {
        m_pBuffer->tagAmmo.m_iShotGunAmmo1 = atoi ( szString ) * iRiotGunUnits;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos4b ( const char *szString, int iX )
{
    if ( atoi ( szString ) < 0 )
    {
        m_pBuffer->tagAmmo.m_iShotGunAmmo2 = -1;
    }
    else
    {
        m_pBuffer->tagAmmo.m_iShotGunAmmo2 = atoi ( szString ) * iRiotGunUnits;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos5 ( const char *szString, int iX )
{
    //m_pBuffer->tagAmmo.m_iUnknown = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos6 ( const char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos7a ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos7b ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos7c ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos8a ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos8b ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAmmos8c ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetUnlimitedAmmos(int iMask)
{
    // m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetUnlimitedAmmos()
{
    //return ( m_pBuffer->cUnlimited );
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::Valid()
{
    if ( m_pBuffer->tagGuns.m_gunRevolver != 0 && ( m_pBuffer->tagGuns.m_gunRevolver & TR40_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunRevolver & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunRiotGun != 0 && ( m_pBuffer->tagGuns.m_gunRiotGun & TR40_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunRiotGun & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunUzis != 0 && ( m_pBuffer->tagGuns.m_gunUzis & TR40_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunUzis & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunGrenadesLauncher != 0 && ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & TR40_GUN_MASK ) == 0  &&
        ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunCrossBow != 0 && ( m_pBuffer->tagGuns.m_gunCrossBow & TR40_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunCrossBow & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunCrowBar != 0 && ( m_pBuffer->tagGuns.m_gunCrowBar & TR40_GUN_MASK ) == 0  &&
        ( m_pBuffer->tagGuns.m_gunCrowBar & TR40_GUN_SET4 ) == 0 )
    {
        return 0;
    }
    return ( 1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetInvalid()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon1 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunPistol & iMaskGun )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon4 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunRiotGun & iMaskRiotGun )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon2 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunRevolver & iMaskRevolver )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon3 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunUzis & iMaskUzi )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon5 ( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon8 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunCrossBow & iMaskCrossBow )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon7 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & iMaskGrenade )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon6 ( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Headset
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::CheckWeapon9 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunCrowBar & iMaskCrowBar )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    // m_pBuffer->tagGuns.cObjects = iMaskCompass;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunPistol;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunPistol &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunPistol |= iMaskGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunPistol &= ( iMaskGun ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRiotGun;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRiotGun &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRiotGun |= iMaskRiotGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRiotGun &= ( iMaskRiotGun ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRevolver;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRevolver &= ( TR40_GUN_SET1 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRevolver |= iMaskRevolver;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRevolver &= ( iMaskRevolver ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunUzis;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunUzis &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunUzis |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunUzis &= ( iMaskUzi ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{

    // m_pBuffer->tagGuns.cObjects |= iMaskMP5;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    // m_pBuffer->tagGuns.cObjects |= iMaskRocket;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunGrenadesLauncher;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunGrenadesLauncher &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunGrenadesLauncher |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunGrenadesLauncher &= ( iMaskGrenade ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunCrossBow;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunCrossBow &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunCrossBow |= iMaskCrossBow;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunCrossBow &= ( iMaskCrossBow ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4SaveGame::GrabWeapon9 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunCrowBar;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunCrowBar &= ( TR40_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunCrowBar |= iMaskCrowBar;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunCrowBar &= ( iMaskCrowBar ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iSmallMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iLargeMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetFlares ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iFlares );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetLaser ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunLaserLight );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetBinocular ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunBinocular );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetAir ( )
{
    return ( m_pBuffer->iAir );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iSmallMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLargeMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iLargeMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetFlares ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iFlares = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLaser ( char *szString, int iX )
{
    m_pBuffer->tagGuns.m_gunLaserLight = atoi (  szString );
    if ( m_pBuffer->tagGuns.m_gunLaserLight != 0 )
    {
        m_pBuffer->tagGuns.m_gunLaserLight = iMaskLaser;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetBinocular ( char *szString, int iX )
{
    m_pBuffer->tagGuns.m_gunBinocular = atoi (  szString );
    if ( m_pBuffer->tagGuns.m_gunBinocular != 0 )
    {
        m_pBuffer->tagGuns.m_gunBinocular = iMaskBinocular;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAir ( const char *szString )
{
    m_pBuffer->iAir = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4SaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetCurrentSecrets ( char *szString, int iX )
{
    //  No Change
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAllSecrets ( )
{
    int     iX;
    for ( iX = 0; iX < m_iMaxLevel; iX++ )
    {
        // m_pBuffer->tagGuns.cSecrets = 7; /* Always 3 secrets */
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::IsLaraBurning ()
{
    if ( ( m_pBuffer->laraState & 0x0c ) == 0x0c )
    {
        return 1;
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLaraBurning (int state)
{
    if ( state != -1 )
    {
        m_pBuffer->laraState = 0x0c;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLaraNoBurning (int state)
{
    if ( state != -1 )
    {
        m_pBuffer->laraState = 0x04;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetMap1 ()
{
    return ( 0 /* m_pBuffer->cObject_P1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetMap2 ()
{
    return ( 0 /*m_pBuffer->cObject_P2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetItem1 ()
{
    return ( 0 /*m_pBuffer->cObjectP1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetItem2 ()
{
    return ( 0 /*m_pBuffer->cObjectP2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetItem3 ()
{
    return ( 0 /*m_pBuffer->cObjectP3 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetItem4 ()
{
    return (0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetKey1 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetKey2 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetKey3 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetKey4 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetTreasure1 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetTreasure2 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetTreasure3 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetTreasure4 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetMap1 ( char *szString )
{
    //m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetMap2 ( char *szString )
{
    //m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetItem1 ( char *szString )
{
    //m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetItem2 ( char *szString )
{
    //m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetItem3 ( char *szString )
{
    //m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetItem4 ( char *szString )
{
    //m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetKey1 ( char *szString )
{
    //m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetKey2 ( char *szString )
{
    //m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetKey3 ( char *szString )
{
    //m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetKey4 ( char *szString )
{
    //m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetTreasure1 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetMaximumSecrets ( int iX )
{
    if ( iX >= 0 && iX < sizeof(TR4NBSecrets) )
    {
        int allSecrets = 0;
        for ( int i = 0; i <= iX; i++ )
        {
            allSecrets += TR4NBSecrets [ i ];
        }
        return allSecrets;
        // return TR4NBSecrets [ iX ];
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetGunAmmos ( )
{
    return GetAmmos1(0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetGunAmmos ( const char *szGunAmmos )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR4SaveGame::GetIndicatorAddress (int index)
{
    //
    int levelIndex  = GetLevelIndex() % ( sizeof(TR4IndicatorRange) / sizeof(TR_POSITION_RANGE) );;
    int minOffset   = TR4IndicatorRange [ levelIndex ].minOffset;
    int maxOffset   = TR4IndicatorRange [ levelIndex ].maxOffset;

    if ( IsCustomArea ( ) || ! CTRXGlobal::m_UseTR4PositionRange )
    {
        minOffset   = MinTR4PositionOffset;
        maxOffset   = MaxTR4PositionOffset;
    }

    //
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    BYTE *pBuffer   = ( BYTE * ) m_pBuffer;
    int count       = 0;
    for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
    {
        //  Compare with Indicators
        for ( int indice = 0; indice < IndicatorsTR4TableCount;  indice++ )
        {
            if ( IndicatorsTR4Table [ indice ].bEnd )
            {
                break;
            }

            //
            //  If not search extended only reliable si if index > 2 break
            if ( ! CTRXGlobal::m_iSearchPosExt && indice >= MaxReliableIndicator )
            {
                break;
            }

            if (    pBuffer [ iBuffer ] == IndicatorsTR4Table [ indice ].b1 &&
                    pBuffer [ iBuffer + 1 ] == IndicatorsTR4Table [ indice ].b2 &&
                    pBuffer [ iBuffer + 3 ] == IndicatorsTR4Table [ indice ].b4 )
            {
                if ( IndicatorsTR4Table [ indice ].useB3 && pBuffer [ iBuffer + 2 ] != IndicatorsTR4Table [ indice ].b3 )
                {
                    continue;
                }

                // In TR4 Life is between 0 and 999 (0 means 1000)
                short life = * (short * ) ( pBuffer + iBuffer + TR4_LIFE_OFFSET );
                //  Life Is valid between 0 and 1000
                if ( life != TR4_ALT_HEALTH && ( life < TR4_MIN_HEALTH || life > TR4_MAX_HEALTH ) )
                {
                    continue;
                }

                count++;
                if ( count > index )
                {
                    strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTR4Table [ indice ].szLabel );

                    return pBuffer + iBuffer;
                }
            }
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD *CTR4SaveGame::GetTR4LifeAddress()
{
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        WORD *pLife = ( WORD * ) ( pBuffer + TR4_LIFE_OFFSET );

#ifdef _DEBUG
        static char szDebugString [ MAX_PATH ];
        DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer, m_pBuffer );
        sprintf_s ( szDebugString, sizeof(szDebugString), 
            "Life Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x H:%-6d\n", 
            dwRelativeAddress, pBuffer [ 0 ] & 0xff, pBuffer [ 1 ] & 0xff, pBuffer [ 2 ] & 0xff, pBuffer [ 3 ] & 0xff, *pLife );
        OutputDebugString ( szDebugString );
#endif

        //
        //  Verify Position
        for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
        {
            TR4_POSITION *pTR4Position = (TR4_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR4_POSITION_OFFSET ) );
            
            DWORD dwSouthToNorth    = ( DWORD) pTR4Position->wSouthToNorth * TR4_FACTOR;
            DWORD dwVertical        = ( DWORD ) pTR4Position->wVertical * TR4_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pTR4Position->wWestToEast * TR4_FACTOR;
            WORD wRoom              = pTR4Position->cRoom;

            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
            if ( bCheck )
            {
                //
                return pLife;
            }
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//  0000004B: 7F 80
//  00000055: 1A 2D
//  00000092: 08 04
//  00000192: 24 23
//  000001C7: B6 B4
//  000001D9: 72 70
//  000001E8: 27 F6
//  000001E9: 66 96
//  000001EA: F4 F6
//  000001FC: 00 01
//  00000CAF: C0 80
//  00000CBE: 52 71
//  00000CC0: D3 00
//  00000CC1: 03 00
//  00000CC5: 00 90
//  00000CC7: 90 00
//  00000CCC: 02 20
//  00000CCD: 00 3E
//  00000CCE: 20 23
//  00000CCF: 3E 00
//  00000CD0: 23 FF
//  00000CD1: 00 FF
//  00000CD2: FF 00
//  00000CD3: FF 00
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetLife ()
{
    //
    WORD *pLife = GetTR4LifeAddress();
    if ( pLife != NULL )
    {
        WORD life = *pLife;
        if ( life == TR4_MIN_HEALTH )
        {
            life = TR4_MAX_HEALTH;
        }
        return life;
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLife ( const char *szLife )
{
    //
    WORD *pLife = GetTR4LifeAddress();
    if ( pLife != NULL )
    {
        WORD life = (WORD) atoi(szLife);
        if ( life == TR4_MAX_HEALTH )
        {
            life = TR4_MIN_HEALTH;
        }

#ifndef _DEBUG
        life = life % TR4_MAX_HEALTH;
#endif

        *pLife = (WORD) life;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4SaveGame::getBufferAddress ()
{
    return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4SaveGame::getBufferBackupAddress ()
{
    return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR4SaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetCurrentSecrets ()
{
    return ( m_pBuffer->tagAmmo.m_iSecretCount );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4SaveGame::GetLaraState ( )
{
    return m_pBuffer->laraState;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetLaraState ( int state )
{
    m_pBuffer->laraState = state;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::SetItems ( int item, BYTE value )
{
    if ( item >= 0 && item < sizeof(m_pBuffer->tagGuns.m_Object) )
    {
        m_pBuffer->tagGuns.m_Object [ item ] = value;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR4SaveGame::GetItems ( int item )
{
    if ( item >= 0 && item < sizeof(m_pBuffer->tagGuns.m_Object) )
    {
        return m_pBuffer->tagGuns.m_Object [ item ];
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4SaveGame::BufferModified ()
{
    if ( m_pBuffer != NULL && m_pBufferBackup != NULL && m_iSaveLength > 0 )
    {
        if ( CTRXTools::MemoryCompare ( m_pBuffer, m_pBufferBackup, m_iSaveLength ) != 0 )
        {
            return TRUE;
        }
    }
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4SaveGame::discard ()
{
    if ( m_pBuffer != NULL && m_pBufferBackup != NULL && m_iSaveLength > 0 )
    {
        memcpy ( m_pBuffer, m_pBufferBackup, m_iSaveLength );
    }
    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR4_POSITION *CTR4SaveGame::GetTR4Position ( )
{
    ZeroMemory ( positionTable, sizeof(positionTable) );
    positionCount   = 0;

#ifdef _DEBUG
    OutputDebugString ( "GetTR4Position\n" );
#endif

    //  We Search n times
    //  The Goal is to see if there is a =atch with position for an index
    //  For example we could have an indicator but no position
    //  So we will look the next indicator
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    for ( int index = 0; index < IndicatorsTR4TableCount; index++ )
    {
        if ( IndicatorsTR4Table [ index ].bEnd )
        {
            break;
        }

        //
        char *pBuffer = (char * ) GetIndicatorAddress(index);
        if ( pBuffer )
        {
            TR4_POSITION *pTR4Position0 = (TR4_POSITION *) ( ( ( BYTE * ) pBuffer - TR4_POSITION_OFFSET ) );

            for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
            {
                //  We Consider pBuffer - i pointing to indicator1
                TR4_POSITION *pTR4Position = (TR4_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR4_POSITION_OFFSET ) );

                DWORD dwSouthToNorth    = pTR4Position->wSouthToNorth * TR4_FACTOR;
                DWORD dwVertical        = pTR4Position->wVertical * TR4_FACTOR;
                DWORD dwWestToEast      = pTR4Position->wWestToEast * TR4_FACTOR;
                WORD wRoom              = pTR4Position->cRoom;

                int countZero = 0;
                if ( dwSouthToNorth == 0 )
                {
                    countZero++;
                }
                if ( dwVertical == 0 )
                {
                    countZero++;
                }
                if ( dwWestToEast == 0 )
                {
                    countZero++;
                }
                if ( wRoom == 0 )
                {
                    countZero++;
                }

                //  Too Much Zeroes
                if ( countZero >= 3 )
                {
                    continue;
                }

                int tombraider = GetFullVersion();
                int levelIndex = GetLevelIndex();
                DWORD dwExtraVertical   = 0;
                if ( CTRXGlobal::m_iExtendVertical )
                {
                    //  Low part is subracted from top
                    //  High part is added to bottom
                    //  Extending vertical range
                    //  So y >= yTop - low && y <= yBottom + high
                    dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                    dwExtraVertical         = ( 0x0100 << 16 );
                }
                BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex, wRoom, dwWestToEast, dwVertical, dwSouthToNorth, false, dwExtraVertical );
                if ( bCheck )
                {
#ifdef _DEBUG
                    // In TR4 Life is between 0 and 999 (0 means 1000)
                    short life = * (short * ) ( pBuffer + TR4_LIFE_OFFSET );

                    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer - i, m_pBuffer );
                    static char szDebugString [ MAX_PATH ];
                    sprintf_s ( szDebugString, sizeof(szDebugString), 
                        "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x R:%-3u V:%-5d SN:%-5d WE:%-5d D:%-3u H:%-6d\n", 
                        dwRelativeAddress,
                        pTR4Position0->indicator1, pTR4Position0->indicator2, pTR4Position0->indicator3, pTR4Position0->indicator4, 
                        wRoom, dwVertical, dwSouthToNorth, dwWestToEast, pTR4Position->cOrientation,
                        life ); 
                    OutputDebugString ( szDebugString );
#endif
                    positionTable [ 0 ] = pTR4Position;
                    positionCount       = 1;
                    return pTR4Position;
                }
            }
        }
    }

    //
    //  Search Extended
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    if ( CTRXGlobal::m_iSearchPosExt )
    {
        char *pBuffer = ( char * ) m_pBuffer;

        TR4_POSITION *pCurrent      = NULL;
        TR4_POSITION *pTR4Position  = NULL;

        //
        int levelIndex  = GetLevelIndex() % ( sizeof(TR4IndicatorRange) / sizeof(TR_POSITION_RANGE) );;
        int minOffset   = TR4IndicatorRange [ levelIndex ].minOffset;
        int maxOffset   = TR4IndicatorRange [ levelIndex ].maxOffset;

        if ( IsCustomArea ( ) || ! CTRXGlobal::m_UseTR4PositionRange )
        {
            minOffset   = MinTR4PositionOffset;
            maxOffset   = MaxTR4PositionOffset;
        }

        //
        for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
        {
            //  We Consider pBuffer + i pointing to indicator1
            pCurrent                = (TR4_POSITION *) ( ( BYTE * ) pBuffer + iBuffer );

            DWORD dwSouthToNorth    = ( DWORD) pCurrent->wSouthToNorth * TR4_FACTOR;
            DWORD dwVertical        = ( DWORD ) pCurrent->wVertical * TR4_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pCurrent->wWestToEast * TR4_FACTOR;
            WORD wRoom              = pCurrent->cRoom;

            int countZero = 0;
            if ( dwSouthToNorth == 0 )
            {
                countZero++;
            }
            if ( dwVertical == 0 )
            {
                countZero++;
            }
            if ( dwWestToEast == 0 )
            {
                countZero++;
            }
            if ( wRoom == 0 )
            {
                countZero++;
            }

            //  Too Much Zeroes
            if ( countZero >= 2 )
            {
                continue;
            }

            DWORD dwExtraVertical   = 0;
            if ( CTRXGlobal::m_iExtendVertical )
            {
                //  Low part is subracted from top
                //  High part is added to bottom
                //  Extending vertical range
                //  So y >= yTop - low && y <= yBottom + high
                dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                dwExtraVertical         = ( 0x0100 << 16 );
            }

            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth, false, dwExtraVertical );
            if ( bCheck )
            {
#ifdef _DEBUG
                DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pCurrent, m_pBuffer );
                static char szDebugString [ MAX_PATH ];
                sprintf_s ( szDebugString, sizeof(szDebugString), "position 0x%08x : R:%-3u V:%-5d SN:%-5d WE:%-5d D:%-3u\n", 
                    dwRelativeAddress,
                    wRoom, dwVertical, dwSouthToNorth, dwWestToEast, pCurrent->cOrientation ); 
                OutputDebugString ( szDebugString );
#endif
                //
                for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
                {
                    TR4_POSITION *pTR4Position0    = (TR4_POSITION *) ( (char *) pCurrent + i );

                    //  Life between 0 and 1000
                    short life = pTR4Position0->heath;

                    //
                    if ( life >= TR4_MIN_HEALTH && life <= TR4_MAX_HEALTH )
                    {
                        positionTable [ positionCount ] = pCurrent;
                        if ( pTR4Position == NULL )
                        {
                            pTR4Position    = pCurrent;
                        }
                        positionCount++;
                    }

#ifdef _DEBUG
                    dwRelativeAddress = CTRXTools::RelativeAddress ( & pTR4Position0->indicator1, m_pBuffer );
                    sprintf_s ( szDebugString, sizeof(szDebugString), 
                        "- indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x H:%-6d\n", 
                        dwRelativeAddress,
                        pTR4Position0->indicator1, pTR4Position0->indicator2, pTR4Position0->indicator3, pTR4Position0->indicator4,
                        life ); 
                    OutputDebugString ( szDebugString );

                    if ( CTRXGlobal::m_iUnchecked == FALSE )
                    {
                        if ( positionCount > MAX_POSITION )
                        {
                            return NULL;
                        }
                    }
#else
                    if ( CTRXGlobal::m_iUnchecked == FALSE )
                    {
                        if ( positionCount > 2 )
                        {
                            return NULL;
                        }
                    }
#endif
                }
            }
        }

        if ( CTRXGlobal::m_iUnchecked == FALSE )
        {
            if ( positionCount > 1 )
            {
                return NULL;
            }
        }

        return pTR4Position;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const TR_POSITION *CTR4SaveGame::GetPosition ( )
{
    static TR_POSITION localTRPosition;
    ZeroMemory ( &localTRPosition, sizeof(localTRPosition)  );

    const TR4_POSITION *pTR4Position = GetTR4Position ( );
    if ( pTR4Position != NULL )
    {
        localTRPosition.dwSouthToNorth      = pTR4Position->wSouthToNorth * TR4_FACTOR;
        localTRPosition.dwVertical          = pTR4Position->wVertical * TR4_FACTOR;
        localTRPosition.dwWestToEast        = pTR4Position->wWestToEast * TR4_FACTOR;
        localTRPosition.wOrientation        = pTR4Position->cOrientation << 8;
        localTRPosition.wRoom               = pTR4Position->cRoom;

        return &localTRPosition;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4SaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
    //
    //  Check If position was good
    if ( GetPosition ( ) != NULL )
    {
        int levelIndex = GetLevel() - 1;
        BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), levelIndex, wRoom, dwWestToEast, dwVertical , dwSouthToNorth);
        if ( ! bCheck )
        {
            return FALSE;
        }

        TR4_POSITION *pTR4Position = GetTR4Position ( );
        if ( pTR4Position != NULL )
        {
            if ( abs ( pTR4Position->wSouthToNorth - (WORD) ( dwSouthToNorth / TR4_FACTOR ) ) > 2 )
            {
                pTR4Position->wSouthToNorth = (WORD) ( dwSouthToNorth / TR4_FACTOR );
            }

            if ( abs ( pTR4Position->wVertical - (WORD) ( dwVertical / TR4_FACTOR ) ) > 2 )
            {
                pTR4Position->wVertical     = (WORD) ( dwVertical / TR4_FACTOR );
            }

            if ( abs ( pTR4Position->wWestToEast - (WORD) ( dwWestToEast / TR4_FACTOR ) ) > 2 )
            {
                pTR4Position->wWestToEast   = (WORD) ( dwWestToEast / TR4_FACTOR );
            }

            if ( abs ( pTR4Position->cOrientation - CTRXTools::MakeOrientationToOneByte ( wDirection ) ) > 2 )
            {
                pTR4Position->cOrientation  = CTRXTools::MakeOrientationToOneByte ( wDirection );
            }

            pTR4Position->cRoom         = (BYTE) wRoom;

            return TRUE;
        }
    }

    return FALSE;
}

