// TRXPosition.cpp : fichier d'implémentation
//

#include "stdafx.h"
// #include "afxdialogex.h"
#include <math.h>

#include "TRXPosition.h"
#include "TR8SaveGame.h"
#include "TR9SaveGame.h"
#include "TR_Areas.h"
#include "TRXMapAreas.h"
#include "TRXGlobal.h"
#include "TRXColors.h"

// Boîte de dialogue CTRXPosition


static const int RoomDivider    = 150;
static const int RoomMargin     = 10;

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPosition, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPosition::CTRXPosition(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXPosition::IDD, pParent)
{
    m_iArea         = -1;
    m_123           = FALSE;
    m_456           = FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPosition::~CTRXPosition()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_WEST_EAST, m_West_East);
    DDX_Control(pDX, IDC_WEST_EAST_M, m_West_East_M);
    DDX_Control(pDX, IDC_WEST_EAST_OLD, m_West_East_Old);
    DDX_Control(pDX, IDC_WEST_EAST_RANGE, m_West_East_Range);

    DDX_Control(pDX, IDC_VERTICAL, m_Vertical);
    DDX_Control(pDX, IDC_VERTICAL_M, m_Vertical_M);
    DDX_Control(pDX, IDC_VERTICAL_OLD, m_Vertical_Old);
    DDX_Control(pDX, IDC_VERTICAL_RANGE, m_Vertical_Range);

    DDX_Control(pDX, IDC_SOUTH_NORTH, m_South_North);
    DDX_Control(pDX, IDC_SOUTH_NORTH_M, m_South_North_M);
    DDX_Control(pDX, IDC_SOUTH_NORTH_OLD, m_South_North_Old);
    DDX_Control(pDX, IDC_SOUTH_NORTH_RANGE, m_South_North_Range);

    DDX_Control(pDX, IDC_WORD0, m_Word0);
    DDX_Control(pDX, IDC_WORD0_X, m_Word0_X);
    DDX_Control(pDX, IDC_WORD0_OLD, m_Word0_Old);

    DDX_Control(pDX, IDC_ORIENTATION, m_Word1);
    DDX_Control(pDX, IDC_ORIENTATION_D, m_Word1_D);
    DDX_Control(pDX, IDC_ORIENTATION_OLD, m_Word1_Old);

    DDX_Control(pDX, IDC_WORD2, m_Word2);
    DDX_Control(pDX, IDC_WORD2_X, m_Word2_X);
    DDX_Control(pDX, IDC_WORD2_OLD, m_Word2_Old);

    DDX_Control(pDX, IDC_WORD3, m_Word3);
    DDX_Control(pDX, IDC_WORD3_X, m_Word3_X);
    DDX_Control(pDX, IDC_WORD3_OLD, m_Word3_Old);

    DDX_Control(pDX, IDC_SPECIAL, m_Word4);
    DDX_Control(pDX, IDC_SPECIAL_X, m_Word4_X);
    DDX_Control(pDX, IDC_SPECIAL_OLD, m_Word4_Old);

    DDX_Control(pDX, IDC_WORD5, m_Word5);
    DDX_Control(pDX, IDC_WORD5_X, m_Word5_X);
    DDX_Control(pDX, IDC_WORD5_OLD, m_Word5_Old);

    DDX_Control(pDX, IDC_SEARCH, m_Search);
    DDX_Control(pDX, IDC_AREAS, m_AreasList);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDC_MAP, m_Map);
    DDX_Control(pDX, IDC_RANGES, m_Ranges);
    DDX_Control(pDX, IDC_COPYPOS, m_CopyPos);
    DDX_Control(pDX, IDC_PASTEPOS, m_PastePos);
    DDX_Control(pDX, IDC_LEFT, m_Left);
    DDX_Control(pDX, IDC_RIGHT, m_Right);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPosition, CTRXDialogBase)
    ON_BN_CLICKED(IDOK, &CTRXPosition::OnBnClickedOk)
    ON_BN_CLICKED(IDC_SEARCH, &CTRXPosition::OnBnClickedSearch)
    ON_CBN_SELCHANGE(IDC_AREAS, &CTRXPosition::OnSelchangeAreas)
    ON_BN_CLICKED(IDC_MAP, &CTRXPosition::OnBnClickedMap)
    ON_EN_CHANGE(IDC_WEST_EAST_M, &CTRXPosition::OnChangeWestEastM)
    ON_EN_CHANGE(IDC_SOUTH_NORTH_M, &CTRXPosition::OnChangeSouthNorthM)
    ON_EN_CHANGE(IDC_ORIENTATION_D, &CTRXPosition::OnChangeOrientationD)
    ON_BN_CLICKED(IDC_COPYPOS, &CTRXPosition::OnBnClickedCopypos)
    ON_BN_CLICKED(IDC_PASTEPOS, &CTRXPosition::OnBnClickedPastepos)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXPosition
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPosition::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    static char szText [ MAX_PATH ];
    static char szLabel [ MAX_PATH ];

    //
    if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
    {
        //
        m_Left.SetWindowText ( "North" );
        m_Right.SetWindowText ( "South" );

        //
        int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
        int levelIndex      = levelNumber - 1;
        const char *pLevelName = CTR9SaveGame::GetLevelName ( m_iTombraider, levelNumber );
        SetWindowText ( pLevelName );

        TR9_POSITION *pDwAddress = ( TR9_POSITION *) CTR9SaveGame::I()->GetPositionAddress ( m_iTombraider, m_iBlock );
        if ( pDwAddress != NULL )
        {
            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwWestToEast );
            m_West_East.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwWestToEast / POSITION_DIVIDER );
            m_West_East_M.SetWindowText ( szText );
            m_West_East_Old.SetWindowText ( szText );

            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwVertical );
            m_Vertical.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwVertical / POSITION_DIVIDER );
            m_Vertical_M.SetWindowText ( szText );
            m_Vertical_Old.SetWindowText ( szText );

            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwSouthToNorth );
            m_South_North.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwSouthToNorth / POSITION_DIVIDER );
            m_South_North_M.SetWindowText ( szText );
            m_South_North_Old.SetWindowText ( szText );

            //  Word 0
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething0 );
            m_Word0.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething0 );
            m_Word0_X.SetWindowText ( szText );
            m_Word0_Old.SetWindowText ( szText );

            //  Orientation
            double dfOrientation = CTRXTools::ConvertOrientationFromWORD ( pDwAddress->wOrientation );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wOrientation );
            m_Word1.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", dfOrientation );
            m_Word1_D.SetWindowText ( szText );
            m_Word1_Old.SetWindowText ( szText );

            //  Word 2
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething2 );
            m_Word2.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething2 );
            m_Word2_X.SetWindowText ( szText );
            m_Word2_Old.SetWindowText ( szText );

            //  Word 3
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething3 );
            m_Word3.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething3 );
            m_Word3_X.SetWindowText ( szText );
            m_Word3_Old.SetWindowText ( szText );

            //  Special
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wRoom );
            m_Word4.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wRoom );
            m_Word4_X.SetWindowText ( szText );
            m_Word4_Old.SetWindowText ( szText );

            //  Word 5
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething5 );
            m_Word5.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething5 );
            m_Word5_X.SetWindowText ( szText );
            m_Word5_Old.SetWindowText ( szText );

            //
            m_West_East_M.SetReadOnly ( FALSE );
            m_Vertical_M.SetReadOnly ( FALSE );
            m_South_North_M.SetReadOnly ( FALSE );

            m_Word0_X.SetReadOnly ( FALSE );
            m_Word1_D.SetReadOnly ( FALSE );
            m_Word2_X.SetReadOnly ( FALSE );
            m_Word3_X.SetReadOnly ( FALSE );
            m_Word4_X.SetReadOnly ( FALSE );
            m_Word5_X.SetReadOnly ( FALSE );

            //
            int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            int currentArea     = -1;

            //  Find may n ot be correct
            currentArea = FindAreaForCoordinates ( m_iTombraider, levelIndex, pDwAddress->dwWestToEast, pDwAddress->dwVertical, pDwAddress->dwSouthToNorth );
            currentArea = pDwAddress->wRoom;

            //  Combo
            long curX           = 0;
            long curZ           = 0;
            long curXSectors    = 0;
            long curZSectors    = 0;
            long curYTop        = 0;
            long curYBottom     = 0;

            const char *pLabel = NULL;
            m_AreasList.ResetContent();
            TR_AREA trArea;
            ZeroMemory ( &trArea, sizeof( trArea) );
            int index           = 0;
            while ( EnumAreaForCoordinates ( m_iTombraider, levelIndex, index, &trArea ) )
            {
                ZeroMemory ( szLabel, sizeof(szLabel) );
                if ( pLabel != NULL && strlen(pLabel) > 0 )
                {
                    strcpy_s ( szLabel, sizeof(szLabel), pLabel );
                }
                else
                {
                    ZeroMemory ( szLabel, sizeof(szLabel) );
                    if ( ( trArea.flags & 0x01 ) == 0x01 )      // Bit 0
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Full of Water; " );
                    }
                    if ( ( trArea.flags & 0x08) == 0x08 )   //  Bit 3
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Open Area; " );
                    }
                    if ( ( trArea.flags & 0x80 ) == 0x80 )      //  Bit 7
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Quicksand / Mud; " );
                    }
                    if ( ( trArea.flags & 0x200 ) == 0x200 )    //  Bit 9
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Some Water; " );
                    }

                    if ( abs(trArea.yBottom - trArea.yTop) < LOW_CEILING )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Low Ceiling; " );
                    }

                    {
#ifdef _DEBUG
                        sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), "Flags : 0x%X", trArea.flags );
#endif
                    }
                }
                sprintf_s ( szText, sizeof(szText), "%03d : [%ld,%ld], [%ld,%ld], [%ld,%ld] %s",
                            trArea.index,
                            trArea.x,
                            trArea.x + TR_SECTOR_SIZE * trArea.xSectors,
                            trArea.z,
                            trArea.z + TR_SECTOR_SIZE * trArea.zSectors, trArea.yTop, trArea.yBottom, szLabel );
                m_AreasList.AddString ( szText );
                if ( trArea.index == currentArea )
                {
                //
                    TR_CUR_POSITION                 currentPosition;
                    ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                    currentPosition.x               = pDwAddress->dwWestToEast;
                    currentPosition.z               = pDwAddress->dwSouthToNorth;
                    currentPosition.orientation     = dfOrientation;

                    m_Room.SetAreaAndPosition ( &trArea, &currentPosition );

                    m_iArea     = currentArea;

                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.x, trArea.x + trArea.xSectors * TR_SECTOR_SIZE, trArea.xSectors );
                    m_West_East_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld ", trArea.yTop, trArea.yBottom );
                    m_Vertical_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.z, trArea.z + trArea.zSectors * TR_SECTOR_SIZE, trArea.zSectors );
                    m_South_North_Range.SetWindowText ( szText );

                    curX            = trArea.x;
                    curZ            = trArea.z;
                    curXSectors     = trArea.xSectors;
                    curZSectors     = trArea.zSectors;
                    curYTop         = trArea.yTop;
                    curYBottom      = trArea.yBottom;
                }
                index++;
            }

            if ( currentArea != 0XFFFF )
            {
                m_SetManualCombo    = false;
                m_AreasList.SetCurSel ( currentArea );
                m_SetManualCombo = true;
            }

            //  Create the Room
            RECT rectRanges;
            m_Ranges.GetWindowRect ( &rectRanges );
            ScreenToClient ( &rectRanges );
            RECT roomRect;
            roomRect.top        = rectRanges.bottom + RoomMargin;
            roomRect.left       = rectRanges.left + RoomMargin;
            roomRect.bottom     = roomRect.top + curXSectors * TR_SECTOR_SIZE / RoomDivider;
            roomRect.right      = roomRect.left + curZSectors * TR_SECTOR_SIZE / RoomDivider;

            m_Room.SetPointMode ( TRUE );
            CTRXRoomPicture::CreateInstanceInside ( this, &m_Room, roomRect, "", SS_NOTIFY );

            //
            m_Room.SetRoomPoint ( ComputeRoomPoint (), &dfOrientation );
            BOOL bShow = m_Room.ShowWindow ( SW_NORMAL );
        }
    }

    //
    if ( m_456 && CTR8SaveGame::I(FALSE) != NULL )
    {
        //
        m_Left.SetWindowText ( "South" );
        m_Right.SetWindowText ( "North" );

        //
        int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
        int levelIndex      = levelNumber - 1;
        const char *pLevelName = CTR8SaveGame::GetLevelName ( m_iTombraider, levelNumber );
        SetWindowText ( pLevelName );

        const TR_POSITION *pDwAddress = CTR8SaveGame::I()->GetPosition ( m_iTombraider, m_iBlock );
        if ( pDwAddress != NULL )
        {
            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwWestToEast );
            m_West_East.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwWestToEast / POSITION_DIVIDER );
            m_West_East_M.SetWindowText ( szText );
            m_West_East_Old.SetWindowText ( szText );

            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwVertical );
            m_Vertical.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwVertical / POSITION_DIVIDER );
            m_Vertical_M.SetWindowText ( szText );
            m_Vertical_Old.SetWindowText ( szText );

            //
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%08X", pDwAddress->dwSouthToNorth );
            m_South_North.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) pDwAddress->dwSouthToNorth / POSITION_DIVIDER );
            m_South_North_M.SetWindowText ( szText );
            m_South_North_Old.SetWindowText ( szText );

            //  Word 0
#if 0
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething0 );
            m_Word0.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething0 );
            m_Word0_X.SetWindowText ( szText );
            m_Word0_Old.SetWindowText ( szText );
#endif
            //  Orientation
            double dfOrientation = CTRXTools::ConvertOrientationFromWORD ( pDwAddress->wOrientation );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wOrientation );
            m_Word1.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%.3f", dfOrientation );
            m_Word1_D.SetWindowText ( szText );
            m_Word1_Old.SetWindowText ( szText );

            //  Word 2
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething2 );
            m_Word2.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething2 );
            m_Word2_X.SetWindowText ( szText );
            m_Word2_Old.SetWindowText ( szText );

            //  Word 3
#if 0
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething3 );
            m_Word3.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething3 );
            m_Word3_X.SetWindowText ( szText );
            m_Word3_Old.SetWindowText ( szText );
#endif
            //  Special
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wRoom );
            m_Word4.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wRoom );
            m_Word4_X.SetWindowText ( szText );
            m_Word4_Old.SetWindowText ( szText );

            //  Word 5
#if 0
            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%04X", pDwAddress->wSomething5 );
            m_Word5.SetWindowText ( szText );

            ZeroMemory ( szText, sizeof(szText) );
            sprintf_s ( szText, sizeof(szText), "%d",  pDwAddress->wSomething5 );
            m_Word5_X.SetWindowText ( szText );
            m_Word5_Old.SetWindowText ( szText );
#endif
            //
            m_West_East_M.SetReadOnly ( FALSE );
            m_Vertical_M.SetReadOnly ( FALSE );
            m_South_North_M.SetReadOnly ( FALSE );

            m_Word0_X.SetReadOnly ( FALSE );
            m_Word1_D.SetReadOnly ( FALSE );
            m_Word2_X.SetReadOnly ( FALSE );
            m_Word3_X.SetReadOnly ( FALSE );
            m_Word4_X.SetReadOnly ( FALSE );
            m_Word5_X.SetReadOnly ( FALSE );

            //
            int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            int currentArea     = -1;

            //  Find may n ot be correct
            currentArea = FindAreaForCoordinates ( m_iTombraider, levelIndex, pDwAddress->dwWestToEast, pDwAddress->dwVertical, pDwAddress->dwSouthToNorth );
            currentArea = pDwAddress->wRoom;

            //  Combo
            long curX           = 0;
            long curZ           = 0;
            long curXSectors    = 0;
            long curZSectors    = 0;
            long curYTop        = 0;
            long curYBottom     = 0;

            const char *pLabel = NULL;
            m_AreasList.ResetContent();
            TR_AREA trArea;
            ZeroMemory ( &trArea, sizeof( trArea) );
            int index           = 0;
            while ( EnumAreaForCoordinates ( m_iTombraider, levelIndex, index, &trArea ) )
            {
                ZeroMemory ( szLabel, sizeof(szLabel) );
                if ( pLabel != NULL && strlen(pLabel) > 0 )
                {
                    strcpy_s ( szLabel, sizeof(szLabel), pLabel );
                }
                else
                {
                    ZeroMemory ( szLabel, sizeof(szLabel) );
                    if ( ( trArea.flags & 0x01 ) == 0x01 )      // Bit 0
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Full of Water; " );
                    }
                    if ( ( trArea.flags & 0x08) == 0x08 )   //  Bit 3
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Open Area; " );
                    }
                    if ( ( trArea.flags & 0x80 ) == 0x80 )      //  Bit 7
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Quicksand / Mud; " );
                    }
                    if ( ( trArea.flags & 0x200 ) == 0x200 )    //  Bit 9
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Some Water; " );
                    }

                    if ( abs(trArea.yBottom - trArea.yTop) < LOW_CEILING )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), "Low Ceiling; " );
                    }

                    {
#ifdef _DEBUG
                        sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), "Flags : 0x%X", trArea.flags );
#endif
                    }
                }
                sprintf_s ( szText, sizeof(szText), "%03d : [%ld,%ld], [%ld,%ld], [%ld,%ld] %s",
                            trArea.index,
                            trArea.x,
                            trArea.x + TR_SECTOR_SIZE * trArea.xSectors,
                            trArea.z,
                            trArea.z + TR_SECTOR_SIZE * trArea.zSectors, trArea.yTop, trArea.yBottom, szLabel );
                m_AreasList.AddString ( szText );
                if ( trArea.index == currentArea )
                {
                //
                    TR_CUR_POSITION                 currentPosition;
                    ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                    currentPosition.x               = pDwAddress->dwWestToEast;
                    currentPosition.z               = pDwAddress->dwSouthToNorth;
                    currentPosition.orientation     = dfOrientation;

                    m_Room.SetAreaAndPosition ( &trArea, &currentPosition );

                    m_iArea     = currentArea;

                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.x, trArea.x + trArea.xSectors * TR_SECTOR_SIZE, trArea.xSectors );
                    m_West_East_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld ", trArea.yTop, trArea.yBottom );
                    m_Vertical_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.z, trArea.z + trArea.zSectors * TR_SECTOR_SIZE, trArea.zSectors );
                    m_South_North_Range.SetWindowText ( szText );

                    curX            = trArea.x;
                    curZ            = trArea.z;
                    curXSectors     = trArea.xSectors;
                    curZSectors     = trArea.zSectors;
                    curYTop         = trArea.yTop;
                    curYBottom      = trArea.yBottom;
                }
                index++;
            }

            if ( currentArea != 0XFFFF )
            {
                m_SetManualCombo    = false;
                m_AreasList.SetCurSel ( currentArea );
                m_SetManualCombo = true;
            }

            //  Create the Room
            RECT rectRanges;
            m_Ranges.GetWindowRect ( &rectRanges );
            ScreenToClient ( &rectRanges );
            RECT roomRect;
            roomRect.top        = rectRanges.bottom + RoomMargin;
            roomRect.left       = rectRanges.left + RoomMargin;
            roomRect.bottom     = roomRect.top + curXSectors * TR_SECTOR_SIZE / RoomDivider;
            roomRect.right      = roomRect.left + curZSectors * TR_SECTOR_SIZE / RoomDivider;

            m_Room.SetPointMode ( TRUE );
            CTRXRoomPicture::CreateInstanceInside ( this, &m_Room, roomRect, "", SS_NOTIFY );

            //
            m_Room.SetRoomPoint ( ComputeRoomPoint (), &dfOrientation );
            BOOL bShow = m_Room.ShowWindow ( SW_NORMAL );
        }
    }

    if ( m_bToolTip )
    {
        const char *pTools = "You will have to adjust thos values\r\nto get a correct position\r\n";
        m_ToolTip.AddTool( &m_Search, "Search Correct Room for Position");
        m_ToolTip.AddTool( &m_AreasList, "List Of All Rooms (Area) for this level");
        m_ToolTip.AddTool( &m_OK, "Validate your changes\r\nBeware the values could be wrong\r\n");
        m_ToolTip.AddTool( &m_Word4_X, "Use Search to find\r\nthe associate ares (room)" );
        m_ToolTip.AddTool( &m_Word1_D, 
            "Orientation \n(For 123 : Facing : South=0, East=90, \nNorth=180, West=270)\n(For 45 : Facing : North=0)\n" );
        m_ToolTip.AddTool( &m_West_East_M, pTools);
        m_ToolTip.AddTool( &m_South_North_M, pTools);
        m_ToolTip.AddTool( &m_Vertical_M, pTools);
        m_ToolTip.AddTool( &m_Room, "You can click on the bitmap\r\nto position Lara precisely\r\nGenerally walls are 1.0 width\r\n" );
        m_ToolTip.AddTool( &m_CopyPos, "Copy Position" );
        m_ToolTip.AddTool( &m_PastePos, "Paste Position" );

        m_ToolTip.Activate(TRUE);

    }

    //
    if ( CTRXColors::m_iDarkTheme != 0 )
    {
        m_CopyPos.SetIconResource ( IDI_COPY );
        m_PastePos.SetIconResource ( IDI_PASTE );
    }
    else
    {
        m_CopyPos.SetIconResource ( IDI_COPY_BLACK );
        m_PastePos.SetIconResource ( IDI_PASTE_BLACK );
    }


    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::ResizeRoom ( long x, long z, long xSectors, long zSectors, long yTop, long yBottom )
{
    //
    RECT roomRect;
    m_Room.GetWindowRect ( &roomRect );
    ScreenToClient ( &roomRect );
    InvalidateRect ( &roomRect );
    roomRect.bottom     = roomRect.top + xSectors * TR_SECTOR_SIZE / RoomDivider;
    roomRect.right      = roomRect.left + zSectors * TR_SECTOR_SIZE / RoomDivider;
    m_Room.MoveWindow ( &roomRect, TRUE );
    m_Room.SetRoomPoint ( ComputeRoomPoint () );
    InvalidateRect ( &roomRect );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CPoint CTRXPosition::ComputeRoomPoint ( )
{
    static char szTextX [ 64 ];
    static char szTextZ [ 64 ];

    m_West_East_M.GetWindowText ( szTextX, sizeof(szTextX) );
    m_South_North_M.GetWindowText ( szTextZ, sizeof(szTextZ) );

    //
    CPoint point;
    point = m_Room.ComputeRoomPoint ( szTextX, szTextZ );

    //
    return point;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::ClickRoom ( UINT nFlags, CPoint point )
{
    static char szText [ 64 ];
    double dfX = 0;
    double dfZ = 0;

    m_Room.GetXZ ( point, dfX, dfZ );

    //
    sprintf_s ( szText, sizeof(szText), "%.3f", dfX );
    m_West_East_M.SetWindowText ( szText );

    sprintf_s ( szText, sizeof(szText), "%.3f", dfZ );
    m_South_North_M.SetWindowText ( szText );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnBnClickedOk()
{
    //
    static char szText [ 64 ];

    //
    if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
    {
        TR9_POSITION *pDwAddress = ( TR9_POSITION *) CTR9SaveGame::I()->GetPositionAddress ( m_iTombraider, m_iBlock );
        if ( pDwAddress != NULL )
        {
            double      dfValue = 0.0;
            unsigned    wValue = 0;

            //
            m_West_East_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            pDwAddress->dwWestToEast    = (DWORD) ( dfValue * POSITION_DIVIDER );

            //
            m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            pDwAddress->dwVertical  = (DWORD) ( dfValue * POSITION_DIVIDER );

            //
            m_South_North_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            pDwAddress->dwSouthToNorth  = (DWORD) ( dfValue * POSITION_DIVIDER );

            //  Word0
            m_Word0_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            pDwAddress->wSomething0  = wValue;

            //  Orientation
            m_Word1_D.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            wValue  = CTRXTools::ConvertOrientationFromDouble ( dfValue );
            pDwAddress->wOrientation = wValue;

            //  Word2
            m_Word2_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            pDwAddress->wSomething2  = wValue;

            //  Word3
            m_Word3_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            pDwAddress->wSomething3  = wValue;


            //  Special
            m_Word4_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            pDwAddress->wRoom  = wValue;

            //  Word5
            m_Word5_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            pDwAddress->wSomething5  = wValue;

        }
    }

    //
    if ( m_456 && CTR8SaveGame::I(FALSE) != NULL )
    {
        const TR_POSITION *pDwAddress = CTR8SaveGame::I()->GetPosition ( m_iTombraider, m_iBlock );
        if ( pDwAddress != NULL )
        {
            double      dfValue = 0.0;
            unsigned    wValue = 0;

            DWORD   dwWestToEast    = pDwAddress->dwWestToEast;
            DWORD   dwVertical      = pDwAddress->dwVertical;
            DWORD   dwSouthToNorth  = pDwAddress->dwSouthToNorth;
            //  0 : ?
            //  1 : Orientation 0x0000 = facing white 0xffff facing red (North)
            //  2 : ??
            //  3 : Room
            WORD    wOrientation    = pDwAddress->wOrientation;
            WORD    wRoom           = pDwAddress->wRoom;

            //
            m_West_East_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            dwWestToEast    = (DWORD) ( dfValue * POSITION_DIVIDER );

            //
            m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            dwVertical  = (DWORD) ( dfValue * POSITION_DIVIDER );

            //
            m_South_North_M.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            dwSouthToNorth  = (DWORD) ( dfValue * POSITION_DIVIDER );

            //  Word0
            m_Word0_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            // pDwAddress->wSomething0  = wValue;

            //  Orientation
            m_Word1_D.GetWindowText ( szText, sizeof(szText) );
            dfValue = atof ( szText );
            wValue  = CTRXTools::ConvertOrientationFromDouble ( dfValue );
            wOrientation = wValue;

            //  Word2
            m_Word2_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            // position.wSomething2  = wValue;

            //  Word3
            m_Word3_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            // pDwAddress->wSomething3  = wValue;


            //  Special
            m_Word4_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            wRoom  = wValue;

            //  Word5
            m_Word5_X.GetWindowText ( szText, sizeof(szText) );
            if ( strncmp ( szText, "0x", 2 ) == 0 )
            {
                sscanf_s ( szText + 2, "%x", &wValue );
            }
            else if ( strncmp ( szText, "x", 1 ) == 0 )
            {
                sscanf_s ( szText + 1, "%x", &wValue );
            }
            else
            {
                sscanf_s ( szText, "%d", &wValue );
            }
            // pDwAddress->wSomething5  = wValue;

            CTR8SaveGame::I()->SetPosition ( m_iTombraider, m_iBlock, dwWestToEast, dwVertical, dwSouthToNorth, wOrientation, wRoom );
        }
    }

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnBnClickedSearch()
{
    //
    static char szText [ 64 ];
    double      dfValue = 0.0;
    if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
    {
        int levelNumber = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
        int levelIndex  = levelNumber - 1;

        m_West_East_M.GetWindowText ( szText, sizeof(szText) );
        dfValue = atof ( szText );
        DWORD dwWestToEast  = (DWORD) ( dfValue * POSITION_DIVIDER );

        //
        m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
        dfValue = atof ( szText );
        DWORD dwVertical    = (DWORD) ( dfValue * POSITION_DIVIDER );

        //
        m_South_North_M.GetWindowText ( szText, sizeof(szText) );
        dfValue = atof ( szText );
        DWORD dwSouthToNorth    = (DWORD) ( dfValue * POSITION_DIVIDER );

        WORD area = FindAreaForCoordinates ( m_iTombraider, levelIndex, dwWestToEast, dwVertical, dwSouthToNorth );
        if ( area != 0XFFFF )
        {
            sprintf_s ( szText, sizeof(szText), "%d", area );
            m_Word4_X.SetWindowText ( szText );

            m_iArea = area;

            m_Room.SetRoomPoint ( ComputeRoomPoint () );
        }
        else
        {
            MessageBox ( "Room/Area Not Found" );
            m_iArea = -1;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::ChangeAreas( int room )
{
    if ( room != -1 )
    {
        int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
        int levelIndex      = levelNumber - 1;
        TR_AREA trArea;
        ZeroMemory ( &trArea, sizeof(trArea) );
        BOOL bDone = EnumAreaForCoordinates ( m_iTombraider, levelIndex, room, &trArea );
        if ( bDone )
        {
            ResizeRoom ( trArea.x, trArea.z, trArea.xSectors, trArea.zSectors, trArea.yTop, trArea.yBottom );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::ChangeAreas()
{
    //
    static char szText [ 64 ];

    if ( m_SetManualCombo )
    {
        int iCurSel = m_AreasList.GetCurSel();
        if ( iCurSel != - 1 )
        {
            //
            if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
            {
                int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
                int levelIndex      = levelNumber - 1;
                TR_AREA trArea;
                ZeroMemory ( &trArea, sizeof(trArea) );
                BOOL bDone = EnumAreaForCoordinates ( m_iTombraider, levelIndex, iCurSel, &trArea );
                if ( bDone )
                {
                    //
                    m_iArea             = trArea.index;

                    //
                    long newX           = ( trArea.x + trArea.x + trArea.xSectors * TR_SECTOR_SIZE ) / 2;
                    long newZ           = ( trArea.z + trArea.z + trArea.zSectors * TR_SECTOR_SIZE ) / 2;
                    long newY           = trArea.yBottom - FROM_THE_GROUND;

                    double dfX          =  (double) (long) newX / POSITION_DIVIDER;
                    double dfZ          =  (double) (long) newZ / POSITION_DIVIDER;
                    double dfY          =  (double) (long) newY / POSITION_DIVIDER;
                
                    //
                    m_Word1_D.GetWindowText ( szText, sizeof(szText) );
                    double dfOrientation = atof(szText);

                    TR_CUR_POSITION                 currentPosition;
                    ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                    currentPosition.x               = newX;
                    currentPosition.z               = newZ;
                    currentPosition.orientation     = dfOrientation;

                    //
                    m_Room.SetAreaAndPosition ( &trArea, &currentPosition );

                    //  X
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newX );
                    m_West_East.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfX );
                    m_West_East_M.SetWindowText ( szText );

                    //  T
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newY );
                    m_Vertical.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfY );
                    m_Vertical_M.SetWindowText ( szText );

                    //  Z
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newZ );
                    m_South_North.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfZ );
                    m_South_North_M.SetWindowText ( szText );

                    //  Area
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%04X", trArea.index );
                    m_Word4.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%d",  trArea.index );
                    m_Word4_X.SetWindowText ( szText );

                    //  Show Ranges
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.x, trArea.x + trArea.xSectors * TR_SECTOR_SIZE, trArea.xSectors );
                    m_West_East_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld ", trArea.yTop, trArea.yBottom );
                    m_Vertical_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.z, trArea.z + trArea.zSectors * TR_SECTOR_SIZE, trArea.zSectors );
                    m_South_North_Range.SetWindowText ( szText );

                    ResizeRoom ( trArea.x, trArea.z, trArea.xSectors, trArea.zSectors, trArea.yTop, trArea.yBottom );
                }
            }

            //
            if ( m_456 && CTR8SaveGame::I(FALSE) != NULL )
            {
                int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
                int levelIndex      = levelNumber - 1;
                TR_AREA trArea;
                ZeroMemory ( &trArea, sizeof(trArea) );
                BOOL bDone = EnumAreaForCoordinates ( m_iTombraider, levelIndex, iCurSel, &trArea );
                if ( bDone )
                {
                    //
                    m_iArea             = trArea.index;

                    //
                    long newX           = ( trArea.x + trArea.x + trArea.xSectors * TR_SECTOR_SIZE ) / 2;
                    long newZ           = ( trArea.z + trArea.z + trArea.zSectors * TR_SECTOR_SIZE ) / 2;
                    long newY           = trArea.yBottom - FROM_THE_GROUND;

                    double dfX          =  (double) (long) newX / POSITION_DIVIDER;
                    double dfZ          =  (double) (long) newZ / POSITION_DIVIDER;
                    double dfY          =  (double) (long) newY / POSITION_DIVIDER;
                
                    //
                    m_Word1_D.GetWindowText ( szText, sizeof(szText) );
                    double dfOrientation = atof(szText);

                    TR_CUR_POSITION                 currentPosition;
                    ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                    currentPosition.x               = newX;
                    currentPosition.z               = newZ;
                    currentPosition.orientation     = dfOrientation;

                    //
                    m_Room.SetAreaAndPosition ( &trArea, &currentPosition );

                    //  X
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newX );
                    m_West_East.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfX );
                    m_West_East_M.SetWindowText ( szText );

                    //  T
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newY );
                    m_Vertical.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfY );
                    m_Vertical_M.SetWindowText ( szText );

                    //  Z
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%08X", newZ );
                    m_South_North.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%.3f", dfZ );
                    m_South_North_M.SetWindowText ( szText );

                    //  Area
                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%04X", trArea.index );
                    m_Word4.SetWindowText ( szText );

                    ZeroMemory ( szText, sizeof(szText) );
                    sprintf_s ( szText, sizeof(szText), "%d",  trArea.index );
                    m_Word4_X.SetWindowText ( szText );

                    //  Show Ranges
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.x, trArea.x + trArea.xSectors * TR_SECTOR_SIZE, trArea.xSectors );
                    m_West_East_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld ", trArea.yTop, trArea.yBottom );
                    m_Vertical_Range.SetWindowText ( szText );
                    sprintf_s ( szText, sizeof(szText), "%ld , %ld, %ld ",
                        trArea.z, trArea.z + trArea.zSectors * TR_SECTOR_SIZE, trArea.zSectors );
                    m_South_North_Range.SetWindowText ( szText );

                    ResizeRoom ( trArea.x, trArea.z, trArea.xSectors, trArea.zSectors, trArea.yTop, trArea.yBottom );
                }
            }

        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnSelchangeAreas()
{
    //
    ChangeAreas();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnBnClickedMap()
{
    //
    static char szText [ MAX_PATH ];

    //
    CTRXMapAreas dlg;
    dlg.SetTombraiderLevelIndex ( m_iTombraider, m_iLevel - 1 );

    //
    if ( m_iArea >= 0 )
    {
        if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
        {
            int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( m_iTombraider, levelIndex, m_iArea );
            if ( pArea )
            {
                m_West_East_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwWestEast        = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_South_North_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwSouthNorth      = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwVertical        = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_Word1_D.GetWindowText ( szText, sizeof(szText) );
                double dfOrientation    =  atof(szText);

                TR_CUR_POSITION currentPosition;
                ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                currentPosition.x               = dwWestEast;
                currentPosition.y               = dwVertical;
                currentPosition.z               = dwSouthNorth;
                currentPosition.orientation     = dfOrientation;

                dlg.SetCurrentArea ( pArea, &currentPosition );
            }
        }

        if ( m_456 && CTR8SaveGame::I(FALSE) != NULL )
        {
            int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( m_iTombraider, levelIndex, m_iArea );
            if ( pArea )
            {
                m_West_East_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwWestEast        = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_South_North_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwSouthNorth      = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
                DWORD dwVertical        = (DWORD) ( atof(szText) * POSITION_DIVIDER);

                m_Word1_D.GetWindowText ( szText, sizeof(szText) );
                double dfOrientation    =  atof(szText);

                TR_CUR_POSITION currentPosition;
                ZeroMemory ( &currentPosition, sizeof(currentPosition) );
                currentPosition.x               = dwWestEast;
                currentPosition.y               = dwVertical;
                currentPosition.z               = dwSouthNorth;
                currentPosition.orientation     = dfOrientation;

                dlg.SetCurrentArea ( pArea, &currentPosition );
            }
        }
    }

    INT_PTR area = dlg.DoModal();
    if ( area != -1 )
    {
        m_AreasList.SetCurSel ( (int) area );
        ChangeAreas();
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnChangeWestEastM()
{
    if ( m_bInitDone )
    {
        RECT roomRect;
        m_Room.GetWindowRect ( &roomRect );
        ScreenToClient ( &roomRect );
        m_Room.SetRoomPoint ( ComputeRoomPoint () );
        InvalidateRect ( &roomRect );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnChangeSouthNorthM()
{
    if ( m_bInitDone )
    {
        RECT roomRect;
        m_Room.GetWindowRect ( &roomRect );
        ScreenToClient ( &roomRect );
        m_Room.SetRoomPoint ( ComputeRoomPoint () );
        InvalidateRect ( &roomRect );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnChangeOrientationD()
{
    if ( m_bInitDone )
    {
        char szText [ 32 ];
        m_Word1_D.GetWindowText ( szText, sizeof(szText) );
        double dfOrientation = atof(szText);
        m_Room.SetOrientation ( dfOrientation );
        RECT roomRect;
        m_Room.GetWindowRect ( &roomRect );
        ScreenToClient ( &roomRect );
        InvalidateRect ( &roomRect );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnBnClickedCopypos()
{
    //
    char szText [ 64 ];

    m_West_East_M.GetWindowText ( szText, sizeof(szText) );
    double dfValue = atof ( szText );
    g_dwWestToEastCopy    = (DWORD) ( dfValue * POSITION_DIVIDER );

    //
    m_Vertical_M.GetWindowText ( szText, sizeof(szText) );
    dfValue = atof ( szText );
    g_dwVerticalCopy  = (DWORD) ( dfValue * POSITION_DIVIDER );

    //
    m_South_North_M.GetWindowText ( szText, sizeof(szText) );
    dfValue = atof ( szText );
    g_dwSouthToNorthCopy  = (DWORD) ( dfValue * POSITION_DIVIDER );

    //  Orientation
    m_Word1_D.GetWindowText ( szText, sizeof(szText) );
    dfValue = atof ( szText );
    unsigned wValue  = CTRXTools::ConvertOrientationFromDouble ( dfValue );
    g_wDirectionCopy = wValue;

    //  Room
    m_Word4_X.GetWindowText ( szText, sizeof(szText) );
    if ( strncmp ( szText, "0x", 2 ) == 0 )
    {
        sscanf_s ( szText + 2, "%x", &wValue );
    }
    else if ( strncmp ( szText, "x", 1 ) == 0 )
    {
        sscanf_s ( szText + 1, "%x", &wValue );
    }
    else
    {
        sscanf_s ( szText, "%d", &wValue );
    }

    g_wRoomCopy         = wValue;
    g_GameCopy          = m_iTombraider;
    g_LevelCopy         = m_iLevel;
    g_bPasteEnabled     = TRUE;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPosition::OnBnClickedPastepos()
{
    //
    char szText [ 64 ];

    if ( g_bPasteEnabled )
    {
        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) g_dwWestToEastCopy / POSITION_DIVIDER );
        m_West_East_M.SetWindowText ( szText );

        //
        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) g_dwVerticalCopy / POSITION_DIVIDER );
        m_Vertical_M.SetWindowText ( szText );

        //
        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%.3f", (double) (long) g_dwSouthToNorthCopy / POSITION_DIVIDER );
        m_South_North_M.SetWindowText ( szText );

        //  Orientation
        double dfOrientation = CTRXTools::ConvertOrientationFromWORD ( g_wDirectionCopy );
        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%.3f", dfOrientation );
        m_Word1_D.SetWindowText ( szText );

        //  Special
        ZeroMemory ( szText, sizeof(szText) );
        sprintf_s ( szText, sizeof(szText), "%d",  g_wRoomCopy );
        m_Word4_X.SetWindowText ( szText );

        TR_CUR_POSITION                 currentPosition;
        ZeroMemory ( &currentPosition, sizeof(currentPosition) );
        currentPosition.x               = g_dwWestToEastCopy;
        currentPosition.z               = g_dwSouthToNorthCopy;
        currentPosition.orientation     = dfOrientation;

        if ( m_123 && CTR9SaveGame::I(FALSE) != NULL )
        {
            int levelNumber     = CTR9SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( m_iTombraider, levelIndex, g_wRoomCopy );
            if ( pArea )
            {
                m_Room.SetAreaAndPosition ( pArea, &currentPosition );
            }
        }

        if ( m_456 && CTR8SaveGame::I(FALSE) != NULL )
        {
            int levelNumber     = CTR8SaveGame::I()->GetBlockLevelNumber ( m_iTombraider, m_iBlock );
            int levelIndex      = levelNumber - 1;
            TR_AREA *pArea = GetTRArea ( m_iTombraider, levelIndex, g_wRoomCopy );
            if ( pArea )
            {
                m_Room.SetAreaAndPosition ( pArea, &currentPosition );
            }
        }
        //
        ChangeAreas ( g_wRoomCopy );
    }
}
