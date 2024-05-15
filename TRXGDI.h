#pragma once

#include <Windows.h>
#include "afxwin.h"

class CTRXPen : public CPen
{
	public :
		virtual ~CTRXPen()
		{
			if ( this->m_hObject != NULL )
			{
				DeleteObject ();
			}
		}
};

class CTRXBrush : public CBrush
{
	public :
		virtual ~CTRXBrush()
		{
			if ( this->m_hObject != NULL )
			{
				DeleteObject ();
			}
		}
};

class CTRXBitmap: public CBitmap
{
	public :
		virtual ~CTRXBitmap()
		{
			if ( this->m_hObject != NULL )
			{
				DeleteObject ();
			}
		}
};

