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


// WIPS 对话框


class CWIPS_O_Dlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CWIPS_O_Dlg)
	::CWD_BIN m_自解头30;

public:
	CWIPS_O_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWIPS_O_Dlg();

// 对话框数据
	enum { IDD = IDD_WIPS_O };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	CString m_strWipsDesc;
	CString m_strOldFile;
	CString m_strNewFle;
	CString m_strBeginOffset;
	CString m_strEndOffset;

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnClose();
};

class CWIPS_I_Dlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CWIPS_I_Dlg)
public:
	CWIPS_I_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWIPS_I_Dlg();
// 对话框数据
	enum { IDD = IDD_WIPS_I };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	CString m_strWipsDesc_Out;
	CString m_strWipsFile;
	CString m_strWipsDesc_In;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	void ShowWipsInfo(const CString& a_strFile);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
