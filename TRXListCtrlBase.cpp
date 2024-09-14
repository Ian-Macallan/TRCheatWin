#include "stdafx.h"
#include "TRXListCtrlBase.h"
#include "TRXTools.h"
#include "TRXGlobal.h"
#include "TRXColors.h"
#include "TRXGDI.h"
#include "TRXRemastered.h"
#include "TRXInfoPage.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXListCtrlBase, CListCtrl)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXListCtrlBase::CTRXListCtrlBase(void)
{
    m_bDefaultColor             = FALSE;
    m_bHeaderControlSubclassed  = false;

    m_bFontsDone                = FALSE;
    m_pBoldFont                 = NULL;
    m_pItalicFont               = NULL;
    m_pNormalFont               = NULL;
    m_pFixedFont                = NULL;
    m_pFixedBoldFont            = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXListCtrlBase::~CTRXListCtrlBase(void)
{
#define DELETE_OBJECT(o) if ( o != NULL ) { delete o; o = NULL; }

    DELETE_OBJECT(m_pItalicFont)
    DELETE_OBJECT(m_pBoldFont)
    DELETE_OBJECT(m_pFixedFont)
    DELETE_OBJECT(m_pFixedBoldFont)
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXListCtrlBase, CListCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CTRXListCtrlBase::OnNMCustomdraw)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXListCtrlBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //

    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    //
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXListCtrlBase::OnEraseBkgnd(CDC* pDC)
{
    //

    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CListCtrl::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXListCtrlBase::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    //
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    //
    if( GetHeaderCtrl ( ) && GetHeaderCtrl()->m_hWnd != NULL && m_bHeaderControlSubclassed == false )
    {
        m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
        m_bHeaderControlSubclassed = true;
        m_HeaderCtrl.RedrawWindow ( );
    }

    //  for this notification, the structure is actually a
    //  NMLVCUSTOMDRAW that tells you what's going on with the custom
    //  draw action. So, we'll need to cast the generic pNMHDR pointer.
    LPNMLVCUSTOMDRAW    pLVCD   = (LPNMLVCUSTOMDRAW)pNMHDR;

    if ( CTRXColors::m_iDarkTheme == 0 )
    {
        *pResult = CDRF_DODEFAULT;
        return;
    }

    if ( pNMCD )
    {
        switch ( pNMCD->dwDrawStage )
        {
            case CDDS_PREPAINT :
            {
                *pResult = CDRF_NOTIFYITEMDRAW;
                break;
            }

            case CDDS_ITEMPREPAINT :
            {
                *pResult = CDRF_NOTIFYSUBITEMDRAW;
                break;
            }
    
            case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
            {
                //
                if ( ! m_bFontsDone )
                {
                    m_bFontsDone    = TRUE;

                    TEXTMETRIC textMetrics;
                    CDC *pDC = CDC::FromHandle ( pNMCD->hdc );
                    GetTextMetrics ( pDC->m_hDC, &textMetrics );

                    //  Create Fonts
                    LOGFONT logDefaultFont;
                    LOGFONT logFont;

                    m_pNormalFont               = GetFont();
                    m_pNormalFont->GetLogFont(&logDefaultFont);

                    logFont                     = logDefaultFont;
                    logFont.lfWeight            = FW_BOLD;
                    m_pBoldFont                 = new CFont();
                    m_pBoldFont->CreateFontIndirect(&logFont);    // Create the font.

                    logFont                     = logDefaultFont;
                    logFont.lfItalic            = TRUE;
                    m_pItalicFont               = new CFont();
                    m_pItalicFont->CreateFontIndirect(&logFont);    // Create the font.

                    logFont                     = logDefaultFont;
                    m_pFixedFont                = new CFont();
                    logFont.lfPitchAndFamily    = FIXED_PITCH;
                    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
                    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
                    m_pFixedFont->CreateFontIndirect(&logFont);    // Create the font.

                    logFont                     = logDefaultFont;
                    m_pFixedBoldFont            = new CFont();
                    logFont.lfPitchAndFamily    = FIXED_PITCH;
                    logFont.lfWeight            = FW_BOLD;
                    logFont.lfHeight            = (LONG) ( (float) textMetrics.tmHeight * 1.10 );
                    strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), "Courier New");
                    m_pFixedBoldFont->CreateFontIndirect(&logFont);    // Create the font.
                }

                CRect rectLabel;

                int iCol            = pLVCD->iSubItem;
                int iRow            = ( int ) pNMCD->dwItemSpec;
                CString strItem     = GetItemText ( iRow, iCol );
                BOOL bResult        = GetSubItemRect ( iRow, iCol, LVIR_LABEL, rectLabel );
                BOOL bSelected      = GetItemState ( iRow, LVIS_SELECTED ) & LVIS_SELECTED;
                DWORD_PTR dwData    = GetItemData ( iRow );

                //
                HDITEM  tagHeaderInfo;
                memset ( &tagHeaderInfo, 0, sizeof ( tagHeaderInfo ) );

                tagHeaderInfo.mask          =  HDI_FORMAT;
                GetHeaderCtrl()->GetItem ( iCol, &tagHeaderInfo );

                // get the device context.
                CDC *pDC = CDC::FromHandle ( pNMCD->hdc );

                //  Test If line is selected
                if ( bSelected )
                {
                    pDC->FillRect ( &rectLabel, CTRXColors::GetBKSelectedCBrush(true) );
                    pDC->SetTextColor ( CTRXColors::GetFGSelectedCR(true)  );
                }
                else
                {
                    if ( iCol % 1 == 0 )
                    {
                        pDC->FillRect ( &rectLabel, CTRXColors::GetBKNormalCBrush(true) );
                        pDC->SetTextColor ( CTRXColors::GetFGNormalCR(true) );
                    }
                    else
                    {
                        pDC->FillRect ( &rectLabel, CTRXColors::GetBKNormalCBrush(true) );
                        pDC->SetTextColor ( CTRXColors::GetFGNormalCR(true) );
                    }
                }

                //
                //  Paint The Text
                rectLabel.left  += 2;
                rectLabel.right -= 2;
                UINT nFormat = DT_SINGLELINE|DT_VCENTER;
                if ( tagHeaderInfo.fmt  & HDF_LEFT )
                {
                    nFormat |= DT_LEFT;
                }
                else if ( tagHeaderInfo.fmt  & HDF_RIGHT  )
                {
                    nFormat |= DT_RIGHT;
                }
                else if ( tagHeaderInfo.fmt  & HDF_CENTER )
                {
                    nFormat |= DT_CENTER;
                }

                CFont *oldFont = NULL;
                if ( dwData == (DWORD_PTR) ITEM_ITALIC )
                {
                    pDC->SelectObject(m_pItalicFont);
                }

                pDC->DrawText(strItem , rectLabel, nFormat );

                if ( oldFont != NULL )
                {
                    pDC->SelectObject(oldFont);
                }

                *pResult= CDRF_SKIPDEFAULT;

                break;
            }

            default:
            {
                *pResult = CDRF_DODEFAULT;
                break;
            }
        }
    }
}

