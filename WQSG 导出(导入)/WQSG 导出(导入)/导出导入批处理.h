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


// 导出导入批处理 对话框

class 导出批处理 : public CDialog
{
	DECLARE_DYNAMIC(导出批处理)

public:
	导出批处理(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~导出批处理();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	virtual void OnOK();
public:
	virtual void OnCancel();
public:
	CListCtrl* W_列表;
public:
	INT W_列表下一个位置;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTbl();
public:
	afx_msg void OnBnClickedButtonTbl2();
public:
	afx_msg void OnEnChangeEditKs();
public:
	afx_msg void OnEnChangeEditJs();
public:
	afx_msg void OnEnChangeEditMin();
public:
	afx_msg void OnEnChangeEditMax();
public:
	afx_msg void OnBnClickedButtonAdd();
public:
	afx_msg void OnBnClickedButtonDel();
public:
	afx_msg void OnLvnItemActivateListW(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButtonEdit();
public:
	afx_msg void OnEnChangeEditTbl();
public:
	afx_msg void OnEnChangeEditTbl2();
public:
	afx_msg void OnBnClickedButtonStart();
public:
	CString W_开始地址;
public:
	CString W_结束地址;
public:
	BOOL W_验证;
public:
	CString W_MIN;
public:
	CString W_MAX;
public:
	CString W_默认码表;
public:
	CString W_默认控制码表;
public:
	CString W_ROM;
public:
	void 保存项目表(CString 文件地址);
public:
	void 载入项目表(CString 文件地址);
public:
	CString W_数据文件地址;
public:
	CString W_当前目录;
};