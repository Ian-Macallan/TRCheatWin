// Analyze.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include <Windows.h>

#include "TRXCHEATWIN.h"
#include "TR9SaveGame.h"

CTRXCHEATWINApp theApp;

//
int _tmain(int argc, _TCHAR* argv[])
{
	CTR9SaveGame *pTR = new CTR9SaveGame();

	TraceMode = TRUE;

	if ( argc > 1 )
	{
		pTR->ReadFile ( argv [ 1 ] );
	}
	else
	{
		pTR->ReadFile ( "C:\\Users\\BARUCH_Y.OSIRIS\\AppData\\Roaming\\TRX\\292733983277432559\\savegame.dat" );
	}
	pTR->Load ( );

	return 0;
}

