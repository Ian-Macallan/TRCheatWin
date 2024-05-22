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
	m_bDefaultColor				= FALSE;
	m_bHeaderControlSubclassed	= false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXListCtrlBase::~CTRXListCtrlBase(void)
{
}
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
	// TODO: Add Code Here

	HBRUSH hBrush = CTRXTools::OnCtlColor ( pDC, pWnd, nCtlColor );
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Add Code Here
	return hbr;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXListCtrlBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add Code Here

	if ( CTRXTools::OnEraseBkgnd(pDC, this ) )
	{
		return TRUE;
	}

	return CListCtrl::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXListCtrlBase::SetColorsNotUsed()
{
	if ( CTRXGlobal::m_iDarkTheme == 0 )
	{
		if ( m_bDefaultColor )
		{
			SetTextBkColor ( m_DefaultBkColor );
			SetTextColor ( m_DefaultTextColor );
		}

		return;
	}

	//
	if ( ! m_bDefaultColor )
	{
		m_DefaultTextColor	= GetTextColor();
		m_DefaultBkColor	= GetTextBkColor();
		m_bDefaultColor		= TRUE;
	}

	//	Does not work
	SetTextBkColor ( CTRXTools::GetBkColor() );
	SetTextColor ( CTRXTools::GetTextColor() );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXListCtrlBase::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add Code Here
	if( GetHeaderCtrl ( ) && m_bHeaderControlSubclassed == false )
	{
		m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
		m_bHeaderControlSubclassed = true;
		m_HeaderCtrl.RedrawWindow ( );
	}

	//	for this notification, the structure is actually a
	//	NMLVCUSTOMDRAW that tells you what's going on with the custom
	//	draw action. So, we'll need to cast the generic pNMHDR pointer.
	LPNMLVCUSTOMDRAW	pLVCD	= (LPNMLVCUSTOMDRAW)pNMHDR;

	if ( CTRXGlobal::m_iDarkTheme == 0 )
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
				CRect rectLabel;

				int iCol		= pLVCD->iSubItem;
				int iRow		= ( int ) pNMCD->dwItemSpec;
				CString strItem = GetItemText ( iRow, iCol );
				BOOL bResult	= GetSubItemRect ( iRow, iCol, LVIR_LABEL, rectLabel );
				BOOL bSelected	= GetItemState ( iRow, LVIS_SELECTED ) & LVIS_SELECTED;

				//
				HDITEM	tagHeaderInfo;
				memset ( &tagHeaderInfo, 0, sizeof ( tagHeaderInfo ) );

				tagHeaderInfo.mask			=  HDI_FORMAT;
				GetHeaderCtrl()->GetItem ( iCol, &tagHeaderInfo );

				// get the device context.
				CDC *pDC= CDC::FromHandle ( pNMCD->hdc );

				//	Test If line is selected
				if ( bSelected )
				{
					pDC->FillRect ( &rectLabel, CTRXColors::GetBlackCBrush() );
					pDC->SetTextColor ( CTRXColors::GetWhiteCR() );
				}
				else
				{
					if ( iCol % 1 == 0 )
					{
						pDC->FillRect ( &rectLabel, CTRXColors::GetBlack64CBrush() );
						pDC->SetTextColor ( CTRXColors::GetWhite208CR() );
					}
					else
					{
						pDC->FillRect ( &rectLabel, CTRXColors::GetBlack32CBrush() );
						pDC->SetTextColor ( CTRXColors::GetWhite224CR() );
					}
				}

				//
				//	Paint The Text
				rectLabel.left	+= 2;
				rectLabel.right	-= 2;
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
				pDC->DrawText(strItem , rectLabel, nFormat );

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

