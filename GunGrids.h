#pragma once

#define GAME_TRR1           1
#define GAME_TR10           10
#define GAME_TR15           15
#define GAME_TR1G           15
#define GAME_TRUB           15
#define GAME_TR19           19

#define GAME_TRR2           2
#define GAME_TR20           20
#define GAME_TR25           25
#define GAME_TR2G           25
#define GAME_TR29           29

#define GAME_TRR3           3
#define GAME_TR30           30
#define GAME_TR35           35
#define GAME_TR3G           35
#define GAME_TR39           39

#define GAME_TRR4           4
#define GAME_TR40           40
#define GAME_TR45           45
#define GAME_TR4G           45
#define GAME_TR49           49

#define GAME_TRR5           5
#define GAME_TR50           50
#define GAME_TR55           55
#define GAME_TR59           59

#define GAME_TRR6           6
#define GAME_TR60           60

#define GAME_TRG5           5

//  Custom Sub Game
#define GAME_TRC9           9
#define GAME_TR99           99

//  Gun Grid determine guns depending on Level
#define TR1_LEVELS          19
#define TR2_LEVELS          23
#define TR3_LEVELS          26

#define TR1G_START          16
#define TR2G_START          19
#define TR3G_START          21

#define TR4_LEVELS          40  // With Time Exclusive
#define TR4G_START          39

#define TR5_LEVELS          14

#define TR4NGMAXLEVEL       64

#define MAX_POSITION        64
#define MAX_INDICATORS      128

#define TR1G_LEVELS (TR1_LEVELS - TR1G_START + 1)
#define TR2G_LEVELS (TR2_LEVELS - TR2G_START + 1)
#define TR3G_LEVELS (TR3_LEVELS - TR3G_START + 1)

#define NB_GUNS             9
extern BOOL GunGrid1 [ TR1_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid15 [ TR1G_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid2 [ TR2_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid25 [ TR2G_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid3 [ TR3_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid35 [ TR3G_LEVELS ] [ NB_GUNS ];
extern BOOL GunGrid4 [ TR4_LEVELS ] [ NB_GUNS ] ;

extern BOOL GunGrid5 [ TR5_LEVELS ] [ NB_GUNS ];

