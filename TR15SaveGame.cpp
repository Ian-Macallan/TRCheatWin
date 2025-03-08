// TUBSaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR15SaveGame.h"
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
static unsigned TUBPositions [ ] =
{
    0xdc3-0x86,     /* 01 */
    0x1117-0x86,    /* 02 */
    0xc26-0x86,     /* 03 */
    0x11d6-0x86,    /* 04 */
    0x0,        /* 05 */
    0x0,        /* 06 */
    0x0,        /* 07 */
    0x0,        /* 08 */
    0x0,        /* 09 */
    0x0,        /* 10 */
    0x0,        /* 11 */
    0x0,        /* 12 */
    0x0,        /* 13 */
    0x0,        /* 14 */
    0x0,        /* 15 */
    0x0,        /* 16 */
    0x0,        /* 17 */
    0x0,        /* 18 */
    0x0,        /* 19 */
    0x0,        /* 20 */
    0x00,       /* 0 */
    0x00,       /* 0 */
    0x00,       /* 0 */
    0x00,       /* 0 */
    0x00,       /* 0 */
};

//
/////////////////////////////////////////////////////////////////////////////
// CTUBSaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTUBSaveGame, CTR123SaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTUBSaveGame::CTUBSaveGame()
{

    m_iVersion          = 15;

    m_iSaveLength       = TUBLEVELMAXSIZE;
    m_iMaxLevel         = TUBMAXLEVEL;

    iMaskCompass        = 0x01;
    iMaskPistol         = 0x02;
    iMaskMagnum         = 0x04;
    iMaskUzi            = 0x08;
    iMaskShotGun        = 0x10;
    iMaskHarpoon        = 0x20;
    iMaskMP5            = 0x40;
    iMaskGrenade        = 0x80;

    m_pLife             = NULL;

    m_pGun              = NULL;

    m_pBuffer           = new ( TUBSAVE );
    ZeroMemory ( m_pBuffer, sizeof(TUBSAVE) );

    m_pBufferBackup     = new ( TUBSAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TUBSAVE) );

    m_iSubVersion       = 5;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTUBSaveGame::~CTUBSaveGame()
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
// CTUBSaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTUBSaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::ReadSavegame( const char *pFilename )
{

    FILE                    *hFile;
    size_t                  uLenBuffer;

    char                    szEmpty [ 1 ];

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
    InitStatus ();

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TUBSAVE ) );

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
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < TUBLEVELMINSIZE  || m_iSaveLength > TUBLEVELMAXSIZE ) )
    {
        AddToStatus ( "Internal error in length." );
        CloseOneFile ( &hFile );
        return 0;
    }

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TUBSAVE ) );
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

    memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TUBSAVE) );

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
void CTUBSaveGame::writeSaveGame()
{
    FILE                    *hFile;
    size_t                  uLenBuffer;

    //
    InitStatus ();

    /*
     *      Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( ! ( m_pBuffer->trSingle.cGunBitmap & iMaskMagnum) )
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

    //
    Backup_Savegame();

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
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < TUBLEVELMINSIZE  || m_iSaveLength > TUBLEVELMAXSIZE ) )
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
void CTUBSaveGame::RetrieveInformation( const char *pFilename )
{
    char            *pStartAddress;
    char            *pGunAddress;
    char            *pAddress;

    unsigned        iPosition;
    int             iX;
    int             iPos;
    int             iCount;

    int             bExactFound;

    TUBAMMOS        *pGun;

    /*
     *  Set address to error.
     */
    m_pGun          = NULL;

    m_iSubVersion   = 5;

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
        do
        {
            pGun = SearchGunStructure ( m_iGunAmmos, m_pBuffer->trSingle.cGunBitmap, &iPos );

            if ( pGun != NULL )
            {
                iCount++;

                m_pGun = pGun;
                AddToStatus ( "Data loaded from the saved game.");

                /*
                 *      Get Level.
                 */
                iX = getLevelIndex ();

                /*
                 *      Test if it it the same as in the table.
                 */
                pStartAddress = ( char * ) m_pBuffer;
                pGunAddress = pStartAddress + TUBPositions [ iX ];
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
            pGunAddress   = pStartAddress + TUBPositions [ iX ];
            m_pGun = (TUBAMMOS * ) pGunAddress;
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
            pGunAddress   = pStartAddress + TUBPositions [ iX ];
            m_pGun = (TUBAMMOS * ) pGunAddress;

            AddFormatToStatus ( "Unable to find something in the file: Setting the address %x.",
                TUBPositions [ iX ] );

            m_pGun->m_iGunAmmos = m_iGunAmmos;
            m_pGun = NULL;

            return;
        }
        else
        {
            AddFormatToStatus ("%d good position(s) found.", iCount );
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

        pGunAddress = pStartAddress + TUBPositions [ iX ];
        if ( ( char * ) m_pGun != pGunAddress )
        {
            pAddress = ( char * ) m_pGun;
            iPosition = (unsigned) ( pAddress - pStartAddress );
            AddFormatToStatus ( 
                "The %d address(es) differ(s): Reference is at the address %lx instead of %lx.",
                iCount, TUBPositions [ iX ], iPosition );
            if ( false )
            {
                AddFormatToStatus ( 
                    "The %d address(es) differ(s): Setting the address %lx instead of %lx.",
                    iCount, TUBPositions [ iX ], iPosition );
                m_pGun = ( TUBAMMOS * ) pGunAddress;
                m_pGun->m_iGunAmmos = m_iGunAmmos;
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TUBAMMOS *CTUBSaveGame::SearchGunStructure ( WORD m_iGunAmmos, WORD gunBitmap, int *iPos )
{
        int                     iX;
        BYTE                   *pString;
        TUBAMMOS                *pGun;
        
        if ( *iPos == -1 )
        {
            iX = TUBBUFFEREND - sizeof ( TUBAMMOS );
        }
        else
        {
            iX = *iPos;
        }

        while ( iX >= 0 )
        {
            pString = & ( m_pBuffer->szRemain [ iX ] );
            pGun = ( TUBAMMOS * )(  pString );
            if (    ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos     ) )
                 || ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos + 1 ) )
                 || ( pGun->m_iGunAmmos == ( unsigned short )( m_iGunAmmos - 1 ) ) )
            {
                if ( CheckIfAmmosMatch ( pGun, gunBitmap ) )
                {
                    *iPos = iX - 1;
                    return ( pGun );
                }
            }

            iX--;
        }

        *iPos = -1;
        pGun = NULL;

        return ( pGun );


}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::CheckIfAmmosMatch ( TUBAMMOS *pGun, WORD gunBitmap )
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
        if ( pGun->m_iUzis     != m_iUzis  )
        {
            return 0;
        }
    }
    else
    {
        if ( pGun->m_iUzis != 0 && pGun->m_iUzis != m_iUzis)
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
void CTUBSaveGame::RetrieveHeader()
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
void CTUBSaveGame::GetAmmosValues()
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
        m_iDesertEagle = m_pBuffer->trTable [ iX ].m_iDesertEagle;
        m_iRiotGun     = m_pBuffer->trTable [ iX ].m_iRiotGun;
        m_iUzis        = m_pBuffer->trTable [ iX ].m_iUzis;

        m_iGunAmmos      = m_pBuffer->trSingle.m_iGunAmmos;
        m_iDesertEagle = m_pBuffer->trSingle.m_iDesertEagle;
        m_iRiotGun     = m_pBuffer->trSingle.m_iRiotGun;
        m_iUzis        = m_pBuffer->trSingle.m_iUzis;

//      m_iGrenades    = m_pBuffer->trTable [ iX ].m_iGrenades;
//      m_iRockets     = m_pBuffer->trTable [ iX ].m_iRockets;
//      m_iHarpoon     = m_pBuffer->trTable [ iX ].m_iHarpoon;
//      m_iMP5         = m_pBuffer->trTable [ iX ].m_iM16Gun;


}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::ConvertSecret(int iSecret)
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
int CTUBSaveGame::ConvertSecretBack(int iSecret)
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
void CTUBSaveGame::GetDetailedInfo (    char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTitle)
{
    strcpy_s ( szGame, iSize, "TR1ub" );
    strcpy_s ( szTitle, iSizeTitle, m_pBuffer->szSavename );
    *iGame          = m_pBuffer->iSaveNumber;
    *iLevel         = m_pBuffer->cLevel + 1;
    m_iSubVersion   = 5;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTUBSaveGame::GetStatus()
{
    return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::getLevel()
{
    return m_pBuffer->cLevel + 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::getLevelIndex()
{
    return m_pBuffer->cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos1(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iGunAmmos );
    }

    return ( m_pGun->m_iGunAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos2(int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iDesertEagle );
    }

    return ( m_pGun->m_iDesertEagle );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos3( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iUzis );
    }

    return ( m_pGun->m_iUzis );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos4a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        return ( m_pBuffer->trSingle.m_iRiotGun );
    }

    return ( m_pGun->m_iRiotGun );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos5( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iM16Gun );
    }

//  return ( m_pGun->m_iM16Gun );

    return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos6( int iX )
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
int CTUBSaveGame::GetAmmos7a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iGrenades );
    }

//  return ( m_pGun->m_iGrenades );

    return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAmmos8a( int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( m_pBuffer->trSingle.m_iHarpoon );
    }

//  return ( m_pGun->m_iHarpoon );

    return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAmmos2 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iDesertEagle = atoi ( szString );
    }

    m_pGun->m_iDesertEagle = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAmmos3 ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iUzis = atoi ( szString );
    }

    m_pGun->m_iUzis = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAmmos4a ( const char *szString, int iX )
{
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
        m_pBuffer->trSingle.m_iRiotGun = atoi ( szString );
    }

    m_pGun->m_iRiotGun = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAmmos5 ( const char *szString, int iX )
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
void CTUBSaveGame::SetAmmos6 ( const char *szString, int iX )
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
void CTUBSaveGame::SetAmmos7a ( const char *szString, int iX )
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
void CTUBSaveGame::SetAmmos8a ( const char *szString, int iX )
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
void CTUBSaveGame::SetUnlimitedAmmos(int iMask)
{
    m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetUnlimitedAmmos()
{
    return ( m_pBuffer->cUnlimited );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::Valid()
{
    if ( m_pGun != NULL )
    {
        return ( 1 );
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetInvalid()
{
    m_pGun = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::CheckWeapon1 ( int iX )
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
int CTUBSaveGame::CheckWeapon4 ( int iX )
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
int CTUBSaveGame::CheckWeapon2 ( int iX )
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
int CTUBSaveGame::CheckWeapon3 ( int iX )
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
int CTUBSaveGame::CheckWeapon5 ( int iX )
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
int CTUBSaveGame::CheckWeapon8 ( int iX )
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
int CTUBSaveGame::CheckWeapon7 ( int iX )
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
int CTUBSaveGame::CheckWeapon6 ( int iX )
{
    if ( m_pBuffer->trSingle.cGunBitmap & iMaskGrenade  )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskCompass;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap = iMaskCompass;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskCompass ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskPistol;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskPistol;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskPistol ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskShotGun;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskShotGun;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskShotGun ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskMagnum;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskMagnum;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskMagnum ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskUzi;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskUzi;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskUzi ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskMP5;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskMP5;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskMP5 ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskHarpoon;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskHarpoon;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskHarpoon ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskGrenade;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskGrenade ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTUBSaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    unsigned char old = m_pBuffer->trSingle.cGunBitmap & iMaskGrenade;
    if ( ! bChange ) return old;

    if ( bChange ) m_pBuffer->trSingle.cGunBitmap |= iMaskGrenade;
    if ( ! bAdd ) m_pBuffer->trSingle.cGunBitmap &= ( iMaskGrenade ^ 0xff );
    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetGunAmmos ( )
{
    return ( m_iGunAmmos );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetSmallMedipak ( int iX )
{
    return ( m_pBuffer->trSingle.cSmallPack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetLargeMedipak ( int iX )
{
    return ( m_pBuffer->trSingle.cLargePack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetFlares ( int iX )
{
//  return ( m_pBuffer->trSingle.cFlare );

    return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetAir ( )
{
    return ( m_pGun->iAir );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetSmallMedipak ( const char *szString, int iX )
{
     m_pBuffer->trSingle.cSmallPack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetLargeMedipak ( const char *szString, int iX )
{
    m_pBuffer->trSingle.cLargePack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetFlares ( const char *szString, int iX )
{
//  m_pBuffer->trSingle.cFlare = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAir ( const char *szString )
{
    m_pGun->iAir = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTUBSaveGame::GetSaveName ()
{
    return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetSaveNumber ()
{
    return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetCurrentSecrets ()
{
    return ( -1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetCurrentSecrets ( char *szString, int iX )
{

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAllSecrets ( )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetLaraState ()
{
    return 0;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::IsLaraBurning ()
{
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetLaraBurning (int state)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetLaraNoBurning (int state)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetMap1 ()
{
    return ( m_pBuffer->cObject_P1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetMap2 ()
{
    return ( m_pBuffer->cObject_P2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetItem1 ()
{
    return ( m_pBuffer->cObjectP1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetItem2 ()
{
    return ( m_pBuffer->cObjectP2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetItem3 ()
{
    return ( m_pBuffer->cObjectP3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetItem4 ()
{
    return ( m_pBuffer->cObjectP4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetKey1 ()
{
    return ( m_pBuffer->cObjectK1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetKey2 ()
{
    return ( m_pBuffer->cObjectK2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetKey3 ()
{
    return ( m_pBuffer->cObjectK3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetKey4 ()
{
    return ( m_pBuffer->cObjectK4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetTreasure1 ( int iX )
{
    return ( m_pBuffer->trSingle.cNbObject1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetTreasure2 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetTreasure3 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetTreasure4 ( int iX )
{
    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetMap1 ( char *szString )
{
    m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetMap2 ( char *szString )
{
    m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetItem1 ( char *szString )
{
    m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetItem2 ( char *szString )
{
    m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetItem3 ( char *szString )
{
    m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetItem4 ( char *szString )
{
    m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetKey1 ( char *szString )
{
    m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetKey2 ( char *szString )
{
    m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetKey3 ( char *szString )
{
    m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetKey4 ( char *szString )
{
    m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetTreasure1 ( char *szString, int iX )
{
    m_pBuffer->trSingle.cNbObject1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTUBSaveGame::GetMaximumSecrets ( int iX )
{
    return ( -1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTUBSaveGame::SetGunAmmos ( const char *szGunAmmos )
{
    // Do Nothing.
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTUBSaveGame::getBufferAddress ()
{
        return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTUBSaveGame::getBufferBackupAddress ()
{
        return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTUBSaveGame::getBufferLength()
{
    return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTUBSaveGame::BufferModified ()
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
void CTUBSaveGame::discard ()
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
const TR_POSITION *CTUBSaveGame::GetPosition ( )
{
    GetLife ();

    if ( m_pLife != NULL )
    {
        long dwDelta = (long)((BYTE* )m_pLife - (BYTE* )m_pBuffer);
        TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pLife - TR_POSITION_OFFSET );
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
        for ( int i = 0xB00; i < 0x1200; i++ )
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
BOOL CTUBSaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
    GetLife ();

    //  Check if position was good
    if ( GetPosition ( ) == NULL )
    {
        return FALSE;
    }

    if ( m_pLife != NULL )
    {
        long dwDelta = (long)( (BYTE* )m_pLife - (BYTE* )m_pBuffer );
        TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pLife - TR_POSITION_OFFSET );
        position->dwWestToEast      = dwWestToEast;
        position->dwVertical        = dwVertical;
        position->dwSouthToNorth    = dwSouthToNorth;
        position->wOrientation      = wDirection;
        position->wRoom             = wRoom;

        return TRUE;
    }

    return FALSE;
}

