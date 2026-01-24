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
enum ForceGame
{
    FORCE_NONE,
    FORCE_TR1,
    FORCE_TUB,
    FORCE_TR2,
    FORCE_TR2G,
    FORCE_TR3,
    FORCE_TR3G,
    FORCE_TR4,
    FORCE_TR4G,
    FORCE_TR4NG,
    FORCE_TR5,
    FORCE_TRR123,
    FORCE_TRR456,
};

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

        static BOOL         m_bAutoSearch;
        static BOOL         m_bWatchFiles;

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
        static BOOL         m_iAlterTRNGIndice;
        static BOOL         m_bAlterTRNGAmmosGuns;

        static ForceGame    m_ForceSaveGame;

        static int          m_CheckAmmosTolerance;

        static char         m_szTRXNumber [ 32 ];
        static char         m_szTRX2Number [ 32 ];
};

//
//  Global Variables for Copy / Paste position
//  Can be used in remastered or normal games
extern BOOL     g_bPasteEnabled;
extern DWORD    g_dwWestToEastCopy;
extern DWORD    g_dwVerticalCopy;
extern DWORD    g_dwSouthToNorthCopy;
extern WORD     g_wDirectionCopy;
extern WORD     g_wRoomCopy;
extern int      g_GameCopy;
extern int      g_LevelCopy;
