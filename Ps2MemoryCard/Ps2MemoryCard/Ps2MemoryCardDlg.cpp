/*  OpenWQSG - Ps2MemoryCard
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

// Ps2MemoryCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ps2MemoryCard.h"
#include "Ps2MemoryCardDlg.h"
#include "VPscMc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPs2MemoryCardDlg 对话框




CPs2MemoryCardDlg::CPs2MemoryCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPs2MemoryCardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPs2MemoryCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
}

BEGIN_MESSAGE_MAP(CPs2MemoryCardDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_MC, &CPs2MemoryCardDlg::OnBnClickedButtonOpenMc)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MC, &CPs2MemoryCardDlg::OnBnClickedButtonSaveMc)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_PSU, &CPs2MemoryCardDlg::OnBnClickedButtonImportPsu)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_PSU, &CPs2MemoryCardDlg::OnBnClickedButtonExportPsu)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CPs2MemoryCardDlg::OnBnClickedButtonAbout)
END_MESSAGE_MAP()


// CPs2MemoryCardDlg 消息处理程序

BOOL CPs2MemoryCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cList.SetExtendedStyle( m_cList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_cList.InsertColumn( 0 , L"存档名" , 0 , 350 );
	m_cList.InsertColumn( 1 , L"size" , 0 , 40 );
	m_cList.InsertColumn( 2 , L"原名" , 0 , 150 );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPs2MemoryCardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CPs2MemoryCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPs2MemoryCardDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

void CPs2MemoryCardDlg::OnBnClickedButtonOpenMc()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open dlg( L"*.ps2|*.ps2||" );
	if( IDOK != dlg.DoModal() )
		return;

	if( !m_Mc.LoadMc( dlg.GetPathName() ) )
	{
		UpdateUI();
		MessageBox( L"加载记忆卡失败" );
		EndDialog( IDCANCEL );
	}

	UpdateUI();
}

void CPs2MemoryCardDlg::OnBnClickedButtonSaveMc()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !m_Mc.isOpen() )
		return ;

	static CWQSGFileDialog_Save dlg( L"*.ps2|*.ps2||" , L"ps2" );
	if( IDOK != dlg.DoModal() )
		return;

	if( !m_Mc.SaveMc( dlg.GetPathName() ) )
	{
		MessageBox( L"保存记忆卡失败" );
	}
}

void CPs2MemoryCardDlg::OnBnClickedButtonImportPsu()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !m_Mc.isOpen() )
		return ;

	static CWQSGFileDialog_Open dlg( L"*.psu|*.psu||" );
	if( IDOK != dlg.DoModal() )
		return;

	m_Mc.Bak();
	if( m_Mc.Import_Psu( dlg.GetPathName() ) )
	{
		UpdateUI();
		MessageBox( L"导入成功" );
	}
	else
	{
		m_Mc.UnBak();
		UpdateUI();
		MessageBox( L"导入PSU失败" );
	}
}

void CPs2MemoryCardDlg::OnBnClickedButtonExportPsu()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !m_Mc.isOpen() )
		return ;

	POSITION pos = m_cList.GetFirstSelectedItemPosition();
	const int iIndex = m_cList.GetNextSelectedItem( pos );
	if( iIndex == -1 )
		return ;

	const CString str = m_cList.GetItemText( iIndex , 2 );

	CWQSGFileDialog_Save dlg( L"*.psu|*.psu||" , L"psu" , str );
	if( IDOK != dlg.DoModal() )
		return;

	char* pName = WQSG_W_char( str.GetString() , 932 );
	if( m_Mc.Export_Psu( dlg.GetPathName() , pName ) )
	{
		delete[]pName;
		MessageBox( L"导出成功" );
	}
	else
	{
		delete[]pName;
		MessageBox( L"导出PSU失败" );
	}
}

void CPs2MemoryCardDlg::UpdateUI()
{
	m_cList.DeleteAllItems();
	if( !m_Mc.isOpen() )
		return ;

	std::vector<CVPscMc::SFileInfo> files;
	if( !m_Mc.GetFiles( files , "" ) )
	{
		MessageBox( L"获取文件列表失败" );
		return;
	}

	m_cList.SetRedraw( FALSE );
	for( std::vector<CVPscMc::SFileInfo>::iterator it = files.begin() ;
		it != files.end() ; ++it )
	{
		const CVPscMc::SFileInfo& info = *it;

		CWQSG_memFile mf;
		if( !m_Mc.Vmc_ReadFile( mf , info.szName , "icon.sys" ) )
			continue;

		mf.Seek( 0xC0 );

		char buf[0x100];
		if( sizeof(buf) != mf.Read( buf , sizeof(buf) ) )
		{
			MessageBox( L"取存档文件名失败" );
			break;
		}

		WCHAR* pTitle = WQSG_char_W( buf , 932 );
		const int iIndex = m_cList.InsertItem( m_cList.GetItemCount() , pTitle );
		delete[]pTitle;

		if( iIndex == -1 )
		{
			MessageBox( L"添加失败" );
			break;
		}

		CString str;

		str.Format( L"%d" , info.uSize );
		m_cList.SetItemText( iIndex , 1 , str );

		WCHAR* pX = WQSG_char_W( info.szName , 932 );
		m_cList.SetItemText( iIndex , 2 , pX );
		delete[]pX;
	}

	m_cList.SetRedraw( TRUE );
}

void CPs2MemoryCardDlg::OnBnClickedButtonAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strAppName;
	strAppName.LoadString( IDS_APP_NAME );

	CString strAppVer;
	strAppVer.LoadString( IDS_APP_VER );

	CString strAuthor1;
	strAuthor1.LoadString( IDS_APP_AUTHOR );

	CString strAuthor2;
	strAuthor2.LoadString( IDS_APP_AUTHOR2 );

	WQSG_About( m_hIcon , m_hWnd , L"关于本软件" , strAppName + L"\r\nv" + strAppVer ,
		L"项目svn : <A HREF=\"http://code.google.com/p/wqsg-umd\">http://code.google.com/p/OpenWqsg</A>\r\n"
		L"依赖库svn : <A HREF=\"http://code.google.com/p/wqsglib\">http://code.google.com/p/wqsglib</A>\r\n                 <A HREF=\"http://wqsg.ys168.com\">http://wqsg.ys168.com</A>\r\n" ,
		strAuthor2 + L"(" + strAuthor1 + L")" );
}
