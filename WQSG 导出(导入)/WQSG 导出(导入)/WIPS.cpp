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
// WIPS.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WIPS.h"
#include"WQSG_IPS.h"


// WIPS 对话框

IMPLEMENT_DYNAMIC(CWIPS_O_Dlg, CDialog)
CWIPS_O_Dlg::CWIPS_O_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWIPS_O_Dlg::IDD, pParent)
	, W_补丁说明_OUT(_T(""))
	, W_原文件(_T(""))
	, W_新文件(_T(""))
	, W_开始地址(_T("0"))
	, W_结束地址(_T("7FFFFFFFFFFFFFFF"))
{
	HRSRC hrexe = ::FindResource(NULL,MAKEINTRESOURCE(IDR_EXE1),_T("EXE"));
//	if(NULL == hrexe)return FALSE;

	DWORD dwSize = ::SizeofResource(NULL, hrexe);
	if(dwSize != 0){
	}
	HGLOBAL hgdexe = ::LoadResource(NULL,hrexe);
//	if(NULL == hgdexe)return FALSE;

	u8 * lp_BIN = (u8*)::LockResource(hgdexe);
///////////////////////////////////
	m_自解头30.BIN = new u8[dwSize];
	m_自解头30.LEN = dwSize;
	::memcpy_s(m_自解头30.BIN,dwSize,lp_BIN,dwSize);
///////////////////////////////////
	::FreeResource(hgdexe);
}

CWIPS_O_Dlg::~CWIPS_O_Dlg()
{
}
void CWIPS_O_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, W_补丁说明_OUT);
	DDV_MaxChars(pDX, W_补丁说明_OUT, 65535);
	DDX_Text(pDX, IDC_EDIT2, W_原文件);
	DDV_MaxChars(pDX, W_原文件, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT3, W_新文件);
	DDV_MaxChars(pDX, W_新文件, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT4, W_开始地址);
	DDV_MaxChars(pDX, W_开始地址, 16);
	DDX_Text(pDX, IDC_EDIT5, W_结束地址);
	DDV_MaxChars(pDX, W_结束地址, 16);
}
BEGIN_MESSAGE_MAP(CWIPS_O_Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CWIPS_O_Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWIPS_O_Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CWIPS_O_Dlg::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT4, &CWIPS_O_Dlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CWIPS_O_Dlg::OnEnChangeEdit5)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
void CWIPS_O_Dlg::OnOK(){}
void CWIPS_O_Dlg::OnCancel(){}
void CWIPS_O_Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CString strPath;

	CWQSGFileDialog fopendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("原文件(*.*)|*.*||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个原文件...");
	fopendlg.m_ofn.lpstrInitialDir = strPath;

	if(IDOK == fopendlg.DoModal())
	{
		strPath = fopendlg.GetFolderPath();
		W_原文件 = fopendlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CString strPath;

	CWQSGFileDialog fopendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("新文件(*.*)|*.*||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个新文件...");
	fopendlg.m_ofn.lpstrInitialDir = strPath;

	if(IDOK == fopendlg.DoModal())
	{
		strPath = fopendlg.GetFolderPath();
		W_新文件 = fopendlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(W_原文件.GetLength() == 0){
		MessageBox(_T("原文件不能为空"));
		return;
	}
	if(W_新文件.GetLength() == 0){
		MessageBox(_T("新文件不能为空"));
		return;
	}
	s64 开始地址 = 0,
		结束地址 = 0;

	::swscanf( W_开始地址.GetString() ,L"%llx" , &开始地址 );
	/////////////////////
	::swscanf( W_结束地址.GetString() ,L"%llx" , &结束地址 );
	//////////////////////////
	BOOL 生成EXE = (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == 1);

	static CString strPath;
	CWQSGFileDialog fopendlg( FALSE );
	fopendlg.m_ofn.lpstrInitialDir = strPath;

	fopendlg.m_ofn.lpstrFilter = (生成EXE)?_T("可执行文件(*.EXE)\0*.EXE\0\0"):_T("WIPS补丁文件(*.WIPS)\0*.WIPS\0\0");
	fopendlg.m_ofn.lpstrDefExt = (生成EXE)?_T("EXE"):_T("WIPS");
	fopendlg.m_ofn.lpstrTitle = _T("补丁保存到...");

	if(IDOK == fopendlg.DoModal())
	{
		strPath = fopendlg.GetFolderPath();
		CString 文件 = fopendlg.GetPathName();
		::CWQSG_IPS_OUT WQSG(m_hWnd,_T("WQSG IPS补丁程序"));
		WQSG.制作补丁30( W_原文件.GetString() , W_新文件.GetString() , 文件.GetString() ,
			开始地址 , 结束地址 , 0 , (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == 1)?1:0 ,
			W_补丁说明_OUT.GetString() , ::WQSG_strlen(W_补丁说明_OUT.GetString()) , (生成EXE)?&m_自解头30:NULL );
	}
}
void CWIPS_O_Dlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_开始地址,(CEdit*)GetDlgItem(IDC_EDIT4),(CWnd*)this);
	if(W_开始地址.GetLength() == 0)
	{
		W_开始地址 = _T("0");
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( W_结束地址 , (CEdit*)GetDlgItem(IDC_EDIT5),(CWnd*)this);
	if(W_结束地址.GetLength() == 0)
	{
		W_结束地址 = _T("0");
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
//	if(IDOK == MessageBox(_T("要结束WIPS补丁程序?"),NULL,MB_OKCANCEL)){
		CDialog::OnCancel();
//	}
}
//------------------------------------------------
IMPLEMENT_DYNAMIC(CWIPS_I_Dlg, CDialog)
CWIPS_I_Dlg::CWIPS_I_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWIPS_I_Dlg::IDD, pParent)
	, W_补丁文件(_T(""))
	, W_补丁说明_IN(_T(""))
{
}

CWIPS_I_Dlg::~CWIPS_I_Dlg()
{
}
void CWIPS_I_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, W_补丁文件);
	DDV_MaxChars(pDX, W_补丁文件, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT10, W_补丁说明_IN);
}


BEGIN_MESSAGE_MAP(CWIPS_I_Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON5, &CWIPS_I_Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CWIPS_I_Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CWIPS_I_Dlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
void CWIPS_I_Dlg::OnOK(){}
void CWIPS_I_Dlg::OnCancel(){}
void CWIPS_I_Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CString strPath;

	CWQSGFileDialog fopendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("WIPS补丁文件(*.WIPS)|*.WIPS||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个WIPS补丁文件...");
	fopendlg.m_ofn.lpstrInitialDir = strPath;

	if(IDOK == fopendlg.DoModal())
	{
		strPath = fopendlg.GetFolderPath();
		W_补丁文件 = fopendlg.GetPathName();
		显示补丁信息(W_补丁文件);
		UpdateData(FALSE);
	}
}

void CWIPS_I_Dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(W_补丁文件.GetLength() == 0)
	{
		MessageBox(_T("补丁文件不能为空"));
		return;
	}

	static CString strPath;

	CWQSGFileDialog fopendlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("目标文件文件(*.*)|*.*||"));
	fopendlg.m_ofn.lpstrTitle = _T("对选择的文件打补丁...");
	fopendlg.m_ofn.lpstrInitialDir = strPath;

	if(IDOK == fopendlg.DoModal())
	{
		strPath = fopendlg.GetFolderPath();
		CString 文件 = fopendlg.GetPathName();
		::CWQSG_IPS_IN WQSG(m_hWnd,_T("WQSG IPS补丁程序"));
		WQSG.打补丁(W_补丁文件.GetString(),文件.GetString());
	}
}
void CWIPS_I_Dlg::显示补丁信息(CString 文件)
{
	::CWQSG_IPS_IN WQSG(m_hWnd,_T("WQSG IPS补丁程序"));
	::CWD_WIPS信息 补丁信息;
	W_补丁说明_IN = _T("非法或者不支持的补丁文件");
	if(WQSG.取文件补丁信息( 文件.GetString(),&补丁信息) )
	{
		if(::WQSG_strlen(补丁信息.说明文本) == 0)
		{
			::WQSG_strcpy(L"无",补丁信息.说明文本);
		}
		if(补丁信息.验证CRC32)
		{
			W_补丁说明_IN.Format(L"补丁版本: %s\r\n目标文件的CRC32验证: %X\r\n打补丁后的文件大小: %u\r\n\r\n补丁说明:\r\n",
				补丁信息.补丁标识,补丁信息.验证CRC32,补丁信息.目标大小);
		}
		else
		{
			W_补丁说明_IN.Format(L"补丁版本: %s\r\n目标文件的CRC32验证: 无\r\n打补丁后的文件大小: %u\r\n\r\n补丁说明:\r\n"
				,补丁信息.补丁标识,补丁信息.目标大小);
		}
		W_补丁说明_IN += 补丁信息.说明文本;
	}
}
void CWIPS_I_Dlg::OnBnClickedButton1()
{
	UpdateData();
	显示补丁信息(W_补丁文件);
	UpdateData(FALSE);
}
void CWIPS_I_Dlg::OnClose()
{
	CDialog::OnClose();
	CDialog::OnCancel();
}
