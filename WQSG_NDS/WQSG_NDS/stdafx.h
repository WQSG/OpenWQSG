/*  OpenWQSG - WQSG_NDS
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
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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


struct SNdsHeader
{
	char GameTitle[0x0C];
	char GameCode[0x04];
	char MakerCode[0x02];
	BYTE UnitCode;
	BYTE DeviceCode;						// type of device in the game card
	BYTE DeviceCap;							// device capacity (128kb<<n Mbit)
	BYTE Reserved_0x015[0x09];				// (zero filled)
	BYTE RomVersion;
	BYTE Autostart;							// (Bit2: Skip "Press Button" after Health and Safety) (Also skips bootmenu, even in Manual mode & even Start pressed)
	UINT Arm9_Rom_Offset;					// copy src
	UINT Arm9_Entry_Address;				// entry point
	UINT Arm9_Ram_Address;					// copy dst
	UINT Arm9_Size;							// size
	UINT Arm7_Rom_Offset;
	UINT Arm7_Entry_Address;
	UINT Arm7_Ram_Address;
	UINT Arm7_Size;
	UINT Fnt_Offset;
	UINT Fnt_Size;
	UINT Fat_Offset;
	UINT Fat_Size;
	UINT Arm9_Overlay_Offset;
	UINT Arm9_Overlay_Size;
	UINT Arm7_Overlay_Offset;
	UINT Arm7_Overlay_Size;
	UINT Port_40001A4h_Normal_Commands;		// Port 40001A4h setting for normal commands (usually 00586000h)
	UINT Port_40001A4h_KEY1_Commands;		// Port 40001A4h setting for KEY1 commands   (usually 001808F8h)
	UINT Banner_Offset;
	WORD Secure_Area_CRC;
	WORD Secure_Area_Loading_Timeout;
	UINT ARM9_Auto_Load_List_RAM_Address;	// ?
	UINT ARM7_Auto_Load_List_RAM_Address;	// ?
	UINT Secure_Area_Disable1;				// unique ID for homebrew
	UINT Secure_Area_Disable2;				// unique ID for homebrew
	UINT Application_End_Offset;			// Total Used ROM size
	UINT Rom_Header_Size;
	BYTE Reserved_0x088[0x38];				// Reserved (zero filled)
	BYTE Nintendo_Logo[0x9C];
	WORD Nintendo_Logo_CRC;
	WORD Header_CRC;
	UINT Debug_Rom_Offset;					// (0=none) (8000h and up)       ;only if debug
	UINT Debug_Size;						// (0=none) (max 3BFE00h)        ;version with
	UINT Debug_Ram_Address;					// (0=none) (2400000h..27BFE00h) ;SIO and 8MB
	UINT Reserved_0x16C;
	BYTE Zero[0x90];
};


#define HEADERCOUNT 8

struct SNdsFileRec
{
	UINT top;
	UINT bottom;	// size = bottom-top
};

struct OVERLAYENTRY
{
	UINT id;
	UINT ram_address;
	UINT ram_size;
	UINT bss_size;
	UINT sinit_init;
	UINT sinit_init_end;
	UINT file_id;
	UINT reserved;
};

struct SNdsDirRec
{
	UINT entry_start;
	WORD top_file_id;
	WORD parent_id_or_count;
};

#include <WQSG_Lib.h>

enum ENdsFindHandleType
{
	E_NFHT_ROOT,
	E_NFHT_OVERLAY,
	E_NFHT_DATA,

	E_NFHT_MAX,
};

struct SNdsFindHandle;

struct SNdsFindData
{
	struct SDir
	{
		ENdsFindHandleType m_eType;
		u16 m_uDir_ID;//文件跟 fnt有关

	};

	struct SFile
	{
		u16 m_uFileID_InRom;//文件跟 fat有关
		u16 m_uFileID_InDir;//文件跟 fat有关
		s32 m_nFatOffset;

		u32 m_uSize;
		u32 m_uRomOffset;
	};

	bool m_bEmpty;
	char m_szName[128];
	bool m_bDir;

	s32 m_nFntOffset;

	union
	{
		SDir m_Dir;
		SFile m_File;
	};
	//u16 m_uParentDirID;
};


#define DEF_Overlay "overlay"

class CNdsRom
{
	CWQSG_File m_hRom;
	u32 m_uRomSize;
	CWQSG_PartitionList* m_pLinkBlock;

	SNdsHeader m_NdsHeader;

	__inline bool zzz_Alloc( const CStringA& a_strPath );
	__inline bool zzz_AllocSize( u32& a_uNewPos , const u32 a_uNewSize , const u32 a_uAlign , const u32 a_uOldPos , const u32 a_uOldSize );

	__inline bool zzz_UdateRec( const SNdsFindData& a_DirData , const SNdsFindData& a_FileData );
public:
	CStringW m_strError;
public:
	CNdsRom();

	virtual ~CNdsRom();

	bool Open( const CStringW& a_strFile );

	void Close();

	u32 GetRomSize()const
	{
		return m_uRomSize;
	}

	bool IsOpen()const
	{
		return m_hRom.IsOpen()?true:false;
	}

	bool IsCanWrite()const
	{
		return IsOpen() && m_hRom.IsCanWrite();
	}

	SNdsFindHandle* FindFile( const CStringA& a_strPath );
	void FindClose( SNdsFindHandle* a_pHandle );

	bool FindNextFile( SNdsFindData& a_Data , SNdsFindHandle* a_pHandle /*, bool a_bFindNext */);

	bool GetPath( SNdsFindData& a_Data , CStringA a_strPath );

	bool ImportFile( const SNdsFindData& a_DirData , CWQSG_xFile& a_InFile , const CStringA& a_strName );
protected:
	bool GetRec_Root( SNdsFindData& a_Data , s32 a_nFileId );

	bool GetRec_Overlay( SNdsFindData& a_Data , s32 a_nFileId );

	s32 GetRec_Data( SNdsFindData& a_Data , u16 a_uDirID , u16 a_uFileId , s32 a_nOffset );

	bool zzz_FindFile( SNdsFindData& a_FileData , const SNdsFindData& a_DirData , CStringA a_strName );
	s32 zzz_FindNextFile( SNdsFindData& a_FileData , const SNdsFindData& a_DirData , s32 a_nOffset , u16 a_uFileID );
};
