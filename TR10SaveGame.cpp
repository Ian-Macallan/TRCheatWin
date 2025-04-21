// TR1SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR10SaveGame.h"
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
 *      Data.
 *      ------------------------------------------------
 */
static unsigned TR1Positions [ ] =
{
    0x635-0x86,     /* 01 */
    0xc18-0x86,     /* 02 */
    0x58c-0x86,     /* 03 */
    0x83e-0x86,     /* 04 */
    0xd64-0x86,     /* 05 */
    0xa4a-0x86,     /* 06 */
    0xd3c-0x86,     /* 07 */
    0xc7e-0x86,     /* 08 */
    0x931-0x86,     /* 09 */
    0x851-0x86,     /* 10 */
    0x897-0x86,     /* 11 */
    0x70c-0x86,     /* 12 */
    0x89c-0x86,     /* 13 */
    0xff2-0x86,     /* 14 */
    0x8c6-0x86,     /* 15 */
    0x0,            /* 16 */
    0x0,            /* 17 */
    0x0,            /* 18 */
    0x0,            /* 19 */
    0x0,            /* 20 */
    0x00,           /* 0 */
    0x00,           /* 0 */
    0x00,           /* 0 */
    0x00,           /* 0 */
    0x00,           /* 0 */
};

//
/////////////////////////////////////////////////////////////////////////////
// CTR1SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR1SaveGame, CTR123SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR1SaveGame::CTR1SaveGame()
{

    m_iVersion          = GAME_TR10;

    m_iSaveLength       = TR1LEVELMAXSIZE;
    m_iMaxLevel         = TR1MAXLEVEL;

    iMaskCompass        = TR1_MASK_COMPASS;
    iMaskPistol         = TR1_MASK_PISTOL;
    iMaskMagnum         = TR1_MASK_MAGNUM;
    iMaskUzi            = TR1_MASK_UZI;
    iMaskShotGun        = TR1_MASK_SHOTGUN;
    iMaskHarpoon        = TR1_MASK_HARPOON;
    iMaskMP5            = TR1_MASK_MP5;
    iMaskGrenade        = TR1_MASK_GRENADE;

    m_pRealHealth       = NULL;
    m_pGun              = NULL;

    m_pBuffer           = new ( TR1SAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR1SAVE) );
    m_pBufferBackup     = new ( TR1SAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR1SAVE) );

    m_iSubVersion       = 0;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR1SaveGame::~CTR1SaveGame()
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
// CTR1SaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR1SaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::ReadSavegame ( const char *pFilename )
{

    FILE                    *hFile;
    size_t                  uLenBuffer;

    char                    szEmpty [ 1 ];

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
    InitStatus ();

    //
    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR1SAVE ) );

    /*
     *      Read file.
     */
    hFile = NULL;
    fopen_s ( &hFile, m_Filename, "rb" );
    if ( hFile == NULL )
    {
        AddToStatus ( "Unable to read file." );
        return 0;
    }

    /*
     *      Get Buffer.
     */
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < TR1LEVELMINSIZE || m_iSaveLength > TR1LEVELMAXSIZE ) )
    {
        AddToStatus ( "Internal error in length." );
        CloseOneFile ( &hFile );
        return 0;
    }

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR1SAVE ) );
    uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        AddToStatus ( "File size is not correct." );
        CloseOneFile ( &hFile );
        return 0;
    }

    if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
    {
        AddToStatus ( "File size is too large." );
        CloseOneFile ( &hFile );
        return 0;
    }

    memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR1SAVE) );

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
void CTR1SaveGame::writeSaveGame()
{
    FILE                    *hFile;
    size_t                  uLenBuffer;

    //
    InitStatus();

    /*
     *      Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( m_pGun != NULL )
    {
        if ( ! ( m_pBuffer->trSingle.cGunBitmap & iMaskMagnum ) )
        {
            m_pGun->m_iDesertEagle = 0 ;
        }

        if ( ! ( m_pBuffer->trSingle.cGunBitmap & iMaskUzi ) )
        {
            m_pGun->m_iUzis = 0;
        }

        if ( ! ( m_pBuffer->trSingle.cGunBitmap & iMaskShotGun ) )
        {
            m_pGun->m_iRiotGun = 0;
        }
    }

    /*
     *      Write file.
     */
    hFile = NULL;
    fopen_s ( &hFile, m_Filename, "wb" );
    if ( hFile == NULL )
    {
        AddToStatus ( "Unable to write the file." );
        return;
    }

    /*
     *      Get Buffer.
     */
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < TR1LEVELMINSIZE || m_iSaveLength > TR1LEVELMAXSIZE ) )
    {
        AddToStatus ( "Internal error in length." );
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
void CTR1SaveGame::RetrieveInformation( const char *pFilename )
{
    char            *pStartAddress;
    char            *pGunAddress;
    char            *pAddress;

    unsigned        iPosition;
    int             iX;
    int             iPos;
    int             iCount;

    int             bExactFound;

    TR1AMMOS        *pGun;

    /*
     *  Set address to error.
     */
    m_pGun          = NULL;

    m_iSubVersion   = 0;

    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );

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
            pGun = SearchGunStructure ( m_iGunAmmos, m_pBuffer->trSingle.cGunBitmap, &iPos );
            if ( pGun != NULL )
            {
                iCount++;

                m_pGun = pGun;
                AddToStatus ( "Data loaded from the saved game.");

                /*
                 *      Test if it it the same as in the table.
                 */
                pStartAddress   = ( char * ) m_pBuffer;
                pGunAddress     = pStartAddress + TR1Positions [ iX ];
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
            pGunAddress   = pStartAddress + TR1Positions [ iX ];
            m_pGun = (TR1AMMOS * ) pGunAddress;
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
            pGunAddress   = pStartAddress + TR1Positions [ iX ];
            m_pGun = (TR1AMMOS * ) pGunAddress;

            AddFormatToStatus ( "Unable to find something in the file: Setting the address %04x.", TR1Positions [ iX ] );

            m_pGun->m_iGunAmmos = m_iGunAmmos;
            m_pGun              = NULL;

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

        pGunAddress = pStartAddress + TR1Positions [ iX ];
        if ( ( char * ) m_pGun != pGunAddress )
        {
            pAddress = ( char * ) m_pGun;
            iPosition = (unsigned) ( pAddress - pStartAddress );
            AddFormatToStatus (
                "The %d address(es) differ(s): Reference is at the address %04lx instead of %04lx.",
                iCount, TR1Positions [ iX ], iPosition );
            if ( false )
            {
                AddFormatToStatus (
                    "The %d address(es) differ(s): Setting the address %04lx instead of %04lx.",
                    iCount, TR1Positions [ iX ], iPosition );
                m_pGun = ( TR1AMMOS * ) pGunAddress;
                m_pGun->m_iGunAmmos = m_iGunAmmos;
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR1AMMOS *CTR1SaveGame::SearchGunStructure ( WORD m_iGunAmmos, WORD gunBitmap, int *iPos )
{
        int                     iX;
        BYTE                    *pString;
        TR1AMMOS                *pGun;
        
        if ( *iPos == -1 )
        {
            iX = TR1BUFFEREND - sizeof ( TR1AMMOS );
        }
        else
        {
            iX = *iPos;
        }

        while ( iX >= 0 )
        {
            pString = & ( m_pBuffer->szRemain [ iX ] );
            pGun = ( TR1AMMOS * )(  pString );
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
int CTR1SaveGame::CheckIfAmmosMatch ( TR1AMMOS *pGun, WORD gunBitmap )
{

    if ( m_pBuffer->trSingle.cGunBitmap & iMaskMagnum )
    {
        if ( pGun->m_iDesertEagle  != m_iDesertEagle  )
        {
            return 0;
        }
    }
    else
    {
        if ( pGun->m_iDesertEagle  != 0 && pGun->m_iDesertEagle  != m_iDesertEagle )
        {
            return 0;
        }
    }

    if ( m_pBuffer->trSingle.cGunBitmap & iMaskUzi )
    {
        if ( pGun->m_iUzis != m_iUzis  )
        {
            return 0;
        }
    }
    else
    {
        if ( pGun->m_iUzis != 0 && pGun->m_iUzis != m_iUzis )
        {
            return 0;
        }
    }

    if ( m_pBuffer->trSingle.cGunBitmap & iMaskShotGun )
    {
        if ( pGun->m_iRiotGun != m_iRiotGun  )
        {
            return 0;
        }
    }
    else
    {
        if ( pGun->m_iRiotGun != 0 && pGun->m_iRiotGun != m_iRiotGun )
        {
            return 0;
        }
    }

    return 1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::RetrieveHeader()
{
        /*
         *      Objects mask.
         */
//      m_pBuffer->cObjectMask = 0xff;

        /*
         */
        GetAmmosValues ( );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::GetAmmosValues()
{
    int     iX;

    /*
     *      Get gun state.
     */
    iX = getLevelIndex ();

    /*
     *      Get current values for ammos.
     */
    m_iGunAmmos      = m_pBuffer->trTable [ iX ].m_iGunAmmos;
    m_iDesertEagle = m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos;
    m_iRiotGun     = m_pBuffer->trTable [ iX ].m_iRiotGunAmmos;
    m_iUzis        = m_pBuffer->trTable [ iX ].m_iUziAmmos;

    m_iGunAmmos      = m_pBuffer->trSingle.m_iGunAmmos;
    m_iDesertEagle = m_pBuffer->trSingle.m_iDesertEagleAmmos;
    m_iRiotGun     = m_pBuffer->trSingle.m_iRiotGunAmmos;
    m_iUzis        = m_pBuffer->trSingle.m_iUziAmmos;

//      m_iGrenades    = m_pBuffer->trTable [ iX ].m_iGrenades;
//      m_iRockets     = m_pBuffer->trTable [ iX ].m_iRockets;
//      m_iHarpoon     = m_pBuffer->trTable [ iX ].m_iHarpoon;
//      m_iMP5         = m_pBuffer->trTable [ iX ].m_iM16Gun;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::ConvertSecret(int iSecret)
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
int CTR1SaveGame::ConvertSecretBack(int iSecret)
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
void CTR1SaveGame::GetDetailedInfo (    char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR1" );
    strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
    *iGame  = m_pBuffer->iSaveNumber;
    *iLevel = m_pBuffer->cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTR1SaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::getLevel()
{
    return m_pBuffer->cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::getLevelIndex()
{
    return m_pBuffer->cLevel - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAmmos1(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iGunAmmos );
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
int CTR1SaveGame::GetAmmos2(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iDesertEagleAmmos );
    }

    if ( m_pGun != NULL )
    {
        return ( m_pGun->m_iDesertEagle );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAmmos3( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iUziAmmos );
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
int CTR1SaveGame::GetAmmos4a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iRiotGunAmmos );
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
int CTR1SaveGame::GetAmmos5( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iM16Gun );
    }

//  return ( m_pGun->m_iM16Gun );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAmmos6( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iRockets );
    }

//  return ( m_pGun->m_iRockets );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAmmos7a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iGrenades );
    }

//  return ( m_pGun->m_iGrenades );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAmmos8a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iHarpoon );
    }

//  return ( m_pGun->m_iHarpoon );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAmmos2 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iDesertEagleAmmos = atoi ( szString );
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
void CTR1SaveGame::SetAmmos3 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iUziAmmos = atoi ( szString );
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
void CTR1SaveGame::SetAmmos4a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iRiotGunAmmos = atoi ( szString );
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
void CTR1SaveGame::SetAmmos5 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      m_pBuffer->trSingle.m_iM16Gun = atoi ( szString );
    }

//  m_pGun->m_iM16Gun = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAmmos6 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      m_pBuffer->trSingle.m_iRockets = atoi ( szString );
    }

//  m_pGun->m_iRockets = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAmmos7a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      m_pBuffer->trSingle.m_iGrenades = atoi ( szString );
    }

//  m_pGun->m_iGrenades = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAmmos8a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      m_pBuffer->trSingle.m_iHarpoon = atoi ( szString );
    }

//  m_pGun->m_iHarpoon = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetUnlimitedAmmos(int iMask)
{
    m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetUnlimitedAmmos()
{
    return m_pBuffer->cUnlimited;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::Valid()
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
void CTR1SaveGame::SetInvalid()
{
    m_pGun = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon1 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskPistol )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon4 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskShotGun )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon2 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskMagnum )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon3 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskUzi )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon5 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskMP5 )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon8 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskHarpoon )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon7 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskGrenade )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::CheckWeapon6 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskGrenade )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskCompass;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskCompass;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskCompass ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskPistol;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskPistol;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskPistol ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskShotGun;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskShotGun;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskShotGun ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskMagnum;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskMagnum;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskMagnum ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskUzi;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskUzi ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskMP5;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskMP5;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskMP5 ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskGrenade;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskGrenade ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskGrenade;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskGrenade ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR1SaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskHarpoon;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap   |= iMaskHarpoon;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap    &= ( iMaskHarpoon ^ TR1_MASK_ANY );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetGunAmmos ( )
{
    return ( m_iGunAmmos );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->trSingle.cSmallPack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->trSingle.cLargePack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetFlares ( int iX )
{
//  return ( m_pBuffer->trSingle.cFlare );

    return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetAir ( )
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
void CTR1SaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->trSingle.cSmallPack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetLargeMedipak ( const char *szString, int iX )
{
    m_pBuffer->trSingle.cLargePack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetFlares ( const char *szString, int iX )
{
//  m_pBuffer->trSingle.cFlare = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAir ( const char *szString )
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
char *CTR1SaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetCurrentSecrets ()
{
    return ( -1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetCurrentSecrets ( char *szString, int iX )
{

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAllSecrets ( )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetLaraState ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::IsLaraBurning ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetLaraBurning (int state)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetLaraNoBurning (int state)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetMap1 ()
{
    return ( m_pBuffer->cObject_P1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetMap2 ()
{
    return ( m_pBuffer->cObject_P2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetItem1 ()
{
    return ( m_pBuffer->cObjectP1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetItem2 ()
{
    return ( m_pBuffer->cObjectP2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetItem3 ()
{
    return ( m_pBuffer->cObjectP3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetItem4 ()
{
    return ( m_pBuffer->cObjectP4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetKey1 ()
{
    return ( m_pBuffer->cObjectK1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetKey2 ()
{
    return ( m_pBuffer->cObjectK2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetKey3 ()
{
    return ( m_pBuffer->cObjectK3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetKey4 ()
{
    return ( m_pBuffer->cObjectK4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetTreasure1 ( int iX )
{
    return ( m_pBuffer->trSingle.cNbObject1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetTreasure2 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetTreasure3 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetTreasure4 ( int iX )
{
    return -1;
}

void CTR1SaveGame::SetMap1 ( char *szString )
{
    m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetMap2 ( char *szString )
{
    m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetItem1 ( char *szString )
{
    m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetItem2 ( char *szString )
{
    m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetItem3 ( char *szString )
{
    m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetItem4 ( char *szString )
{
    m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetKey1 ( char *szString )
{
    m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetKey2 ( char *szString )
{
    m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetKey3 ( char *szString )
{
    m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetKey4 ( char *szString )
{
    m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetTreasure1 ( char *szString, int iX )
{
    m_pBuffer->trSingle.cNbObject1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR1SaveGame::GetMaximumSecrets ( int iX )
{
    return ( -1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR1SaveGame::SetGunAmmos ( const char *szGunAmmos )
{
    // Do Nothing.
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR1SaveGame::getBufferAddress ()
{
        return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR1SaveGame::getBufferBackupAddress ()
{
        return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR1SaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR1SaveGame::BufferModified ()
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
void CTR1SaveGame::discard ()
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
const TR_POSITION *CTR1SaveGame::GetPosition ( )
{
    GetRealHealth ();

    if ( m_pRealHealth != NULL )
    {
        long dwDelta = ( long ) ( (BYTE* )m_pRealHealth - (BYTE* )m_pBuffer );
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
        for ( int i = 0x500; i < 0x1000; i++ )
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
BOOL CTR1SaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
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

