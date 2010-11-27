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

IMPLEMENT_DYNAMIC(CWIPS_O_Dlg, CBaseDialog)
CWIPS_O_Dlg::CWIPS_O_Dlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CWIPS_O_Dlg::IDD, pParent)
	, m_strWipsDesc(_T(""))
	, m_strOldFile(_T(""))
	, m_strNewFle(_T(""))
	, m_strBeginOffset(_T("0"))
	, m_strEndOffset(_T("7FFFFFFFFFFFFFFF"))
{
	HRSRC hrexe = ::FindResource(NULL,MAKEINTRESOURCE(IDR_EXE1),_T("EXE"));
//	if(NULL == hrexe)return FALSE;

	DWORD dwSize = ::SizeofResource(NULL, hrexe);
	if(dwSize != 0)
	{
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
	CBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_strWipsDesc);
	DDV_MaxChars(pDX, m_strWipsDesc, 65535);
	DDX_Text(pDX, IDC_EDIT2, m_strOldFile);
	DDV_MaxChars(pDX, m_strOldFile, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT3, m_strNewFle);
	DDV_MaxChars(pDX, m_strNewFle, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT4, m_strBeginOffset);
	DDV_MaxChars(pDX, m_strBeginOffset, 16);
	DDX_Text(pDX, IDC_EDIT5, m_strEndOffset);
	DDV_MaxChars(pDX, m_strEndOffset, 16);
}
BEGIN_MESSAGE_MAP(CWIPS_O_Dlg, CBaseDialog)
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

	static CWQSGFileDialog_Open fopendlg(_T("原文件(*.*)|*.*||"));
	fopendlg.SetWindowTitle( _T("选择一个原文件...") );

	if(IDOK == fopendlg.DoModal())
	{
		m_strOldFile = fopendlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CWQSGFileDialog_Open fopendlg(_T("新文件(*.*)|*.*||"));
	fopendlg.SetWindowTitle( _T("选择一个新文件...") );

	if(IDOK == fopendlg.DoModal())
	{
		m_strNewFle = fopendlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_strOldFile.GetLength() == 0)
	{
		MessageBox(_T("原文件不能为空"));
		return;
	}
	if(m_strNewFle.GetLength() == 0)
	{
		MessageBox(_T("新文件不能为空"));
		return;
	}
	s64 nBeginOffset = 0,
		nEndOffset = 0;

	::swscanf( m_strBeginOffset.GetString() ,L"%llx" , &nBeginOffset );
	/////////////////////
	::swscanf( m_strEndOffset.GetString() ,L"%llx" , &nEndOffset );
	//////////////////////////
	BOOL bMakeExe = (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == 1);

	CWQSGFileDialog_Save fopendlg( ((bMakeExe)?_T("可执行文件(*.EXE)|*.EXE||"):_T("WIPS补丁文件(*.WIPS)|*.WIPS||")) , (bMakeExe)?_T("EXE"):_T("WIPS") );
	fopendlg.SetWindowTitle( _T("补丁保存到...") );

	if(IDOK == fopendlg.DoModal())
	{
		CString strFile = fopendlg.GetPathName();
		::CWQSG_IPS_OUT WQSG(m_hWnd,_T("WQSG IPS补丁程序"));

		WQSG.MakeWips30( m_strOldFile.GetString() , m_strNewFle.GetString() , strFile.GetString() ,
			nBeginOffset , nEndOffset , 0 , (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == 1)?1:0 ,
			m_strWipsDesc.GetString() , ::WQSG_strlen(m_strWipsDesc.GetString()) , (bMakeExe)?&m_自解头30:NULL );
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
	EditCheckHaxStr(m_strBeginOffset,(CEdit*)GetDlgItem(IDC_EDIT4),(CWnd*)this);
	if(m_strBeginOffset.GetLength() == 0)
	{
		m_strBeginOffset = _T("0");
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
	EditCheckHaxStr( m_strEndOffset , (CEdit*)GetDlgItem(IDC_EDIT5),(CWnd*)this);
	if(m_strEndOffset.GetLength() == 0)
	{
		m_strEndOffset = _T("0");
		UpdateData(FALSE);
	}
}

void CWIPS_O_Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseDialog::OnClose();
//	if(IDOK == MessageBox(_T("要结束WIPS补丁程序?"),NULL,MB_OKCANCEL))
//	{
		CBaseDialog::OnCancel();
//	}
}
//------------------------------------------------
IMPLEMENT_DYNAMIC(CWIPS_I_Dlg, CBaseDialog)
CWIPS_I_Dlg::CWIPS_I_Dlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CWIPS_I_Dlg::IDD, pParent)
	, m_strWipsFile(_T(""))
	, m_strWipsDesc_In(_T(""))
{
}

CWIPS_I_Dlg::~CWIPS_I_Dlg()
{
}
void CWIPS_I_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, m_strWipsFile);
	DDV_MaxChars(pDX, m_strWipsFile, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT10, m_strWipsDesc_In);
}


BEGIN_MESSAGE_MAP(CWIPS_I_Dlg, CBaseDialog)
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
	static CWQSGFileDialog_Open fopendlg( _T("WIPS补丁文件(*.WIPS)|*.WIPS||") );
	fopendlg.SetWindowTitle( _T("选择一个WIPS补丁文件...") );

	if(IDOK == fopendlg.DoModal())
	{
		m_strWipsFile = fopendlg.GetPathName();
		ShowWipsInfo(m_strWipsFile);
		UpdateData(FALSE);
	}
}

void CWIPS_I_Dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_strWipsFile.GetLength() == 0)
	{
		MessageBox(_T("补丁文件不能为空"));
		return;
	}


	static CWQSGFileDialog_Save fopendlg( _T("目标文件文件(*.*)|*.*||") );
	fopendlg.SetWindowTitle( _T("对选择的文件打补丁...") );

	if(IDOK == fopendlg.DoModal())
	{
		CString strFile = fopendlg.GetPathName();
		::CWQSG_IPS_IN WQSG(m_hWnd,_T("WQSG IPS补丁程序"));
		WQSG.打补丁(m_strWipsFile.GetString(),strFile.GetString());
	}
}
void CWIPS_I_Dlg::ShowWipsInfo(const CString& a_strFile)
{
	::CWD_WipsInfo info;
	m_strWipsDesc_In = _T("非法或者不支持的补丁文件");

	if(info.LoadWipsDesc( a_strFile.GetString() ) )
	{
		if(::WQSG_strlen(info.m_szDesc) == 0)
		{
			::WQSG_strcpy(L"无",info.m_szDesc);
		}

		if(info.m_uCrc32)
		{
			m_strWipsDesc_In.Format( L"补丁版本: %s\r\n目标文件的CRC32验证: %X\r\n打补丁后的文件大小: %u\r\n\r\n补丁说明:\r\n",
				info.补丁标识 , info.m_uCrc32 , info.目标大小 );
		}
		else
		{
			m_strWipsDesc_In.Format( L"补丁版本: %s\r\n目标文件的CRC32验证: 无\r\n打补丁后的文件大小: %u\r\n\r\n补丁说明:\r\n"
				, info.补丁标识 , info.目标大小 );
		}
		m_strWipsDesc_In += info.m_szDesc;
	}
}
void CWIPS_I_Dlg::OnBnClickedButton1()
{
	UpdateData();
	ShowWipsInfo(m_strWipsFile);
	UpdateData(FALSE);
}
void CWIPS_I_Dlg::OnClose()
{
	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}
