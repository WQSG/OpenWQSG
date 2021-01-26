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
#ifndef __WQSG_CFG_H__
#define __WQSG_CFG_H__

#define	DEF_ON_TXTIO		1
#define	DEF_ON_PTXTIO		0
#define	DEF_ON_WIPS			1
#define	DEF_ON_TBL			1
#define DEF_ON_文本替换		1
#define DEF_ON_差值搜索		1

struct SImportData
{
	CString     m_strItemName;

	CString		m_strROMPath;
	CString		m_strTXTPath;
	CString		m_strTBLPathName;
	CString		m_strTBL2PathName;
	CString		m_strExtName;

	BOOL		m_bUseDirectory;
	BOOL		m_bCheckTblOverlap;
	BOOL		m_bUseTBL2;
	BOOL		m_bTxtDirDefault;
	BOOL		m_bSubDir;
	BOOL		m_bLenOverStop;
	u32			m_uFill;
	CString		m_strFillByte;
	CString		m_strFillWord;

	SImportData()
		: m_strItemName()
		, m_strROMPath() , m_strTXTPath()
		, m_strTBLPathName() , m_strTBL2PathName()
		, m_strExtName()
		, m_bUseDirectory(FALSE)
		, m_bCheckTblOverlap(TRUE)
		, m_bUseTBL2(FALSE)
		, m_bTxtDirDefault(FALSE)
		, m_bSubDir(FALSE)
		, m_bLenOverStop(FALSE)
		, m_uFill(0)
		, m_strFillByte(L"20")
		, m_strFillWord(L"8140")
	{
	}
};

struct SExportData
{
	CString     m_strItemName;

	CString		m_strROMPath;
	CString		m_strTXTPath;
	CString		m_strTBLPathName;
	CString		m_strTBL2PathName;
	CString		m_strExtName;
	u32			m_uSegmentAddr;
	u32			m_uBeginOffset;
	u32			m_uEndOffset;
	u32			m_uMinLen;
	u32			m_uMaxLen;
	BOOL		m_bUseDirectory;
	BOOL		m_bCheckTblOverlap;
	BOOL		m_bUseTBL2;
	BOOL		m_bTxtDirDefault;
	BOOL		m_bSubDir;

	SExportData()
		: m_strItemName()
		, m_strROMPath() , m_strTXTPath()
		, m_strTBLPathName() , m_strTBL2PathName()
		, m_strExtName() , m_uSegmentAddr(0)
		, m_uBeginOffset(0) , m_uEndOffset(0)
		, m_uMinLen(0) , m_uMaxLen(0)
		, m_bUseDirectory(FALSE)
		, m_bCheckTblOverlap(TRUE)
		, m_bUseTBL2(FALSE)
		, m_bTxtDirDefault(FALSE)
		, m_bSubDir(FALSE)
	{
	}
};

class CGlobalData
{
public:
	static std::vector<SImportData> m_ImportDatas;
	static std::vector<SExportData> m_ExportDatas;

#if USE_XML
	static BOOL LoadXml();
	static BOOL SaveXml();
#endif
};

#endif
