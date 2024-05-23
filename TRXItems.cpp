// TR3Items.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"
#include "TRXItems.h"
#include "TRXTools.h"

#include "resource.h"

#include "TRXRemastered.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
static const    char *MessageTitle = "Tombraider Standard Editions";

//
static  UINT IDArray [ ] =
{
    IDC_ITEM_01, IDC_ITEM_02, IDC_ITEM_03, IDC_ITEM_04,
    IDC_KEY1, IDC_KEY2, IDC_KEY3, IDC_KEY4,
    IDC_MAP1, IDC_MAP2,
    IDC_TREASURE1, IDC_TREASURE2, IDC_TREASURE3, IDC_TREASURE4,
};


//
/////////////////////////////////////////////////////////////////////////////
// CTRXItems property page
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTRXItems, CTRXPropertyPage)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXItems::CTRXItems() : CTRXPropertyPage(CTRXItems::IDD)
{
    //{{AFX_DATA_INIT(CTRXItems)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    SetGUIModified ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXItems::~CTRXItems()
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
void CTRXItems::DoDataExchange(CDataExchange* pDX)
{
    CTRXPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTRXItems)
    DDX_Control(pDX, IDC_TREASURE1, m_Treasure1);
    DDX_Control(pDX, IDC_TREASURE2, m_Treasure2);
    DDX_Control(pDX, IDC_TREASURE3, m_Treasure3);
    DDX_Control(pDX, IDC_TREASURE4, m_Treasure4);

    DDX_Control(pDX, IDC_MAP1, m_Map1);
    DDX_Control(pDX, IDC_MAP2, m_Map2);

    DDX_Control(pDX, IDC_ITEM_01, m_Item1);
    DDX_Control(pDX, IDC_ITEM_02, m_Item2);
    DDX_Control(pDX, IDC_ITEM_03, m_Item3);
    DDX_Control(pDX, IDC_ITEM_04, m_Item4);

    DDX_Control(pDX, IDC_KEY1, m_Key1);
    DDX_Control(pDX, IDC_KEY2, m_Key2);
    DDX_Control(pDX, IDC_KEY3, m_Key3);
    DDX_Control(pDX, IDC_KEY4, m_Key4);

    DDX_Control(pDX, IDC_NTREASURE1, m_N_Treasure1);
    DDX_Control(pDX, IDC_NTREASURE2, m_N_Treasure2);
    DDX_Control(pDX, IDC_NTREASURE3, m_N_Treasure3);
    DDX_Control(pDX, IDC_NTREASURE4, m_N_Treasure4);

    DDX_Control(pDX, IDC_NMAP1, m_N_Map1);
    DDX_Control(pDX, IDC_NMAP2, m_N_Map2);

    DDX_Control(pDX, IDC_NKEY1, m_N_Key1);
    DDX_Control(pDX, IDC_NKEY2, m_N_Key2);
    DDX_Control(pDX, IDC_NKEY3, m_N_Key3);
    DDX_Control(pDX, IDC_NKEY4, m_N_Key4);

    DDX_Control(pDX, IDC_NKEY1_SPIN, m_N_Key1Spin);
    DDX_Control(pDX, IDC_NKEY2_SPIN, m_N_Key2Spin);
    DDX_Control(pDX, IDC_NKEY3_SPIN, m_N_Key3Spin);
    DDX_Control(pDX, IDC_NKEY4_SPIN, m_N_Key4Spin);

    DDX_Control(pDX, IDC_NITEM_1, m_N_Item1);
    DDX_Control(pDX, IDC_NITEM_2, m_N_Item2);
    DDX_Control(pDX, IDC_NITEM_3, m_N_Item3);
    DDX_Control(pDX, IDC_NITEM_4, m_N_Item4);

    DDX_Control(pDX, IDC_STATUS, m_Status);
    DDX_Control(pDX, IDC_ZERO, m_Zero);
    DDX_Control(pDX, IDC_ONE, m_One);
    DDX_Control(pDX, IDC_FOUR, m_Four);
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXItems, CTRXPropertyPage)
    //{{AFX_MSG_MAP(CTRXItems)
    ON_BN_CLICKED(IDC_KEY1, OnKey1)
    ON_BN_CLICKED(IDC_KEY2, OnKey2)
    ON_BN_CLICKED(IDC_KEY3, OnKey3)
    ON_BN_CLICKED(IDC_KEY4, OnKey4)
    ON_BN_CLICKED(IDC_MAP1, OnMap1)
    ON_BN_CLICKED(IDC_MAP2, OnMap2)
    ON_BN_CLICKED(IDC_ITEM_01, OnItem1)
    ON_BN_CLICKED(IDC_ITEM_02, OnItem2)
    ON_BN_CLICKED(IDC_ITEM_03, OnItem3)
    ON_BN_CLICKED(IDC_ITEM_04, OnItem4)
    ON_BN_CLICKED(IDC_TREASURE1, OnTreasure1)
    ON_BN_CLICKED(IDC_TREASURE2, OnTreasure2)
    ON_BN_CLICKED(IDC_TREASURE3, OnTreasure3)
    ON_BN_CLICKED(IDC_TREASURE4, OnTreasure4)
    ON_BN_CLICKED(ID_HELP, OnHelp)
    ON_BN_CLICKED(IDC_ZERO, &CTRXItems::OnBnClickedZero)
    ON_BN_CLICKED(IDC_ONE, &CTRXItems::OnBnClickedOne)
    ON_BN_CLICKED(IDC_FOUR, &CTRXItems::OnBnClickedFour)

    ON_EN_CHANGE(IDC_NMAP1, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NMAP2, &CTRXItems::OnChangeEdit)

    ON_EN_CHANGE(IDC_NITEM_1, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_2, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_3, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NITEM_4, &CTRXItems::OnChangeEdit)

    ON_EN_CHANGE(IDC_NKEY1, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NKEY2, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NKEY3, &CTRXItems::OnChangeEdit)
    ON_EN_CHANGE(IDC_NKEY4, &CTRXItems::OnChangeEdit)

    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

    ON_WM_CTLCOLOR()
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP

END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXItems message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnKey1()
{
    // TODO: Add your control notification handler code here
    if ( m_Key1.GetCheck ( ) )
    {
        m_N_Key1.SetWindowText ( "1" );
    }
    else
    {
        m_N_Key1.SetWindowText ( "0" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnKey2()
{
    // TODO: Add your control notification handler code here
    if ( m_Key2.GetCheck ( ) )
    {
        m_N_Key2.SetWindowText ( "1" );
    }
    else
    {
        m_N_Key2.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnKey3()
{
    // TODO: Add your control notification handler code here
    if ( m_Key3.GetCheck ( ) )
    {
        m_N_Key3.SetWindowText ( "1" );
    }
    else
    {
        m_N_Key3.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnKey4()
{
    // TODO: Add your control notification handler code here
    if ( m_Key4.GetCheck ( ) )
    {
        m_N_Key4.SetWindowText ( "1" );
    }
    else
    {
        m_N_Key4.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnMap1()
{
    // TODO: Add your control notification handler code here
    if ( m_Map1.GetCheck ( ) )
    {
        m_N_Map1.SetWindowText ( "1" );
    }
    else
    {
        m_N_Map1.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnMap2()
{
    // TODO: Add your control notification handler code here
    if ( m_Map2.GetCheck ( ) )
    {
        m_N_Map2.SetWindowText ( "1" );
    }
    else
    {
        m_N_Map2.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnItem1()
{
    // TODO: Add your control notification handler code here
    if ( m_Item1.GetCheck () )
    {
        m_N_Item1.SetWindowText ( "1" );
    }
    else
    {
        m_N_Item1.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnItem2()
{
    // TODO: Add your control notification handler code here
    if ( m_Item2.GetCheck () )
    {
        m_N_Item2.SetWindowText ( "1" );
    }
    else
    {
        m_N_Item2.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnItem3()
{
    // TODO: Add your control notification handler code here
    if ( m_Item3.GetCheck () )
    {
        m_N_Item3.SetWindowText ( "1" );
    }
    else
    {
        m_N_Item3.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnItem4()
{
    // TODO: Add your control notification handler code here
    if ( m_Item4.GetCheck () )
    {
        m_N_Item4.SetWindowText ( "1" );
    }
    else
    {
        m_N_Item4.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnTreasure1()
{
    // TODO: Add your control notification handler code here
    if ( m_Treasure1.GetCheck ( ) )
    {
        m_N_Treasure1.SetWindowText ( "1" );
    }
    else
    {
        m_N_Treasure1.SetWindowText ( "0" );
    }
    
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnTreasure2()
{
    // TODO: Add your control notification handler code here
    if ( m_Treasure2.GetCheck ( ) )
    {
        m_N_Treasure2.SetWindowText ( "1" );
    }
    else
    {
        m_N_Treasure2.SetWindowText ( "0" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnTreasure3()
{
    // TODO: Add your control notification handler code here
    if ( m_Treasure3.GetCheck ( ) )
    {
        m_N_Treasure3.SetWindowText ( "1" );
    }
    else
    {
        m_N_Treasure3.SetWindowText ( "0" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnTreasure4()
{
    // TODO: Add your control notification handler code here
    if ( m_Treasure4.GetCheck ( ) )
    {
        m_N_Treasure4.SetWindowText ( "1" );
    }
    else
    {
        m_N_Treasure4.SetWindowText ( "0" );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnHelp()
{
    // TODO: Add your control notification handler code here
    CTRXHelpDialog dlg;
    dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::DisplayValues()
{
    BOOL bModified = IsGUIModified();

    int             iX;
    char            szString [ 32 ];

    if ( CTRSaveGame::I() != NULL )
    {   
        // m_Status.SetWindowText ( CTRSaveGame::I()->GetStatus () );
    }

    EnableForVersion ();

    /*
     *      Reset buttons.
     */
    m_Key1.SetCheck ( 0 );
    m_Key2.SetCheck ( 0 );
    m_Key3.SetCheck ( 0 );
    m_Key4.SetCheck ( 0 );

    m_Item1.SetCheck ( 0 );
    m_Item2.SetCheck ( 0 );
    m_Item3.SetCheck ( 0 );
    m_Item4.SetCheck ( 0 );

    m_Map1.SetCheck ( 0 );
    m_Map2.SetCheck ( 0 );

    m_Treasure1.SetCheck ( 0 );
    m_Treasure2.SetCheck ( 0 );
    m_Treasure3.SetCheck ( 0 );
    m_Treasure4.SetCheck ( 0 );

    if ( CTRSaveGame::IsValid() )
    {
        /*
         *      Set level.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        /*
         *      Maps.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetMap1 () );
        m_N_Map1.SetWindowText ( szString );
        m_Map1.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Map1.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetMap2 () );
        m_N_Map2.SetWindowText ( szString );
        m_Map2.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Map2.SetCheck ( 1 );
        }

        /*
         *      Items.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetItem1 () );
        m_N_Item1.SetWindowText ( szString );
        m_Item1.SetCheck ( 0);
        if ( *szString != '0' )
        {
            m_Item1.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetItem2 () );
        m_N_Item2.SetWindowText ( szString );
        m_Item2.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Item2.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetItem3 () );
        m_N_Item3.SetWindowText ( szString );
        m_Item3.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Item3.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetItem4 () );
        m_N_Item4.SetWindowText ( szString );
        m_Item4.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Item4.SetCheck ( 1 );
        }

        /*
         *      Keys.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetKey1 () );
        m_N_Key1.SetWindowText ( szString );
        m_Key1.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Key1.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetKey2 () );
        m_N_Key2.SetWindowText ( szString );
        m_Key2.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Key2.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetKey3 () );
        m_N_Key3.SetWindowText ( szString );
        m_Key3.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Key3.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetKey4 () );
        m_N_Key4.SetWindowText ( szString );
        m_Key4.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Key4.SetCheck ( 1 );
        }

        /*
         *  Treasures.
         */
        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetTreasure1 ( iX ) );
        m_N_Treasure1.SetWindowText ( szString );
        m_Treasure1.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Treasure1.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetTreasure2 ( iX ) );
        m_N_Treasure2.SetWindowText ( szString );
        m_Treasure2.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Treasure2.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetTreasure3 ( iX ) );
        m_N_Treasure3.SetWindowText ( szString );
        m_Treasure3.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Treasure3.SetCheck ( 1 );
        }

        sprintf_s ( szString, sizeof(szString), "%d", CTRSaveGame::I()->GetTreasure4 ( iX ) );
        m_N_Treasure4.SetWindowText ( szString );
        m_Treasure4.SetCheck ( 0 );
        if ( *szString != '0' )
        {
            m_Treasure4.SetCheck ( 1 );
        }
    }

    SetGUIModified ( bModified, "Items DisplayValues" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItems::OnSetActive()
{
    // TODO: Add your specialized code here and/or call the base class
    //
    m_Status.SetWindowText ( "Tombraider 1, Unfinished Busines, 2, Golden Mask, 3, Lost Artifact" );

    //
    BOOL bModified = IsGUIModified ( );
    if ( CTRSaveGame::IsValid( ) )
    {
        bModified = CTRSaveGame::IsBufferModified();
    }

    //  Set Label
    int iVersion        = CTRSaveGame::GetVersion();
    int iFullVersion    = CTRSaveGame::GetFullVersion();
    int iLevelIndex     = CTRSaveGame::GetLevelIndex();

    //
    CWnd *tableCheckWnd [] =
    {
        &m_Item1, &m_Item2, &m_Item3, &m_Item4,
        &m_Key1, &m_Key2, &m_Key3, &m_Key4,
        &m_Map1, &m_Map2,
        &m_Treasure1, &m_Treasure2, &m_Treasure3, &m_Treasure4,
    };

    CWnd *tableEditWnd [] =
    {
        &m_N_Item1, &m_N_Item2, &m_N_Item3, &m_N_Item4,
        &m_N_Key1, &m_N_Key2, &m_N_Key3, &m_N_Key4,
        &m_N_Map1, &m_N_Map2,
        &m_N_Treasure1, &m_N_Treasure2, &m_N_Treasure3, &m_N_Treasure4,
    };

    const char *pLabel = "";
    static char szLabel [ MAX_PATH ];
    for ( int iX = 0; iX < sizeof(tableCheckWnd)/sizeof(CWnd *); iX++ )
    {
        CWnd *pCheckWnd = tableCheckWnd [ iX ];
        CWnd *pEditWnd  = tableEditWnd [ iX ];

        pLabel = CTRXRemastered::GetLabelForObject ( iFullVersion, iLevelIndex, iX );
        if ( strlen(pLabel) > 0 )
        {
            strcpy_s ( szLabel, sizeof(szLabel), pLabel );
            pCheckWnd->SetFont ( m_pBoldFont, TRUE );
            pEditWnd->SetFont ( m_pBoldFont, TRUE );
        }
        else if ( iX < 4 )
        {
            sprintf_s ( szLabel, sizeof(szLabel), "Object %d", iX + 1 );
            pCheckWnd->SetFont ( m_pItalicFont, TRUE );
            pEditWnd->SetFont ( m_pItalicFont, TRUE );
        }
        else if ( iX < 8 )
        {
            sprintf_s ( szLabel, sizeof(szLabel), "Key %d", iX + 1 - 4 );
            pCheckWnd->SetFont ( m_pItalicFont, TRUE );
            pEditWnd->SetFont ( m_pItalicFont, TRUE );
        }
        else if ( iX < 10 )
        {
            sprintf_s ( szLabel, sizeof(szLabel), "Map %d", iX + 1 - 8 );
            pCheckWnd->SetFont ( m_pItalicFont, TRUE );
            pEditWnd->SetFont ( m_pItalicFont, TRUE );
        }
        else
        {
            sprintf_s ( szLabel, sizeof(szLabel), "Item %d", iX + 1 - 10 );
            pCheckWnd->SetFont ( m_pItalicFont, TRUE );
            pEditWnd->SetFont ( m_pItalicFont, TRUE );
        }
        pCheckWnd->SetWindowText ( szLabel );
    }

    //
    DisplayValues ();

    SetGUIModified ( bModified, "Items SetActive" );

    return CTRXPropertyPage::OnSetActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::UpdateBuffer()
{
    int                     iX;

    char                    szString [ 32 ];

    if ( CTRSaveGame::IsValid() )
    {

        /*
         *      Get gun state.
         */
        iX = CTRSaveGame::GetLevelIndex ();

        /*
         *  Treasures.
         */
        m_N_Treasure1.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetTreasure1 ( szString, iX );

        m_N_Treasure2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetTreasure2 ( szString, iX );

        m_N_Treasure3.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetTreasure3 ( szString, iX );

        m_N_Treasure4.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetTreasure4 ( szString, iX );

        /*
         *      Maps.
         */
        m_N_Map1.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetMap1 ( szString );

        m_N_Map2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetMap2 ( szString );

        /*
         *      Items.
         */
        m_N_Item1.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetItem1 ( szString );

        m_N_Item2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetItem2 ( szString );

        m_N_Item3.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetItem3 ( szString );

        m_N_Item4.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetItem4 ( szString );

        /*
         *      Keys.
         */
        m_N_Key1.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetKey1 ( szString );

        m_N_Key2.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetKey2 ( szString );

        m_N_Key3.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetKey3 ( szString );

        m_N_Key4.GetWindowText ( szString, sizeof ( szString ) );
        CTRSaveGame::I()->SetKey4 ( szString );

        /*
         *      Objects mask.
         */
        CTRSaveGame::I()->SetAllObjects ();

    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItems::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateBuffer();

    BOOL bModified = IsGUIModified();

    return CTRXPropertyPage::OnKillActive();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
int CTRXItems::EnableForVersion()
{
    BOOL bModified = IsGUIModified();

    int iVersion = CTRXPropertyPage::EnableForVersion();

    /*
     *      Reset buttons.
     */
    if ( iVersion != 0 )
    {
        m_Treasure1.EnableWindow ( iVersion == 30 );
        m_Treasure2.EnableWindow ( iVersion == 30 );
        m_Treasure3.EnableWindow ( iVersion == 30 );
        m_Treasure4.EnableWindow ( iVersion == 30 );

        m_N_Treasure1.EnableWindow ( iVersion == 30 );
        m_N_Treasure2.EnableWindow ( iVersion == 30 );
        m_N_Treasure3.EnableWindow ( iVersion == 30 );
        m_N_Treasure4.EnableWindow ( iVersion == 30 );

        m_N_Treasure4.EnableWindow ( iVersion == 30 );
        m_N_Treasure3.EnableWindow ( iVersion == 30 );
        m_N_Treasure2.EnableWindow ( iVersion == 30 );
        m_N_Treasure1.EnableWindow ( iVersion == 30 );

        //
        m_Treasure4.EnableWindow ( iVersion == 30 );
        m_Treasure3.EnableWindow ( iVersion == 30 );
        m_Treasure2.EnableWindow ( iVersion == 30 );
        m_Treasure1.EnableWindow ( iVersion == 30 );

        m_Map1.EnableWindow ( iVersion <= 30 );
        m_Map2.EnableWindow ( iVersion <= 30 );

        m_Item1.EnableWindow ( iVersion <= 30 );
        m_Item2.EnableWindow ( iVersion <= 30 );
        m_Item3.EnableWindow ( iVersion <= 30 );
        m_Item4.EnableWindow ( iVersion <= 30 );

        m_Key4.EnableWindow ( iVersion <= 30 );
        m_Key3.EnableWindow ( iVersion <= 30 );
        m_Key2.EnableWindow ( iVersion <= 30 );
        m_Key1.EnableWindow ( iVersion <= 30 );

        m_N_Map2.EnableWindow ( iVersion <= 30 );
        m_N_Map1.EnableWindow ( iVersion <= 30 );
        m_N_Key4.EnableWindow ( iVersion <= 30 );
        m_N_Key3.EnableWindow ( iVersion <= 30 );
        m_N_Key2.EnableWindow ( iVersion <= 30 );
        m_N_Key1.EnableWindow ( iVersion <= 30 );

        m_N_Item4.EnableWindow ( iVersion <= 30 );
        m_N_Item3.EnableWindow ( iVersion <= 30 );
        m_N_Item2.EnableWindow ( iVersion <= 30 );
        m_N_Item1.EnableWindow ( iVersion <= 30 );

        m_Zero.EnableWindow ( iVersion <= 30 );
        m_One.EnableWindow ( iVersion <= 30 );
        m_Four.EnableWindow ( iVersion <= 30 );

        m_Status.EnableWindow ( TRUE );
    }
    else
    {
        m_Treasure1.EnableWindow ( FALSE );
        m_Treasure2.EnableWindow ( FALSE );
        m_Treasure3.EnableWindow ( FALSE );
        m_Treasure4.EnableWindow ( FALSE );

        m_N_Treasure1.EnableWindow ( FALSE );
        m_N_Treasure2.EnableWindow ( FALSE );
        m_N_Treasure3.EnableWindow ( FALSE );
        m_N_Treasure4.EnableWindow ( FALSE );

        //
        m_Treasure4.EnableWindow ( FALSE );
        m_Treasure3.EnableWindow ( FALSE );
        m_Treasure2.EnableWindow ( FALSE );
        m_Treasure1.EnableWindow ( FALSE );

        m_Map1.EnableWindow ( FALSE );
        m_Map2.EnableWindow ( FALSE );

        m_Item1.EnableWindow ( FALSE );
        m_Item2.EnableWindow ( FALSE );
        m_Item3.EnableWindow ( FALSE );
        m_Item4.EnableWindow ( FALSE );

        m_Key4.EnableWindow ( FALSE );
        m_Key3.EnableWindow ( FALSE );
        m_Key2.EnableWindow ( FALSE );
        m_Key1.EnableWindow ( FALSE );

        m_N_Treasure4.EnableWindow ( FALSE );
        m_N_Treasure3.EnableWindow ( FALSE );
        m_N_Treasure2.EnableWindow ( FALSE );
        m_N_Treasure1.EnableWindow ( FALSE );

        m_N_Map2.EnableWindow ( FALSE );
        m_N_Map1.EnableWindow ( FALSE );
        m_N_Key4.EnableWindow ( FALSE );
        m_N_Key3.EnableWindow ( FALSE );
        m_N_Key2.EnableWindow ( FALSE );
        m_N_Key1.EnableWindow ( FALSE );

        m_N_Item4.EnableWindow ( FALSE );
        m_N_Item3.EnableWindow ( FALSE );
        m_N_Item2.EnableWindow ( FALSE );
        m_N_Item1.EnableWindow ( FALSE );

        m_Zero.EnableWindow ( FALSE );
        m_One.EnableWindow ( FALSE );
        m_Four.EnableWindow ( FALSE );

        m_Status.EnableWindow ( FALSE );
    }

    SetGUIModified ( bModified );

    return iVersion;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnBnClickedZero()
{
#define SET_VALUE(b,v,c)    m_##b.SetCheck ( ##c ); m_N_##b.SetWindowText (#v);

#define SET_VALUE_ZERO(b)   SET_VALUE(b,0,0)

    SET_VALUE_ZERO(Map1)
    SET_VALUE_ZERO(Map2)

    SET_VALUE_ZERO(Item1)
    SET_VALUE_ZERO(Item2)
    SET_VALUE_ZERO(Item3)
    SET_VALUE_ZERO(Item4)

    SET_VALUE_ZERO(Key1)
    SET_VALUE_ZERO(Key2)
    SET_VALUE_ZERO(Key3)
    SET_VALUE_ZERO(Key4)

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnBnClickedOne()
{
#define SET_VALUE_ONE(b)    SET_VALUE(b,1,1)

    SET_VALUE_ONE(Map1)
    SET_VALUE_ONE(Map2)

    SET_VALUE_ONE(Item1)
    SET_VALUE_ONE(Item2)
    SET_VALUE_ONE(Item3)
    SET_VALUE_ONE(Item4)

    SET_VALUE_ONE(Key1)
    SET_VALUE_ONE(Key2)
    SET_VALUE_ONE(Key3)
    SET_VALUE_ONE(Key4)

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnBnClickedFour()
{
#define SET_VALUE_FOUR(b)   SET_VALUE(b,4,1)

    SET_VALUE_FOUR(Map1)
    SET_VALUE_FOUR(Map2)

    SET_VALUE_FOUR(Item1)
    SET_VALUE_FOUR(Item2)
    SET_VALUE_FOUR(Item3)
    SET_VALUE_FOUR(Item4)

    SET_VALUE_FOUR(Key1)
    SET_VALUE_FOUR(Key2)
    SET_VALUE_FOUR(Key3)
    SET_VALUE_FOUR(Key4)

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItems::OnInitDialog()
{
    CTRXPropertyPage::OnInitDialog();

    // TODO: Add Code Here
    if ( m_bToolTip )
    {
        m_ToolTip.AddTool( &m_Status, ("Status"));

        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_01), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_02), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_03), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_ITEM_04), LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool ( GetDlgItem(IDC_KEY1), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_KEY2), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_KEY3), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_KEY4), LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool ( GetDlgItem(IDC_MAP1), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_MAP2), LPSTR_TEXTCALLBACK );

        m_ToolTip.AddTool ( GetDlgItem(IDC_TREASURE1), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_TREASURE2), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_TREASURE3), LPSTR_TEXTCALLBACK );
        m_ToolTip.AddTool ( GetDlgItem(IDC_TREASURE4), LPSTR_TEXTCALLBACK );

        m_ToolTip.Activate(TRUE);
    }

    m_bInitDone = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItems::OnApply()
{
    // TODO: Add Code Here
    WriteWhenOnApply("Items");

    return CTRXPropertyPage::OnApply();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnCancel()
{
    // TODO: Add Code Here
    if ( CTRSaveGame::I() != NULL && ( IsGUIModified() || CTRSaveGame::IsBufferModified() ) )
    {
        INT_PTR response = AskToSave("Items");
        DisplayValues ( );
    }

    CTRXPropertyPage::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnOK()
{
    INT_PTR response = AskToSave("Items");

    CTRXPropertyPage::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXItems::OnChangeEdit()
{
    SetGUIModified ( TRUE, "Items ChangeEdit" );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXItems::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
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
    static char szText [ MAX_PATH ] = "";

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
            int iVersion        = CTRSaveGame::GetVersion();
            int iFullVersion    = CTRSaveGame::GetFullVersion();
            int iLevelIndex     = CTRSaveGame::GetLevelIndex();

            const char *pLabel = CTRXRemastered::GetLabelForObject ( iFullVersion, iLevelIndex, iX );
            if ( strlen(pLabel) > 0 )
            {
                strcpy_s ( szText, sizeof(szText), pLabel );
                pText->lpszText = szText;
                return TRUE;
            }
            else if ( iX < 4 )
            {
                sprintf_s ( szText, sizeof(szText), "Object %d", iX + 1 );
                pText->lpszText = szText;
                return TRUE;
            }
            else if ( iX < 8 )
            {
                sprintf_s ( szText, sizeof(szText), "Key %d", iX + 1 - 4 );
                pText->lpszText = szText;
                return TRUE;
            }
            else if ( iX < 10 )
            {
                sprintf_s ( szText, sizeof(szText), "Map %d", iX + 1 - 8 );
                pText->lpszText = szText;
                return TRUE;
            }
            else
            {
                sprintf_s ( szText, sizeof(szText), "Item %d", iX + 1 - 10 );
                pText->lpszText = szText;
                return TRUE;
            }
            return FALSE;   
        }
    }

    return FALSE;
}

