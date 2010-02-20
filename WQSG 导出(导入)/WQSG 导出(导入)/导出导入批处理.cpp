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
// 导出导入批处理.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "导出导入批处理.h"

CString 严格读入一行文本(HANDLE F_ID){
	DWORD III;
	CString tmp;
	WCHAR wtmp;
	while(::ReadFile(F_ID,&wtmp,2,&III,NULL),2 == III){
		if(0x0D == wtmp){
			if(::ReadFile(F_ID,&wtmp,2,&III,NULL),2 == III){
				if(0x0A != wtmp){
					//读写位置 -2
					::SetFilePointer(F_ID,-2L,NULL,FILE_CURRENT);
				}
			}
			break;
		}
		else if(0x00 == wtmp){
			break;
		}
		tmp += wtmp;
	}
	return tmp;
}
void 导出批处理::保存项目表(CString 文件地址){
	CString tmp = W_当前目录;
	tmp += _T("\\DATA");
	CreateDirectory(tmp,NULL);
	tmp += _T("\\WQSG 导出(导入)");
	CreateDirectory(tmp,NULL);
	::CWQSG_File File;
	if( !File.OpenFile( 文件地址,4,3)){
		MessageBox(_T("保存项目表失败"));
		return;
	}
	U32 II = W_列表下一个位置 - 1;
	File.Write("WIO0",4);
	File.Write(&II,4);
	for(INT 索引 = 0;索引 < (INT)II;索引++){
		tmp = W_列表->GetItemText(索引,1);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,2);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,3);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,4);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,5);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,6);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,7);
		tmp += L"\15\12";
		tmp += W_列表->GetItemText(索引,8);
		tmp += L"\15\12";
		File.Write(tmp.GetBuffer(),(tmp.GetLength()<< 1));
	}
}
void 导出批处理::载入项目表(CString 文件地址){
	CString tmp = W_当前目录;
	tmp += _T("\\DATA");
	CreateDirectory(tmp,NULL);
	tmp += _T("\\WQSG 导出(导入)");
	CreateDirectory(tmp,NULL);

	::CWQSG_File File;
	if(! File.OpenFile(文件地址,1,3)){
		return;
	}

	U32 II;
	File.Read(&II,4);
	if( *((U32*)"WIO0") != II){
		W_列表->DeleteAllItems();
		W_列表下一个位置 = 1;
		MessageBox(_T("数据库已经损坏"));
		return;
	}
	File.Read(&II,4);

	WCHAR 编号[6];
	for(INT 索引 = 0;索引 < (INT)II;索引++){
		::swprintf_s(编号,6,L"%05d",W_列表下一个位置++);
		W_列表->InsertItem(索引,编号);

		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,1,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,2,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,3,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,4,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,5,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,6,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,7,tmp);
		tmp = 严格读入一行文本(File.GetFileHANDLE());
		if(tmp.GetLength() == 0){
			W_列表->DeleteAllItems();
			W_列表下一个位置 = 1;
			MessageBox(_T("数据库已经损坏"));
			return;
		}
		W_列表->SetItemText(索引,8,tmp);
	};
}
void 导出批处理::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void 导出批处理::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if( IDOK == MessageBox(_T("是否要关闭?"),NULL,MB_OKCANCEL) )
	{
		//ReleaseMutex(WQSG_Mutex);
		CDialog::OnCancel();
	}
}
// 导出导入批处理 对话框

IMPLEMENT_DYNAMIC(导出批处理, CDialog)

导出批处理::导出批处理(CWnd* pParent /*=NULL*/)
	: CDialog(导出批处理::IDD, pParent)
//	, W_开始地址(_T(""))
//	, W_结束地址(_T(""))
	, W_验证(TRUE)
	, W_MIN(_T("0"))
	, W_MAX(_T("99999"))
//	, W_默认码表(_T(""))
//	, W_默认控制码表(_T(""))
//	, W_ROM(_T(""))
{
	W_列表下一个位置 = 1;
	
	TCHAR exeFullPath[MAX_PATH]; 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	W_当前目录 = exeFullPath;
	W_当前目录 = W_当前目录.Left(W_当前目录.ReverseFind(_T('\\')));
	W_数据文件地址 = W_当前目录;

	W_数据文件地址 += _T("\\DATA\\WQSG 导出(导入)\\配置0.CFG");
}

导出批处理::~导出批处理()
{
}
BOOL 导出批处理::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	W_列表 = (CListCtrl*)GetDlgItem(IDC_LIST_W);
	W_列表->InsertColumn(0,_T("项目编号"),LVCFMT_CENTER,65);
	W_列表->InsertColumn(1,_T("ROM地址"),LVCFMT_CENTER,335);
	W_列表->InsertColumn(2,_T("码表地址"),LVCFMT_CENTER,200);
	W_列表->InsertColumn(3,_T("控制码表地址"),LVCFMT_CENTER,200);
	W_列表->InsertColumn(4,_T("开始地址(HEX)"),LVCFMT_CENTER,100);
	W_列表->InsertColumn(5,_T("结束地址(HEX)"),LVCFMT_CENTER,100);
	W_列表->InsertColumn(6,_T("验证码表重复"),LVCFMT_CENTER,90);
	W_列表->InsertColumn(7,_T("过滤<="),LVCFMT_CENTER,70);
	W_列表->InsertColumn(8,_T("过滤>="),LVCFMT_CENTER,70);


	W_列表->SetExtendedStyle(W_列表->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	载入项目表(W_数据文件地址);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void 导出批处理::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KS, W_开始地址);
	DDV_MaxChars(pDX, W_开始地址, 8);
	DDX_Text(pDX, IDC_EDIT_JS, W_结束地址);
	DDV_MaxChars(pDX, W_结束地址, 8);
	DDX_Check(pDX, IDC_CHECK1, W_验证);
	DDX_Text(pDX, IDC_EDIT_MIN, W_MIN);
	DDV_MaxChars(pDX, W_MIN, 5);
	DDX_Text(pDX, IDC_EDIT_MAX, W_MAX);
	DDV_MaxChars(pDX, W_MAX, 5);
	DDX_Text(pDX, IDC_EDIT_TBL, W_默认码表);
	DDV_MaxChars(pDX, W_默认码表, MAX_PATH - 1);
	DDX_Text(pDX, IDC_EDIT_TBL2, W_默认控制码表);
	DDV_MaxChars(pDX, W_默认控制码表, MAX_PATH - 1);
}


BEGIN_MESSAGE_MAP(导出批处理, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &导出批处理::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &导出批处理::OnBnClickedButtonTbl2)
	ON_EN_CHANGE(IDC_EDIT_KS, &导出批处理::OnEnChangeEditKs)
	ON_EN_CHANGE(IDC_EDIT_JS, &导出批处理::OnEnChangeEditJs)
	ON_EN_CHANGE(IDC_EDIT_MIN, &导出批处理::OnEnChangeEditMin)
	ON_EN_CHANGE(IDC_EDIT_MAX, &导出批处理::OnEnChangeEditMax)
	ON_BN_CLICKED(IDC_BUTTON_Add, &导出批处理::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Del, &导出批处理::OnBnClickedButtonDel)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_W, &导出批处理::OnLvnItemActivateListW)
	ON_BN_CLICKED(IDC_BUTTON_Edit, &导出批处理::OnBnClickedButtonEdit)
	ON_EN_CHANGE(IDC_EDIT_TBL, &导出批处理::OnEnChangeEditTbl)
	ON_EN_CHANGE(IDC_EDIT_TBL2, &导出批处理::OnEnChangeEditTbl2)
	ON_BN_CLICKED(IDC_BUTTON_START, &导出批处理::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// 导出导入批处理 消息处理程序

void 导出批处理::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open fopendlg(_T("码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个码表...");
	if(IDOK == fopendlg.DoModal())
	{
		W_默认码表 = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}

void 导出批处理::OnBnClickedButtonTbl2()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open fopendlg(_T("码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个控制码表...");
	if(IDOK == fopendlg.DoModal())
	{
		W_默认控制码表 = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}

void 导出批处理::OnEnChangeEditKs()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_开始地址,(CEdit*)GetDlgItem(IDC_EDIT_KS),(CWnd*)this);
}

void 导出批处理::OnEnChangeEditJs()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本(W_结束地址,(CEdit*)GetDlgItem(IDC_EDIT_JS),(CWnd*)this);
}

void 导出批处理::OnEnChangeEditMin()
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

void 导出批处理::OnEnChangeEditMax()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十进制文本(W_MAX,(CEdit*)GetDlgItem(IDC_EDIT_MAX),(CWnd*)this);
	if(0 == W_MIN.GetLength())
	{
		W_MIN = _T("99999");
		UpdateData(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MAX))->SetSel(5,5,FALSE);
	}
}
void 导出批处理::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(0 == W_默认码表.GetLength())
	{
		MessageBox(_T("码表地址不能为空"));
		return;
	}
	if(0 == W_默认控制码表.GetLength())
	{
		MessageBox(_T("控制码表地址不能为空"));
		return;
	}
	if(0 == W_开始地址.GetLength()){
		MessageBox(_T("开始地址不能为空"));
		return;
	}
	if(0 == W_结束地址.GetLength()){
		MessageBox(_T("结束地址不能为空"));
		return;
	}

	static CWQSGFileDialog_Open fopendlg(_T("ROM文件(*.*)|*.*||"));
	fopendlg.m_ofn.lpstrTitle = _T("选择一个ROM...");
	if(IDOK == fopendlg.DoModal())
	{
		W_ROM = fopendlg.GetPathName();
		INT 位置 = W_列表下一个位置;
		if(W_列表下一个位置)
		{
			位置--;
		}
		TCHAR 编号[6];
		::_stprintf_s(编号,6,_T("%05d"),W_列表下一个位置);
		W_列表->InsertItem(位置,编号);
		W_列表->SetItemText(位置,1,W_ROM);
		W_列表->SetItemText(位置,2,W_默认码表);
		W_列表->SetItemText(位置,3,W_默认控制码表);
		W_列表->SetItemText(位置,4,W_开始地址);
		W_列表->SetItemText(位置,5,W_结束地址);
		if(W_验证)
		{
			W_列表->SetItemText(位置,6,_T("○"));
		}
		else
		{
			W_列表->SetItemText(位置,6,_T("×"));
		}
		W_列表->SetItemText(位置,7,W_MIN);
		W_列表->SetItemText(位置,8,W_MAX);
		
		W_列表下一个位置++;
		UpdateData(FALSE);
		保存项目表(W_数据文件地址);
	}
}
void 导出批处理::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION P = W_列表->GetFirstSelectedItemPosition();
	if(NULL != P)
	{
		INT 索引 = (INT)P - 1;
		W_列表->DeleteItem(索引);
		W_列表下一个位置--;
		WCHAR 编码[6];
		INT tmp;
		for(;索引 < (W_列表下一个位置 - 1);索引++)
		{
			tmp = ::_ttoi(W_列表->GetItemText(索引,0)) - 1;
			::swprintf_s(编码,6,L"%05d",tmp);
			W_列表->SetItemText(索引,0,编码);
		}
	}
	保存项目表(W_数据文件地址);
}

void 导出批处理::OnLvnItemActivateListW(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION P = W_列表->GetFirstSelectedItemPosition();
	if(	NULL != P )
	{
		UpdateData();
		INT 位置 = (INT)P - 1;
		W_默认码表 = W_列表->GetItemText(位置,2);
		W_默认控制码表 = W_列表->GetItemText(位置,3);
		W_开始地址 = W_列表->GetItemText(位置,4);
		W_结束地址 = W_列表->GetItemText(位置,5);
		W_MIN = W_列表->GetItemText(位置,6);
		if(W_MIN == _T("○"))
		{
			W_验证 = TRUE;
		}
		else
		{
			W_验证 = FALSE;
		}
		W_MIN = W_列表->GetItemText(位置,7);
		W_MAX = W_列表->GetItemText(位置,8);
		UpdateData(FALSE);
	}
	*pResult = 0;
}
void 导出批处理::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(0 == W_默认码表.GetLength())
	{
		MessageBox(_T("码表地址不能为空"));
		return;
	}
	if(0 == W_默认控制码表.GetLength())
	{
		MessageBox(_T("控制码表地址不能为空"));
		return;
	}
	if(0 == W_开始地址.GetLength())
	{
		MessageBox(_T("开始地址不能为空"));
		return;
	}
	if(0 == W_结束地址.GetLength())
	{
		MessageBox(_T("结束地址不能为空"));
		return;
	}

	POSITION P = W_列表->GetFirstSelectedItemPosition();
	if(	NULL != P )
	{
		UpdateData();
		INT 位置 = (INT)P - 1;

		W_列表->SetItemText(位置,2,W_默认码表);
		W_列表->SetItemText(位置,3,W_默认控制码表);
		W_列表->SetItemText(位置,4,W_开始地址);
		W_列表->SetItemText(位置,5,W_结束地址);
		if(W_验证)
		{
			W_列表->SetItemText(位置,6,_T("○"));
		}
		else
		{
			W_列表->SetItemText(位置,6,_T("×"));
		}
		W_列表->SetItemText(位置,7,W_MIN);
		W_列表->SetItemText(位置,8,W_MAX);

		UpdateData(FALSE);
		保存项目表(W_数据文件地址);
	}
}
void 导出批处理::OnEnChangeEditTbl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_默认码表.Left(1))
		|| (_T('　') == W_默认码表.Left(1))
		)
	{
		W_默认码表.TrimLeft(_T(' '));
		W_默认码表.TrimLeft(_T('　'));
	}
	UpdateData(FALSE);
}

void 导出批处理::OnEnChangeEditTbl2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_默认控制码表.Left(1))
		|| (_T('　') == W_默认控制码表.Left(1))
		)
	{
		W_默认控制码表.TrimLeft(_T(' '));
		W_默认控制码表.TrimLeft(_T('　'));
	}
	UpdateData(FALSE);
}

void 导出批处理::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString 项目编号,ROM,TBL,TBL2,TXT;
	u64 KS,JS;
	INT 验证;
	UINT MIN,MAX;

	::WQSG_普通导出任务 WQSG(m_hWnd,L"WQSG 批量导出");
	INT 总数 = W_列表->GetItemCount();

	for(INT I = 0;I < 总数;I++)
	{
		if(W_列表->GetCheck(I))
		{
			项目编号 = W_列表->GetItemText(I,0);
			ROM = W_列表->GetItemText(I,1);
			TBL = W_列表->GetItemText(I,2);
			TBL2 = W_列表->GetItemText(I,3);

			KS = JS = 0;
			TXT = W_列表->GetItemText(I,4);
			_stscanf_s(TXT,_T("%x"),&KS);

			TXT = W_列表->GetItemText(I,5);
			_stscanf_s(TXT,_T("%x"),&JS);

			TXT = W_列表->GetItemText(I,6);
			if(_T("○") == TXT)
			{
				验证 = 1;
			}
			else
			{
				验证 = 0;
			}
			TXT = W_列表->GetItemText(I,7);
			MIN = ::_ttoi(TXT);

			TXT = W_列表->GetItemText(I,8);
			MAX = ::_ttoi(TXT);

			TXT = ROM;
			TXT += _T(".TXT");

			WQSG.添加(项目编号.GetBuffer(),ROM.GetBuffer(),TBL.GetBuffer(),TBL2.GetBuffer(),TXT.GetBuffer(),KS,JS,验证,MIN,MAX);
		}
	}
	WQSG.导出();
}
