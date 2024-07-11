// TR4NGSaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR4NGSaveGame.h"
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
static TR45_INDICATORS IndicatorsTR4NGTable [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE },
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE },
    {   FALSE,  0x02,   0x02,   0x00,   0x28,   FALSE },
    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   FALSE },
    {   FALSE,  0x02,   0x02,   0x00,   0x0c,   FALSE },
    {   FALSE,  0x02,   0x02,   0x00,   0x1f,   FALSE },
    {   FALSE,  0x02,   0x02,   0x00,   0xbd,   FALSE },
    {   FALSE,  0x02,   0x02,   0x00,   0xdd,   FALSE },
    {   FALSE,  0x0d,   0x0d,   0x00,   0x6c,   FALSE },
    {   FALSE,  0x12,   0x00,   0x00,   0x02,   FALSE },        // Flare
    {   FALSE,  0xfd,   0xff,   0x00,   0x00,   FALSE },        // Jeep

    {   FALSE,  0x47,   0x47,   0x47,   0xde,   TRUE },         // Kneeling
    {   FALSE,  0x10,   0x00,   0x51,   0x51,   TRUE },         // Crawlling
    {   FALSE,  0x00,   0x02,   0x00,   0x02,   TRUE },
    {   FALSE,  0x00,   0x02,   0x00,   0x03,   TRUE },
    {   FALSE,  0x0c,   0x00,   0x00,   0x02,   TRUE },
    {   FALSE,  0x21,   0x21,   0x00,   0x6e,   TRUE },         // In Water

    {   TRUE,   0xff,   0xff,   0xff,   0xff,   TRUE },         // End
};
static int IndicatorsTR4NGTableCount = sizeof(IndicatorsTR4NGTable)/sizeof(TR45_INDICATORS);

//
static int positionCount = 0;
static TR4NG_POSITION *positionTable [ MAX_POSITION ];

//
/////////////////////////////////////////////////////////////////////////////
// CTR4NGSaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR4NGSaveGame, CTR45SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR4NGSaveGame::CTR4NGSaveGame()
{

    m_iVersion          = 49;
    m_iSubVersion       = 9,

    m_iSaveLength       = CTRXGlobal::m_iMaxNGSize;
    m_iMaxLevel         = TR4NGMAXLEVEL;

    iMaskGun            = TR40NG_GUN_SET1 | TR40NG_GUN_SET8;        //  Pistol
    iMaskUzi            = TR40NG_GUN_SET1;
    iMaskRiotGun        = TR40NG_GUN_SET1 | TR40NG_GUN_SET8;
    iMaskCrossBow       = TR40NG_GUN_SET1 | TR40NG_GUN_SET8;        // Crossbow
    iMaskGrenade        = TR40NG_GUN_SET1 | TR40NG_GUN_SET8;
    iMaskRevolver       = TR40NG_GUN_SET1;                          // Revolver
    iMaskLaser          = TR40NG_GUN_SET1;
    iMaskBinocular      = TR40NG_GUN_SET1;
    iMaskCrowBar        = TR40NG_GUN_SET1;

    iRiotGunUnits       = 6;

    m_pLife             = NULL;

    m_pBuffer           = new ( TR4NGSAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR4NGSAVE) );

    m_pBufferBackup     = new ( TR4NGSAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR4NGSAVE) );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR4NGSaveGame::~CTR4NGSaveGame()
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
// CTR4NGSaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR4NGSaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::ReadSavegame ( const char *pFilename )
{

        FILE                    *hFile;
        size_t                uLenBuffer;

        char                    szEmpty [ 1 ];

        strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
        strcpy_s ( m_Status, sizeof(m_Status), "" );

        memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4NGSAVE ) );

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
        if ( m_iSaveLength < CTRXGlobal::m_iMinNGSize || m_iSaveLength > CTRXGlobal::m_iMaxNGSize )
        {
            sprintf_s ( m_Status, sizeof(m_Status), "Internal error in length %d versus %d = %d.",
                (int) sizeof ( TR4NGSAVE ), m_iSaveLength,
                m_iSaveLength - (int) sizeof ( TR4NGSAVE ) );
            fclose ( hFile );
            return 0;
        }

        //
        memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4NGSAVE ) );
        uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
        if ( uLenBuffer != m_iSaveLength )
        {
            strcpy_s ( m_Status, sizeof(m_Status), "File size is not correct." );
            fclose ( hFile );
            return 0;
        }

        //
        //  The last eight byte if the file is
        //  4E474C4526120000    NGLE&...
        char *pSignature = (char* ) m_pBuffer + uLenBuffer - 8;
        if ( memcmp ( pSignature, "NGLE", 4 ) != 0  )
        {
            strcpy_s ( m_Status, sizeof(m_Status), "File Signature is not correct." );
            fclose ( hFile );
            return 0;
        }

        //
        long lPos = ftell ( hFile );
        if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
        {
            fseek ( hFile, 0, SEEK_END );
            long lEnd = ftell ( hFile );
            sprintf_s ( m_Status, sizeof(m_Status), "File size is too large %ld til %ld = %ld.", lPos, lEnd, lEnd - lPos );
            fclose ( hFile );
            return 0;
        }

        memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR4NGSAVE) );

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
void CTR4NGSaveGame::writeSaveGame()
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
    for ( int i = 0; i < sizeof(TR4NGSAVE) - 1; i++ )
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
    if ( m_iSaveLength < CTRXGlobal::m_iMinNGSize || m_iSaveLength > CTRXGlobal::m_iMaxNGSize )
    {
        sprintf_s ( m_Status, sizeof(m_Status), "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR4NGSAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR4NGSAVE ) );
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
void CTR4NGSaveGame::RetrieveInformation( const char *pFilename )
{
    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );
        m_iSubVersion   = 9;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR4NGGUN *CTR4NGSaveGame::SearchGunStructure ( unsigned short m_iHealth, int *iPos )
{
    return ( &m_pBuffer->tagGuns );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckIfAmmosMatch ( TR4NGGUN *pGun, WORD gunBitmap )
{
    int             iX;

    iX = getLevelIndex ();

    return Valid();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::RetrieveHeader()
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
void CTR4NGSaveGame::GetAmmosValues()
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
int CTR4NGSaveGame::ConvertSecret(int iSecret)
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
int CTR4NGSaveGame::ConvertSecretBack(int iSecret)
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
void CTR4NGSaveGame::GetDetailedInfo (  char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR4 Next Gen" );
    strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
    *iGame  = m_pBuffer->iSaveNumber;
    *iLevel = getLevel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::getLevel()
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
const char * CTR4NGSaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::getLevelIndex()
{
    return getLevel() - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos1(int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGunAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos2(int iX )
{
    return ( m_pBuffer->tagAmmo.m_iRevolverAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos3( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iUziAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos4a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iShotGunAmmo1 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos4b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iShotGunAmmo2 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos5 ( int iX )
{
    return ( -1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos6( int iX )
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
int CTR4NGSaveGame::GetAmmos7a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos7b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade2 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos7c( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iGrenade3 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8a( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8c( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iCrossbow3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos1 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGunAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos2 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iRevolverAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos3 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iUziAmmos = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos4a ( const char *szString, int iX )
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
void CTR4NGSaveGame::SetAmmos4b ( const char *szString, int iX )
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
void CTR4NGSaveGame::SetAmmos5 ( const char *szString, int iX )
{
    // m_pBuffer->tagAmmo.m_iUnknown = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos6 ( const char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7a ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7b ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7c ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrenade3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8a ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8b ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8c ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iCrossbow3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetUnlimitedAmmos(int iMask)
{
    // m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetUnlimitedAmmos()
{
    //return ( m_pBuffer->cUnlimited );
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::Valid()
{
    if ( m_pBuffer->tagGuns.m_gunRevolver != 0 && ( m_pBuffer->tagGuns.m_gunRevolver & TR40NG_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunRevolver & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunRiotGun != 0 && ( m_pBuffer->tagGuns.m_gunRiotGun & TR40NG_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunRiotGun & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunUzis != 0 && ( m_pBuffer->tagGuns.m_gunUzis & TR40NG_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunUzis & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunGrenadesLauncher != 0 && ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & TR40NG_GUN_MASK ) == 0  &&
        ( m_pBuffer->tagGuns.m_gunGrenadesLauncher & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunCrossBow != 0 && ( m_pBuffer->tagGuns.m_gunCrossBow & TR40NG_GUN_MASK ) == 0 &&
        ( m_pBuffer->tagGuns.m_gunCrossBow & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunCrowBar != 0 && ( m_pBuffer->tagGuns.m_gunCrowBar & TR40NG_GUN_MASK ) == 0  &&
        ( m_pBuffer->tagGuns.m_gunCrowBar & TR40NG_GUN_SET4 ) == 0 )
    {
        return 0;
    }
    return ( 1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetInvalid()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon1 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon4 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon2 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon3 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon5 ( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon8 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon7 ( int iX )
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
int CTR4NGSaveGame::CheckWeapon6 ( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Headset
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon9 ( int iX )
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
unsigned char CTR4NGSaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    // m_pBuffer->tagGuns.cObjects = iMaskCompass;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunPistol;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunPistol &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunPistol |= iMaskGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunPistol &= ( iMaskGun ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRiotGun;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRiotGun &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRiotGun |= iMaskRiotGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRiotGun &= ( iMaskRiotGun ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRevolver;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRevolver &= ( TR40NG_GUN_SET1 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRevolver |= iMaskRevolver;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRevolver &= ( iMaskRevolver ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunUzis;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunUzis &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunUzis |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunUzis &= ( iMaskUzi ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{

    // m_pBuffer->tagGuns.cObjects |= iMaskMP5;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    // m_pBuffer->tagGuns.cObjects |= iMaskRocket;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunGrenadesLauncher;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunGrenadesLauncher &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunGrenadesLauncher |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunGrenadesLauncher &= ( iMaskGrenade ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunCrossBow;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunCrossBow &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunCrossBow |= iMaskCrossBow;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunCrossBow &= ( iMaskCrossBow ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon9 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunCrowBar;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunCrowBar &= ( TR40NG_GUN_SET4 ^ 0xffff );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunCrowBar |= iMaskCrowBar;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunCrowBar &= ( iMaskCrowBar ^ 0xff );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iSmallMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iLargeMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetFlares ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iFlares );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetLaser ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunLaserLight );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetBinocular ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunBinocular );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAir ( )
{
    return ( m_pBuffer->iAir );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iSmallMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLargeMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iLargeMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetFlares ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iFlares = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLaser ( char *szString, int iX )
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
void CTR4NGSaveGame::SetBinocular ( char *szString, int iX )
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
void CTR4NGSaveGame::SetAir ( const char *szString )
{
    m_pBuffer->iAir = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4NGSaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetCurrentSecrets ( char *szString, int iX )
{
    //  No Change
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAllSecrets ( )
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
int CTR4NGSaveGame::IsLaraBurning ()
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
void CTR4NGSaveGame::SetLaraBurning (int state)
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
void CTR4NGSaveGame::SetLaraNoBurning (int state)
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
void CTR4NGSaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetMap1 ()
{
    return ( 0 /* m_pBuffer->cObject_P1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetMap2 ()
{
    return ( 0 /*m_pBuffer->cObject_P2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetItem1 ()
{
    return ( 0 /*m_pBuffer->cObjectP1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetItem2 ()
{
    return ( 0 /*m_pBuffer->cObjectP2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetItem3 ()
{
    return ( 0 /*m_pBuffer->cObjectP3 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetItem4 ()
{
    return (0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetKey1 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetKey2 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetKey3 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetKey4 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetTreasure1 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetTreasure2 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetTreasure3 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetTreasure4 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetMap1 ( char *szString )
{
    //m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetMap2 ( char *szString )
{
    //m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetItem1 ( char *szString )
{
    //m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetItem2 ( char *szString )
{
    //m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetItem3 ( char *szString )
{
    //m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetItem4 ( char *szString )
{
    //m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetKey1 ( char *szString )
{
    //m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetKey2 ( char *szString )
{
    //m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetKey3 ( char *szString )
{
    //m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetKey4 ( char *szString )
{
    //m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetTreasure1 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetMaximumSecrets ( int iX )
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
int CTR4NGSaveGame::GetGunAmmos ( )
{
    return GetAmmos1(0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetGunAmmos ( const char *szGunAmmos )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR4NGSaveGame::GetIndicatorAddress (int index)
{
    //
    BYTE *pBuffer   = ( BYTE * ) m_pBuffer;
    int count       = 0;

    for ( int i = 0x0280; i < 0x3000; i++ )
    {
        //  Compare with Indicators
        for ( int j = 0; j < IndicatorsTR4NGTableCount;  j++ )
        {
            if ( IndicatorsTR4NGTable [ j ].bEnd )
            {
                break;
            }

            if (    pBuffer [ i ] == IndicatorsTR4NGTable [ j ].b1 &&
                    pBuffer [ i + 1 ] == IndicatorsTR4NGTable [ j ].b2 &&
                    pBuffer [ i + 3 ] == IndicatorsTR4NGTable [ j ].b4 )
            {
                if ( IndicatorsTR4NGTable [ j ].useB3 && pBuffer [ i + 2 ] != IndicatorsTR4NGTable [ j ].b3 )
                {
                    continue;
                }

                // In TR4 Life is between 0 and 999 (0 means 1000)
                short life = * (short * ) ( pBuffer + i + TR4NG_LIFE_OFFSET );

                //  Life Is valid between 0 and 1000
                if ( life < 0 || life > 1000 )
                {
                    continue;
                }

                //
                count++;
                if ( count > index )
                {
                    return pBuffer + i;
                }
            }
        }
    }

    //
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
int CTR4NGSaveGame::GetLife ()
{
    //
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        short *pLife = ( short * ) ( &pBuffer [ TR4NG_LIFE_OFFSET ] );
        return *pLife;
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLife ( const char *szLife )
{
    //
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        WORD *pLife = ( WORD * ) ( & pBuffer [ TR4NG_LIFE_OFFSET ] );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4NGSaveGame::getBufferAddress ()
{
    return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR4NGSaveGame::getBufferBackupAddress ()
{
    return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR4NGSaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetBufferLength(size_t len)
{
    m_iSaveLength   = (unsigned) len;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetCurrentSecrets ()
{
    return ( m_pBuffer->tagAmmo.m_iSecretCount );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetLaraState ( )
{
    return m_pBuffer->laraState;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLaraState ( int state )
{
    m_pBuffer->laraState = state;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetItems ( int item, BYTE value )
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
BYTE CTR4NGSaveGame::GetItems ( int item )
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
BOOL CTR4NGSaveGame::BufferModified ()
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
void CTR4NGSaveGame::discard ()
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
TR4NG_POSITION *CTR4NGSaveGame::GetTR4Position ( )
{
    const int extraSearch = 8;

    ZeroMemory ( positionTable, sizeof(positionTable) );
    positionCount   = 0;

#ifdef _DEBUG
    OutputDebugString ( "\n" );
#endif

    //  We Search n times
    //  The Goal is to see if there is a =atch with position for an index
    //  For example we could have an indicator but no position
    //  So we will look the next indicator
    for ( int index = 0; index < IndicatorsTR4NGTableCount; index++ )
    {
        if ( IndicatorsTR4NGTable [ index ].bEnd )
        {
            break;
        }

        char *pBuffer = (char * ) GetIndicatorAddress(index);
        if ( pBuffer )
        {
            for ( int i = 0; i < extraSearch; i++ )
            {
                TR4NG_POSITION *pTR4Position = (TR4NG_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR4NG_POSITION_OFFSET ) );

                DWORD dwSouthToNorth    = pTR4Position->wSouthToNorth * TR4NG_FACTOR;
                DWORD dwVertical        = pTR4Position->wVertical * TR4NG_FACTOR;
                DWORD dwWestToEast      = pTR4Position->wWestToEast * TR4NG_FACTOR;
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
                    dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                    dwExtraVertical         = ( 0x0100 << 16 );
                }
                BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex, wRoom, dwWestToEast, dwVertical, dwSouthToNorth, true, dwExtraVertical );
                if ( bCheck )
                {
#ifdef _DEBUG
                    //  Life is not there
                    short life = * (short * ) ( pBuffer + TR4NG_LIFE_OFFSET );

                    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer + i, m_pBuffer );
                    static char szDebugString [ MAX_PATH ];
                    sprintf_s ( szDebugString, sizeof(szDebugString), "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x %3u %5d %5d %5d %3u %4d\n", 
                        dwRelativeAddress, pTR4Position->indicator1, pTR4Position->indicator2, pTR4Position->indicator3, pTR4Position->indicator4, 
                        pTR4Position->cRoom, pTR4Position->wVertical, pTR4Position->wSouthToNorth, pTR4Position->wWestToEast, pTR4Position->cOrientation,
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
    if ( CTRXGlobal::m_iSearchPosExt )
    {
        char *pBuffer = ( char * ) m_pBuffer;

        //
        TR4NG_POSITION *pCurrent        = NULL;
        TR4NG_POSITION *pTR4Position    = NULL;

        for ( int i = 0x280; i < 0x3000; i++ )
        {
            pCurrent                = (TR4NG_POSITION *) ( ( BYTE * ) pBuffer + i );

            DWORD dwSouthToNorth    = ( DWORD) pCurrent->wSouthToNorth * TR4NG_FACTOR;
            DWORD dwVertical        = ( DWORD ) pCurrent->wVertical * TR4NG_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pCurrent->wWestToEast * TR4NG_FACTOR;
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
                dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                dwExtraVertical         = ( 0x0100 << 16 );
            }
            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth, true, dwExtraVertical );
            if ( bCheck )
            {
                //  Life between 0 and 1000
                short life = * (short * ) ( pBuffer + i + TR4NG_LIFE_OFFSET );

                positionTable [ positionCount ] = pCurrent;

                if ( life >= 0 && life <= 1000 )
                {
                    if ( pTR4Position == NULL )
                    {
                        pTR4Position    = pCurrent;
                    }
                    positionCount++;
                }

#ifdef _DEBUG
                DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer + i, m_pBuffer );
                static char szDebugString [ MAX_PATH ];
                sprintf_s ( szDebugString, sizeof(szDebugString), "indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x %3u %5d %5d %5d %3u %4d\n", 
                    dwRelativeAddress, pCurrent->indicator1, pCurrent->indicator2, pCurrent->indicator3, pCurrent->indicator4,
                    pCurrent->cRoom, pCurrent->wVertical, pCurrent->wSouthToNorth, pCurrent->wWestToEast, pCurrent->cOrientation,
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
const TR_POSITION *CTR4NGSaveGame::GetPosition ( )
{
    static TR_POSITION localTRPosition;
    ZeroMemory ( &localTRPosition, sizeof(localTRPosition)  );

    const TR4NG_POSITION *pTR4Position = GetTR4Position ( );
    if ( pTR4Position != NULL )
    {
        localTRPosition.dwSouthToNorth      = pTR4Position->wSouthToNorth * TR4NG_FACTOR;
        localTRPosition.dwVertical          = pTR4Position->wVertical * TR4NG_FACTOR;
        localTRPosition.dwWestToEast        = pTR4Position->wWestToEast * TR4NG_FACTOR;
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
BOOL CTR4NGSaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
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

        TR4NG_POSITION *pTR4Position = GetTR4Position ( );
        if ( pTR4Position != NULL )
        {
            if ( abs( pTR4Position->wSouthToNorth - (WORD) ( dwSouthToNorth / TR4NG_FACTOR ) ) > 2 ) 
            {
                pTR4Position->wSouthToNorth = (WORD) ( dwSouthToNorth / TR4NG_FACTOR );
            }

            if ( abs( pTR4Position->wVertical - (WORD) ( dwVertical / TR4NG_FACTOR ) ) > 2 )
            {
                pTR4Position->wVertical     = (WORD) ( dwVertical / TR4NG_FACTOR );
            }

            if ( abs( pTR4Position->wWestToEast - (WORD) ( dwWestToEast / TR4NG_FACTOR ) ) > 2 )
            {
                pTR4Position->wWestToEast   = (WORD) ( dwWestToEast / TR4NG_FACTOR );
            }

            if ( abs( pTR4Position->cOrientation - CTRXTools::MakeOrientationToOneByte ( wDirection ) ) > 2 )
            {
                pTR4Position->cOrientation  = CTRXTools::MakeOrientationToOneByte ( wDirection );
            }

            pTR4Position->cRoom         = (BYTE) wRoom;
            return TRUE;
        }
    }

    return FALSE;
}

