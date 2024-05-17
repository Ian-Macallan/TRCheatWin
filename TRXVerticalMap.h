#pragma once

#include "resource.h"

#include "TRXDialogBase.h"
#include "TR_Areas.h"
#include "afxwin.h"

//
/////////////////////////////////////////////////////////////////////////////
// Boîte de dialogue CTRXVerticalMap
//
/////////////////////////////////////////////////////////////////////////////
class CTRXVerticalMap : public CTRXDialogBase
{
		DECLARE_DYNAMIC(CTRXVerticalMap)

	public:
		CTRXVerticalMap(CWnd* pParent = NULL);   // constructeur standard
		virtual ~CTRXVerticalMap();

	// Données de boîte de dialogue
		enum { IDD = IDD_TRXVERTICALMAP };

	public :
		void SetParent ( CWnd *parent );
		void SetCurrentArea ( const TR_AREA *pArea, const TR_CUR_POSITION *pCurrentPosition );
		void SetSelectedArea ( const TR_AREA *pArea );
		void SetClickable ( BOOL bClickable );
		void SetTombraiderLevelIndex ( int iTombraider, int iLevelIndex );
		void SetWallOrientation();
		void ShowWindows();
		void Free();
		//	Point in Client Coordinates
		int FindWindowAtCPoint( CPoint point);
		//	In Screen Coordinates
		int FindWindowAtPoint( POINT point);

		//	Search Point if not found use Text and call parent SelectArea
		void SelectAreaAndExit ( const char *pText );

		void SetButtonFonts();

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
	public:
		CTRXButtonBase m_South;
		CTRXButtonBase m_West;
		CTRXButtonBase m_East;
		CTRXButtonBase m_North;

	public:
		virtual BOOL OnInitDialog();
		virtual BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnBnClickedFromNorth();
		afx_msg void OnBnClickedFromSouth();
		afx_msg void OnBnClickedFromWest();
		afx_msg void OnBnClickedFromEast();
		afx_msg void OnClose();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	protected:
		CWnd				*m_Parent;

		TR_AREA				m_SelectedArea;
		TR_AREA				m_CurrentArea;
		TR_CUR_POSITION		m_CurrentPosition;
		bool				m_bCurrentArea;
		bool				m_bSelectedArea;
		bool				m_bCurrentPosition;

		int					m_iTombraider;
		int					m_iTombraiderFull;
		int					m_iLevel;
		int					m_iLevelIndex;

		int					m_iMoving;
		BOOL				m_bClickable;

		//
		long				m_minX;
		long				m_maxX;
		long				m_minZ;
		long				m_maxZ;
		//	yBottom always greater than yTop
		//	So we search max yBottom and min Ytop
		long				m_minY;
		long				m_maxY;
	public:
		afx_msg void OnDestroy();
	
};
