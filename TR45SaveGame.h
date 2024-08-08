#pragma once

#include "TRSaveGame.h"

#define TR4_MIN_HEALTH      0
#define TR4_MAX_HEALTH      1000
#define TR4_ALT_HEALTH      0x7fff
//  If bit 9 is Set in Health this means invisible
#define TR4_INVISIBLE       0x100

#define TR5_MIN_HEALTH      0
#define TR5_MAX_HEALTH      1000
#define TR5_ALT_HEALTH      0x7fff

//  Indicator Structure for TR 4 and 5
#pragma pack(push, pack1, 1)
typedef struct indicatorTR45Struct
{
    BOOL    bEnd;
    BYTE    b1;
    BYTE    b2;
    BYTE    b3;
    BYTE    b4;
    BOOL    useB3;
    //  0 for reliable
    //  1-8 for usable
    //  9 for unusable
    int     step;
    char    szLabel [ INDICATOR_LABEL_SIZE ];
} TR45_INDICATORS;
#pragma pack(pop, pack1)

//
//
static const int MinTR4PositionOffset   = 0x280;
static const int MaxTR4PositionOffset   = 0x3000;

static const int MinTR5PositionOffset   = 0x280;
static const int MaxTR5PositionOffset   = 0xD00;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTR45SaveGame : public CTRSaveGame
{
    DECLARE_DYNAMIC(CTR45SaveGame)

    public:
        CTR45SaveGame(void);
        virtual ~CTR45SaveGame(void);

        static int ReadIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static BOOL WriteIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static int CountIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MinIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable );
        static int MaxIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable );
};

