#pragma once

#include "TRSaveGame.h"

#define TR4_MIN_HEALTH      0
#define TR4_MAX_HEALTH      1000
#define TR4_ALT_HEALTH      0x7fff

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
} TR45_INDICATORS;
#pragma pack(pop, pack1)

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

        static BOOL ReadIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
        static BOOL WriteIndicators( TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename );
};

