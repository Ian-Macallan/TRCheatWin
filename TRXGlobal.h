#pragma once

#define	TR_NG_MIN_SIZE			33000
#define	TR_NG_MAX_SIZE			53000

//
/////////////////////////////////////////////////////////////////////////////
class CTRXGlobal
{
	private :
		CTRXGlobal(void);
		~CTRXGlobal(void);

	public :
		static BOOL			m_iUnchecked;
		static BOOL			m_iSearchExt;

		static unsigned		m_iMinNGSize;
		static unsigned		m_iMaxNGSize;

		static int			m_iDarkTheme;
};

