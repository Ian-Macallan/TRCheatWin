#include "stdafx.h"

#include <Windows.h>

#include "TRXTools.h"

#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXGlobal.h"
#include "TRXColors.h"
#include "TR_Areas.h"

extern CTRXCHEATWINApp theApp;

static int          DifferencesCount                = 0;
static const int    DifferencesMax                  = 1024;
static int          Differences [ DifferencesMax ]  = { 0 };
static BYTE         *FirstBlock                     = NULL;
static BYTE         *SecondBlock                    = NULL;

//  Length Of Lines is 15
//  HHHHHHHH;HH;HH\n
#define LEN_DIFF_LINE       15
static char szDifference [ ( LEN_DIFF_LINE + 2 ) * DifferencesMax ] = "";

//
//  0 for TRR123
//  1 for TRR456
//  2 for TR1-5
HANDLE CTRXTools::changeNotification [ numberOfChangeNotification ] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

//  1000, 1001, 1002
BOOL CTRXTools::TimerState [ numberOfChangeNotification ] = { FALSE, FALSE, FALSE };

//
// #pragma comment(lib, "uxtheme.lib")

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXTools::CTRXTools(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXTools::~CTRXTools(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::IsAmmosEnabled ( int tombraider, int level, int gun )
{
    if ( CTRXGlobal::m_iUnchecked )
    {
        return TRUE;
    }

    if ( level == 0 )
    {
        level = 1;
    }

    //
    switch ( tombraider )
    {
        //  TR1
        case GAME_TRR1:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR1_LEVELS )
            {
                return GunGrid1 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR10:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR1_LEVELS )
            {
                return GunGrid1 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR HUB
        case GAME_TR15:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR1G_LEVELS )
            {
                return GunGrid15 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR19:
        {
            return TRUE;
            break;
        }

        //  TR2
        case GAME_TRR2:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR2_LEVELS )
            {
                return GunGrid2 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR20:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR2_LEVELS )
            {
                return GunGrid2 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR2 Gold
        case GAME_TR25:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR2G_LEVELS )
            {
                return GunGrid25 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR29:
        {
            return TRUE;
        }

        //
        case GAME_TRR3:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR3_LEVELS )
            {
                return GunGrid3 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR30:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR3_LEVELS )
            {
                return GunGrid3 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR3 Gold
        case GAME_TR35:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR3G_LEVELS )
            {
                return GunGrid35 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR39:
        {
            return TRUE;
            break;
        }

        //  Last Revelations
        case GAME_TRR4:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR4_LEVELS )
            {
                return GunGrid4 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR40:
        case GAME_TR45:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR4_LEVELS )
            {
                return GunGrid4 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR49:
        {
            return TRUE;
            break;
        }

        //  Chronicles
        case GAME_TRR5:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR5_LEVELS )
            {
                return GunGrid5 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR50:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR5_LEVELS )
            {
                return GunGrid5 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR59:
        {
            return TRUE;
            break;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::IsAGunEnabled ( int tombraider, int level, int gun )
{
    if ( CTRXGlobal::m_iUnchecked )
    {
        return TRUE;
    }

    if ( level == 0 )
    {
        level = 1;
    }

    //
    switch ( tombraider )
    {
        //  TR1
        case GAME_TRR1:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR1_LEVELS )
            {
                return GunGrid1 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR10:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR1_LEVELS )
            {
                return GunGrid1 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR HUB
        case GAME_TR15:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR1G_LEVELS )
            {
                return GunGrid15 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR19:
        {
            return TRUE;
            break;
        }

        //  TR2
        case GAME_TRR2:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR2_LEVELS )
            {
                return GunGrid2 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR20:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR2_LEVELS )
            {
                return GunGrid2 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR2 Gold
        case GAME_TR25:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR2G_LEVELS )
            {
                return GunGrid25 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR29:
        {
            return TRUE;
            break;
        }

        //
        case GAME_TRR3:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR3_LEVELS )
            {
                return GunGrid3 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR30:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR3_LEVELS )
            {
                return GunGrid3 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        //  TR3 Gold
        case GAME_TR35:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR3G_LEVELS )
            {
                return GunGrid35 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR39:
        {
            return TRUE;
            break;
        }

        //  Last Revelations
        case GAME_TRR4:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR4_LEVELS )
            {
                return GunGrid4 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR40:
        case GAME_TR45:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR4_LEVELS )
            {
                return GunGrid4 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR49:
        {
            return TRUE;
            break;
        }

        //  Chronicles
        case GAME_TRR5:
        {
            level       = level - 1;
            if ( level >= 0 && level < TR5_LEVELS )
            {
                return GunGrid5 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR50:
        {
            level       = level - 1;
            if ( ! IsCustomArea () && level >= 0 && level < TR5_LEVELS )
            {
                return GunGrid5 [ level ] [ gun ];
            }
            else
            {
                return TRUE;    // MayBe a custom level
            }
            break;
        }

        case GAME_TR59:
        {
            return TRUE;
            break;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::IsAmmosMagnum ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_MAG );
}

BOOL CTRXTools::IsAmmosUzi ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_UZI );
}

BOOL CTRXTools::IsAmmosRiotgun ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_RIO );
}

BOOL CTRXTools::IsAmmosM16 ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_M16 );
}

BOOL CTRXTools::IsAmmosGrenade ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_GRE );
}

BOOL CTRXTools::IsAmmosRocket ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_ROC );
}

BOOL CTRXTools::IsAmmosHarpoon ( int tombraider, int level )
{
    return IsAmmosEnabled ( tombraider, level, G_HAR );
}

BOOL CTRXTools::IsAmmosCrowbar ( int tombraider, int level )
{
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::IsEnablePistol ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_PIS );
}

BOOL CTRXTools::IsEnableMagnum ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_MAG );
}

BOOL CTRXTools::IsEnableUzi ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_UZI );
}

BOOL CTRXTools::IsEnableRiotgun ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_RIO );
}

BOOL CTRXTools::IsEnableM16 ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_M16 );
}

BOOL CTRXTools::IsEnableGrenade ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_GRE );
}

BOOL CTRXTools::IsEnableRocket ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_ROC );
}

BOOL CTRXTools::IsEnableHarpoon ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_HAR );
}

BOOL CTRXTools::IsEnableCrowbar ( int tombraider, int level )
{
    return IsAGunEnabled ( tombraider, level, G_CRO );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXTools::CountBitsW ( WORD value )
{
    int retVal = 0;
    while ( value > 0 )
    {
        if ( ( value & 1 ) == 1 )
        {
            retVal++;
        }
        value /= 2;
    }

    return retVal;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXTools::CountBitsB ( BYTE value )
{
    int retVal = 0;
    while ( value > 0 )
    {
        if ( ( value & 1 ) == 1 )
        {
            retVal++;
        }
        value /= 2;
    }

    return retVal;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTRXTools::FillBitsW ( WORD value, bool bTR3 )
{
    WORD retVal = 0;

    while ( value > 1 )
    {
        retVal  = retVal * 2;
        retVal  = retVal | 1;
        value--;
    }

    if ( value > 0 )
    {
        if ( ! bTR3 )
        {
            retVal  = retVal * 2;
            retVal  = retVal | 1;
        }
        else
        {
            retVal = retVal | 0x20;
        }
        value--;
    }

    return retVal;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTRXTools::FillBitsB ( BYTE value, bool bTR3 )
{
    BYTE retVal = 0;

    while ( value > 1 )
    {
        retVal  = retVal * 2;
        retVal  = retVal | 1;
        value--;
    }

    if ( value > 0 )
    {
        if ( ! bTR3 )
        {
            retVal  = retVal * 2;
            retVal  = retVal | 1;
        }
        else
        {
            retVal = retVal | 0x20;
        }
        value--;
    }

    return retVal;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXTools::ToBinary ( BYTE value )
{
    static char szBinary [ sizeof(value) * 8 + 1 ];
    ZeroMemory ( szBinary, sizeof(szBinary) );

    bool bOne   = false;
    int iPlace  = 0;
    for ( int i = 0; i < sizeof(value) * 8; i++ )
    {
        if ( ( value & 0x80 ) == 0x80 )
        {
            szBinary [ iPlace ] = '1';
            iPlace++;
            bOne            = true;
        }
        else if ( bOne || ( i == sizeof(value) * 8 - 1 ) )
        {
            szBinary [ iPlace ] = '0';
            iPlace++;
        }
        value = value << 1;
    }

    return szBinary;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXTools::ToBinary ( WORD value )
{
    static char szBinary [ sizeof(value) * 8 + 1 ];
    ZeroMemory ( szBinary, sizeof(szBinary) );

    bool bOne   = false;
    int iPlace  = 0;
    for ( int i = 0; i < sizeof(value) * 8; i++ )
    {
        if ( ( value & 0x8000 ) == 0x8000 )
        {
            szBinary [ iPlace ] = '1';
            iPlace++;
            bOne            = true;
        }
        else if ( bOne || ( i == sizeof(value) * 8 - 1 ) )
        {
            szBinary [ iPlace ] = '0';
            iPlace++;
        }
        value = value << 1;
    }

    return szBinary;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXTools::ToBinary ( DWORD value )
{
    static char szBinary [ sizeof(value) * 8 + 1 ];
    ZeroMemory ( szBinary, sizeof(szBinary) );

    bool bOne   = false;
    int iPlace  = 0;
    for ( int i = 0; i < sizeof(value) * 8; i++ )
    {
        if ( ( value & 0x80000000 ) == 0x80000000 )
        {
            szBinary [ iPlace ] = '1';
            iPlace++;
            bOne            = true;
        }
        else if ( bOne || ( i == sizeof(value) * 8 - 1 ) )
        {
            szBinary [ iPlace ] = '0';
            iPlace++;
        }
        value = value << 1;
    }

    return szBinary;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
DWORD CTRXTools::RelativeAddress ( const void *pAddress, const void *pReference )
{
    unsigned long long relative = 0;
    if ( pAddress >= pReference )
    {
        relative = ( unsigned long long ) pAddress - ( unsigned long long ) pReference;
    }
    return ( DWORD ) relative;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Returns 0 if equals else retur the first offset (from 1) that differs
/////////////////////////////////////////////////////////////////////////////
int CTRXTools::MemoryCompare ( BYTE *pFirst, BYTE *pSecond, size_t length )
{
    ZeroMemory ( szDifference, sizeof(szDifference) );
    if ( pFirst == NULL || pSecond == NULL || length == 0 )
    {
        return 0;
    }

    //
    DifferencesCount    = 0;

    FirstBlock      = pFirst;
    SecondBlock     = pSecond;

    //
    int resuit      = 0;
    bool bResultSet = false;

    //
    for ( size_t i = 0; i < length; i++ )
    {
        BYTE cFirst     = pFirst [ i ];
        BYTE cSecond    = pSecond [ i ];
        if ( cFirst < cSecond )
        {
            if ( ! bResultSet )
            {
                resuit      = -1 * ( (int) i + 1 );
                bResultSet  = true;
            }

            if ( DifferencesCount < DifferencesMax )
            {
                Differences [ DifferencesCount ] = (int) i;
                DifferencesCount++;
            }
        }
        else if ( cFirst > cSecond )
        {
            if ( ! bResultSet )
            {
                resuit      = (int) i + 1;
                bResultSet  = true;
            }

            if ( DifferencesCount < DifferencesMax )
            {
                Differences [ DifferencesCount ] = (int) i;
                DifferencesCount++;
            }
        }
    }

    return resuit;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXTools::MemoryCompare ( void *pFirst, void *pSecond, size_t length )
{
    return MemoryCompare ( ( BYTE *) pFirst, ( BYTE *) pSecond, length );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXTools::MemoryDifferences()
{
    ZeroMemory ( szDifference, sizeof(szDifference) );
    if ( FirstBlock == NULL || SecondBlock == NULL)
    {
        return szDifference;
    }

    //
    //  sprintf_s ( szDifference, sizeof(szDifference), "Address  : Old  New\r\n" );
    for ( int i = 0; i < DifferencesCount; i++ )
    {
        size_t len = strlen(szDifference);
        //  Some space left
        if ( len < sizeof(szDifference) - 64 )
        {
            int index = Differences [ i ];
            sprintf_s ( szDifference + len, sizeof(szDifference) - len,
                        "%08X;%02X;%02X\n", index, FirstBlock [ index ], SecondBlock [ index ] );
        }
        else
        {
            break;
        }
    }

    //
    return szDifference;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BYTE CTRXTools::MakeOrientationToOneByte( WORD value )
{
    double dfValue  = value;
    dfValue         /= 256.0;
    dfValue         += 0.5;
    return (BYTE) dfValue;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
WORD CTRXTools::ConvertOrientationFromDouble ( double dfOrientation )
{
    dfOrientation           = dfOrientation * (double) 0x10000;
    dfOrientation           = dfOrientation / 360.0;
    WORD wDirection         = ( WORD ) dfOrientation;
    return wDirection;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
double CTRXTools::ConvertOrientationFromWORD ( WORD wOrientation )
{
    double dfOrientation    = wOrientation;
    dfOrientation           = dfOrientation * 360.0;
    dfOrientation           = dfOrientation / (double) 0x10000;
    return dfOrientation;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXTools::ToLowercase ( char *pText )
{
    if ( pText != NULL )
    {
        while ( *pText != '\0' )
        {
            *pText = tolower(*pText);

            pText++;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXTools::ToUppercase ( char *pText )
{
    if ( pText != NULL )
    {
        while ( *pText != '\0' )
        {
            *pText = toupper(*pText);

            pText++;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HANDLE CTRXTools::TrackFirstChange( int notif, const char *dirname )
{
    notif = notif % numberOfChangeNotification;

    TrackClose ( notif );

    if ( dirname != NULL && strlen(dirname) > 0 && PathFileExists ( dirname ) )
    {
        changeNotification [ notif ] = 
            FindFirstChangeNotification ( dirname, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME |  FILE_NOTIFY_CHANGE_LAST_WRITE );
    }

    return changeNotification [ notif ];
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::TrackNextChange ( int notif )
{
    BOOL bResult = FALSE;

    notif = notif % numberOfChangeNotification;

    if ( changeNotification [ notif ] != INVALID_HANDLE_VALUE )
    {
        bResult = FindNextChangeNotification ( changeNotification );
    }

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//  Can be used un a Timer
//  Call if return TRUE
//  Do The job
//  Then call TrackNextChange
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::TrackCheckChange ( int notif )
{
    BOOL bSignaled = FALSE;

    notif = notif % numberOfChangeNotification;

    DWORD dwWaitStatus = WaitForSingleObject ( changeNotification [ notif ], 0 );
    if ( dwWaitStatus == WAIT_OBJECT_0 )
    {
        bSignaled = TRUE;
    }
    return bSignaled;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::TrackClose ( int notif )
{
    BOOL bResult = FALSE;

    notif = notif % numberOfChangeNotification;

    if ( changeNotification [ notif ] != INVALID_HANDLE_VALUE )
    {
        bResult = FindCloseChangeNotification ( changeNotification [ notif ] );
        changeNotification [ notif ]  = INVALID_HANDLE_VALUE;
    }

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::StartTimer ( HWND hWnd, int notif )
{
    if ( notif >= 0 && notif <= 2 )
    {
        if ( ! TimerState [ notif ] )
        {
            SetTimer ( hWnd, notif + 1000, 3500, NULL );

            TimerState [ notif ] = TRUE;
        }
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXTools::StopTimer ( HWND hWnd, int notif )
{
    if ( notif >= 0 && notif <= 2 )
    {
        if ( TimerState [ notif ] )
        {
            KillTimer ( hWnd, notif + 1000 );
        }

        TimerState [ notif ] = FALSE;
    }

    //
    return TRUE;
}
