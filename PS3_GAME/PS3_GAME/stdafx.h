/*  OpenWQSG - PS3_GAME
*  Copyright (C) WQSG <wqsg@qq.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <atlstr.h>

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


#include <WQSG_Lib.h>
#include <commonLib/cximage.h>


#include <vector>

class CPs3GameInfo
{
	CString m_strDirName;
	CString m_strGameName;
	CxImage* m_pLogo;
	BOOL m_bHasBigFile;
public:
	CPs3GameInfo()
		: m_pLogo(NULL)
	{
	}

	~CPs3GameInfo()
	{
		if( m_pLogo )
		{
			delete m_pLogo;
			m_pLogo = NULL;
		}
	}

	BOOL LoadInfo( CString a_strPath , const CString& a_strDirName );

	const CString& GetDirName()const
	{
		return m_strDirName;
	}

	const CString& GetGameName()const
	{
		return m_strGameName;
	}

	BOOL IsHasBigFile()const
	{
		return m_bHasBigFile;
	}

	void Draw( HDC a_hdc, const RECT& a_rect )
	{
		if( !m_pLogo )
			return;

		m_pLogo->Stretch( a_hdc , a_rect );
	}
};
