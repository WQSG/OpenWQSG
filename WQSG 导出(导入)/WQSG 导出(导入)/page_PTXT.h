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


// Cpage_PTXT_OUT 对话框

class Cpage_PTXT_OUT : public CDialog
{
	DECLARE_DYNAMIC(Cpage_PTXT_OUT)

public:
	Cpage_PTXT_OUT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cpage_PTXT_OUT();

// 对话框数据
	enum { IDD = IDD_PTXT_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual void OnCancel();
	CString W_pKS;
	CString W_pJS;
	CString W_p文本基础;
	CString W_p指针间距;
	CString W_p实际指针长度;
	CString W_p指针倍数;
public:
	int W_p指针格式;
public:
	CString W_p长度块倍数;
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnKillfocusEdit4();
	CString W_文本长度;
};

// Cpage_PTXT_IN 对话框

class Cpage_PTXT_IN : public CDialog
{
	DECLARE_DYNAMIC(Cpage_PTXT_IN)

public:
	Cpage_PTXT_IN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cpage_PTXT_IN();

// 对话框数据
	enum { IDD = IDD_PTXT_IN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit11();
public:
	CString m_p文本区界限地址;
	CString m_p文本区开始地址;
};
