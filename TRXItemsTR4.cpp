// TRXItemsTR4.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include <math.h>

#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TRXItemsTR4.h"
#include "afxdialogex.h"

#include "TRXTools.h"
#include "TRLabelItems.h"
#include "resource.h"

//
static  UINT IDArray [ ] =
{
    IDC_ITEM_01, IDC_ITEM_02, IDC_ITEM_03, IDC_ITEM_04, IDC_ITEM_05, IDC_ITEM_06, IDC_ITEM_07,
    IDC_ITEM_08, IDC_ITEM_09, IDC_ITEM_10, IDC_ITEM_11, IDC_ITEM_12, IDC_ITEM_13, IDC_ITEM_14,
    IDC_ITEM_15, IDC_ITEM_16, IDC_ITEM_17, IDC_ITEM_18, IDC_ITEM_19, IDC_ITEM_20, IDC_ITEM_21,
    IDC_ITEM_22, IDC_ITEM_23, IDC_ITEM_24, IDC_ITEM_25, IDC_ITEM_26, IDC_ITEM_27, IDC_ITEM_28,
    IDC_ITEM_29,
};

//
static  UINT NIDArray [ ] =
{
    IDC_NITEM_01, IDC_NITEM_02, IDC_NITEM_03, IDC_NITEM_04, IDC_NITEM_05, IDC_NITEM_06, IDC_NITEM_07,
    IDC_NITEM_08, IDC_NITEM_09, IDC_NITEM_10, IDC_NITEM_11, IDC_NITEM_12, IDC_NITEM_13, IDC_NITEM_14,
    IDC_NITEM_15, IDC_NITEM_16, IDC_NITEM_17, IDC_NITEM_18, IDC_NITEM_19, IDC_NITEM_20, IDC_NITEM_21,
    IDC_NITEM_22, IDC_NITEM_23, IDC_NITEM_24, IDC_NITEM_25, IDC_NITEM_26, IDC_NITEM_27, IDC_NITEM_28,
    IDC_NITEM_29,
};

static  UINT IDMenuItem [] =
{
    ID_ITEMS_ITEM1, ID_ITEMS_ITEM2, ID_ITEMS_ITEM3, ID_ITEMS_ITEM4, 
    ID_ITEMS_ITEM5, ID_ITEMS_ITEM6, ID_ITEMS_ITEM7, ID_ITEMS_ITEM8, 
};

//
static CTRXButtonBase buttomItems [ 29 ];

//
static const    char *MessageTitle = "Tombraider Standard Editions";

//
#define MAX_MENU_LABEL          8
#define MAX_MENU_LABEL_STRING   256

static char     menuLabel [ MAX_MENU_LABEL ][ MAX_MENU_LABEL_STRING ];

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXItemsTR4
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXItemsTR4, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXItemsTR4::CTRXItemsTR4() : CTRXPropertyPage(CTRXItemsTR4::IDD)
{
    SetGUIModified ( FALSE );
    m_MenuItemIndex     = -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXItemsTR4::~CTRXItemsTR4()
{
    CTRSaveGame *pGame = CTRSaveGame::I(FALSE);
    if ( pGame != NULL )
    {
        delete pGame;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnHelp()
{
    // TODO
    CTRXHelpDialog dlg;
    dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATUS, m_Status);
    //
    DDX_Control(pDX, IDC_ZERO, m_Zero);
    DDX_Control(pDX, IDC_ONE, m_One);
    DDX_Control(pDX, IDC_FOUR, m_Four);
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXItemsTR4, CTRXPropertyPage)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_ITEM_01, &CTRXItemsTR4::OnClickedItem01)
    ON_BN_CLICKED(IDC_ITEM_02, &CTRXItemsTR4::OnClickedItem02)
    ON_BN_CLICKED(IDC_ITEM_03, &CTRXItemsTR4::OnClickedItem03)
    ON_BN_CLICKED(IDC_ITEM_04, &CTRXItemsTR4::OnClickedItem04)
    ON_BN_CLICKED(IDC_ITEM_05, &CTRXItemsTR4::OnClickedItem05)
    ON_BN_CLICKED(IDC_ITEM_06, &CTRXItemsTR4::OnClickedItem06)
    ON_BN_CLICKED(IDC_ITEM_07, &CTRXItemsTR4::OnClickedItem07)
    ON_BN_CLICKED(IDC_ITEM_08, &CTRXItemsTR4::OnClickedItem08)
    ON_BN_CLICKED(IDC_ITEM_09, &CTRXItemsTR4::OnClickedItem09)
    ON_BN_CLICKED(IDC_ITEM_10, &CTRXItemsTR4::OnClickedItem10)
    ON_BN_CLICKED(IDC_ITEM_11, &CTRXItemsTR4::OnClickedItem11)
    ON_BN_CLICKED(IDC_ITEM_12, &CTRXItemsTR4::OnClickedItem12)
    ON_BN_CLICKED(IDC_ITEM_13, &CTRXItemsTR4::OnClickedItem13)
    ON_BN_CLICKED(IDC_ITEM_14, &CTRXItemsTR4::OnClickedItem14)
    ON_BN_CLICKED(IDC_ITEM_15, &CTRXItemsTR4::OnClickedItem15)
    ON_BN_CLICKED(IDC_ITEM_16, &CTRXItemsTR4::OnClickedItem16)
    ON_BN_CLICKED(IDC_ITEM_17, &CTRXItemsTR4::OnClickedItem17)
    ON_BN_CLICKED(IDC_ITEM_18, &CTRXItemsTR4::OnClickedItem18)
    ON_BN_CLICKED(IDC_ITEM_19, &CTRXItemsTR4::OnClickedItem19)
    ON_BN_CLICKED(IDC_ITEM_20, &CTRXItemsTR4::OnClickedItem20)
    ON_BN_CLICKED(IDC_ITEM_21, &CTRXItemsTR4::OnClickedItem21)
    ON_BN_CLICKED(IDC_ITEM_22, &CTRXItemsTR4::OnClickedItem22)
    ON_BN_CLICKED(IDC_ITEM_23, &CTRXItemsTR4::OnClickedItem23)
    ON_BN_CLICKED(IDC_ITEM_24, &CTRXItemsTR4::OnClickedItem24)
    ON_BN_CLICKED(IDC_ITEM_25, &CTRXItemsTR4::OnClickedItem25)
    ON_BN_CLICKED(IDC_ITEM_26, &CTRXItemsTR4::OnClickedItem26)
    ON_BN_CLICKED(IDC_ITEM_27, &CTRXItemsTR4::OnClickedItem27)
    ON_BN_CLICKED(IDC_ITEM_28, &CTRXItemsTR4::OnClickedItem28)
    ON_BN_CLICKED(IDC_ITEM_29, &CTRXItemsTR4::OnClickedItem29)

    ON_BN_CLICKED(IDC_ZERO, &CTRXItemsTR4::OnBnClickedZero)
    ON_BN_CLICKED(IDC_ONE, &CTRXItemsTR4::OnBnClickedOne)
    ON_BN_CLICKED(IDC_FOUR, &CTRXItemsTR4::OnBnClickedFour)

    ON_EN_CHANGE(IDC_NITEM_01, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_02, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_03, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_04, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_05, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_06, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_07, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_06, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_09, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_10, &CTRXItemsTR4::OnChangeEdit)

    ON_EN_CHANGE(IDC_NITEM_11, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_12, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_13, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_14, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_15, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_16, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_17, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_18, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_19, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_20, &CTRXItemsTR4::OnChangeEdit)

    ON_EN_CHANGE(IDC_NITEM_21, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_22, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_23, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_24, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_25, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_26, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_27, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_28, &CTRXItemsTR4::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_29, &CTRXItemsTR4::OnChangeEdit)

    // ON_NOTIFY_EX( TTN_NEEDTEXT, IDC_ITEM_01, OnToolTipNotify )
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

    ON_NOTIFY(NM_RCLICK, IDC_ITEM_01, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_02, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_03, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_04, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_05, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_06, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_07, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_08, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_09, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_10, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_11, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_12, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_13, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_14, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_15, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_16, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_17, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_18, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_19, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_20, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_21, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_22, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_23, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_24, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_25, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_26, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_27, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_28, OnRClicked)
    ON_NOTIFY(NM_RCLICK, IDC_ITEM_29, OnRClicked)

    ON_COMMAND(ID_ITEMS_ITEM1, &CTRXItemsTR4::OnItemsItem1)
    ON_COMMAND(ID_ITEMS_ITEM2, &CTRXItemsTR4::OnItemsItem2)
    ON_COMMAND(ID_ITEMS_ITEM3, &CTRXItemsTR4::OnItemsItem3)
    ON_COMMAND(ID_ITEMS_ITEM4, &CTRXItemsTR4::OnItemsItem4)
    ON_COMMAND(ID_ITEMS_ITEM5, &CTRXItemsTR4::OnItemsItem5)
    ON_COMMAND(ID_ITEMS_ITEM6, &CTRXItemsTR4::OnItemsItem6)
    ON_COMMAND(ID_ITEMS_ITEM7, &CTRXItemsTR4::OnItemsItem7)
    ON_COMMAND(ID_ITEMS_ITEM8, &CTRXItemsTR4::OnItemsItem8)

    ON_WM_CTLCOLOR()

    END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXItemsTR4
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::SetLabel ( UINT checkId, UINT editId, char **pTableInd, char **pTableGen, int index )
{
    static char szLabel [ MAX_PATH * 4 ];

    //
    CWnd *pCheckWnd = GetDlgItem ( checkId );
    CWnd *pEditWnd  = GetDlgItem ( editId );

    //
    if ( pCheckWnd != NULL )
    {
        if ( pTableInd != NULL && pTableInd [ index ] != NULL && strlen(pTableInd [ index ]) > 0 )
        {
            ZeroMemory ( szLabel, sizeof(szLabel) );
            strcpy_s ( szLabel, pTableInd [ index ] );
            char *pCR = strchr ( szLabel, '\r' );
            if ( pCR != NULL )
            {
                *pCR = '\0';
            }
            pCheckWnd->SetFont ( m_pBoldFont, TRUE );
            pEditWnd->SetFont ( m_pBoldFont, TRUE );
            pCheckWnd->SetWindowText ( szLabel );
        }
        else if ( pTableGen != NULL && pTableGen [ index ] != NULL && strlen(pTableGen [ index ]) > 0 )
        {
            ZeroMemory ( szLabel, sizeof(szLabel) );
            strcpy_s ( szLabel, pTableGen [ index ] );
            char *pCR = strchr ( szLabel, '\r' );
            if ( pCR != NULL )
            {
                *pCR = '\0';
            }
            pCheckWnd->SetFont ( m_pNormalFont, TRUE );
            pEditWnd->SetFont ( m_pNormalFont, TRUE );
            pCheckWnd->SetWindowText ( szLabel );
        }
        else
        {
            char szText [ 32 ];
            sprintf_s ( szText, sizeof(szText), "Item #%02d", index + 1 );
            pCheckWnd->SetFont ( m_pItalicFont, TRUE );
            pEditWnd->SetFont ( m_pItalicFont, TRUE );
            pCheckWnd->SetWindowText ( szText );
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::OnSetActive()
{
    // TODO

    //
    m_Status.SetWindowText ( "Tombraider 4 and 5" );

    //
    BOOL bModified = IsGUIModified();
    if ( CTRSaveGame::IsValid( ) )
    {
        bModified = CTRSaveGame::IsBufferModified();
    }

    //
    char **pTableGen = NULL;
    char **pTableInd = NULL;
    int iVersion = CTRSaveGame::GetVersion();

    //
    bool bStandardLevel = true;
    if ( TR49ItemsNameGen [ 0 ] != NULL )
    {
        bStandardLevel = false;
    }

    //
    int iLevelIndex = CTRSaveGame::GetLevelIndex();

    if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 45 ) )
    {
        //
        if ( bStandardLevel )
        {
            pTableGen = TR4ItemsName;
            switch ( iLevelIndex )
            {
                case 0  : pTableInd = TR4ItemsName01; break;
                case 1  : pTableInd = TR4ItemsName02; break;
                case 2  : pTableInd = TR4ItemsName03; break;
                case 3  : pTableInd = TR4ItemsName04; break;
                case 4  : pTableInd = TR4ItemsName05; break;
                case 5  : pTableInd = TR4ItemsName06; break;
                case 6  : pTableInd = TR4ItemsName07; break;
                case 7  : pTableInd = TR4ItemsName08; break;
                case 8  : pTableInd = TR4ItemsName09; break;
                case 9  : pTableInd = TR4ItemsName10; break;
                case 10 : pTableInd = TR4ItemsName11; break;
                case 11 : pTableInd = TR4ItemsName12; break;
                case 12 : pTableInd = TR4ItemsName13; break;
                case 13 : pTableInd = TR4ItemsName14; break;
                case 14 : pTableInd = TR4ItemsName15; break;
                case 15 : pTableInd = TR4ItemsName16; break;
                case 16 : pTableInd = TR4ItemsName17; break;
                case 17 : pTableInd = TR4ItemsName18; break;
                case 18 : pTableInd = TR4ItemsName19; break;
                case 19 : pTableInd = TR4ItemsName20; break;
                case 20 : pTableInd = TR4ItemsName21; break;
                case 21 : pTableInd = TR4ItemsName22; break;
                case 22 : pTableInd = TR4ItemsName23; break;
                case 23 : pTableInd = TR4ItemsName24; break;
                case 24 : pTableInd = TR4ItemsName25; break;
                case 25 : pTableInd = TR4ItemsName26; break;
                case 26 : pTableInd = TR4ItemsName27; break;
                case 27 : pTableInd = TR4ItemsName28; break;
                case 28 : pTableInd = TR4ItemsName29; break;
                case 29 : pTableInd = TR4ItemsName30; break;
                case 30 : pTableInd = TR4ItemsName31; break;
                case 31 : pTableInd = TR4ItemsName32; break;
                case 32 : pTableInd = TR4ItemsName33; break;
                case 33 : pTableInd = TR4ItemsName34; break;
                case 34 : pTableInd = TR4ItemsName35; break;
                case 35 : pTableInd = TR4ItemsName36; break;
                case 36 : pTableInd = TR4ItemsName37; break;
                case 37 : pTableInd = TR4ItemsName38; break;
            }
        }
        else
        {
            pTableGen = TR49ItemsNameGen;

            //  Skip Title
            pTableInd = TR49ItemsNameInd [ iLevelIndex + 1 ];
        }
    }
    else if ( iVersion == 49 )
    {
        pTableGen = TR49ItemsNameGen;

        //  Skip Title
        pTableInd = TR49ItemsNameInd [ iLevelIndex + 1 ];
    }
    else if ( iVersion == 5 || iVersion == 50 )
    {
        pTableGen = TR5ItemsName;
        switch ( iLevelIndex )
        {
            case 0  : pTableInd = TR5ItemsName01; break;
            case 1  : pTableInd = TR5ItemsName02; break;
            case 2  : pTableInd = TR5ItemsName03; break;
            case 3  : pTableInd = TR5ItemsName04; break;
            case 4  : pTableInd = TR5ItemsName05; break;
            case 5  : pTableInd = TR5ItemsName06; break;
            case 6  : pTableInd = TR5ItemsName07; break;
            case 7  : pTableInd = TR5ItemsName08; break;
            case 8  : pTableInd = TR5ItemsName09; break;
            case 9  : pTableInd = TR5ItemsName10; break;
            case 10 : pTableInd = TR5ItemsName11; break;
            case 11 : pTableInd = TR5ItemsName12; break;
            case 12 : // No 12 Level
            case 13 : pTableInd = TR5ItemsName14; break;
        }
    }

    //
#define SET_LABEL(id,v) SetLabel ( IDC_ITEM_ ##id, IDC_NITEM_##id, pTableInd, pTableGen, ##v )
    if ( pTableInd != NULL || pTableGen != NULL )
    {
        SET_LABEL(01,0);
        SET_LABEL(02,1);
        SET_LABEL(03,2);
        SET_LABEL(04,3);
        SET_LABEL(05,4);
        SET_LABEL(06,5);
        SET_LABEL(07,6);
        SET_LABEL(08,7);
        SET_LABEL(09,8);
        SET_LABEL(10,9);
        SET_LABEL(11,10);
        SET_LABEL(12,11);
        SET_LABEL(13,12);
        SET_LABEL(14,13);
        SET_LABEL(15,14);
        SET_LABEL(16,15);
        SET_LABEL(17,16);
        SET_LABEL(18,17);
        SET_LABEL(19,18);
        SET_LABEL(20,19);
        SET_LABEL(21,20);
        SET_LABEL(22,21);
        SET_LABEL(23,22);
        SET_LABEL(24,23);
        SET_LABEL(25,24);
        SET_LABEL(26,25);
        SET_LABEL(27,26);
        SET_LABEL(28,27);
        if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
        {
            SET_LABEL(29,28);
        }
    }

    //
    DisplayValues();

    //
    SetGUIModified ( bModified, "ItemsTR4 SetActive" );

    return CTRXPropertyPage::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::GetMenuLabel ( int button )
{
    //
    static char szLabels [ 4096 ];
    ZeroMemory ( szLabels, sizeof(szLabels) );

    //
    ZeroMemory ( menuLabel, sizeof(menuLabel) );

    //
    char **pTableGen = NULL;
    char **pTableInd = NULL;
    int iVersion = CTRSaveGame::GetVersion();

    //
    bool bStandardLevel = true;
    if ( TR49ItemsNameGen [ 0 ] != NULL )
    {
        bStandardLevel = false;
    }

    //
    int iLevelIndex = CTRSaveGame::GetLevelIndex();

    if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 45 ) )
    {
        //
        if ( bStandardLevel )
        {
            pTableGen = TR4ItemsName;
            switch ( iLevelIndex )
            {
                case 0  : pTableInd = TR4ItemsName01; break;
                case 1  : pTableInd = TR4ItemsName02; break;
                case 2  : pTableInd = TR4ItemsName03; break;
                case 3  : pTableInd = TR4ItemsName04; break;
                case 4  : pTableInd = TR4ItemsName05; break;
                case 5  : pTableInd = TR4ItemsName06; break;
                case 6  : pTableInd = TR4ItemsName07; break;
                case 7  : pTableInd = TR4ItemsName08; break;
                case 8  : pTableInd = TR4ItemsName09; break;
                case 9  : pTableInd = TR4ItemsName10; break;
                case 10 : pTableInd = TR4ItemsName11; break;
                case 11 : pTableInd = TR4ItemsName12; break;
                case 12 : pTableInd = TR4ItemsName13; break;
                case 13 : pTableInd = TR4ItemsName14; break;
                case 14 : pTableInd = TR4ItemsName15; break;
                case 15 : pTableInd = TR4ItemsName16; break;
                case 16 : pTableInd = TR4ItemsName17; break;
                case 17 : pTableInd = TR4ItemsName18; break;
                case 18 : pTableInd = TR4ItemsName19; break;
                case 19 : pTableInd = TR4ItemsName20; break;
                case 20 : pTableInd = TR4ItemsName21; break;
                case 21 : pTableInd = TR4ItemsName22; break;
                case 22 : pTableInd = TR4ItemsName23; break;
                case 23 : pTableInd = TR4ItemsName24; break;
                case 24 : pTableInd = TR4ItemsName25; break;
                case 25 : pTableInd = TR4ItemsName26; break;
                case 26 : pTableInd = TR4ItemsName27; break;
                case 27 : pTableInd = TR4ItemsName28; break;
                case 28 : pTableInd = TR4ItemsName29; break;
                case 29 : pTableInd = TR4ItemsName30; break;
                case 30 : pTableInd = TR4ItemsName31; break;
                case 31 : pTableInd = TR4ItemsName32; break;
                case 32 : pTableInd = TR4ItemsName33; break;
                case 33 : pTableInd = TR4ItemsName34; break;
                case 34 : pTableInd = TR4ItemsName35; break;
                case 35 : pTableInd = TR4ItemsName36; break;
                case 36 : pTableInd = TR4ItemsName37; break;
                case 37 : pTableInd = TR4ItemsName38; break;
            }
        }
        else
        {
            pTableGen = TR49ItemsNameGen;

            //  Skip Title
            pTableInd = TR49ItemsNameInd [ iLevelIndex + 1 ];
        }
    }
    else if ( iVersion == 49 )
    {
        pTableGen = TR49ItemsNameGen;

        //  Skip Title
        pTableInd = TR49ItemsNameInd [ iLevelIndex + 1 ];
    }
    else if ( iVersion == 5 || iVersion == 50 )
    {
        pTableGen = TR5ItemsName;
        switch ( iLevelIndex )
        {
            case 0  : pTableInd = TR5ItemsName01; break;
            case 1  : pTableInd = TR5ItemsName02; break;
            case 2  : pTableInd = TR5ItemsName03; break;
            case 3  : pTableInd = TR5ItemsName04; break;
            case 4  : pTableInd = TR5ItemsName05; break;
            case 5  : pTableInd = TR5ItemsName06; break;
            case 6  : pTableInd = TR5ItemsName07; break;
            case 7  : pTableInd = TR5ItemsName08; break;
            case 8  : pTableInd = TR5ItemsName09; break;
            case 9  : pTableInd = TR5ItemsName10; break;
            case 10 : pTableInd = TR5ItemsName11; break;
            case 11 : pTableInd = TR5ItemsName12; break;
            case 12 : // No 12 Level
            case 13 : pTableInd = TR5ItemsName14; break;
        }
    }

    //
    if ( pTableInd != NULL && pTableInd [ button ] != NULL )
    {
        strcat_s ( szLabels, sizeof(szLabels), pTableInd [ button ] );
    }

    //
    if ( pTableGen != NULL && pTableGen [ button ] != NULL )
    {
        strcat_s ( szLabels, sizeof(szLabels), pTableGen [ button ] );
    }

    char *nextToken = NULL;
    char *token = strtok_s( szLabels, "\r\n", &nextToken);
    while ( token != NULL) 
    {
        char *parenthesis = NULL;
        for ( int l = (int) strlen(token) - 1; l >= 0; l-- )
        {
            if ( token [ l ] == '(' )
            {
                parenthesis = token + l;
                break;
            }
        }
        
        //
        if ( parenthesis != NULL )
        {
            int val  = atoi ( parenthesis + 1 );
            int bit = 0;
            *parenthesis = L'\0';

            //
            while ( val > 0 )
            {
                bit++;
                val = val / 2;
            }

            if ( bit >= 1 && bit <= MAX_MENU_LABEL )
            {
                if ( strstr ( menuLabel [ bit - 1 ], token ) == NULL )
                {
                    if ( strlen(menuLabel [ bit - 1 ]) == 0 )
                    {
                        strcpy_s ( menuLabel [ bit - 1 ], sizeof(menuLabel [ bit - 1 ]), token );
                    }
                    else
                    {
                        strcat_s ( menuLabel [ bit - 1 ], sizeof(menuLabel [ bit - 1 ]), "/ " );
                        strcat_s ( menuLabel [ bit - 1 ], sizeof(menuLabel [ bit - 1 ]), token );
                    }
                }
            }
        }

        token = strtok_s( NULL, "\r\n", &nextToken);
    }

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXItemsTR4::EnableForVersion()
{
    BOOL bModified = IsGUIModified();

    int iVersion = CTRXPropertyPage::EnableForVersion();

    /*
     *      Reset buttons.
     */
    #define DIS_ITEM(n)                                         \
        GetDlgItem ( IDC_ITEM_##n)->EnableWindow ( FALSE );     \
        GetDlgItem ( IDC_NITEM_##n)->EnableWindow ( FALSE );        

    #define ENA_ITEM(n)                                         \
        GetDlgItem ( IDC_ITEM_##n)->EnableWindow ( TRUE );      \
        GetDlgItem ( IDC_NITEM_##n)->EnableWindow ( TRUE );     


    if ( iVersion < 40 )
    {
        DIS_ITEM ( 01 );
        DIS_ITEM ( 02 );
        DIS_ITEM ( 03 );
        DIS_ITEM ( 04 );
        DIS_ITEM ( 05 );
        DIS_ITEM ( 06 );
        DIS_ITEM ( 07 );
        DIS_ITEM ( 08 );
        DIS_ITEM ( 09 );
        DIS_ITEM ( 10 );
        DIS_ITEM ( 11 );
        DIS_ITEM ( 12 );
        DIS_ITEM ( 13 );
        DIS_ITEM ( 14 );
        DIS_ITEM ( 15 );
        DIS_ITEM ( 16 );
        DIS_ITEM ( 17 );
        DIS_ITEM ( 18 );
        DIS_ITEM ( 19 );
        DIS_ITEM ( 20 );
        DIS_ITEM ( 21 );
        DIS_ITEM ( 22 );
        DIS_ITEM ( 23 );
        DIS_ITEM ( 24 );
        DIS_ITEM ( 25 );
        DIS_ITEM ( 26 );
        DIS_ITEM ( 27 );
        DIS_ITEM ( 28 );
        DIS_ITEM ( 29 );
        m_Zero.EnableWindow ( FALSE );
        m_One.EnableWindow ( FALSE );
        m_Four.EnableWindow ( FALSE );
}
    else
    {
        ENA_ITEM ( 01 );
        ENA_ITEM ( 02 );
        ENA_ITEM ( 03 );
        ENA_ITEM ( 04 );
        ENA_ITEM ( 05 );
        ENA_ITEM ( 06 );
        ENA_ITEM ( 07 );
        ENA_ITEM ( 08 );
        ENA_ITEM ( 09 );
        ENA_ITEM ( 10 );
        ENA_ITEM ( 11 );
        ENA_ITEM ( 12 );
        ENA_ITEM ( 13 );
        ENA_ITEM ( 14 );
        ENA_ITEM ( 15 );
        ENA_ITEM ( 16 );
        ENA_ITEM ( 17 );
        ENA_ITEM ( 18 );
        ENA_ITEM ( 19 );
        ENA_ITEM ( 20 );
        ENA_ITEM ( 21 );
        ENA_ITEM ( 22 );
        ENA_ITEM ( 23 );
        ENA_ITEM ( 24 );
        ENA_ITEM ( 25 );
        ENA_ITEM ( 26 );
        ENA_ITEM ( 27 );
        ENA_ITEM ( 28 );
        if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
        {
            ENA_ITEM ( 29 );
        }
        else
        {
            DIS_ITEM ( 29 );
        }
        m_Zero.EnableWindow ( TRUE );
        m_One.EnableWindow ( TRUE );
        m_Four.EnableWindow ( TRUE );
    }

    //
    SetGUIModified ( bModified, "ItemsTR4 EnableForVersion" );

    return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::OnKillActive()
{
    // TODO
    UpdateBuffer();

    BOOL bModified = IsGUIModified();

    return CTRXPropertyPage::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::DisplayValues()
{
    BOOL bModified = IsGUIModified();

    int iVersion = EnableForVersion ();

#define SET_DLG_ITEM(n,v)                                                                           \
    if ( CTRSaveGame::I()->GetItems ( ##v - 1) ) ((CButton *)GetDlgItem( IDC_ITEM_##n ))->SetCheck (1); \
    if ( ! CTRSaveGame::I()->GetItems ( ##v - 1) ) ((CButton *)GetDlgItem( IDC_ITEM_##n ))->SetCheck (0);   \
    SetDlgItemInt ( IDC_NITEM_##n, CTRSaveGame::I()->GetItems ( ##v - 1), FALSE );

    if ( CTRSaveGame::IsValid() )
    {
        SET_DLG_ITEM(01,1);
        SET_DLG_ITEM(02,2);
        SET_DLG_ITEM(03,3);
        SET_DLG_ITEM(04,4);
        SET_DLG_ITEM(05,5);
        SET_DLG_ITEM(06,6);
        SET_DLG_ITEM(07,7);
        SET_DLG_ITEM(08,8);
        SET_DLG_ITEM(09,9);
        SET_DLG_ITEM(10,10);
        SET_DLG_ITEM(11,11);
        SET_DLG_ITEM(12,12);
        SET_DLG_ITEM(13,13);
        SET_DLG_ITEM(14,14);
        SET_DLG_ITEM(15,15);
        SET_DLG_ITEM(16,16);
        SET_DLG_ITEM(17,17);
        SET_DLG_ITEM(18,18);
        SET_DLG_ITEM(19,19);
        SET_DLG_ITEM(20,20);
        SET_DLG_ITEM(21,21);
        SET_DLG_ITEM(22,22);
        SET_DLG_ITEM(23,23);
        SET_DLG_ITEM(24,24);
        SET_DLG_ITEM(25,25);
        SET_DLG_ITEM(26,26);
        SET_DLG_ITEM(27,27);
        SET_DLG_ITEM(28,28);
        if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
        {
            SET_DLG_ITEM(29,29);
        }
    }

    SetGUIModified ( bModified, "ItemsTR4 DisplayValues" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::UpdateBuffer()
{
    //
#define GET_DLG_ITEM(n,v)   CTRSaveGame::I()->SetItems ( ##v - 1,  GetDlgItemInt ( IDC_NITEM_##n ) );

    int iVersion = EnableForVersion ();

    if ( CTRSaveGame::IsValid() )
    {
        GET_DLG_ITEM(01,1);
        GET_DLG_ITEM(02,2);
        GET_DLG_ITEM(03,3);
        GET_DLG_ITEM(04,4);
        GET_DLG_ITEM(05,5);
        GET_DLG_ITEM(06,6);
        GET_DLG_ITEM(07,7);
        GET_DLG_ITEM(08,8);
        GET_DLG_ITEM(09,9);
        GET_DLG_ITEM(10,10);
        GET_DLG_ITEM(11,11);
        GET_DLG_ITEM(12,12);
        GET_DLG_ITEM(13,13);
        GET_DLG_ITEM(14,14);
        GET_DLG_ITEM(15,15);
        GET_DLG_ITEM(16,16);
        GET_DLG_ITEM(17,17);
        GET_DLG_ITEM(18,18);
        GET_DLG_ITEM(19,19);
        GET_DLG_ITEM(20,20);
        GET_DLG_ITEM(21,21);
        GET_DLG_ITEM(22,22);
        GET_DLG_ITEM(23,23);
        GET_DLG_ITEM(24,24);
        GET_DLG_ITEM(25,25);
        GET_DLG_ITEM(26,26);
        GET_DLG_ITEM(27,27);
        GET_DLG_ITEM(28,28);
        if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
        {
            GET_DLG_ITEM(29,29);
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

#define CHECK_UNCHECK(n)                            \
void CTRXItemsTR4::OnClickedItem##n()               \
{                                                   \
    if ( IsDlgButtonChecked ( IDC_ITEM_##n ) )      \
    {                                               \
        SetDlgItemInt ( IDC_NITEM_##n, 1 );         \
    }                                               \
    else                                            \
    {                                               \
        SetDlgItemInt ( IDC_NITEM_##n, 0 );         \
    }                                               \
}

CHECK_UNCHECK(01)
CHECK_UNCHECK(02)
CHECK_UNCHECK(03)
CHECK_UNCHECK(04)
CHECK_UNCHECK(05)
CHECK_UNCHECK(06)
CHECK_UNCHECK(07)
CHECK_UNCHECK(08)
CHECK_UNCHECK(09)
CHECK_UNCHECK(10)
CHECK_UNCHECK(11)
CHECK_UNCHECK(12)
CHECK_UNCHECK(13)
CHECK_UNCHECK(14)
CHECK_UNCHECK(15)
CHECK_UNCHECK(16)
CHECK_UNCHECK(17)
CHECK_UNCHECK(18)
CHECK_UNCHECK(19)
CHECK_UNCHECK(20)
CHECK_UNCHECK(21)
CHECK_UNCHECK(22)
CHECK_UNCHECK(23)
CHECK_UNCHECK(24)
CHECK_UNCHECK(25)
CHECK_UNCHECK(26)
CHECK_UNCHECK(27)
CHECK_UNCHECK(28)
CHECK_UNCHECK(29)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
#define SET_DLG_ITEM_VAL(n,v) SetDlgItemInt ( IDC_NITEM_##n, v, FALSE );
void CTRXItemsTR4::OnBnClickedZero()
{
#define SET_DLG_ITEM_ZERO(n)    SET_DLG_ITEM_VAL(n,0)

    int iVersion = EnableForVersion ();

    SET_DLG_ITEM_ZERO(01);
    SET_DLG_ITEM_ZERO(02);
    SET_DLG_ITEM_ZERO(03);
    SET_DLG_ITEM_ZERO(04);
    SET_DLG_ITEM_ZERO(05);
    SET_DLG_ITEM_ZERO(06);
    SET_DLG_ITEM_ZERO(07);
    SET_DLG_ITEM_ZERO(08);
    SET_DLG_ITEM_ZERO(09);
    SET_DLG_ITEM_ZERO(10);
    SET_DLG_ITEM_ZERO(11);
    SET_DLG_ITEM_ZERO(12);
    SET_DLG_ITEM_ZERO(13);
    SET_DLG_ITEM_ZERO(14);
    SET_DLG_ITEM_ZERO(15);
    SET_DLG_ITEM_ZERO(16);
    SET_DLG_ITEM_ZERO(17);
    SET_DLG_ITEM_ZERO(18);
    SET_DLG_ITEM_ZERO(19);
    SET_DLG_ITEM_ZERO(20);
    SET_DLG_ITEM_ZERO(21);
    SET_DLG_ITEM_ZERO(22);
    SET_DLG_ITEM_ZERO(23);
    SET_DLG_ITEM_ZERO(24);
    SET_DLG_ITEM_ZERO(25);
    SET_DLG_ITEM_ZERO(26);
    SET_DLG_ITEM_ZERO(27);
    SET_DLG_ITEM_ZERO(28);
    if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
    {
        SET_DLG_ITEM_ZERO(29);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnBnClickedOne()
{
#define SET_DLG_ITEM_ONE(n) SET_DLG_ITEM_VAL(n,1)

    int iVersion = EnableForVersion ();

    SET_DLG_ITEM_ONE(01);
    SET_DLG_ITEM_ONE(02);
    SET_DLG_ITEM_ONE(03);
    SET_DLG_ITEM_ONE(04);
    SET_DLG_ITEM_ONE(05);
    SET_DLG_ITEM_ONE(06);
    SET_DLG_ITEM_ONE(07);
    SET_DLG_ITEM_ONE(08);
    SET_DLG_ITEM_ONE(09);
    SET_DLG_ITEM_ONE(10);
    SET_DLG_ITEM_ONE(11);
    SET_DLG_ITEM_ONE(12);
    SET_DLG_ITEM_ONE(13);
    SET_DLG_ITEM_ONE(14);
    SET_DLG_ITEM_ONE(15);
    SET_DLG_ITEM_ONE(16);
    SET_DLG_ITEM_ONE(17);
    SET_DLG_ITEM_ONE(18);
    SET_DLG_ITEM_ONE(19);
    SET_DLG_ITEM_ONE(20);
    SET_DLG_ITEM_ONE(21);
    SET_DLG_ITEM_ONE(22);
    SET_DLG_ITEM_ONE(23);
    SET_DLG_ITEM_ONE(24);
    SET_DLG_ITEM_ONE(25);
    SET_DLG_ITEM_ONE(26);
    SET_DLG_ITEM_ONE(27);
    SET_DLG_ITEM_ONE(28);
    if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
    {
        SET_DLG_ITEM_ONE(29);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnBnClickedFour()
{
    int count = 1;
#ifdef _DEBUG
#define SET_DLG_ITEM_FOUR(n)    SET_DLG_ITEM_VAL(n,++count)
#else
#define SET_DLG_ITEM_FOUR(n)    SET_DLG_ITEM_VAL(n,4)
#endif

    int iVersion = EnableForVersion ();

    SET_DLG_ITEM_FOUR(01);
    SET_DLG_ITEM_FOUR(02);
    SET_DLG_ITEM_FOUR(03);
    SET_DLG_ITEM_FOUR(04);
    SET_DLG_ITEM_FOUR(05);
    SET_DLG_ITEM_FOUR(06);
    SET_DLG_ITEM_FOUR(07);
    SET_DLG_ITEM_FOUR(08);
    SET_DLG_ITEM_FOUR(09);
    SET_DLG_ITEM_FOUR(10);
    SET_DLG_ITEM_FOUR(11);
    SET_DLG_ITEM_FOUR(12);
    SET_DLG_ITEM_FOUR(13);
    SET_DLG_ITEM_FOUR(14);
    SET_DLG_ITEM_FOUR(15);
    SET_DLG_ITEM_FOUR(16);
    SET_DLG_ITEM_FOUR(17);
    SET_DLG_ITEM_FOUR(18);
    SET_DLG_ITEM_FOUR(19);
    SET_DLG_ITEM_FOUR(20);
    SET_DLG_ITEM_FOUR(21);
    SET_DLG_ITEM_FOUR(22);
    SET_DLG_ITEM_FOUR(23);
    SET_DLG_ITEM_FOUR(24);
    SET_DLG_ITEM_FOUR(25);
    SET_DLG_ITEM_FOUR(26);
    SET_DLG_ITEM_FOUR(27);
    SET_DLG_ITEM_FOUR(28);

    if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 49 ) )
    {
        SET_DLG_ITEM_FOUR(29);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::OnInitDialog()
{
    BOOL bModified = IsGUIModified ();

    CTRXPropertyPage::OnInitDialog();

    // TODO
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_Status, ("Status"));
#if 0
        m_ToolTip.AddTool ( this, LPSTR_TEXTCALLBACK );
#else
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_01), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_02), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_03), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_04), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_05), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_06), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_07), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_08), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_09), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_10), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_11), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_12), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_13), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_14), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_15), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_16), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_17), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_18), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_19), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_20), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_21), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_22), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_23), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_24), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_25), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_26), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_27), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_28), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_29), LPSTR_TEXTCALLBACK );
#endif
        m_ToolTip.Activate(TRUE);
    }

    //
    //  SubClass
    for ( int iX = 0; iX < sizeof(IDArray)/sizeof(UINT); iX++ )
    {
        UINT id             = IDArray[iX];
        buttomItems [ iX ].SubclassDlgItem ( id, this );
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnOK()
{
    // TODO
    INT_PTR response = AskToSave("Items TR4");
    DisplayValues ( );

    CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnCancel()
{
    // TODO
    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        INT_PTR response = AskToSave("Items TR4");
    }

    CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::OnApply()
{
    // TODO
    WriteWhenOnApply("Items TR4");

    return CTRXPropertyPage::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnChangeEdit()
{
    SetGUIModified ( TRUE, "ItemsTR4 ChangeEdit" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItemsTR4::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
    //
    if ( ! m_bToolTip )
    {
        return FALSE;
    }

    //
    POINT           ptAction;
    GetCursorPos ( &ptAction );
    ScreenToClient ( &ptAction );

    //
    static char szText [ MAX_PATH * 4 ] = "";

    TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMH;

    //
    for ( int iX = 0; iX < sizeof(IDArray)/sizeof(UINT); iX++ )
    {
        UINT nID        = IDArray[iX];
        CWnd    *pWnd   = GetDlgItem(nID);

        RECT rect;
        pWnd->GetWindowRect ( &rect );
        ScreenToClient ( &rect );
        if ( ptAction.x >= rect.left && ptAction.x <= rect.right && ptAction.y >= rect.top && ptAction.y <= rect.bottom )
        {
            //
            bool bStandardLevel = true;
            if ( TR49ItemsNameGen [ 0 ] != NULL )
            {
                bStandardLevel = false;
            }

            //
            char **pTable       = NULL;
            int iVersion = CTRSaveGame::GetVersion();
            if ( iVersion == 4 || ( iVersion >= 40 && iVersion <= 45 ) )
            {
                if ( bStandardLevel )
                {
                    pTable = TR4ItemsName;
                }
                else
                {
                    pTable = TR49ItemsNameGen;
                }
            }
            else if ( iVersion == 49 )
            {
                pTable = TR49ItemsNameGen;
            }
            else if ( iVersion == 5 || iVersion == 50 )
            {
                pTable = TR5ItemsName;
#if 0
                int iLevelIndex = CTRSaveGame::GetLevelIndex();
                switch ( iLevelIndex )
                {
                    case 0  : pTable = TR5ItemsName01; break;
                    case 1  : pTable = TR5ItemsName02; break;
                    case 2  : pTable = TR5ItemsName03; break;
                    case 3  : pTable = TR5ItemsName04; break;
                    case 4  : pTable = TR5ItemsName05; break;
                    case 5  : pTable = TR5ItemsName06; break;
                    case 6  : pTable = TR5ItemsName07; break;
                    case 7  : pTable = TR5ItemsName08; break;
                    case 8  : pTable = TR5ItemsName09; break;
                    case 9  : pTable = TR5ItemsName10; break;
                    case 10 : pTable = TR5ItemsName11; break;
                    case 11 : pTable = TR5ItemsName12; break;
                    case 12 : // No 12 Level
                    case 13 : pTable = TR5ItemsName14; break;
                }
#endif
            }

            //
            if ( pTable != NULL && pTable [ iX ] != NULL )
            {
                if ( strlen(pTable [ iX ]) > 0 )
                {
                    pText->lpszText = pTable [ iX ];
                }
                else
                {
                    sprintf_s ( szText, sizeof(szText), "Item #%02d \r\nNo usage found \r\n", iX + 1 );
                    pText->lpszText = szText;
                }
                return TRUE;
            }
            return FALSE;   
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnRClicked(NMHDR * pNotifyStruct, LRESULT * result)
{
    //
    POINT           point;
    GetCursorPos ( &point );
    ScreenToClient ( &point );

    //
    char    szValue [ 32 ];

    //
    for ( int iX = 0; iX < sizeof(IDArray)/sizeof(UINT); iX++ )
    {
        UINT id             = IDArray[iX];
        CWnd *pCheckWnd     = GetDlgItem(id);

        //
        RECT rect;
        pCheckWnd->GetWindowRect ( &rect );
        ScreenToClient ( &rect );
        if ( point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom )
        {
            UINT nid            = NIDArray[iX];
            CWnd *pEditWnd      = GetDlgItem(nid);

            CPoint menuPoint    = point;
            ClientToScreen ( &menuPoint );

            //
            BOOL bGetMenuLabel = GetMenuLabel ( iX );

            //
            CTRXMenuBase        menu;
            menu.LoadMenu ( IDR_MENU_ITEMS );

            //
            m_pContextMenu = menu.GetSubMenu ( 0 );

            for ( int iM = 0; iM < sizeof(IDMenuItem)/sizeof(UINT); iM++ )
            {
                if ( strlen ( menuLabel [ iM ] ) > 0 )
                {
                    m_pContextMenu->ModifyMenu ( IDMenuItem [ iM ], MF_BYCOMMAND | MF_STRING, IDMenuItem [ iM ], menuLabel [ iM ] );
                }
            }

            //
            pEditWnd->GetWindowText ( szValue, sizeof(szValue) );
            UINT val = atoi(szValue);

            //
            for ( int iM = 0; iM < sizeof(IDMenuItem)/sizeof(UINT); iM++ )
            {
                //
                UINT bit = (UINT) pow ( 2.0, iM );
                //
                if ( val & bit )
                {
                    m_pContextMenu->CheckMenuItem(IDMenuItem [ iM ],MF_CHECKED|MF_BYCOMMAND);
                }
                else
                {
                    m_pContextMenu->CheckMenuItem(IDMenuItem [ iM ],MF_UNCHECKED|MF_BYCOMMAND);
                }
            }

            //
            m_pContextMenu->TrackPopupMenu ( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, menuPoint.x, menuPoint.y, this );

            m_MenuItemIndex    = iX;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItemN(int menuId)
{
    //
    char    szValue [ 32 ];

    //
    if ( m_MenuItemIndex != -1 )
    {
        UINT bit            = ( (UINT)pow ( 2.0, menuId - 1) ) &  0xFF;
        UINT mask           = 0xFF ^ bit;

        UINT nid            = NIDArray[m_MenuItemIndex];
        CWnd *pEditWnd      = GetDlgItem(nid);

        pEditWnd->GetWindowText ( szValue, sizeof(szValue) );
        UINT val = atoi(szValue);

        UINT check          = val & bit;
        if ( check )
        {
            val             = val & mask;
        }
        else
        {
            val             = val | bit;
        }

        sprintf_s ( szValue, sizeof(szValue), "%d", val );
        pEditWnd->SetWindowText ( szValue );
    }

    //
    m_MenuItemIndex    = -1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem1()
{
    OnItemsItemN(1);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem2()
{
    OnItemsItemN(2);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem3()
{
    OnItemsItemN(3);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem4()
{
    OnItemsItemN(4);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem5()
{
    OnItemsItemN(5);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem6()
{
    OnItemsItemN(6);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem7()
{
    OnItemsItemN(7);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItemsTR4::OnItemsItem8()
{
    OnItemsItemN(8);
}
