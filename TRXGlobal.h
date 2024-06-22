#pragma once

#include <Windows.h>

#define TR_NG_MIN_SIZE          33000
#define TR_NG_MAX_SIZE          53000

//
/////////////////////////////////////////////////////////////////////////////
class CTRXGlobal
{
    private :
        CTRXGlobal(void);
        ~CTRXGlobal(void);

    public :
        static BOOL         m_iUnchecked;
        static BOOL         m_iSearchExt;
        static BOOL         m_iSearchPosExt;

        static unsigned     m_iMinNGSize;
        static unsigned     m_iMaxNGSize;

};

extern BOOL     g_bPasteEnabled;
extern DWORD    g_dwWestToEastCopy;
extern DWORD    g_dwVerticalCopy;
extern DWORD    g_dwSouthToNorthCopy;
extern WORD     g_wDirectionCopy;
extern WORD     g_wRoomCopy;
extern int      g_GameCopy;
extern int      g_LevelCopy;
