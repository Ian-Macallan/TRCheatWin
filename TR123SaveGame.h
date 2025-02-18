#pragma once

#include "TRSaveGame.h"

//
/////////////////////////////////////////////////////////////////////////////
//  Base class for Standard TR 1 2 and 3
/////////////////////////////////////////////////////////////////////////////
class CTR123SaveGame : public CTRSaveGame
{
    DECLARE_DYNAMIC(CTR123SaveGame)

    public:
        CTR123SaveGame(void);
        virtual ~CTR123SaveGame(void);
};

