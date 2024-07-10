#pragma once

#include "TRSaveGame.h"

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
};

//  Indicator Structure for TR 4 and 5
typedef struct indicatorTR45Struct
{
    BOOL    bEnd;
    BYTE    b1;
    BYTE    b2;
    BYTE    b3;
    BYTE    b4;
    BOOL    useB3;
} TR45_INDICATORS;

