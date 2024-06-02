#pragma once

#include "GunGrids.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
#define G_PIS       0       //  Pistols
#define G_MAG       1       //  Magnum
#define G_UZI       2       //  Uzi
#define G_RIO       3       //  Riotgun
#define G_M16       4       //  M16
#define G_GRE       5       //  Grenade
#define G_ROC       6       //  Rocket
#define G_HAR       7       //  Harpoon
#define G_CRO       8       //  Crowbar


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXTools
{
    private:
        CTRXTools(void);
        virtual ~CTRXTools(void);

        //
    public:

        //
        static BOOL IsAmmosEnabled ( int tombraider, int level, int gun );

        //
        static BOOL IsAGunEnabled ( int tombraider, int level, int gun );

        //
        static BOOL IsEnablePistol ( int tombraider, int level );
        static BOOL IsEnableMagnum ( int tombraider, int level );
        static BOOL IsEnableUzi ( int tombraider, int level );
        static BOOL IsEnableRiotgun ( int tombraider, int level );
        static BOOL IsEnableM16 ( int tombraider, int level );
        static BOOL IsEnableGrenade ( int tombraider, int level );
        static BOOL IsEnableRocket ( int tombraider, int level );
        static BOOL IsEnableHarpoon ( int tombraider, int level );
        static BOOL IsEnableCrowbar ( int tombraider, int level );

        static BOOL IsAmmosMagnum ( int tombraider, int level );
        static BOOL IsAmmosUzi ( int tombraider, int level );
        static BOOL IsAmmosRiotgun ( int tombraider, int level );
        static BOOL IsAmmosM16 ( int tombraider, int level );
        static BOOL IsAmmosGrenade ( int tombraider, int level );
        static BOOL IsAmmosRocket ( int tombraider, int level );
        static BOOL IsAmmosHarpoon ( int tombraider, int level );
        static BOOL IsAmmosCrowbar ( int tombraider, int level );

        static int  CountBitsW ( WORD value );
        static int  CountBitsB ( BYTE value );
        static WORD FillBitsW ( WORD value, bool bTR3 = false );
        static BYTE FillBitsB ( BYTE value, bool bTR3 = false );

        static const char *ToBinary ( BYTE value );
        static const char *ToBinary ( WORD value );
        static const char *ToBinary ( DWORD value );

        static DWORD RelativeAddress ( const void *pAddress, const void *pReference );

        static int MemoryCompare ( BYTE *pFirst, BYTE *pSecond, size_t length );
        static int MemoryCompare ( void *pFirst, void *pSecond, size_t length );
        static const char *MemoryDifferences();

        //  Orientation WORD is C000 and will be converted to C0
        static BYTE MakeOrientationToOneByte( WORD value );
        static WORD ConvertOrientationFromDouble ( double dfOrientation );
        static double ConvertOrientationFromWORD ( WORD dfOrientation );

};

