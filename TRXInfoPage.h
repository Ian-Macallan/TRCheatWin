#pragma once

//
/////////////////////////////////////////////////////////////////////////////
// TRXInfoPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "TRXPropertyPage.h"
#include "TRXRoomPicture.h"
#include "afxwin.h"
#include "READTR2/READTRX.h"

#define COL_FILENAME    0
#define COL_GAME        1
#define COL_GAME_NO     2
#define COL_LEVEL_NO    3
#define COL_TITLE       4
#define COL_PATHNAME    5
#define COL_DATE        6

//
/////////////////////////////////////////////////////////////////////////////
// CTRXInfoPage dialog
//
/////////////////////////////////////////////////////////////////////////////
class CTRXInfoPage : public CTRXPropertyPage
{
    DECLARE_DYNCREATE(CTRXInfoPage)

    // Construction
    public:
        void DisplayDirectory(const char *pDirectory);
        BOOL SelectCustomFromDir (const char *pDirectory);
        BOOL SelectCustomFromPath (const char *pathname);
        //  If bManualChange we will not search best map
        void ChangeCustomCombo( bool bManualChange );
        int EnableForVersion();
        void FillListCtrl( const char *pDirectory);

        //
        void SetComboSelection ( );
        void SetComboSelection ( const char *pDirectory );

        void ResizeRoom ( long x, long z, long xSectors, long zSectors, long yTop, long yBottom );
        void ChangeRoom();

        CBitmap *m_pBitmapLevel;
        CBitmap *m_pBitmapContinent;

        HBITMAP m_hBitmapLevel;
        HBITMAP m_hBitmapContinent;

        HBITMAP m_hFirstBitmapLevel;
        HBITMAP m_hFirstBitmapContinent;

        CTRXListCtrlBase    *m_pListCtrl;
        int                 m_Line;

        CTRXRoomPicture     m_Room;
        bool                m_bRoomCreated;
        int                 m_Custom_Selected;

        CTRXInfoPage();
        ~CTRXInfoPage();

        //
        void UpdateBuffer();
        void DisplayValues();
        void LoadDirectory();
        void SortItems ( int col);
        void DisplayOne ( int line );
        void SetCurrent ( );
        void ShowRoom(TR_AREA *pArea, DWORD x, DWORD z, WORD wDirection);
        void ClickRoom ( UINT nFlags, CPoint point );
        BOOL ExtractAfterScript ( int tombraider, TR_MODE trMode, STRUCTLOCATION *pTable,
                                    const char *pScriptDirectory, const char *pathname );
        BOOL GetCustomParams ( TR_MODE *pTrMode, STRUCTLOCATION  **ppTable, char BASED_CODE **ppFilter, char **ppNames, char **ppTypeName );

        virtual void SetThemeChanged ( bool bDarkTheme );

    // Dialog Data
        //{{AFX_DATA(CTRXInfoPage)
        enum { IDD = IDD_INFO };
        CTRXListCtrlBase    m_ListCtrl;
        CTRXStaticBase      m_ImageContinent;
        CTRXStaticBase      m_ImageLevel;
        CTRXEditBase        m_Levelnumber;
        CTRXEditBase        m_Filename;
        CTRXEditBase        m_Savenumber;
        CTRXButtonBase      m_LoadDirectory;
        CTRXButtonBase      m_Write;
        CTRXEditBase        m_Status;
        CTRXButtonBase      m_Lara_Burning;
        CTRXEditBase        m_Nb_Secrets;
        CTRXEditBase        m_Secrets;
        CTRXEditBase        m_Level_Name;
        CTRXEditBase        m_Lara;
        CTRXButtonBase      m_Max;
        CTRXButtonBase      m_Refresh;
        CTRXCComboBoxBase   m_Combo;
        CTRXButtonBase      m_Remove;
        CTRXButtonBase      m_Journal;
        CTRXButtonBase      m_Secret_All;
        CTRXButtonBase      m_Map;
        CTRXEditBase        m_Area;
        CTRXEditBase        m_Direction;
        CTRXEditBase        m_West_East;
        CTRXEditBase        m_Vertical;
        CTRXEditBase        m_South_North;
        CTRXStaticBase      m_Position;
        CTRXCComboBoxBase   m_Custom_Combo;
        CTRXButtonBase      m_Remove_Custom;
        CTRXButtonBase      m_Add_Custom;
        CTRXButtonBase      m_See_Custom;
        CTRXButtonBase      m_Sort;
        CTRXStaticBase      m_Square;
        CTRXButtonBase      m_CopyPosition;
        CTRXButtonBase      m_PastePosition;

        //}}AFX_DATA


    // Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CTRXInfoPage)
        public:
        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();
        virtual void OnCancel();
        virtual void OnOK();
        virtual BOOL OnApply();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

    // Implementation
    protected:
        // Generated message map functions
        //{{AFX_MSG(CTRXInfoPage)
        afx_msg void OnWrite();
        afx_msg void OnLoad();
        afx_msg void OnHelp();
        afx_msg void OnClickListctrl(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnItemchangedListctrl(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnBnClickedMax();
        afx_msg void OnBnClickedRefresh();
        afx_msg void OnSelchangeCombo();
        afx_msg void OnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnBnClickedRemove();
        afx_msg void OnBnClickedLaraburning();
        afx_msg void OnBnClickedSecretAll();
        afx_msg void OnBnClickedJournal();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
        void DoDropFiles(const char *pFilename);

    public:
        afx_msg void OnBnClickedMap();
        afx_msg void OnChangeEdit();
        afx_msg void OnChangeWestEast();
        afx_msg void OnChangeSouthNorth();
        afx_msg void OnChangeOrientation();
        afx_msg void OnChangeRoom();

        afx_msg void OnBnClickedAddCustom();
        afx_msg void OnBnClickedRemoveCustom();
        afx_msg void OnSelchangeCustomCombo();
        afx_msg void OnDropdownCustomCombo();
        afx_msg void OnBnClickedSeeCustom();
        afx_msg void OnBnClickedSort();
        afx_msg void OnBnClickedCopypos();
        afx_msg void OnBnClickedPastepos();
};

