#include "stdafx.h"
#include "TRXGlobal.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL            CTRXGlobal::m_iUnchecked            = FALSE;
BOOL            CTRXGlobal::m_iSearchPosExt         = FALSE;
BOOL            CTRXGlobal::m_iSearchExt            = FALSE;
int             CTRXGlobal::m_iExtendVertical       = 0;
int             CTRXGlobal::m_iTRRBackup            = 32;
int             CTRXGlobal::m_iTRBackup             = 4;
int             CTRXGlobal::m_iExtSearchPos  = 3;
unsigned        CTRXGlobal::m_iMinNGSize            = 37400;
unsigned        CTRXGlobal::m_iMaxNGSize            = 37500;

BOOL            CTRXGlobal::m_UseTR4PositionRange   = TRUE;
BOOL            CTRXGlobal::m_UseTR5PositionRange   = TRUE;

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
