#pragma once

#include "GunGrids.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
struct TwoLabels
{
    char *label1;
    char *label2;
};

//
struct FourLabels
{
    char *label1;
    char *label2;
    char *label3;
    char *label4;
};


extern FourLabels TR1_Key_Label [TR1_LEVELS];
extern FourLabels TR1_Object_Label [TR1_LEVELS];
extern TwoLabels TR1_Map_Label [TR1_LEVELS];

extern FourLabels TR2_Key_Label [TR2_LEVELS];
extern FourLabels TR2_Object_Label [TR2_LEVELS];
extern TwoLabels TR2_Map_Label [TR2_LEVELS];

extern FourLabels TR3_Key_Label [TR3_LEVELS];
extern FourLabels TR3_Object_Label [TR3_LEVELS];
extern TwoLabels TR3_Map_Label [TR3_LEVELS];
extern FourLabels TR3_Treasure_Label [TR3_LEVELS];

#define     NB_BUTTONS      29
//  TR4
extern char *TR4ItemsName [NB_BUTTONS];

//  TR4
extern char *TR4ItemsName01 [NB_BUTTONS];
//  Race For Iris
#define TR4ItemsName02  TR4ItemsName01

//
extern char *TR4ItemsName03 [NB_BUTTONS];
extern char *TR4ItemsName04 [NB_BUTTONS];
extern char *TR4ItemsName05 [NB_BUTTONS];
extern char *TR4ItemsName06 [NB_BUTTONS];
extern char *TR4ItemsName07 [NB_BUTTONS];
extern char *TR4ItemsName08 [NB_BUTTONS];
extern char *TR4ItemsName09 [NB_BUTTONS];
extern char *TR4ItemsName10 [NB_BUTTONS];
extern char *TR4ItemsName11 [NB_BUTTONS];
extern char *TR4ItemsName12 [NB_BUTTONS];
extern char *TR4ItemsName13 [NB_BUTTONS];
extern char *TR4ItemsName14 [NB_BUTTONS];
extern char *TR4ItemsName15 [NB_BUTTONS];
extern char *TR4ItemsName16 [NB_BUTTONS];
extern char *TR4ItemsName17 [NB_BUTTONS];
extern char *TR4ItemsName18 [NB_BUTTONS];
extern char *TR4ItemsName19 [NB_BUTTONS];
extern char *TR4ItemsName20 [NB_BUTTONS];
extern char *TR4ItemsName21 [NB_BUTTONS];
extern char *TR4ItemsName22 [NB_BUTTONS];
extern char *TR4ItemsName23 [NB_BUTTONS];
extern char *TR4ItemsName24 [NB_BUTTONS];
extern char *TR4ItemsName25 [NB_BUTTONS];
extern char *TR4ItemsName26 [NB_BUTTONS];
extern char *TR4ItemsName27 [NB_BUTTONS];
extern char *TR4ItemsName28 [NB_BUTTONS];
extern char *TR4ItemsName29 [NB_BUTTONS];
extern char *TR4ItemsName30 [NB_BUTTONS];
extern char *TR4ItemsName31 [NB_BUTTONS];
extern char *TR4ItemsName32 [NB_BUTTONS];
extern char *TR4ItemsName33 [NB_BUTTONS];
extern char *TR4ItemsName34 [NB_BUTTONS];
extern char *TR4ItemsName35 [NB_BUTTONS];
extern char *TR4ItemsName36 [NB_BUTTONS];
extern char *TR4ItemsName37 [NB_BUTTONS];
//  Temple Of Horus
#define TR4ItemsName38  TR4ItemsName37

//  TR5
extern char *TR5ItemsName [NB_BUTTONS];

//
extern char *TR5ItemsName01 [NB_BUTTONS];
extern char *TR5ItemsName02 [NB_BUTTONS];
extern char *TR5ItemsName03 [NB_BUTTONS];
extern char *TR5ItemsName04 [NB_BUTTONS];
extern char *TR5ItemsName05 [NB_BUTTONS];
extern char *TR5ItemsName06 [NB_BUTTONS];
extern char *TR5ItemsName07 [NB_BUTTONS];
extern char *TR5ItemsName08 [NB_BUTTONS];
extern char *TR5ItemsName09 [NB_BUTTONS];
extern char *TR5ItemsName10 [NB_BUTTONS];
extern char *TR5ItemsName11 [NB_BUTTONS];
extern char *TR5ItemsName12 [NB_BUTTONS];
extern char *TR5ItemsName14 [NB_BUTTONS];

//  TR4 Custom Levels
extern char *TR49ItemsNameInd [ TR4NGMAXLEVEL ] [ NB_BUTTONS ];
extern char *TR49ItemsNameGen [ NB_BUTTONS ];

extern FourLabels   TR39PuzzleName [ TR4NGMAXLEVEL ];
extern TwoLabels    TR39PickupName [ TR4NGMAXLEVEL ];
extern FourLabels   TR39KeyName [ TR4NGMAXLEVEL ];

extern void AddToItemsLabels ( int what, int level, int button, const char *pText, const char *pTitle );
