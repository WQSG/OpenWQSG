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
// 批量文本替换Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "批量文本替换Dlg.h"


// C批量文本替换Dlg 对话框

IMPLEMENT_DYNAMIC(C批量文本替换Dlg, CBaseDialog)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C批量文本替换Dlg 对话框


C批量文本替换Dlg::C批量文本替换Dlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(C批量文本替换Dlg::IDD, pParent)
	, m_查找内容(_T(""))
	, m_替换为(_T(""))
	, m_替换表(_T(""))
{
}

void C批量文本替换Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_查找内容);
	DDV_MaxChars(pDX, m_查找内容, 255);
	DDX_Text(pDX, IDC_EDIT2, m_替换为);
	DDV_MaxChars(pDX, m_替换为, 255);
	DDX_Control(pDX, IDC_CHECK1, m_C高级替换);
	DDX_Control(pDX, IDC_CHECK3, m_C替换子目录);
	DDX_Text(pDX, IDC_EDIT3, m_替换表);
	DDV_MaxChars(pDX, m_替换表, 260);
}

BEGIN_MESSAGE_MAP(C批量文本替换Dlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &C批量文本替换Dlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &C批量文本替换Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &C批量文本替换Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C批量文本替换Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C批量文本替换Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// C批量文本替换Dlg 消息处理程序

BOOL C批量文本替换Dlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	OnBnClickedCheck1();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C批量文本替换Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	m_完成数量 = 0;
	m_替换链表[0].DelAll();
	m_替换链表[1].DelAll();
	if( m_查找内容.GetLength() <= 0 )
	{
		MessageBox( L"查找内容不能为空" );
		return;
	}
	if( m_替换为.GetLength() <= 0 )
	{
		MessageBox( L"替换为不能为空" );
		return;
	}
	m_替换链表[0].AddItem( m_查找内容 );
	m_替换链表[1].AddItem( m_替换为 );

	static CWQSGFileDialog_OpenS fopendlg( _T("文本文件(*.TXT)|*.TXT||") );
	fopendlg.SetWindowTitle( _T("选择要替换的文本...") );

	if( IDOK != fopendlg.DoModal() )
		return;

	POSITION pos = fopendlg.GetStartPosition();
	UINT 数量 = 0;
	CString str;
	while( fopendlg.GetNextPathName( str , pos) )
	{
		if( !替换文本( str ) )
			break;
	}
	str.Format( L"成功替换 %d 个" , m_完成数量 );
	MessageBox( str );
}



void C批量文本替换Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseDialog::OnClose();
	CBaseDialog::OnCancel();
}
void C批量文本替换Dlg::OnCancel(){}
void C批量文本替换Dlg::OnOK(){}
void C批量文本替换Dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_C高级替换.GetCheck() != 0 )
	{
		GetDlgItem( IDC_EDIT1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );

		GetDlgItem( IDC_EDIT3 )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON3 )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON4 )->EnableWindow( TRUE );
		GetDlgItem( IDC_CHECK3 )->EnableWindow( TRUE );

		
	}
	else
	{
		GetDlgItem( IDC_EDIT1 )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT2 )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( TRUE );

		GetDlgItem( IDC_EDIT3 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON3 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON4 )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK3 )->EnableWindow( FALSE );
	}
}

void C批量文本替换Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CWQSGFileDialog_Open dlg( L"*.TXT,*.TBL|*.TXT;*.TBL||" );
	dlg.SetWindowTitle( L"选择一个\"替换表\"" );

	if( dlg.DoModal() != IDOK )
		return;

	m_替换表 = dlg.GetPathName();
	UpdateData( FALSE );
}

void C批量文本替换Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( !载入替换表() )
		return;

	static CWQSGFileDialog_OpenS WQSG( _T("*.文本|*.TXT|") );
	WQSG.SetWindowTitle( _T("请选择要参加替换的文本...") );

	if(WQSG.DoModal() != IDOK)
	{
		return;
	}

	POSITION pos = WQSG.GetStartPosition();

	CString str;

	while(WQSG.GetNextPathName(str,pos))
	{
		if( !替换文本( str ) )
			break;
	}

	if( m_完成数量 > 0 )
	{
		str.Format( L"已经替换 %u 个文件" , m_完成数量 );
		MessageBox( str );
	}
}

void C批量文本替换Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( !载入替换表() )return;

	CWQSG_DirDlg dlg( m_hWnd );
	WCHAR path[ MAX_PATH * 2 ];
	if( !dlg.GetPath( path ) ) return;

	CString dirPath( path );

	替换目录( path );
	if( m_完成数量 > 0 )
	{
		CString str;
		str.Format( L"已经替换 %u 个文件" , m_完成数量 );
		MessageBox( str );
	}
	
}

bool C批量文本替换Dlg::载入替换表(void)
{
	m_完成数量 = 0;
	m_替换链表[0].DelAll();
	m_替换链表[1].DelAll();

	if( m_C高级替换.GetCheck() != 0 )
	{
		::CMemTextW tfp;
		if( !tfp.Load( m_替换表.GetString() , 1024*1024*16 ) )
		{
			MessageBox( L"替换表\r\n" + m_替换表 + L"\r\n" + tfp.GetErrTXT() );
			return false;
		}
		const WCHAR* pStr;
		while( pStr = tfp.GetLine() )
		{
			CString str( pStr );
			delete[]pStr;
			if( str.GetLength() <= 0 )
				continue;

			int pos;
			if( ( pos = str.Find( L'=') ) < 0 )
			{
				MessageBox( str + L"\r\n找不到 \'=\' " );
				return false;;
			}
			CString strL( str.Left( pos ) );
			CString strR( str.Mid( pos + 1 ) );
			if( strR.GetLength() <= 0 )
				continue;

			if( ( pos = m_替换链表[0].AddItem( strL ) ) < 0 )
			{
				MessageBox( str + L"\r\n内存不足??" );
				return false;
			}
			if( m_替换链表[1].AddItem( strR ) < 0 )
			{
				m_替换链表[0].DelItem( pos );
				MessageBox( str + L"\r\n内存不足??" );
				return false;
			}
		}
	}
	else
	{
		if( m_查找内容.GetLength() == 0 )
		{
			MessageBox(_T("查找内容不能为空"));
			return false;
		}
		int pos ;
		if( ( pos = m_替换链表[0].AddItem( m_查找内容 ) ) < 0 )
		{
			MessageBox( L"\r\n内存不足??" );
			return false;
		}
		if( m_替换链表[1].AddItem( m_替换为 ) < 0 )
		{
			m_替换链表[0].DelItem( pos );
			MessageBox( L"\r\n内存不足??" );
			return false;
		}
	}
	return true;
}

bool C批量文本替换Dlg::替换目录(CString 路径)
{
	if( 路径.Right(1) != L'\\' )
		路径 += L'\\';

	WIN32_FIND_DATA FindFileData;

	bool rev = false;
	HANDLE hFind = FindFirstFile( 路径 + L"*.*" , &FindFileData );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		rev = true;
		do
		{
			CString 文件( FindFileData.cFileName );

			if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( ( m_C替换子目录.GetCheck() != 0 ) && ( 文件 != "." ) && ( 文件 != ".." ) )
				{
					if( !替换目录( 路径 + FindFileData.cFileName ) )
					{
						rev = false;
						break;
					}
				}
			}
			else
			{
				CString 扩展名( 文件.Right(4) );
				扩展名 = 扩展名.MakeUpper();
				if( 扩展名 == L".TXT" )
				{
					if( !替换文本( 路径 + FindFileData.cFileName ) )
					{
						rev = false;
						break;
					}
				}
			}
		}
		while( FindNextFileW( hFind , &FindFileData ) );
		FindClose( hFind );
	}
	else
	{
		return false;
	}
	return rev;
}

bool C批量文本替换Dlg::替换文本(CString 文件)
{
	::CMemTextW tfp;
	CWQSG_File WFile;

	if( !tfp.Load( 文件.GetString() , 1024*1024*16 ) )
	{
		MessageBox( L"文件打开失败" , 文件 );
		return false;
	}

	CString 字符串( tfp.GetText() );
	tfp.Clear();

	CString 查找内容,替换为;
	for( int i = 0; m_替换链表[0].GetItem( i , 查找内容 ) && m_替换链表[1].GetItem( i , 替换为 );++i )
	{
		字符串.Replace( 查找内容 , 替换为 );
	}

	if( !WFile.OpenFile( 文件.GetString() , 4 , 3 ) )
	{
		MessageBox( L"打开失败" , 文件 );
		return false;
	}
	if( ( 2 != WFile.Write("\xFF\xFE",2) ) ||
		( (字符串.GetLength()<<1) != WFile.Write( 字符串.GetString() , 字符串.GetLength()<<1 ) ) )
	{
		MessageBox( L"文件失败" , 文件 );
		return false;
	}
	WFile.Close();

	++m_完成数量;
	return true;
}
