/*  OpenWQSG - WQSG ����(����)
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
// TXT_OutBox.cpp : ʵ���ļ�
#include "stdafx.h"
#include "WQSG ����(����).h"
#include "TXT_OutBox.h"
#include "WQSG_MAIN.h"
#include <Common/WQSG��������.h>

#define WM_WQSG_THREAD_MSG	( WM_USER + 1 )
#define WM_WQSG_THREAD_LOG		( WM_WQSG_THREAD_MSG + 1 )

static BOOL zzz��ͨ����( const CString& a_strFileName , CWQSG_TxtExport& WQSG , SExportDataEx& a_Data )
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
// CTXT_OutBox �Ի���
static DWORD WINAPI ��ͨ����_�ļ�(LPVOID lpParameter)
{
	SExportDataEx& data = *(SExportDataEx*)lpParameter;

	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�����ļ�ģʽ" );
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
		zzz��ͨ����( strFile , WQSG , data );
		/*
		if( !zzz��ͨ����( �ļ� , WQSG , data ) )
			goto __gt�˳�;
			*/
	}
	RunTime.Stop();
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"����ʱ:%d �� %d ʱ %d �� %d �� %d ����" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );
	}
__gtExit:
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�ر��ļ�ģʽ" );
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_MSG , 0 , 0 );
	return 0;
}
static BOOL zzz_��ͨ����_�ļ���( CStringW a_strPath , SExportDataEx& a_Data , CWQSG_TxtExport& WQSG , INT& countAll , std::vector<CStringW>& szExt )
{
	if( a_strPath.Right(1) != L'\\')
		a_strPath += L'\\';

	::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"���� " + a_strPath ).GetString() );

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
					zzz_��ͨ����_�ļ���( a_strPath + FindFileData.cFileName , a_Data , WQSG , count , szExt );
				/*	if( !zzz_��ͨ����_�ļ���( ·�� + FindFileData.cFileName , a_Data , WQSG , count , szExt ) )
						return FALSE;*/

					::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"���� " + a_strPath ).GetString() );
				}
			}
			else
			{
				for( int i = 0 ; (i >= 0) && ( (size_t)i < szExt.size() ) ; ++i )
				{
					if( fileName.Right( szExt[i].GetLength() ).MakeUpper() == szExt[i] )
					{
						if( zzz��ͨ����( a_strPath + FindFileData.cFileName , WQSG , a_Data ) )
						{
							++count;
							++countAll;
						}
						/*if( !zzz��ͨ����( ·�� + FindFileData.cFileName , WQSG , a_Data ) )
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
		a_strPath.Format( L"���� %u ���ļ�" , count );
		::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)a_strPath.GetString() );
	}
	else 
	{
		GetLastError();
	}
	return TRUE;
}
static DWORD WINAPI ��ͨ����_�ļ���(LPVOID lpParameter)
{
	SExportDataEx& data = *(SExportDataEx*)lpParameter;
///-------------------------------------------------
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�����ļ���ģʽ" );

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
	�ֽ�Ext( data.m_strExtName , szExt );
	if( szExt.size() == 0 )
		szExt.push_back( CStringW(L"") );
	//
	RunTime.Start();
	zzz_��ͨ����_�ļ���( data.m_strROMPath , data , WQSG , count , szExt );
	RunTime.Stop();
	//
	{
		WQSG_tgElapsedTime _tg;
		WQSG_Milliseconds2struct( RunTime.getElapsedMilliseconds() , _tg );
		CString str;
		str.Format( L"����ʱ:%d �� %d ʱ %d �� %d �� %d ����" , _tg.wDay ,_tg.wHour ,_tg.wMinute ,_tg.wSecond , _tg.wMilliseconds );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );

		str.Format( L"һ���ɹ����� %u ���ļ�" , count );
		::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)str.GetString() );
	}
__gtExit:
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�ر��ļ���ģʽ" );
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
	DDX_Control(pDX, IDC_CHECK3, m_C�ı���ͬĿ¼);
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
	ON_MESSAGE( WM_WQSG_THREAD_MSG , &CTXT_OutBox::�߳���Ϣ )
	ON_MESSAGE( WM_WQSG_THREAD_LOG , &CTXT_OutBox::�߳�LOG )
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if( m_C�ı���ͬĿ¼.GetCheck() != 0 )
	{
		m_Data.m_strTXTPath = L"";
	}
	else
	{
		if( !::WQSG_IsDir( m_EDIT_TXTPath.GetString() ) )
		{
			MessageBoxW( L"�ı�Ŀ¼����" );
			return;
		}
		m_Data.m_strTXTPath = m_EDIT_TXTPath;
		if( m_Data.m_strTXTPath.Right(1) != L'\\')
			m_Data.m_strTXTPath += L'\\';
	}
	if( !::WQSG_IsFile( m_EDIT_TBLPathName.GetString() ) )
	{
		MessageBoxW( L"�������·��" );
		return;
	}
	if( (m_cUseTBL2.GetCheck() != 0 ) && !::WQSG_IsFile( m_EDIT_TBL2PathName.GetString() ) )
	{
		MessageBoxW( L"����������·��" );
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
		MessageBoxW( L"����\"�κ�\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_Data.m_uBeginOffset = val;
	m_Data.m_uBeginOffset <<= 32;
	m_Data.m_uEndOffset = m_Data.m_uBeginOffset;

	if ( 1 != swscanf( m_EDIT_BeginOffset , L"%X" , &val ) )
	{
		MessageBoxW( L"����\"��ʼƫ��\"!!" );
		GetDlgItem( IDC_EDIT_ADDR_KS_L )->SetFocus();
		return;
	}
	m_Data.m_uBeginOffset |= val;

	if ( 1 != swscanf( m_EDIT_EndOffset , L"%X" , &val ) )
	{
		MessageBoxW( L"����\"����ƫ��\"!!" );
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
			MessageBox( L"��չ������");
			return;
		}
		m_Data.m_strExtName = ( m_EDIT_ExtName.GetLength() > 0 )?m_EDIT_ExtName:L"*";

		m_Data.m_strROMPath = m_EDIT_ROMPath;
		if( m_Data.m_strROMPath.Right(1) != L'\\')
			m_Data.m_strROMPath += L'\\';

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"���ڵ���..." );
		handle = CreateThread( NULL , NULL
		, ��ͨ����_�ļ���
		, &m_Data , NULL , NULL );
	}
	else
	{
		static CWQSGFileDialog_OpenS fileDlg( L"ROM�ļ�(*.*)|*.*||" );
		fileDlg.SetWindowTitle( L"ѡ��Ҫ�����ı����ļ�(��ѡ)..." );

		if( fileDlg.DoModal() != IDOK )
			return;

		POSITION pos = fileDlg.GetStartPosition();
		CString strFile;
		while( fileDlg.GetNextPathName( strFile , pos ) )
		{
			m_Data.m_Files.push_back( strFile );
		}

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"���ڵ���..." );

		handle = CreateThread( NULL , NULL
			, ��ͨ����_�ļ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXTPath = path;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	static CWQSGFileDialog_Open dlg( L"����ļ�(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBLPathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonTbl2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"��������ļ�(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBL2PathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	const BOOL ѡ�� = m_cUseDirectory.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( ѡ�� );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( ѡ�� );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( ѡ�� );
}

BOOL CTXT_OutBox::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Data.m_hWnd = m_hWnd;

	m_cCheckTblOverlap.SetCheck(TRUE);

	m_C�ı���ͬĿ¼.SetCheck( TRUE );
	OnBnClickedCheck3();

	LoadFromData();

	return TRUE;
}

void CTXT_OutBox::OnEnChangeEditAddrH()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	EditCheckHaxStr( m_EDIT_SegmentAddr , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_H ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrKsL()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	EditCheckHaxStr( m_EDIT_BeginOffset , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_KS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnChangeEditAddrJsL()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	EditCheckHaxStr( m_EDIT_EndOffset , (CEdit*)GetDlgItem( IDC_EDIT_ADDR_JS_L ) , this , TRUE );
}

void CTXT_OutBox::OnEnKillfocusEditAddrH()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if( m_EDIT_SegmentAddr.GetLength() == 0)
	{
		m_EDIT_SegmentAddr = L"0";
		UpdateData( FALSE );
	}
}

void CTXT_OutBox::OnEnKillfocusEditMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if( m_EDIT_min.GetLength() == 0)
	{
		m_EDIT_min = L"0";
		UpdateData( FALSE );
	}
}

void CTXT_OutBox::OnEnKillfocusEditMax()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if( m_EDIT_max.GetLength() == 0)
	{
		m_EDIT_max = L"9999";
		UpdateData( FALSE );
	}
}
LRESULT CTXT_OutBox::�߳���Ϣ( WPARAM ���� , LPARAM �ı� )
{
	if( 0 == �ı� )
	{
		AppLog( (WCHAR*)�ı� );
//		MessageBox(L"�������!!");
	}

	g_pMAIN_CWND->SendMessage( WM_WQSG_SetText , 0 , �ı� );
	return 0;
}

LRESULT CTXT_OutBox::�߳�LOG( WPARAM ���� , LPARAM �ı� )
{
	AppLog( (WCHAR*)�ı� );
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetLogText , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}
void CTXT_OutBox::OnBnClickedButtonRom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_ROMPath = path;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnEnChangeEditExt()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_EDIT_ExtName.TrimLeft();
	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LOG = L"";
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_OutBox::OnBnClickedCheck4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_cUseTBL2.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox( L"�Ѿ�������ͬ������");
			return;
		}
	}
	CGlobalData::m_ExportDatas.push_back(SExportData());
	(*CGlobalData::m_ExportDatas.rbegin()).m_strItemName = m_NodeName;
	UpdateExportData(*CGlobalData::m_ExportDatas.rbegin());
	m_CList.SetCurSel( m_CList.AddString( m_NodeName ) );
#if USE_XML
	CGlobalData::SaveXml();
#endif
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
	a_data.m_bTxtDirDefault = m_C�ı���ͬĿ¼.GetCheck() != 0;
	a_data.m_bSubDir = m_cSubDir.GetCheck() != 0;
}

void CTXT_OutBox::OnLbnSelchangeList2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )return;

	const SExportData& data = CGlobalData::m_ExportDatas[sel];
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

	m_C�ı���ͬĿ¼.SetCheck( data.m_bTxtDirDefault );
	UpdateData( FALSE );
	OnBnClickedCheck3();

	m_cSubDir.SetCheck( data.m_bSubDir );
#undef DEF_FN_XYZ
	m_NodeName = data.m_strItemName;

	UpdateData( FALSE );
}

void CTXT_OutBox::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"��~~~~��ûѡ����ôɾ" );
		return;
	}

	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = CGlobalData::m_ExportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
	}
	CGlobalData::m_ExportDatas.erase( CGlobalData::m_ExportDatas.begin() + sel );

	m_CList.DeleteString( sel );
	m_CList.SetCurSel( -1 );
#if USE_XML
	CGlobalData::SaveXml();
#endif
}

void CTXT_OutBox::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"�ҳ�~~~~��ûѡ����ô��" );
		return;
	}

	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = CGlobalData::m_ExportDatas[sel].m_strItemName;
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
				MessageBox( L"�Ѿ�������ͬ������");
				return;
			}
		}

		CGlobalData::m_ExportDatas[sel].m_strItemName = m_NodeName;

		m_CList.DeleteString( sel );
		m_CList.InsertString( sel , m_NodeName );
	}

	UpdateExportData( CGlobalData::m_ExportDatas[sel] );
#if USE_XML
	CGlobalData::SaveXml();
#endif
}

void CTXT_OutBox::OnEnKillfocusEditName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData( FALSE );
}

void CTXT_OutBox::OnEnChangeEditName()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	m_NodeName.Trim( L' ' );
	m_NodeName.Trim( L'��' );

	WCHAR wCh;
	BOOL ��ʾ = FALSE;
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
			��ʾ = TRUE;
			m_NodeName.Delete( i );
		}
	}
	if( ��ʾ )
		AppLog( L"����ֻ��ʹ�� ��ĸ ����  �»��� �Լ� ȫ���ַ�" );


	if( ( ( wCh = m_NodeName.GetAt( 0 ) ) >= L'0' ) && ( wCh <= L'9' ) )
	{
		while( ( ( wCh = m_NodeName.GetAt( 0 ) ) >= L'0' ) && ( wCh <= L'9' ) )
		{
			m_NodeName.Delete( 0 );
		}
		AppLog( L"���Ʋ��������ֿ�ͷ" );
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem( IDC_EDIT_TXT_DIR )->EnableWindow( m_C�ı���ͬĿ¼.GetCheck() == 0 );
}

void CTXT_OutBox::LoadFromData()
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	for(std::vector<SExportData>::iterator it = CGlobalData::m_ExportDatas.begin() ; it != CGlobalData::m_ExportDatas.end() ; ++it)
	{
		const SExportData& data = *it;
		m_CList.AddString( data.m_strItemName );
	}
}
