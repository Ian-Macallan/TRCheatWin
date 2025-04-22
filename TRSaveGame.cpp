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
TR123_INDICATORS IndicatorsTR123Table1 [ MAX_INDICATORS ] =
{
    {   FALSE,  0x0002, 0x0002, 0x0000, 0x0067, FALSE,  0,  "Standing" },
    {   FALSE,  0x0008, 0x0008, 0x0000, 0x0112, FALSE,  1,  "Quad Bike" },
    {   FALSE,  0x000d, 0x000d, 0x0000, 0x006c, FALSE,  1,  "Swimming" },
    {   FALSE,  0x0012, 0x0012, 0x0000, 0x0057, FALSE,  1,  "Indicator 2" },
    {   FALSE,  0x0021, 0x0021, 0x0000, 0x006E, TRUE,   1,  "Swimming" },       //  Use W3
    {   FALSE,  0x0018, 0x0018, 0x0000, 0x0046, TRUE,   1,  "Sliding" },        //  Use W3
    {   FALSE,  0x000d, 0x0012, 0x0000, 0x006c, TRUE,   1,  "Underwater" },     //  Use W3
    {   FALSE,  0x0002, 0x0002, 0x0047, 0x00bd, TRUE,   1,  "Standing" },       //  Use W3

    {   FALSE,  0x0002, 0x0002, 0x0000, 0x000B, TRUE,   9,  "Standing" },       //  Use W3
    {   TRUE,   0xffff, 0xffff, 0xffff, 0xffff, TRUE,   0,  "End" },            //  End
};
int IndicatorsTR123Table1Count = sizeof(IndicatorsTR123Table1)/sizeof(TR123_INDICATORS);

//
TR123_INDICATORS IndicatorsTR123Table2 [ MAX_INDICATORS ] =
{
    {   FALSE,  0x000f, 0x000f, 0x0000, 0x0173, TRUE,   2,  "Quad Bike" },      //  Use W3
    {   FALSE,  0x0001, 0x0001, 0x0000, 0x0163, TRUE,   2,  "Quad Bike" },      //  Use W3

    {   FALSE,  0x0001, 0x0002, 0x0000, 0x000a, FALSE,  2,  "Indicator 10" },
    {   FALSE,  0x0001, 0x0002, 0x0000, 0x0008, FALSE,  2,  "Indicator 11" },
    {   TRUE,   0xffff, 0xffff, 0xffff, 0xffff, TRUE,   0,  "End" },            //  End
};
int IndicatorsTR123Table2Count = sizeof(IndicatorsTR123Table2)/sizeof(TR123_INDICATORS);

//
static int LastGoodHealth = 0;

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

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

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

    m_iGunAmmos             = 0;

    m_wRealHealth           = 0;
    m_pRealHealth           = NULL;

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

    size_t  uLen            = 0;

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
        CloseOneFile ( &hFile );

        //
        switch ( CTRXGlobal::m_ForceSaveGame )
        {
            case FORCE_TR1 :
            {
                iVersion = GAME_TR10;
                CTR1SaveGame *pGame = dynamic_cast<CTR1SaveGame *>( GetTR1Instance() );
                if ( pGame != NULL && uLen <= sizeof(TR1SAVE) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR1SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR1SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            case FORCE_TUB :
            {
                iVersion = GAME_TR15;
                CTUBSaveGame *pGame = dynamic_cast<CTUBSaveGame *>( GetTUBInstance() );
                if ( pGame != NULL && uLen <= sizeof(TUBSAVE) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TUBSAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TUBSAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            case FORCE_TR2 :
            case FORCE_TR2G :
            {
                iVersion = GAME_TR20;
                CTR2SaveGame *pGame = dynamic_cast<CTR2SaveGame *>( GetTR2Instance() );
                if ( pGame != NULL && uLen <= sizeof(TR2SAVE) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR2SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR2SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            case FORCE_TR3 :
            case FORCE_TR3G :
            {
                iVersion = GAME_TR30;
                CTR3SaveGame *pGame = dynamic_cast<CTR3SaveGame *>( GetTR3Instance() );
                if ( pGame != NULL && uLen <= sizeof(TR3SAVE) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR3SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR3SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            case FORCE_TR4 :
            {
                iVersion    = GAME_TR40;
                CTR4SaveGame *pGame = dynamic_cast<CTR4SaveGame *>( GetTR4Instance() );
                if ( pGame != NULL && uLen <= sizeof(TR4SAVE) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR4SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR4SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            case FORCE_TR4NG :
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
                    iVersion    = GAME_TR49;
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

            case FORCE_TR5 :
            {
                iVersion = GAME_TR50;
                CTR5SaveGame *pGame = dynamic_cast<CTR5SaveGame *>( GetTR5Instance() );
                if ( pGame != NULL && uLen <= sizeof(TR5SAVE ) )
                {
                    pGame->Reset();
                    memcpy_s ( pGame->getBufferAddress(), sizeof(TR5SAVE), m_szBuffer, uLen );
                    memcpy_s ( pGame->getBufferBackupAddress(), sizeof(TR5SAVE), m_szBuffer, uLen );
                    pGame->SetBufferLength ( uLen );
                }
                break;
            }

            //
            case FORCE_NONE :
            default:
            {
                //
                switch ( uLen )
                {
                    case TR1LEVELMINSIZE:
                    {
                        iVersion = GAME_TR10;
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
                        iVersion = GAME_TR15;
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
                    case TR2LEVELALT1SIZE : // Not Correct
                    case TR2LEVELALT2SIZE : // Not Used
                    {
                        iVersion = GAME_TR20;
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
                    case TR3LEVELALT1SIZE :
                    case TR3LEVELALT2SIZE :
                    {
                        iVersion = GAME_TR30;
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
                    case TR4LEVELALT1SIZE :
                    case TR4LEVELALT2SIZE :
                    {
                        iVersion    = GAME_TR40;
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

                    case TR5LEVELMAXSIZE :
                    {
                        iVersion = GAME_TR50;
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
                            iVersion    = GAME_TR49;
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
TRREALHEALTH *CTRSaveGame::GetRealHealthAddress ()
{
    if ( m_pRealHealth != NULL )
    {
        return m_pRealHealth;
    }

    static char szDebugString [ MAX_PATH ];
#ifdef _DEBUG
    ZeroMemory ( szDebugString, sizeof(szDebugString) );
    strcpy_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard\n" );
    OutputDebugString ( szDebugString );
#endif

    //
    char        *pBuffer;
    TRREALHEALTH      *pRealHealth;
    int         iX;

    m_pRealHealth     = NULL;

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
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    while ( iX > sizeof ( TRREALHEALTH ) )
    {
        pRealHealth = ( TRREALHEALTH * ) pBuffer;

        for ( int i = 0; i < IndicatorsTR123Table1Count; i++ )
        {
            if ( IndicatorsTR123Table1 [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTR123Table1 [ i ].step > CTRXGlobal::m_TR123IndexMaximum )
            {
                continue;
            }

            if (    pRealHealth->w1 == IndicatorsTR123Table1 [ i ].w1 &&
                    pRealHealth->w2 == IndicatorsTR123Table1 [ i ].w2 &&
                    pRealHealth->w4 == IndicatorsTR123Table1 [ i ].w4 )
            {
                if ( IndicatorsTR123Table1 [ i ].useW3 && pRealHealth->w3 != IndicatorsTR123Table1 [ i ].w3 )
                {
                    continue;
                }
                m_pRealHealth = pRealHealth;
                m_wRealHealth = pRealHealth->wRealHealth;
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTR123Table1 [ i ].szLabel );
                //  Just To Log It
                LastGoodHealth  = (int) ( (BYTE *) pRealHealth - (BYTE *) getBufferAddress () );
#ifdef _DEBUG
                ZeroMemory ( szDebugString, sizeof(szDebugString) );
                sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: Last Address: %04lx\n", LastGoodHealth );
                OutputDebugString ( szDebugString );

                TRREALHEALTH *pStruct = (TRREALHEALTH *) ( getBufferAddress () + LastGoodHealth );
                ZeroMemory ( szDebugString, sizeof(szDebugString) );
                sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: %04x %04x %04x %04x\n",
                    pStruct->w1, pStruct->w2, pStruct->w3, pStruct->w4 );
                OutputDebugString ( szDebugString );
#endif
                return m_pRealHealth;
            }
        }
        
        iX--;
        pBuffer++;
    }

    /*
     *      Second Try.
     */
    iX          = m_iSaveLength;
    pBuffer     = getBufferAddress ();

    while ( iX > sizeof ( TRREALHEALTH ) )
    {
        pRealHealth = ( TRREALHEALTH * ) pBuffer;

        for ( int i = 0; i < IndicatorsTR123Table2Count; i++ )
        {
            if ( IndicatorsTR123Table2 [ i ].bEnd )
            {
                break;
            }

            if ( IndicatorsTR123Table2 [ i ].step > CTRXGlobal::m_TR123IndexMaximum )
            {
                continue;
            }

            if (    pRealHealth->w1 == IndicatorsTR123Table2 [ i ].w1 &&
                    pRealHealth->w2 == IndicatorsTR123Table2 [ i ].w2 &&
                    pRealHealth->w4 == IndicatorsTR123Table2 [ i ].w4 )
            {
                if ( IndicatorsTR123Table2 [ i ].useW3 && pRealHealth->w3 != IndicatorsTR123Table2 [ i ].w3 )
                {
                    continue;
                }

                m_pRealHealth = pRealHealth;
                m_wRealHealth = pRealHealth->wRealHealth;
                strcpy_s ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTR123Table2 [ i ].szLabel );
                //  Just To Log It
                LastGoodHealth  = (int) ( (BYTE *) pRealHealth - (BYTE *) getBufferAddress () );
#ifdef _DEBUG
                ZeroMemory ( szDebugString, sizeof(szDebugString) );
                sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: Last Address: %04lx\n", LastGoodHealth );
                OutputDebugString ( szDebugString );

                TRREALHEALTH *pStruct = (TRREALHEALTH *) ( getBufferAddress () + LastGoodHealth );
                ZeroMemory ( szDebugString, sizeof(szDebugString) );
                sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: %04x %04x %04x %04x\n",
                    pStruct->w1, pStruct->w2, pStruct->w3, pStruct->w4 );
                OutputDebugString ( szDebugString );
#endif
                return m_pRealHealth;
            }
        }

        iX--;
        pBuffer++;
    }

#ifdef _DEBUG
    ZeroMemory ( szDebugString, sizeof(szDebugString) );
    sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: Last Address: %04lx\n", LastGoodHealth );
    OutputDebugString ( szDebugString );
    if ( LastGoodHealth > 0 )
    {
        TRREALHEALTH *pStruct = (TRREALHEALTH *) ( getBufferAddress () + LastGoodHealth );
        ZeroMemory ( szDebugString, sizeof(szDebugString) );
        sprintf_s ( szDebugString, sizeof(szDebugString), "GetRealHealthAddress TR Standard: %04x %04x %04x %04x\n",
            pStruct->w1, pStruct->w2, pStruct->w3, pStruct->w4 );
        OutputDebugString ( szDebugString );
    }
#endif

    //
    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::GetRealHealth ()
{
    TRREALHEALTH      *pRealHealth  = GetRealHealthAddress();
    if ( pRealHealth != NULL )
    {
        return ( pRealHealth->wRealHealth );
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRSaveGame::SetRealHealth ( const char *szRealHealth )
{
    if ( m_pRealHealth != NULL )
    {
        m_pRealHealth->wRealHealth = atoi ( szRealHealth );
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//  { FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE }
/////////////////////////////////////////////////////////////////////////////
BOOL CTRSaveGame::ReadIndicators(TR123_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    if ( ! PathFileExists(pFilename) )
    {
        return FALSE;
    }

    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "r" );
    if ( hFile == NULL )
    {
        return FALSE;
    }

    //
    int line    = 0;
    static char szLine [ MAX_PATH ];

    //
    do
    {
        ZeroMemory ( szLine, sizeof(szLine) );
        char *pLine = fgets ( szLine, sizeof(szLine), hFile );
        if ( pLine != NULL )
        {
            //
            TR123_INDICATORS indicators;
            ZeroMemory ( &indicators, sizeof(indicators) );

            //
            //  Get Label
            char *pLabel = strchr ( szLine, '"' );
            if ( pLabel )
            {
                //  Skip quote
                pLabel++;
                for ( int i = 0; i < sizeof(indicators.szLabel) - 1; i++ )
                {
                    if ( *pLabel == '"' )
                    {
                        break;
                    }
                    indicators.szLabel [ i ] = *pLabel;
                    pLabel++;
                }
            }

            //
            char    strDelimit[]    = " \t,{}";
            char    *strToken       = NULL;
            char    *context        = NULL;
            int     value           = 0;

            char *pAccolade         = strchr ( pLine, '{' );
            if ( pAccolade != NULL )
            {
                pLine   = pAccolade + 1;
            }

            //
            //      Treat Tokens
            int index = 0;
            strToken = strtok_s ( pLine, strDelimit, &context);
            while ( strToken != NULL && index <= 6 )
            {
                //
                bool bSkip = true;
                while ( bSkip )
                {
                    if ( *strToken == ' ' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '\t' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == ';' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '{' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '}' )
                    {
                        strToken++;
                    }
                    else
                    {
                        bSkip   = false;
                    }
                }

                //
                if ( _strnicmp ( strToken, "0x", 2 ) == 0 )
                {
                    sscanf_s ( strToken + 2, "%x", &value );
                }
                else if ( _strnicmp ( strToken, "true", strlen("true") ) == 0 )
                {
                    value   = TRUE;
                }
                else if ( _strnicmp ( strToken, "false", strlen("false") ) == 0 )
                {
                    value   = FALSE;
                }
                else
                {
                    value   = atoi(strToken);
                }

                //
                switch ( index )
                {
                    case 0 : indicators.bEnd    = (BOOL) value; break;
                    case 1 : indicators.w1      = (WORD) value; break;
                    case 2 : indicators.w2      = (WORD) value; break;
                    case 3 : indicators.w3      = (WORD) value; break;
                    case 4 : indicators.w4      = (WORD) value; break;
                    case 5 : indicators.useW3   = (BOOL) value; break;
                    case 6 : indicators.step    = value; break;
                }

                //      Get next token:
                strToken = strtok_s( NULL, strDelimit, &context);
                index++;
            }

            //
            //  Add Entry
            if ( line < maxTable )
            {
                IndicatorsTRTable [ line ] = indicators;

                //
                line++;
            }
        }
    }
    while ( ! feof ( hFile ) && ! ferror ( hFile ) );

    //
    //  Add End of entries
    //
    if ( line < maxTable )
    {
        TR123_INDICATORS indicators;
        ZeroMemory ( &indicators, sizeof(indicators) );
        indicators.bEnd     = TRUE;
        indicators.w1       = 0xffff;
        indicators.w2       = 0xffff;
        indicators.w3       = 0xffff;
        indicators.w4       = 0xffff;
        indicators.step     = 0;
        indicators.useW3    = TRUE;
        strcpy_s ( indicators.szLabel, sizeof(indicators.szLabel), "End" );
        IndicatorsTRTable [ line ] = indicators;
    }

    //
    CloseOneFile ( &hFile );

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRSaveGame::WriteIndicators(TR123_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    //
    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "w" );
    if ( hFile == NULL )
    {
        return FALSE;
    }

    //
    int index = 0;
    do
    {
        fprintf_s ( hFile, "{ " );
        TR123_INDICATORS indicator = IndicatorsTRTable [ index ];
        if ( indicator.bEnd )
        {
            fprintf_s ( hFile, "TRUE, " );
        }
        else
        {
            fprintf_s ( hFile, "FALSE, " );
        }

        fprintf_s ( hFile, "0x%04x, ", indicator.w1 & 0xffff );
        fprintf_s ( hFile, "0x%04x, ", indicator.w2 & 0xffff );
        fprintf_s ( hFile, "0x%04x, ", indicator.w3 & 0xffff );
        fprintf_s ( hFile, "0x%04x, ", indicator.w4 & 0xffff );

        if ( indicator.useW3 )
        {
            fprintf_s ( hFile, "TRUE, " );
        }
        else
        {
            fprintf_s ( hFile, "FALSE, " );
        }

        fprintf_s ( hFile, "%d, ", indicator.step );
        fprintf_s ( hFile, "\"%s\", ", indicator.szLabel );

        fprintf_s ( hFile, "}\n" );

        if ( indicator.bEnd )
        {
            break;
        }

        index++;
    }
    while ( index < maxTable );

    CloseOneFile ( &hFile );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRSaveGame::CountIndicators( TR123_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count = 0;
    do
    {
        TR123_INDICATORS indicator = IndicatorsTRTable [ count ];
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
int CTRSaveGame::MinIndicators( TR123_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int minimum     = -1;
    do
    {
        TR123_INDICATORS indicator = IndicatorsTRTable [ count ];
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
int CTRSaveGame::MaxIndicators( TR123_INDICATORS *IndicatorsTRTable, const int maxTable )
{
    //
    int count       = 0;
    int maximum     = -1;
    do
    {
        TR123_INDICATORS indicator = IndicatorsTRTable [ count ];
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

