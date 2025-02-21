#pragma once

#include "resource.h"
#include "afxwin.h"

#include "TRXDialogBase.h"
#include "TR_Areas.h"
#include "TRXRoomPicture.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXPosition
//
/////////////////////////////////////////////////////////////////////////////
class CTRXPosition : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXPosition)

    public:
        CTRXPosition(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXPosition();

        void ResizeRoom ( long x, long z, long xSectors, long zSectors, long yTop, long yBottom );
        void ClickRoom ( UINT nFlags, CPoint point );
        void ChangeAreas();
        void ChangeAreas( int room );

        CPoint ComputeRoomPoint ( );
    // Données de boîte de dialogue
        enum { IDD = IDD_POSITION };

    public:
        int                 m_iTombraider;
        int                 m_iBlock;
        int                 m_iSlot;
        int                 m_iLevel;
        int                 m_iArea;
        CTRXRoomPicture     m_Room;
        BOOL                m_123;
        BOOL                m_456;
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        afx_msg void OnBnClickedOk();
        afx_msg void OnSelchangeAreas();
        afx_msg void OnBnClickedSearch();
        afx_msg void OnBnClickedMap();
        afx_msg void OnChangeWestEastM();
        afx_msg void OnChangeSouthNorthM();
        afx_msg void OnChangeOrientationD();
        afx_msg void OnBnClickedCopypos();
        afx_msg void OnBnClickedPastepos();

        CTRXEditBase m_West_East;
        CTRXEditBase m_West_East_M;
        CTRXEditBase m_West_East_Old;
        CTRXEditBase m_West_East_Range;

        CTRXEditBase m_Vertical;
        CTRXEditBase m_Vertical_M;
        CTRXEditBase m_Vertical_Old;
        CTRXEditBase m_Vertical_Range;

        CTRXEditBase m_South_North;
        CTRXEditBase m_South_North_M;
        CTRXEditBase m_South_North_Old;
        CTRXEditBase m_South_North_Range;

        CTRXEditBase m_Word0;
        CTRXEditBase m_Word0_X;
        CTRXEditBase m_Word0_Old;

        CTRXEditBase m_Word1;
        CTRXEditBase m_Word1_D;
        CTRXEditBase m_Word1_Old;

        CTRXEditBase m_Word2;
        CTRXEditBase m_Word2_X;
        CTRXEditBase m_Word2_Old;

        CTRXEditBase m_Word3;
        CTRXEditBase m_Word3_X;
        CTRXEditBase m_Word3_Old;

        CTRXEditBase m_Word4;
        CTRXEditBase m_Word4_X;
        CTRXEditBase m_Word4_Old;

        CTRXEditBase m_Word5;
        CTRXEditBase m_Word5_X;
        CTRXEditBase m_Word5_Old;

        CTRXButtonBase m_Search;
        CTRXCComboBoxBase m_AreasList;
        CTRXButtonBase m_OK;
        CTRXButtonBase m_Map;
        CTRXStaticBase m_Ranges;
        CTRXButtonBase m_CopyPos;
        CTRXButtonBase m_PastePos;

        //
        CTRXStaticBase m_Left;
        CTRXStaticBase m_Right;
};
