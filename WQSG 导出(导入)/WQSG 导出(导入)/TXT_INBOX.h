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


// CTXT_INBOX 对话框

class CTXT_INBOX : public CDialog
{
	DECLARE_DYNAMIC(CTXT_INBOX)
	struct tg参数
	{
		HWND		m_hWnd;
		CString		m_TBL;
		CString		m_TBL2;
		CString		m_TXT_DIR;
		CString		m_ROM_DIR;
		BOOL		m_单字节;
		u8			m_SP1;
		BOOL		m_双字节;
		u16			m_SP2;
		BOOL		m_验证;
		BOOL		m_长度不足提示;
		std::vector<CStringW> m_Files;
		CString		m_Ext;
		BOOL		m_SubDir;

		CStringW m_LastDir;
		tg参数()
		{
		}
	};
	tg参数 m_参数;

	CString m_文件列表缓存;

	CString m_EDIT_Rom;
	CString m_EDIT_TXT_DIR;
	CString m_EDIT_TBL;
	CString m_EDIT_TBL2;

public:
	CTXT_INBOX(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTXT_INBOX();

// 对话框数据
	enum { IDD = IDD_TXT_I };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	static DWORD WINAPI 普通导入_文件(LPVOID lpParameter);
	static BOOL zzz_普通导入_文件夹( CStringW 路径 , tg参数& 参数 , WQSG_TXT_I& WQSG , INT& countAll , std::vector<CStringW>& szExt );
	static DWORD WINAPI 普通导入_文件夹(LPVOID lpParameter);
	static BOOL zzz普通导入( CString& 文件名 , WQSG_TXT_I& WQSG , tg参数& 参数 );
	afx_msg LRESULT 线程信息( WPARAM 保留 , LPARAM 文本 );
	afx_msg LRESULT 线程LOG( WPARAM 保留 , LPARAM 文本 );
	CButton m_C验证TBL;
	CButton m_C从目录导出;
	CButton m_C长度不足中止;
	CComboBox m_C选择填充;
	CString m_EDIT_SP1;
	CString m_EDIT_SP2;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButtonRom();
	afx_msg void OnBnClickedButtonTxtDir();
	afx_msg void OnBnClickedButtonTbl();
	afx_msg void OnBnClickedButtonTbl2();
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnEnChangeEditSp1();
	afx_msg void OnEnChangeEditSp2();
	afx_msg void OnEnKillfocusEditSp1();
	afx_msg void OnEnKillfocusEditSp2();
private:
	CButton m_C使用控制码表;
public:
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck1();
private:
	CString m_EDIT_EXT;
public:
	afx_msg void OnEnChangeEditExt();
	afx_msg void OnBnClickedButton1();
private:
	CString m_LOG;
	CEdit m_CEDIT_LOG;
	void 载入配置(void);
	BOOL 写配置(void);
public:
	afx_msg void OnEnChangeEditName();
private:
	CString m_NodeName;
public:
	afx_msg void OnBnClickedButtonAdd();
private:
	void AppLog(CString str);
public:
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnEnKillfocusEditName();
private:
	CListBox m_CList;
public:
	afx_msg void OnBnClickedCheck3();
private:
	CButton m_C文本在同目录;
	CButton m_c包括子目录;
};
