#include "stdafx.h"

#include "ExportDataFiles.h"


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void ExportDataFiles (   FILE *hOutputFile, const char *pDirectory, int game, TR_MODE TRMode, 
                                const char *pFilespec, const char *pPrefix )
{
    static char szDirectory [ MAX_PATH ];
    strcpy_s ( szDirectory, sizeof(szDirectory), pDirectory );

    static char szPathname [ MAX_PATH ];
    strcpy_s ( szPathname, sizeof(szPathname), szDirectory );
    strcat_s ( szPathname, sizeof(szPathname), "\\" );
    strcat_s ( szPathname, sizeof(szPathname), pFilespec );

    struct _finddata_t fileinfo;
    intptr_t handle = _findfirst ( szPathname, &fileinfo );
    if ( handle !=  -1 )
    {
        int iRes = 0;
        do
        {
            strcpy_s ( szPathname, sizeof(szPathname), szDirectory );
            strcat_s ( szPathname, sizeof(szPathname), "\\" );
            strcat_s ( szPathname, sizeof(szPathname), fileinfo.name );
            ExtractData ( hOutputFile, game, szPathname, fileinfo.name, TRMode, pPrefix );

            //
            iRes = _findnext ( handle, &fileinfo );
        }
        while ( iRes == 0 );

        _findclose(handle);
    }

}

