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
// TXT_OutBox.cpp : 实现文件
#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "TXT_OutBox.h"
#include "WQSG_MAIN.h"

#include "WQSG_DirDlg.h"
#define WM_WQSG_线程信息	( WM_USER + 1 )
#define WM_WQSG_线程LOG		( WM_WQSG_线程信息 + 1 )

#define	DEF_V_export L"export"
BOOL CTXT_OutBox::zzz普通导出( CString& 文件名 , CWQSG_TXT_O& WQSG , tg参数& 参数 )
{
	CString 文本pathName;
	if( 参数.m_TXT_DIR.GetLength() <= 0 )
	{
		文本pathName = 文件名.Left( 文件名.ReverseFind(L'\\') + 1 ) + 文件名.Mid( 文件名.ReverseFind(L'\\') + 1 ) + L".TXT";
	}
	else
	{
		WQSG_CreateDir( 参数.m_TXT_DIR.GetString() );
		文本pathName = 参数.m_TXT_DIR + 文件名.Mid( 文件名.ReverseFind(L'\\') + 1 ) + L".TXT";
	}
	return WQSG.导出文本( 文件名.GetString() , 文本pathName.GetString() , 参数.m_KS , 参数.m_JS , 参数.m_min , 参数.m_max );
}
// CTXT_OutBox 对话框
DWORD WINAPI CTXT_OutBox::普通导出_文件(LPVOID lpParameter)
{
	tg参数& 参数 = *(tg参数*)lpParameter;

	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"启动文件模式" );
///--------------------------
	CWQSG_TXT_O WQSG;
	CString 文件;
///-------------------------
	HiResTimer RunTime;
	if( !WQSG.LoadTbl( 参数.m_TBL.GetString() , (参数.m_TBL2.GetLength() < 4)?NULL:参数.m_TBL2.GetString() , 参数.m_验证 ) )
		goto __gt退出;

	RunTime.Start();
	for( std::vector<CStringW>::iterator it = 参数.m_Files.begin() ; it != 参数.m_Files.end() ; ++it )
	{
		文件 = *it;
		zzz普通导出( 文件 , WQSG , 参数 );
		/*
		if( !zzz普通导出( 文件 , WQSG , 参数 ) )
			goto __gt退出;
			*/
	}
	RunTime.Stop();
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)str.GetString() );
	}
__gt退出:
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"关闭文件模式" );
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程信息 , 0 , 0 );
	return 0;
}
BOOL CTXT_OutBox::zzz_普通导出_文件夹( CStringW 路径 , tg参数& 参数 , CWQSG_TXT_O& WQSG , INT& countAll , std::vector<CStringW>& szExt )
{
	if( 路径.Right(1) != L'\\')
		路径 += L'\\';

	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)( L"进入 " + 路径 ).GetString() );

	WIN32_FIND_DATAW FindFileData;
	const HANDLE hFind = ::FindFirstFileW( 路径 + L"*", &FindFileData );
	if ( hFind != INVALID_HANDLE_VALUE )
	{
		INT count = 0;
		do
		{
			CStringW fileName( FindFileData.cFileName );
			if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( ( fileName != L"." ) && ( fileName != L".." ) && 参数.m_SubDir )
				{
					zzz_普通导出_文件夹( 路径 + FindFileData.cFileName , 参数 , WQSG , count , szExt );
				/*	if( !zzz_普通导出_文件夹( 路径 + FindFileData.cFileName , 参数 , WQSG , count , szExt ) )
						return FALSE;*/

					::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)( L"返回 " + 路径 ).GetString() );
				}
			}
			else
			{
				for( int i = 0 ; (i >= 0) && ( (size_t)i < szExt.size() ) ; ++i )
				{
					if( fileName.Right( szExt[i].GetLength() ).MakeUpper() == szExt[i] )
					{
						if( zzz普通导出( 路径 + FindFileData.cFileName , WQSG , 参数 ) )
						{
							++count;
							++countAll;
						}
						/*if( !zzz普通导出( 路径 + FindFileData.cFileName , WQSG , 参数 ) )
							return FALSE;
						++count;
						++countAll;*/
						break;
					}
				}
			}
		}
		while( ::FindNextFile( hFind , &FindFileData ) );

		::FindClose( hFind );
		路径.Format( L"导出 %u 个文件" , count );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)路径.GetString() );
	}
	else 
	{
		GetLastError();
	}
	return TRUE;
}
DWORD WINAPI CTXT_OutBox::普通导出_文件夹(LPVOID lpParameter)
{
	tg参数& 参数 = *(tg参数*)lpParameter;
///-------------------------------------------------
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"启动文件夹模式" );

	if( 参数.m_TXT_DIR.GetLength() > 0 )
	{
		if( 参数.m_TXT_DIR.Right(1) != L'\\')
			参数.m_TXT_DIR += L'\\';
	}

	CWQSG_TXT_O WQSG;
///-------------------------------------------------
	HiResTimer RunTime;
	std::vector<CStringW> szExt;
	INT count = 0;

	if( !WQSG.LoadTbl( 参数.m_TBL.GetString() , (参数.m_TBL2.GetLength() < 4)?NULL:参数.m_TBL2.GetString() , 参数.m_验证 ) )
		goto __gt退出;
	//
	分解Ext( 参数.m_Ext , szExt );
	if( szExt.size() == 0 )
		szExt.push_back( CStringW(L"") );
	//
	RunTime.Start();
	zzz_普通导出_文件夹( 参数.m_ROM_DIR , 参数 , WQSG , count , szExt );
	RunTime.Stop();
	//
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)str.GetString() );

		str.Format( L"一共成功导出 %u 个文件" , count );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)str.GetString() );
	}
__gt退出:
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"关闭文件夹模式" );
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程信息 , 0 , 0 );
	return 0;
}
IMPLEMENT_DYNAMIC(CTXT_OutBox, CDialog)
CTXT_OutBox::CTXT_OutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CTXT_OutBox::IDD, pParent)
	, m_EDIT_Rom(_T(""))
	, m_EDIT_TXT_DIR(_T(""))
	, m_EDIT_TBL(_T(""))
	, m_EDIT_TBL2(_T(""))
	, m_EDIT_min(_T("2"))
	, m_EDIT_max(_T("99999"))
	, m_EDIT_段号(_T("0"))
	, m_EDIT_开始偏移(_T("0"))
	, m_EDIT_结束偏移(_T("FFFFFFFF"))
	, m_EDIT_EXT(_T(""))
	, m_LOG(_T(""))
	, m_NodeName(_T(""))
{
}

CTXT_OutBox::~CTXT_OutBox()
{
}

void CTXT_OutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_C从目录导出);
	DDX_Text(pDX, IDC_EDIT_ROM, m_EDIT_Rom);
	DDV_MaxChars(pDX, m_EDIT_Rom, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TXT_DIR, m_EDIT_TXT_DIR);
	DDV_MaxChars(pDX, m_EDIT_TXT_DIR, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL, m_EDIT_TBL);
	DDV_MaxChars(pDX, m_EDIT_TBL, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL2, m_EDIT_TBL2);
	DDV_MaxChars(pDX, m_EDIT_TBL2, MAX_PATH);
	DDX_Control(pDX, IDC_CHKTBL, m_C验证TBL);
	DDX_Text(pDX, IDC_EDIT_MIN, m_EDIT_min);
	DDV_MaxChars(pDX, m_EDIT_min, 5);
	DDX_Text(pDX, IDC_EDIT_MAX, m_EDIT_max);
	DDV_MaxChars(pDX, m_EDIT_max, 5);
	DDX_Text(pDX, IDC_EDIT_ADDR_H, m_EDIT_段号);
	DDV_MaxChars(pDX, m_EDIT_段号, 8);
	DDX_Text(pDX, IDC_EDIT_ADDR_KS_L, m_EDIT_开始偏移);
	DDV_MaxChars(pDX, m_EDIT_开始偏移, 8);
	DDX_Text(pDX, IDC_EDIT_ADDR_JS_L, m_EDIT_结束偏移);
	DDV_MaxChars(pDX, m_EDIT_结束偏移, 8);
	DDX_Text(pDX, IDC_EDIT_EXT, m_EDIT_EXT);
	DDV_MaxChars(pDX, m_EDIT_EXT, 100);
	DDX_Control(pDX, IDC_EDIT_LOG, m_CEDIT_LOG);
	DDX_Control(pDX, IDC_CHECK4, m_C使用控制码表);
	DDX_Control(pDX, IDC_LIST2, m_CList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_NodeName);
	DDV_MaxChars(pDX, m_NodeName, 20);
	DDX_Control(pDX, IDC_CHECK3, m_C文本在同目录);
	DDX_Control(pDX, IDC_CHECK5, m_c包括子目录);
}


BEGIN_MESSAGE_MAP(CTXT_OutBox, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTXT_OutBox::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_TXT_DIR, &CTXT_OutBox::OnBnClickedButtonTxtDir)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &CTXT_OutBox::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &CTXT_OutBox::OnBnClickedButtonTbl2)
	ON_BN_CLICKED(IDC_CHECK1, &CTXT_OutBox::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT_ADDR_H, &CTXT_OutBox::OnEnChangeEditAddrH)
	ON_EN_CHANGE(IDC_EDIT_ADDR_KS_L, &CTXT_OutBox::OnEnChangeEditAddrKsL)
	ON_EN_CHANGE(IDC_EDIT_ADDR_JS_L, &CTXT_OutBox::OnEnChangeEditAddrJsL)
	ON_EN_KILLFOCUS(IDC_EDIT_ADDR_H, &CTXT_OutBox::OnEnKillfocusEditAddrH)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN, &CTXT_OutBox::OnEnKillfocusEditMin)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX, &CTXT_OutBox::OnEnKillfocusEditMax)
	ON_MESSAGE( WM_WQSG_线程信息 , &CTXT_OutBox::线程信息 )
	ON_MESSAGE( WM_WQSG_线程LOG , &CTXT_OutBox::线程LOG )
	ON_BN_CLICKED(IDC_BUTTON_ROM, &CTXT_OutBox::OnBnClickedButtonRom)
	ON_EN_CHANGE(IDC_EDIT_EXT, &CTXT_OutBox::OnEnChangeEditExt)
	ON_BN_CLICKED(IDC_BUTTON1, &CTXT_OutBox::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK4, &CTXT_OutBox::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_BUTTON_Add, &CTXT_OutBox::OnBnClickedButtonAdd)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTXT_OutBox::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON_Del, &CTXT_OutBox::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_Edit, &CTXT_OutBox::OnBnClickedButtonEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CTXT_OutBox::OnEnKillfocusEditName)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CTXT_OutBox::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_CHECK3, &CTXT_OutBox::OnBnClickedCheck3)
END_MESSAGE_MAP()
void CTXT_OutBox::OnOK(){}
void CTXT_OutBox::OnCancel(){}
void CTXT_OutBox::OnClose()
{
	CDialog::OnClose();
	CDialog::OnCancel();
}

//--------------------------------------------------------
void CTXT_OutBox::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if( m_C文本在同目录.GetCheck() != 0 )
	{
		m_参数.m_TXT_DIR = L"";
	}
	else
	{
		if( !::WQSG_IsDir( m_EDIT_TXT_DIR.GetString() ) )
		{
			MessageBoxW( L"文本目录出错" );
			return;
		}
		m_参数.m_TXT_DIR = m_EDIT_TXT_DIR;
		if( m_参数.m_TXT_DIR.Right(1) != L'\\')
			m_参数.m_TXT_DIR += L'\\';
	}
	if( !::WQSG_IsFile( m_EDIT_TBL.GetString() ) )
	{
		MessageBoxW( L"请检查码表路径" );
		return;
	}
	if( (m_C使用控制码表.GetCheck() != 0 ) && !::WQSG_IsFile( m_EDIT_TBL2.GetString() ) )
	{
		MessageBoxW( L"请检查控制码表路径" );
		return;
	}
	m_参数.m_验证 = m_C验证TBL.GetCheck() != 0;
	m_参数.m_SubDir = m_c包括子目录.GetCheck() != 0;
	m_参数.m_TBL = m_EDIT_TBL;
	m_参数.m_TBL2 = (m_C使用控制码表.GetCheck() != 0)?m_EDIT_TBL2:L"";

	m_参数.m_Files.clear();

	u32 val;
	if ( 1 != swscanf( m_EDIT_段号 , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"段号\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_参数.m_KS = val;
	m_参数.m_KS <<= 32;
	m_参数.m_JS = m_参数.m_KS;

	if ( 1 != swscanf( m_EDIT_开始偏移 , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"开始偏移\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_参数.m_KS |= val;

	if ( 1 != swscanf( m_EDIT_结束偏移 , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"结束偏移\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_JS_L )->SetFocus();
		return;
	}
	m_参数.m_JS |= val;

	m_参数.m_min = _wtoi( m_EDIT_min.GetString() );
	m_参数.m_max = _wtoi( m_EDIT_max.GetString() );

	HANDLE handle;
	if( m_C从目录导出.GetCheck() != 0 )
	{
		if( !验证Ext( m_EDIT_EXT ) )
		{
			MessageBox( L"扩展名错误");
			return;
		}
		m_参数.m_Ext = ( m_EDIT_EXT.GetLength() > 0 )?m_EDIT_EXT:L"*";

		m_参数.m_ROM_DIR = m_EDIT_Rom;
		if( m_参数.m_ROM_DIR.Right(1) != L'\\')
			m_参数.m_ROM_DIR += L'\\';

		SendMessage( WM_WQSG_线程信息 , 0 , (LPARAM)L"正在导出..." );
		handle = CreateThread( NULL , NULL
		, 普通导出_文件夹
		, &m_参数 , NULL , NULL );
	}
	else
	{
		

		CWQSGFileDialog fileDlg ( TRUE , NULL , NULL , OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , L"ROM文件(*.*)|*.*|" );
		fileDlg.m_ofn.lpstrTitle = L"选择要导出文本的文件(多选)...";
		fileDlg.m_ofn.lpstrFile = m_文件列表缓存.GetBuffer();
		fileDlg.m_ofn.nMaxFile = 65535;
		fileDlg.m_ofn.lpstrInitialDir = m_参数.m_LastDir;

		if( fileDlg.DoModal() != IDOK )
			return;

		m_参数.m_LastDir = fileDlg.GetFolderPath();

		POSITION pos = fileDlg.GetStartPosition();
		while( pos )
		{
			m_参数.m_Files.push_back( fileDlg.GetNextPathName( pos ) );
		}

		SendMessage( WM_WQSG_线程信息 , 0 , (LPARAM)L"正在导出..." );

		handle = CreateThread( NULL , NULL
			, 普通导出_文件
			, &m_参数 , NULL , NULL );
	}
	if( handle != NULL )
	{
		CloseHandle( handle );
	}
	else
		SendMessage( WM_WQSG_线程信息 , 0 , 0 );
}
//--------------------------------------------------------
void CTXT_OutBox::OnBnClickedButtonTxtDir()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXT_DIR = path;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CString strPath;

	CWQSGFileDialog dlg( TRUE );
	dlg.m_ofn.lpstrFilter = L"码表文件(*.TBL,*.TXT)\0*.TBL;*.TXT\0\0";
	dlg.m_ofn.lpstrInitialDir = strPath;

	if( IDOK != dlg.DoModal() )
		return;

	strPath = dlg.GetFolderPath();

	m_EDIT_TBL = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CString strPath;

	CWQSGFileDialog dlg( TRUE );
	dlg.m_ofn.lpstrFilter = L"控制码表文件(*.TBL,*.TXT)\0*.TBL;*.TXT\0\0";
	dlg.m_ofn.lpstrInitialDir = strPath;

	if( IDOK != dlg.DoModal() )
		return;

	strPath = dlg.GetFolderPath();

	m_EDIT_TBL2 = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	const BOOL 选中 = m_C从目录导出.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( 选中 );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( 选中 );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( 选中 );
}

BOOL CTXT_OutBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_参数.m_hWnd = m_hWnd;
	if( NULL == m_文件列表缓存.GetBuffer(65536) )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_C验证TBL.SetCheck(TRUE);

	m_C文本在同目录.SetCheck( TRUE );
	OnBnClickedCheck3();

	载入配置();

	return TRUE;
}

void CTXT_OutBox::OnEnChangeEditAddrH()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( m_EDIT_段号 , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_H ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrKsL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( m_EDIT_开始偏移 , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_KS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrJsL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( m_EDIT_结束偏移 , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_JS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnKillfocusEditAddrH()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_段号.GetLength() == 0)
	{
		m_EDIT_段号 = L"0";
		UpdateData( FALSE );
	}
}

void CTXT_OutBox::OnEnKillfocusEditMin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_min.GetLength() == 0)
	{
		m_EDIT_min = L"0";
		UpdateData( FALSE );
	}
}

void CTXT_OutBox::OnEnKillfocusEditMax()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_max.GetLength() == 0)
	{
		m_EDIT_max = L"9999";
		UpdateData( FALSE );
	}
}
LRESULT CTXT_OutBox::线程信息( WPARAM 保留 , LPARAM 文本 )
{
	if( 0 == 文本 )
	{
		AppLog( (WCHAR*)文本 );
//		MessageBox(L"导出完毕!!");
	}

	WQSG_MAIN_CWND->SendMessage( WM_WQSG_设置文本 , 0 , 文本 );
	return 0;
}

LRESULT CTXT_OutBox::线程LOG( WPARAM 保留 , LPARAM 文本 )
{
	AppLog( (WCHAR*)文本 );
	WQSG_MAIN_CWND->SendMessage( WM_WQSG_设置LOG文本 , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}
void CTXT_OutBox::OnBnClickedButtonRom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_Rom = path;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnEnChangeEditExt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	删除首空( m_EDIT_EXT );
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LOG = L"";
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_OutBox::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_C使用控制码表.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
}


void CTXT_OutBox::载入配置(void)
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	if( !WQSG_ini.SelConfigName( DEF_V_export ) )
		return ;

	CString str;
	for( long count = WQSG_ini.GetItemCount() , i = 0;
		i < count ; ++i )
	{
		if( WQSG_ini.GetAppName( i , str ) )
			m_CList.AddString( str );
	}
}

void CTXT_OutBox::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_NodeName.GetLength() <= 0 )
	{
		return;
	}
	int count = m_CList.GetCount();
	CString str;
	for( int i = 0 ; i < count; ++i )
	{
		m_CList.GetText( i , str );
		if( str == m_NodeName )
		{
			MessageBox( L"已经存在相同的名称");
			return;
		}
	}
	if( 写配置() )
	{
		m_CList.SetCurSel( m_CList.AddString( m_NodeName ) );
	}
}

BOOL CTXT_OutBox::写配置( )
{
	UpdateData();
	if( !WQSG_ini.SelConfigName( DEF_V_export ) )
	{
		MessageBox( L"选择节点失败" );
		return FALSE;
	}
#define DEF_FN_XYZ( __DEF_x , __DEF_y ) \
	if( !WQSG_ini.SetApp( m_NodeName , __DEF_x , __DEF_y )	){	MessageBox( L"添加失败" );	return FALSE;	}

	DEF_FN_XYZ( L"ROMPath" , m_EDIT_Rom )
	DEF_FN_XYZ( L"TXTPath" , m_EDIT_TXT_DIR )
	DEF_FN_XYZ( L"TBLPathName" , m_EDIT_TBL )
	DEF_FN_XYZ( L"TBL2PathName" , m_EDIT_TBL2 )
	DEF_FN_XYZ( L"ExtName" , m_EDIT_EXT )
	DEF_FN_XYZ( L"SegmentAddr" , m_EDIT_段号 )
	DEF_FN_XYZ( L"BeginOffset" , m_EDIT_开始偏移 )
	DEF_FN_XYZ( L"EndOffset" , m_EDIT_结束偏移 )
	DEF_FN_XYZ( L"MinLen" , m_EDIT_min )
	DEF_FN_XYZ( L"MaxLen" , m_EDIT_max )
	DEF_FN_XYZ( L"UseDirectory" , (m_C从目录导出.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"CheckTblOverlap" , (m_C验证TBL.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"UseTBL2" , (m_C使用控制码表.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"TxtDirDefault" , (m_C文本在同目录.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"SubDir" , (m_c包括子目录.GetCheck()!=0)?L"1":L"0" )
#undef DEF_FN_XYZ

	if( !WQSG_ini.Save( WQSG_iniSavePathName ) )
	{
		MessageBox( L"保存文件失败" );
		return FALSE;
	}

	return TRUE;
}

void CTXT_OutBox::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )return;

	if( !WQSG_ini.SelConfigName( DEF_V_export ) )
	{
		MessageBox( L"选择节点失败" );
		return ;
	}
	CString NodeName;
	m_CList.GetText( sel , NodeName );

#define DEF_FN_XYZ( __DEF_x , __DEF_y , __DEF_z ) WQSG_ini.GetApp( NodeName , __DEF_x , __DEF_y , __DEF_z )

	DEF_FN_XYZ( L"ROMPath" , m_EDIT_Rom , L"" );
	DEF_FN_XYZ( L"TXTPath" , m_EDIT_TXT_DIR , L"" );
	DEF_FN_XYZ( L"TBLPathName" , m_EDIT_TBL , L"" );
	DEF_FN_XYZ( L"TBL2PathName" , m_EDIT_TBL2 , L"" );
	DEF_FN_XYZ( L"ExtName" , m_EDIT_EXT , L"" );
	DEF_FN_XYZ( L"SegmentAddr" , m_EDIT_段号 , L"0" );
	DEF_FN_XYZ( L"BeginOffset" , m_EDIT_开始偏移 , L"" );
	DEF_FN_XYZ( L"EndOffset" , m_EDIT_结束偏移 , L"" );
	DEF_FN_XYZ( L"MinLen" , m_EDIT_min , L"2" );
	DEF_FN_XYZ( L"MaxLen" , m_EDIT_max , L"9999" );
	CString str;

	DEF_FN_XYZ( L"UseDirectory" , str , L"0" );
	m_C从目录导出.SetCheck( 0 != ::_wtoi( str.GetString() ) );
	UpdateData( FALSE );
	OnBnClickedCheck1();

	DEF_FN_XYZ( L"CheckTblOverlap" , str , L"1" );
	m_C验证TBL.SetCheck( 0 != ::_wtoi( str.GetString() ) );

	DEF_FN_XYZ( L"UseTBL2" , str , L"0" );
	m_C使用控制码表.SetCheck( 0 != ::_wtoi( str.GetString() ) );
	UpdateData( FALSE );
	OnBnClickedCheck4();

	DEF_FN_XYZ( L"TxtDirDefault" , str , L"0" );
	m_C文本在同目录.SetCheck( 0 != ::_wtoi( str.GetString() ) );
	UpdateData( FALSE );
	OnBnClickedCheck3();

	DEF_FN_XYZ( L"SubDir" , str , L"0" );
	m_c包括子目录.SetCheck( 0 != ::_wtoi( str.GetString() ) );
#undef DEF_FN_XYZ
	m_NodeName = NodeName;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"抽~~~~还没选择怎么删" );
		return;
	}

	if( !WQSG_ini.SelConfigName( DEF_V_export ) )
	{
		MessageBox( L"选择节点失败" );
		return ;
	}

	CString NodeName;
	m_CList.GetText( sel , NodeName );

	m_CList.DeleteString( sel );
	m_CList.SetCurSel( -1 );

	WQSG_ini.DelApp( NodeName );

	if( !WQSG_ini.Save( WQSG_iniSavePathName ) )
	{
		MessageBox( L"保存文件失败" );
	}
}

void CTXT_OutBox::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"找抽~~~~还没选择怎么改" );
		return;
	}

	if( !WQSG_ini.SelConfigName( DEF_V_export ) )
	{
		MessageBox( L"选择节点失败" );
		return ;
	}

	CString NodeName;
	m_CList.GetText( sel , NodeName );
	if( NodeName != m_NodeName )
	{
		int count = m_CList.GetCount();
		CString str;
		for( int i = 0 ; i < count; ++i )
		{
			m_CList.GetText( i , str );
			if( str == m_NodeName )
			{
				MessageBox( L"已经存在相同的名称");
				return;
			}
		}
		if( !WQSG_ini.修改AppName( NodeName , m_NodeName ) )
		{
			MessageBox( L"修改失败" );
			return ;
		}
		m_CList.DeleteString( sel );
		m_CList.InsertString( sel , m_NodeName );
		if( !WQSG_ini.Save( WQSG_iniSavePathName ) )
		{
			MessageBox( L"保存文件失败" );
		}
	}
	else if( !写配置() )
	{
	}
}

void CTXT_OutBox::OnEnKillfocusEditName()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( FALSE );
}

void CTXT_OutBox::OnEnChangeEditName()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	m_NodeName.Trim( L' ' );
	m_NodeName.Trim( L'　' );

	WCHAR wCh;
	BOOL 提示 = FALSE;
	for(int i = 0 ; wCh = m_NodeName.GetAt( i ) ; )
	{
		if(
			( wCh >= L'0' && wCh <= L'9' ) ||
			( wCh >= L'a' && wCh <= L'z' ) ||
			( wCh >= L'A' && wCh <= L'Z' ) ||
			( wCh == L'_' )
			)
		{
			++i;
		}
		else
		{
			提示 = TRUE;
			m_NodeName.Delete( i );
		}
	}
	if( 提示 )
		AppLog( L"名称只能使用 字母 数字  下划线 以及 全角字符" );


	if( ( ( wCh = m_NodeName.GetAt( 0 ) ) >= L'0' ) && ( wCh <= L'9' ) )
	{
		while( ( ( wCh = m_NodeName.GetAt( 0 ) ) >= L'0' ) && ( wCh <= L'9' ) )
		{
			m_NodeName.Delete( 0 );
		}
		AppLog( L"名称不能以数字开头" );
	}

	UpdateData( FALSE );
	((CEdit*)GetDlgItem( IDC_EDIT_NAME ))->SetSel( m_NodeName.GetLength() , m_NodeName.GetLength() ,TRUE );
}

void CTXT_OutBox::AppLog(CString str)
{
	m_LOG += (str + L"\r\n");
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_OutBox::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem( IDC_EDIT_TXT_DIR )->EnableWindow( m_C文本在同目录.GetCheck() == 0 );
}
