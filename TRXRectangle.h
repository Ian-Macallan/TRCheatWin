#pragma once

#include <Windows.h>
#include <afx.h>
#include <afxwin.h>

//
/////////////////////////////////////////////////////////////////////////////
// CTRXRectangle
//
/////////////////////////////////////////////////////////////////////////////
class CTRXRectangle : public CStatic
{
        DECLARE_DYNAMIC(CTRXRectangle)

    public:
        CTRXRectangle();
        virtual ~CTRXRectangle();

    protected :
        char        m_szText [ 256 ];

    protected:
        DECLARE_MESSAGE_MAP()
    public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );
};


