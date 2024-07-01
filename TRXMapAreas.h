#pragma once

#include "TRXDialogBase.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "TRXRoomWall.h"
#include "TRXRectangle.h"
#include "TR_Areas.h"
#include "TRXVerticalMap.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXMapAreas
//
/////////////////////////////////////////////////////////////////////////////
class CTRXMapAreas : public CTRXDialogBase
{
        DECLARE_DYNAMIC(CTRXMapAreas)

    public:
        CTRXMapAreas(CWnd* pParent = NULL);   // constructeur standard
        virtual ~CTRXMapAreas();

        //
        void ShowLevelBetween ( int yTop, int yBottom, int room = -1  );
        void ShowLevelLess ( int yTop, int yBottom, int room = -1  );
        void ShowLevelGreater ( int yTop, int yBottom, int room = -1 );
        void ShowOne ( int index );
        //  Point in Client Coordinates
        int FindWindowAtCPoint( CPoint point);
        //  In Screen Coordinates
        int FindWindowAtPoint( POINT point);
        void SetCurSel ( CComboBox *pBox, long value );
        void ComputeRoomRect ( const RECT *pBase, RECT *pRect, TR_AREA *pArea );

        //  Direct Select Area
        void SelectAreaAndExit ( int area );
        //  Search Point : if not found use the text
        void SelectAreaAndExit ( const char *pText );
        void ShowMap ();
        void SetCurrentArea ( const TR_AREA *pArea, const TR_CUR_POSITION *pCurrentPosition );
        void SetTombraiderLevelIndex ( int iTombraider, int iLevelIndex );
        void SetClickable ( BOOL bClickable );

        void SetVerticalMap ( CTRXVerticalMap *pVerticalMap );

        // Données de boîte de dialogue
        enum { IDD = IDD_TRXMAPAREAS };

    protected :
        int                 m_iTombraider;
        int                 m_iTombraiderFull;
        int                 m_iLevel;
        int                 m_iLevelIndex;
        INT_PTR             m_iSelectedArea;

        int                 m_iCx;
        int                 m_iCy;

        //
        long                m_yMin;
        long                m_yMax;

        //  Min is the hghest point
        long                m_yMinTop;
        //  Max  is the deepest point
        long                m_yMaxTop;
        //  Min is the hghest point
        long                m_yMinBottom;
        //  Max  is the deepest point
        long                m_yMaxBottom;

        //  Add abs() to RECT.top and botton
        long                m_xMin;
        //  Add abs() to RECT.left and right
        long                m_zMin;

        //
        int                 m_iMoving;

        TR_AREA             m_CurrentArea;
        TR_CUR_POSITION     m_CurrentPosition;
        bool                m_bCurrentArea;
        bool                m_bCurrentPosition;

        BOOL                m_bClickable;

        CTRXRectangle       m_Rectangle;

        CTRXEditBase        m_Current;
        CTRXCComboBoxBase   m_LevelTop;
        CTRXCComboBoxBase   m_LevelBottom;
        CTRXButtonBase      m_Refresh;
        CTRXEditBase        m_Area;
        CTRXEditBase        m_Direction;


        CTRXVerticalMap     *m_pVerticalMap;

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);

        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnMove(int x, int y);

        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnBnClickedRefresh();
        afx_msg void OnChangeArea();
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnSelchangeLevelTop();
        afx_msg void OnSelchangeLevelBottom();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

        virtual void OnOK();
        virtual void OnCancel();
        virtual INT_PTR DoModal();
        afx_msg void OnClose();
};
