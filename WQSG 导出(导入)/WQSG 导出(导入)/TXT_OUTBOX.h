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
#pragma once
#include "afxwin.h"
#include <vector>
// CTXT_OutBox 对话框
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

struct SExportDataEx
{
	HWND		m_hWnd;
	std::vector<CStringW> m_Files;

	//----------------
	CString		m_strROMPath;
	CString		m_strTXTPath;
	CString		m_strTBLPathName;
	CString		m_strTBL2PathName;
	CString		m_strExtName;
	u64			m_uBeginOffset;
	u64			m_uEndOffset;
	u32			m_uMinLen;
	u32			m_uMaxLen;
	BOOL		m_bCheckTblOverlap;
	BOOL		m_bSubDir;

	SExportDataEx()
	{
	}
};

class CTXT_OutBox : public CBaseDialog
{
	DECLARE_DYNAMIC(CTXT_OutBox)

	SExportDataEx m_Data;

	std::vector<SExportData> m_ExportDatas;

	CButton m_cUseDirectory;
	CString m_EDIT_ROMPath;
	CString m_EDIT_TXTPath;
	CString m_EDIT_TBLPathName;
	CString m_EDIT_TBL2PathName;
	CButton m_cCheckTblOverlap;
	CString m_EDIT_min;
	CString m_EDIT_max;
	CString m_EDIT_SegmentAddr;
	CString m_EDIT_BeginOffset;
	CString m_EDIT_EndOffset;
	CString m_EDIT_ExtName;
	CString m_LOG;
	CEdit m_CEDIT_LOG;
	CButton m_cUseTBL2;
	CListBox m_CList;
	CString m_NodeName;
	CButton m_C文本在同目录;
	CButton m_cSubDir;
public:
	CTXT_OutBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTXT_OutBox();

// 对话框数据
	enum { IDD = IDD_TXT_O };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void AppLog(CString str);
	void UpdateExportData( SExportData& a_data );
#if USE_XML
	void LoadXml( TiXmlElement& a_Root );
	void SaveXml();
#endif
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonTxtDir();
	afx_msg void OnBnClickedButtonTbl();
	afx_msg void OnBnClickedButtonTbl2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnChangeEditAddrH();
	afx_msg void OnEnChangeEditAddrKsL();
	afx_msg void OnEnChangeEditAddrJsL();
	afx_msg void OnEnKillfocusEditAddrH();
	afx_msg void OnEnKillfocusEditMin();
	afx_msg void OnEnKillfocusEditMax();
	afx_msg LRESULT 线程信息( WPARAM 保留 , LPARAM 文本 );
	afx_msg LRESULT 线程LOG( WPARAM 保留 , LPARAM 文本 );
	afx_msg void OnBnClickedButtonRom();
	afx_msg void OnEnChangeEditExt();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnEnKillfocusEditName();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedCheck3();
};
