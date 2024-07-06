#pragma once

//  Gun Grid determine guns depending on Level
#define TR1_LEVELS          19
#define TR2_LEVELS          23
#define TR3_LEVELS          26

#define TR1G_START          16
#define TR2G_START          19
#define TR3G_START          21

#define TR4_LEVELS          38
#define TR5_LEVELS          14

#define TR4NGMAXLEVEL       64

#define TR1G_LEVELS (TR1_LEVELS - TR1G_START + 1)
#define TR2G_LEVELS (TR2_LEVELS - TR2G_START + 1)
#define TR3G_LEVELS (TR3_LEVELS - TR3G_START + 1)

extern BOOL GunGrid1 [ TR1_LEVELS ] [ 9 ];
extern BOOL GunGrid15 [ TR1G_LEVELS ] [ 9 ];
extern BOOL GunGrid2 [ TR2_LEVELS ] [ 9 ];
extern BOOL GunGrid25 [ TR2G_LEVELS ] [ 9 ];
extern BOOL GunGrid3 [ TR3_LEVELS ] [ 9 ];
extern BOOL GunGrid35 [ TR3G_LEVELS ] [ 9 ];
extern BOOL GunGrid4 [ TR4_LEVELS ] [ 9 ] ;

extern BOOL GunGrid5 [ TR5_LEVELS ] [ 9 ];

