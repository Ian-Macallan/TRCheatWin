#include "stdafx.h"
#include <windows.h>
#include "GunGrids.h"

//
BOOL GunGrid1 [ TR1_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  5
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  10
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  15
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  16  Return to Egypt
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  19
};

BOOL GunGrid15 [ TR1G_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  16  Return to Egypt
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  19
};

BOOL GunGrid2 [ TR2_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  5
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  10
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  15
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   FALSE,  FALSE,  FALSE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  18  Home Sweet Home
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  19  The Cold War
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  20
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  23  Vegas
};

BOOL GunGrid25 [ TR2G_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  19  The Cold War
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  20
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  //  23  Vegas
};

BOOL GunGrid3 [ TR3_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  5
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  10
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  15
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  20  London - All Hallows
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  21  Highland Fling
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  25
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  26  Reunion
};

BOOL GunGrid35 [ TR3G_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  21  Highland Fling
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  25
    {   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   TRUE,   FALSE   },  //  26  Reunion
};

BOOL GunGrid4 [ TR4_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     M16     GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  1
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  Time Exclusive
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  TRUE,   FALSE,  TRUE,   TRUE    },  //  Time Exclusive
};

//  Grappling is in column HAR
BOOL GunGrid5 [ TR5_LEVELS ] [ NB_GUNS ] =
{
    //  PIS     MAG     UZI     RIO     HK      GRE     ROC     HAR     CRO
    {   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 1  STREETS OF ROME
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 2  TRAJAN'S MARKETS
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 3  THE COLOSSEUM
    {   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 4  THE BASE
    {   TRUE,   FALSE,  FALSE,  TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 5  THE SUBMARINE
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 6  DEEPSEA DIVE
    {   TRUE,   TRUE,   TRUE,   TRUE,   FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 7  SINKING SUBMARINE
    {   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 8  GALLOWS TREE
    {   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 9  LEVEL 9: LABYRINTH
    {   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  TRUE    },  // Level 10 OLD MILL
    {   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,  FALSE,  FALSE,  TRUE    },  // Level 11 THE 13th FLOOR
    {   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,  FALSE,  FALSE,  TRUE    },  // Level 12 ESCAPE WITH THE IRIS
    {   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE   },  // Level 13 Security Breach
    {   FALSE,  FALSE,  FALSE,  FALSE,  TRUE,   FALSE,  FALSE,  TRUE,   TRUE    },  // Level 14 RED ALERT!
};

