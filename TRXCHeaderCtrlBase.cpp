#include "stdafx.h"
#include "TRXCHeaderCtrlBase.h"
#include "TRXTools.h"
#include "TRXColors.h"
#include "TRXGlobal.h"
#include "TRXGDI.h"

#define LEN_TEXT64  64

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXCHeaderCtrlBase, CHeaderCtrl)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXCHeaderCtrlBase::CTRXCHeaderCtrlBase(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXCHeaderCtrlBase::~CTRXCHeaderCtrlBase(void)
{
}
BEGIN_MESSAGE_MAP(CTRXCHeaderCtrlBase, CHeaderCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CTRXCHeaderCtrlBase::OnNMCustomdraw)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTRXCHeaderCtrlBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO
    HBRUSH hBrush = CTRXColors::OnCtlColor ( pDC, pWnd, nCtlColor );
    if ( hBrush != NULL )
    {
        return hBrush;
    }

    HBRUSH hbr = CHeaderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO
    return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXCHeaderCtrlBase::OnEraseBkgnd(CDC* pDC)
{
    // TODO
    if ( CTRXColors::OnEraseBkgnd(pDC, this ) )
    {
        return TRUE;
    }

    return CHeaderCtrl::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXCHeaderCtrlBase::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    // TODO

    if ( CTRXGlobal::m_iDarkTheme == 0 )
    {
        *pResult = CDRF_DODEFAULT;
        return;
    }

    if ( pNMCD )
    {
        switch(pNMCD->dwDrawStage)
        {
            case CDDS_PREPAINT:
            {
                *pResult = CDRF_NOTIFYITEMDRAW;
                *pResult = CDRF_NOTIFYSUBITEMDRAW;
                break;
            }

//          case CDDS_ITEMPREPAINT:
//          {
//              *pResult = CDRF_NOTIFYSUBITEMDRAW;
//              break;
//          }

            case CDDS_SUBITEM:
            case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
            case CDDS_ITEMPREPAINT:
            {
                int iCol = ( int ) pNMCD->dwItemSpec;

                HDITEM  tagHeaderInfo;
                memset ( &tagHeaderInfo, 0, sizeof ( tagHeaderInfo ) );

                char        szText [ LEN_TEXT64 ];
                ZeroMemory ( szText, sizeof(szText) );

                tagHeaderInfo.mask          = HDI_TEXT | HDI_IMAGE | HDI_FORMAT;
                tagHeaderInfo.pszText       = szText;
                tagHeaderInfo.cchTextMax    = sizeof ( szText ) - 1;
                GetItem ( iCol, &tagHeaderInfo );

                CRect rectLabel;
                BOOL bResult = GetItemRect( iCol, rectLabel );

                // get the device context.
                CDC *pDC= CDC::FromHandle ( pNMCD->hdc );

                pDC->SetBkMode ( TRANSPARENT );
                pDC->SetTextColor ( CTRXColors::GetWhiteCR() );
                if ( tagHeaderInfo.iImage == 0 )
                {
                    pDC->FillRect ( &rectLabel, CTRXColors::GetBlack32CBrush() );
                    // EraseRectWithGrade0 ( &rectLabel, pDC );
                    // DrawBitmap ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 1 )
                {
                    pDC->FillRect ( &rectLabel, CTRXColors::GetBlack32CBrush() );
                    // EraseRectWithGrade1 ( &rectLabel, pDC );
                    // DrawBitmapDes ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 2 )
                {
                    pDC->FillRect ( &rectLabel, CTRXColors::GetBlack32CBrush() );
                    // EraseRectWithGrade2 ( &rectLabel, pDC );
                    // DrawBitmapAsc ( pDC->m_hDC, &rectLabel );
                }
                else
                {
                    if ( iCol % 2 == 0 )
                    {
                        pDC->FillRect ( &rectLabel, CTRXColors::GetBlack32CBrush() );
                    }
                    else
                    {
                        pDC->FillRect ( &rectLabel, CTRXColors::GetBlack48CBrush() );
                    }

                    //DrawBitmap ( pDC->m_hDC, &rectLabel );

                }

                // paint the text.
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

                pDC->DrawText(szText, rectLabel, nFormat);

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
