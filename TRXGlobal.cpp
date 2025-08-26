#include "stdafx.h"
#include "TRXGlobal.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL            CTRXGlobal::m_iUnchecked                    = FALSE;
BOOL            CTRXGlobal::m_iSearchPosExt                 = FALSE;
BOOL            CTRXGlobal::m_iSearchExt                    = FALSE;
int             CTRXGlobal::m_iExtendVertical               = 0;
int             CTRXGlobal::m_iTRRBackup                    = 32;
int             CTRXGlobal::m_iTRBackup                     = 4;
int             CTRXGlobal::m_iExtSearchPos                 = 3;
unsigned        CTRXGlobal::m_iMinNGSize                    = 37400;
unsigned        CTRXGlobal::m_iMaxNGSize                    = 37500;
BOOL            CTRXGlobal::m_bAutoSearch                   = TRUE;

BOOL            CTRXGlobal::m_UseTR4PositionRange           = TRUE;
BOOL            CTRXGlobal::m_UseTR5PositionRange           = TRUE;

int             CTRXGlobal::m_TR123IndexMaximum             = TR_SECURE_POSITION;

int             CTRXGlobal::m_TR4IndexMaximum               = TR_SECURE_POSITION;
int             CTRXGlobal::m_TR4NGIndexMaximum             = TR_SECURE_POSITION;

int             CTRXGlobal::m_TR5IndexMaximum               = TR_SECURE_POSITION;

int             CTRXGlobal::m_TRRIndexMaximum               = TR_SECURE_POSITION;

BOOL            CTRXGlobal::m_bTraceTRNGSavegame            = TRUE;
BOOL            CTRXGlobal::m_bAlterTRNGPosition            = TRUE;
BOOL            CTRXGlobal::m_iAlterTRNGIndice              = 84;
BOOL            CTRXGlobal::m_bAlterTRNGAmmosGuns           = FALSE;
ForceGame       CTRXGlobal::m_ForceSaveGame                 = FORCE_NONE;

char            CTRXGlobal::m_szTRXNumber [ 32 ]            = ""; // "292733983277432559";
char            CTRXGlobal::m_szTRX2Number [ 32 ]           = ""; // "72057598332895233";


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXGlobal::CTRXGlobal(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXGlobal::~CTRXGlobal(void)
{
}
