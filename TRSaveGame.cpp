// TRSaveGame.cpp : implementation file
//

#include "stdafx.h"

#include <afxwin.h>

#include "TRXCHEATWIN.h"
#include "TRSaveGame.h"
#include "TRXGlobal.h"

#include "TR10SaveGame.h"
#include "TR15SaveGame.h"
#include "TR2SaveGame.h"
#include "TR3SaveGame.h"
#include "TR4SaveGame.h"
#include "TR4NGSaveGame.h"
#include "TR5SaveGame.h"

#include "TRXMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CTRSaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRSaveGame, CObject)

CTRSaveGame *CTRSaveGame::m_pInstance = NULL;
char        CTRSaveGame::m_szBuffer [ MAX_SAVELENGTH ] = "";
char        CTRSaveGame::m_szBufferBackup [ MAX_SAVELENGTH ] = "";

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame::CTRSaveGame()
{
    m_iVersion          = 0;
    m_iSubVersion       = 0;

    m_iHarpoon          = 0;
    m_iCrossbow         = 0;
    m_iRockets          = 0;
    m_iGrenades         = 0;
    m_iHK               = 0;
    m_iMP5              = 0;
    m_iRiotGun          = 0;
    m_iShotgun          = 0;
    m_iUzis             = 0;
    m_iDesertEagle      = 0;
    m_iRevolver         = 0;

    m_iSaveLength       = 0;
    m_iMaxLevel         = 0;

    Reset();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::StaticReset()
{
    ZeroMemory ( m_szBuffer, sizeof(m_szBuffer) );
    ZeroMemory ( m_szBufferBackup, sizeof(m_szBufferBackup) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::Reset()
{
    ZeroMemory ( m_Filename, sizeof(m_Filename) );
    ZeroMemory ( m_FilenameBak, sizeof(m_FilenameBak) );
    ZeroMemory ( m_Status, sizeof(m_Status) );

    m_iGunAmmos         = 0;

    m_iLife             = 0;
    m_pLife             = NULL;

}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame::~CTRSaveGame()
{
    m_pInstance = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetInstance( BOOL bCreate )
{
    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::I(BOOL bCreate)
{
    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRSaveGame::IsValid()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->Valid();
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR1Instance()
{
    CTR1SaveGame *pInstance = dynamic_cast<CTR1SaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR1SaveGame();
    }
    
    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTUBInstance()
{
    CTUBSaveGame *pInstance = dynamic_cast<CTUBSaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTUBSaveGame();
    }
    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR2Instance()
{
    CTR2SaveGame *pInstance = dynamic_cast<CTR2SaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR2SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR3Instance()
{
    CTR3SaveGame *pInstance = dynamic_cast<CTR3SaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR3SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR4Instance()
{
    CTR4SaveGame *pInstance = dynamic_cast<CTR4SaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR4SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR4NGInstance()
{
    CTR4NGSaveGame *pInstance = dynamic_cast<CTR4NGSaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR4NGSaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRSaveGame *CTRSaveGame::GetTR5Instance()
{
    CTR5SaveGame *pInstance = dynamic_cast<CTR5SaveGame *>( m_pInstance );
    if ( pInstance == NULL )
    {
        if ( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
        m_pInstance = new CTR5SaveGame();
    }

    return m_pInstance;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRSaveGame::IsBufferModified()
{
    if ( m_pInstance != NULL )
    {
        if ( ! m_pInstance->Valid() )
        {
            return FALSE;
        }

        return m_pInstance->BufferModified();
    }

    return FALSE;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// CTRSaveGame serialization

//
/////////////////////////////////////////////////////////////////////////////
// CTRSaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetInfo (  char *szGame, size_t iSize, int *iGame, int *iLevel, char *szTitle, size_t iSizeTitle )
{

    //
    CTRXGlobal::m_iUnchecked = theApp.GetProfileInt( PROFILE_SETTING, PROFILE_UNCHECKED, 0 );

    //
    int         iVersion    = 0;

    /*
     *  Size of saved games.
     *
        10675       TR1     SAVEGAME.*
        10435       TRUB    SAVEUB.*
        7442        TR2     SAVEGAME.*
        13913       TR2     SAVEGAME.*
        16384       TR4     SAVEGAME.*
     *
     */

    strcpy_s ( szGame, sizeof(szGame), "???" );
    *iGame = 0;
    strcpy_s ( szTitle, iSizeTitle,  "Unknown Level" );

    //
    if ( m_pInstance != NULL )
    {
        m_pInstance->GetDetailedInfo ( szGame, iSize, iGame, iLevel, szTitle, iSizeTitle);
        iVersion = GetVersion();
    }

    return ( iVersion );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::InstanciateVersion (const char *szFilename )
{
    FILE        *hFile      = NULL;

    size_t  uLen        = 0;

    int         iVersion    = 0;

    /*
     *  Size of saved games.
     *
    10675       TR1     SAVEGAME.*
    10435       TRUB    SAVEUB.*
    7442        TR2     SAVEGAME.*
    13913       TR2     SAVEGAME.*
    16384       TR4     SAVEGAME.*
    *
    */

    //
    hFile = NULL;
    fopen_s ( &hFile,  szFilename, "rb" );
    if ( hFile != NULL )
    {
        StaticReset();

        //
        ZeroMemory ( m_szBuffer, sizeof ( m_szBuffer ) );
        uLen = fread ( m_szBuffer, 1, sizeof ( m_szBuffer ), hFile );
        fclose ( hFile );

        //
        switch ( uLen )
        {
            case TR1LEVELMINSIZE:
            {
                iVersion = 10;
                CTR1SaveGame *pGame = dynamic_cast<CTR1SaveGame *>( GetTR1Instance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR1SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR1SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            case TUBLEVELMINSIZE :
            case TUBLEVELMAXSIZE :
            {
                iVersion = 15;
                CTUBSaveGame *pGame = dynamic_cast<CTUBSaveGame *>( GetTUBInstance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TUBSAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TUBSAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }

                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            case TR2LEVELMINSIZE :
            case TR2LEVELMAXSIZE :
            {
                iVersion = 20;
                CTR2SaveGame *pGame = dynamic_cast<CTR2SaveGame *>( GetTR2Instance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR2SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR2SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            case TR3LEVELMINSIZE :
            case TR3LEVELMAXSIZE :
            {
                iVersion = 30;
                CTR3SaveGame *pGame = dynamic_cast<CTR3SaveGame *>( GetTR3Instance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR3SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR3SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            case TR4LEVELMINSIZE :
            case TR4LEVELMAXSIZE :
            {
                iVersion    = 40;
                CTR4SaveGame *pGame = dynamic_cast<CTR4SaveGame *>( GetTR4Instance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR4SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR4SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            case TR5LEVELSIZE :
            {
                iVersion = 50;
                CTR5SaveGame *pGame = dynamic_cast<CTR5SaveGame *>( GetTR5Instance() );
                if ( pGame != NULL )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR5SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR5SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                // m_pInstance->RetrieveInformation( szFilename );
                break;
            }

            default:
            {
                if ( uLen >= CTRXGlobal::m_iMinNGSize && uLen <= CTRXGlobal::m_iMaxNGSize )
                {
                    //
                    //  The last eight byte if the file is
                    //  4E474C4526120000    NGLE&...
                    char *pSignature = (char* ) m_szBuffer + uLen - 8;
                    if ( memcmp ( pSignature, "NGLE", 4 ) != 0  )
                    {
                        CTRXMessageBox::ShowMessage( "Load Savegame Error", "File Signature does not match NGLE");
                        return -1;
                    }

                    //
                    iVersion    = 49;
                    CTR4NGSaveGame *pGame = dynamic_cast<CTR4NGSaveGame *>( GetTR4NGInstance() );
                    if ( pGame != NULL )
                    {
                        pGame->Reset();
                        memcpy_s ( pGame->getBufferAddress(), sizeof(TR4NGSAVE), m_szBuffer, uLen );
                        memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR4NGSAVE), m_szBuffer, uLen );
                        pGame->SetBufferLength ( uLen );
                    }
                }
                else
                {
                    //
                    CTRXMessageBox::ShowMessage( "Load Savegame Error", "File size does not match any game format");
                    return -1;
                }
                break;
            }
        }
    }

    return iVersion;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetVersion ()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->m_iVersion;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetFullVersion ()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->getFullVersion();
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetLevelIndex()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->getLevelIndex();
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetLevel()
{
    if ( m_pInstance != NULL )
    {
        return m_pInstance->getLevel();
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::WriteSaveGame()
{
    if ( m_pInstance != NULL )
    {
        m_pInstance->writeSaveGame();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::Discard()
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
char *CTRSaveGame::GetBufferAddress()
{
    if ( m_pInstance != NULL )
    {
        CTR1SaveGame *pTR1Instance = dynamic_cast<CTR1SaveGame *>(m_pInstance);
        if ( pTR1Instance != NULL )
        {
            return pTR1Instance->getBufferAddress();
        }
        CTUBSaveGame *pTUBInstance = dynamic_cast<CTUBSaveGame *>(m_pInstance);
        if ( pTUBInstance != NULL )
        {
            return pTUBInstance->getBufferAddress();
        }
        CTR2SaveGame *pTR2Instance = dynamic_cast<CTR2SaveGame *>(m_pInstance);
        if ( pTR2Instance != NULL )
        {
            return pTR2Instance->getBufferAddress();
        }
        CTR3SaveGame *pTR3Instance = dynamic_cast<CTR3SaveGame *>(m_pInstance);
        if ( pTR3Instance != NULL )
        {
            return pTR3Instance->getBufferAddress();
        }
        CTR4SaveGame *pTR4Instance = dynamic_cast<CTR4SaveGame *>(m_pInstance);
        if ( pTR4Instance != NULL )
        {
            return pTR4Instance->getBufferAddress();
        }
        CTR5SaveGame *pTR5Instance = dynamic_cast<CTR5SaveGame *>(m_pInstance);
        if ( pTR5Instance != NULL )
        {
            return pTR5Instance->getBufferAddress();
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTRSaveGame::GetBufferBackupAddress()
{
    if ( m_pInstance != NULL )
    {
        CTR1SaveGame *pTR1Instance = dynamic_cast<CTR1SaveGame *>(m_pInstance);
        if ( pTR1Instance != NULL )
        {
            return pTR1Instance->getBufferBackupAddress();
        }
        CTUBSaveGame *pTUBInstance = dynamic_cast<CTUBSaveGame *>(m_pInstance);
        if ( pTUBInstance != NULL )
        {
            return pTUBInstance->getBufferBackupAddress();
        }
        CTR2SaveGame *pTR2Instance = dynamic_cast<CTR2SaveGame *>(m_pInstance);
        if ( pTR2Instance != NULL )
        {
            return pTR2Instance->getBufferBackupAddress();
        }
        CTR3SaveGame *pTR3Instance = dynamic_cast<CTR3SaveGame *>(m_pInstance);
        if ( pTR3Instance != NULL )
        {
            return pTR3Instance->getBufferBackupAddress();
        }
        CTR4SaveGame *pTR4Instance = dynamic_cast<CTR4SaveGame *>(m_pInstance);
        if ( pTR4Instance != NULL )
        {
            return pTR4Instance->getBufferBackupAddress();
        }
        CTR5SaveGame *pTR5Instance = dynamic_cast<CTR5SaveGame *>(m_pInstance);
        if ( pTR5Instance != NULL )
        {
            return pTR5Instance->getBufferBackupAddress();
        }
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTRSaveGame::RelativeAddress( const void *pAddress )
{
    void *pBuffer = GetBufferAddress();
    if ( pBuffer != NULL )
    {
        return CTRXTools::RelativeAddress ( pAddress, pBuffer );
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::Backup_Savegame()
{
    //  32 Backup file
    static char szOriginal [ MAX_PATH ];
    static char szRenamed [ MAX_PATH ];

    //  Four Backup Files
    for ( int i = CTRXGlobal::m_iTRBackup - 1; i >= 1; i-- )
    {
        sprintf_s ( szOriginal, sizeof(szOriginal), "%s.bak.%d", m_Filename, i - 1 );
        sprintf_s ( szRenamed, sizeof(szRenamed), "%s.bak.%d", m_Filename, i );
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
    if ( CTRXGlobal::m_iTRBackup >= 1 )
    {
        sprintf_s ( szRenamed, sizeof(szRenamed), "%s.bak.%d", m_Filename, 0 );
        if ( PathFileExists ( szRenamed ) )     //  Normally already renamed
        {
            _unlink ( szRenamed );
        }

        if ( PathFileExists ( m_Filename ) )
        {
            rename ( m_Filename, szRenamed );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TRLIFE *CTRSaveGame::GetLifeAddress ()
{
    if ( m_pLife != NULL )
    {
        return m_pLife;
    }

    //
    char        *pBuffer;
    TRLIFE      *pLife;
    int         iX;

    m_pLife     = NULL;

    /*
     *      Initializations.
     */
    pBuffer = getBufferAddress ();
    if ( pBuffer == NULL )
    {
        return NULL;
    }

    iX    = m_iSaveLength;

    /*
     *      Search Life.
     */
    while ( iX > sizeof ( TRLIFE ) )
    {
        pLife = ( TRLIFE * ) pBuffer;

        if (    ( pLife->iOne == 0x0002 && pLife->iTwo == 0x0002 /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x0067 ) ||
                ( pLife->iOne == 0x0008 && pLife->iTwo == 0x0008 /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x0112 ) ||      // Quad
                ( pLife->iOne == 0x000d && pLife->iTwo == 0x000d /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x006c ) ||
                ( pLife->iOne == 0x0012 && pLife->iTwo == 0x0012 /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x0057 ) )
        {
            m_pLife = pLife;
            m_iLife = pLife->iLife;
            return m_pLife;
        }
        else
        {
            iX--;
            pBuffer++;
        }
    }

    /*
     *      Second Try.
     */
    iX          = m_iSaveLength;
    pBuffer     = getBufferAddress ();

    while ( iX > sizeof ( TRLIFE ) )
    {
        pLife = ( TRLIFE * ) pBuffer;

        if (    ( pLife->iOne == 0x0001 && pLife->iTwo == 0x0002 /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x000a ) ||
                ( pLife->iOne == 0x0001 && pLife->iTwo == 0x0002 /* && pLife->cFiller1 == 0 */ && pLife->iThree == 0x0008 )   )
        {
            m_pLife = pLife;
            m_iLife = pLife->iLife;
            return m_pLife;
        }
        else
        {
            iX--;
            pBuffer++;
        }
    }

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetLife ()
{
    TRLIFE      *pLife  = GetLifeAddress();
    if ( pLife != NULL )
    {
        return ( pLife->iLife );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::SetLife ( const char *szLife )
{
    if ( m_pLife != NULL )
    {
        m_pLife->iLife = atoi ( szLife );
    }

    return;
}

