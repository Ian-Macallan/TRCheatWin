// TRXDifferences.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TRXDifferences.h"
// #include "afxdialogex.h"
// #include <Commctrl.h>

#include "TRXTools.h"
#include "TR10SaveGame.h"
#include "TR15SaveGame.h"
#include "TR2SaveGame.h"
#include "TR3SaveGame.h"
#include "TR4SaveGame.h"
#include "TR4NGSaveGame.h"
#include "TR5SaveGame.h"

#define COL_ADDRESS     0
#define COL_OLD         1
#define COL_NEW         2
#define COL_DEC_OLD     3
#define COL_DEC_NEW     4
#define COL_LABEL       5


// Boîte de dialogue CTRXDifferences

#ifndef HDF_CHECKBOX
#define HDF_CHECKBOX            0x0040
#endif

#ifndef HDF_CHECKED
#define HDF_CHECKED             0x0080
#endif

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTRXDifferences, CTRXDialogBase)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXDifferences::CTRXDifferences(CWnd* pParent /*=NULL*/) : CTRXDialogBase(CTRXDifferences::IDD, pParent)
{
    m_pListCtrl     = NULL;
    m_bRemastered   = FALSE;
    m_bInitList     = FALSE;
    m_Buttons       = MB_OKCANCEL;
    m_Result        = -1;
    ZeroMemory ( m_szTitle, sizeof(m_szTitle) );
    ZeroMemory ( m_szMessage, sizeof(m_szMessage) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CTRXDifferences::~CTRXDifferences()
{
    //
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXDifferences::MessageBoxR( const char *pMessage, const char *pTitle, UINT buttons )
{
    CTRXDifferences dlg;

    dlg.m_bRemastered   = TRUE;
    dlg.m_Buttons       = buttons;
    strcpy_s ( dlg.m_szTitle, sizeof(dlg.m_szTitle), pTitle );
    strcpy_s ( dlg.m_szMessage, sizeof(dlg.m_szMessage), pMessage );
    
    return dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXDifferences::MessageBoxS( const char *pMessage, const char *pTitle, UINT buttons)
{
    CTRXDifferences dlg;

    dlg.m_bRemastered   = FALSE;
    dlg.m_Buttons       = buttons;
    dlg.m_iVersion      = CTRSaveGame::GetFullVersion();

    strcpy_s ( dlg.m_szTitle, sizeof(dlg.m_szTitle), pTitle );
    strcpy_s ( dlg.m_szMessage, sizeof(dlg.m_szMessage), pMessage );

    return dlg.DoModal();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::DoDataExchange(CDataExchange* pDX)
{
    CTRXDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DIFFERENCES, m_ListCtrl);
    DDX_Control(pDX, IDOK, m_OK);
    DDX_Control(pDX, IDCANCEL, m_Cancel);
    DDX_Control(pDX, IDYES, m_Yes);
    DDX_Control(pDX, IDNO, m_No);
    DDX_Control(pDX, IDC_MESSAGE, m_Message);
    DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTRXDifferences, CTRXDialogBase)
    ON_BN_CLICKED(IDOK, &CTRXDifferences::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CTRXDifferences::OnBnClickedCancel)
    ON_BN_CLICKED(IDYES, &CTRXDifferences::OnBnClickedYes)
    ON_BN_CLICKED(IDNO, &CTRXDifferences::OnBnClickedNo)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_DIFFERENCES, &CTRXDifferences::OnItemchangedDifferences)
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////////////////
// Gestionnaires de messages de CTRXDifferences
//
/////////////////////////////////////////////////////////////////////////////

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTRXDifferences::OnInitDialog()
{
    CTRXDialogBase::OnInitDialog();

    m_bInitList     = TRUE;

    //
    //
    if ( m_pListCtrl == NULL )
    {
        m_pListCtrl = &m_ListCtrl;

        m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

        m_ListCtrl.InsertColumn( COL_ADDRESS, "Address", LVCFMT_LEFT, 96 );
        m_ListCtrl.InsertColumn( COL_OLD, "Old Hexa", LVCFMT_LEFT, 64);
        m_ListCtrl.InsertColumn( COL_NEW, "New Hexa", LVCFMT_LEFT, 64);
        m_ListCtrl.InsertColumn( COL_DEC_OLD, "Old", LVCFMT_RIGHT, 48);
        m_ListCtrl.InsertColumn( COL_DEC_NEW, "New", LVCFMT_RIGHT, 48);
        m_ListCtrl.InsertColumn( COL_LABEL, "Label", LVCFMT_LEFT, 280);
    }

    // Add Handler Here
    if ( m_bRemastered  && CTR9SaveGame::I() != NULL )
    {
        CTRXTools::MemoryCompare ( CTR9SaveGame::I()->getBufferBackupAddress(), CTR9SaveGame::I()->getBufferAddress(), CTR9SaveGame::I()->getBufferLength() );
        const char *pText = CTRXTools::MemoryDifferences();
        if ( pText )
        {
            SetWindowText ( "Tombraider Remastered 123 Differences" );
            AddToListCtrl ( pText );
        }
    }
    else if ( ! m_bRemastered &&  CTRSaveGame::I() != NULL )
    {
        switch ( m_iVersion )
        {
            case 1:
            case 10:
            {
                CTR1SaveGame *pGame = (CTR1SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 1 Differences" );
                break;
            }
            case 15:
            {
                CTUBSaveGame *pGame = (CTUBSaveGame * )CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider Unfinished Business Differences" );
                break;
            }
            case 2:
            case 20:
            case 25:
            {
                CTR2SaveGame *pGame = (CTR2SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 2 and Gold Differences" );
                break;
            }
            case 3:
            case 30:
            case 35:
            {
                CTR3SaveGame *pGame = (CTR3SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 3 and Gold Differences" );
                break;
            }
            case 4:
            case 40:
            case 45:
            {
                CTR4SaveGame *pGame = (CTR4SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 4 Differences" );
                break;
            }
            case 49:
            {
                CTR4SaveGame *pGame = (CTR4SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 4 Next Generation Differences" );
                break;
            }
            case 5:
            case 50:
            {
                CTR5SaveGame *pGame = (CTR5SaveGame *) CTRSaveGame::I();
                CTRXTools::MemoryCompare ( pGame->getBufferBackupAddress(), pGame->getBufferAddress(), pGame->getBufferLength() );
                SetWindowText ( "Tombraider 5 Differences" );
                break;
            }
        }

        const char *pText = CTRXTools::MemoryDifferences();
        if ( pText )
        {
            AddToListCtrl ( pText );
        }
    }

    //
    if ( ( m_Buttons & 0x0f ) == MB_OKCANCEL )
    {
        m_Yes.ShowWindow ( SW_HIDE );
        m_No.ShowWindow ( SW_HIDE );
        m_OK.ShowWindow ( SW_NORMAL );
        m_Cancel.ShowWindow ( SW_NORMAL );
        m_Message.ShowWindow ( SW_HIDE );
        m_OK.SetFocus();
    }
    else if ( ( m_Buttons & 0x0f ) == MB_YESNO )
    {
        if ( ( m_Buttons & 0xf0 ) == MB_ICONQUESTION )
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_QUESTION);
            m_Picture.SetIcon(hIconQuestion);
        }

        m_Yes.ShowWindow ( SW_NORMAL );
        m_No.ShowWindow ( SW_NORMAL );
        m_Cancel.ShowWindow ( SW_HIDE );
        m_OK.ShowWindow ( SW_HIDE );
        m_Message.ShowWindow ( SW_NORMAL );
        m_Yes.SetFocus();

    }
    else if ( ( m_Buttons & 0x0f ) == MB_YESNOCANCEL )
    {
        if ( ( m_Buttons & 0xf0 ) == MB_ICONQUESTION )
        {
            const HICON hIconQuestion = AfxGetApp()->LoadStandardIcon(IDI_QUESTION);
            m_Picture.SetIcon(hIconQuestion);
        }

        m_Yes.ShowWindow ( SW_NORMAL );
        m_No.ShowWindow ( SW_NORMAL );
        m_Cancel.ShowWindow ( SW_NORMAL );
        m_OK.ShowWindow ( SW_HIDE );
        m_Message.ShowWindow ( SW_NORMAL );
        m_Yes.SetFocus();
    }
    else
    {
        m_Yes.ShowWindow ( SW_HIDE );
        m_No.ShowWindow ( SW_HIDE );
        m_OK.ShowWindow ( SW_NORMAL );
        m_Cancel.ShowWindow ( SW_NORMAL );
        m_Message.ShowWindow ( SW_HIDE );
        m_OK.SetFocus();
    }

    if ( strlen(m_szMessage) > 0 )
    {
        m_Message.SetWindowText ( m_szMessage );
    }

    if ( strlen(m_szTitle) > 0 )
    {
        SetWindowText ( m_szTitle );
    }

    //
    m_bInitList     = FALSE;
    m_bInitDone     = true;

    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::AddToListCtrl(const char *pText )
{
    //
    char    szLine [ 128 ];
    char    szDecimal [ 64 ];

    //
    m_ListCtrl.DeleteAllItems();

    if ( pText == NULL )
    {
        return;
    }

    int iLine       = 0;
    while ( *pText != '\0' )
    {
        ZeroMemory ( szLine, sizeof(szLine) );
        ZeroMemory ( szDecimal, sizeof(szDecimal) );
        int index = 0;
        while ( *pText != '\0' && *pText != '\r' && *pText != '\n' )
        {
            szLine [ index ] = *pText;
            index++;
            pText++;
        }

        if ( strlen(szLine) > 0 )
        {
            char *pOld = NULL;
            char *pNew = NULL;
            pOld = strchr(szLine,';');
            if ( pOld != NULL )
            {
                *pOld = '\0';
                pOld++;
                pNew = strchr(pOld,';');
                if ( pNew != NULL )
                {
                    *pNew = '\0';
                    pNew++;
                }
            }

            //
            unsigned uAddress = 0;
            m_ListCtrl.InsertItem ( iLine, szLine );
            sscanf_s ( szLine, "%x", &uAddress );
            if ( pOld != NULL )
            {
                unsigned uOld;
                sscanf_s ( pOld, "%x", &uOld );
                m_ListCtrl.SetItemText ( iLine, COL_OLD, pOld );
                sprintf_s ( szDecimal, sizeof(szDecimal), "%u", uOld );
                m_ListCtrl.SetItemText ( iLine, COL_DEC_OLD, szDecimal );
                if ( pNew != NULL )
                {
                    m_ListCtrl.SetItemText ( iLine, COL_NEW, pNew );

                    unsigned uNew;
                    sscanf_s ( pNew, "%x", &uNew );
                    sprintf_s ( szDecimal, sizeof(szDecimal), "%u", uNew );
                    m_ListCtrl.SetItemText ( iLine, COL_DEC_NEW, szDecimal );
                }
            }

            m_ListCtrl.SetItemText ( iLine, COL_LABEL, GetLabel( uAddress) );

            m_ListCtrl.SetCheck ( iLine, TRUE );

            iLine++;
        }

        while ( *pText == '\r' || *pText == '\n' )
        {
            pText++;
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnOK()
{
    // Add Handler Here
    ApplyChanges();

    if ( m_Result == -1 )
    {
        m_Result    = IDOK;
    }

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnCancel()
{
    // Add Handler Here
    if ( m_Result == -1 )
    {
        m_Result    = IDCANCEL;
    }

    CTRXDialogBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
INT_PTR CTRXDifferences::DoModal()
{
    // Add Handler Here

    INT_PTR result = CTRXDialogBase::DoModal();
    if ( m_Result != -1 )
    {
        return m_Result;
    }
    return result;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnBnClickedOk()
{
    // Add Handler Here
    ApplyChanges();

    if ( m_Result == -1 )
    {
        m_Result    = IDOK;
    }

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnBnClickedCancel()
{
    // Add Handler Here
    if ( m_Result == -1 )
    {
        m_Result    = IDCANCEL;
    }
    CTRXDialogBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnBnClickedYes()
{
    ApplyChanges();

    m_Result = IDYES;

    CTRXDialogBase::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnBnClickedNo()
{
    m_Result = IDNO;
    CTRXDialogBase::OnCancel();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::OnItemchangedDifferences(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // Add Handler Here
    if ( ! m_bInitList )
    {
    }

    *pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CTRXDifferences::ApplyChanges()
{
    for ( int iLine = 0; iLine < m_ListCtrl.GetItemCount(); iLine++ )
    {
        if ( ! m_ListCtrl.GetCheck ( iLine ) )
        {
            char szAddress [ 32 ];
            char szOld [ 32 ];
            char szNew [ 32 ];

            unsigned uAddress   = NULL;
            unsigned uOld       = NULL;
            unsigned uNew       = NULL;

            ZeroMemory ( szAddress, sizeof(szAddress) );
            ZeroMemory ( szOld, sizeof(szOld) );
            ZeroMemory ( szNew, sizeof(szNew) );

            m_ListCtrl.GetItemText ( iLine, COL_ADDRESS, szAddress, sizeof(szAddress) );
            m_ListCtrl.GetItemText ( iLine, COL_OLD, szOld, sizeof(szOld) );
            m_ListCtrl.GetItemText ( iLine, COL_NEW, szNew, sizeof(szNew) );

            sscanf_s ( szAddress, "%x", & uAddress );
            sscanf_s ( szOld, "%x", & uOld );
            sscanf_s ( szNew, "%x", & uNew );

            BYTE *pBuffer       = NULL;
            BYTE *pBufferBackup = NULL;
            size_t length       = 0;

            if ( m_bRemastered  && CTR9SaveGame::I() != NULL )
            {
                pBufferBackup   = (BYTE*) CTR9SaveGame::I()->getBufferBackupAddress();
                pBuffer         = (BYTE*) CTR9SaveGame::I()->getBufferAddress();
                length          = CTR9SaveGame::I()->getBufferLength();
            }
            else if ( ! m_bRemastered &&  CTRSaveGame::I() != NULL )
            {
                switch ( m_iVersion )
                {
                    case 1:
                    case 10:
                    {
                        CTR1SaveGame *pGame = (CTR1SaveGame *) CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                    case 15:
                    {
                        CTUBSaveGame *pGame = (CTUBSaveGame * )CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                    case 2:
                    case 20:
                    case 25:
                    {
                        CTR2SaveGame *pGame = (CTR2SaveGame *) CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                    case 3:
                    case 30:
                    case 35:
                    {
                        CTR3SaveGame *pGame = (CTR3SaveGame *) CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                    case 4:
                    case 40:
                    case 45:
                    {
                        CTR4SaveGame *pGame = (CTR4SaveGame *) CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                    case 49:
                    {
                        CTR4NGSaveGame *pGame = (CTR4NGSaveGame *) CTRSaveGame::I();
                        pBufferBackup           = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer                 = (BYTE*) pGame->getBufferAddress();
                        length                  = pGame->getBufferLength();
                        break;
                    }
                    case 5:
                    case 50:
                    {
                        CTR5SaveGame *pGame = (CTR5SaveGame *) CTRSaveGame::I();
                        pBufferBackup       = (BYTE*) pGame->getBufferBackupAddress();
                        pBuffer             = (BYTE*) pGame->getBufferAddress();
                        length              = pGame->getBufferLength();
                        break;
                    }
                }
            }

            //
            if ( pBuffer != NULL && pBufferBackup != NULL && length > 0 && uAddress >= 0 && uAddress < length )
            {
                pBuffer [ uAddress ] = pBufferBackup [ uAddress ];
            }
        }
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  Compare offset >= offsetInStruct && offset <= offsetInStruct + length
/////////////////////////////////////////////////////////////////////////////
static BOOL MatchNBytesOffset ( unsigned offsetInStruct, unsigned offset, unsigned length )
{
    if ( offset >= offsetInStruct && offset <= offsetInStruct + length )
    {
        return TRUE;
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL Match1ByteOffset ( unsigned offsetInStruct, unsigned offset )
{
    return MatchNBytesOffset ( offsetInStruct, offset, sizeof(BYTE) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL Match2BytesOffset ( unsigned offsetInStruct, unsigned offset )
{
    return MatchNBytesOffset ( offsetInStruct, offset, sizeof(WORD) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static BOOL Match4BytesOffset ( unsigned offsetInStruct, unsigned offset )
{
    return MatchNBytesOffset ( offsetInStruct, offset, sizeof(DWORD) );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
const char *CTRXDifferences::GetLabel ( unsigned offset )
{
    static char szLabel [ MAX_PATH ];

    ZeroMemory ( szLabel, sizeof(szLabel) );

    if ( m_bRemastered  && CTR9SaveGame::I() != NULL )
    {
        int tombraider      = -1;
        int block           = -1;
        int slot            = -1;
        bool bFound         = false;

        if ( offset >= TR1_BLOCK_START && offset < TR2_BLOCK_START )
        {
            tombraider = 1;
            strcpy_s ( szLabel, sizeof(szLabel), "Tombraider I" );
            block = ( offset - TR1_BLOCK_START ) / TR1_BLOCK_LENGTH;
            sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Block %d", block );
            unsigned blockStart = TR1_BLOCK_START + TR1_BLOCK_OFFSET + block * TR1_BLOCK_LENGTH;
            unsigned slotStart  = blockStart;
            unsigned slotEnd    = slotStart + NB_OF_SLOTS * TR1_SLOT_LENGTH;
            if ( offset >= slotStart && offset < slotEnd  )
            {
                slot                    = ( offset - slotStart ) / TR1_SLOT_LENGTH;
                unsigned slotCurrent    = slotStart + slot * TR1_SLOT_LENGTH;
                unsigned offsetInSlot   = offset - slotCurrent;
                sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Slot %d", slot );
                if ( Match2BytesOffset ( offsetof(TABLE_TR1,guns), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Marker" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR1,magnum), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Magnum" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR1,uzi), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Uzi" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR1,riotgun), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Riot" );
                    bFound = true;
                }
                else if (  Match1ByteOffset ( offsetof(TABLE_TR1,gunmask), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table GunMask" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR1,smallMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Small Medipak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR1,largeMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Large MediPak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR1,scion) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Scion" );
                    bFound = true;
                }
                else
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table" );
                    bFound = true;
                }
            }

            //
            if ( ! bFound )
            {
                TABLE_TR1_STATS *pStat = (TABLE_TR1_STATS *) CTR9SaveGame::I()->GetSlotsEnd ( tombraider, block, true );
                if ( pStat != NULL )
                {
                    unsigned statOffset =  CTR9SaveGame::RelativeAddress ( pStat );
                    if ( offset >= statOffset && offset < statOffset + sizeof(TABLE_TR1_STATS) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Stats" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                TR9_POSITION *position = CTR9SaveGame::I()->GetPositionAddress(tombraider, block );
                if ( position != NULL )
                {
                    unsigned positionOffset =  CTR9SaveGame::RelativeAddress ( position );
                    if ( offset >= positionOffset && offset < positionOffset + sizeof(TR9_POSITION) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Position" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                //
                for ( slot = 0; slot < NB_OF_SLOTS; slot++ )
                {
                    BYTE *pGun = (BYTE *)CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot );
                    if ( pGun != NULL )
                    {
                        unsigned gunOffset  = CTR9SaveGame::I()->RelativeAddress ( pGun );
                        if ( offset >= gunOffset && offset < gunOffset + sizeof(GUN_TR1) )
                        {
                            unsigned offsetInGuns   = offset - gunOffset;
                            if ( Match2BytesOffset ( offsetof(GUN_TR1,guns), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Marker" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR1,magnum), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Magnum" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR1,uzi), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Uzi" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR1,riotgun), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Riot" );
                                bFound = true;
                                break;
                            }
                            else
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Ammos Structure" );
                                bFound = true;
                                break;
                            }
                        }
                        else if ( MatchNBytesOffset ( gunOffset + TR1_OFFSET_SEC_GUN, offset, LEN_TR1_SECRETS ) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Secrets Table" );
                            bFound = true;
                            break;
                        }
                    }
                }
            }
        }
        else if ( offset >= TR2_BLOCK_START && offset < TR3_BLOCK_START )
        {
            tombraider = 2;
            strcpy_s ( szLabel, sizeof(szLabel), "Tombraider II" );
            block = ( offset - TR2_BLOCK_START ) / TR2_BLOCK_LENGTH;
            sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Block %d", block );
            unsigned blockStart = TR2_BLOCK_START + TR2_BLOCK_OFFSET + block * TR2_BLOCK_LENGTH;
            unsigned slotStart  = blockStart;
            unsigned slotEnd    = slotStart + NB_OF_SLOTS * TR2_SLOT_LENGTH;
            if ( offset >= slotStart && offset < slotEnd  )
            {
                slot                    = ( offset - slotStart ) / TR2_SLOT_LENGTH;
                unsigned slotCurrent    = slotStart + slot * TR2_SLOT_LENGTH;
                unsigned offsetInSlot   = offset - slotCurrent;
                sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Slot %d", slot );
                if ( Match2BytesOffset ( offsetof(TABLE_TR2,guns), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Marker" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,magnum), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Magnum" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,uzi), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Uzi" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,riotgun), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Riot" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,m16), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table M16" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,harpoon), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Harpoon" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR2,grenade), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Grenade" );
                    bFound = true;
                }
                else if (  Match1ByteOffset ( offsetof(TABLE_TR2,gunmask), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - GunMask" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR2,smallMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Small Medipak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR2,largeMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Large MediPak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR2,flare) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Flare" );
                    bFound = true;
                }
                else
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table" );
                    bFound = true;
                }
            }

            //
            if ( ! bFound )
            {
                TABLE_TR2_STATS *pStat = (TABLE_TR2_STATS *) CTR9SaveGame::I()->GetSlotsEnd ( tombraider, block, true );
                if ( pStat != NULL )
                {
                    unsigned statOffset =  CTR9SaveGame::RelativeAddress ( pStat );
                    if ( offset >= statOffset && offset < statOffset + sizeof(TABLE_TR1_STATS) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Stats" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                TR9_POSITION *position = CTR9SaveGame::I()->GetPositionAddress(tombraider, block );
                if ( position != NULL )
                {
                    unsigned positionOffset =  CTR9SaveGame::RelativeAddress ( position );
                    if ( offset >= positionOffset && offset < positionOffset + sizeof(TR9_POSITION) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Position" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                //
                for ( slot = 0; slot < NB_OF_SLOTS; slot++ )
                {
                    BYTE *pGun = (BYTE *)CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot );
                    if ( pGun != NULL )
                    {
                        unsigned gunOffset  = CTR9SaveGame::I()->RelativeAddress ( pGun );
                        if ( offset >= gunOffset && offset < gunOffset + sizeof(GUN_TR2) )
                        {
                            unsigned offsetInGuns   = offset - gunOffset;
                            if ( Match2BytesOffset ( offsetof(GUN_TR2,guns), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Marker" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,magnum), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Magnum" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,uzi), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Uzi" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,riotgun), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Riot" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,m16), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - M16" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,harpoon), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Harpoon" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR2,grenade), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Grenade" );
                                bFound = true;
                                break;
                            }
                            else
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Ammos Structure" );
                                bFound = true;
                                break;
                            }
                        }
                        else if ( MatchNBytesOffset ( gunOffset + TR2_OFFSET_SEC_GUN, offset, LEN_TR2_SECRETS  ) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Secrets Table" );
                            bFound = true;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            tombraider = 3;
            strcpy_s ( szLabel, sizeof(szLabel), "Tombraider III" );
            block = ( offset - TR3_BLOCK_START ) / TR3_BLOCK_LENGTH;
            sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Block %d", block );
            unsigned blockStart = TR3_BLOCK_START + TR3_BLOCK_OFFSET + block * TR3_BLOCK_LENGTH;
            unsigned slotStart  = blockStart;
            unsigned slotEnd    = slotStart + NB_OF_SLOTS * TR3_SLOT_LENGTH;
            if ( offset >= slotStart && offset < slotEnd  )
            {
                slot                    = ( offset - slotStart ) / TR3_SLOT_LENGTH;
                unsigned slotCurrent    = slotStart + slot * TR3_SLOT_LENGTH;
                unsigned offsetInSlot   = offset - slotCurrent;
                sprintf_s ( szLabel + strlen(szLabel), sizeof(szLabel) - strlen(szLabel), " - Slot %d", slot );
                if ( Match2BytesOffset ( offsetof(TABLE_TR3,guns), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Marker" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,magnum), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Magnum" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,uzi), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Uzi" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,riotgun), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Riot" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,m16), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table M16" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,rocket), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Rocket" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,harpoon), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Harpoon" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,grenade), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table Grenade" );
                    bFound = true;
                }
                else if (  Match2BytesOffset ( offsetof(TABLE_TR3,gunmask), offsetInSlot ) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - GunMask" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR3,smallMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Small Medipak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR3,largeMedipak) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Large MediPak" );
                    bFound = true;
                }
                else if ( offsetof(TABLE_TR3,flare) == offsetInSlot )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Flare" );
                    bFound = true;
                }
                else
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Table" );
                    bFound = true;
                }
            }

            //
            if ( ! bFound )
            {
                TABLE_TR3_STATS *pStat = (TABLE_TR3_STATS *) CTR9SaveGame::I()->GetSlotsEnd ( tombraider, block, true );
                if ( pStat != NULL )
                {
                    unsigned statOffset =  CTR9SaveGame::RelativeAddress ( pStat );
                    if ( offset >= statOffset && offset < statOffset + sizeof(TABLE_TR3_STATS) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Stats" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                TR9_POSITION *position = CTR9SaveGame::I()->GetPositionAddress(tombraider, block );
                if ( position != NULL )
                {
                    unsigned positionOffset =  CTR9SaveGame::RelativeAddress ( position );
                    if ( offset >= positionOffset && offset < positionOffset + sizeof(TR9_POSITION) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Position" );
                        bFound = true;
                    }
                }
            }

            //
            if ( ! bFound )
            {
                //
                for ( slot = 0; slot < NB_OF_SLOTS; slot++ )
                {
                    BYTE *pGun = (BYTE *)CTR9SaveGame::I()->GetGunAddress( tombraider, block, slot );
                    if ( pGun != NULL )
                    {
                        unsigned gunOffset  = CTR9SaveGame::I()->RelativeAddress ( pGun );
                        if ( offset >= gunOffset && offset < gunOffset + sizeof(GUN_TR3) )
                        {
                            unsigned offsetInGuns   = offset - gunOffset;
                            if ( Match2BytesOffset ( offsetof(GUN_TR3,guns), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Marker" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,magnum), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Magnum" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,uzi), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Uzi" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,riotgun), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Riot" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,m16), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - M16" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,rocket), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Rocket" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,harpoon), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Harpoon" );
                                bFound = true;
                                break;
                            }
                            else if (  Match2BytesOffset ( offsetof(GUN_TR3,grenade), offsetInGuns ) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Grenade" );
                                bFound = true;
                                break;
                            }
                            else
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Ammos Structure" );
                                bFound = true;
                                break;
                            }
                        }
                        else if ( MatchNBytesOffset ( gunOffset + TR3_OFFSET_SEC_GUN, offset, LEN_TR3_SECRETS  ) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Secrets Table" );
                            bFound = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    else if ( ! m_bRemastered &&  CTRSaveGame::I() != NULL )
    {
        bool bFound         = false;

        switch ( m_iVersion )
        {
            //
            case 1:
            case 10:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider I" );

                //
                if ( ! bFound )
                {
                    unsigned offsetTable    = offsetof(TR1SAVE, trTable);
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TR10STRUCT) * TR1MAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //
                if ( ! bFound )
                {
                    unsigned offsetSingle   = offsetof(TR1SAVE, trSingle);
                    if ( offset >= offsetSingle && offset < offsetSingle + sizeof(TR10STRUCT) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  Until cObjectK4
                if ( ! bFound )
                {
                    unsigned offsetFirstObject = offsetof(TR1SAVE, cObject_P1);
                    unsigned offsetLastObject = offsetof(TR1SAVE, cObjectK4);
                    if ( offset >= offsetFirstObject && offset < offsetLastObject )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects" );
                        bFound = true;
                    }
                }

                break;
            }

            //
            case 15:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider Unfinished Business" );

                //
                if ( ! bFound )
                {
                    unsigned offsetTable    = offsetof(TUBSAVE, trTable);
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TUBSTRUCT) * TUBMAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //
                if ( ! bFound )
                {
                    unsigned offsetSingle   = offsetof(TUBSAVE, trSingle);
                    if ( offset >= offsetSingle && offset < offsetSingle + sizeof(TUBSTRUCT) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  Until cObjectK4
                if ( ! bFound )
                {
                    unsigned offsetFirstObject = offsetof(TUBSAVE, cObject_P1);
                    unsigned offsetLastObject = offsetof(TUBSAVE, cObjectK4);
                    if ( offset >= offsetFirstObject && offset < offsetLastObject )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects" );
                        bFound = true;
                    }
                }
                break;
            }

            //
            case 2:
            case 20:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider II" );

                //
                unsigned offsetTable    = offsetof(TR2SAVE, trTable);
                if ( ! bFound )
                {
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TR20STRUCT) * TR2MAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  cSecrets
                if ( ! bFound )
                {
                    unsigned offsetSecret = offsetof(TR2SAVE, cSecrets);
                    if ( offset >= offsetSecret && offset < offsetSecret + sizeof(BYTE) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Secrets" );
                        bFound = true;
                    }
                }

                //  Until cObjectK4
                if ( ! bFound )
                {
                    unsigned offsetFirstObject = offsetof(TR2SAVE, cObject_P1);
                    unsigned offsetLastObject = offsetof(TR2SAVE, cObjectK4);
                    if ( offset >= offsetFirstObject && offset < offsetLastObject )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects" );
                        bFound = true;
                    }
                }
                break;
            }

            //
            case 25:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider II Gold" );
                //
                unsigned offsetTable    = offsetof(TR2SAVE, trTable);
                if ( ! bFound )
                {
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TR20STRUCT) * TR2MAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  cSecrets
                if ( ! bFound )
                {
                    unsigned offsetSecret = offsetof(TR2SAVE, cSecrets);
                    if ( offset >= offsetSecret && offset < offsetSecret + sizeof(BYTE) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Secrets" );
                        bFound = true;
                    }
                }

                //  Until cObjectK4
                if ( ! bFound )
                {
                    unsigned offsetFirstObject = offsetof(TR2SAVE, cObject_P1);
                    unsigned offsetLastObject = offsetof(TR2SAVE, cObjectK4);
                    if ( offset >= offsetFirstObject && offset < offsetLastObject )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects" );
                        bFound = true;
                    }
                }
                break;
            }

            //
            case 3:
            case 30:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider III" );

                //
                unsigned offsetTable    = offsetof(TR3SAVE, trTable);
                if ( ! bFound )
                {
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TR30STRUCT) * TR3MAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  Permanent
                if ( ! bFound )
                {
                    unsigned offsetPermanent = offsetof(TR3SAVE, trSingle);
                    if ( offset >= offsetPermanent && offset < offsetPermanent + sizeof(TR3Permanent) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects / Secrets" );
                        bFound = true;
                    }
                }

                break;
            }

            //
            case 35:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider III Gold" );

                //
                unsigned offsetTable    = offsetof(TR3SAVE, trTable);
                if ( ! bFound )
                {
                    if ( offset >= offsetTable && offset < offsetTable + sizeof(TR30STRUCT) * TR3MAXLEVEL )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Table Ammos / Guns / Medipak" );
                        bFound = true;
                    }
                }

                //  Permanent
                if ( ! bFound )
                {
                    unsigned offsetPermanent = offsetof(TR3SAVE, trSingle);
                    if ( offset >= offsetPermanent && offset < offsetPermanent + sizeof(TR3Permanent) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Objects / Secrets" );
                        bFound = true;
                    }
                }

                break;
            }

            //
            case 4:
            case 40:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider IV" );
                //
                CTR4SaveGame *pGame = dynamic_cast<CTR4SaveGame *>(CTRSaveGame::I() );
                if ( pGame != NULL )
                {
                    const TR4_POSITION *position = pGame->GetTR4Position( );
                    if ( position != NULL )
                    {
                        unsigned positionOffset =  CTRSaveGame::RelativeAddress ( position );
                        if ( offset >= positionOffset && offset < positionOffset + sizeof(TR4_POSITION) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Position" );

                            if (    offset >= positionOffset + offsetof(TR4_POSITION,cOrientation) &&
                                    offset < positionOffset + offsetof(TR4_POSITION,cOrientation) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Orientation" );
                            }

                            if (    offset >= positionOffset + offsetof(TR4_POSITION,cRoom) &&
                                    offset < positionOffset + offsetof(TR4_POSITION,cRoom) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Room" );
                            }

                            if (    offset < positionOffset + offsetof(TR4_POSITION,wSouthToNorth) + sizeof(WORD) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Coordinates" );
                            }

                            bFound = true;
                        }
                    }
                }

                break;
            }

            //
            case 45:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider IV Times" );
                //
                CTR4SaveGame *pGame = dynamic_cast<CTR4SaveGame *>(CTRSaveGame::I() );
                if ( pGame != NULL )
                {
                    const TR4_POSITION *position = pGame->GetTR4Position( );
                    if ( position != NULL )
                    {
                        unsigned positionOffset =  CTRSaveGame::RelativeAddress ( position );
                        if ( offset >= positionOffset && offset < positionOffset + sizeof(TR4_POSITION) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Position" );

                            if (    offset >= positionOffset + offsetof(TR4_POSITION,cOrientation) &&
                                    offset < positionOffset + offsetof(TR4_POSITION,cOrientation) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Orientation" );
                            }

                            if (    offset >= positionOffset + offsetof(TR4_POSITION,cRoom) &&
                                    offset < positionOffset + offsetof(TR4_POSITION,cRoom) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Room" );
                            }

                            if (    offset < positionOffset + offsetof(TR4_POSITION,wSouthToNorth) + sizeof(WORD) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Coordinates" );
                            }

                            bFound = true;
                        }
                    }
                }
                break;
            }

            //
            case 5:
            case 50:
            {
                strcpy_s ( szLabel, sizeof(szLabel), "Tombraider V" );

                //
                CTR5SaveGame *pGame = dynamic_cast<CTR5SaveGame *>(CTRSaveGame::I() );
                if ( pGame != NULL )
                {
                    const TR5_POSITION *position = pGame->GetTR5Position( );
                    if ( position != NULL )
                    {
                        unsigned positionOffset =  CTRSaveGame::RelativeAddress ( position );
                        if ( offset >= positionOffset && offset < positionOffset + sizeof(TR5_POSITION) )
                        {
                            strcat_s ( szLabel, sizeof(szLabel), " - Position" );

                            if (    offset >= positionOffset + offsetof(TR5_POSITION,cOrientation) &&
                                    offset < positionOffset + offsetof(TR5_POSITION,cOrientation) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Orientation" );
                            }

                            if (    offset >= positionOffset + offsetof(TR5_POSITION,cRoom) &&
                                    offset < positionOffset + offsetof(TR5_POSITION,cRoom) + sizeof(BYTE) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Room" );
                            }

                            if (    offset < positionOffset + offsetof(TR5_POSITION,wSouthToNorth) + sizeof(WORD) )
                            {
                                strcat_s ( szLabel, sizeof(szLabel), " - Coordinates" );
                            }

                            bFound = true;
                        }
                    }
                }
                break;
            }
        }

        //
        if ( ! bFound )
        {
            const TR_POSITION *position = CTRSaveGame::I()->GetPosition( );
            if ( position != NULL )
            {
                unsigned positionOffset =  CTRSaveGame::RelativeAddress ( position );
                if ( offset >= positionOffset && offset < positionOffset + sizeof(TR_POSITION) )
                {
                    strcat_s ( szLabel, sizeof(szLabel), " - Position" );

                    if (    offset >= positionOffset + offsetof(TR_POSITION,wOrientation) &&
                            offset < positionOffset + offsetof(TR_POSITION,wOrientation) + sizeof(WORD) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Orientation" );
                    }

                    if (    offset >= positionOffset + offsetof(TR_POSITION,wRoom) &&
                            offset < positionOffset + offsetof(TR_POSITION,wRoom) + sizeof(WORD) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Room" );
                    }

                    if (    offset < positionOffset + offsetof(TR_POSITION,dwSouthToNorth) + sizeof(DWORD) )
                    {
                        strcat_s ( szLabel, sizeof(szLabel), " - Coordinates" );
                    }

                    bFound = true;
                }
            }
        }
    }

    return szLabel;
}
