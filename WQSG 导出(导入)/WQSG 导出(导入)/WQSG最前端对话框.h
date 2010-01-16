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


// WQSG最前端对话框 对话框

class WQSG最前端对话框 : public CDialog
{
	DECLARE_DYNAMIC(WQSG最前端对话框)

public:
	WQSG最前端对话框(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WQSG最前端对话框();

// 对话框数据
	enum { IDD = IDD_AB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_内容;
public:
	void SetWindowText(LPCTSTR lpszString){
		CDialog::SetWindowText(lpszString);
	}
	void 设置内容(CString 内容);
};
