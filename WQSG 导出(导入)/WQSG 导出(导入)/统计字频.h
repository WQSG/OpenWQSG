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
#include "afxwin.h"
#if DEF_ON_TBL
// C统计字频 对话框

class C统计字频 : public CDialog
{
	DECLARE_DYNAMIC(C统计字频)
	WCHAR W_KZF_L;
	WCHAR W_KZF_R;
public:
	C统计字频(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C统计字频();

// 对话框数据
	enum { IDD = IDD_DIALOG_TongJiZiPin };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	struct SCharTblInfo 
	{
		u32 m_uCount;
		BOOL m_bUnTbl;

		void Clean()
		{
			m_uCount = 0;
			m_bUnTbl = TRUE;
		}
	};
public:
	virtual INT_PTR DoModal();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual void OnOK();
	virtual void OnCancel();
	CString W_码表地址;
	afx_msg void OnBnClickedButtonTbl();
	void 清零();
	BOOL 载入码表();
	afx_msg void OnBnClickedButton2();

	SCharTblInfo m_Info[0x10000];

	void 统计一句字频_过滤控制符(WCHAR* TXT);
	void 统计一句字频_纯文本(WCHAR* TXT);
	CString W_显示框;
	int W_控制符类型;
	BOOL W_对照码表;
	afx_msg void OnBnClickedCheckTbl();
	BOOL W_统计模式;
	CString W_等待编码的字;
	CString W_第一个编码;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnKillfocusEdit3();
	BOOL W_高低交换;
	int W_窗口类型;
	afx_msg void OnBnClickedCheck1();
	CString W_编码表;
	afx_msg void OnBnClickedButtonTbl3();
	afx_msg void OnBnClickedButton3();
	BOOL 编码表编码(::CWQSG_File * TXT_File);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnEnKillfocusEditTbl3();
	afx_msg void OnClose();
private:
	CComboBox m_OutType;
};

#endif //DEF_ON_TBL