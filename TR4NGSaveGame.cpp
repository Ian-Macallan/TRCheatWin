// TR4NGSaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"

#include "TR4NGSaveGame.h"
#include "TRXTools.h"
#include "TR_Areas.h"
#include "TRXGlobal.h"
#include "GunGrids.h"
#include "TRXMessageBox.h"
#include "ReadTR2\ReadTRXScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTRXCHEATWINApp theApp;

//
static int LastGoodIndicator = 0;

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
TR45_INDICATORS IndicatorsTR4NGTable [ MAX_INDICATORS ] =
{
    {   FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE,   0,  "Standing", },      // Step 0 must be
    {   FALSE,  0x02,   0x02,   0x47,   0x67,   TRUE,   0,  "Standing", },      // At Start

    {   FALSE,  0x02,   0x02,   0x00,   0x28,   TRUE,   1,  "Indicator 1", },
    {   FALSE,  0x02,   0x02,   0x00,   0x0c,   TRUE,   1,  "Indicator 3", },
    {   FALSE,  0x02,   0x02,   0x00,   0x1f,   TRUE,   1,  "Indicator 4", },
    {   FALSE,  0x02,   0x02,   0x00,   0xbd,   TRUE,   1,  "Indicator 5", },
    {   FALSE,  0x02,   0x02,   0x00,   0xdd,   TRUE,   1,  "Indicator 6", },

    {   FALSE,  0x0d,   0x0d,   0x00,   0x6c,   TRUE,   1,  "Swimming", },
    {   FALSE,  0x0d,   0x0d,   0x47,   0x6c,   TRUE,   1,  "Swimming", },

    {   FALSE,  0x47,   0x47,   0x47,   0xde,   TRUE,   1,  "Kneeling", },
    {   FALSE,  0x10,   0x00,   0x51,   0x51,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x21,   0x21,   0x00,   0x6e,   TRUE,   1,  "In Water", },

    {   FALSE,  0x13,   0x13,   0x47,   0x61,   TRUE,   1,  "Indicator 10", },
    {   FALSE,  0x54,   0x50,   0x47,   0x0d,   TRUE,   1,  "Indicator 11", },

    {   FALSE,  0x02,   0x02,   0x00,   0x52,   TRUE,   1,  "Indicator 12", },
    {   FALSE,  0x02,   0x02,   0x47,   0x0b,   TRUE,   1,  "Indicator 13", }, 

    {   FALSE,  0x02,   0x02,   0x47,   0xbd,   TRUE,   1,  "Dropping Flare", }, 
    {   FALSE,  0x01,   0x02,   0x47,   0x08,   TRUE,   1,  "Standing", }, 

    {   FALSE,  0x21,   0x21,   0x47,   0x6e,   TRUE,   1,  "In Water", }, 
    {   FALSE,  0x0d,   0x12,   0x00,   0x6c,   TRUE,   1,  "Swimming", }, 
    {   FALSE,  0x12,   0x12,   0x00,   0x57,   TRUE,   1,  "Swimming", }, 

    {   FALSE,  0x50,   0x50,   0x00,   0x07,   TRUE,   1,  "Crawling", },
    {   FALSE,  0x50,   0x50,   0x47,   0x07,   TRUE,   1,  "Crawling", },

    {   FALSE,  0x47,   0x47,   0x00,   0xde,   TRUE,   1,  "Kneeling", },

    {   FALSE,  0x02,   0x02,   0x00,   0x18,   TRUE,   1,  "Stand", },

    {   FALSE,  0x02,   0x02,   0x47,   0x87,   TRUE,   1,  "Standing", },

    {   FALSE,  0x01,   0x02,   0x00,   0x0a,   TRUE,   2,  "Advancing 1", }, 
    {   FALSE,  0x01,   0x02,   0x00,   0x08,   TRUE,   2,  "Advancing 2", },

    {   FALSE,  0x18,   0x18,   0x00,   0x46,   TRUE,   8,  "Sliding", },
    {   FALSE,  0x18,   0x18,   0x47,   0x46,   TRUE,   8,  "Sliding", },

    {   FALSE,  0x0d,   0x12,   0x47,   0x6c,   TRUE,   8,  "Swimming", },

    //
    {   FALSE,  0x0f,   0x0f,   0x00,   0x1f,   TRUE,   9,  "Quad Bike", },
    {   FALSE,  0x0f,   0x0f,   0x00,   0x46,   TRUE,   9,  "Quad Bike", },


    {   FALSE,  0x09,   0x09,   0x00,   0x66,   TRUE,   9,  "Small Car", },     // Cannot move so unusable
    {   FALSE,  0x0f,   0x0f,   0x00,   0x1d,   TRUE,   9,  "Motocycle", },     // Cannot move so unusable
    {   FALSE,  0x0f,   0x0f,   0x00,   0x40,   TRUE,   9,  "Motocycle", },     // Cannot move so unusable
    {   FALSE,  0x0f,   0x0f,   0x00,   0x25,   TRUE,   9,  "Skydoo", },        // Cannot move so unusable
    {   FALSE,  0x01,   0x01,   0x00,   0x15,   TRUE,   9,  "Skydoo", },        // Cannot move so unusable

    //  Not Good
    {   FALSE,  0x0c,   0x00,   0x00,   0x02,   TRUE,   10,  "Indicator 9", },
    {   FALSE,  0x00,   0x02,   0x00,   0x02,   TRUE,   10,  "Indicator 7", },
    {   FALSE,  0x00,   0x02,   0x00,   0x03,   TRUE,   10,  "Indicator 8", },
    {   FALSE,  0x12,   0x00,   0x00,   0x02,   TRUE,   10,  "Flare", },
    {   FALSE,  0x02,   0x02,   0x00,   0x0b,   TRUE,   10,  "Indicator 2", },

    //
    {   TRUE,   0xff,   0xff,   0xff,   0xff,   TRUE,   0,  "End", },         // End
};
int IndicatorsTR4NGTableCount = sizeof(IndicatorsTR4NGTable)/sizeof(TR45_INDICATORS);

//
static int positionCount = 0;
static TR4NG_POSITION *positionTable [ MAX_POSITION ];

//
static TR_POSITION_RANGE TR4NGIndicatorRange [ 1 ] =
{
    {   0x280,  0x3000  },
};

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

    m_iVersion          = GAME_TR49;
    m_iSubVersion       = GAME_TRC9;

    m_iSaveLength       = CTRXGlobal::m_iMaxNGSize;
    m_iMaxLevel         = TR4NGMAXLEVEL;

    iMaskGun            = TR4NG_MASK_PISTOL;        //  Pistol
    iMaskUzi            = TR4NG_MASK_UZI;
    iMaskRiotGun        = TR4NG_MASK_SHOTGUN;
    iMaskCrossBow       = TR4NG_MASK_CROSSBOW;      // Crossbow
    iMaskGrenade        = TR4NG_MASK_GRENADE;
    iMaskRevolver       = TR4NG_MASK_REVOLVER;      // Revolver
    iMaskLaser          = TR4NG_MASK_LASER;
    iMaskBinocular      = TR4NG_MASK_BINOCULAR;
    iMaskCrowBar        = TR4NG_MASK_CROWBAR;

    iRiotGunUnits       = 6;

    m_pRealHealth       = NULL;

    m_pBuffer           = new ( TR4NGSAVE );
    ZeroMemory ( m_pBuffer, sizeof(TR4NGSAVE) );

    m_pBufferBackup     = new ( TR4NGSAVE );
    ZeroMemory ( m_pBufferBackup, sizeof(TR4NGSAVE) );

    //  TRNG Specific
    m_bBlindedTRNG          = FALSE;

    m_pTRNGHealth           = NULL;
    m_pTRNGAir              = NULL;
    m_pTRNGGuns             = NULL;
    m_pTRNGAmmos            = NULL;
    m_pTRNGStatusNG         = NULL;
    m_pTRNGCold             = NULL;
    m_pTRNGDamage           = NULL;
    m_pKeysToStop           = NULL;

    m_pCoordinates          = NULL;
    m_iCoordinateIndex      = -1;
    m_iCoordinateCount      = 0;

    ZeroMemory ( m_SaveGameVersion, sizeof(m_SaveGameVersion) );

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
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::GetTRNGPointers()
{
    //  TRNG Specific
    m_pTRNGHealth           = NULL;
    m_pTRNGAir              = NULL;
    m_pTRNGGuns             = NULL;
    m_pTRNGAmmos            = NULL;
    m_pTRNGStatusNG         = NULL;
    m_pTRNGCold             = NULL;
    m_pTRNGDamage           = NULL;
    m_pKeysToStop           = NULL;
    m_pCoordinates          = NULL;
    m_iCoordinateIndex      = -1;
    m_iCoordinateCount      = 0;

    ZeroMemory ( m_SaveGameVersion, sizeof(m_SaveGameVersion) );

    //
    char *pSignature    = (char* ) m_pBuffer + m_iSaveLength - 8;
    if ( memcmp ( pSignature, "NGLE", 4 ) != 0  )
    {
        return FALSE;
    }

    DWORD *pDwLength    = (DWORD *) ( (char* ) m_pBuffer + m_iSaveLength - 4 );
    DWORD dwLength      = *pDwLength;

    //  This is the real address
    BYTE *pBuffer = ( ( BYTE * ) m_pBuffer + m_iSaveLength - dwLength );
    //  But the normal address s
    // pBuffer = ( BYTE * ) m_pBuffer + 0x8000;

    //
    TRNGSPECIFIC *pTRNG = (TRNGSPECIFIC *) pBuffer;
    if ( memcmp ( pTRNG->signature, "NG", sizeof(pTRNG->signature) ) != 0 )
    {
        return FALSE;
    }

    //
    TRNGITERATION *pIteration = & pTRNG->iteration;

    //
    BOOL bContinue      = TRUE;
    DWORD length        = 0;
    WORD *pCodeOp       = NULL;
    DWORD ExtraWords    = 0;
    WORD *pValues       = NULL;

    //
    while ( bContinue )
    {
        length  = pIteration->w2.length;
        pCodeOp = &pIteration->w2.codeOp;
        pValues = pIteration->w2.values;

        //
        if ( pIteration->w2.length & 0x8000)
        {
            // size e' DWORD
            length      = ( ( (DWORD) (pIteration->w3.lengthHi & 0x7fff) ) << 16 ) + (DWORD) pIteration->w3.lengthLo;
            pCodeOp     = &pIteration->w3.codeOp;
            pValues     = pIteration->w3.values;
            ExtraWords  = 3;
        }
        else
        {
            // size e' WORD
            length      = pIteration->w2.length;
            ExtraWords  = 2;
        }

        //
        if ( length == 0 )
        {
            bContinue = FALSE;
            break;
        }

        if ( *pCodeOp < 0x8000 || *pCodeOp > 0x80ff )
        {
            bContinue = FALSE;
            break;
        }

        //
        switch ( *pCodeOp )
        {
            //
            case NGTAG_SAVEGAME_INFOS :
            {
                TRNGSAVEGAMEINFOS *pSave    = (TRNGSAVEGAMEINFOS *) pValues;
                m_pTRNGHealth               = &pSave->LaraVitality;

                DWORD offsetAir     = offsetof(TR4NGSAVE,iAir) - 0x57;
                if ( offsetAir >= 0 && offsetAir < sizeof(pSave->Copy_057_0F3) )
                {
                    m_pTRNGAir      = ( WORD * ) ( ( BYTE * ) pSave->Copy_057_0F3 + offsetAir );
                }

                DWORD offsetGuns    =  offsetof(TR4NGSAVE,tagGuns) - 0x169;
                if ( offsetGuns >= 0 && offsetGuns < sizeof(pSave->Copy_169_1AC) )
                {
                    m_pTRNGGuns     = (TR4NGGUN *) ( (BYTE*) pSave->Copy_169_1AC + offsetGuns );
                }

                DWORD offsetAmmos   =  offsetof(TR4NGSAVE,tagAmmo) - 0x169;
                if ( offsetAmmos >= 0 && offsetAmmos < sizeof(pSave->Copy_169_1AC) )
                {
                    m_pTRNGAmmos    = (TR4NGAMMO *) ( (BYTE*) pSave->Copy_169_1AC + offsetAmmos );
                }

                break;
            }

            //  Original Position
            case NGTAG_SALVA_COORDINATE :
            {
                //  Normally the first two words must be 
                TRNGBASESAVECOORD *pSave    = (TRNGBASESAVECOORD *) pValues;
                WORD count                  = pSave->TotSalvati;
                m_iCoordinateCount          = count;
                WORD *pIndices              = ( WORD *) &pValues [ 1 ];
                TRNGSaveCoord *pCoord       = ( TRNGSaveCoord * ) &pValues [ count + 1 ];
                m_pCoordinates              = pCoord;
                break;
            }

            //  Some Position
            case NGTAG_SALVA_STATICS : 
            {
                WORD count              = * pValues;
                TRNGSALVASTATIC *pSave  = (TRNGSALVASTATIC *) &pValues [ 1 ];
                for ( int i = 0; i < count; i++ )
                {
                    pSave++;
                }
                break;
            }

            //
            case NGTAG_VAR_GLOBAL_TRNG :
            {
                TRNGGLOBALVARIABLES *pSave = (TRNGGLOBALVARIABLES *) pValues;

                break;
            }

            //
            case NGTAG_VAR_LOCAL_TRNG :
            {
                TRNGBLOCKNUM *pSave = (TRNGBLOCKNUM *) pValues;

                break;
            }

            //
            case NGTAG_NG_HUB_HEADERS :
            {
                break;
            }

            //
            case NGTAG_VERSION_HEADER :
            {
                TRNGVERSIONHEADER *pSave = (TRNGVERSIONHEADER *) pValues;
                sprintf_s ( m_SaveGameVersion, sizeof(m_SaveGameVersion), "%u.%u.%u.%u", 
                    pSave->VetVersione [ 0 ], pSave->VetVersione [ 1 ],
                    pSave->VetVersione [ 2 ], pSave->VetVersione [ 3 ] );
                break;
            }

    		case NGTAG_VARIABLE_DATA :
            {
                TRNGDATIVARIABILIFIELDS *pSave = (TRNGDATIVARIABILIFIELDS *)pValues;
                m_pTRNGStatusNG     = &pSave->StatusNG;
                m_pTRNGCold         = &pSave->ValoreCold;
                m_pTRNGDamage       = &pSave->ValoreDamage;
                m_pKeysToStop       = &pSave->KeysToStop;
                break;
            }
        };

        //
        pIteration = ( TRNGITERATION * ) ( ( WORD * ) pIteration + length );
    };

    return TRUE;
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::TraceTRNG(FILE *hLogFile)
{
    //
    char *pSignature    = (char* ) m_pBuffer + m_iSaveLength - 8;
    if ( memcmp ( pSignature, "NGLE", 4 ) != 0  )
    {
        return;
    }

    DWORD *pDwLength    = (DWORD *) ( (char* ) m_pBuffer + m_iSaveLength - 4 );
    DWORD dwLength      = *pDwLength;

    //  This is the real address
    BYTE *pBuffer = ( ( BYTE * ) m_pBuffer + m_iSaveLength - dwLength );
    // But the normal address is
    // pBuffer = ( BYTE * ) m_pBuffer + 0x8000;

    //
    static char szDebugString [ MAX_PATH ];

    //
    TRNGSPECIFIC *pTRNG = (TRNGSPECIFIC *) pBuffer;
    if ( memcmp ( pTRNG->signature, "NG", sizeof(pTRNG->signature) ) != 0 )
    {
        return;
    }

    //
    TRNGITERATION *pIteration = & pTRNG->iteration;

    //
    BOOL bContinue      = TRUE;
    DWORD length        = 0;
    WORD *pCodeOp       = NULL;
    DWORD ExtraWords    = 0;
    WORD *pValues       = NULL;
    //
    while ( bContinue )
    {
        length  = pIteration->w2.length;
        pCodeOp = &pIteration->w2.codeOp;
        pValues = pIteration->w2.values;

        //
        if ( pIteration->w2.length & 0x8000)
        {
            // size e' DWORD
            length      = ( ( (DWORD) (pIteration->w3.lengthHi & 0x7fff) ) << 16 ) + (DWORD) pIteration->w3.lengthLo;
            pCodeOp     = &pIteration->w3.codeOp;
            pValues     = pIteration->w3.values;
            ExtraWords  = 3;
        }
        else
        {
            // size e' WORD
            length      = pIteration->w2.length;
            ExtraWords  = 2;
        }

        //
        if ( length == 0 )
        {
            DWORD relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, m_pBuffer );
            sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x Length zero - Code is 0x%04x\n",
                relativeAddress, *pCodeOp );
            OutputTRNGSaveString( szDebugString, hLogFile );
            bContinue = FALSE;
            break;
        }

        if ( *pCodeOp < 0x8000 || *pCodeOp > 0x80ff )
        {
            DWORD relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, m_pBuffer );
            sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x Code is 0x%04x - Length : %ld\n",  
                relativeAddress, *pCodeOp, length );
            OutputTRNGSaveString( szDebugString, hLogFile );
            bContinue = FALSE;
            break;
        }

        //
        DWORD relativeAddress = CTRXTools::RelativeAddress ( pCodeOp, m_pBuffer );
        sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : Code 0x%04x (%s) (%ld words %ld bytes)\n", 
            relativeAddress,
            *pCodeOp, GetTRNGTagLabel(*pCodeOp), length, (long) sizeof(WORD) * ( length - ExtraWords ) );
        OutputTRNGSaveString( szDebugString, hLogFile );

        switch ( *pCodeOp )
        {
            //
            case NGTAG_SAVEGAME_INFOS :
            {
                TRNGSAVEGAMEINFOS *pSave = (TRNGSAVEGAMEINFOS *) pValues;
                relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tHealth : %d Level : %s\tState Id: 0x%0x\n", 
                    relativeAddress,
                    (int) sizeof(TRNGSAVEGAMEINFOS),
                    pSave->LaraVitality, pSave->Tr4Name, pSave->LaraStateId );
                OutputTRNGSaveString( szDebugString, hLogFile );
                break;
            }

            //  Original Position
            case NGTAG_SALVA_COORDINATE :
            {
                //  Normally the first two words must be 
                TRNGBASESAVECOORD *pSave    = (TRNGBASESAVECOORD *) pValues;
                WORD count                  = pSave->TotSalvati;
                WORD *pIndices              = ( WORD *) &pValues [ 1 ]; // Skip Count
                TRNGSaveCoord *pCoord       = ( TRNGSaveCoord * ) &pValues [ 1 + count ];   // Skip Count and Indices
                for ( int i = 0; i < count; i++ )
                {
                    relativeAddress = CTRXTools::RelativeAddress ( &pCoord [ i ], m_pBuffer );
                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\t Count: %2d Indice: %3d - Room : %3d - x: %6ld - y: %6d - z: %6ld\n", 
                        relativeAddress,
                        (int) sizeof(TRNGBASESAVECOORD),
                        count,
                        pIndices [ i ],
                        pCoord [ i ].Room,
                        pCoord [ i ].CordX, pCoord [ i ].CordY, pCoord [ i ].CordZ );
                    OutputTRNGSaveString( szDebugString, hLogFile );
                }
                break;
            }

            //  Some Position
            case NGTAG_SALVA_STATICS : 
            {
                WORD count              = *pValues;
                TRNGSALVASTATIC *pSave  = (TRNGSALVASTATIC *) &pValues [ 1 ];   // Sklp count
                relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                for ( int i = 0; i < count; i++ )
                {
                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tCount: %2d Indice: %3d - Room : %3d - x: %6d - y: %6d - z: %6d - fl: 0x%x\n", 
                        relativeAddress,
                        (int) sizeof(TRNGSALVASTATIC),
                        count,
                        pSave->Indici.IndiceStatic,
                        pSave->Indici.IndiceRoom,
                        pSave->OrgX, pSave->OrgY, pSave->OrgZ,
                        pSave->Flags );
                    OutputTRNGSaveString( szDebugString, hLogFile );
                    pSave++;
                }
                break;
            }

            //
            case NGTAG_VAR_GLOBAL_TRNG :
            {
                TRNGGLOBALVARIABLES *pSave = (TRNGGLOBALVARIABLES *) pValues;
                relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tCurrent Value : 0x%x\n", 
                    relativeAddress,
                    (int) sizeof(TRNGGLOBALVARIABLES),
                    pSave->CurrentValue );
                OutputTRNGSaveString( szDebugString, hLogFile );
                break;
            }

            //
            case NGTAG_VAR_LOCAL_TRNG :
            {
                TRNGBLOCKNUM *pSave = (TRNGBLOCKNUM *) pValues;
                relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tSomething\n", 
                    relativeAddress,
                    (int) sizeof(TRNGBLOCKNUM) );
                OutputTRNGSaveString( szDebugString, hLogFile );
                break;
            }

            //
            case NGTAG_NG_HUB_HEADERS :
            {
                TRNGBASENGHUB       *pSave      = (TRNGBASENGHUB *) pValues;
                TRNGEXTRACTNG       *pHub       = (TRNGEXTRACTNG *) ( ( (BYTE * ) pSave ) + sizeof(TRNGBASENGHUB) );
                TRNGMININGNGHEADER  *pHeader    = (TRNGMININGNGHEADER *) ( ( (BYTE * ) pHub ) + sizeof(TRNGEXTRACTNG) + pHub->NWords * sizeof(WORD) );

                DWORD relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : %d\tTRNGBASENGHUB : 0x%08x\n", 
                    (int) sizeof(TRNGBASENGHUB), relativeAddress );
                OutputTRNGSaveString( szDebugString, hLogFile );

                relativeAddress = CTRXTools::RelativeAddress ( pHub, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : %d\tTRNGEXTRACTNG : 0x%08x\n", 
                    (int) sizeof(TRNGEXTRACTNG), relativeAddress );
                OutputTRNGSaveString( szDebugString, hLogFile );

                relativeAddress = CTRXTools::RelativeAddress ( pHeader, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : %d\tTRNGMININGNGHEADER : 0x%08x\n", 
                    (int) sizeof(TRNGMININGNGHEADER), relativeAddress );
                OutputTRNGSaveString( szDebugString, hLogFile );

                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : \tTotHub : %d - LastIndex : %d - LaraHUB.NWords : %d\n", 
                    pSave->TotHub, pSave->LastIndex, pHub->NWords );
                OutputTRNGSaveString( szDebugString, hLogFile );

                for ( int i = 0; i < 10; i++ )
                {
                    relativeAddress = CTRXTools::RelativeAddress ( pHeader, m_pBuffer );
                    sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : NumeroLivello : %d - TotWords : %d\n", 
                        relativeAddress,
                        pHeader->NumeroLivello, pHeader->TotWords );
                    pHeader = (TRNGMININGNGHEADER *) ( ( ( BYTE * ) pHeader ) + sizeof(TRNGMININGNGHEADER) + pHeader->TotWords * sizeof(WORD) );
                    OutputTRNGSaveString( szDebugString, hLogFile );
                }

                break;
            }

    		case NGTAG_VARIABLE_DATA :
            {
                TRNGDATIVARIABILIFIELDS *pSave = (TRNGDATIVARIABILIFIELDS *)pValues;
                DWORD relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tStatus : 0x%lx - Cold : %u - Damage : %u\n", 
                    relativeAddress,
                    (int) sizeof(TRNGDATIVARIABILIFIELDS),
                    pSave->StatusNG, 
                    pSave->ValoreCold,
                    pSave->ValoreDamage );
                OutputTRNGSaveString( szDebugString, hLogFile );
                break;
            }

            //
            case NGTAG_VERSION_HEADER :
            {
                TRNGVERSIONHEADER *pSave = (TRNGVERSIONHEADER *) pValues;
                DWORD relativeAddress = CTRXTools::RelativeAddress ( pSave, m_pBuffer );
                sprintf_s ( szDebugString, sizeof(szDebugString), "; TRNGSAVE : 0x%08x : %d\tVersion : %u.%u.%u.%u - Flag : 0x%04x\n", 
                    relativeAddress,
                    (int) sizeof(TRNGVERSIONHEADER),
                    pSave->VetVersione [ 0 ], pSave->VetVersione [ 1 ],
                    pSave->VetVersione [ 2 ], pSave->VetVersione [ 3 ],
                    pSave->Flags );
                OutputTRNGSaveString( szDebugString, hLogFile );
                break;
            }
        };

        //
        pIteration = ( TRNGITERATION * ) ( ( WORD * ) pIteration + length );
    };

}

//
/////////////////////////////////////////////////////////////////////////////
// CTR4NGSaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::ReadSavegame ( const char *pFilename )
{

    FILE                    *hFile;
    size_t                  uLenBuffer;

    char                    szEmpty [ 1 ];

    m_bBlindedTRNG          = FALSE;

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
    InitStatus ();

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4NGSAVE ) );

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
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < CTRXGlobal::m_iMinNGSize || m_iSaveLength > CTRXGlobal::m_iMaxNGSize ) )
    {
        AddFormatToStatus ( "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR4NGSAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR4NGSAVE ) );
        CloseOneFile ( &hFile );
        return 0;
    }

    //
    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR4NGSAVE ) );
    uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
        AddToStatus ( "File size is not correct." );
        CloseOneFile ( &hFile );
        return 0;
    }

    //
    //  The last eight byte if the file is
    //  4E474C4526120000    NGLE&...
    char *pSignature = (char* ) m_pBuffer + uLenBuffer - 8;
    if ( memcmp ( pSignature, "NGLE", 4 ) != 0  )
    {
        AddToStatus ( "File Signature is not correct." );
        CloseOneFile ( &hFile );
        return 0;
    }

    //
    long lPos = ftell ( hFile );
    if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
    {
        fseek ( hFile, 0, SEEK_END );
        long lEnd = ftell ( hFile );
        AddFormatToStatus ( "File size is too large %ld til %ld = %ld.", lPos, lEnd, lEnd - lPos );
        CloseOneFile ( &hFile );
        return 0;
    }

    memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR4NGSAVE) );

    /*
     *      Close file.
     */
    CloseOneFile ( &hFile );

    //
    //  Test Flag 
    if ( m_pBuffer->flagsffff != 0xffff )
    {
        m_bBlindedTRNG = IsScriptBlinded ( pFilename ); 
        if ( m_bBlindedTRNG )
        {
            CTRXMessageBox::ShowMessage( "Load Savegame Warning", "Warning this file will not be correctly treated\nUse -unblind");
            AddToStatus ( "Load Savegame Warning : Warning this file will not be correctly treated - Use -unblind" );
        }
    }

    //
    //  Get Pointers
    GetTRNGPointers();

    //
    //  A Good Place to check guns
    if ( m_pTRNGGuns != NULL )
    {
        static char szDebugString [ MAX_PATH ];

        TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

        if ( m_pTRNGGuns->m_gunPistol != pGun->m_gunPistol )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on Pistol : %02x %02x\n", m_pTRNGGuns->m_gunPistol, pGun->m_gunPistol);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunUzis != pGun->m_gunUzis )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on Uzis : %02x %02x\n", m_pTRNGGuns->m_gunUzis, pGun->m_gunUzis);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunRiotGun != pGun->m_gunRiotGun )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on RiotGun : %02x %02x\n", m_pTRNGGuns->m_gunRiotGun, pGun->m_gunRiotGun);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunCrossBow != pGun->m_gunCrossBow )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on CrossBow : %02x %02x\n", m_pTRNGGuns->m_gunCrossBow, pGun->m_gunCrossBow);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunGrenadesLauncher != pGun->m_gunGrenadesLauncher )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on GrenadesLauncher : %02x %02x\n", m_pTRNGGuns->m_gunGrenadesLauncher, pGun->m_gunGrenadesLauncher);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunRevolver != pGun->m_gunRevolver )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on Revolver : %02x %02x\n", m_pTRNGGuns->m_gunRevolver, pGun->m_gunRevolver);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunLaserLight != pGun->m_gunLaserLight )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on LaserLight : %02x %02x\n", m_pTRNGGuns->m_gunLaserLight, pGun->m_gunLaserLight);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunBinocular != pGun->m_gunBinocular )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on Binocular : %02x %02x\n", m_pTRNGGuns->m_gunBinocular, pGun->m_gunBinocular);
            OutputDebugString ( szDebugString );
        }

        if ( m_pTRNGGuns->m_gunCrowBar != pGun->m_gunCrowBar )
        {
            sprintf_s ( szDebugString, sizeof(szDebugString), "Difference on CrowBar : %02x %02x\n", m_pTRNGGuns->m_gunCrowBar, pGun->m_gunCrowBar);
            OutputDebugString ( szDebugString );
        }

    }

    //
    //  Trace
    FILE    *hLogFile = NULL;
    static char szLogFile [ MAX_PATH ];
    strcpy_s ( szLogFile, sizeof(szLogFile), pFilename );
    strcat_s ( szLogFile, sizeof(szLogFile), ".log" );

    if ( CTRXGlobal::m_bTraceTRNGSavegame )
    {
        fopen_s ( &hLogFile, szLogFile, "w" );
    }

    //
    TraceTRNG(hLogFile);

    if ( hLogFile != NULL )
    {
        CloseOneFile ( &hLogFile );
        hLogFile    = NULL;
    }

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

    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;
    TR4NGAMMO   *pAmmo      = &m_pBuffer->tagAmmo;

    InitStatus ();

    //  Even if we write in m_pTRNGGuns and m_pTRNGAmmos
    //  The result does not show the difference
    //  So it is probably written elsewhere in the savegame
    if ( m_bBlindedTRNG )
    {
        CTRXMessageBox::ShowMessage( "Write Savegame Warning", "Warning this file will not be saved\nUse -unblind");
        return;
    }

    /*
     *  Correct guns.
     */
    int     iX      = getLevelIndex ();

    if ( ! ( pGun->m_gunRevolver & iMaskRevolver ) )
    {
        pGun->m_gunRevolver  = 0 ;
    }

    if ( ! ( pGun->m_gunUzis & iMaskUzi ) )
    {
        pGun->m_gunUzis = 0;
    }

    if ( ! ( pGun->m_gunRiotGun & iMaskRiotGun ) )
    {
        pGun->m_gunRiotGun = 0;
    }

    if ( ! ( pGun->m_gunGrenadesLauncher & iMaskGrenade ) )
    {
        pGun->m_gunGrenadesLauncher = 0;
    }

    if ( ! ( pGun->m_gunCrossBow & iMaskCrossBow ) )
    {
        pGun->m_gunCrossBow = 0;
    }

    //
    //  Here will be a good position to adjust 
    //  m_pTRNGGuns and m_pTRNGAmmos
    if ( CTRXGlobal::m_bAlterTRNGAmmosGuns && ( m_pTRNGGuns != NULL ) && ( m_pTRNGAmmos != NULL ) )
    {
        m_pTRNGGuns->m_gunPistol            = pGun->m_gunPistol;
        m_pTRNGGuns->m_gunUzis              = pGun->m_gunUzis;
        m_pTRNGGuns->m_gunRiotGun           = pGun->m_gunRiotGun;
        m_pTRNGGuns->m_gunCrossBow          = pGun->m_gunCrossBow;
        m_pTRNGGuns->m_gunGrenadesLauncher  = pGun->m_gunGrenadesLauncher;
        m_pTRNGGuns->m_gunRevolver          = pGun->m_gunRevolver;
        m_pTRNGGuns->m_gunLaserLight        = pGun->m_gunLaserLight;
        m_pTRNGGuns->m_gunBinocular         = pGun->m_gunBinocular;
        m_pTRNGGuns->m_gunCrowBar           = pGun->m_gunCrowBar;

        m_pTRNGAmmos->m_iSmallMedipak       = pAmmo->m_iSmallMedipak;
        m_pTRNGAmmos->m_iLargeMedipak       = pAmmo->m_iLargeMedipak;
        m_pTRNGAmmos->m_iFlares             = pAmmo->m_iFlares;
        m_pTRNGAmmos->m_iGunAmmos           = pAmmo->m_iGunAmmos;
        m_pTRNGAmmos->m_iUziAmmos           = pAmmo->m_iUziAmmos;
        m_pTRNGAmmos->m_iRevolverAmmos      = pAmmo->m_iRevolverAmmos;
        m_pTRNGAmmos->m_iShotGunAmmo1       = pAmmo->m_iShotGunAmmo1;
        m_pTRNGAmmos->m_iShotGunAmmo2       = pAmmo->m_iShotGunAmmo2;
        m_pTRNGAmmos->m_iGrenade1           = pAmmo->m_iGrenade1;
        m_pTRNGAmmos->m_iGrenade2           = pAmmo->m_iGrenade2;
        m_pTRNGAmmos->m_iGrenade3           = pAmmo->m_iGrenade3;
        m_pTRNGAmmos->m_iCrossbow1          = pAmmo->m_iCrossbow1;
        m_pTRNGAmmos->m_iCrossbow2          = pAmmo->m_iCrossbow2;
        m_pTRNGAmmos->m_iCrossbow3          = pAmmo->m_iCrossbow3;

    }

    //  Compute CheckSum
    unsigned char *pBackup = (unsigned char *)m_pBufferBackup;
    unsigned char *pBuffer = (unsigned char *)m_pBuffer;

    //  Compute checksum without TR4NG Extension
    unsigned checkSum = m_pBufferBackup->checkSum;
    int offsetCheckSum = offsetof ( TR4NGSAVE, checkSum );
    for ( int i = 0; i < offsetCheckSum - 1; i++ )
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
        AddToStatus ( "Unable to write the file." );
        return;
    }

    /*
     *      Get Buffer.
     */
    if ( CTRXGlobal::m_ForceSaveGame == FORCE_NONE && ( m_iSaveLength < CTRXGlobal::m_iMinNGSize || m_iSaveLength > CTRXGlobal::m_iMaxNGSize ) )
    {
        AddFormatToStatus ( "Internal error in length %d versus %d = %d.",
            (int) sizeof ( TR4NGSAVE ), m_iSaveLength,
            m_iSaveLength - (int) sizeof ( TR4NGSAVE ) );
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
void CTR4NGSaveGame::RetrieveInformation( const char *pFilename )
{
    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
    {
        RetrieveHeader ( );
        m_iSubVersion   = GAME_TRC9;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR4NGGUN *CTR4NGSaveGame::SearchGunStructure ( unsigned short m_iHealth, int *iPos )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    return pGun;
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
    TR4NGGUN *pGun      = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    m_iDesertEagle      = pGun->m_gunRevolver;
    m_iRiotGun          = pGun->m_gunRiotGun;
    m_iUzis             = pGun->m_gunUzis;
    m_iGrenades         = pGun->m_gunGrenadesLauncher;
    m_iHarpoon          = pGun->m_gunCrossBow;
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
//  This is used in the savegame list
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::GetDetailedInfo (  char *szGame, size_t iSize, int *iGame, int *iLevel,
                                        char *szTitle, size_t iSizeTile)
{
    strcpy_s ( szGame, iSize, "TR4NG" );
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
    if ( m_bBlindedTRNG )
    {
        return -1;
    }

    return getLevel() - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos1(int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iGunAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos2(int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iRevolverAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos3( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iUziAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos4a( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iShotGunAmmo1 / iRiotGunUnits );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos4b( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iShotGunAmmo2 / iRiotGunUnits );
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
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    if ( iX >= 0 && iX < m_iMaxLevel )
    {
//      return ( pGun->m_iRockets );
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
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iGrenade1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos7b( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iGrenade2 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos7c( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iGrenade3 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8a( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iCrossbow1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8b( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iCrossbow2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAmmos8c( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iCrossbow3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos1 ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iGunAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos2 ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iRevolverAmmos = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos3 ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iUziAmmos = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos4a ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    if ( atoi ( szString ) < 0 )
    {
        pAmmo->m_iShotGunAmmo1 = -1;
    }
    else
    {
        pAmmo->m_iShotGunAmmo1 = atoi ( szString ) * iRiotGunUnits;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos4b ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    if ( atoi ( szString ) < 0 )
    {
        pAmmo->m_iShotGunAmmo2 = -1;
    }
    else
    {
        pAmmo->m_iShotGunAmmo2 = atoi ( szString ) * iRiotGunUnits;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos5 ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    // pAmmo->m_iUnknown = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos6 ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7a ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iGrenade1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7b ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iGrenade2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos7c ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iGrenade3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8a ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iCrossbow1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8b ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iCrossbow2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAmmos8c ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iCrossbow3 = atoi ( szString );
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunRevolver != 0 && ( pGun->m_gunRevolver & TR4NG_MASK_ANY ) == 0 &&
        ( pGun->m_gunRevolver & TR4NG_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Revolver Invalid" );
        return 0;
    }

    if ( pGun->m_gunRiotGun != 0 && ( pGun->m_gunRiotGun & TR4NG_MASK_ANY ) == 0 &&
        ( pGun->m_gunRiotGun & TR4NG_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Riot Gun Invalid" );
        return 0;
    }

    if ( pGun->m_gunUzis != 0 && ( pGun->m_gunUzis & TR4NG_MASK_ANY ) == 0 &&
        ( pGun->m_gunUzis & TR4NG_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Uzi Invalid" );
        return 0;
    }

    if ( pGun->m_gunGrenadesLauncher != 0 && ( pGun->m_gunGrenadesLauncher & TR4NG_MASK_ANY ) == 0  &&
        ( pGun->m_gunGrenadesLauncher & TR4NG_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "Grenade Launcher Invalid" );
        return 0;
    }

    if ( pGun->m_gunCrossBow != 0 && ( pGun->m_gunCrossBow & TR4NG_MASK_ANY ) == 0 &&
        ( pGun->m_gunCrossBow & TR4NG_GUN_SET4 ) == 0 )
    {
        AddToStatus ( "CrossBow Invalid" );
        return 0;
    }

    if ( pGun->m_gunCrowBar != 0 && ( pGun->m_gunCrowBar & TR40NG_CROWBAR_MASK ) == 0  &&
        ( pGun->m_gunCrowBar & TR4NG_GUN_SET6 ) == 0 )
    {
        AddToStatus ( "Crowbar Invalid" );
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunPistol & iMaskGun )
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunRevolver & iMaskRevolver )
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunUzis & iMaskUzi )
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunRiotGun & iMaskRiotGun )
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon6 ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon7 ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunGrenadesLauncher & iMaskGrenade )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon8 ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunCrossBow & iMaskCrossBow )
    {
        return 1;
    }

    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Headset
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::CheckWeapon9 ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( pGun->m_gunCrowBar & iMaskCrowBar )
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    // pGun->cObjects = iMaskCompass;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunPistol;
    if ( ! bChange ) return old;

    pGun->m_gunPistol &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunPistol |= iMaskGun;
    if ( ! bAdd ) pGun->m_gunPistol &= ( iMaskGun ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Return TRUE if bbled
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::EnableGuns ( BOOL bGetOnly, BOOL bEnable )
{
    if ( m_pTRNGStatusNG != NULL )
    {
        if ( bGetOnly )
        {
            return ( *m_pTRNGStatusNG & SNG_DISABLE_WEAPONS ) != SNG_DISABLE_WEAPONS;
        }
        else
        {
            if ( bEnable )
            {
                *m_pTRNGStatusNG &= ( 0xFFFFFFFF ^ SNG_DISABLE_WEAPONS );
                return TRUE;
            }
            else
            {
                *m_pTRNGStatusNG |= SNG_DISABLE_WEAPONS;
                return FALSE;
            }
        }
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Returns TRUE of we are in God Mode
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::TRNGGodMode ( BOOL bGetOnly, BOOL bSet, BOOL bReset )
{
    if ( m_pTRNGStatusNG != NULL )
    {
        if ( bGetOnly )
        {
            return ( *m_pTRNGStatusNG & SNG_IMMORTAL_LARA ) == SNG_IMMORTAL_LARA;
        }
        else
        {
            if ( bReset )
            {
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_IMMORTAL_LARA );
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_INFINITE_AIR );
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_REMOVE_IMMORTAL_LARA );
            }
            else if ( bSet )
            {
                *m_pTRNGStatusNG    |= SNG_IMMORTAL_LARA;
                *m_pTRNGStatusNG    |= SNG_INFINITE_AIR;
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_REMOVE_IMMORTAL_LARA );
                return TRUE;
            }
            else
            {
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_IMMORTAL_LARA );
                *m_pTRNGStatusNG    &= ( 0xFFFFFFFF ^ SNG_INFINITE_AIR );
                *m_pTRNGStatusNG    |= SNG_REMOVE_IMMORTAL_LARA;
                return FALSE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Returns TRUE of we are in God Mode
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::TRNGOpenDoors ( BOOL bGetOnly, BOOL bSet )
{
    if ( m_pTRNGStatusNG != NULL )
    {
        if ( bGetOnly )
        {
            return ( *m_pTRNGStatusNG & SNG_OPEN_ALL_DOORS ) == SNG_OPEN_ALL_DOORS;
        }
        else
        {
            if ( bSet )
            {
                *m_pTRNGStatusNG |= SNG_OPEN_ALL_DOORS;
                return TRUE;
            }
            else
            {
                *m_pTRNGStatusNG &= ( 0xFFFFFFFF ^ SNG_OPEN_ALL_DOORS );
                return FALSE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Returns TRUE of we are in God Mode
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::TRNGKillEnemies ( BOOL bGetOnly, BOOL bSet )
{
    if ( m_pTRNGStatusNG != NULL )
    {
        if ( bGetOnly )
        {
            return ( *m_pTRNGStatusNG & SNG_KILL_ALL_ENEMIES ) == SNG_KILL_ALL_ENEMIES;
        }
        else
        {
            if ( bSet )
            {
                *m_pTRNGStatusNG |= SNG_KILL_ALL_ENEMIES;
                return TRUE;
            }
            else
            {
                *m_pTRNGStatusNG &= ( 0xFFFFFFFF ^ SNG_KILL_ALL_ENEMIES );
                return FALSE;
            }
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunRevolver;
    if ( ! bChange ) return old;

    pGun->m_gunRevolver &= ( TR4NG_GUN_SET1 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunRevolver |= iMaskRevolver;
    if ( ! bAdd ) pGun->m_gunRevolver &= ( iMaskRevolver ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunUzis;
    if ( ! bChange ) return old;

    pGun->m_gunUzis &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunUzis |= iMaskUzi;
    if ( ! bAdd ) pGun->m_gunUzis &= ( iMaskUzi ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunRiotGun;
    if ( ! bChange ) return old;

    pGun->m_gunRiotGun &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunRiotGun |= iMaskRiotGun;
    if ( ! bAdd ) pGun->m_gunRiotGun &= ( iMaskRiotGun ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    // pGun->cObjects |= iMaskMP5;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    // pGun->cObjects |= iMaskRocket;
    return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunGrenadesLauncher;
    if ( ! bChange ) return old;

    pGun->m_gunGrenadesLauncher &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunGrenadesLauncher |= iMaskGrenade;
    if ( ! bAdd ) pGun->m_gunGrenadesLauncher &= ( iMaskGrenade ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunCrossBow;
    if ( ! bChange ) return old;

    pGun->m_gunCrossBow &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunCrossBow |= iMaskCrossBow;
    if ( ! bAdd ) pGun->m_gunCrossBow &= ( iMaskCrossBow ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Grenade Laucher
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR4NGSaveGame::GrabWeapon9 ( int iX, bool bAdd, bool bChange )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    unsigned char old = pGun->m_gunCrowBar;
    if ( ! bChange ) return old;

    pGun->m_gunCrowBar &= ( TR4NG_GUN_SET4 ^ TR4NG_MASK_ALL );
    if ( bAdd ) pGun->m_gunCrowBar |= iMaskCrowBar;
    if ( ! bAdd ) pGun->m_gunCrowBar &= ( iMaskCrowBar ^ TR4NG_MASK_ALL );

    return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetSmallMedipak ( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iSmallMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetLargeMedipak ( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iLargeMedipak  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetFlares ( int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iFlares );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetLaser ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    return ( pGun->m_gunLaserLight );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetBinocular ( int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    return ( pGun->m_gunBinocular );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::GetAir ( )
{

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAir != NULL )
    {
        return (int) *m_pTRNGAir;
    }

    return ( m_pBuffer->iAir );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetSmallMedipak ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
     pAmmo->m_iSmallMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLargeMedipak ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
     pAmmo->m_iLargeMedipak  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetFlares ( const char *szString, int iX )
{
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    pAmmo->m_iFlares = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetLaser ( char *szString, int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    pGun->m_gunLaserLight = atoi (  szString );
    if ( pGun->m_gunLaserLight != 0 )
    {
        pGun->m_gunLaserLight = iMaskLaser;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetBinocular ( char *szString, int iX )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    pGun->m_gunBinocular = atoi (  szString );
    if ( pGun->m_gunBinocular != 0 )
    {
        pGun->m_gunBinocular = iMaskBinocular;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetAir ( const char *szString )
{

    //  For Blind Savegame at least show values
    if ( m_bBlindedTRNG && m_pTRNGAir != NULL )
    {
        // *m_pTRNGAir = atoi ( szString );
        return;
    }

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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    int     iX;
    for ( iX = 0; iX < m_iMaxLevel; iX++ )
    {
        // pGun->cSecrets = 7; /* Always 3 secrets */
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR4NGSaveGame::IsLaraBurning ()
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
void CTR4NGSaveGame::SetLaraBurning (int state)
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
void CTR4NGSaveGame::SetLaraNoBurning (int state)
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
    int levelIndex  = GetLevelIndex() % ( sizeof(TR4NGIndicatorRange) / sizeof(TR_POSITION_RANGE) );;
    int minOffset   = TR4NGIndicatorRange [ levelIndex ].minOffset;
    int maxOffset   = TR4NGIndicatorRange [ levelIndex ].maxOffset;

    //
    BYTE *pBuffer   = ( BYTE * ) m_pBuffer;
    int count       = 0;

    //
    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

    //
    for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
    {
        //  Compare with Indicators
        for ( int indice = 0; indice < IndicatorsTR4NGTableCount;  indice++ )
        {
            if ( IndicatorsTR4NGTable [ indice ].bEnd )
            {
                break;
            }

            //
            //  Reliable index are step = 0
            if ( ! CTRXGlobal::m_iSearchPosExt && IndicatorsTR4NGTable [ indice ].step != 0 )
            {
                break;
            }

            if ( IndicatorsTR4NGTable [ indice ].step > CTRXGlobal::m_TR4NGIndexMaximum )
            {
                continue;
            }

            if (    pBuffer [ iBuffer ] == IndicatorsTR4NGTable [ indice ].b1 &&
                    pBuffer [ iBuffer + 1 ] == IndicatorsTR4NGTable [ indice ].b2 &&
                    pBuffer [ iBuffer + 3 ] == IndicatorsTR4NGTable [ indice ].b4 )
            {
                if ( IndicatorsTR4NGTable [ indice ].useB3 && pBuffer [ iBuffer + 2 ] != IndicatorsTR4NGTable [ indice ].b3 )
                {
                    continue;
                }

                // In TR4 Life is between 0 and 999 (0 means 1000)
                WORD wRealHealth = * (WORD * ) ( pBuffer + iBuffer + TR4NG_REALHEALTH_OFFSET );

                //  Life Is valid between 0 and 1000
                if ( ! IsTR4NGHealthValid ( wRealHealth, false ) )
                {
                    continue;
                }

                //
                count++;
                if ( count > index )
                {
                    strcpy_s (  m_szIndicatorLabel, sizeof(m_szIndicatorLabel), IndicatorsTR4NGTable [ indice ].szLabel );

                    LastGoodIndicator = iBuffer;
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
WORD *CTR4NGSaveGame::GetTR4NGRealHealthAddress()
{
    char *pBuffer   = ( char * ) GetIndicatorAddress();
    if ( pBuffer != NULL )
    {
        WORD *pRealHealth = ( WORD * ) ( pBuffer + TR4NG_REALHEALTH_OFFSET );

        if ( ! IsTR4NGHealthValid ( *pRealHealth, true ) )
        {
            return NULL;
        }

#ifdef _DEBUG
        static char szDebugString [ MAX_PATH ];
        DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer, m_pBuffer );
        sprintf_s ( szDebugString, sizeof(szDebugString),
            "Life Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x H:%-6d\n", 
            dwRelativeAddress, 
            pBuffer [ 0 ] & 0xff, pBuffer [ 1 ] & 0xff, pBuffer [ 2 ] & 0xff, pBuffer [ 3 ] & 0xff,
            *pRealHealth );
        OutputDebugString ( szDebugString );
#endif

        //
        //  Verify Position
        for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
        {
            TR4NG_POSITION *pTR4Position = (TR4NG_POSITION *) ( ( ( BYTE * ) pBuffer - i - TR4NG_POSITION_OFFSET ) );
            
            DWORD dwSouthToNorth    = ( DWORD) pTR4Position->wSouthToNorth * TR4NG_FACTOR;
            DWORD dwVertical        = ( DWORD ) pTR4Position->wVertical * TR4NG_FACTOR;
            DWORD dwWestToEast      = ( DWORD ) pTR4Position->wWestToEast * TR4NG_FACTOR;
            WORD wRoom              = pTR4Position->cRoom;

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
int CTR4NGSaveGame::GetRealHealth ()
{
    //
    WORD *pRealHealth = GetTR4NGRealHealthAddress();
    if ( pRealHealth != NULL )
    {
        WORD wRealHealth = *pRealHealth;
        if ( wRealHealth == TR4_MIN_HEALTH )
        {
            wRealHealth = TR4_MAX_HEALTH;
        }
        return wRealHealth;
    }

    return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR4NGSaveGame::SetRealHealth ( const char *szRealHealth )
{
    //
    WORD *pRealHealth = GetTR4NGRealHealthAddress();
    if ( pRealHealth != NULL )
    {
        WORD wRealHealth = (WORD) atoi(szRealHealth);
        if ( wRealHealth == TR4_MAX_HEALTH )
        {
            wRealHealth = TR4_MIN_HEALTH;
        }

        *pRealHealth = (WORD) wRealHealth;
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
    TR4NGAMMO *pAmmo      = &m_pBuffer->tagAmmo;
    if ( m_bBlindedTRNG && m_pTRNGAmmos != NULL )
    {
        pAmmo    = m_pTRNGAmmos;
    }

    //
    return ( pAmmo->m_iSecretCount );
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
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( item >= 0 && item < sizeof(pGun->m_Object) )
    {
        pGun->m_Object [ item ] = value;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTR4NGSaveGame::GetItems ( int item )
{
    TR4NGGUN    *pGun       = &m_pBuffer->tagGuns;
    if ( m_bBlindedTRNG && m_pTRNGGuns != NULL )
    {
        pGun    = m_pTRNGGuns;
    }

    //
    if ( item >= 0 && item < sizeof(pGun->m_Object) )
    {
        return pGun->m_Object [ item ];
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
    ZeroMemory ( positionTable, sizeof(positionTable) );
    positionCount   = 0;

#ifdef _DEBUG
    OutputDebugString ( "GetTR4NGPosition\n" );
#endif

    ZeroMemory ( m_szIndicatorLabel, sizeof(m_szIndicatorLabel) );

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

        if ( IndicatorsTR4NGTable [ index ].step > CTRXGlobal::m_TR4NGIndexMaximum )
        {
            continue;
        }

        char *pBuffer = (char * ) GetIndicatorAddress(index);
        if ( pBuffer )
        {
            TR4NG_POSITION *pTR4Position0 = (TR4NG_POSITION *) ( ( ( BYTE * ) pBuffer - TR4NG_POSITION_OFFSET ) );

            for ( int i = 0; i <= CTRXGlobal::m_iExtSearchPos; i++ )
            {
                //  We Consider pBuffer - i pointing to indicator1
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
                    //  Low part is subracted from top
                    //  High part is added to bottom
                    //  Extending vertical range
                    //  So y >= yTop - low && y <= yBottom + high
                    dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                    dwExtraVertical         = ( 0x0100 << 16 );
                }
                BOOL bCheck = CheckAreaForCoordinates ( tombraider, levelIndex, wRoom, dwWestToEast, dwVertical, dwSouthToNorth, true, dwExtraVertical );
                if ( bCheck )
                {
#ifdef _DEBUG
                    //  Life is not there
                    WORD wRealHealth = * (WORD * ) ( pBuffer + TR4NG_REALHEALTH_OFFSET );

                    DWORD dwRelativeAddress = CTRXTools::RelativeAddress ( pBuffer - i, m_pBuffer );
                    static char szDebugString [ MAX_PATH ];
                    sprintf_s ( szDebugString, sizeof(szDebugString), 
                        "Indicators 0x%08x : 0x%02x 0x%02x 0x%02x 0x%02x R:%-3u V:%-5d SN:%-5d WE:%-5d D:%-3u H:%-6d\n", 
                        dwRelativeAddress, 
                        pTR4Position0->indicator1, pTR4Position0->indicator2, pTR4Position0->indicator3, pTR4Position0->indicator4, 
                        wRoom, dwVertical, dwSouthToNorth, dwWestToEast, pTR4Position->cOrientation,
                        wRealHealth ); 
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

        //
        TR4NG_POSITION *pCurrent        = NULL;
        TR4NG_POSITION *pTR4Position    = NULL;

        //
        int levelIndex  = GetLevelIndex() % ( sizeof(TR4NGIndicatorRange) / sizeof(TR_POSITION_RANGE) );;
        int minOffset   = TR4NGIndicatorRange [ levelIndex ].minOffset;
        int maxOffset   = TR4NGIndicatorRange [ levelIndex ].maxOffset;

        //
        for ( int iBuffer = minOffset; iBuffer <= maxOffset; iBuffer++ )
        {
            //  We Consider pBuffer + i pointing to indicator1
            pCurrent                = (TR4NG_POSITION *) ( ( BYTE * ) pBuffer + iBuffer );

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
                //  Low part is subracted from top
                //  High part is added to bottom
                //  Extending vertical range
                //  So y >= yTop - low && y <= yBottom + high
                dwExtraVertical         = ( 0x0100 << 16 ) | 0x0100;
                dwExtraVertical         = ( 0x0100 << 16 );
            }
            BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), GetLevelIndex(), wRoom, dwWestToEast, dwVertical, dwSouthToNorth, true, dwExtraVertical );
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
                    TR4NG_POSITION *pTR4Position0    = (TR4NG_POSITION *) ( (char * ) pCurrent + i );

                    //  Life between 0 and 1000
                    WORD wRealHealth = pTR4Position0->health;

                    //
                    if ( IsTR4NGHealthValid ( wRealHealth, false )  )
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
                        wRealHealth ); 
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

        SearchTRNGCoordinates ( localTRPosition.wRoom, localTRPosition.dwWestToEast, localTRPosition.dwSouthToNorth, localTRPosition.dwVertical );

        return &localTRPosition;
    }

    return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR4NGSaveGame::SearchTRNGCoordinates ( WORD wRoom, DWORD dwWestToEast, DWORD dwSouthToNorth, DWORD dwVertical )
{
    m_iCoordinateIndex  = -1;

    if ( m_pCoordinates == NULL )
    {
        return FALSE;
    }

    for ( int i = 0; i < m_iCoordinateCount; i++ )
    {
        if ( m_pCoordinates [ i ].Room != wRoom )
        {
            continue;
        }

        if ( m_pCoordinates [ i ].CordX / 2 != dwWestToEast / 2 )
        {
            continue;
        }

        if ( m_pCoordinates [ i ].CordZ / 2 != dwSouthToNorth / 2 )
        {
            continue;
        }

        if ( m_pCoordinates [ i ].CordY / 2 != dwVertical / 2 )
        {
            continue;
        }

        m_iCoordinateIndex = i;

        break;

    }

    return FALSE;
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
        BOOL bCheck = CheckAreaForCoordinates ( GetFullVersion(), levelIndex, wRoom, dwWestToEast, dwVertical , dwSouthToNorth );
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

            if ( CTRXGlobal::m_bAlterTRNGPosition )
            {
                if ( m_pCoordinates != NULL && m_iCoordinateIndex >= 0 )
                {
                    m_pCoordinates [ m_iCoordinateIndex ].Room      = wRoom;
                    m_pCoordinates [ m_iCoordinateIndex ].CordX     = dwWestToEast;
                    m_pCoordinates [ m_iCoordinateIndex ].CordZ     = dwSouthToNorth;
                    m_pCoordinates [ m_iCoordinateIndex ].CordY     = dwVertical;
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR4NGSaveGame::GetScriptVersion()
{
    return GetTRXScriptVersion();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTR4NGSaveGame::GetSavegameVersion()
{
    return m_SaveGameVersion;
}

