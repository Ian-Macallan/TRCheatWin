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

#include "TRXPropertySheetStandard.h"
#include "TRXContainer.h"

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
// CTRXPropertySheetStandard
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXPropertySheetStandard, CTRXPropertySheet)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetStandard::CTRXPropertySheetStandard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
    CTRXPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_bApplyActive      = FALSE;
    m_iStandardAdded    = 0;
    AddStandardPage ();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetStandard::CTRXPropertySheetStandard(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage) :
        CTRXPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_iStandardAdded    = 0;
    AddStandardPage ();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXPropertySheetStandard::~CTRXPropertySheetStandard()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXPropertySheetStandard, CTRXPropertySheet) //{{AFX_MSG_MAP(CTRXPropertySheetStandard)
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
// CTRXPropertySheetStandard message handlers
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetStandard::OnInitDialog()
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
HCURSOR CTRXPropertySheetStandard::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetStandard::OnSysCommand(UINT nID, LPARAM lParam)
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
void CTRXPropertySheetStandard::OnSize(UINT nType, int cx, int cy)
{
    CTRXPropertySheet::OnSize(nType, cx, cy);

    // TODO: Add Your handler here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXPropertySheetStandard::OnMove(int x, int y)
{
    CTRXPropertySheet::OnMove(x, y);

    // TODO: Add Your handler here
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXPropertySheetStandard::DoModal()
{
    // TODO: Add Your handler here
    return CTRXPropertySheet::DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXPropertySheetStandard::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add Your handler here
    return CTRXPropertySheet::PreCreateWindow(cs);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CTRXPropertySheetStandard::OnGetDlgCode()
{
    // TODO: Add Code Here
    UINT iResult = CTRXPropertySheet::OnGetDlgCode();
    return iResult;
}

