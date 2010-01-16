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


// CPTXT_OutDlg 对话框

class CPTXT_OutDlg : public CDialog
{
	DECLARE_DYNAMIC(CPTXT_OutDlg)

public:
	CPTXT_OutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPTXT_OutDlg();

// 对话框数据
	enum { IDD = IDD_PTXT_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_index0;
	CString m_index1;
	CString m_baseOffset;
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
private:
	CString m_indexLen;
	CString m_ptrLen;
	CString m_ptrMul;
	int m_Type;
public:
	afx_msg void OnCbnSelendokCombo5();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonRom();
private:
	CString m_ROM;
	CString m_TBL;
	CString m_TBL2;
public:
	afx_msg void OnBnClickedButtonTbl();
	afx_msg void OnBnClickedButtonTbl2();
	afx_msg void OnBnClickedButton1();
};
