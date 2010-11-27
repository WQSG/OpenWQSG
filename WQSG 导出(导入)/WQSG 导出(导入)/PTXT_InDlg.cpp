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
// PTXT_InDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "PTXT_InDlg.h"


// CPTXT_InDlg 对话框

IMPLEMENT_DYNAMIC(CPTXT_InDlg, CBaseDialog)

CPTXT_InDlg::CPTXT_InDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CPTXT_InDlg::IDD, pParent)
{

}

CPTXT_InDlg::~CPTXT_InDlg()
{
}

void CPTXT_InDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPTXT_InDlg, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPTXT_InDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
// CPTXT_InDlg 消息处理程序

void CPTXT_InDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPTXT_InDlg::OnCancel(){}
void CPTXT_InDlg::OnOK(){}
BOOL CPTXT_InDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPTXT_InDlg::OnClose()
{
	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}
