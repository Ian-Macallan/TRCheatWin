// TR3SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR3SaveGame.h"
#include "GunGrids.h"
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
 *      TR3 Secret Mask
 *      ------------------------------------------------
 */
static char    TR3Secrets [ ] =
{
    /*
     *              India
     */
    0x3F,   /* 01 6 */
    0x0F,   /* 02 4 */
    0x1F,   /* 03 5 */
    0x00,   /* 04 0 */

    /*
     *              Pacific
     */
    0x07,   /* 05 3 */
    0x07,   /* 06 3 */
    0x07,   /* 07 3 */
    0x01,   /* 08 1 */

    /*
     *              London
     */
    0x1F,   /* 09 5 */
    0x1F,   /* 10 5 */
    0x3F,   /* 11 6 */
    0x01,   /* 12 1 */

    /*
     *              Nevada
     */
    0x07,   /* 13 3 */
    0x03,   /* 14 2 */
    0x07,   /* 15 3 */

    /*
     *              Antarctica
     */
    0x07,   /* 16 3 */
    0x07,   /* 17 3 */
    0x07,   /* 18 3 */
    0x00,   /* 19 0 */

    /*
     *              All Hallows
     */
    0x00,   /* 20 0 */

    0x00,   /* 0 */
    0x00,   /* 0 */
    0x00,   /* 0 */
};

//  Gold : The last Artifact
static char    TR3GSecrets [ ] =
{
    0x07,   /* 01 3 */
    0x07,   /* 02 3 */
    0x07,   /* 03 3 */
    0x07,   /* 04 3 */
    0x07,   /* 05 3 */
    0x00    /* 06 0 */
};

//  Standard TR3 positions
static unsigned TR30Positions [ ] =
{
    0x158f,     /* 01 */
    0x2311,     /* 02 */
    0x1748,     /* 03 */
    0xc63,      /* 04 */
    0x16f5,     /* 05 */
    0x1817,     /* 06 */
    0x1361,     /* 07 */
    0x105d,     /* 08 */
    0x17b7,     /* 09 */
    0x2243,     /* 10 */
    0x1ccd,     /* 11 */
    0xa37,      /* 12 */
    0x16e8,     /* 13 */
    0x1d47,     /* 14 */
    0x202d,     /* 15 */
    0x18d9,     /* 16 */
    0x189b,     /* 17 */
    0x1cdb,     /* 18 */
    0xa3f,      /* 19 */
    0xf71,      /* 20 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
};

static unsigned TR39Positions [ ] =
{
    0x0001,     /* 01 */
    0x0002,     /* 02 */
    0x0003,     /* 03 */
    0x0004,     /* 04 */
    0x0005,     /* 05 */
    0x0006,     /* 06 */
    0x0007,     /* 07 */
    0x0008,     /* 08 */
    0x0009,     /* 09 */
    0x000a,     /* 10 */
    0x000b,     /* 11 */
    0x000c,     /* 12 */
    0x000d,     /* 13 */
    0x000e,     /* 14 */
    0x000f,     /* 15 */
    0x0000,     /* 16 */
    0x0000,     /* 17 */
    0x0000,     /* 18 */
    0x0000,     /* 19 */
    0x0000,     /* 20 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
    0x0000,     /* 0 */
};


//
/////////////////////////////////////////////////////////////////////////////
// CTR3SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR3SaveGame, CTR123SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR3SaveGame::CTR3SaveGame()
{
    m_iVersion          = GAME_TR30;

    m_iSaveLength       = TR3LEVELMAXSIZE;
    m_iMaxLevel         = TR3MAXLEVEL;

    //
    iMaskCompass        = TR3_MASK_COMPASS;
    iMaskPistol         = TR3_MASK_PISTOL;
    iMaskDesertEagle    = TR3_MASK_DESERT;
    iMaskUzi            = TR3_MASK_UZI;
    iMaskShotGun        = TR3_MASK_SHOTGUN;
    iMaskMP5            = TR3_MASK_MP5;
    iMaskRocket         = TR3_MASK_ROCKET;
    iMaskGrenade        = TR3_MASK_GRENADE;
    iMaskHarpoon        = TR3_MASK_HARPOON;

    m_pRealHealth       = NULL;

    m_pGun              = NULL;

    m_pBuffer           = new ( TR3SAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR3SAVE) );

    m_pBufferBackup     = new ( TR3SAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR3SAVE) );

    m_iSubVersion       = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR3SaveGame::~CTR3SaveGame()
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
// CTR3SaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR3SaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::ReadSavegame( const char *pFilename )
{

    FILE                    *hFile;
    size_t                  uLenBuffer;

    char                    szEmpty [ 1 ];

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
    InitStatus ();

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR3SAVE ) );

    /*
     *      Read file.
     */
    hFile = NULL;
    openRead ( hFile,  m_Filename, "rb" );
    if ( hFile == NULL )
    {
        AddToStatus ( "Unable to read file." );
        return 0;
    }

    /*
     *      Get Buffer.
     */
    if (    CTRXGlobal::m_ForceSaveGame == FORCE_NONE &&
            m_iSaveLength != TR3LEVELALT1SIZE && 
            m_iSaveLength != TR3LEVELALT2SIZE && 
            ( m_iSaveLength < TR3LEVELMINSIZE || m_iSaveLength > TR3LEVELMAXSIZE ) )
    {
        AddToStatus ( "Internal error in length." );
        CloseOneFile ( &hFile );
        return 0;
    }

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR3SAVE ) );
    uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        AddToStatus ( "File size is not correct." );
        CloseOneFile ( &hFile );
        return 0;
    }

    //
    if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
    {
        AddToStatus ( "File size is too large." );
        CloseOneFile ( &hFile );
        return 0;
    }

    memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR3SAVE) );

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
void CTR3SaveGame::writeSaveGame()
{
    FILE                    *hFile;
    unsigned                uLenBuffer;

    InitStatus ();

    /*
     *      Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( m_pGun != NULL )
    {
        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle ) )
        {
            m_pGun->m_iDesertEagle  = 0 ;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi ) )
        {
            m_pGun->m_iUzis = 0;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskShotGun ) )
        {
            m_pGun->m_iRiotGun = 0;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade ) )
        {
            m_pGun->m_iGrenades = 0;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskRocket ) )
        {
            m_pGun->m_iRockets = 0;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon ) )
        {
            m_pGun->m_iHarpoon = 0;
        }

        if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskMP5 ) )
        {
            m_pGun->m_iMP5 = 0;
        }
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
    if (    CTRXGlobal::m_ForceSaveGame == FORCE_NONE &&
            m_iSaveLength != TR3LEVELALT1SIZE && 
            m_iSaveLength != TR3LEVELALT2SIZE && 
            ( m_iSaveLength < TR3LEVELMINSIZE || m_iSaveLength > TR3LEVELMAXSIZE ) )
    {
        AddToStatus ( "Internal error in length." );
        CloseOneFile ( &hFile );
        return;
    }

    uLenBuffer = (unsigned) fwrite ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
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
void CTR3SaveGame::RetrieveInformation( const char *pFilename )
{
    char            *pStartAddress;
    char            *pGunAddress;
    char            *pAddress;

    unsigned        iPosition;
    int             iX;
    int             iPos;
    int             iCount;

    int             bExactFound;

    TR3AMMOS        *pGun;

    /*
     *  Set address to error.
     */
    m_pGun                      = NULL;

    m_iSubVersion               = 0;

    unsigned *TR3Positions     = TR30Positions;

    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );

        if ( m_pBuffer->ind1 == 0xFA )
        {
            m_iSubVersion = 0;
        }
        else
        {
            m_iSubVersion       = GAME_TRG5;
        }

        if ( m_iSaveLength < TR3LEVELMINSIZE || m_iSaveLength > TR3LEVELMAXSIZE )
        {
            m_iSubVersion       = GAME_TRC9;
            TR3Positions        = TR39Positions;
        }

        //
        const char *saves = "saves";
        if ( _stricmp ( theApp.FindLastDirectory ( pFilename ), saves ) == 0 )
        {
            m_iSubVersion       = GAME_TRC9;
            TR3Positions        = TR39Positions;
        }

        /*
         *      Search for health in file.
         */
        iPos            = -1;
        iCount          = 0;
        bExactFound     = 0;

        /*
         *      Get Level.
         */
        iX = getLevelIndex ();

        do
        {
            pGun = SearchGunStructure ( m_iGunAmmos, m_pBuffer->trTable [ iX ].cGunBitmap, &iPos );
            if ( pGun != NULL )
            {
                iCount++;

                m_pGun = pGun;
                AddToStatus ( "Data loaded from the saved game.");

                /*
                 *      Test if it it the same as in the table.
                 */
                pStartAddress   = ( char * ) m_pBuffer;
                pGunAddress     = pStartAddress + TR3Positions [ iX ];
                if ( ( char * ) m_pGun == pGunAddress )
                {
                    bExactFound = 1;
                }
            }
        }
        while ( iPos >= 0 /* && ! bExactFound */ );

        /*
         *      If the exact position has been found set it.
         */
        if ( bExactFound )
        {
            pStartAddress = ( char * ) m_pBuffer;
            pGunAddress   = pStartAddress + TR3Positions [ iX ];
            m_pGun = ( TR3AMMOS * ) pGunAddress;
        }

        /*
         *      If nothing found set the position to the default.
         */
        if ( m_pGun == NULL )
        {
            /*
             *      Get Level.
             */
            iX = getLevelIndex ();

            pStartAddress = ( char * ) m_pBuffer;
            pGunAddress   = pStartAddress + TR3Positions [ iX ];
            m_pGun = ( TR3AMMOS * ) pGunAddress;
            AddFormatToStatus ( "Unable to find something in the file: Setting the address %04x.",
                TR3Positions [ iX ] );

            m_pGun->m_iGunAmmos = m_iGunAmmos;
            m_pGun = NULL;

            return;
        }
        else
        {
            AddFormatToStatus ( "%d good position(s) found.", iCount );
            if ( bExactFound )
            {
                AddToStatus ( "Exact position found too." );
            }
        }

        /*
         *      Verify address.
         */
        pStartAddress = ( char * ) m_pBuffer;

        /*
         *      Get Level.
         */
        iX = getLevelIndex ();

        pGunAddress = pStartAddress + TR3Positions [ iX ];
        if ( ( char * ) m_pGun != pGunAddress )
        {
            pAddress = ( char * ) m_pGun;
            iPosition = (unsigned) ( pAddress - pStartAddress );
            AddFormatToStatus ( 
                "The %d address(es) differ(s): Reference is at the address %04lx instead of %04lx.",
                iCount, TR3Positions [ iX ], iPosition );

            if (  false )
            {
                AddFormatToStatus ( 
                    "The %d address(es) differ(s): Setting the address %04lx instead of %04lx.",
                    iCount, TR3Positions [ iX ], iPosition );
                m_pGun = ( TR3AMMOS * ) pGunAddress;
                m_pGun->m_iGunAmmos = m_iGunAmmos;
            }

        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR3AMMOS *CTR3SaveGame::SearchGunStructure ( unsigned short m_iGunAmmos, WORD gunBitmap, int *iPos )
{
        int                     iX;
        unsigned char           *pString;
        TR3AMMOS                *pGun;
        
        //
        DWORD offset = offsetof(TR3SAVE,szRemain);

        //
        if ( *iPos == -1 )
        {
            //  Size of remains minus size of structure
            iX = TR3BUFFEREND - sizeof ( TR3AMMOS );
        }
        else
        {
            iX = *iPos;
        }

        while ( iX >= 0 )
        {
            pString = & ( m_pBuffer->szRemain [ iX ] );
            pGun = ( TR3AMMOS * )(  pString );
            if (    ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos     ) )
                 || ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos + 1 ) )
                 || ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos - 1 ) ) )
            {
                if ( CheckIfAmmosMatch ( pGun, gunBitmap ) )
                {
                    *iPos = iX - 1;
                    return pGun;
                }
            }

            //  Special Case
            if (    ( m_iSaveLength < TR3LEVELMINSIZE || m_iSaveLength > TR3LEVELMAXSIZE ) &&
                    pGun->m_iGunAmmos == 9999 && pGun->iAir == 1800 )
            {
                if ( CheckIfAmmosMatch ( pGun, gunBitmap ) )
                {
                    *iPos = iX - 1;
                    return pGun;
                }
            }

            iX--;
        }

        *iPos = -1;
        pGun = NULL;

        return pGun;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckIfAmmosMatch ( TR3AMMOS *pGun, WORD gunBitmap )
{
    int             iX;

    //
    int iCount = 0;

    iX = getLevelIndex ();

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle )
    {
        if ( pGun->m_iDesertEagle  != m_iDesertEagle  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iDesertEagle  != 0 && pGun->m_iDesertEagle  != m_iDesertEagle )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi )
    {
        if ( pGun->m_iUzis     != m_iUzis  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if (    pGun->m_iUzis  != 0 && pGun->m_iUzis  != m_iUzis )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskShotGun )
    {
        if ( pGun->m_iRiotGun != m_iRiotGun  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iRiotGun != 0 && pGun->m_iRiotGun != m_iRiotGun )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade )
    {
        if ( pGun->m_iGrenades != m_iGrenades  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iGrenades != 0 && pGun->m_iGrenades != m_iGrenades )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskRocket )
    {
        if ( pGun->m_iRockets != m_iRockets  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iRockets != 0 && pGun->m_iRockets != m_iRockets )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon )
    {
        if ( pGun->m_iHarpoon != m_iHarpoon  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iHarpoon  != 0 && pGun->m_iHarpoon  != m_iHarpoon )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    //
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskMP5 )
    {
        if ( pGun->m_iMP5 != m_iMP5  )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }
    else
    {
        if ( pGun->m_iMP5 != 0 && pGun->m_iMP5 != m_iMP5 )
        {
            // return FALSE;
        }
        else
        {
            iCount++;
        }
    }

    if ( iCount >= ( 7 - CTRXGlobal::m_CheckAmmosTolerance ) )
    {
        return TRUE;
    }

    return FALSE;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::RetrieveHeader()
{
        /*
         *      Objects mask.
         */
        m_pBuffer->trSingle.cObjectMask = 0xff;

        /*
         */
        GetAmmosValues ( );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::GetAmmosValues()
{
        int     iX;

        /*
         *      Get gun state.
         */
        iX = getLevelIndex ();

        /*
         *      Get current values for ammos.
         */
        m_iGunAmmos    = m_pBuffer->trTable [ iX ].m_iGunAmmos;
        m_iDesertEagle = m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos;
        m_iRiotGun     = m_pBuffer->trTable [ iX ].m_iRiotGunAmmos;
        m_iUzis        = m_pBuffer->trTable [ iX ].m_iUziAmmos;
        m_iGrenades    = m_pBuffer->trTable [ iX ].m_iGrenadeAmmos;
        m_iRockets     = m_pBuffer->trTable [ iX ].m_iRocketAmmos;
        m_iHarpoon     = m_pBuffer->trTable [ iX ].m_iHarpoonAmmos;
        m_iMP5         = m_pBuffer->trTable [ iX ].m_iMP5Ammos;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::ConvertSecret(int iSecret)
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
int CTR3SaveGame::ConvertSecretBack(int iSecret)
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
void CTR3SaveGame::GetDetailedInfo (    char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR3" );
    strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
    *iGame  = m_pBuffer->iSaveNumber;
    *iLevel = m_pBuffer->trSingle.cLevel;
    if ( m_pBuffer->ind1 != 0xFA )
    {
        strcpy_s ( szGame, iSize, "TR3G" );
        m_iSubVersion       = GAME_TRG5;
    }

    if ( m_iSaveLength < TR3LEVELMINSIZE || m_iSaveLength > TR3LEVELMAXSIZE )
    {
        strcpy_s ( szGame, iSize, "TR3LE" );
        m_iSubVersion       = GAME_TRC9;
    }

    //
    const char *saves = "saves";
    if ( _stricmp ( theApp.FindLastDirectory ( m_Filename ), saves ) == 0 )
    {
        strcpy_s ( szGame, iSize, "TR3LE" );
        m_iSubVersion       = GAME_TRC9;
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTR3SaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::getLevel()
{
    return m_pBuffer->trSingle.cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::getLevelIndex()
{
    return m_pBuffer->trSingle.cLevel - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos1(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iGunAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iGunAmmos );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos2(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos );
    }

    return ( m_pGun->m_iDesertEagle );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos3( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iUziAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iUzis );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos4a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iRiotGunAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iRiotGun );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos5( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iMP5Ammos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iMP5 );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos6( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iRocketAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iRockets );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos7a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iGrenadeAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iGrenades );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAmmos8a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trTable [ iX ].m_iHarpoonAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iHarpoon );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos2 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iDesertEagle = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos3 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iUziAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iUzis = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos4a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iRiotGunAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iRiotGun = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos5 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iMP5Ammos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iMP5 = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos6 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iRocketAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iRockets = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos7a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iGrenadeAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iGrenades = atoi ( szString );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAmmos8a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].m_iHarpoonAmmos = atoi ( szString );
    }

    if ( m_pGun != NULL )
    {
        m_pGun->m_iHarpoon = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetUnlimitedAmmos(int iMask)
{
    m_pBuffer->trSingle.cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetUnlimitedAmmos()
{
    return ( m_pBuffer->trSingle.cUnlimited );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::Valid()
{
    if ( m_pGun != NULL )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetInvalid()
{
    m_pGun = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon1 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskPistol )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon4 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskShotGun )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon2 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon3 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon5 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskMP5 )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon6 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskRocket )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon7 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::CheckWeapon8 ( int iX )
{
    if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskCompass;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap = iMaskCompass;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskCompass ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskPistol;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskPistol;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskPistol ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskShotGun;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskShotGun;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskShotGun ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskDesertEagle;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskDesertEagle ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskUzi ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskMP5;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskMP5;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskMP5 ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskHarpoon;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskHarpoon ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskGrenade ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR3SaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskRocket;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskRocket;
    if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskRocket ^ TR3_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetGunAmmos ( )
{
        int     iX;

        /*
         *      Get gun state.
         */
        iX = getLevelIndex ();
        if ( m_pGun != NULL )
        {
            return ( m_pGun->m_iGunAmmos );
        }

        return ( m_pBuffer->trTable [ iX ].m_iGunAmmos );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cSmallPack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cLargePack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetFlares ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cFlare );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetAir ( )
{
    if ( m_pGun != NULL )
    {
        return ( m_pGun->iAir );
    }
    
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->trTable [ iX ].cSmallPack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetLargeMedipak ( const char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cLargePack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetFlares ( const char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cFlare = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAir ( const char *szString )
{
    if ( m_pGun != NULL )
    {
        m_pGun->iAir = atoi ( szString );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR3SaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetCurrentSecrets ()
{
    return ( ConvertSecret ( m_pBuffer->trSingle.cSecrets ) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetCurrentSecrets ( char *szString, int iX )
{
    m_pBuffer->trSingle.cSecrets =
        ConvertSecretBack ( atoi ( szString ) );
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trTable [ iX ].cSecrets =
            ConvertSecretBack ( atoi ( szString ) );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAllSecrets ( )
{
    int     iX;

    for ( iX = 0; iX < m_iMaxLevel; iX++ )
    {
        m_pBuffer->trTable [ iX ].cSecrets =    TR3Secrets [ iX ];
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetLaraState ( )
{
    if ( m_pGun != NULL )
    {
        return m_pGun->cState;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::IsLaraBurning ()
{
    if ( m_pGun != NULL )
    {
        if ( m_pGun->cState & STATE_123_BURNING )
        {
            return 1;
        }
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetLaraBurning (int state)
{
    if ( m_pGun != NULL && state != -1 )
    {
        m_pGun->cState &= STATE_123_SAFE;
        m_pGun->cState |= STATE_123_BURNING;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetLaraNoBurning (int state)
{
    if ( m_pGun != NULL && state != -1 )
    {
        m_pGun->cState &= STATE_123_SAFE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetAllObjects ()
{
    m_pBuffer->trSingle.cObjectMask = 0xff;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetMap1 ()
{
    return ( m_pBuffer->trSingle.cObject_P1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetMap2 ()
{
    return ( m_pBuffer->trSingle.cObject_P2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetItem1 ()
{
    return ( m_pBuffer->trSingle.cObjectP1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetItem2 ()
{
    return ( m_pBuffer->trSingle.cObjectP2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetItem3 ()
{
    return ( m_pBuffer->trSingle.cObjectP3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetItem4 ()
{
    return ( m_pBuffer->trSingle.cObjectP4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetKey1 ()
{
    return ( m_pBuffer->trSingle.cObjectK1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetKey2 ()
{
    return ( m_pBuffer->trSingle.cObjectK2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetKey3 ()
{
    return ( m_pBuffer->trSingle.cObjectK3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetKey4 ()
{
    return ( m_pBuffer->trSingle.cObjectK4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetTreasure1 ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cNbObject1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetTreasure2 ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cNbObject2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetTreasure3 ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cNbObject3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetTreasure4 ( int iX )
{
    return ( m_pBuffer->trTable [ iX ].cNbObject4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetMap1 ( char *szString )
{
    m_pBuffer->trSingle.cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetMap2 ( char *szString )
{
    m_pBuffer->trSingle.cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetItem1 ( char *szString )
{
    m_pBuffer->trSingle.cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetItem2 ( char *szString )
{
    m_pBuffer->trSingle.cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetItem3 ( char *szString )
{
    m_pBuffer->trSingle.cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetItem4 ( char *szString )
{
    m_pBuffer->trSingle.cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetKey1 ( char *szString )
{
    m_pBuffer->trSingle.cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetKey2 ( char *szString )
{
    m_pBuffer->trSingle.cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetKey3 ( char *szString )
{
    m_pBuffer->trSingle.cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetKey4 ( char *szString )
{
    m_pBuffer->trSingle.cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetTreasure1 ( char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cNbObject1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetTreasure2 ( char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cNbObject2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetTreasure3 ( char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cNbObject3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetTreasure4 ( char *szString, int iX )
{
    m_pBuffer->trTable [ iX ].cNbObject4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR3SaveGame::GetMaximumSecrets ( int iX )
{
    return ( ConvertSecret ( TR3Secrets [ iX ] ) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR3SaveGame::SetGunAmmos ( const char *szGunAmmos )
{
    // Do Nothing.
    /*
    int             iX;

    if ( m_pGun != NULL )
    {
        iX = getLevelIndex ();
        m_pGun->m_iGunAmmos = atoi ( szHealth );
        m_pBuffer->trTable [ iX ].m_iGunAmmos = atoi ( szHealth );
        m_pBuffer->trSingle.m_iGunAmmos = atoi ( szHealth );
    }
     */
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR3SaveGame::getBufferAddress ()
{
    return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR3SaveGame::getBufferBackupAddress ()
{
    return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR3SaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR3SaveGame::BufferModified ()
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
void CTR3SaveGame::discard ()
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
const TR_POSITION *CTR3SaveGame::GetPosition ( )
{
    GetRealHealth ();
    if ( m_pRealHealth != NULL )
    {
        long dwDelta = (long) ( (BYTE* )m_pRealHealth - (BYTE* )m_pBuffer );
        TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pRealHealth - TR_POSITION_OFFSET );
        int tombraider = GetFullVersion();
        int levelIndex = GetLevelIndex();
        BOOL bCheck =
            CheckAreaForCoordinates ( tombraider, levelIndex,
                position->wRoom, position->dwWestToEast, position->dwVertical,  position->dwSouthToNorth );
        if ( bCheck )
        {
            return position;
        }
    }

    //
    //  Search Extended
    if ( CTRXGlobal::m_iSearchPosExt )
    {
        for ( int i = 0xA00; i < 0x2350; i++ )
        {
            TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pBuffer + i );
            int tombraider = GetFullVersion();
            int levelIndex = GetLevelIndex();
            BOOL bCheck =
                CheckAreaForCoordinates ( tombraider, levelIndex,
                    position->wRoom, position->dwWestToEast, position->dwVertical,  position->dwSouthToNorth );
            if ( bCheck )
            {
                return position;
            }
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR3SaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
    GetRealHealth ();

    //  Check if position was good
    if ( GetPosition ( ) == NULL )
    {
        return FALSE;
    }

    if ( m_pRealHealth != NULL )
    {
        long dwDelta = (long) ( (BYTE* )m_pRealHealth - (BYTE* )m_pBuffer );
        TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pRealHealth - TR_POSITION_OFFSET );
        position->dwWestToEast      = dwWestToEast;
        position->dwVertical        = dwVertical;
        position->dwSouthToNorth    = dwSouthToNorth;
        position->wOrientation      = wDirection;
        position->wRoom             = wRoom;

        return TRUE;
    }

    return FALSE;
}

