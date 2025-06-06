// TR5SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR5SaveGame.h"
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

//
/////////////////////////////////////////////////////////////////////////////
//  Indicator Table
/////////////////////////////////////////////////////////////////////////////
TR45_INDICATORS IndicatorsTR5Table [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE,   0,  "Standing", },          // Step 0 must be
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE,   0,  "Standing", },          // At Start

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
int IndicatorsTR5TableCount = sizeof(IndicatorsTR5Table)/sizeof(TR45_INDICATORS);

//
static int positionCount = 0;
static TR5_POSITION *positionTable [ MAX_POSITION ];

/*
 *      ------------------------------------------------
 *      Data.
 *      ------------------------------------------------
 */
static const int Many32K            = 0x7fff;

static char    TR5NBSecrets [ ] =
{
    /* Streets of Rome */       3,      //  01
    /* Trajan's Markets */      3,      //  12
    /* The Colosseum */         3,      //  03
    /* The Base */              3,      //  04
    /* The Submarine */         3,      //  05
    /* Deep Sea Dive */         1,      //  06
    /* Sinking Submarine */     2,      //  07
    /* Gallows Tree */          3,      //  08
    /* Labyrinth */             3,      //  09
    /* Old Mill */              3,      //  10
    /* 13th Floor */            3,      //  11
    /* Escape With The Iris */  3,      //  12
    /*  Security Breach */      0,      //  13
    /* Red Alert! */            3       //  14
};

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static WORD HealthPosition [ TR5_LEVELS ] =
{
    0x4f4,  //      For Level 1 Street
    0x542,  //      for Level 2 trajan
    0x4d4,  //      for Level 3 colise
    0x55a,  //      for Level 4 the base
    0x520,  //      for Level 5 the submarine
    0x644,  //      for Level 6 deep sea
    0x5d2,  //      for Level 7 Sinking
    0x4f0,  //      for Level 8 Gallow
    0x538,  //      for Level 9 Labyrith
    0x512,  //      for Level 10 Old Mill
    0x52a,  //      for Level 11 13th floor
    0x6f6,  //      for Level 12 Escape with iris
    0x52e,  //      for Level 13 Security Breach
    0x52e,  //      for Level 14 Red Alert
};

//
static TR_POSITION_RANGE TR5IndicatorRange [ TR5_LEVELS ] =
{
    { 0x4e0, 0x4f0 },       //      For Level 1 Street
    { 0x530, 0x5f0 },       //      for Level 2 trajan
    { 0x4c0, 0x4ef },       //      for Level 3 colise
    { 0x540, 0x6bf },       //      for Level 4 the base
    { 0x510, 0x6AF },       //      for Level 5 the submarine
    { 0x630, 0x6ef },       //      for Level 6 deep sea
    { 0x580, 0x6FF },       //      for Level 7 Sinking
    { 0x4e0, 0x530 },       //      for Level 8 Gallow
    { 0x530, 0x620 },       //      for Level 9 Labyrith
    { 0x500, 0x620 },       //      for Level 10 Old Mill
    { 0x520, 0x530 },       //      for Level 11 13th floor
    { 0x6d0, 0xc30 },       //      for Level 12 Escape with iris
    { 0x280, 0xD00 },       //      for Level 13 Security Breach
    { 0x520, 0x590 },       //      for Level 14 Red Alert
};

//
/////////////////////////////////////////////////////////////////////////////
// CTR5SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR5SaveGame, CTR45SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR5SaveGame::CTR5SaveGame()
{

    m_iVersion          = GAME_TR50;

    m_iSaveLength       = TR5LEVELMAXSIZE;
    m_iMaxLevel         = TR5MAXLEVEL;

    iMaskPistol         = TR5_MASK_PISTOL;      //  Pistol
    iMaskUzi            = TR5_MASK_UZI;
    iMaskRiotGun        = TR5_MASK_SHOTGUN;
    iMaskHKGun          = TR5_MASK_HKGUN;
    iMaskDesertEagle    = TR5_MASK_DESERT;      // Revolver
    iMaskLaser          = TR5_MASK_LASER;
    iMaskBinocular      = TR5_MASK_BINOCULAR;
    iMaskHeadSet        = TR5_MASK_HEADSET;
    iMaskRevolver       = TR5_MASK_REVOLVER;

    iRiotGunUnits       = 6;

    m_pRealHealth       = NULL;

    m_pBuffer           = new ( TR5SAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR5SAVE) );

    m_pBufferBackup     = new ( TR5SAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR5SAVE) );

    m_iSubVersion       = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR5SaveGame::~CTR5SaveGame()
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
// CTR5SaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR5SaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::ReadSavegame ( const char *pFilename )
{

    FILE                    *hFile;
    size_t                  uLenBuffer;

    char                    szEmpty [ 1 ];

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
    InitStatus ();

    //
    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR5SAVE ) );
    memset ( ( char * ) m_pBufferBackup, 0, sizeof ( TR5SAVE ) );

    /*
     *      Read file.
     */
    hFile = NULL;
    fopen_s ( &hFile,  m_Filename, "rb" );
    if ( hFile == NULL )
    {
        AddToStatus ( "Unable to read file." );
        return 0;
    }

    /*
     *      Get Buffer.
     */
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && m_iSaveLength != TR5LEVELMAXSIZE )
    {
        AddFormatToStatus ( "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR5SAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR5SAVE ) );
        CloseOneFile ( &hFile );
        return 0;
    }

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR5SAVE ) );
    uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        AddToStatus ( "File size is not correct." );
        CloseOneFile ( &hFile );
        return 0;
    }

    long lPos = ftell ( hFile );
    if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
    {
        fseek ( hFile, 0, SEEK_END );
        long lEnd = ftell ( hFile );
        AddFormatToStatus ( "File size is too large %ld til %ld = %ld.", lPos, lEnd, lEnd - lPos );
        CloseOneFile ( &hFile );
        return 0;
    }

    memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR5SAVE) );

    /*
     *      Close file.
     */
    CloseOneFile ( &hFile );

    return 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::writeSaveGame()
{
    FILE                    *hFile;
    size_t                  uLenBuffer;

    AddToStatus ( "" );

    /*
     *      Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( ! ( m_pBuffer->tagGuns.m_gunDesertEagle & iMaskDesertEagle ) )
    {
        m_pBuffer->tagGuns.m_gunDesertEagle  = 0 ;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunUzis & iMaskUzi ) )
    {
        m_pBuffer->tagGuns.m_gunUzis = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunRiotGun & iMaskRiotGun ) )
    {
        m_pBuffer->tagGuns.m_gunRiotGun = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunHK & iMaskHKGun ) )
    {
        m_pBuffer->tagGuns.m_gunHK = 0;
    }

    if ( ! ( m_pBuffer->tagGuns.m_gunRevolver & iMaskDesertEagle ) )
    {
        m_pBuffer->tagGuns.m_gunRevolver = 0;
    }

    //
    Backup_Savegame();

    /*
     *      Write file.
     */
    hFile = NULL;
    fopen_s ( &hFile,  m_Filename, "wb" );
    if ( hFile == NULL )
    {
        AddToStatus ( "Unable to write the file." );
        return;
    }

    /*
     *      Get Buffer.
     */
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && m_iSaveLength != TR5LEVELMAXSIZE )
    {
        AddFormatToStatus ( "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR5SAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR5SAVE ) );
        CloseOneFile ( &hFile );
        return;
    }

    uLenBuffer = fwrite ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        AddToStatus ( "File size is not correct." );
        CloseOneFile ( &hFile );
        return;
    }

    /*
     *      Close file.
     */
    CloseOneFile ( &hFile );

    //
    memcpy ( m_pBufferBackup,  m_pBuffer, m_iSaveLength );

    AddToStatus ( "File successfully updated." );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::RetrieveInformation( const char *pFilename )
{
    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR5GUN *CTR5SaveGame::SearchGunStructure ( unsigned short m_iHealth, int *iPos )
{
    return ( &m_pBuffer->tagGuns );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckIfAmmosMatch ( TR5GUN *pGun, WORD gunBitmap )
{
    int             iX;

    iX = getLevelIndex ();

    return Valid();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::RetrieveHeader()
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
void CTR5SaveGame::GetAmmosValues()
{
        int     iX;

        /*
         *      Get gun state.
         */
        iX = getLevelIndex ();

        /*
         *      Get current values for Guns.
         */
        m_iDesertEagle      = m_pBuffer->tagGuns.m_gunDesertEagle;
        m_iRiotGun          = m_pBuffer->tagGuns.m_gunRiotGun;
        m_iUzis             = m_pBuffer->tagGuns.m_gunUzis;
        m_iHK               = m_pBuffer->tagGuns.m_gunHK;
        m_iRevolver         = m_pBuffer->tagGuns.m_gunRevolver;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::ConvertSecret(int iSecret)
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
int CTR5SaveGame::ConvertSecretBack(int iSecret)
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
void CTR5SaveGame::GetDetailedInfo (    char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR5 Chronicle" );
    strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
    *iGame  = m_pBuffer->iSaveNumber;
    *iLevel = m_pBuffer->m_cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTR5SaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::getLevel()
{
    return m_pBuffer->m_cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::getLevelIndex()
{
    return m_pBuffer->m_cLevel - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos1(int iX )
{
    return m_pBuffer->tagAmmo.m_iGunAmmos;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos2(int iX )
{
    return m_pBuffer->tagAmmo.m_iRevolverAmmos;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos3( int iX )
{
    return m_pBuffer->tagAmmo.m_iUziAmmos;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos4a( int iX )
{
    return m_pBuffer->tagAmmo.m_iShotGunAmmo1 / iRiotGunUnits;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos4b( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iShotGunAmmo2 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//  HK
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos5 ( int iX )
{
    return m_pBuffer->tagAmmo.m_iHKAmmos;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos6( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos7a( int iX )
{
    // return m_pBuffer->tagAmmo.m_iUnknown1;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos7b( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos7c( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos8a( int iX )
{
    return m_pBuffer->tagAmmo.m_iGrapplingAmmos;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos8b( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAmmos8c( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos1 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGunAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos2 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iRevolverAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos3 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iUziAmmos = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos4a ( const char *szString, int iX )
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
void CTR5SaveGame::SetAmmos4b ( const char *szString, int iX )
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
void CTR5SaveGame::SetAmmos5 ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iHKAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos6 ( const char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos7a ( const char *szString, int iX )
{
//  m_pBuffer->tagAmmo.m_iUnknown1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos7b ( const char *szString, int iX )
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos7c ( const char *szString, int iX )
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos8a ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iGrapplingAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos8b ( const char *szString, int iX )
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAmmos8c ( const char *szString, int iX )
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetUnlimitedAmmos(int iMask)
{
    // m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetUnlimitedAmmos()
{
    //return ( m_pBuffer->cUnlimited );
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::Valid()
{
    if ( m_pBuffer->tagGuns.m_gunUzis != 0 && ( m_pBuffer->tagGuns.m_gunUzis & TR5_MASK_ANY ) == 0  &&
            ( m_pBuffer->tagGuns.m_gunUzis & TR5_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Guns Invalid" );
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunRiotGun != 0 && ( m_pBuffer->tagGuns.m_gunRiotGun & TR5_MASK_ANY ) == 0  &&
            ( m_pBuffer->tagGuns.m_gunRiotGun & TR5_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Riot Gun Invalid" );
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunDesertEagle != 0 && ( m_pBuffer->tagGuns.m_gunDesertEagle & TR5_MASK_ANY ) == 0  &&
            ( m_pBuffer->tagGuns.m_gunDesertEagle & TR5_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Desert Eagle Invalid" );
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunHK != 0 && ( m_pBuffer->tagGuns.m_gunHK & TR5_MASK_ANY ) == 0 &&
            ( m_pBuffer->tagGuns.m_gunHK & TR5_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "HK Invalid" );
        return 0;
    }

    if (    m_pBuffer->tagGuns.m_gunRevolver != 0 && ( m_pBuffer->tagGuns.m_gunRevolver & TR5_MASK_ANY ) == 0 )
    {
        AddToStatus ( "Revolver Invalid" );
        return 0;
    }

    if ( m_pBuffer->tagGuns.m_gunHeadSet != 0 && ( m_pBuffer->tagGuns.m_gunHeadSet & TR5_MASK_ANY ) == 0 &&
            ( m_pBuffer->tagGuns.m_gunHeadSet & TR5_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "HeadSet Invalid" );
        return 0;
    }

    return ( 1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetInvalid()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon1 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunPistol & iMaskPistol )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon2 ( int iX )
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
int CTR5SaveGame::CheckWeapon3 ( int iX )
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
int CTR5SaveGame::CheckWeapon4 ( int iX )
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
int CTR5SaveGame::CheckWeapon5 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunHK & iMaskHKGun )
    {
        return 1;
    }

    return  0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon6 ( int iX )
{

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon7 ( int iX )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon8 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunDesertEagle & iMaskDesertEagle )
    {
        return 1;
    }

    return  0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Headset
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::CheckWeapon9 ( int iX )
{
    if ( m_pBuffer->tagGuns.m_gunHeadSet & iMaskHeadSet )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    // m_pBuffer->tagGuns.cObjects = iMaskCompass;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunPistol;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunPistol &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunPistol |= iMaskPistol;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunPistol &= ( iMaskPistol ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRevolver;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRevolver &= ( TR5_GUN_SET1 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRevolver |= iMaskRevolver;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRevolver &= ( iMaskRevolver ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunUzis;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunUzis &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunUzis |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunUzis &= ( iMaskUzi ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunRiotGun;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunRiotGun &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunRiotGun |= iMaskRiotGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunRiotGun &= ( iMaskRiotGun ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunHK;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunHK &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunHK |= iMaskHKGun;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunHK &= ( iMaskHKGun ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunDesertEagle;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunDesertEagle &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunDesertEagle |= iMaskDesertEagle;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunDesertEagle &= ( iMaskDesertEagle ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR5SaveGame::GrabWeapon9 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->tagGuns.m_gunHeadSet;
    if ( ! bChange ) return old;

    m_pBuffer->tagGuns.m_gunHeadSet &= ( TR5_GUN_SET4 ^ TR5_MASK_ALL );
    if ( bAdd ) m_pBuffer->tagGuns.m_gunHeadSet |= iMaskHeadSet;
    if ( ! bAdd ) m_pBuffer->tagGuns.m_gunHeadSet &= ( iMaskHeadSet ^ TR5_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iSmallMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iLargeMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetFlares ( int iX )
{
    return ( m_pBuffer->tagAmmo.m_iFlares );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetLaser ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunLaserLight );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetBinocular ( int iX )
{
    return ( m_pBuffer->tagGuns.m_gunBinocular );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetAir ( )
{
    return m_pBuffer->iAir;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iSmallMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetLargeMedipak ( const char *szString, int iX )
{
     m_pBuffer->tagAmmo.m_iLargeMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetFlares ( const char *szString, int iX )
{
    m_pBuffer->tagAmmo.m_iFlares = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetLaser ( char *szString, int iX )
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
void CTR5SaveGame::SetBinocular ( char *szString, int iX )
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
void CTR5SaveGame::SetAir ( const char *szString )
{
    m_pBuffer->iAir = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR5SaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetCurrentSecrets ( char *szString, int iX )
{
    //  No Change
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAllSecrets ( )
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
int CTR5SaveGame::IsLaraBurning ()
{
    if ( m_pBuffer->laraState & STATE_45_BURNING )
    {
        return 1;
    }
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetLaraBurning (int state)
{
    if ( state != -1 )
    {
        m_pBuffer->laraState |= STATE_45_BURNING;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetLaraNoBurning (int state)
{
    if ( state != -1 )
    {
        m_pBuffer->laraState &= STATE_45_SAFE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetMap1 ()
{
    return ( 0 /* m_pBuffer->cObject_P1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetMap2 ()
{
    return ( 0 /*m_pBuffer->cObject_P2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetItem1 ()
{
    return ( 0 /*m_pBuffer->cObjectP1 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetItem2 ()
{
    return ( 0 /*m_pBuffer->cObjectP2 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetItem3 ()
{
    return ( 0 /*m_pBuffer->cObjectP3 */ );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetItem4 ()
{
    return (0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetKey1 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetKey2 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetKey3 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetKey4 ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetTreasure1 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetTreasure2 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetTreasure3 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetTreasure4 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetMap1 ( char *szString )
{
    //m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetMap2 ( char *szString )
{
    //m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetItem1 ( char *szString )
{
    //m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetItem2 ( char *szString )
{
    //m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetItem3 ( char *szString )
{
    //m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetItem4 ( char *szString )
{
    //m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetKey1 ( char *szString )
{
    //m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetKey2 ( char *szString )
{
    //m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetKey3 ( char *szString )
{
    //m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetKey4 ( char *szString )
{
    //m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetTreasure1 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetMaximumSecrets ( int iX )
{
    if ( iX >= 0 && iX < sizeof(TR5NBSecrets) )
    {
        int allSecrets = 0;
        for ( int i = 0; i <= iX; i++ )
        {
            allSecrets += TR5NBSecrets [ i ];
        }
        return allSecrets;
        // return TR5NBSecrets [ iX ];
    }
    return 0;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetGunAmmos ( )
{
    return GetAmmos1(0);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetGunAmmos ( const char *szGunAmmos )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void *CTR5SaveGame::GetIndicatorAddress ( int index )
{
    int levelIndex  = GetLevelIndex() % ( sizeof(TR5IndicatorRange) / sizeof(TR_POSITION_RANGE) );
    int minOffset   = TR5IndicatorRange [ levelIndex ].minOffset;
    int maxOffset   = TR5IndicatorRange [ levelIndex ].maxOffset;

    if ( ! CTRXGlobal::m_UseTR5PositionRange )
    {
        minOffset   = MinTR5PositionOffset;
        maxOffset   = MaxTR5PositionOffset;
    }

    //
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    BYTE *pBuffer   = ( BYTE * ) m_pBuffer;
    int count = 0;
    for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
    {
        //  Compare with Indicators
        for ( int indice = 0; indice < IndicatorsTR5TableCount;  indice++ )
        {
            if ( IndicatorsTR5Table [ indice ].bEnd )
            {
                break;
            }

            //
            //  Reliable index are step = 0
            if ( ! CTRXGlobal::m_iSearchPosExt && IndicatorsTR5Table [ indice ].step != 0 )
            {
                break;
            }

            if ( IndicatorsTR5Table [ indice ].step > CTRXGlobal::m_TR5IndexMaximum )
            {
                continue;
            }

            if (    pBuffer [ iBuffer ] == IndicatorsTR5Table [ indice ].b1 &&
                    pBuffer [ iBuffer + 1 ] == IndicatorsTR5Table [ indice ].b2 &&
                    pBuffer [ iBuffer + 3 ] == IndicatorsTR5Table [ indice ].b4 )
            {
                if ( IndicatorsTR5Table [ indice ].useB3 && pBuffer [ iBuffer + 2 ] != IndicatorsTR5Table [ indice ].b3 )
                {
                    continue;
                }

                WORD wRealHealth = * ( WORD * ) ( pBuffer + iBuffer + TR5_REALHEALTH_OFFSET );
                if ( ! IsTR5HealthValid ( wRealHealth, false ) )
                {
                    continue;
                }

                count++;
                if ( count > index )
                {
                    strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTR5Table [ indice ].szLabel );
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
WORD *CTR5SaveGame::GetTR5RealHealthAddress ()
{
    //
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer )
    {
        WORD *pRealHealth = ( WORD * ) ( pBuffer + TR5_REALHEALTH_OFFSET );

        if ( ! IsTR5HealthValid ( *pRealHealth, true ) )
        {
            return NULL;
        }

#ifdef _DEBUG
        static char szDebugString [ MAX_PATH ];
        DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer, m_pBuffer );
        sprintf_s ( szDebugString, sizeof(szDebugString), 
            "Life Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x H:%-6d\n", 
            dwRelativeAddress, pBuffer [ 0 ] & 0xff, pBuffer [ 1 ] & 0xff, pBuffer [ 2 ] & 0xff, pBuffer [ 3 ] & 0xff, *pRealHealth );
        OutputDebugString ( szDebugString );
#endif

        //
        //  Verify Position
        for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
        {
            TR5_POSITION *pTR5Position = (TR5_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR5_POSITION_OFFSET ) );
            
            DWORD dwSouthToNorth    = ( DWORD) pTR5Position->wSouthToNorth * TR5_FACTOR;
            DWORD dwVertical        = ( DWORD ) pTR5Position->wVertical * TR5_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pTR5Position->wWestToEast * TR5_FACTOR;
            WORD wRoom              = pTR5Position->cRoom;

            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
            if ( bCheck )
            {
                //
                return pRealHealth;
            }
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetRealHealth ()
{
    //
    WORD *pRealHealth = GetTR5RealHealthAddress();
    if ( pRealHealth )
    {
        return (int) *pRealHealth;
    }

    return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetRealHealth ( const char *szRealHealth )
{
    int iLife = atoi(szRealHealth);
    WORD *pRealHealth = GetTR5RealHealthAddress();
    if ( pRealHealth )
    {
        *pRealHealth = (WORD) iLife;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR5SaveGame::getBufferAddress ()
{
    return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR5SaveGame::getBufferBackupAddress ()
{
    return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR5SaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetCurrentSecrets ()
{
    return ( m_pBuffer->tagAmmo.m_iSecretCount );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR5SaveGame::GetLaraState ( )
{
    return m_pBuffer->laraState;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetLaraState ( int state )
{
    m_pBuffer->laraState = state;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR5SaveGame::SetItems ( int item, BYTE value )
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
BYTE CTR5SaveGame::GetItems ( int item )
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
BOOL CTR5SaveGame::BufferModified ()
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
void CTR5SaveGame::discard ()
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
TR5_POSITION *CTR5SaveGame::GetTR5Position ( )
{
    

    ZeroMemory ( positionTable, sizeof(positionTable) );
    positionCount   = 0;

#ifdef _DEBUG
    OutputDebugString ( "GetTR5Position\n" );
#endif

    //  We Search n times
    //  The Goal is to see if there is a =atch with position for an index
    //  For example we could have an indicator but no position
    //  So we will look the next indicator
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    for ( int index = 0; index < IndicatorsTR5TableCount; index++ )
    {
        if ( IndicatorsTR5Table [ index ].bEnd )
        {
            break;
        }

        if ( IndicatorsTR5Table [ index ].step > CTRXGlobal::m_TR5IndexMaximum )
        {
            continue;
        }

        char *pBuffer = (char * ) GetIndicatorAddress(index);
        if ( pBuffer )
        {
            TR5_POSITION *pTR5Position0 = (TR5_POSITION *) ( ( ( BYTE * ) pBuffer - TR5_POSITION_OFFSET ) );

            //
            for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
            {
                //  We Consider pBuffer - i pointing to indicator1
                TR5_POSITION *pTR5Position = (TR5_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR5_POSITION_OFFSET ) );
            
                DWORD dwSouthToNorth    = ( DWORD) pTR5Position->wSouthToNorth * TR5_FACTOR;
                DWORD dwVertical        = ( DWORD ) pTR5Position->wVertical * TR5_FACTOR;
                DWORD dwWestToEast      = ( DWORD ) pTR5Position->wWestToEast * TR5_FACTOR;
                WORD wRoom              = pTR5Position->cRoom;

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

                BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
                if ( bCheck )
                {
#ifdef _DEBUG
                    WORD wRealHealth = * ( WORD * ) ( pBuffer + TR5_REALHEALTH_OFFSET );

                    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer - i, m_pBuffer );
                    static char szDebugString [ MAX_PATH ];
                    sprintf_s ( szDebugString, sizeof(szDebugString),
                        "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x R:%-3u V:%-5d SN:%-5d WE:%-5d D:%-3u H:%-6d\n", 
                        dwRelativeAddress, 
                        pTR5Position0->indicator1, pTR5Position0->indicator2, pTR5Position0->indicator3, pTR5Position0->indicator4,
                        wRoom, dwVertical, dwSouthToNorth, dwWestToEast, pTR5Position->cOrientation,
                        wRealHealth ); 
                    OutputDebugString ( szDebugString );
#endif
                    positionTable [ 0 ] = pTR5Position;
                    positionCount       = 1;
                    return pTR5Position;
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

        TR5_POSITION *pCurrent          = NULL;
        TR5_POSITION *pTR5Position      = NULL;

        //
        int levelIndex  = GetLevelIndex() % ( sizeof(TR5IndicatorRange) / sizeof(TR_POSITION_RANGE) );;
        int minOffset   = TR5IndicatorRange [ levelIndex ].minOffset;
        int maxOffset   = TR5IndicatorRange [ levelIndex ].maxOffset;

        if ( ! CTRXGlobal::m_UseTR5PositionRange )
        {
            minOffset   = MinTR5PositionOffset;
            maxOffset   = MaxTR5PositionOffset;
        }

        //
        for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
        {
            //  We Consider pBuffer + i pointing to indicator1
            pCurrent                = (TR5_POSITION *) ( ( BYTE * ) pBuffer + iBuffer );

            DWORD dwSouthToNorth    = ( DWORD) pCurrent->wSouthToNorth * TR5_FACTOR;
            DWORD dwVertical        = ( DWORD ) pCurrent->wVertical * TR5_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pCurrent->wWestToEast * TR5_FACTOR;
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

            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(),  wRoom, dwWestToEast, dwVertical, dwSouthToNorth, true );
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
                    TR5_POSITION *pTR5Position0    = (TR5_POSITION *) ( (char *) pCurrent + i );

                    WORD wRealHealth = pTR5Position0->health;

                    //
                    if ( IsTR5HealthValid ( wRealHealth, false ) )
                    {
                        positionTable [ positionCount ] = pCurrent;
                        if ( pTR5Position == NULL )
                        {
                            pTR5Position = pCurrent;
                        }
                        positionCount++;
                    }

#ifdef _DEBUG
                    dwRelativeAddress = CTRXTools::RelativeAddress ( & pTR5Position0->indicator1, m_pBuffer );
                    sprintf_s ( szDebugString, sizeof(szDebugString),
                        "- indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x H:%-6d\n", 
                        dwRelativeAddress,
                        pTR5Position0->indicator1, pTR5Position0->indicator2, pTR5Position0->indicator3, pTR5Position0->indicator4,
                        wRealHealth ); 
                    OutputDebugString ( szDebugString );

                    if ( CTRXGlobal::m_iUnchecked == FALSE )
                    {
                        if ( positionCount > 20 )
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

        return pTR5Position;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const TR_POSITION *CTR5SaveGame::GetPosition ( )
{
    static TR_POSITION localTRPosition;
    ZeroMemory ( &localTRPosition, sizeof(localTRPosition)  );

    const TR5_POSITION *pTR5Position = GetTR5Position ( );
    if ( pTR5Position != NULL )
    {
        int levelIndex = GetLevel() - 1;

        localTRPosition.dwSouthToNorth      = ( DWORD) pTR5Position->wSouthToNorth * TR5_FACTOR;
        localTRPosition.dwVertical          = ( DWORD ) pTR5Position->wVertical * TR5_FACTOR;
        localTRPosition.dwWestToEast        = ( DWORD ) pTR5Position->wWestToEast * TR5_FACTOR;
        localTRPosition.wOrientation        = pTR5Position->cOrientation << 8;
        localTRPosition.wRoom               = pTR5Position->cRoom;

        return &localTRPosition;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR5SaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
    //
    //  Check If position was good
    int levelIndex = GetLevel() - 1;

    BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), levelIndex, wRoom, dwWestToEast, dwVertical, dwSouthToNorth );
    if ( ! bCheck )
    {
        return FALSE;
    }

    TR5_POSITION *pTR5Position = GetTR5Position ( );
    if ( pTR5Position != NULL )
    {
        if ( abs ( pTR5Position->wSouthToNorth - (WORD) ( dwSouthToNorth / TR5_FACTOR ) ) > 2 )
        {
            pTR5Position->wSouthToNorth = (WORD) ( dwSouthToNorth / TR5_FACTOR );
        }

        if ( abs ( pTR5Position->wVertical - (WORD) ( dwVertical / TR5_FACTOR ) ) > 2 )
        {
            pTR5Position->wVertical     = (WORD) ( dwVertical / TR5_FACTOR );
        }

        if ( abs ( pTR5Position->wWestToEast - (WORD) ( dwWestToEast / TR5_FACTOR ) ) > 2 )
        {
            pTR5Position->wWestToEast   = (WORD) ( dwWestToEast / TR5_FACTOR );
        }

        if ( abs ( pTR5Position->cOrientation - CTRXTools::MakeOrientationToOneByte ( wDirection ) ) > 2 )
        {
            pTR5Position->cOrientation  = CTRXTools::MakeOrientationToOneByte ( wDirection );
        }

        pTR5Position->cRoom         = (BYTE) wRoom;
        return TRUE;
    }

    return FALSE;
}

