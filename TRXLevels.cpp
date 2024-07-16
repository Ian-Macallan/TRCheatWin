// TRXLevels.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
#include "TRXLevels.h"
#include "TRXInfoPage.h"
#include "TR9SaveGame.h"


// Bo�te de dialogue CTRXLevels
#define COL_LEVEL           0
#define COL_NAME            1
#define COL_DATA            2

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXLevels, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXLevels::CTRXLevels(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXLevels::IDD, pParent)
{
    m_pListCtrl = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXLevels::~CTRXLevels()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXLevels::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
    DDX_Control(pDX, IDC_LEVEL_LIST, m_LevelList);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXLevels, CTRXDialogBase)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXLevels::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    // TODO:  Ajoutez ici une initialisation suppl�mentaire
    //
    //
    if ( m_pListCtrl == NULL )
    {
        m_pListCtrl = &m_LevelList;

        m_LevelList.SetExtendedStyle(m_LevelList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

        int countCustom = 0;
        for ( int i = 0; i < TR4NGMAXLEVEL; i++ )
        {
            if ( strlen(CustomDataFiles [ i ].title) > 0 )
            {
                countCustom++;
            }
        }

        if ( countCustom > 0 )
        {
            m_iVersion  = 49;
        }

        m_LevelList.InsertColumn( COL_LEVEL, "Level", LVCFMT_RIGHT, 64 );
        m_LevelList.InsertColumn( COL_NAME, "Name", LVCFMT_LEFT, 320 );
        if ( m_iVersion == 49 )
        {
            m_LevelList.InsertColumn( COL_DATA, "Data File", LVCFMT_LEFT, 192 );
        }
        else
        {
            m_LevelList.InsertColumn( COL_DATA, "Variant", LVCFMT_LEFT, 192 );
        }

        //
        switch ( m_iVersion )
        {
            //
            case 1 :
            case 10 :
            case 15 :
            {
                for ( int level = 1; level <= TR1_LEVELS; level++ )
                {
                    int index = level - 1;

                    const char *pTitle = CTR9SaveGame::GetLevelName ( m_iVersion, level );
                    char szNumber [ 64 ];
                    sprintf_s ( szNumber, sizeof(szNumber), "%2d", level );
                    m_LevelList.InsertItem ( index, szNumber );
                    m_LevelList.SetItemText ( index, COL_NAME, pTitle );
                    if ( level >= TR1G_START )
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 1 (Unfinished Business)" );
                    }
                    else
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 1" );
                    }
                }
                break;
            }
            //
            case 2 :
            case 20 :
            case 25 :
            {
                for ( int level = 1; level <= TR2_LEVELS; level++ )
                {
                    int index = level - 1;

                    const char *pTitle = CTR9SaveGame::GetLevelName ( m_iVersion, level );
                    char szNumber [ 64 ];
                    sprintf_s ( szNumber, sizeof(szNumber), "%2d", level );
                    m_LevelList.InsertItem ( index, szNumber );
                    m_LevelList.SetItemText ( index, COL_NAME, pTitle );
                    if ( level >= TR2G_START )
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 2 (Golden Mask)" );
                    }
                    else
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 2" );
                    }
                }
                break;
            }
            //
            case 3 :
            case 30 :
            case 35 :
            case 39 :
            {
                for ( int level = 1; level <= TR3_LEVELS; level++ )
                {
                    int index = level - 1;

                    const char *pTitle = CTR9SaveGame::GetLevelName ( m_iVersion, level );
                    char szNumber [ 64 ];
                    sprintf_s ( szNumber, sizeof(szNumber), "%2d", level );
                    m_LevelList.InsertItem ( index, szNumber );
                    m_LevelList.SetItemText ( index, COL_NAME, pTitle );
                    if ( level >= TR3G_START )
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 3 (Lost Artifact)" );
                    }
                    else
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 3" );
                    }
                }
                break;
            }
            //
            case 4 :
            case 40 :
            case 45 :
            {
                for ( int level = 1; level <= TR4_LEVELS; level++ )
                {
                    int index = level - 1;

                    const char *pTitle = CTR9SaveGame::GetLevelName ( m_iVersion, level );
                    char szNumber [ 64 ];
                    sprintf_s ( szNumber, sizeof(szNumber), "%2d", level );
                    m_LevelList.InsertItem ( index, szNumber );
                    m_LevelList.SetItemText ( index, COL_NAME, pTitle );
                    if ( level >= TR4G_START )
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 4 (Times Exclusive)" );
                    }
                    else
                    {
                        m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 4 (Last Revelation)" );
                    }
                }
                break;
            }
            //
            case 49 :
            {
                for ( int levelIndex = 0; levelIndex < TR4NGMAXLEVEL; levelIndex++ )
                {
                    if ( strlen(CustomDataFiles [ levelIndex ].title) > 0 )
                    {
                        char szNumber [ 64 ];
                        sprintf_s ( szNumber, sizeof(szNumber), "%2d", levelIndex );
                        m_LevelList.InsertItem ( levelIndex, szNumber );
                        m_LevelList.SetItemText ( levelIndex, COL_NAME, CustomDataFiles [ levelIndex ].title );
                        m_LevelList.SetItemText ( levelIndex, COL_DATA, CustomDataFiles [ levelIndex ].datafile );
                    }
                }
                break;
            }
            //
            case 5 :
            case 50 :
            {
                for ( int level = 1; level <= TR5_LEVELS; level++ )
                {
                    int index = level - 1;

                    const char *pTitle = CTR9SaveGame::GetLevelName ( m_iVersion, level );
                    char szNumber [ 64 ];
                    sprintf_s ( szNumber, sizeof(szNumber), "%2d", level );
                    m_LevelList.InsertItem ( index, szNumber );
                    m_LevelList.SetItemText ( index, COL_NAME, pTitle );
                    m_LevelList.SetItemText ( index, COL_DATA, "Tombraider 5 (Chronicles)" );
                }
                break;
            }
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

