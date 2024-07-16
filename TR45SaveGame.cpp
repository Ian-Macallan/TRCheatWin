#include "stdafx.h"
#include "TR45SaveGame.h"


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTR45SaveGame,CTRSaveGame)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR45SaveGame::CTR45SaveGame(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTR45SaveGame::~CTR45SaveGame(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//  { FALSE,  0x02,   0x02,   0x00,   0x67,   TRUE }
/////////////////////////////////////////////////////////////////////////////
BOOL CTR45SaveGame::ReadIndicators(TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    if ( ! PathFileExists(pFilename) )
    {
        return FALSE;
    }

    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "r" );
    if ( hFile == NULL )
    {
        return FALSE;
    }

    //
    int line    = 0;
    static char szLine [ MAX_PATH ];

    //
    do
    {
        ZeroMemory ( szLine, sizeof(szLine) );
        char *pLine = fgets ( szLine, sizeof(szLine), hFile );
        if ( pLine != NULL )
        {
            //
            TR45_INDICATORS indicators;
            ZeroMemory ( &indicators, sizeof(indicators) );

            //
            char    strDelimit[]    = " \t,{}";
            char    *strToken       = NULL;
            char    *context        = NULL;
            int     value           = 0;

            char *pAccolade         = strchr ( pLine, '{' );
            if ( pAccolade != NULL )
            {
                pLine   = pAccolade + 1;
            }

            //
            //      Treat Tokens
            int index = 0;
            strToken = strtok_s ( pLine, strDelimit, &context);
            while ( strToken != NULL && index < 6 )
            {
                //
                bool bSkip = true;
                while ( bSkip )
                {
                    if ( *strToken == ' ' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '\t' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == ';' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '{' )
                    {
                        strToken++;
                    }
                    else if ( *strToken == '}' )
                    {
                        strToken++;
                    }
                    else
                    {
                        bSkip   = false;
                    }
                }

                //
                if ( _strnicmp ( strToken, "0x", 2 ) == 0 )
                {
                    sscanf_s ( strToken + 2, "%x", &value );
                }
                else if ( _strnicmp ( strToken, "true", strlen("true") ) == 0 )
                {
                    value   = TRUE;
                }
                else if ( _strnicmp ( strToken, "false", strlen("false") ) == 0 )
                {
                    value   = FALSE;
                }
                else
                {
                    value   = atoi(strToken);
                }

                //
                switch ( index )
                {
                    case 0 : indicators.bEnd    = (BOOL) value; break;
                    case 1 : indicators.b1      = (BYTE) value; break;
                    case 2 : indicators.b2      = (BYTE) value; break;
                    case 3 : indicators.b3      = (BYTE) value; break;
                    case 4 : indicators.b4      = (BYTE) value; break;
                    case 5 : indicators.useB3   = (BOOL) value; break;
                }

                //      Get next token:
                strToken = strtok_s( NULL, strDelimit, &context);
                index++;
            }

            //
            //  Add Entry
            if ( line < maxTable )
            {
                IndicatorsTRTable [ line ] = indicators;

                //
                line++;
            }
        }
    }
    while ( ! feof ( hFile ) && ! ferror ( hFile ) );

    //
    //  Add End of entries
    //
    if ( line < maxTable )
    {
        TR45_INDICATORS indicators;
        ZeroMemory ( &indicators, sizeof(indicators) );
        indicators.bEnd     = TRUE;
        indicators.b1       = 0xff;
        indicators.b2       = 0xff;
        indicators.b3       = 0xff;
        indicators.b4       = 0xff;
        indicators.useB3    = TRUE;
        IndicatorsTRTable [ line ] = indicators;
    }

    //
    fclose ( hFile );

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTR45SaveGame::WriteIndicators(TR45_INDICATORS *IndicatorsTRTable, const int maxTable, const char *pFilename)
{
    //
    FILE *hFile = NULL;
    fopen_s ( &hFile, pFilename, "w" );
    if ( hFile == NULL )
    {
        return FALSE;
    }

    //
    int index = 0;
    do
    {
        fprintf_s ( hFile, "{ " );
        TR45_INDICATORS indicator = IndicatorsTRTable [ index ];
        if ( indicator.bEnd )
        {
            fprintf_s ( hFile, "TRUE, " );
        }
        else
        {
            fprintf_s ( hFile, "FALSE, " );
        }

        fprintf_s ( hFile, "0x%02x, ", indicator.b1 );
        fprintf_s ( hFile, "0x%02x, ", indicator.b2 );
        fprintf_s ( hFile, "0x%02x, ", indicator.b3 );
        fprintf_s ( hFile, "0x%02x, ", indicator.b4 );

        if ( indicator.useB3 )
        {
            fprintf_s ( hFile, "TRUE, " );
        }
        else
        {
            fprintf_s ( hFile, "FALSE, " );
        }

        fprintf_s ( hFile, "}\n" );

        if ( indicator.bEnd )
        {
            break;
        }

        index++;
    }
    while ( index < maxTable );

    fclose ( hFile );

    return TRUE;
}
