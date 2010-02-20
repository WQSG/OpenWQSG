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
// chazhiSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "chazhiSearch.h"


template <typename TYPE_1>
class C相对搜索
{
	HWND m_hWnd;
	CString m_标题;
	INT MessageBox( CString str , UINT 按钮 = MB_OK )
	{
		return ::MessageBox( m_hWnd , str , m_标题 , 按钮 );
	}
public:
	C相对搜索( HWND hwnd , CString 标题 );
	void 开始搜索0( CString& log , CString 文件名 , CWQSG_TypeLinkList<s64>& 插值表 )
	{
		CWQSG_File fp;
		if( !fp.OpenFile( 文件名.GetString() , 1 , 3 ) )
		{
			MessageBox( L"打开文件失败" );
			return;
		}
		u64 fileSize;
		fp.GetFileSize( &fileSize );

		CWD_BIN bin;
		{
			u32 余数 = (u32)( fileSize % sizeof(TYPE_1) );
			fileSize -= 余数;
			bin.LEN = ( fileSize > (1024*1024*32) )?(1024*1024*32):(u32)fileSize;
		}
		if( NULL == bin.SetBufSize( bin.LEN ) )
		{
			MessageBox( L"申请内存失败" );
			return;
		}
		//-----------------------------------------------------------
		const int itemCount = 插值表.GetCount();
		u64 filePos = 0;
		u32 计数器 = 0;
		while( u64 size = (fileSize - filePos) )
		{
			fp.Seek( filePos );

			if( size > bin.LEN )
			{
				if( bin.LEN != fp.Read( bin.BIN , bin.LEN ) )
				{
					MessageBox( L"读取文件失败" );
					return;
				}
				size = bin.LEN;
			}
			else
			{
				if( (u32)size != fp.Read( bin.BIN , (u32)size ) )
				{
					MessageBox( L"读取文件失败" );
					return;
				}
			}
			size -= ( size%sizeof(TYPE_1) );

			TYPE_1* sp1 = (TYPE_1*)bin.BIN;
			TYPE_1* 界限A = sp1 + ( (u32)size / sizeof(TYPE_1) );

			if( 界限A <= (sp1 + itemCount) )
			{
				break;
			}
			TYPE_1* 界限B = 界限A - itemCount;
			while( sp1 < 界限B )
			{
				TYPE_1 上一个 = *(sp1++);
				TYPE_1* sp0 = sp1;

				u32 偏移计数 = 1;
				int i_count;
				for( i_count = 0 ; (i_count < itemCount) && ( sp1 <= 界限A );
					++i_count , ++偏移计数 , ++sp1 )
				{
#ifdef DEBUG
					s64 fffffff = 上一个;
#endif
					s64 f1 = *sp1 - 上一个;
					上一个 = *sp1;
					s64 f2;
					if( (!插值表.GetItem( i_count , f2 )) || ( f1 != f2 ) )
					{
						sp1 = sp0;
						偏移计数 = 1;
						break;
					}
					INT X = 0;
				}

				if( itemCount == i_count )
				{
					CString str;
					str.Format( L"%08X:%08X\r\n" , (INT)(filePos>>32) , (INT)(filePos&0xFFFFFFFF) );
					log += str;
					if( ++计数器 > 10000 )
					{
						filePos = fileSize;
						MessageBox( L"为了安全起见,只显示前10000项" );
						break;
					}
				}
				filePos += ( 偏移计数 * sizeof(TYPE_1) );
			}
		}
		MessageBox( L"查找结束" );
	}
	void 开始搜索1( CString& log , CString 文件名 , CWQSG_TypeLinkList<s64>& 插值表 )
	{
		WQSG_File_mem mapfp;
		if( !mapfp.OpenFile( 文件名.GetString() , 3 ) )
		{
			MessageBox( L"打开文件失败" );
			return;
		}
		const s64 fileSize = mapfp.GetFileSize( );
		//-----------------------------------------------------------
		const int itemCount = 插值表.GetCount();
		s64 filePos = 0;
		u32 计数器 = 0;
		const u32 len8 = 1024*1024*128;
		while( filePos < fileSize )
		{
			u64 size = (fileSize - filePos);
			const u8* bin8;
			if( size > len8 )
			{
				bin8 = mapfp.GetPtr( filePos , len8 );
				size = len8;
			}
			else
			{
				bin8 = mapfp.GetPtr( filePos , (u32)size );
			}

			if( NULL == bin8 )
			{
				MessageBox( L"读取文件失败" );
				return;
			}

			size -= ( size%sizeof(TYPE_1) );

			const TYPE_1* sp1 = (TYPE_1*)bin8;
			const TYPE_1* 界限A = sp1 + ( (u32)size / sizeof(TYPE_1) );

			if( 界限A <= (sp1 + itemCount) )
			{
				mapfp.Free();
				break;
			}
			const TYPE_1* 界限B = 界限A - itemCount;
			while( sp1 < 界限B )
			{
				TYPE_1 上一个 = *(sp1++);
				const TYPE_1* sp0 = sp1;

				u32 偏移计数 = 1;
				int i_count;
				for( i_count = 0 ; (i_count < itemCount) && ( sp1 <= 界限A );
					++i_count , ++偏移计数 , ++sp1 )
				{
#ifdef DEBUG
					s64 fffffff = 上一个;
#endif
					s64 f1 = *sp1 - 上一个;
					上一个 = *sp1;
					s64 f2;
					if( (!插值表.GetItem( i_count , f2 )) || ( f1 != f2 ) )
					{
						sp1 = sp0;
						偏移计数 = 1;
						break;
					}
					INT X = 0;
				}

				if( itemCount == i_count )
				{
					CString str;
					str.Format( L"%08X:%08X\r\n" , (INT)(filePos>>32) , (INT)(filePos&0xFFFFFFFF) );
					log += str;
					if( ++计数器 > 10000 )
					{
						filePos = fileSize;
						MessageBox( L"为了安全起见,只显示前10000项" );
						break;
					}
				}
				filePos += ( 偏移计数 * sizeof(TYPE_1) );
			}
			mapfp.Free();
		}
		MessageBox( L"查找结束" );
	}
};
#define DEF_XXX( DEF_X ) C相对搜索<DEF_X>::C相对搜索( HWND hwnd , CString 标题 ): m_hWnd( hwnd ), m_标题(标题){}
DEF_XXX(u8)
DEF_XXX(u16)
DEF_XXX(u32)
#undef DEF_XXX
// C相对搜索Dlg 对话框
IMPLEMENT_DYNAMIC(CchazhiSearch, CDialog)
CchazhiSearch::CchazhiSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CchazhiSearch::IDD, pParent)
	, m_input(_T(""))
	, m_字节类型(_T("单字节"))
	, m_输入类型(_T("十六进制"))
	, m_log(_T(""))
	, m_文件路径(_T(""))
{
}

void CchazhiSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_input);
	DDV_MaxChars(pDX, m_input, 10);
	DDX_CBString(pDX, IDC_COMBO2, m_字节类型);
	DDX_CBString(pDX, IDC_COMBO1, m_输入类型);
	DDX_Text(pDX, IDC_EDIT1, m_log);
	DDX_Control(pDX, IDC_CHECK1, m_C高低交换);
	DDX_Text(pDX, IDC_EDIT3, m_文件路径);
	DDV_MaxChars(pDX, m_文件路径, 255);
	DDX_Control(pDX, IDC_LIST99, m_CList);
}

BEGIN_MESSAGE_MAP(CchazhiSearch, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CchazhiSearch::OnBnClickedButton1)
	ON_CBN_SELENDOK(IDC_COMBO1, &CchazhiSearch::OnCbnSelendokCombo1)
	ON_EN_CHANGE(IDC_EDIT2, &CchazhiSearch::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON4, &CchazhiSearch::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CchazhiSearch::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CchazhiSearch::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON5, &CchazhiSearch::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK1, &CchazhiSearch::OnBnClickedCheck1)
END_MESSAGE_MAP()
BOOL CchazhiSearch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	m_CList.InsertColumn( 0 , L"输入值" , 0 , 60 );
	m_CList.InsertColumn( 1 , L"差值" , 0 , 60 );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// 如果向对话框添加最小化按钮，则需要下面的代码
void CchazhiSearch::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
	CDialog::OnCancel();
}
void CchazhiSearch::OnCancel(){}
void CchazhiSearch::OnOK(){}
void CchazhiSearch::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_input.GetLength() <= 0 )
	{
		MessageBox( L"不能添加空白的值" );
		return;
	}
	UINT val = 0;

	if( m_输入类型 == L"十六进制" )
	{
		swscanf( m_input.GetString() , L"%x" , &val );
	}
	else
	{
		val = _wtoi( m_input.GetString() );
	}
	WCHAR* 格式 = L"??";
	if( m_字节类型 == L"单字节" )
	{
		if( val > 0xFF )
		{
			MessageBox( L"输入数字过大" );
			return;
		}
		格式 = L"%02X";
	}
	else if( m_字节类型 == L"双字节" )
	{
		if( val > 0xFFFF )
		{
			MessageBox( L"输入数字过大" );
			return;
		}
		格式 = L"%04X";
		if( m_C高低交换.GetCheck() != 0 )
		{
			u8* ptr = (u8*)&val;
			u8 tmp = ptr[0];
			ptr[0] = ptr[1];
			ptr[1] = tmp;
		}
	}
	else if( m_字节类型 == L"四字节" )
	{
		if( val > 0xFFFFFFFF )
		{
			MessageBox( L"输入数字过大" );
			return;
		}
		格式 = L"%08X";
		if( m_C高低交换.GetCheck() != 0 )
		{
			u8* ptr = (u8*)&val;
			u8 tmp = ptr[0];
			ptr[0] = ptr[1];
			ptr[1] = tmp;

			tmp = ptr[2];
			ptr[2] = ptr[3];
			ptr[3] = tmp;
		}
	}
	else
	{
		MessageBox( L"未知类型,输入数字过大" );
		return;
	}
	//------------------------------------
	CString str;
	str.Format( 格式 , val );
	int pos = m_CList.InsertItem( m_CList.GetItemCount() , str );
	if( pos >= 0 )
		刷新差值( pos );
}

void CchazhiSearch::OnCbnSelendokCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
//	UpdateData();
//	m_input = L"";
//	UpdateData( FALSE );
}
#include<2/wqsg_afx.h>
void CchazhiSearch::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	if( m_输入类型 == L"十六进制" )
		编辑框验证十六进制文本( m_input , (CEdit*)GetDlgItem( IDC_EDIT2 ) , this , FALSE );
	else
		编辑框验证十进制文本( m_input , (CEdit*)GetDlgItem( IDC_EDIT2 ) , this );
}

void CchazhiSearch::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( m_文件路径.GetLength() <= 0 )
	{
		MessageBox( L"先选择一个文件" );
		return;
	}
	m_插值表.DelAll();
	const int count = m_CList.GetItemCount();
	if( count <= 1 )
	{
		MessageBox( L"条件不足" );
		return;
	}
	CString str;
	for( int i = 1 ; i < m_CList.GetItemCount() ; ++i )
	{
		str = m_CList.GetItemText( i , 1 );
		LONGLONG val1;
		if( 1 != swscanf( str.GetString() , L"%lld" , &val1 ) )
		{
			MessageBox( L"未知错误1" );
			return;
		}
		//----------------------------------
		if( m_插值表.AddItem( val1 ) < 0 )
		{
			MessageBox( L"未知错误2" );
			return;
		}
	}
	CString 标题;
	GetWindowTextW( 标题 );
	m_log = L"";
	if( m_字节类型 == L"单字节"	)
	{
		C相对搜索<u8> xxx( m_hWnd , 标题 );
		xxx.开始搜索1(m_log , m_文件路径 , m_插值表 );
	}
	else if( m_字节类型 == L"双字节" )
	{
		C相对搜索<u16> xxx( m_hWnd , 标题 );
		xxx.开始搜索1(m_log , m_文件路径 , m_插值表 );
	}
	else if( m_字节类型 == L"四字节" )
	{
		C相对搜索<u32> xxx( m_hWnd , 标题 );
		xxx.开始搜索1(m_log , m_文件路径 , m_插值表 );
	}
	else
	{
		MessageBox( L"未知搜索模式?" );
	}
	UpdateData( FALSE );
	m_插值表.DelAll();
}
void CchazhiSearch::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_CList.DeleteAllItems();

	m_log = L"";
	UpdateData( FALSE );
}

void CchazhiSearch::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton3();
}

void CchazhiSearch::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	static CWQSGFileDialog_Open dlg;

	if( dlg.DoModal() != IDOK )
		return;

	m_文件路径 = dlg.GetPathName();
	UpdateData( FALSE );
}

void CchazhiSearch::刷新差值( INT  startPos )
{
	m_CList.SetRedraw( FALSE );
	m_CList.SetItemText( 0 , 1 , L"0" );

	if( startPos < 1 )
		startPos = 1;

	for( int i = startPos , count = m_CList.GetItemCount();
		i < count ; ++i )
	{
		CString str1( m_CList.GetItemText( i -1 , 0 ) );
		CString str2( m_CList.GetItemText( i , 0 ) );
		LONGLONG val1 , val2 ;
		swscanf( str1.GetString() , L"%X" , &val1 );
		swscanf( str2.GetString() , L"%X" , &val2 );

		val2 -= val1;

		str1.Format( L"%lld" , val2 );

		m_CList.SetItemText( i , 1 , str1 );
	}

	m_CList.SetRedraw( TRUE );
}

void CchazhiSearch::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CList.SetRedraw( FALSE );

	for( int i = 0 , count = m_CList.GetItemCount();
		i < count ; ++i )
	{
		CString str1( m_CList.GetItemText( i , 0 ) );
		UINT val1 ;
		swscanf( str1.GetString() , L"%X" , &val1 );

		if( m_字节类型 == L"单字节" )
		{
			m_CList.SetRedraw( TRUE );
			return;
		}
		else if( m_字节类型 == L"双字节" )
		{
			u8* ptr = (u8*)&val1;
			u8 tmp = ptr[0];
			ptr[0] = ptr[1];
			ptr[1] = tmp;
			str1.Format( L"%04X" , val1 );
		}
		else if( m_字节类型 == L"四字节" )
		{
			u8* ptr = (u8*)&val1;
			u8 tmp = ptr[0];
			ptr[0] = ptr[3];
			ptr[3] = tmp;
			tmp = ptr[1];
			ptr[1] = ptr[2];
			ptr[2] = tmp;
			str1.Format( L"%08X" , val1 );
		}
		else
		{
			m_CList.SetRedraw( TRUE );
			return;
		}
		m_CList.SetItemText( i , 0 , str1 );
	}

	刷新差值( 1 );

	m_CList.SetRedraw( TRUE );
}
