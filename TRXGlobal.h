#pragma once

#include <Windows.h>

#define TR_NG_MIN_SIZE              16442
#define TR_NG_MAX_SIZE              53000

/////////////////////////////////////////////////////////////////////////////

//  For Step
//  0 is secure
//  1-7 nearly secure
//  8 not sure
//  9 not effective
#define TR_SECURE_POSITION          7

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

        //  Index for search
        static int          m_TR123IndexMaximum;
        static int          m_TR4IndexMaximum;
        static int          m_TR4NGIndexMaximum;
        static int          m_TR5IndexMaximum;
        static int          m_TRRIndexMaximum;

        static BOOL         m_bTraceTRNGSavegame;
        static BOOL         m_bAlterTRNGPosition;
        static BOOL         m_bAlterTRNGIndice;
};

extern BOOL     g_bPasteEnabled;
extern DWORD    g_dwWestToEastCopy;
extern DWORD    g_dwVerticalCopy;
extern DWORD    g_dwSouthToNorthCopy;
extern WORD     g_wDirectionCopy;
extern WORD     g_wRoomCopy;
extern int      g_GameCopy;
extern int      g_LevelCopy;
