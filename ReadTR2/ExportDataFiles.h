#pragma once

#include <Windows.h>
#include <io.h>

#include "ReadTRX.h"

extern void ExportDataFiles (   FILE *hOutputFile, const char *pDirectory, int game, TR_MODE TRMode, 
                                const char *pFilespec, const char *pPrefix );