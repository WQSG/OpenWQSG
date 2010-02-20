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
// MFCDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "WQSG最前端对话框.h"


class CWQSG导出导入Dlg : public CDialog
{

// 构造
public:
	CWQSG导出导入Dlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_MFC_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void 改变选项(void);
	void 写配置(void);
	void 写列表(CListBox * lb_tmp);
	void 创建目录(void);
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedButtonRom();
	afx_msg void OnBnClickedButtonTbl();
	afx_msg void OnBnClickedButtonTbl2();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnEnUpdateEditName();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnEnKillfocusEditName();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckPtxt();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnEnChangeEditRom();
	afx_msg void OnEnChangeEditTbl();
	afx_msg void OnEnChangeEditTbl2();
	afx_msg void OnBnClickedButtonBat();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnKillfocusEditRom();

	CString 配置文件,
		配置列表文件;

	CString m_ROM
		, m_ROM_DIR
		, m_TXT
		, m_TBL
		, m_TBL
		, m_NAME;

	CString W_运行目录;

protected:
	afx_msg LRESULT 设置前端内容( WPARAM 保留 , LPARAM 内容 );
	afx_msg LRESULT 取前端窗口( WPARAM 保留 , LPARAM 窗口指针 );
	WQSG最前端对话框 m_最前端对话框;
	static DWORD WINAPI 普通导出(LPVOID lpParameter);
	static DWORD WINAPI 普通导入(LPVOID lpParameter);
	static DWORD WINAPI 地址_指针导出(LPVOID lpParameter);
	static DWORD WINAPI 地址_指针导入(LPVOID lpParameter);
	struct 参数结构{
		HWND hwnd;
		CString ROM,TBL,TBL2,TXT;
		int 验证;
	}m_参数表;

public:
	afx_msg void OnClose();
private:
	CString m_EDIT_TXT;
};
