/*  OpenWQSG - WQSG 导出(导入)
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
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <WQSG_LIB.h>
#include <WQSG_DirDlg.h>
#include <2/WQSG_afx.h>
#include <WQSG导出导入.h>

#include "WQSG_cfg.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


static BOOL WQSG_Is合法FileName( const WCHAR* FileName )
{
	if( NULL == FileName )		return FALSE;

	while( ( L' ' == *FileName ) || ( L'　' == *FileName ) )
		++FileName;

	if( L'\0' == *FileName )		return FALSE;

//	BOOL 末尾空白 = FALSE;
	while( *FileName )
	{
		switch( *FileName )
		{
		case L'/':
		case L'\\':
		case L':':
		case L'*':
		case L'?':
		case L'\"':
		case L'<':
		case L'>':
		case L'|':
			return FALSE;
		case L' ':
		case L'　':
//			末尾空白 = TRUE;
//			break;
		default:
//			末尾空白 = FALSE;
			break;
		}
		++FileName;
	}

	return TRUE;
}
inline void 删除首空( CString& str )
{
	const WCHAR* tmp = str.GetString();

	while( ( L' ' == *tmp ) || ( L'　' == *tmp ) )
		++tmp;

	str = tmp;
}
static BOOL 验证Ext( CString test )
{
	CString ext;

	WCHAR* tmp = test.GetBuffer();

	if( L'\0' == *tmp )return FALSE;

	INT pos;
	INT count = 0;
	while( *tmp )
	{
		pos = WQSG_strchr( tmp , L';' );
		if( pos >= 0 )
		{
			tmp[ pos++ ] = '\0';
			ext = tmp;
			tmp += pos;
		}
		else
		{
			ext = tmp;
			*tmp = L'\0';
		}

		if( ext.GetLength() )
		{
			if( /*( ext != L"*" ) && */( !::WQSG_Is合法FileName( ext.GetString() ) ))
			{
				return FALSE;
			}
			++count;
		}
	}
	return ( count > 0 );
}
static void 分解Ext( CString test , std::vector<CStringW>& szExt )
{
	WCHAR* tmp = test.GetBuffer();
	while( *tmp )
	{
		CString ext;
		INT pos = WQSG_strchr( tmp , L';' );
		if( pos >= 0 )
		{
			tmp[ pos++ ] = '\0';
			ext = tmp;
			tmp += pos;
		}
		else
		{
			ext = tmp;
			*tmp = L'\0';
		}

		删除首空( ext );

		if( ext.GetLength() )
			szExt.push_back( L'.' + ext.MakeUpper() );
	}
}
/*
struct tgWM_消息
{
	HWND m_hWnd;
	UINT m_message;
	void SendMSG( WPARAM wParam, LPARAM lParam )
	{
		::SendMessage( m_hWnd , m_message , wParam , lParam );
	}
}*/
#pragma warning( disable : 4482 )
#pragma warning( disable : 4996 )
//------------------------------------------------------------------------
#pragma once
#define WIN32_LEAD_AND_MEAN
#include <windows.h>

#undef  ___i___
#define ___i___
class HiResTimer
{
private:
    static  inline  bool&       s_bTimerInstalled       ( void ) { static bool bTimerInstalled; return bTimerInstalled; }
    static  inline  __int64&    s_i64Freq               ( void ) { static __int64 i64Freq; return i64Freq; }
private:
    ___i___         bool        m_bRunning;
    ___i___         __int64     m_i64Start;
    ___i___         __int64     m_i64Stop;
private:
    static  inline  void        Initialize              ( void );
    static  inline  bool        IsInitialized           ( void );
public:
    static  inline  bool        IsAvailable             ( void );
public:
    ___i___ inline              HiResTimer              ( void );
    ___i___ inline  void        Reset                   ( void );
    ___i___ inline  void        Start                   ( void );
    ___i___ inline  void        Stop                    ( void );
public:
    ___i___ inline  bool        getIsRunning            ( void );
public:
    ___i___ inline  float       getElapsedTicks         ( void );
    ___i___ inline  float       getElapsedMilliseconds  ( void );
    ___i___ inline  float       getElapsedSeconds       ( void );
    ___i___ inline  float       getElapsedMinutes       ( void );
    ___i___ inline  float       getElapsedHours         ( void );
    ___i___ inline  float       getElapsedDays          ( void );
};
inline  void    HiResTimer::Initialize              ( void )
{
    s_bTimerInstalled() = ( 0 != QueryPerformanceFrequency( (LARGE_INTEGER*) &s_i64Freq() ) );
}

inline  bool    HiResTimer::IsInitialized           ( void )
{
    return ( s_bTimerInstalled() && s_i64Freq() != 0 );
}

inline  bool    HiResTimer::IsAvailable             ( void )
{
    if ( !IsInitialized() ) Initialize();
    return IsInitialized();
}

inline          HiResTimer::HiResTimer              ( void ) : m_i64Start(0), m_i64Stop(0), m_bRunning(false)
{
    if ( !IsInitialized() ) Initialize();
}

inline  void    HiResTimer::Reset                   ( void )
{
    m_i64Start = m_i64Stop = 0;
    m_bRunning = false;
}

inline  void    HiResTimer::Start                   ( void )
{
    QueryPerformanceCounter( (LARGE_INTEGER*) &m_i64Start );
    m_bRunning = true;
}

inline  void    HiResTimer::Stop                    ( void )
{
    QueryPerformanceCounter( (LARGE_INTEGER*) &m_i64Stop );
    m_bRunning = false;
}

inline  bool    HiResTimer::getIsRunning            ( void )
{
    return m_bRunning;
}

inline  float   HiResTimer::getElapsedTicks         ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( i64elapsed - m_i64Start );
    }
    return elapsed;
}

inline  float   HiResTimer::getElapsedMilliseconds  ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( ( i64elapsed - m_i64Start ) * 1000 / (float) s_i64Freq() );
    }
    return elapsed;
}

inline  float   HiResTimer::getElapsedSeconds       ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( ( i64elapsed - m_i64Start ) / (float) s_i64Freq() );
    }
    return elapsed;
}

inline  float   HiResTimer::getElapsedMinutes       ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( ( i64elapsed - m_i64Start ) / (float) s_i64Freq() / 60.0f );
    }
    return elapsed;
}

inline  float   HiResTimer::getElapsedHours         ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( ( i64elapsed - m_i64Start ) / (float) s_i64Freq() / 360.0f );
    }
    return elapsed;
}

inline  float   HiResTimer::getElapsedDays          ( void )
{
    __int64 i64elapsed = 0;
    float elapsed = 0.0f;

    if ( IsInitialized() )
    {
        if ( m_bRunning )
        {
            QueryPerformanceCounter( (LARGE_INTEGER*) &i64elapsed );
        }
        else
        {
            i64elapsed = m_i64Stop;
        }
        elapsed = (float) ( ( i64elapsed - m_i64Start ) / (float) s_i64Freq() / 8640.0f );
    }
    return elapsed;
}
struct WQSG_tgElapsedTime
{
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};
inline	void	WQSG_Milliseconds2struct( const float _Milliseconds , WQSG_tgElapsedTime& _tgTime )
{
	memset( &_tgTime , 0 , sizeof( WQSG_tgElapsedTime ) );
	float milliSeconds = _Milliseconds;

	_tgTime.wDay = (WORD)( milliSeconds / 86400000.0f );
	milliSeconds -= ( _tgTime.wDay * 86400000.0f );

	_tgTime.wHour = (WORD)( milliSeconds / 3600000.0f );
	milliSeconds -= ( _tgTime.wHour * 3600000.0f );

	_tgTime.wMinute = (WORD)( milliSeconds / 60000.0f );
	milliSeconds -= ( _tgTime.wMinute * 60000.0f );

	_tgTime.wSecond = (WORD)( milliSeconds / 1000.0f );
	_tgTime.wMilliseconds = (WORD)(milliSeconds - ( _tgTime.wSecond * 1000.0f ));
}

#include "atlstr.h"
