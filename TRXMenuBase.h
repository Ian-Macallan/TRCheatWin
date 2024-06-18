#pragma once
#include "afxwin.h"

//  Menu Indicator
#define ID_MNU_MASK         0xff
#define ID_SYS_MENU         0xff
#define ID_POP_MENU         0xfe

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CTRXMenuBase : public CMenu
{
    DECLARE_DYNCREATE(CTRXMenuBase)

    public:
        CTRXMenuBase(void);
        virtual ~CTRXMenuBase(void);

        //
        void SetOwnDraw ( HMENU hMenu, bool bOwnDrawn = true );

        int         m_iBitmapWidth;
        int         m_iBitmapHeight;

        //
        virtual BOOL CreateMenu();
        virtual BOOL CreatePopupMenu();

        //  Entry Point for Popup Menu
        virtual BOOL LoadMenu(LPCTSTR lpszResourceName);
        //  Entry Point for Popup Menu
        virtual BOOL LoadMenu(UINT nIDResource);

        //  Entry Point for System Menu
        CTRXMenuBase *SetSystemMenu(CWnd* pWnd, CMenu* pSysMenu);
        void UnSetSystemMenu(CMenu* pSysMenu);

        CTRXMenuBase *GetSubMenu ( int nPos );
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );
        virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
        virtual BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = NULL);

        virtual UINT GetMenuItemCount ( );
        virtual BOOL GetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE );
        virtual BOOL SetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE );
        virtual UINT EnableMenuItem ( UINT nIDEnableItem, UINT nEnable );

        void        MeasureMenuItem ( CDC *pDC, const char *pText, CSize *pSize );
        void        MeasureMenuItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
        void        DrawMenuItem (  LPDRAWITEMSTRUCT lpDrawItemStruct, CDC *pDC, CRect *pRect, const char *pText );
        void        DrawMenuItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );

        void        ComputeBitmapMaxSize ( );
        void        ComputeBitmapMaxSize ( HBITMAP hBitmap );
        void        DrawBitmap ( CDC *pDC, CBitmap *pBitmap, CRect *pRect );
        int         DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct );
        BOOL        SetMenuItemBitmaps ( UINT nPosition, UINT nFlags, const CBitmap *pBmpUnchecked, const CBitmap *pBmpChecked );

    protected :
        CWnd        *m_pWnd;
        CMenu       *m_pSubMenu;
        HICON       m_hCheckWhiteIcon;
        HICON       m_hCheckBlackIcon;
};

