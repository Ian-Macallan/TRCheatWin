// TR4SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR4SaveGame.h"
#include "TRXTools.h"
#include "TR_Areas.h"
#include "TRXGlobal.h"

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
// CTR4SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR4SaveGame, CTRSaveGame)

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
void *CTR4SaveGame::GetIndicatorAddress ()
{
    //
    BYTE *pBuffer   = ( BYTE * ) m_pBuffer;
    for ( int i = 0x0280; i < 0x3000; i++ )
    {
        if ( ( pBuffer [ i ] == 0x02 &&  pBuffer [ i + 1 ] == 0x02 /* && pBuffer [ i + 2 ] == 0x00 */ && pBuffer [ i + 3 ] == 0x67 ) )  // Normal
        {
            return pBuffer + i;
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
int CTR4SaveGame::GetLife ()
{
    //
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        WORD *pLife = ( WORD * ) ( &pBuffer [ 20 ] );
        return *pLife;
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
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        WORD *pLife = ( WORD * ) ( & pBuffer [ 20 ] );
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
    const int extraSearch = 16;

    char *pBuffer = (char * )GetIndicatorAddress();
    if ( pBuffer )
    {
        for ( int i = 0; i < extraSearch; i++ )
        {
            TR4_POSITION *pTR4Position = (TR4_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR4_POSITION_OFFSET ) );

            DWORD dwSouthToNorth    = pTR4Position->wSouthToNorth * TR4_FACTOR;
            DWORD dwVertical        = pTR4Position->wVertical * TR4_FACTOR;
            DWORD dwWestToEast      = pTR4Position->wWestToEast * TR4_FACTOR;
            WORD wRoom              = pTR4Position->cRoom;

            int tombraider = GetFullVersion();
            int levelIndex = GetLevelIndex();
            BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex, wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
            if ( bCheck )
            {
                return pTR4Position;
            }
        }
    }

    //
    //  Search Extended
    if ( CTRXGlobal::m_iSearchPosExt )
    {
        pBuffer = ( char * ) m_pBuffer;
        for ( int i = 0x280; i < 0x3000; i++ )
        {
            TR4_POSITION *pTR4Position = (TR4_POSITION *) ( ( BYTE * ) pBuffer + i );

            DWORD dwSouthToNorth    = ( DWORD) pTR4Position->wSouthToNorth * TR4_FACTOR;
            DWORD dwVertical        = ( DWORD ) pTR4Position->wVertical * TR4_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pTR4Position->wWestToEast * TR4_FACTOR;
            WORD wRoom              = pTR4Position->cRoom;

            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
            if ( bCheck )
            {
                return pTR4Position;
            }
        }
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
            pTR4Position->wSouthToNorth = (WORD) ( dwSouthToNorth / TR4_FACTOR );
            pTR4Position->wVertical     = (WORD) ( dwVertical / TR4_FACTOR );
            pTR4Position->wWestToEast   = (WORD) ( dwWestToEast / TR4_FACTOR );
            pTR4Position->cOrientation  = CTRXTools::MakeOrientationToOneByte ( wDirection );
            pTR4Position->cRoom         = (BYTE) wRoom;
            return TRUE;
        }
    }

    return FALSE;
}

