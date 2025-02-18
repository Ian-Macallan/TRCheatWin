// TR3PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TRXCHEATWIN.h"
#include "TRXCHEATWINDlg.h"

#include "TRXEquipmentPage.h"   // Added by ClassView
#include "TRXGunPage.h" // Added by ClassView
#include "TRXInfoPage.h"    // Added by ClassView
#include "TRXItems.h"   // Added by ClassView
#include "TRXItemsTR4.h"    // Added by ClassView
#include "TRXAmmosPage.h"   // Added by ClassView
#include "TRXRemastered.h"  // Added by ClassView

#include "TRXPropertySheetRemastered.h"

#include "AutomaticVersionHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
extern CTRXCHEATWINApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetRemastered
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertySheetRemastered, CTRXPropertySheet)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetRemastered::CTRXPropertySheetRemastered(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
    CTRXPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_iRemastered123Added  = 0;
    AddRemastered123Page ();

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetRemastered::CTRXPropertySheetRemastered(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage) :
        CTRXPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_iRemastered123Added  = 0;
    AddRemastered123Page ();

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetRemastered::~CTRXPropertySheetRemastered()
{
    RemoveRemastered123Page();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertySheetRemastered, CTRXPropertySheet) //{{AFX_MSG_MAP(CTRXPropertySheetRemastered)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_GETDLGCODE()
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTRXPropertySheetRemastered message handlers
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetRemastered::OnInitDialog()
{
    //  Parent Is NULL
    BOOL bResult = CTRXPropertySheet::OnInitDialog();

    m_bInitDone = true;

    return bResult;
}

//
/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//
/////////////////////////////////////////////////////////////////////////////
HCURSOR CTRXPropertySheetRemastered::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetRemastered::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CTRXPropertySheet::OnSysCommand(nID, lParam);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetRemastered::OnSize(UINT nType, int cx, int cy)
{
    CTRXPropertySheet::OnSize(nType, cx, cy);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetRemastered::OnMove(int x, int y)
{
    CTRXPropertySheet::OnMove(x, y);

    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXPropertySheetRemastered::DoModal()
{
    //
    return CTRXPropertySheet::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetRemastered::PreCreateWindow(CREATESTRUCT& cs)
{
    //
    return CTRXPropertySheet::PreCreateWindow(cs);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXPropertySheetRemastered::OnGetDlgCode()
{
    //
    UINT iResult = CTRXPropertySheet::OnGetDlgCode();
    return iResult;
}
