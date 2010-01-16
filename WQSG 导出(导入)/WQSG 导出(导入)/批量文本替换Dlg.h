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

#include "WQSG_cfg.h"

#if DEF_ON_文本替换

#include "afxwin.h"


// C批量文本替换Dlg 对话框

class C批量文本替换Dlg : public CDialog
{
	DECLARE_DYNAMIC(C批量文本替换Dlg)
	CWQSG_TypeLinkList< CStringW >	m_替换链表[2];
	u32 m_完成数量;
// 构造
public:
	C批量文本替换Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TXT_R };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_查找内容;
public:
	CString m_替换为;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCheck1();
private:
	CButton m_C高级替换;
	CButton m_C替换子目录;
public:
	afx_msg void OnBnClickedButton2();
private:
	CString m_替换表;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
private:
	bool 载入替换表(void);
	bool 替换目录(CString 路径);
	bool 替换文本(CString 文件);
};

#endif //DEF_ON_文本替换