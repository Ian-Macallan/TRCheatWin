// ReadTR2.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <Windows.h>
#include <io.h>

#include "ReadTRX.h"


static TR_MODE TRMode = TRR3_MODE;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void TreatDirectory ( FILE *hOutputFile, const char *pDirectory, int game, TR_MODE TRMode, const char *pFilespec, const char *pPrefix )
{
	static char szDirectory [ MAX_PATH ];
	strcpy_s ( szDirectory, sizeof(szDirectory), pDirectory );

	static char szPathname [ MAX_PATH ];
	strcpy_s ( szPathname, sizeof(szPathname), szDirectory );
	strcat_s ( szPathname, sizeof(szPathname), "\\" );
	strcat_s ( szPathname, sizeof(szPathname), pFilespec );

	struct _finddata_t fileinfo;
	intptr_t handle = _findfirst ( szPathname, &fileinfo );
	if ( handle !=  - 1 )
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

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int main(int iArgCount, _TCHAR* pArgValue[])
{
	FILE *hOutputFile = NULL;

	if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR1" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR1_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 10, TRR1_MODE, "*.PHD", "TR1_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "-tr1" ) == 0 )
	{
		if ( iArgCount > 2 )
		{
			hOutputFile = NULL;
			fopen_s ( &hOutputFile, ".\\TR1_Custom_inc.h", "w" );
			if ( hOutputFile != NULL )
			{
				TreatDirectory ( hOutputFile, pArgValue [ 2 ], 99, TRR1_MODE, "*.PHD", "TR1_" );
				fclose ( hOutputFile );
			}
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR1G" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR1G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 15, TRR1_MODE, "*.PHD", "TR1G_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR2" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR2_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 20, TRR2_MODE, "*.tr2", "TR2_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "-tr2" ) == 0 )
	{
		if ( iArgCount > 2 )
		{
			hOutputFile = NULL;
			fopen_s ( &hOutputFile, ".\\TR2_Custom_inc.h", "w" );
			if ( hOutputFile != NULL )
			{
				TreatDirectory ( hOutputFile, pArgValue [ 2 ], 99, TRR2_MODE, "*.tr2", "TR2_" );
				fclose ( hOutputFile );
			}
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR2G" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR2G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\Program Files (x86)\\Core Design\\Tomb Raider II Gold (Full Net)\\data", 25, TRR2_MODE, "*.tr2", "TR2G_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR3" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR3_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 30, TRR3_MODE, "*.tr2", "TR3_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "-tr3" ) == 0 )
	{
		if ( iArgCount > 2 )
		{
			hOutputFile = NULL;
			fopen_s ( &hOutputFile, ".\\TR3_Custom_inc.h", "w" );
			if ( hOutputFile != NULL )
			{
				TreatDirectory ( hOutputFile, pArgValue [ 2 ], 99, TRR3_MODE, "*.tr2", "TR3_" );
				fclose ( hOutputFile );
			}
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR3G" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR3G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 35, TRR3_MODE, "*.tr2", "TR3G_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR4" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR4_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 40, TR4_MODE, "*.tr4", "TR4_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "-tr4" ) == 0 )
	{
		if ( iArgCount > 2 )
		{
			hOutputFile = NULL;
			fopen_s ( &hOutputFile, ".\\TR4_Custom_inc.h", "w" );
			if ( hOutputFile != NULL )
			{
				TreatDirectory ( hOutputFile, pArgValue [ 2 ], 99, TR4_MODE, "*.tr4", "TR4_" );
				fclose ( hOutputFile );
			}
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR4G" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR4G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, 
				"G:\\Program Files (x86)\\Core Design\\The Times - Exclusive Tomb Raider Level\\data", 45, TR4_MODE, "*.tr4", "TR4G_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "TR5" ) == 0 )
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TR5_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "H:\\data", 50, TR5_MODE, "*.trc", "TR5_" );
			fclose ( hOutputFile );
		}
	}
	else if ( iArgCount > 1 && strcmp ( pArgValue [ 1 ], "-tr5" ) == 0 )
	{
		if ( iArgCount > 2 )
		{
			hOutputFile = NULL;
			fopen_s ( &hOutputFile, ".\\TR5_Custom_inc.h", "w" );
			if ( hOutputFile != NULL )
			{
				TreatDirectory ( hOutputFile, pArgValue [ 2 ], 99, TR5_MODE, "*.trc", "TR5_" );
				fclose ( hOutputFile );
			}
		}
	}
	//	Remastered
	else
	{
		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR1_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\1\\DATA", 10, TRR1_MODE, "*.PHD", "TRR1_" );
			fclose ( hOutputFile );
		}

		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR1G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\1\\DATA\\UB", 15, TRR1_MODE, "*.PHD", "TRR1G_" );
			fclose ( hOutputFile );
		}

		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR2_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\2\\DATA", 20, TRR2_MODE, "*.TR2", "TRR2_"  );
			fclose ( hOutputFile );
		}

		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR2G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\2\\DATA\\GM", 25, TRR2_MODE, "*.TR2", "TRR2G_"  );
			fclose ( hOutputFile );
		}

		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR3_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\3\\DATA", 30, TRR3_MODE, "*.TR2", "TRR3_" );
			fclose ( hOutputFile );
		}

		hOutputFile = NULL;
		fopen_s ( &hOutputFile, ".\\TRR3G_Areas_inc.h", "w" );
		if ( hOutputFile != NULL )
		{
			TreatDirectory ( hOutputFile, "G:\\GOG Games\\Tomb Raider I-III Remastered\\3\\DATA\\LA", 35, TRR3_MODE, "*.TR2", "TRR3G_" );
			fclose ( hOutputFile );
		}
	}

	return 0;
}

