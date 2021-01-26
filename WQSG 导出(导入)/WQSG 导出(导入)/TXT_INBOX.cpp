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
// TXT_INBOX.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WQSG ����(����).h"
#include "TXT_INBOX.h"
#include "WQSG_MAIN.h"
#include <Common/WQSG��������.h>

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

	return WQSG.ImportText( a_strFileName.GetString() , strTxtPathName.GetString() , ( (a_Data.m_���ֽ�)?(&a_Data.m_SP1):NULL ) , ( (a_Data.m_˫�ֽ�)?(&a_Data.m_SP2):NULL ) , a_Data.m_bLenOverStop );
}
// CTXT_INBOX �Ի���
static DWORD WINAPI Import_File(LPVOID lpParameter)
{
	SImportDataEx& data = *(SImportDataEx*)lpParameter;
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�����ļ�ģʽ" );

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
		if( !zzz��ͨ����( �ļ� , WQSG , a_Data ) )
			break;*/
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
static BOOL zzz_Import_Dir( CStringW a_strPath , SImportDataEx& a_Data , CWQSG_TxtImport& WQSG , INT& countAll , std::vector<CStringW>& szExt )
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
					zzz_Import_Dir( a_strPath + FindFileData.cFileName , a_Data , WQSG , count , szExt );
/*
					if( !zzz_��ͨ����_�ļ���( ·�� + FindFileData.cFileName , a_Data , WQSG , count , szExt ) )
						return FALSE;
*/
					::SendMessage( a_Data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)( L"���� " + a_strPath ).GetString() );
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
						if( !zzz��ͨ����( ·�� + FindFileData.cFileName , WQSG , a_Data ) )
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
static DWORD WINAPI Import_Dir(LPVOID lpParameter)
{
	SImportDataEx& data = *(SImportDataEx*)lpParameter;
///-------------------------------------------------
	::SendMessage( data.m_hWnd , WM_WQSG_THREAD_LOG , 0 , (LPARAM)L"�����ļ���ģʽ" );

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

	�ֽ�Ext( data.m_strExtName , szExt );
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
	DDX_Control(pDX, IDC_CHECK3, m_C�ı���ͬĿ¼);
	DDX_Control(pDX, IDC_CHECK5, m_cSubDir);
}
BEGIN_MESSAGE_MAP(CTXT_INBOX, CBaseDialog)
	ON_BN_CLICKED(IDC_CHECK2, &CTXT_INBOX::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_ROM, &CTXT_INBOX::OnBnClickedButtonRom)
	ON_BN_CLICKED(IDC_BUTTON_TXT_DIR, &CTXT_INBOX::OnBnClickedButtonTxtDir)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &CTXT_INBOX::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &CTXT_INBOX::OnBnClickedButtonTbl2)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTXT_INBOX::OnBnClickedButtonStart)
	ON_MESSAGE( WM_WQSG_THREAD_MSG , &CTXT_INBOX::�߳���Ϣ )
	ON_MESSAGE( WM_WQSG_THREAD_LOG , &CTXT_INBOX::�߳�LOG )
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CTXT_INBOX::OnBnClickedButtonRom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_ROMPath = path;

	UpdateData( FALSE );
}
void CTXT_INBOX::OnBnClickedButtonTxtDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CWQSG_DirDlg DirDlg( m_hWnd );
	WCHAR path[ MAX_PATH ];
	if( !DirDlg.GetPath( path ) )return;
	m_EDIT_TXTPath = path;

	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonTbl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"����ļ�(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBLPathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonTbl2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"��������ļ�(*.TBL,*.TXT)|*.TBL;*.TXT||" );

	if( IDOK != dlg.DoModal() )
		return;

	m_EDIT_TBL2PathName = dlg.GetPathName();
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedButtonStart()
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

	m_Data.m_bLenOverStop = (m_cLenOverStop.GetCheck() != 0);

	m_Data.m_���ֽ� = m_Data.m_˫�ֽ� = FALSE;

	switch ( m_cFill.GetCurSel() )
	{
	case 1:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( FALSE );
		m_Data.m_���ֽ� = TRUE;
		break;
	case 2:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_Data.m_˫�ֽ� = TRUE;
		break;
	case 3:
		GetDlgItem( IDC_EDIT_SP1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_SP2 )->EnableWindow( TRUE );
		m_Data.m_���ֽ� = TRUE;
		m_Data.m_˫�ֽ� = TRUE;
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
			MessageBox( L"��չ������");
			return;
		}
		m_Data.m_strExtName = ( m_EDIT_ExtName.GetLength() > 0 )?m_EDIT_ExtName:L"*";

		m_Data.m_strROMPath = m_EDIT_ROMPath;
		if( m_Data.m_strROMPath.Right(1) != L'\\')
			m_Data.m_strROMPath += L'\\';

		SendMessage( WM_WQSG_THREAD_MSG , 0 , (LPARAM)L"���ڵ���..." );
		handle = CreateThread( NULL , NULL
			, Import_Dir
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
LRESULT CTXT_INBOX::�߳���Ϣ( WPARAM ���� , LPARAM �ı� )
{
	if( 0 == �ı� )
	{
		AppLog( (WCHAR*)�ı� );
//		MessageBox(L"�������!!");
	}
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetText , 0 , �ı� );
	return 0;
}
LRESULT CTXT_INBOX::�߳�LOG( WPARAM ���� , LPARAM �ı� )
{
	AppLog( (WCHAR*)�ı� );
	g_pMAIN_CWND->SendMessage( WM_WQSG_SetLogText , 0 , (LPARAM)m_LOG.GetString() );
	return 0;
}

void CTXT_INBOX::OnOK(){}
void CTXT_INBOX::OnCancel(){}
BOOL CTXT_INBOX::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Data.m_hWnd = m_hWnd;

	m_cFill.SetCurSel( 0 );
	m_cCheckTblOverlap.SetCheck(TRUE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_C�ı���ͬĿ¼.SetCheck( TRUE );
	OnBnClickedCheck3();

	LoadFromData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CTXT_INBOX::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}
void CTXT_INBOX::OnCbnSelendokCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	EditCheckHaxStr( m_EDIT_SP1 , (CEdit*)GetDlgItem( IDC_EDIT_SP1 ) , this , FALSE );
}

void CTXT_INBOX::OnEnChangeEditSp2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	EditCheckHaxStr( m_EDIT_SP2 , (CEdit*)GetDlgItem( IDC_EDIT_SP2 ) , this , FALSE );
}

void CTXT_INBOX::OnEnKillfocusEditSp1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_cUseTBL2.GetCheck() != 0 )
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( TRUE );
	else
		GetDlgItem( IDC_EDIT_TBL2 )->EnableWindow( FALSE );
}

void CTXT_INBOX::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const BOOL bSel = m_cUseDirectory.GetCheck() != 0;
	GetDlgItem( IDC_EDIT_ROM )->EnableWindow( bSel );
	GetDlgItem( IDC_EDIT_EXT )->EnableWindow( bSel );
	GetDlgItem( IDC_CHECK5 )->EnableWindow( bSel );
}

void CTXT_INBOX::OnEnChangeEditExt()
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

void CTXT_INBOX::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	a_data.m_bTxtDirDefault = m_C�ı���ͬĿ¼.GetCheck() != 0;
	a_data.m_bSubDir = m_cSubDir.GetCheck() != 0;
	//
	a_data.m_bLenOverStop = m_cLenOverStop.GetCheck() != 0;
	a_data.m_uFill = m_cFill.GetCurSel();
	a_data.m_strFillByte = m_EDIT_SP1;
	a_data.m_strFillWord = m_EDIT_SP2;
}

void CTXT_INBOX::OnEnChangeEditName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	m_NodeName.Trim( L' ' );
	m_NodeName.Trim( L'��' );

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

void CTXT_INBOX::OnBnClickedButtonAdd()
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
	CGlobalData::m_ImportDatas.push_back(SImportData());
	(*CGlobalData::m_ImportDatas.rbegin()).m_strItemName = m_NodeName;
	UpdateImportData(*CGlobalData::m_ImportDatas.rbegin());
	m_CList.SetCurSel( m_CList.AddString( m_NodeName ) );
#if USE_XML
	CGlobalData::SaveXml();
#endif
}

void CTXT_INBOX::AppLog(CString str)
{
	m_LOG += (str + L"\r\n");
	m_CEDIT_LOG.SetWindowTextW( m_LOG );
}

void CTXT_INBOX::OnLbnSelchangeList2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )return;

	const SImportData& data = CGlobalData::m_ImportDatas[sel];
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

	m_C�ı���ͬĿ¼.SetCheck( data.m_bTxtDirDefault );
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

		CString strNodeName = CGlobalData::m_ImportDatas[sel].m_strItemName;
		ASSERT( strNodeName.MakeLower() == NodeName.MakeLower() );
	}

	m_CList.DeleteString( sel );
	m_CList.SetCurSel( -1 );

	CGlobalData::m_ImportDatas.erase( CGlobalData::m_ImportDatas.begin() + sel );
#if USE_XML
	CGlobalData::SaveXml();
#endif
}

void CTXT_INBOX::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int sel = m_CList.GetCurSel();
	if( sel < 0 )
	{
		MessageBox( L"�г�~~~~��ûѡ����ô��" );
		return;
	}
	{
		CString NodeName;
		m_CList.GetText( sel , NodeName );

		CString strNodeName = CGlobalData::m_ImportDatas[sel].m_strItemName;
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
				MessageBox( L"�Ѿ�������ͬ������");
				return;
			}
		}

		CGlobalData::m_ImportDatas[sel].m_strItemName = m_NodeName;

		m_CList.DeleteString( sel );
		m_CList.InsertString( sel , m_NodeName );
	}

	UpdateImportData(CGlobalData::m_ImportDatas[sel]);
#if USE_XML
	CGlobalData::SaveXml();
#endif
}

void CTXT_INBOX::OnEnKillfocusEditName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData( FALSE );
}

void CTXT_INBOX::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem( IDC_EDIT_TXT_DIR )->EnableWindow( m_C�ı���ͬĿ¼.GetCheck() == 0 );
}

void CTXT_INBOX::LoadFromData()
{
	while( m_CList.GetCount() )
	{
		m_CList.DeleteString( 0 );
	}

	for(std::vector<SImportData>::iterator it = CGlobalData::m_ImportDatas.begin() ; it != CGlobalData::m_ImportDatas.end() ; ++it)
	{
		const SImportData& data = *it;
		m_CList.AddString( data.m_strItemName );
	}
}

