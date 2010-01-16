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
// page_TXT_OUT.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "page_TXT.h"


// Cpage_TXT_OUT 对话框

IMPLEMENT_DYNAMIC(Cpage_TXT_OUT, CDialog)
void Cpage_TXT_OUT::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void Cpage_TXT_OUT::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
Cpage_TXT_OUT::Cpage_TXT_OUT(CWnd* pParent /*=NULL*/)
	: CDialog(Cpage_TXT_OUT::IDD, pParent)
	, W_KS(_T(""))
	, W_JS(_T(""))
	, W_MIN(_T("0"))
	, W_MAX(_T("99999"))
	, W_KS2(_T("0"))
	, W_ROM地址(_T(""))
{
}

Cpage_TXT_OUT::~Cpage_TXT_OUT()
{
}

void Cpage_TXT_OUT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KS, W_KS);
	DDV_MaxChars(pDX, W_KS, 8);
	DDX_Text(pDX, IDC_EDIT_JS, W_JS);
	DDV_MaxChars(pDX, W_JS, 8);
	DDX_Text(pDX, IDC_EDIT_MIN, W_MIN);
	DDV_MaxChars(pDX, W_MIN, 5);
	DDX_Text(pDX, IDC_EDIT_MAX, W_MAX);
	DDV_MaxChars(pDX, W_MAX, 5);
	DDX_Text(pDX, IDC_EDIT_KS2, W_KS2);
	DDV_MaxChars(pDX, W_KS2, 8);
}


BEGIN_MESSAGE_MAP(Cpage_TXT_OUT, CDialog)
	ON_EN_CHANGE(IDC_EDIT_KS, &Cpage_TXT_OUT::OnEnChangeEditKs)
	ON_EN_CHANGE(IDC_EDIT_JS, &Cpage_TXT_OUT::OnEnChangeEditJs)
	ON_EN_CHANGE(IDC_EDIT_MIN, &Cpage_TXT_OUT::OnEnChangeEditMin)
	ON_EN_CHANGE(IDC_EDIT_MAX, &Cpage_TXT_OUT::OnEnChangeEditMax)
	ON_EN_CHANGE(IDC_EDIT_KS2, &Cpage_TXT_OUT::OnEnChangeEditKs2)
	ON_BN_CLICKED(IDC_BUTTON1, &Cpage_TXT_OUT::OnBnClickedButton1)
END_MESSAGE_MAP()

// Cpage_TXT_OUT 消息处理程序
void Cpage_TXT_OUT::OnEnChangeEditKs()
	{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_KS,(CEdit*)GetDlgItem(IDC_EDIT_KS),(CWnd*)this);
	}

void Cpage_TXT_OUT::OnEnChangeEditJs()
	{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_JS,(CEdit*)GetDlgItem(IDC_EDIT_JS),(CWnd*)this);
	}

void Cpage_TXT_OUT::OnEnChangeEditMin()
	{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十进制文本(W_MIN,(CEdit*)GetDlgItem(IDC_EDIT_MIN),(CWnd*)this);
	if(0 == W_MIN.GetLength())
		{
		W_MIN = _T("0");
		UpdateData(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MIN))->SetSel(1,1,FALSE);
		}
	}

void Cpage_TXT_OUT::OnEnChangeEditMax()
	{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十进制文本(W_MAX,(CEdit*)GetDlgItem(IDC_EDIT_MAX),(CWnd*)this);
	if(0 == W_MAX.GetLength())
		{
		W_MAX = _T("99999");
		UpdateData(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MAX))->SetSel(5,5,FALSE);
		}
	}

void Cpage_TXT_OUT::OnEnChangeEditKs2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_KS2,(CEdit*)GetDlgItem(IDC_EDIT_KS2),(CWnd*)this);
	if(0 == W_KS2.GetLength())
	{
		W_KS2 = _T("0");
		UpdateData(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_KS2))->SetSel(1,1,FALSE);
	}
}
void Cpage_TXT_OUT::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	U64 段号 = 0;
	::swscanf_s(W_KS2.GetBuffer(),_T("%x"),&段号);
	段号 <<= 32;
	if(W_ROM地址.GetLength() == 0){
		W_JS = _T("");
		UpdateData(FALSE);
		return;
	}
	::CWQSG_File ROM_File;
	if(! ROM_File.OpenFile(W_ROM地址.GetBuffer(),1,3)){
		MessageBox(_T("获取ROM长度失败"));
		return;
	}
	U64 长度;
	ROM_File.GetFileSize(&长度);
	if(长度 < 段号){
		W_JS = _T("");
	}
	else{
		if(长度 >= (段号 + 0x100000000)){
			长度 = 0xFFFFFFFF;
		}
		else{
			长度 -= 段号;
		}
		W_JS.Format(_T("%X"),(长度 - 1));
	}
	UpdateData(FALSE);
}

// page_TXT_IN.cpp : 实现文件
//


// Cpage_TXT_IN 对话框

IMPLEMENT_DYNAMIC(Cpage_TXT_IN, CDialog)
void Cpage_TXT_IN::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void Cpage_TXT_IN::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
Cpage_TXT_IN::Cpage_TXT_IN(CWnd* pParent /*=NULL*/)
	: CDialog(Cpage_TXT_IN::IDD, pParent)
	, 填充类型(0)
	, 单字节填充(_T(""))
	, 双字节填充(_T(""))
	, W_长度不足提示(FALSE)
{

}

Cpage_TXT_IN::~Cpage_TXT_IN()
{
}

void Cpage_TXT_IN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO1, 填充类型);
	DDX_Text(pDX, IDC_EDIT1, 单字节填充);
	DDV_MaxChars(pDX, 单字节填充, 2);
	DDX_Text(pDX, IDC_EDIT2, 双字节填充);
	DDV_MaxChars(pDX, 双字节填充, 4);
	DDX_Check(pDX, IDC_CHECK1, W_长度不足提示);
}


BEGIN_MESSAGE_MAP(Cpage_TXT_IN, CDialog)
//	ON_BN_CLICKED(IDC_BUTTON_ST, &Cpage_TXT_IN::OnBnClickedButtonSt)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cpage_TXT_IN::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &Cpage_TXT_IN::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &Cpage_TXT_IN::OnEnChangeEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT1, &Cpage_TXT_IN::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &Cpage_TXT_IN::OnEnKillfocusEdit2)
END_MESSAGE_MAP()


// Cpage_TXT_IN 消息处理程序
void Cpage_TXT_IN::OnCbnSelchangeCombo1()
	{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	switch(填充类型)
		{
		case 1:
			GetDlgItem(IDC_EDIT1)->EnableWindow();
			GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
			break;
		case 2:
			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT2)->EnableWindow();
			break;
		case 3:
			GetDlgItem(IDC_EDIT1)->EnableWindow();
			GetDlgItem(IDC_EDIT2)->EnableWindow();
			break;
		default:
			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		}
	}
void Cpage_TXT_IN::OnEnChangeEdit1(){
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(单字节填充,(CEdit*)GetDlgItem(IDC_EDIT1),(CWnd*)this,0);
}

void Cpage_TXT_IN::OnEnChangeEdit2(){
		// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
		// 发送该通知，除非重写 CDialog::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码
		UpdateData();
		编辑框验证十六进制文本(双字节填充,(CEdit*)GetDlgItem(IDC_EDIT2),(CWnd*)this,0);
}
void Cpage_TXT_IN::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(单字节填充.GetLength()&1){
		单字节填充.Insert(0,_T('0'));
		UpdateData();
	}
}

void Cpage_TXT_IN::OnEnKillfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(双字节填充.GetLength()&1){
		双字节填充.Insert(0,_T('0'));
		UpdateData();
	}
}
