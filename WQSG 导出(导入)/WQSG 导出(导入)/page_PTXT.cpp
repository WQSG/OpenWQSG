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
// page_PTXT_OUT.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "page_PTXT.h"


// Cpage_PTXT_OUT 对话框

IMPLEMENT_DYNAMIC(Cpage_PTXT_OUT, CDialog)
void Cpage_PTXT_OUT::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void Cpage_PTXT_OUT::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
Cpage_PTXT_OUT::Cpage_PTXT_OUT(CWnd* pParent /*=NULL*/)
	: CDialog(Cpage_PTXT_OUT::IDD, pParent)
	, W_pKS(_T(""))
	, W_pJS(_T(""))
	, W_p文本基础(_T(""))
	, W_p指针间距(_T("2"))
	, W_p实际指针长度(_T("2"))
	, W_p指针倍数(_T("1"))
	, W_p指针格式(0)
	, W_p长度块倍数(_T("1"))
	, W_文本长度(_T("2"))
{

}

Cpage_PTXT_OUT::~Cpage_PTXT_OUT()
{
}

void Cpage_PTXT_OUT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, W_pKS);
	DDV_MaxChars(pDX, W_pKS, 8);
	DDX_Text(pDX, IDC_EDIT2, W_pJS);
	DDV_MaxChars(pDX, W_pJS, 8);
	DDX_Text(pDX, IDC_EDIT3, W_p文本基础);
	DDV_MaxChars(pDX, W_p文本基础, 8);
	DDX_CBString(pDX, IDC_COMBO1, W_p指针间距);
	DDX_CBString(pDX, IDC_COMBO2, W_p实际指针长度);
	DDX_CBString(pDX, IDC_COMBO4, W_p指针倍数);
	DDX_CBIndex(pDX, IDC_COMBO5, W_p指针格式);
	DDX_CBString(pDX, IDC_COMBO6, W_p长度块倍数);
	DDX_CBString(pDX, IDC_COMBO3, W_文本长度);
}

BEGIN_MESSAGE_MAP(Cpage_PTXT_OUT, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO5, &Cpage_PTXT_OUT::OnCbnSelchangeCombo5)
	ON_EN_CHANGE(IDC_EDIT1, &Cpage_PTXT_OUT::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &Cpage_PTXT_OUT::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &Cpage_PTXT_OUT::OnEnChangeEdit3)
END_MESSAGE_MAP()

// Cpage_TXT_OUT 消息处理程序
void Cpage_PTXT_OUT::OnCbnSelchangeCombo5(){
	// TODO: 在此添加控件通知处理程序代码
	if( 2 == ((CComboBox*)GetDlgItem(IDC_COMBO5))->GetCurSel()){
		GetDlgItem(IDC_COMBO6)->EnableWindow();
		GetDlgItem(IDC_COMBO3)->EnableWindow();
	}
	else{
		GetDlgItem(IDC_COMBO6)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
	}
}

void Cpage_PTXT_OUT::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_pKS,(CEdit*)GetDlgItem(IDC_EDIT1),(CWnd*)this);
}

void Cpage_PTXT_OUT::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_pJS,(CEdit*)GetDlgItem(IDC_EDIT2),(CWnd*)this);
}

void Cpage_PTXT_OUT::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_p文本基础,(CEdit*)GetDlgItem(IDC_EDIT3),(CWnd*)this);
}



// page_PTXT_IN.cpp : 实现文件
//


// Cpage_PTXT_IN 对话框

IMPLEMENT_DYNAMIC(Cpage_PTXT_IN, CDialog)
void Cpage_PTXT_IN::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void Cpage_PTXT_IN::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
Cpage_PTXT_IN::Cpage_PTXT_IN(CWnd* pParent /*=NULL*/)
	: CDialog(Cpage_PTXT_IN::IDD, pParent)
	, m_p文本区界限地址(_T(""))
	, m_p文本区开始地址(_T(""))
{

}

Cpage_PTXT_IN::~Cpage_PTXT_IN()
{
}

void Cpage_PTXT_IN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_p文本区界限地址);
	DDV_MaxChars(pDX, m_p文本区界限地址, 8);
	DDX_Text(pDX, IDC_EDIT11, m_p文本区开始地址);
	DDV_MaxChars(pDX, m_p文本区开始地址, 8);
}


BEGIN_MESSAGE_MAP(Cpage_PTXT_IN, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &Cpage_PTXT_IN::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT11, &Cpage_PTXT_IN::OnEnChangeEdit11)
END_MESSAGE_MAP()

// Cpage_PTXT_IN 消息处理程序

void Cpage_PTXT_IN::OnEnChangeEdit1(){
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(m_p文本区界限地址,(CEdit*)GetDlgItem(IDC_EDIT1),(CWnd*)this);
}

void Cpage_PTXT_IN::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(m_p文本区开始地址,(CEdit*)GetDlgItem(IDC_EDIT11),(CWnd*)this);
}
