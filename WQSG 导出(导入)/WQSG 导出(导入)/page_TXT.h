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


// Cpage_TXT_OUT 对话框

class Cpage_TXT_OUT : public CDialog
{
	DECLARE_DYNAMIC(Cpage_TXT_OUT)

public:
	Cpage_TXT_OUT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cpage_TXT_OUT();

// 对话框数据
	enum { IDD = IDD_TXT_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
public:
	CString W_KS;
	CString W_ROM地址;
	CString W_JS;
	CString W_MIN;
	CString W_MAX;
	afx_msg void OnEnChangeEditKs();
	afx_msg void OnEnChangeEditJs();
	afx_msg void OnEnChangeEditMin();
	afx_msg void OnEnChangeEditMax();
	afx_msg void OnEnChangeEditKs2();
	CString W_KS2;
	afx_msg void OnBnClickedButton1();
};

// Cpage_TXT_IN 对话框

class Cpage_TXT_IN : public CDialog
{
	DECLARE_DYNAMIC(Cpage_TXT_IN)

public:
	Cpage_TXT_IN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cpage_TXT_IN();

// 对话框数据
	enum { IDD = IDD_TXT_IN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
public:
	virtual void OnCancel();
public:
//	afx_msg void OnBnClickedButtonSt();
public:
	int 填充类型;
public:
	CString 单字节填充;
public:
	CString 双字节填充;
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	afx_msg void OnEnChangeEdit1();
public:
	afx_msg void OnEnChangeEdit2();
public:
	BOOL W_长度不足提示;
public:
	afx_msg void OnEnKillfocusEdit1();
public:
	afx_msg void OnEnKillfocusEdit2();
};
