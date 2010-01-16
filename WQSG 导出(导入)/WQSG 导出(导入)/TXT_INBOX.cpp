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
// TXT_INBOX.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "TXT_INBOX.h"

#include "WQSG_MAIN.h"

#include "WQSG_DirDlg.h"

#define WM_WQSG_线程信息	( WM_USER + 1 )
#define WM_WQSG_线程LOG		( WM_WQSG_线程信息 + 1 )

#define	DEF_V_import L"import"
BOOL CTXT_INBOX::zzz普通导入( CString& 文件名 , WQSG_TXT_I& WQSG , tg参数& 参数 )
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

	return WQSG.导入文本( 文件名.GetString() , 文本pathName.GetString() , ( (参数.m_单字节)?(&参数.m_SP1):NULL ) , ( (参数.m_双字节)?(&参数.m_SP2):NULL ) , 参数.m_长度不足提示 );
}
// CTXT_INBOX 对话框
DWORD WINAPI CTXT_INBOX::普通导入_文件(LPVOID lpParameter)
{
	tg参数& 参数 = *(tg参数*)lpParameter;
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"启动文件模式" );

///--------------------------
	WQSG_TXT_I WQSG;
	CString 文件;
///-------------------------
	HiResTimer RunTime;
	if( !WQSG.LoadTbl( 参数.m_TBL.GetString() , (参数.m_TBL2.GetLength() < 4)?NULL:参数.m_TBL2.GetString() , 参数.m_验证 ) )
		goto __gt退出;

	RunTime.Start();
	for( std::vector<CStringW>::iterator it = 参数.m_Files.begin() ; it != 参数.m_Files.end() ; ++it )
	{
		文件 = *it;
		zzz普通导入( 文件 , WQSG , 参数 );
		/*
		if( !zzz普通导入( 文件 , WQSG , 参数 ) )
			break;*/
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
BOOL CTXT_INBOX::zzz_普通导入_文件夹( CStringW 路径 , tg参数& 参数 , WQSG_TXT_I& WQSG , INT& countAll , std::vector<CStringW>& szExt )
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
					zzz_普通导入_文件夹( 路径 + FindFileData.cFileName , 参数 , WQSG , count , szExt );
/*
					if( !zzz_普通导入_文件夹( 路径 + FindFileData.cFileName , 参数 , WQSG , count , szExt ) )
						return FALSE;
*/
					::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)( L"返回 " + 路径 ).GetString() );
				}
			}
			else
			{
				for( int i = 0 ; (i >= 0) && ( (size_t)i < szExt.size() ) ; ++i )
				{
					if( fileName.Right( szExt[i].GetLength() ).MakeUpper() == szExt[i] )
					{
						if( zzz普通导入( 路径 + FindFileData.cFileName , WQSG , 参数 ) )
						{
							++count;
							++countAll;
						}
						/*
						if( !zzz普通导入( 路径 + FindFileData.cFileName , WQSG , 参数 ) )
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
		路径.Format( L"导入 %u 个文件" , count );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)路径.GetString() );
	}
	else 
	{
		GetLastError();
	}
	return TRUE;
}
DWORD WINAPI CTXT_INBOX::普通导入_文件夹(LPVOID lpParameter)
{
	tg参数& 参数 = *(tg参数*)lpParameter;
///-------------------------------------------------
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"启动文件夹模式" );

	if( 参数.m_TXT_DIR.GetLength() > 0 )
	{
		if( 参数.m_TXT_DIR.Right(1) != L'\\')
			参数.m_TXT_DIR += L'\\';
	}

	WQSG_TXT_I WQSG;
	HiResTimer RunTime;
	std::vector<CStringW> szExt;
	INT count = 0;
///-------------------------------------------------
	if( !WQSG.LoadTbl( 参数.m_TBL.GetString() , (参数.m_TBL2.GetLength() < 4)?NULL:参数.m_TBL2.GetString() , 参数.m_验证 ) )
		goto __gt退出;

	分解Ext( 参数.m_Ext , szExt );
	if( szExt.size() == 0 )
		szExt.push_back( CStringW(L"") );
	//
	RunTime.Start();
	zzz_普通导入_文件夹( 参数.m_ROM_DIR , 参数 , WQSG , count , szExt );
	RunTime.Stop();
	//
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)str.GetString() );

		str.Format( L"一共成功导入 %u 个文件" , count );
		::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)str.GetString() );
	}
__gt退出:
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程LOG , 0 , (LPARAM)L"关闭文件夹模式" );
	::SendMessage( 参数.m_hWnd , WM_WQSG_线程信息 , 0 , 0 );
	return 0;
}
IMPLEMENT_DYNAMIC(CTXT_INBOX, CDialog)
CTXT_INBOX::CTXT_INBOX(CWnd* pParent /*=NULL*/)
	: CDialog(CTXT_INBOX::IDD, pParent)
	, m_EDIT_SP1(_T("20"))
	, m_EDIT_SP2(_T("8140"))
	, m_EDIT_EXT(_T(""))
	, m_NodeName(_T(""))
{

}

CTXT_INBOX::~CTXT_INBOX()
{
}

void CTXT_INBOX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROM, m_EDIT_Rom);
	DDV_MaxChars(pDX, m_EDIT_Rom, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TXT_DIR, m_EDIT_TXT_DIR);
	DDV_MaxChars(pDX, m_EDIT_TXT_DIR, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL, m_EDIT_TBL);
	DDV_MaxChars(pDX, m_EDIT_TBL, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL2, m_EDIT_TBL2);
	DDV_MaxChars(pDX, m_EDIT_TBL2, MAX_PATH);
	DDX_Control(pDX, IDC_CHKTBL, m_C验证TBL);
	DDX_Control(pDX, IDC_CHECK1, m_C从目录导出);
	DDX_Control(pDX, IDC_CHECK2, m_C长度不足中止);
	DDX_Control(pDX, IDC_COMBO1, m_C选择填充);
	DDX_Text(pDX, IDC_EDIT_SP1, m_EDIT_SP1);
	DDV_MaxChars(pDX, m_EDIT_SP1, 2);
	DDX_Text(pDX, IDC_EDIT_SP2, m_EDIT_SP2);
	DDV_MaxChars(pDX, m_EDIT_SP2, 4);
	DDX_Control(pDX, IDC_CHECK4, m_C使用控制码表);
	DDX_Text(pDX, IDC_EDIT_EXT, m_EDIT_EXT);
	DDV_MaxChars(pDX, m_EDIT_EXT, 100);
	DDX_Control(pDX, IDC_EDIT_LOG, m_CEDIT_LOG);
	DDX_Text(pDX, IDC_EDIT_NAME, m_NodeName);
	DDV_MaxChars(pDX, m_NodeName, 20);
	DDX_Control(pDX, IDC_LIST2, m_CList);
	DDX_Control(pDX, IDC_CHECK3, m_C文本在同目录);
	DDX_Control(pDX, IDC_CHECK5, m_c包括子目录);
}
BEGIN_MESSAGE_MAP(CTXT_INBOX, CDialog)
	ON_BN_CLICKED(IDC_CHECK2, &CTXT_INBOX::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_ROM, &CTXT_INBOX::OnBnClickedButtonRom)
	ON_BN_CLICKED(IDC_BUTTON_TXT_DIR, &CTXT_INBOX::OnBnClickedButtonTxtDir)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &CTXT_INBOX::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &CTXT_INBOX::OnBnClickedButtonTbl2)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTXT_INBOX::OnBnClickedButtonStart)
	ON_MESSAGE( WM_WQSG_线程信息 , &CTXT_INBOX::线程信息 )
	ON_MESSAGE( WM_WQSG_线程LOG , &CTXT_INBOX::线程LOG )
	ON_WM_CLOSE()
	ON_CBN_SELENDOK(IDC_COMBO1, &CTXT_INBOX::OnCbnSelendokCombo1)
	ON_EN_CHANGE(IDC_EDIT_SP1, &CTXT_INBOX::OnEnChangeEditSp1)
	ON_EN_CHANGE(IDC_EDIT_SP2, &CTXT_INBOX::OnEnChangeEditSp2)
	ON_EN_KILLFOCUS(IDC_EDIT_SP1, &CTXT_INBOX::OnEnKillfocusEditSp1)
	ON_EN_KILLFOCUS(IDC_EDIT_SP2, &CTXT_INBOX::OnEnKillfocusEditSp2)
	ON_BN_CLICKED(IDC_CHECK4, &CTXT_INBOX::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK1, &CTXT_INBOX::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT_EXT, &CTXT_INBOX::OnEnChangeEditExt)
	ON_BN_CLICKED(IDC_BUTTON1, &CTXT_INBOX::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CTXT_INBOX::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_Add, &CTXT_INBOX::OnBnClickedButtonAdd)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTXT_INBOX::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON_Del, &CTXT_INBOX::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_Edit, &CTXT_INBOX::OnBnClickedButtonEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CTXT_INBOX::OnEnKillfocusEditName)
	ON_BN_CLICKED(IDC_CHECK3, &CTXT_INBOX::OnBnClickedCheck3)
END_MESSAGE_MAP()
void CTXT_INBOX::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}
void CTXT_INBOX::OnBnClickedButtonRom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_Rom = path;

	UpdateData( FALSE );
}
void CTXT_INBOX::OnBnClickedButtonTxtDir()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXT_DIR = path;

	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonTbl()
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

void CTXT_INBOX::OnBnClickedButtonTbl2()
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

void CTXT_INBOX::OnBnClickedButtonStart()
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

	m_参数.m_长度不足提示 = (m_C长度不足中止.GetCheck() != 0);

	m_参数.m_单字节 = m_参数.m_双字节 = FALSE;

	switch ( m_C选择填充.GetCurSel() )
	{
	case 1:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
		m_参数.m_单字节 = TRUE;
		break;
	case 2:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_参数.m_双字节 = TRUE;
		break;
	case 3:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_参数.m_单字节 = TRUE;
		m_参数.m_双字节 = TRUE;
		break;
	default:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
	}
	UINT uiTmp;
	if( 1 != swscanf( m_EDIT_SP1.GetString() , L"%X" , &uiTmp ) )
	{
		return;
	}
	m_参数.m_SP1 = uiTmp;

	if( 1 != swscanf( m_EDIT_SP2.GetString() , L"%X" , &uiTmp ) )
	{
		return;
	}
	m_参数.m_SP2 = ((uiTmp&0xFF)<<8) | ((uiTmp)>>8);

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

		SendMessage( WM_WQSG_线程信息 , 0 , (LPARAM)L"正在导入..." );
		handle = CreateThread( NULL , NULL
			, 普通导入_文件夹
			, &m_参数 , NULL , NULL );
	}
	else
	{
		CWQSGFileDialog fileDlg ( TRUE , NULL , NULL , OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , L"ROM文件(*.*)|*.*|" );
		fileDlg.m_ofn.lpstrTitle = L"选择要导入文本的文件(多选)...";
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

		SendMessage( WM_WQSG_线程信息 , 0 , (LPARAM)L"正在导入..." );

		handle = CreateThread( NULL , NULL
			, 普通导入_文件
			, &m_参数 , NULL , NULL );
	}
	if( handle != NULL )
	{
		CloseHandle( handle );
	}
	else
		SendMessage( WM_WQSG_线程信息 , 0 , 0 );
}
LRESULT CTXT_INBOX::线程信息( WPARAM 保留 , LPARAM 文本 )
{
	if( 0 == 文本 )
	{
		AppLog( m_LOG );
//		MessageBox(L"导入完毕!!");
	}
	WQSG_MAIN_CWND->SendMessage( WM_WQSG_设置文本 , 0 , 文本 );
	return 0;
}
LRESULT CTXT_INBOX::线程LOG( WPARAM 保留 , LPARAM 文本 )
{
	AppLog( (WCHAR*)文本 );
	WQSG_MAIN_CWND->SendMessage( WM_WQSG_设置LOG文本 , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}

void CTXT_INBOX::OnOK(){}
void CTXT_INBOX::OnCancel(){}
BOOL CTXT_INBOX::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_参数.m_hWnd = m_hWnd;
	if( NULL == m_文件列表缓存.GetBuffer(65536) )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_C选择填充.SetCurSel( 0 );
	m_C验证TBL.SetCheck(TRUE);

	// TODO: 在此添加控件通知处理程序代码
	m_C文本在同目录.SetCheck( TRUE );
	OnBnClickedCheck3();

	载入配置();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CTXT_INBOX::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
	CDialog::OnCancel();
}
void CTXT_INBOX::OnCbnSelendokCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	switch ( m_C选择填充.GetCurSel() )
	{
	case 1:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
		break;
	case 2:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		break;
	case 3:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		break;
	default:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
	}
}
void CTXT_INBOX::OnEnChangeEditSp1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( m_EDIT_SP1 , (CEdit*)GetDlgItem( IDC_EDIT_SP1 ) , this , FALSE );
}

void CTXT_INBOX::OnEnChangeEditSp2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	编辑框验证十六进制文本( m_EDIT_SP2 , (CEdit*)GetDlgItem( IDC_EDIT_SP2 ) , this , FALSE );
}

void CTXT_INBOX::OnEnKillfocusEditSp1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_SP1.GetLength() == 0 )
	{
		m_EDIT_SP1 = L"20";
	}
	else
	{
		while( m_EDIT_SP1.GetLength() < 2 )
			m_EDIT_SP1 = L'0' + m_EDIT_SP1;
	}
	UpdateData(FALSE);
}

void CTXT_INBOX::OnEnKillfocusEditSp2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_SP2.GetLength() == 0 )
	{
		m_EDIT_SP2 = L"20";
	}
	else
	{
		while( m_EDIT_SP2.GetLength() < 4 )
			m_EDIT_SP2 = L'0' + m_EDIT_SP2;
	}
	UpdateData(FALSE);
}

void CTXT_INBOX::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_C使用控制码表.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
}

void CTXT_INBOX::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	const BOOL 选中 = m_C从目录导出.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( 选中 );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( 选中 );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( 选中 );
}

void CTXT_INBOX::OnEnChangeEditExt()
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

void CTXT_INBOX::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LOG = L"";
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_INBOX::载入配置(void)
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	if( !WQSG_ini.SelConfigName( DEF_V_import ) )
		return ;

	CString str;
	for( long count = WQSG_ini.GetItemCount() , i = 0;
		i < count ; ++i )
	{
		if( WQSG_ini.GetAppName( i , str ) )
			m_CList.AddString( str );
	}
}

BOOL CTXT_INBOX::写配置(void)
{
	UpdateData();
	if( !WQSG_ini.SelConfigName( DEF_V_import ) )
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
//	DEF_FN_XYZ( L"SegmentAddr" , m_EDIT_段号 )
//	DEF_FN_XYZ( L"BeginOffset" , m_EDIT_开始偏移 )
//	DEF_FN_XYZ( L"EndOffset" , m_EDIT_结束偏移 )
//	DEF_FN_XYZ( L"MinLen" , m_EDIT_min )
//	DEF_FN_XYZ( L"MaxLen" , m_EDIT_max )
	DEF_FN_XYZ( L"UseDirectory" , (m_C从目录导出.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"CheckTblOverlap" , (m_C验证TBL.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"UseTBL2" , (m_C使用控制码表.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"TxtDirDefault" , (m_C文本在同目录.GetCheck()!=0)?L"1":L"0" )
	DEF_FN_XYZ( L"SubDir" , (m_c包括子目录.GetCheck()!=0)?L"1":L"0" )
	//
	DEF_FN_XYZ( L"LenOverStop" , (m_C长度不足中止.GetCheck()!=0)?L"1":L"0" )
	CString str;
	str.Format( L"%d" , m_C选择填充.GetCurSel() );
	DEF_FN_XYZ( L"Fill" , str )
	DEF_FN_XYZ( L"FillByte" , m_EDIT_SP1 )
	DEF_FN_XYZ( L"FillWord" , m_EDIT_SP2 )
#undef DEF_FN_XYZ

	if( !WQSG_ini.Save( WQSG_iniSavePathName ) )
	{
		MessageBox( L"保存文件失败" );
		return FALSE;
	}

	return TRUE;
}

void CTXT_INBOX::OnEnChangeEditName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

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

void CTXT_INBOX::OnBnClickedButtonAdd()
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

void CTXT_INBOX::AppLog(CString str)
{
	m_LOG += (str + L"\r\n");
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_INBOX::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )return;

	if( !WQSG_ini.SelConfigName( DEF_V_import ) )
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
//	DEF_FN_XYZ( L"SegmentAddr" , m_EDIT_段号 , L"0" );
//	DEF_FN_XYZ( L"BeginOffset" , m_EDIT_开始偏移 , L"" );
//	DEF_FN_XYZ( L"EndOffset" , m_EDIT_结束偏移 , L"" );
//	DEF_FN_XYZ( L"MinLen" , m_EDIT_min , L"0" );
//	DEF_FN_XYZ( L"MaxLen" , m_EDIT_max , L"9999" );
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
	//
	DEF_FN_XYZ( L"LenOverStop" , str , L"0" );
	m_C长度不足中止.SetCheck( 0 != ::_wtoi( str.GetString() ) );

	DEF_FN_XYZ( L"Fill" , str , L"0" );
	m_C选择填充.SetCurSel( ::_wtoi( str.GetString() ) );
	UpdateData( FALSE );
	OnCbnSelendokCombo1();

	DEF_FN_XYZ( L"FillByte" , m_EDIT_SP1 , L"20" );
	DEF_FN_XYZ( L"FillWord" , m_EDIT_SP2 , L"8140" );
#undef DEF_FN_XYZ
	m_NodeName = NodeName;

	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"抽~~~~还没选择怎么删" );
		return;
	}

	if( !WQSG_ini.SelConfigName( DEF_V_import ) )
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

void CTXT_INBOX::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"招抽~~~~还没选择怎么改" );
		return;
	}

	if( !WQSG_ini.SelConfigName( DEF_V_import ) )
	{
		MessageBox( L"选择节点失败" );
		return ;
	}

	CString NodeName;
	m_CList.GetText( sel , NodeName );
	if( m_NodeName != NodeName )
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

void CTXT_INBOX::OnEnKillfocusEditName()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem( IDC_EDIT_TXT_DIR )->EnableWindow( m_C文本在同目录.GetCheck() == 0 );
}
