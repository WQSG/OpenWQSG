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
// WQSG最前端对话框.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WQSG最前端对话框.h"


// WQSG最前端对话框 对话框

IMPLEMENT_DYNAMIC(WQSG最前端对话框, CDialog)

WQSG最前端对话框::WQSG最前端对话框(CWnd* pParent /*=NULL*/)
	: CDialog(WQSG最前端对话框::IDD, pParent)
	, m_内容(_T(""))
{
}

WQSG最前端对话框::~WQSG最前端对话框()
{
}

void WQSG最前端对话框::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_内容);
}


BEGIN_MESSAGE_MAP(WQSG最前端对话框, CDialog)
END_MESSAGE_MAP()


// WQSG最前端对话框 消息处理程序

void WQSG最前端对话框::设置内容(CString 内容)
{
	m_内容 = 内容;
	UpdateData(FALSE);
}
