#pragma once

#include "resource.h"

#include "TR8SaveGame.h"

#include "TRXPropertyPage456.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "TRXRoomPicture.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXRemastered
//
/////////////////////////////////////////////////////////////////////////////
class CTRXRemastered456 : public CTRXPropertyPage456
{
        DECLARE_DYNAMIC(CTRXRemastered456)

    public:
        CTRXRemastered456();
        virtual ~CTRXRemastered456();

    // Données de boîte de dialogue
        enum { IDD = IDD_REMASTERED_456 };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:

        CTRXRoomPicture     m_Room;
        bool                m_bRoomCreated;

        CTRXListCtrlBase    *m_pListCtrl;
        int                 m_Line;

        CTRXListCtrlBase    m_ListCtrl;

        //
        CTRXButtonBase m_Load;
        CTRXButtonBase m_Write;
        CTRXButtonBase m_Refresh;
        CTRXButtonBase m_Max;
        CTRXButtonBase m_List_All;

        //
        CTRXEditBase m_Filename;
        CTRXCComboBoxBase m_Combo;
        //
        CTRXEditBase m_Default;

        CTRXEditBase m_Status;

        //
        CTRXButtonBase m_Max_One;

        //  Check Box Gun
        CTRXButtonBase m_Guns;
        CTRXButtonBase m_Magnum;
        CTRXButtonBase m_Uzi;
        CTRXButtonBase m_Riotgun;
        CTRXButtonBase m_M16;
        CTRXButtonBase m_Grenade;
        CTRXButtonBase m_Rocket;
        CTRXButtonBase m_Harpoon;

        //  Ammos
        CTRXEditBase m_AMagnum;
        CTRXEditBase m_AUzi;
        CTRXEditBase m_ARiotgun;
        CTRXEditBase m_AM16;
        CTRXEditBase m_AGrenade;
        CTRXEditBase m_ARocket;
        CTRXEditBase m_AHarpoon;

        //
        CTRXEditBase m_AGuns;
        CTRXEditBase m_LargePak;
        CTRXEditBase m_SmallPak;
        CTRXEditBase m_Flare;

        //
        CTRXEditBase m_Air;
        CTRXButtonBase m_Air_Full;
        CTRXButtonBase m_Air_Infinite;
        CTRXEditBase m_State;

        //  From Tag
        CTRXEditBase m_XGuns;
        CTRXEditBase m_XMagnum;
        CTRXEditBase m_XUzi;
        CTRXEditBase m_XRiotgun;
        CTRXEditBase m_XM16;
        CTRXEditBase m_XGrenade;
        CTRXEditBase m_XRocket;
        CTRXEditBase m_XHarpoon;


        CTRXButtonBase m_Update;
        CTRXButtonBase m_Burning;
        CTRXButtonBase m_Remove_Loc;

        CTRXEditBase m_Real_Health;
        CTRXButtonBase m_Invincible;

        CTRXEditBase m_Pickup;
        CTRXEditBase m_Crystal;
        CTRXEditBase m_Kill;
        CTRXEditBase m_Secrets_A;
        CTRXEditBase m_Secrets_C;
        CTRXEditBase m_Secrets_E;

        CTRXEditBase m_Secrets_Max;
        CTRXEditBase m_Col_Crystal;
        CTRXButtonBase m_Set;

        CTRXButtonBase m_Show;
        CTRXButtonBase m_TR1_Plus;
        CTRXButtonBase m_TR2_Plus;
        CTRXButtonBase m_TR3_Plus;
        CTRXButtonBase m_Game_Plus;

        CTRXButtonBase m_Kill_Willard;
        CTRXButtonBase m_Infos;

        CTRXEditBase m_Save_No;
        CTRXEditBase m_Pickup_Max;
        CTRXEditBase m_Crystal_Max;
        CTRXEditBase m_Kill_Max;

        CTRXButtonBase m_ResetToPlus;
        CTRXButtonBase m_ResetGames;
        CTRXButtonBase m_Journal;

        CTRXEditBase m_Uzi_Clip;

        CTRXEditBase m_M16_Clip;
        CTRXEditBase m_Grenade_Clip;

        CTRXButtonBase m_Position;
        CTRXButtonBase m_Show_Map;

        CTRXStaticBase m_Frame_Position;
        CTRXButtonBase m_Shell;

        CTRXStaticBase m_Position_Label;

        CTRXButtonBase m_Items;

        CTRXButtonBase m_Binocular;
        CTRXButtonBase m_Laser;
        CTRXButtonBase m_Crowbar;

        //
        afx_msg void OnBnClickedLoad();
        afx_msg void OnBnClickedWrite();
        afx_msg void OnClickListctrl(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnBnClickedRefresh();
        afx_msg void OnItemchangedListctrl(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnBnClickedMax();
        afx_msg void OnBnClickedMaxOne();
        afx_msg void OnBnClickedArFull();
        afx_msg void OnBnClickedArInfinite();
        afx_msg void OnBnClickedListAll();
        afx_msg void OnBnClickedUpdate();
        afx_msg void OnSelchangeCombo();
        afx_msg void OnBnClickedBurning();
        afx_msg void OnBnClickedRemoveLoc();
        afx_msg void OnBnClickedStrong();
        afx_msg void OnBnClickedShow();
        afx_msg void OnBnClickedSet();
        afx_msg void OnBnClickedKillWillard();
        afx_msg void OnRclickListctrl(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnMenulistDelete();
        afx_msg void OnMenulistExport();
        afx_msg void OnMenulistImport();
        afx_msg void OnMenulistInfos();
        afx_msg void OnMenulistPosition();
        afx_msg void OnMenulistRenumber();
        afx_msg void OnMenulistresettoplus();
        afx_msg void OnMenulistresettostart();
        afx_msg void OnChangeEdit();
        afx_msg void OnClickedMagnum();
        afx_msg void OnClickedUzi();
        afx_msg void OnClickedRiotgun();
        afx_msg void OnClickedM16();
        afx_msg void OnClickedGrenade();
        afx_msg void OnClickedHarpoon();
        afx_msg void OnClickedGun();
        afx_msg void OnClickedRocket();
        afx_msg void OnClickedTr1Plus();
        afx_msg void OnClickedTr2Plus();
        afx_msg void OnBnClickedTr3Plus();
        afx_msg void OnBnClickedGamePlus();
        afx_msg void OnBnClickedReset();
        afx_msg void OnBnClickedResetgame();
        afx_msg void OnBnClickedJournal();

        afx_msg void OnSelendokCombo();
        afx_msg void OnBnClickedPosition();
        afx_msg void OnBnClickedShowMap();
        afx_msg void OnHelp();
        afx_msg void OnBnClickedShell();
        afx_msg void OnBnClickedItems();

        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
        virtual BOOL OnInitDialog();
        virtual BOOL OnSetActive();
        virtual BOOL OnKillActive();
        virtual void OnOK();
        virtual BOOL OnApply();
        virtual void OnCancel();
        virtual void SetThemeChanged ( bool bDarkTheme );

        //
        void SetComboSelection ( );
        void SetComboSelection ( const char *pFilename );

        //  value = -1 : value of the Cedit Entry
        void SetBlockObjectOnDisplay(int tombraider, int block, int value);

        //  Read / Load / Display
        void DisplayList ( const char *pFilename );

        //  Assume file data are loaded
        void DisplayList ( );
        void DisplayListBrief ( );
        void DisplayListFull ( bool bShort  );
        void DisplayOne ( int line );

        void SetValue ( CEdit &edit, int value, BOOL enable, BOOL bShowAnyWay = FALSE );
        void SetValue ( CButton &button, int check, BOOL enable, BOOL bShowAnyWay = FALSE );

        int GetValue ( CEdit &edit );
        BOOL GetValue ( CButton &edit );

        void UpdateBuffer( );

        //  Update Block
        void UpdateBlock(TABLE_TR4 *pBlock, bool bMax, int level );
        //  Update Block and Block that has the Gun address
        void UpdateBlock(int tombraider, int block, TABLE_TR4 *pBlock, const GUN_TR4 *pTag, bool bMax, int level );
        void UpdateGun(int tombraider, int block, GUN_TR4 *pTag, TABLE_TR4 *pBlock, bool bMax, int level );
        void UpdateBuffer(int tombraider, int block, TABLE_TR4 *pBlock, GUN_TR4 *pTag, bool bMax );

        //  Update Block
        void UpdateBlock(TABLE_TR5 *pBlock, bool bMax, int level );
        //  Update Block and Block that has the Gun address
        void UpdateBlock(int tombraider, int block, TABLE_TR5 *pBlock, GUN_TR5 *pTag, bool bMax, int level );
        void UpdateGun(int tombraider, int block, GUN_TR5 *pTag, TABLE_TR5 *pBlock, bool bMax, int level );
        void UpdateBuffer(int tombraider, int block, TABLE_TR5 *pBlock, GUN_TR5 *pTag, bool bMax );

        //  Update Block
        void UpdateBlock(TABLE_TR6 *pBlock, bool bMax, int level );
        //  Update Block and Block that has the Gun address
        void UpdateBlock(int tombraider, int block, TABLE_TR6 *pBlock, GUN_TR6 *pTag, bool bMax, int level );
        void UpdateGun(int tombraider, int block, GUN_TR6 *pTag, TABLE_TR6 *pBlock, bool bMax, int level );
        void UpdateBuffer(int tombraider, int block, TABLE_TR6 *pBlock, GUN_TR6 *pTag, bool bMax );

        void SortItems ( int col );

        void MaxOne ( int line );
        void ResetToPlus ( int line );
        void ResetToGame ( int line );

        //
        // BOOL AddLocation ( const char *pLocation );
        // BOOL RemoveLocation ( const char *pLocation );
        // int CountLocation ( );

        void Enable ( int tombraider, int level );

        DWORD RelativeAddress ( const void *pAddress );

        void Renumber();
        void DeleteOneGame();
        void ShowInfo();
        void ShowPosition();
        void ShowMap();
        void ShowRoom();

        void WriteFile();

        void SingleClick();
        void DoDropFiles(const char *pFilename);

        //  Ca Be Use by other
        static const char *GetLabelForObject ( int tombraider, int levelIndex, int iObject, bool bDouble = false );
        CButton m_HeadSet;
};
