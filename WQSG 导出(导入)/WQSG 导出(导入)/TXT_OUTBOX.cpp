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
#include <WQSG导出导入.h>

#define WM_WQSG_THREAD_MSG	( WM_USER + 1 )
#define WM_WQSG_THREAD_LOG		( WM_WQSG_THREAD_MSG + 1 )

static BOOL zzz普通导出( const CString& a_strFileName , CWQSG_TxtExport& WQSG , SExportDataEx& a_Data )
{
	CString strTxtPathName;
	if( a_Data.m_strTXTPath.GetLength() <= 0 )
	{
		strTxtPathName = a_strFileName.Left( a_strFileName.ReverseFind(L'\\') + 1 ) + a_strFileName.Mid( a_strFileName.ReverseFind(L'\\') + 1 ) + L".TXT";
	}
	else
	{
		WQSG_CreateDir( a_Data.m_strTXTPath.GetString() );
		strTxtPathName = a_Data.m_strTXTPath + a_strFileName.Mid( a_strFileName.ReverseFind(L'\\') + 1 ) + L".TXT";
	}
	return WQSG.ExportText( a_strFileName.GetString() , strTxtPathName.GetString() , a_Data.m_uBeginOffset , a_Data.m_uEndOffset , a_Data.m_uMinLen , a_Data.m_uMaxLen );
}
// CTXT_OutBox 对话框
static DWORD WINAPI 普通导出_文件(LPVOID lpParameter)
{
	SExportDataEx& data = *(SExportDataEx*)lpParameter;

	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"启动文件模式" );
///--------------------------
	static CWQSG_TxtExport WQSG;
	CString strFile;
///-------------------------
	CHiResTimer RunTime;
	if( !WQSG.LoadTbl( data.m_strTBLPathName.GetString() , (data.m_strTBL2PathName.GetLength() < 4)?NULL:data.m_strTBL2PathName.GetString() , data.m_bCheckTblOverlap ) )
		goto __gtExit;

	RunTime.Start();
	for( std::vector<CStringW>::iterator it = data.m_Files.begin() ; it != data.m_Files.end() ; ++it )
	{
		strFile = *it;
		zzz普通导出( strFile , WQSG , data );
		/*
		if( !zzz普通导出( 文件 , WQSG , data ) )
			goto __gt退出;
			*/
	}
	RunTime.Stop();
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );
	}
__gtExit:
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"关闭文件模式" );
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_MSG , 0 , 0 );
	return 0;
}
static BOOL zzz_普通导出_文件夹( CStringW a_strPath , SExportDataEx& a_Data , CWQSG_TxtExport& WQSG , INT& countAll , std::vector<CStringW>& szExt )
{
	if( a_strPath.Right(1) != L'\\')
		a_strPath += L'\\';

	::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"进入 " + a_strPath ).GetString() );

	WIN32_FIND_DATAW FindFileData;
	const HANDLE hFind = ::FindFirstFileW( a_strPath + L"*", &FindFileData );
	if ( hFind != INVALID_HANDLE_VALUE )
	{
		INT count = 0;
		do
		{
			CStringW fileName( FindFileData.cFileName );
			if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( ( fileName != L"." ) && ( fileName != L".." ) && a_Data.m_bSubDir )
				{
					zzz_普通导出_文件夹( a_strPath + FindFileData.cFileName , a_Data , WQSG , count , szExt );
				/*	if( !zzz_普通导出_文件夹( 路径 + FindFileData.cFileName , a_Data , WQSG , count , szExt ) )
						return FALSE;*/

					::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"返回 " + a_strPath ).GetString() );
				}
			}
			else
			{
				for( int i = 0 ; (i >= 0) && ( (size_t)i < szExt.size() ) ; ++i )
				{
					if( fileName.Right( szExt[i].GetLength() ).MakeUpper() == szExt[i] )
					{
						if( zzz普通导出( a_strPath + FindFileData.cFileName , WQSG , a_Data ) )
						{
							++count;
							++countAll;
						}
						/*if( !zzz普通导出( 路径 + FindFileData.cFileName , WQSG , a_Data ) )
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
		a_strPath.Format( L"导出 %u 个文件" , count );
		::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)a_strPath.GetString() );
	}
	else 
	{
		GetLastError();
	}
	return TRUE;
}
static DWORD WINAPI 普通导出_文件夹(LPVOID lpParameter)
{
	SExportDataEx& data = *(SExportDataEx*)lpParameter;
///-------------------------------------------------
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"启动文件夹模式" );

	if( data.m_strTXTPath.GetLength() > 0 )
	{
		if( data.m_strTXTPath.Right(1) != L'\\')
			data.m_strTXTPath += L'\\';
	}

	static CWQSG_TxtExport WQSG;
///-------------------------------------------------
	CHiResTimer RunTime;
	std::vector<CStringW> szExt;
	INT count = 0;

	if( !WQSG.LoadTbl( data.m_strTBLPathName.GetString() , (data.m_strTBL2PathName.GetLength() < 4)?NULL:data.m_strTBL2PathName.GetString() , data.m_bCheckTblOverlap ) )
		goto __gtExit;
	//
	分解Ext( data.m_strExtName , szExt );
	if( szExt.size() == 0 )
		szExt.push_back( CStringW(L"") );
	//
	RunTime.Start();
	zzz_普通导出_文件夹( data.m_strROMPath , data , WQSG , count , szExt );
	RunTime.Stop();
	//
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );

		str.Format( L"一共成功导出 %u 个文件" , count );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );
	}
__gtExit:
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"关闭文件夹模式" );
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_MSG , 0 , 0 );
	return 0;
}
IMPLEMENT_DYNAMIC(CTXT_OutBox, CBaseDialog)
CTXT_OutBox::CTXT_OutBox(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTXT_OutBox::IDD, pParent)
	, m_EDIT_ROMPath(_T(""))
	, m_EDIT_TXTPath(_T(""))
	, m_EDIT_TBLPathName(_T(""))
	, m_EDIT_TBL2PathName(_T(""))
	, m_EDIT_min(_T("2"))
	, m_EDIT_max(_T("99999"))
	, m_EDIT_SegmentAddr(_T("0"))
	, m_EDIT_BeginOffset(_T("0"))
	, m_EDIT_EndOffset(_T("FFFFFFFF"))
	, m_EDIT_ExtName(_T(""))
	, m_LOG(_T(""))
	, m_NodeName(_T(""))
{
}

CTXT_OutBox::~CTXT_OutBox()
{
}

void CTXT_OutBox::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_cUseDirectory);
	DDX_Text(pDX, IDC_EDIT_ROM, m_EDIT_ROMPath);
	DDV_MaxChars(pDX, m_EDIT_ROMPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TXT_DIR, m_EDIT_TXTPath);
	DDV_MaxChars(pDX, m_EDIT_TXTPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL, m_EDIT_TBLPathName);
	DDV_MaxChars(pDX, m_EDIT_TBLPathName, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL2, m_EDIT_TBL2PathName);
	DDV_MaxChars(pDX, m_EDIT_TBL2PathName, MAX_PATH);
	DDX_Control(pDX, IDC_CHKTBL, m_cCheckTblOverlap);
	DDX_Text(pDX, IDC_EDIT_MIN, m_EDIT_min);
	DDV_MaxChars(pDX, m_EDIT_min, 5);
	DDX_Text(pDX, IDC_EDIT_MAX, m_EDIT_max);
	DDV_MaxChars(pDX, m_EDIT_max, 5);
	DDX_Text(pDX, IDC_EDIT_ADDR_H, m_EDIT_SegmentAddr);
	DDV_MaxChars(pDX, m_EDIT_SegmentAddr, 8);
	DDX_Text(pDX, IDC_EDIT_ADDR_KS_L, m_EDIT_BeginOffset);
	DDV_MaxChars(pDX, m_EDIT_BeginOffset, 8);
	DDX_Text(pDX, IDC_EDIT_ADDR_JS_L, m_EDIT_EndOffset);
	DDV_MaxChars(pDX, m_EDIT_EndOffset, 8);
	DDX_Text(pDX, IDC_EDIT_EXT, m_EDIT_ExtName);
	DDV_MaxChars(pDX, m_EDIT_ExtName, 100);
	DDX_Control(pDX, IDC_EDIT_LOG, m_CEDIT_LOG);
	DDX_Control(pDX, IDC_CHECK4, m_cUseTBL2);
	DDX_Control(pDX, IDC_LIST2, m_CList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_NodeName);
	DDV_MaxChars(pDX, m_NodeName, 20);
	DDX_Control(pDX, IDC_CHECK3, m_C文本在同目录);
	DDX_Control(pDX, IDC_CHECK5, m_cSubDir);
}


BEGIN_MESSAGE_MAP(CTXT_OutBox, CBaseDialog)
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
	ON_MESSAGE( WM_WQSG_THREAD_MSG , &CTXT_OutBox::线程信息 )
	ON_MESSAGE( WM_WQSG_THREAD_LOG , &CTXT_OutBox::线程LOG )
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
void CTXT_OutBox::OnClose()
{
	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}

//--------------------------------------------------------
void CTXT_OutBox::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if( m_C文本在同目录.GetCheck() != 0 )
	{
		m_Data.m_strTXTPath = L"";
	}
	else
	{
		if( !::WQSG_IsDir( m_EDIT_TXTPath.GetString() ) )
		{
			MessageBoxW( L"文本目录出错" );
			return;
		}
		m_Data.m_strTXTPath = m_EDIT_TXTPath;
		if( m_Data.m_strTXTPath.Right(1) != L'\\')
			m_Data.m_strTXTPath += L'\\';
	}
	if( !::WQSG_IsFile( m_EDIT_TBLPathName.GetString() ) )
	{
		MessageBoxW( L"请检查码表路径" );
		return;
	}
	if( (m_cUseTBL2.GetCheck() != 0 ) && !::WQSG_IsFile( m_EDIT_TBL2PathName.GetString() ) )
	{
		MessageBoxW( L"请检查控制码表路径" );
		return;
	}
	m_Data.m_bCheckTblOverlap = m_cCheckTblOverlap.GetCheck() != 0;
	m_Data.m_bSubDir = m_cSubDir.GetCheck() != 0;
	m_Data.m_strTBLPathName = m_EDIT_TBLPathName;
	m_Data.m_strTBL2PathName = (m_cUseTBL2.GetCheck() != 0)?m_EDIT_TBL2PathName:L"";

	m_Data.m_Files.clear();

	u32 val;
	if ( 1 != swscanf( m_EDIT_SegmentAddr , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"段号\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_Data.m_uBeginOffset = val;
	m_Data.m_uBeginOffset <<= 32;
	m_Data.m_uEndOffset = m_Data.m_uBeginOffset;

	if ( 1 != swscanf( m_EDIT_BeginOffset , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"开始偏移\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_Data.m_uBeginOffset |= val;

	if ( 1 != swscanf( m_EDIT_EndOffset , L"%X" , &val ) )
	{
		MessageBoxW( L"请检查\"结束偏移\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_JS_L )->SetFocus();
		return;
	}
	m_Data.m_uEndOffset |= val;

	m_Data.m_uMinLen = _wtoi( m_EDIT_min.GetString() );
	m_Data.m_uMaxLen = _wtoi( m_EDIT_max.GetString() );

	HANDLE handle;
	if( m_cUseDirectory.GetCheck() != 0 )
	{
		if( !CheckExt( m_EDIT_ExtName ) )
		{
			MessageBox( L"扩展名错误");
			return;
		}
		m_Data.m_strExtName = ( m_EDIT_ExtName.GetLength() > 0 )?m_EDIT_ExtName:L"*";

		m_Data.m_strROMPath = m_EDIT_ROMPath;
		if( m_Data.m_strROMPath.Right(1) != L'\\')
			m_Data.m_strROMPath += L'\\';

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"正在导出..." );
		handle = CreateThread( NULL , NULL
		, 普通导出_文件夹
		, &m_Data , NULL , NULL );
	}
	else
	{
		static CWQSGFileDialog_OpenS fileDlg( L"ROM文件(*.*)|*.*||" );
		fileDlg.SetWindowTitle( L"选择要导出文本的文件(多选)..." );

		if( fileDlg.DoModal() != IDOK )
			return;

		POSITION pos = fileDlg.GetStartPosition();
		CString strFile;
		while( fileDlg.GetNextPathName( strFile , pos ) )
		{
			m_Data.m_Files.push_back( strFile );
		}

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"正在导出..." );

		handle = CreateThread( NULL , NULL
			, 普通导出_文件
			, &m_Data , NULL , NULL );
	}
	if( handle != NULL )
	{
		CloseHandle( handle );
	}
	else
		SendMessage( WM_WQSG_THREAD_MSG , 0 , 0 );
}
//--------------------------------------------------------
void CTXT_OutBox::OnBnClickedButtonTxtDir()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXTPath = path;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CWQSGFileDialog_Open dlg( L"码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBLPathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"控制码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBL2PathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	const BOOL 选中 = m_cUseDirectory.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( 选中 );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( 选中 );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( 选中 );
}

BOOL CTXT_OutBox::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Data.m_hWnd = m_hWnd;

	m_cCheckTblOverlap.SetCheck(TRUE);

	m_C文本在同目录.SetCheck( TRUE );
	OnBnClickedCheck3();

	LoadXml( LockConfig() );
	UnLockConfig();

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
	EditCheckHaxStr( m_EDIT_SegmentAddr , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_H ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrKsL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	EditCheckHaxStr( m_EDIT_BeginOffset , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_KS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrJsL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	EditCheckHaxStr( m_EDIT_EndOffset , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_JS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnKillfocusEditAddrH()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_EDIT_SegmentAddr.GetLength() == 0)
	{
		m_EDIT_SegmentAddr = L"0";
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

	g_pMAIN_CWND->SendMessage( WM_WQSG_SetText , 0 , 文本 );
	return 0;
}

LRESULT CTXT_OutBox::线程LOG( WPARAM 保留 , LPARAM 文本 )
{
	AppLog( (WCHAR*)文本 );
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetLogText , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}
void CTXT_OutBox::OnBnClickedButtonRom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_ROMPath = path;

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
	m_EDIT_ExtName.TrimLeft();
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
	if( m_cUseTBL2.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
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
	m_ExportDatas.push_back(SExportData());
	(*m_ExportDatas.rbegin()).m_strItemName = m_NodeName;
	UpdateExportData(*m_ExportDatas.rbegin());
	m_CList.SetCurSel( m_CList.AddString( m_NodeName ) );

	SaveXml();
}

void CTXT_OutBox::UpdateExportData( SExportData& a_data )
{
	UpdateData();

	a_data.m_strROMPath = m_EDIT_ROMPath;
	a_data.m_strTXTPath = m_EDIT_TXTPath;
	a_data.m_strTBLPathName = m_EDIT_TBLPathName;
	a_data.m_strTBL2PathName = m_EDIT_TBL2PathName;
	a_data.m_strExtName = m_EDIT_ExtName;
	a_data.m_uSegmentAddr = _wtoi( m_EDIT_SegmentAddr );
	a_data.m_uBeginOffset = _wtoi( m_EDIT_BeginOffset );
	a_data.m_uEndOffset = _wtoi( m_EDIT_EndOffset );
	a_data.m_uMinLen = _wtoi( m_EDIT_min );
	a_data.m_uMaxLen = _wtoi( m_EDIT_max );
	a_data.m_bUseDirectory = m_cUseDirectory.GetCheck() != 0;
	a_data.m_bCheckTblOverlap = m_cCheckTblOverlap.GetCheck() != 0;
	a_data.m_bUseTBL2 = m_cUseTBL2.GetCheck() != 0;
	a_data.m_bTxtDirDefault = m_C文本在同目录.GetCheck() != 0;
	a_data.m_bSubDir = m_cSubDir.GetCheck() != 0;
}

void CTXT_OutBox::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )return;

	const SExportData& data = m_ExportDatas[sel];
	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = data.m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
	}

	m_EDIT_ROMPath = data.m_strROMPath;
	m_EDIT_TXTPath = data.m_strTXTPath;
	m_EDIT_TBLPathName = data.m_strTBLPathName;
	m_EDIT_TBL2PathName = data.m_strTBL2PathName;
	m_EDIT_ExtName = data.m_strExtName;
	m_EDIT_SegmentAddr.Format( L"%u" , data.m_uSegmentAddr );
	m_EDIT_BeginOffset.Format( L"%u" , data.m_uBeginOffset );
	m_EDIT_EndOffset.Format( L"%u" , data.m_uEndOffset );
	m_EDIT_min.Format( L"%u" , data.m_uMinLen );
	m_EDIT_max.Format( L"%u" , data.m_uMaxLen );
	m_cUseDirectory.SetCheck( data.m_bUseDirectory );
	UpdateData( FALSE );
	OnBnClickedCheck1();

	m_cCheckTblOverlap.SetCheck( data.m_bCheckTblOverlap );
	m_cUseTBL2.SetCheck( data.m_bUseTBL2 );
	UpdateData( FALSE );
	OnBnClickedCheck4();

	m_C文本在同目录.SetCheck( data.m_bTxtDirDefault );
	UpdateData( FALSE );
	OnBnClickedCheck3();

	m_cSubDir.SetCheck( data.m_bSubDir );
#undef DEF_FN_XYZ
	m_NodeName = data.m_strItemName;

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

	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = m_ExportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
	}
	m_ExportDatas.erase( m_ExportDatas.begin() + sel );

	m_CList.DeleteString( sel );
	m_CList.SetCurSel( -1 );

	SaveXml();
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

	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = m_ExportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
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

		m_ExportDatas[sel].m_strItemName = m_NodeName;

		m_CList.DeleteString( sel );
		m_CList.InsertString( sel , m_NodeName );
	}

	UpdateExportData( m_ExportDatas[sel] );
	SaveXml();
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

void CTXT_OutBox::LoadXml( TiXmlElement& a_Root )
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	m_ExportDatas.clear();

	const TiXmlElement* pNode;

	pNode = a_Root.FirstChildElement( "Export" );
	if( !pNode )
		return;

	for( pNode = pNode->FirstChildElement( "ExportItem" ) ; pNode ; pNode = pNode->NextSiblingElement( "ExportItem" ) )
	{
		const char* pNameUtf8 = pNode->Attribute( "Name" );
		if( !pNameUtf8 )
			continue;

		SExportData data;
		data.m_strItemName = Utf82Utf16le( pNameUtf8 );
#define DEF_FN_XYZ( b , c ) (pNode->Attribute( Utf16le2Utf8( b ) )?Utf82Utf16le(pNode->Attribute( Utf16le2Utf8( b ) )):c)

		data.m_strROMPath = DEF_FN_XYZ( L"ROMPath" , L"" );
		data.m_strTXTPath = DEF_FN_XYZ( L"TXTPath" , L"" );
		data.m_strTBLPathName = DEF_FN_XYZ( L"TBLPathName" , L"" );
		data.m_strTBL2PathName = DEF_FN_XYZ( L"TBL2PathName" , L"" );
		data.m_strExtName = DEF_FN_XYZ( L"ExtName" , L"" );
		data.m_uSegmentAddr = _wtoi(DEF_FN_XYZ( L"SegmentAddr" , L"0" ));
		data.m_uBeginOffset = _wtoi(DEF_FN_XYZ( L"BeginOffset" , L"" ));
		data.m_uEndOffset = _wtoi(DEF_FN_XYZ( L"EndOffset" , L"" ));
		data.m_uMinLen = _wtoi(DEF_FN_XYZ( L"MinLen" , L"2" ));
		data.m_uMaxLen = _wtoi(DEF_FN_XYZ( L"MaxLen" , L"9999" ));
		data.m_bUseDirectory = 0 != _wtoi(DEF_FN_XYZ( L"UseDirectory" , L"0" ));
		data.m_bCheckTblOverlap = 0 != _wtoi(DEF_FN_XYZ( L"CheckTblOverlap" , L"1" ));
		data.m_bUseTBL2 = 0 != _wtoi(DEF_FN_XYZ( L"UseTBL2" , L"0" ));
		data.m_bTxtDirDefault = 0 != _wtoi(DEF_FN_XYZ( L"TxtDirDefault" , L"0" ));
		data.m_bSubDir = 0 != _wtoi(DEF_FN_XYZ( L"SubDir" , L"0" ));
#undef DEF_FN_XYZ
		m_ExportDatas.push_back( data );

		m_CList.AddString( data.m_strItemName );
	}
}

void CTXT_OutBox::SaveXml()
{
	TiXmlElement& a_Root = LockConfig();

	TiXmlNode*const pOldNode = a_Root.FirstChildElement( "Export" );
	TiXmlElement*const pNewNode = a_Root.LinkEndChild( new TiXmlElement( "Export" ) )->ToElement();

	std::vector<SExportData>::iterator it = m_ExportDatas.begin();

	for ( ; it != m_ExportDatas.end() ; ++it )
	{
		const SExportData& data = *it;
		TiXmlElement*const pNode = pNewNode->LinkEndChild( new TiXmlElement( "ExportItem" ) )->ToElement();

		pNode->SetAttribute( "Name" , Utf16le2Utf8( data.m_strItemName ) );
		pNode->SetAttribute( "ROMPath" , Utf16le2Utf8( data.m_strROMPath ) );
		pNode->SetAttribute( "TXTPath" , Utf16le2Utf8( data.m_strTXTPath ) );
		pNode->SetAttribute( "TBLPathName" , Utf16le2Utf8( data.m_strTBLPathName ) );
		pNode->SetAttribute( "TBL2PathName" , Utf16le2Utf8( data.m_strTBL2PathName ) );
		pNode->SetAttribute( "ExtName" , Utf16le2Utf8( data.m_strExtName ) );
		pNode->SetAttribute( "SegmentAddr" , X2Utf8( data.m_uSegmentAddr ) );
		pNode->SetAttribute( "BeginOffset" , X2Utf8( data.m_uBeginOffset ) );
		pNode->SetAttribute( "EndOffset" , X2Utf8( data.m_uEndOffset ) );
		pNode->SetAttribute( "MinLen" , X2Utf8( data.m_uMinLen ) );
		pNode->SetAttribute( "MaxLen" , X2Utf8( data.m_uMaxLen ) );
		pNode->SetAttribute( "UseDirectory" , X2Utf8( data.m_bUseDirectory ) );
		pNode->SetAttribute( "CheckTblOverlap" , X2Utf8( data.m_bCheckTblOverlap ) );
		pNode->SetAttribute( "UseTBL2" , X2Utf8( data.m_bUseTBL2 ) );
		pNode->SetAttribute( "TxtDirDefault" , X2Utf8( data.m_bTxtDirDefault ) );
		pNode->SetAttribute( "SubDir" , X2Utf8( data.m_bSubDir ) );
	}

	if( pOldNode )
	{
		a_Root.RemoveChild( pOldNode );
		delete pOldNode;
	}

	UnLockConfig();
	SaveConfig();
}
