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
        static int          m_iExtendVertical;
        static int          m_iTRRBackup;
        static int          m_iTRBackup;
        static int          m_iExtSearchPos;
        static unsigned     m_iMinNGSize;
        static unsigned     m_iMaxNGSize;

        static BOOL         m_UseTR4PositionRange;
        static BOOL         m_UseTR5PositionRange;

};

extern BOOL     g_bPasteEnabled;
extern DWORD    g_dwWestToEastCopy;
extern DWORD    g_dwVerticalCopy;
extern DWORD    g_dwSouthToNorthCopy;
extern WORD     g_wDirectionCopy;
extern WORD     g_wRoomCopy;
extern int      g_GameCopy;
extern int      g_LevelCopy;
