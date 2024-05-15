#pragma once

#include "../TR_Areas.h"

enum TR_MODE
{
	TR_NONE,
	TRR1_MODE,
	TRR2_MODE,
	TRR3_MODE,
	TR4_MODE,
	TR5_MODE
};


extern BOOL ExtractData (	FILE *hOutputFile, int game,
							const char *pPathname, const char *pName, TR_MODE TRMode, const char *pPrefix,
							FCT_AddToCustomArea function = NULL );