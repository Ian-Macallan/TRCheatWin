// TR2SaveGame.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXTools.h"

#include "TR2SaveGame.h"
#include "GunGrids.h"
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
static unsigned	TR2Positions [ ] =
{
	0xfc0-0xA6,		/* 01 */
	0x10cc-0xA6,	/* 02 */
	0x1294-0xA6,	/* 03 */
	0x19da-0xA6,	/* 04 */
	0x1000-0xA6,	/* 05 */
	0x1254-0xA6,	/* 06 */
	0xbee-0xA6,		/* 07 */
	0x16c8-0xA6,	/* 08 */
	0xe78-0xA6,		/* 09 */
	0x11b4-0xA6,	/* 10 */
	0x13ee-0xA6,	/* 11 */
	0x1946-0xA6,	/* 12 */
	0x1502-0xA6,	/* 13 */
	0x120a-0xA6,	/* 14 */
	0x1a4a-0xA6,	/* 15 */
	0x11e4-0xA6,	/* 16 */
	0xd10-0xA6,		/* 17 */
	0x103c-0xA6,	/* 18 */
	0xa3f-0xA6,		/* 19 */
	0xf71-0xA6,		/* 20 */
    0x00,			/* 0 */
    0x00,			/* 0 */
    0x00,			/* 0 */
    0x00,			/* 0 */
    0x00,			/* 0 */
};

static unsigned	TR2GPositions [ ] =
{
	0x1b80,				/* 01 */
	0x1b7e,				/* 02 */
	0x1a6e,				/* 03 */
	0x12f0,				/* 04 */
	0x14be,				/* 05 */
	0x00,				/* 06 */
	0x00,				/* 07 */
	0x00,				/* 08 */
	0x00,				/* 09 */
	0x00,				/* 10 */
	0x00,				/* 11 */
	0x00,				/* 12 */
	0x00,				/* 13 */
	0x00,				/* 14 */
	0x00,				/* 15 */
	0x00,				/* 16 */
	0x00,				/* 17 */
	0x00,				/* 18 */
	0x00,				/* 19 */
	0x00,				/* 20 */
    0x00,				/* 0 */
    0x00,				/* 0 */
    0x00,				/* 0 */
    0x00,				/* 0 */
    0x00,				/* 0 */
};

//
/////////////////////////////////////////////////////////////////////////////
// CTR2SaveGame
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR2SaveGame, CTRSaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR2SaveGame::CTR2SaveGame()
{

	m_iVersion			= 20;

	m_iSaveLength		= TR2LEVELMAXSIZE;
	m_iMaxLevel			= TR2MAXLEVEL;

	iMaskCompass		= 0x01;
	iMaskPistol			= 0x02;
	iMaskDesertEagle	= 0x04;
	iMaskUzi            = 0x08;
	MaskShotGun			= 0x10;
	iMaskHarpoon		= 0x20;
	iMaskM16            = 0x40;
	iMaskGrenade		= 0x80;

	m_pLife				= NULL;

	m_pGun				= NULL;

	m_pBuffer			= new ( TR2SAVE );
	ZeroMemory ( m_pBuffer, sizeof(TR2SAVE) );

	m_pBufferBackup		= new ( TR2SAVE );
	ZeroMemory ( m_pBufferBackup, sizeof(TR2SAVE) );

	m_iSubVersion		= 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR2SaveGame::~CTR2SaveGame()
{
	if ( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	if ( m_pBufferBackup )
	{
		delete m_pBufferBackup;
		m_pBuffer = NULL;
	}
}


//
/////////////////////////////////////////////////////////////////////////////
// CTR2SaveGame serialization
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
// CTR2SaveGame commands
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::ReadSavegame( const char *pFilename )
{

    FILE                    *hFile;
    size_t					uLenBuffer;

    char                    szEmpty [ 1 ];

    strcpy_s ( m_Filename, sizeof(m_Filename), pFilename );
	strcpy_s ( m_Status, sizeof(m_Status), "" );

	memset ( ( char * ) m_pBuffer, 0, sizeof ( TR2SAVE ) );

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
    if ( m_iSaveLength <  TR2LEVELMINSIZE || m_iSaveLength > TR2LEVELMAXSIZE )
    {
		strcpy_s ( m_Status, sizeof(m_Status), "Internal error in length." );
		fclose ( hFile );
        return 0;
    }

    memset ( ( char * ) m_pBuffer, 0, sizeof ( TR2SAVE ) );
    uLenBuffer = fread ( ( char * ) m_pBuffer, 1, m_iSaveLength, hFile );
    if ( uLenBuffer != m_iSaveLength )
    {
		strcpy_s ( m_Status, sizeof(m_Status), "File size is not correct." );
		fclose ( hFile );
        return 0;
    }

    if ( fread ( &szEmpty, 1, 1, hFile ) != 0  )
    {
		strcpy_s ( m_Status, sizeof(m_Status), "File size is too large." );
		fclose ( hFile );
        return 0;
    }

	memcpy ( m_pBufferBackup, m_pBuffer, sizeof(TR2SAVE) );

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
void CTR2SaveGame::writeSaveGame()
{
    FILE                    *hFile;
    size_t					uLenBuffer;

	strcpy_s ( m_Status, sizeof(m_Status), "" );

	/*
	 *		Correct guns.
	 */
	int		iX		= getLevelIndex ();

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle ) )
	{
		m_pGun->m_iDesertEagle  = 0 ;
	}

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi ) )
	{
		m_pGun->m_iUzis = 0;
	}

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & MaskShotGun ) )
	{
		m_pGun->m_iRiotGun = 0;
	}

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade ) )
	{
		m_pGun->m_iGrenades = 0;
	}

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon ) )
	{
		m_pGun->m_iHarpoon = 0;
	}

	if ( ! ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskM16 ) )
	{
		m_pGun->m_iM16Gun = 0;
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
		strcpy_s ( m_Status, sizeof(m_Status), "Unable to write the file." );
        return;
    }

    /*
     *      Get Buffer.
     */
    if (  m_iSaveLength < TR2LEVELMINSIZE || m_iSaveLength > TR2LEVELMAXSIZE )
    {
		strcpy_s ( m_Status, sizeof(m_Status), "Internal error in length." );
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
void CTR2SaveGame::RetrieveInformation( const char *pFilename )
{
	char		*pStartAddress;
	char		*pGunAddress;
	char		*pAddress;

	unsigned	iPosition;
	int			iX;
	int			iPos;
	int			iCount;

	int			bExactFound;
	int			bExactGold;

	TR2AMMOS	*pGun;

	/*
	 *	Set address to error.
	 */
	m_pGun			= NULL;

	m_iSubVersion	= 0;

    /*
     *      Read file.
     */
    if ( ReadSavegame ( pFilename ) )
	{
		RetrieveHeader ( );

		if ( m_pBuffer->ind1 == 'P' )
		{
			m_iSubVersion = 5;
		}
		else if ( m_pBuffer->ind1 == 'L' )
		{
			m_iSubVersion = 0;
		}

		/*
		 *		Search for health in file.
		 */
		iPos			= -1;
		iCount			= 0;
		bExactFound		= 0;
		bExactGold		= 0;

		/*
 		 *		Get Level.
		 */
		iX = getLevelIndex ();

		do
		{
			pGun = SearchGunStructure ( m_iGunAmmos, m_pBuffer->trTable [ iX ].cGunBitmap, &iPos );

			if ( pGun != NULL )
			{
				iCount++;

				m_pGun = pGun;
				strcpy_s ( m_Status, sizeof(m_Status), "Data loaded from the saved game.");

				/*
				 *		Test if it it the same as in the table.
				 */
	 			pStartAddress = ( char * ) m_pBuffer;
				pGunAddress = pStartAddress + TR2Positions [ iX ];
				if ( ( char * ) m_pGun == pGunAddress )
				{
					bExactFound = 1;
				}

	 			pStartAddress = ( char * ) m_pBuffer;
				pGunAddress = pStartAddress + TR2GPositions [ iX ];
				if ( ( char * ) m_pGun == pGunAddress )
				{
					bExactGold	= 1;
				}
			}
		}
		while ( iPos >= 0 /* && ! bExactFound && ! bExactGold */ );

		/*
		 *		If the exact position has been found set it.
		 */
		if ( bExactFound )
		{
 			pStartAddress = ( char * ) m_pBuffer;
			pGunAddress   = pStartAddress + TR2Positions [ iX ];
			m_pGun = (TR2AMMOS * ) pGunAddress;
		}

		if ( bExactGold )
		{
 			pStartAddress = ( char * ) m_pBuffer;
			pGunAddress   = pStartAddress + TR2GPositions [ iX ];
			m_pGun = (TR2AMMOS * ) pGunAddress;
		}

		/*
		 *		If nothing found set the position to the default.
		 */
		if ( m_pGun == NULL )
		{
			/*
			 *		Get Level.
			 */
			iX = getLevelIndex ();

 			pStartAddress = ( char * ) m_pBuffer;
			pGunAddress   = pStartAddress + TR2Positions [ iX ];
			m_pGun = (TR2AMMOS * ) pGunAddress;

			sprintf_s ( m_Status, sizeof(m_Status), "Unable to find something in the file: Setting the address %x.",
				TR2Positions [ iX ] );

			m_pGun->m_iGunAmmos = m_iGunAmmos;
			m_pGun = NULL;

			return;
		}
		else
		{
			sprintf_s( m_Status, sizeof(m_Status), "%d good position(s) found.", iCount );
			if ( bExactFound )
			{
				strcat_s ( m_Status, sizeof(m_Status), " Exact position found too." );
			}

			if ( bExactGold )
			{
				strcat_s ( m_Status, sizeof(m_Status), " Exact position found too for gold." );
			}
		}

		/*
		 *		Verify address.
		 */
 		pStartAddress = ( char * ) m_pBuffer;

		/*
		 *		Get Level.
		 */
		iX = getLevelIndex ();

		pGunAddress			= pStartAddress + TR2Positions [ iX ];
		char *pGoldAddress	= pStartAddress + TR2GPositions [ iX ];
		if (   ( char * ) m_pGun != pGunAddress && ( char * ) m_pGun != pGoldAddress )
		{
			pAddress = ( char * ) m_pGun;
			iPosition = (unsigned) ( pAddress - pStartAddress );
			sprintf_s( m_Status, sizeof(m_Status), 
				"The %d address(es) differ(s): Reference is at the address %lx or %lx instead of %lx.",
				iCount, TR2Positions [ iX ], TR2GPositions [ iX ], iPosition );
			if ( false )
			{
				sprintf_s( m_Status, sizeof(m_Status), 
					"The %d address(es) differ(s): Setting the address %lx instead of %lx.",
					iCount, TR2Positions [ iX ], iPosition );
				m_pGun = ( TR2AMMOS * ) pGunAddress;
				m_pGun->m_iGunAmmos = m_iGunAmmos;
			}
		}

	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
TR2AMMOS *CTR2SaveGame::SearchGunStructure ( unsigned short m_iGunAmmos, WORD gunBitmap, int *iPos )
{
        int                     iX;
        unsigned char           *pString;
		TR2AMMOS				*pGun;
		
		if ( *iPos == -1 )
		{
			iX = TR2BUFFEREND - sizeof ( TR2AMMOS );
		}
		else
		{
			iX = *iPos;
		}

        while ( iX >= 0 )
        {
            pString = & ( m_pBuffer->szRemain [ iX ] );
            pGun = ( TR2AMMOS * )(  pString );
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
int CTR2SaveGame::CheckIfAmmosMatch ( TR2AMMOS *pGun, WORD gunBitmap )
{
	int				iX;

	iX = getLevelIndex ();

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle )
	{
		if ( pGun->m_iDesertEagle  != m_iDesertEagle  ) 
		{
			return 0;
		}
	}
	else
	{
		if ( pGun->m_iDesertEagle  != 0 && pGun->m_iDesertEagle != m_iDesertEagle ) 
		{
			return 0;
		}
	}

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi )
	{
		if ( pGun->m_iUzis     != m_iUzis  ) 
		{
			return 0;
		}
	}
	else
	{
		if (    pGun->m_iUzis  != 0 && pGun->m_iUzis  != m_iUzis ) 
		{
			return 0;
		}
	}

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & MaskShotGun )
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

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade )
	{
		if ( pGun->m_iGrenades != m_iGrenades  ) 
		{
			return 0;
		}
	}
	else
	{
		if ( pGun->m_iGrenades != 0 && pGun->m_iGrenades != m_iGrenades) 
		{
			return 0;
		}
	}

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon )
	{
		if ( pGun->m_iHarpoon != m_iHarpoon  )
		{
			return 0;
		}
	}
	else
	{
		if ( pGun->m_iHarpoon != 0 && pGun->m_iHarpoon != m_iHarpoon ) 
		{
			return 0;
		}
	}

	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskM16 )
	{
		if ( pGun->m_iM16Gun != m_iMP5  )
		{
			return 0;
		}
	}
	else
	{
		if ( pGun->m_iM16Gun != 0  && pGun->m_iM16Gun != m_iMP5 ) 
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
void CTR2SaveGame::RetrieveHeader()
{
		/*
		 *		Objects mask.
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
void CTR2SaveGame::GetAmmosValues()
{
		int		iX;

		/*
		 *		Get gun state.
		 */
        iX = getLevelIndex ();

        /*
         *      Get current values for ammos.
         */
        m_iGunAmmos      = m_pBuffer->trTable [ iX ].m_iGunAmmos;
        m_iDesertEagle = m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos;
        m_iRiotGun     = m_pBuffer->trTable [ iX ].m_iRiotGunAmmos;
        m_iUzis        = m_pBuffer->trTable [ iX ].m_iUziAmmos;
        m_iGrenades    = m_pBuffer->trTable [ iX ].m_iGrenadeAmmos;
//      m_iRockets     = m_pBuffer->trTable [ iX ].m_iRockets;
        m_iHarpoon     = m_pBuffer->trTable [ iX ].m_iHarpoonAmmos;
        m_iMP5         = m_pBuffer->trTable [ iX ].m_iM16GunAmmos;


}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::ConvertSecret(int iSecret)
{
	int		iResult;

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
int CTR2SaveGame::ConvertSecretBack(int iSecret)
{
	int		iResult;

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
void CTR2SaveGame::GetDetailedInfo (	char *szGame, size_t iSize, int *iGame, int *iLevel, 
										char *szTitle, size_t iSizeTile)
{
	strcpy_s ( szGame, iSize, "TR2" );
	strcpy_s ( szTitle, iSizeTile, m_pBuffer->szSavename );
	*iGame	= m_pBuffer->iSaveNumber;
	*iLevel	= m_pBuffer->cLevel;
	if ( m_pBuffer->ind1 == 'P' )
	{
		strcpy_s ( szGame, iSize, "TR2G" );
		m_iSubVersion = 5;
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char * CTR2SaveGame::GetStatus()
{
	return ( m_Status );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::getLevel()
{
	return m_pBuffer->cLevel;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::getLevelIndex()
{
	return m_pBuffer->cLevel - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos1(int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iGunAmmos );
	}

	return ( m_pGun->m_iGunAmmos );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos2(int iX )
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
int CTR2SaveGame::GetAmmos3( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iUziAmmos );
	}

	return ( m_pGun->m_iUzis );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos4a ( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iRiotGunAmmos );
	}

	return ( m_pGun->m_iRiotGun );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos5( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iM16GunAmmos );
	}

	return ( m_pGun->m_iM16Gun );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos6( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
//		return ( m_pBuffer->trTable [ iX ].m_iRockets );
	}

//	return ( m_pGun->m_iRockets );

	return ( -1 );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos7a( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iGrenadeAmmos );
	}

	return ( m_pGun->m_iGrenades );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAmmos8a( int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		return ( m_pBuffer->trTable [ iX ].m_iHarpoonAmmos );
	}

	return ( m_pGun->m_iHarpoon );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos2 ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iDesertEagleAmmos = atoi ( szString );
	}

	m_pGun->m_iDesertEagle = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos3 ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iUziAmmos = atoi ( szString );
	}

	m_pGun->m_iUzis = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos4a ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iRiotGunAmmos = atoi ( szString );
	}

	m_pGun->m_iRiotGun = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos5 ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iM16GunAmmos = atoi ( szString );
	}

	m_pGun->m_iM16Gun = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos6 ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
//		m_pBuffer->trTable [ iX ].m_iRockets = atoi ( szString );
	}

//	m_pGun->m_iRockets = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos7a ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iGrenadeAmmos = atoi ( szString );
	}

	m_pGun->m_iGrenades = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAmmos8a ( const char *szString, int iX )
{
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].m_iHarpoonAmmos = atoi ( szString );
	}

	m_pGun->m_iHarpoon = atoi ( szString );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetUnlimitedAmmos(int iMask)
{
	m_pBuffer->cUnlimited = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetUnlimitedAmmos()
{
	return ( m_pBuffer->cUnlimited );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::Valid()
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
void CTR2SaveGame::SetInvalid()
{
	m_pGun = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::CheckWeapon1 ( int iX )
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
int CTR2SaveGame::CheckWeapon4 ( int iX )
{
	if ( m_pBuffer->trTable [ iX ].cGunBitmap & MaskShotGun )
	{
		return 1;
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::CheckWeapon2 ( int iX )
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
int CTR2SaveGame::CheckWeapon3 ( int iX )
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
int CTR2SaveGame::CheckWeapon5 ( int iX )
{
	if ( m_pBuffer->trTable [ iX ].cGunBitmap & iMaskM16 )
	{
		return 1;
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::CheckWeapon6 ( int iX )
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
int CTR2SaveGame::CheckWeapon7 ( int iX )
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
int CTR2SaveGame::CheckWeapon8 ( int iX )
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
unsigned char CTR2SaveGame::GrabWeapon0 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskCompass;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap = iMaskCompass;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskCompass ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon1 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskPistol;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskPistol;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskPistol ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon4 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & MaskShotGun;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= MaskShotGun;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( MaskShotGun ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon2 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskDesertEagle;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskDesertEagle;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskDesertEagle ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon3 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskUzi;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskUzi;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskUzi ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon5 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskM16;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskM16;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskM16 ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon6 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskGrenade;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskGrenade ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon7 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskGrenade;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskGrenade;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskGrenade ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
unsigned char CTR2SaveGame::GrabWeapon8 ( int iX, bool bAdd, bool bChange )
{
	unsigned char old = m_pBuffer->trTable [ iX ].cGunBitmap & iMaskHarpoon;
	if ( ! bChange ) return old;

	if ( bChange ) m_pBuffer->trTable [ iX ].cGunBitmap |= iMaskHarpoon;
	if ( ! bAdd ) m_pBuffer->trTable [ iX ].cGunBitmap &= ( iMaskHarpoon ^ 0xff );
	return old;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetGunAmmos ( )
{
	return ( m_iGunAmmos );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetSmallMedipak ( int iX )
{
	return ( m_pBuffer->trTable [ iX ].cSmallPack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetLargeMedipak ( int iX )
{
	return ( m_pBuffer->trTable [ iX ].cLargePack  );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetFlares ( int iX )
{
	return ( m_pBuffer->trTable [ iX ].cFlare );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetAir ( )
{
	return ( m_pGun->iAir );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetSmallMedipak ( const char *szString, int iX )
{
	 m_pBuffer->trTable [ iX ].cSmallPack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetLargeMedipak ( const char *szString, int iX )
{
	m_pBuffer->trTable [ iX ].cLargePack  = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetFlares ( const char *szString, int iX )
{
	m_pBuffer->trTable [ iX ].cFlare = atoi (  szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAir ( const char *szString )
{
	m_pGun->iAir = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR2SaveGame::GetSaveName ()
{
	return ( m_pBuffer->szSavename );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetSaveNumber ()
{
	return ( m_pBuffer->iSaveNumber );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetCurrentSecrets ( char *szString, int iX )
{
	m_pBuffer->cSecrets = ConvertSecretBack ( atoi ( szString ) );
	if ( iX >= 0 && iX < m_iMaxLevel )
	{
		m_pBuffer->trTable [ iX ].cSecrets = ConvertSecretBack ( atoi ( szString ) );
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAllSecrets ( )
{
	int		iX;

	for ( iX = 0; iX < m_iMaxLevel; iX++ )
	{
		m_pBuffer->trTable [ iX ].cSecrets = 7;	/* Always 3 secrets */
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int	CTR2SaveGame::IsLaraBurning ()
{
	if ( m_pGun != NULL )
	{
		if ( m_pGun->cState & 0x10 )
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
void CTR2SaveGame::SetLaraBurning (int state)
{
	if ( m_pGun != NULL && state != -1 )
	{
		m_pGun->cState &= 0xEF;
		m_pGun->cState |= 0x10;
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetLaraNoBurning (int state)
{
	if ( m_pGun != NULL && state != -1 )
	{
		m_pGun->cState &= 0xEF;
	}
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetAllObjects ()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetMap1 ()
{
	return ( m_pBuffer->cObject_P1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetMap2 ()
{
	return ( m_pBuffer->cObject_P2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetItem1 ()
{
	return ( m_pBuffer->cObjectP1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetItem2 ()
{
	return ( m_pBuffer->cObjectP2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetItem3 ()
{
	return ( m_pBuffer->cObjectP3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetItem4 ()
{
	return ( m_pBuffer->cObjectP4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetKey1 ()
{
	return ( m_pBuffer->cObjectK1 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetKey2 ()
{
	return ( m_pBuffer->cObjectK2 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetKey3 ()
{
	return ( m_pBuffer->cObjectK3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetKey4 ()
{
	return ( m_pBuffer->cObjectK4 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetTreasure1 ( int iX )
{
	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetTreasure2 ( int iX )
{
	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetTreasure3 ( int iX )
{
	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetTreasure4 ( int iX )
{
	return -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetMap1 ( char *szString )
{
	m_pBuffer->cObject_P1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetMap2 ( char *szString )
{
	m_pBuffer->cObject_P2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetItem1 ( char *szString )
{
	m_pBuffer->cObjectP1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetItem2 ( char *szString )
{
	m_pBuffer->cObjectP2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetItem3 ( char *szString )
{
	m_pBuffer->cObjectP3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetItem4 ( char *szString )
{
	m_pBuffer->cObjectP4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetKey1 ( char *szString )
{
	m_pBuffer->cObjectK1 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetKey2 ( char *szString )
{
	m_pBuffer->cObjectK2 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetKey3 ( char *szString )
{
	m_pBuffer->cObjectK3 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetKey4 ( char *szString )
{
	m_pBuffer->cObjectK4 = atoi ( szString );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetTreasure1 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetTreasure2 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetTreasure3 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetTreasure4 ( char *szString, int iX )
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetMaximumSecrets ( int iX )
{
	return ( 3 );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTR2SaveGame::SetGunAmmos ( const char *szGunAmmos )
{
	// Do Nothing.
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR2SaveGame::getBufferAddress ()
{
	return ( ( char * ) m_pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
char *CTR2SaveGame::getBufferBackupAddress ()
{
		return ( ( char * ) m_pBufferBackup );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
size_t CTR2SaveGame::getBufferLength()
{
	return m_iSaveLength;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTR2SaveGame::GetCurrentSecrets ()
{
	return ( ConvertSecret ( m_pBuffer->cSecrets ) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR2SaveGame::BufferModified ()
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
void CTR2SaveGame::discard ()
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
const TR_POSITION *CTR2SaveGame::GetPosition ( )
{
	GetLife ();
	if ( m_pLife != NULL )
	{
		long dwDelta = (long) ( (BYTE* )m_pLife - (BYTE* )m_pBuffer );
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
	//	Search Extended
	if ( CTRXGlobal::m_iSearchPosExt )
	{
		for ( int i = 0xA00; i < 0x2000; i++ )
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
BOOL CTR2SaveGame::SetPosition ( DWORD dwWestToEast, DWORD dwVertical, DWORD dwSouthToNorth, WORD wDirection, WORD wRoom )
{
	GetLife ();

	//	Check if position was good
	if ( GetPosition ( ) == NULL )
	{
		return FALSE;
	}

	if ( m_pLife != NULL )
	{
		long dwDelta = (long)( (BYTE* )m_pLife - (BYTE* )m_pBuffer );
		TR_POSITION *position = ( TR_POSITION *) ( (BYTE *) m_pLife - TR_POSITION_OFFSET );
		position->dwWestToEast		= dwWestToEast;
		position->dwVertical		= dwVertical;
		position->dwSouthToNorth	= dwSouthToNorth;
		position->wOrientation		= wDirection;
		position->wRoom				= wRoom;

		return TRUE;
	}

	return FALSE;
}

