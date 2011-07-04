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
#include <Common/WQSG导出导入.h>

#define WM_WQSG_THREAD_MSG	( WM_USER + 1 )
#define WM_WQSG_THREAD_LOG		( WM_WQSG_THREAD_MSG + 1 )

static BOOL zzzImport( const CString& a_strFileName , CWQSG_TxtImport& WQSG , SImportDataEx& a_Data )
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

	return WQSG.ImportText( a_strFileName.GetString() , strTxtPathName.GetString() , ( (a_Data.m_单字节)?(&a_Data.m_SP1):NULL ) , ( (a_Data.m_双字节)?(&a_Data.m_SP2):NULL ) , a_Data.m_bLenOverStop );
}
// CTXT_INBOX 对话框
static DWORD WINAPI Import_File(LPVOID lpParameter)
{
	SImportDataEx& data = *(SImportDataEx*)lpParameter;
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"启动文件模式" );

///--------------------------
	static CWQSG_TxtImport WQSG;
	CString strFile;
///-------------------------
	CHiResTimer RunTime;
	if( !WQSG.LoadTbl( data.m_strTBLPathName.GetString() , (data.m_strTBL2PathName.GetLength() < 4)?NULL:data.m_strTBL2PathName.GetString() , data.m_bCheckTblOverlap ) )
		goto __gtExit;

	RunTime.Start();
	for( std::vector<CStringW>::iterator it = data.m_Files.begin() ; it != data.m_Files.end() ; ++it )
	{
		strFile = *it;
		zzzImport( strFile , WQSG , data );
		/*
		if( !zzz普通导入( 文件 , WQSG , a_Data ) )
			break;*/
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
static BOOL zzz_Import_Dir( CStringW a_strPath , SImportDataEx& a_Data , CWQSG_TxtImport& WQSG , INT& countAll , std::vector<CStringW>& szExt )
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
					zzz_Import_Dir( a_strPath + FindFileData.cFileName , a_Data , WQSG , count , szExt );
/*
					if( !zzz_普通导入_文件夹( 路径 + FindFileData.cFileName , a_Data , WQSG , count , szExt ) )
						return FALSE;
*/
					::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"返回 " + a_strPath ).GetString() );
				}
			}
			else
			{
				for( int i = 0 ; (i >= 0) && ( (size_t)i < szExt.size() ) ; ++i )
				{
					if( fileName.Right( szExt[i].GetLength() ).MakeUpper() == szExt[i] )
					{
						if( zzzImport( a_strPath + FindFileData.cFileName , WQSG , a_Data ) )
						{
							++count;
							++countAll;
						}
						/*
						if( !zzz普通导入( 路径 + FindFileData.cFileName , WQSG , a_Data ) )
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
		a_strPath.Format( L"导入 %u 个文件" , count );
		::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)a_strPath.GetString() );
	}
	else 
	{
		GetLastError();
	}
	return TRUE;
}
static DWORD WINAPI Import_Dir(LPVOID lpParameter)
{
	SImportDataEx& data = *(SImportDataEx*)lpParameter;
///-------------------------------------------------
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"启动文件夹模式" );

	if( data.m_strTXTPath.GetLength() > 0 )
	{
		if( data.m_strTXTPath.Right(1) != L'\\')
			data.m_strTXTPath += L'\\';
	}

	static CWQSG_TxtImport WQSG;
	CHiResTimer RunTime;
	std::vector<CStringW> szExt;
	INT count = 0;
///-------------------------------------------------
	if( !WQSG.LoadTbl( data.m_strTBLPathName.GetString() , (data.m_strTBL2PathName.GetLength() < 4)?NULL:data.m_strTBL2PathName.GetString() , data.m_bCheckTblOverlap ) )
		goto __gtExit;

	分解Ext( data.m_strExtName , szExt );
	if( szExt.size() == 0 )
		szExt.push_back( CStringW(L"") );
	//
	RunTime.Start();
	zzz_Import_Dir( data.m_strROMPath , data , WQSG , count , szExt );
	RunTime.Stop();
	//
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"共耗时:%d 天 %d 时 %d 分 %d 秒 %d 毫秒" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );

		str.Format( L"一共成功导入 %u 个文件" , count );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );
	}
__gtExit:
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"关闭文件夹模式" );
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_MSG , 0 , 0 );
	return 0;
}
IMPLEMENT_DYNAMIC(CTXT_INBOX, CBaseDialog)
CTXT_INBOX::CTXT_INBOX(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTXT_INBOX::IDD, pParent)
	, m_EDIT_SP1(_T("20"))
	, m_EDIT_SP2(_T("8140"))
	, m_EDIT_ExtName(_T(""))
	, m_NodeName(_T(""))
{

}

CTXT_INBOX::~CTXT_INBOX()
{
}

void CTXT_INBOX::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROM, m_EDIT_ROMPath);
	DDV_MaxChars(pDX, m_EDIT_ROMPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TXT_DIR, m_EDIT_TXTPath);
	DDV_MaxChars(pDX, m_EDIT_TXTPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL, m_EDIT_TBLPathName);
	DDV_MaxChars(pDX, m_EDIT_TBLPathName, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL2, m_EDIT_TBL2PathName);
	DDV_MaxChars(pDX, m_EDIT_TBL2PathName, MAX_PATH);
	DDX_Control(pDX, IDC_CHKTBL, m_cCheckTblOverlap);
	DDX_Control(pDX, IDC_CHECK1, m_cUseDirectory);
	DDX_Control(pDX, IDC_CHECK2, m_cLenOverStop);
	DDX_Control(pDX, IDC_COMBO1, m_cFill);
	DDX_Text(pDX, IDC_EDIT_SP1, m_EDIT_SP1);
	DDV_MaxChars(pDX, m_EDIT_SP1, 2);
	DDX_Text(pDX, IDC_EDIT_SP2, m_EDIT_SP2);
	DDV_MaxChars(pDX, m_EDIT_SP2, 4);
	DDX_Control(pDX, IDC_CHECK4, m_cUseTBL2);
	DDX_Text(pDX, IDC_EDIT_EXT, m_EDIT_ExtName);
	DDV_MaxChars(pDX, m_EDIT_ExtName, 100);
	DDX_Control(pDX, IDC_EDIT_LOG, m_CEDIT_LOG);
	DDX_Text(pDX, IDC_EDIT_NAME, m_NodeName);
	DDV_MaxChars(pDX, m_NodeName, 20);
	DDX_Control(pDX, IDC_LIST2, m_CList);
	DDX_Control(pDX, IDC_CHECK3, m_C文本在同目录);
	DDX_Control(pDX, IDC_CHECK5, m_cSubDir);
}
BEGIN_MESSAGE_MAP(CTXT_INBOX, CBaseDialog)
	ON_BN_CLICKED(IDC_CHECK2, &CTXT_INBOX::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_ROM, &CTXT_INBOX::OnBnClickedButtonRom)
	ON_BN_CLICKED(IDC_BUTTON_TXT_DIR, &CTXT_INBOX::OnBnClickedButtonTxtDir)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &CTXT_INBOX::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &CTXT_INBOX::OnBnClickedButtonTbl2)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTXT_INBOX::OnBnClickedButtonStart)
	ON_MESSAGE( WM_WQSG_THREAD_MSG , &CTXT_INBOX::线程信息 )
	ON_MESSAGE( WM_WQSG_THREAD_LOG , &CTXT_INBOX::线程LOG )
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
	m_EDIT_ROMPath = path;

	UpdateData( FALSE );
}
void CTXT_INBOX::OnBnClickedButtonTxtDir()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXTPath = path;

	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBLPathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonTbl2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"控制码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBL2PathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonStart()
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

	m_Data.m_bLenOverStop = (m_cLenOverStop.GetCheck() != 0);

	m_Data.m_单字节 = m_Data.m_双字节 = FALSE;

	switch ( m_cFill.GetCurSel() )
	{
	case 1:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
		m_Data.m_单字节 = TRUE;
		break;
	case 2:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_Data.m_双字节 = TRUE;
		break;
	case 3:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_Data.m_单字节 = TRUE;
		m_Data.m_双字节 = TRUE;
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
	m_Data.m_SP1 = uiTmp;

	if( 1 != swscanf( m_EDIT_SP2.GetString() , L"%X" , &uiTmp ) )
	{
		return;
	}
	m_Data.m_SP2 = ((uiTmp&0xFF)<<8) | ((uiTmp)>>8);

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

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"正在导入..." );
		handle = CreateThread( NULL , NULL
			, Import_Dir
			, &m_Data , NULL , NULL );
	}
	else
	{
		static CWQSGFileDialog_OpenS fileDlg( L"ROM文件(*.*)|*.*||" );
		fileDlg.SetWindowTitle( L"选择要导入文本的文件(多选)..." );

		if( fileDlg.DoModal() != IDOK )
			return;

		POSITION pos = fileDlg.GetStartPosition();
		CString strFile;
		while( fileDlg.GetNextPathName( strFile , pos ) )
		{
			m_Data.m_Files.push_back( strFile );
		}

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"正在导入..." );

		handle = CreateThread( NULL , NULL
			, Import_File
			, &m_Data , NULL , NULL );
	}
	if( handle != NULL )
	{
		CloseHandle( handle );
	}
	else
		SendMessage( WM_WQSG_THREAD_MSG , 0 , 0 );
}
LRESULT CTXT_INBOX::线程信息( WPARAM 保留 , LPARAM 文本 )
{
	if( 0 == 文本 )
	{
		AppLog( (WCHAR*)文本 );
//		MessageBox(L"导入完毕!!");
	}
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetText , 0 , 文本 );
	return 0;
}
LRESULT CTXT_INBOX::线程LOG( WPARAM 保留 , LPARAM 文本 )
{
	AppLog( (WCHAR*)文本 );
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetLogText , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}

void CTXT_INBOX::OnOK(){}
void CTXT_INBOX::OnCancel(){}
BOOL CTXT_INBOX::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Data.m_hWnd = m_hWnd;

	m_cFill.SetCurSel( 0 );
	m_cCheckTblOverlap.SetCheck(TRUE);

	// TODO: 在此添加控件通知处理程序代码
	m_C文本在同目录.SetCheck( TRUE );
	OnBnClickedCheck3();
#if USE_XML
	LoadXml( LockConfig().GetConfig() );
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CTXT_INBOX::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}
void CTXT_INBOX::OnCbnSelendokCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	switch ( m_cFill.GetCurSel() )
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
	EditCheckHaxStr( m_EDIT_SP1 , (CEdit*)GetDlgItem( IDC_EDIT_SP1 ) , this , FALSE );
}

void CTXT_INBOX::OnEnChangeEditSp2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	EditCheckHaxStr( m_EDIT_SP2 , (CEdit*)GetDlgItem( IDC_EDIT_SP2 ) , this , FALSE );
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
	if( m_cUseTBL2.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
}

void CTXT_INBOX::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	const BOOL bSel = m_cUseDirectory.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( bSel );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( bSel );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( bSel );
}

void CTXT_INBOX::OnEnChangeEditExt()
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

void CTXT_INBOX::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LOG = L"";
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_INBOX::UpdateImportData(SImportData& a_data)
{
	UpdateData();

	a_data.m_strROMPath = m_EDIT_ROMPath;
	a_data.m_strTXTPath = m_EDIT_TXTPath;
	a_data.m_strTBLPathName = m_EDIT_TBLPathName;
	a_data.m_strTBL2PathName = m_EDIT_TBL2PathName;
	a_data.m_strExtName = m_EDIT_ExtName;
	a_data.m_bUseDirectory = m_cUseDirectory.GetCheck() != 0;
	a_data.m_bCheckTblOverlap = m_cCheckTblOverlap.GetCheck() != 0;
	a_data.m_bUseTBL2 = m_cUseTBL2.GetCheck() != 0;
	a_data.m_bTxtDirDefault = m_C文本在同目录.GetCheck() != 0;
	a_data.m_bSubDir = m_cSubDir.GetCheck() != 0;
	//
	a_data.m_bLenOverStop = m_cLenOverStop.GetCheck() != 0;
	a_data.m_uFill = m_cFill.GetCurSel();
	a_data.m_strFillByte = m_EDIT_SP1;
	a_data.m_strFillWord = m_EDIT_SP2;
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
	BOOL bError = FALSE;
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
			bError = TRUE;
			m_NodeName.Delete( i );
		}
	}
	if( bError )
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
	m_ImportDatas.push_back(SImportData());
	(*m_ImportDatas.rbegin()).m_strItemName = m_NodeName;
	UpdateImportData(*m_ImportDatas.rbegin());
	m_CList.SetCurSel( m_CList.AddString( m_NodeName ) );
#if USE_XML
	SaveXml();
#endif
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

	const SImportData& data = m_ImportDatas[sel];
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
	//
	m_cLenOverStop.SetCheck( data.m_bLenOverStop );

	m_cFill.SetCurSel( data.m_uFill );
	UpdateData( FALSE );
	OnCbnSelendokCombo1();

	m_EDIT_SP1 = data.m_strFillByte;
	m_EDIT_SP2 = data.m_strFillWord;
#undef DEF_FN_XYZ
	m_NodeName = data.m_strItemName;

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

	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = m_ImportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
	}

	m_CList.DeleteString( sel );
	m_CList.SetCurSel( -1 );

	m_ImportDatas.erase( m_ImportDatas.begin() + sel );
#if USE_XML
	SaveXml();
#endif
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
	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = m_ImportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
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

		m_ImportDatas[sel].m_strItemName = m_NodeName;

		m_CList.DeleteString( sel );
		m_CList.InsertString( sel , m_NodeName );
	}

	UpdateImportData(m_ImportDatas[sel]);
#if USE_XML
	SaveXml();
#endif
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
#if USE_XML
void CTXT_INBOX::LoadXml( TiXmlElement& a_Root )
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	m_ImportDatas.clear();

	const TiXmlElement* pNode;

	pNode = a_Root.FirstChildElement( "Import" );
	if( pNode )
	{
		for( pNode = pNode->FirstChildElement( "ImportItem" ) ; pNode ; pNode = pNode->NextSiblingElement( "ImportItem" ) )
		{
			const char* pNameUtf8 = pNode->Attribute( "Name" );
			if( !pNameUtf8 )
				continue;

			SImportData data;
			data.m_strItemName = Utf82Utf16le( pNameUtf8 );
#define DEF_FN_XYZ( b , c ) (pNode->Attribute( Utf16le2Utf8( b ) )?Utf82Utf16le(pNode->Attribute( Utf16le2Utf8( b ) )):c)

			data.m_strROMPath = DEF_FN_XYZ( L"ROMPath" , L"" );
			data.m_strTXTPath = DEF_FN_XYZ( L"TXTPath" , L"" );
			data.m_strTBLPathName = DEF_FN_XYZ( L"TBLPathName" , L"" );
			data.m_strTBL2PathName = DEF_FN_XYZ( L"TBL2PathName" , L"" );
			data.m_strExtName = DEF_FN_XYZ( L"ExtName" , L"" );
			data.m_bUseDirectory = 0 != _wtoi(DEF_FN_XYZ( L"UseDirectory" , L"0" ));
			data.m_bCheckTblOverlap = 0 != _wtoi(DEF_FN_XYZ( L"CheckTblOverlap" , L"1" ));
			data.m_bUseTBL2 = 0 != _wtoi(DEF_FN_XYZ( L"UseTBL2" , L"0" ));
			data.m_bTxtDirDefault = 0 != _wtoi(DEF_FN_XYZ( L"TxtDirDefault" , L"0" ));
			data.m_bSubDir = 0 != _wtoi(DEF_FN_XYZ( L"SubDir" , L"0" ));

			data.m_bLenOverStop = 0 != _wtoi(DEF_FN_XYZ( L"LenOverStop" , L"0" ));
			data.m_uFill = _wtoi(DEF_FN_XYZ( L"Fill" , L"0" ));
			data.m_strFillByte = DEF_FN_XYZ( L"FillByte" , L"20" );
			data.m_strFillWord = DEF_FN_XYZ( L"FillWord" , L"8140" );

#undef DEF_FN_XYZ
			m_ImportDatas.push_back( data );

			m_CList.AddString( data.m_strItemName );
		}
	}
}

void CTXT_INBOX::SaveXml()
{
	CConfigLockGuard guard = LockConfig();
	TiXmlElement& a_Root = guard.GetConfig();

	TiXmlNode*const pOldNode = a_Root.FirstChildElement( "Import" );
	TiXmlElement*const pNewNode = a_Root.LinkEndChild( new TiXmlElement( "Import" ) )->ToElement();

	std::vector<SImportData>::iterator it = m_ImportDatas.begin();

	for ( ; it != m_ImportDatas.end() ; ++it )
	{
		const SImportData& data = *it;
		TiXmlElement*const pNode = pNewNode->LinkEndChild( new TiXmlElement( "ImportItem" ) )->ToElement();

		pNode->SetAttribute( "Name" , Utf16le2Utf8( data.m_strItemName ) );
		pNode->SetAttribute( "ROMPath" , Utf16le2Utf8( data.m_strROMPath ) );
		pNode->SetAttribute( "TXTPath" , Utf16le2Utf8( data.m_strTXTPath ) );
		pNode->SetAttribute( "TBLPathName" , Utf16le2Utf8( data.m_strTBLPathName ) );
		pNode->SetAttribute( "TBL2PathName" , Utf16le2Utf8( data.m_strTBL2PathName ) );
		pNode->SetAttribute( "ExtName" , Utf16le2Utf8( data.m_strExtName ) );
		pNode->SetAttribute( "UseDirectory" , X2Utf8( data.m_bUseDirectory ) );
		pNode->SetAttribute( "CheckTblOverlap" , X2Utf8( data.m_bCheckTblOverlap ) );
		pNode->SetAttribute( "UseTBL2" , X2Utf8( data.m_bUseTBL2 ) );
		pNode->SetAttribute( "TxtDirDefault" , X2Utf8( data.m_bTxtDirDefault ) );
		pNode->SetAttribute( "SubDir" , X2Utf8( data.m_bSubDir ) );
		//
		pNode->SetAttribute( "LenOverStop" , X2Utf8( data.m_bLenOverStop ) );
		pNode->SetAttribute( "Fill" , X2Utf8( data.m_uFill ) );
		pNode->SetAttribute( "FillByte" , Utf16le2Utf8( data.m_strFillByte ) );
		pNode->SetAttribute( "FillWord" , Utf16le2Utf8( data.m_strFillWord ) );
	}

	if( pOldNode )
	{
		a_Root.RemoveChild( pOldNode );
		delete pOldNode;
	}

	SaveConfig();
}
#endif
