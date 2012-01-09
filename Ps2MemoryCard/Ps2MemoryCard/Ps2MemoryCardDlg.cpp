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

struct SItemSortData
{
	CStringW m_strTitleName;
	n32 m_nSize;
	CStringW m_strFileName;
};

int CALLBACK ItemSort( LPARAM a , LPARAM b , LPARAM c )
{
	SItemSortData* pData1 = (SItemSortData*)((c&0xF0000000)?a:b);
	SItemSortData* pData2 = (SItemSortData*)((c&0xF0000000)?b:a);

	switch ( c & 0x0FFFFFFF )
	{
	case 1:
		return pData1->m_nSize - pData2->m_nSize;
		break;
	case 2:
		return pData1->m_strFileName.Compare( pData2->m_strFileName );
		break;
	}

	return pData1->m_strTitleName.Compare( pData2->m_strTitleName );
}

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
	ON_BN_CLICKED(IDC_BUTTON1, &CPs2MemoryCardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_BIN, &CPs2MemoryCardDlg::OnBnClickedButtonSaveBin)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CPs2MemoryCardDlg::OnHdnItemclickList1)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST1, &CPs2MemoryCardDlg::OnLvnDeleteitemList1)
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

	m_cList.InsertColumn( 0 , L"存档名" , 0 , 450 );
	m_cList.InsertColumn( 1 , L"size" , 0 , 40 );
	m_cList.InsertColumn( 2 , L"原名" , 0 , 150 );

	HDITEM hi = {};
	hi.mask = HDI_FORMAT;
	m_cList.GetHeaderCtrl()->GetItem( 0 , &hi );
	hi.fmt |= HDF_SORTUP;
	m_cList.GetHeaderCtrl()->SetItem( 0 , &hi );

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
	static CWQSGFileDialog_Open dlg( L"*.ps2;*.bin|*.ps2;*.bin||" );
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

	static CWQSGFileDialog_OpenS dlg( L"*.psu|*.psu||" );
	if( IDOK != dlg.DoModal() )
		return;

	CString strName;
	POSITION pos = dlg.GetStartPosition();
	while( dlg.GetNextPathName( strName , pos ) )
	{
		m_Mc.Bak();
		if( !m_Mc.Import_Psu( strName ) )
		{
			m_Mc.UnBak();
			UpdateUI();
			MessageBox( L"导入PSU失败" , strName );
			return;
		}
	}
	UpdateUI();
	MessageBox( L"导入成功" );
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

void CPs2MemoryCardDlg::UISort()
{
	CHeaderCtrl& cc = *m_cList.GetHeaderCtrl();

	HDITEM hi = {};
	hi.mask = HDI_FORMAT;

	u32 uFlag = 0;
	for( int i = 0 ; i < cc.GetItemCount() ; ++i )
	{
		cc.GetItem( i , &hi );

		if( hi.fmt & (HDF_SORTUP|HDF_SORTDOWN) )
		{
			uFlag = i;
			break;
		}
	}

	if( hi.fmt & HDF_SORTDOWN )
		uFlag |= 0xF0000000;

	m_cList.SortItems( &ItemSort , uFlag );
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

		CStringW str;

		CWQSG_memFile mf;
		if( m_Mc.Vmc_ReadFile( mf , info.szName , "icon.sys" ) )
		{
			mf.Seek( 0xC0 );

			char buf[0x100];
			if( sizeof(buf) != mf.Read( buf , sizeof(buf) ) )
			{
				MessageBox( L"取存档文件名失败" );
				break;
			}

			WCHAR* pTitle = WQSG_char_W( buf , 932 );
			str = pTitle;
			delete[]pTitle;
		}
		else
		{
			WCHAR* pX = WQSG_char_W( info.szName , 932 );
			str = pX;
			delete[]pX;
		}

		const int iIndex = m_cList.InsertItem( m_cList.GetItemCount() , str );

		if( iIndex == -1 )
		{
			MessageBox( L"添加失败" );
			break;
		}

		SItemSortData* pData = new SItemSortData;
		m_cList.SetItemData( iIndex , (DWORD_PTR)pData );

		pData->m_strTitleName = str;
		pData->m_nSize = info.uSize;
		

		str.Format( L"%d" , info.uSize );
		m_cList.SetItemText( iIndex , 1 , str );

		WCHAR* pX = WQSG_char_W( info.szName , 932 );
		pData->m_strFileName = pX;
		delete[]pX;

		m_cList.SetItemText( iIndex , 2 , pData->m_strFileName );
	}
	//--------------------------
	UISort();
	//--------------------------
	m_cList.SetRedraw( TRUE );

	u32 count = 0;
	if( m_Mc.GetFreeClusterCount( count ) )
	{
		CStringW str;
		str.Format( L"空闲块 = %d , size = %d\r\n" , count , m_Mc.GetPreClusterSize() * count );
		OutputDebugString( str );
	}
	else
	{
		CStringW str;
		str.Format( L"取空闲块失败,空闲块 = %d , size = %d\r\n" , count , m_Mc.GetPreClusterSize() * count );
		OutputDebugString( str );
	}
}
// 00833A00 map? 00000000 ,总之全FF
// 0083DF00 MAP  0000A500
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
		L"项目svn : <A HREF=\"http://code.google.com/p/openwqsg\">http://code.google.com/p/openwqsg</A>\r\n"
		L"依赖库svn : <A HREF=\"http://code.google.com/p/wqsglib\">http://code.google.com/p/wqsglib</A>\r\n                 <A HREF=\"http://wqsg.ys168.com\">http://wqsg.ys168.com</A>\r\n" ,
		strAuthor2 + L"(" + strAuthor1 + L")" );
}

bool CPs2MemoryCardDlg::Load_Psu( const CStringW& a_strFile , SPsuData& a_Files )
{
	CWQSG_File fp;
	if( !fp.OpenFile( a_strFile.GetString() , 1 , 3 ) )
		return false;

	SPsu_header psu_head = {};
	if( sizeof(psu_head) != fp.Read( &psu_head , sizeof(psu_head) ) )
		return false;

	if( !(psu_head.attr & DF_DIRECTORY) )
		return false;

	if( !(psu_head.attr & DF_EXISTS) )
		return false;

	{
		static const SPsu_header psu_x = {};

		if( psu_head.unknown_1_u16 != 0 || psu_head.unknown_2_u64 != 0 || psu_head.EMS_used_u64 != 0 )
			return false;

		if( memcmp( psu_head.unknown_3_24_bytes , psu_x.unknown_3_24_bytes , sizeof(psu_x.unknown_3_24_bytes) ) != 0 ||
			memcmp( psu_head.unknown_4_416_bytes , psu_x.unknown_4_416_bytes , sizeof(psu_x.unknown_4_416_bytes) ) != 0)
			return false;
	}


	a_Files.m_strName = (const char*)psu_head.name;
	a_Files.m_files.clear();

	for( u32 i = 0 ; i < psu_head.size ; ++i )
	{
		SPsu_header head1 = {};
		if( sizeof(head1) != fp.Read( &head1 , sizeof(head1) ) )
			return false;

		{
			static const SPsu_header psu_x = {};

			if( head1.unknown_1_u16 != 0 || head1.unknown_2_u64 != 0 || head1.EMS_used_u64 != 0 )
				return false;

			if( memcmp( head1.unknown_3_24_bytes , psu_x.unknown_3_24_bytes , sizeof(psu_x.unknown_3_24_bytes) ) != 0 ||
				memcmp( head1.unknown_4_416_bytes , psu_x.unknown_4_416_bytes , sizeof(psu_x.unknown_4_416_bytes) ) != 0)
				return false;
		}

		if( head1.attr & DF_DIRECTORY )
		{
			if( head1.size == 0 )
				continue;

			return false;
		}

		if( !(head1.attr & DF_EXISTS) )
			return false;

		std::vector<u8>& buff = a_Files.m_files[std::string((const char*)head1.name)];

		if( head1.size )
		{
			buff.resize( head1.size , 0 );

			if( head1.size != fp.Read( &buff[0] , head1.size ) )
				return false;

			const u32 p = head1.size % 0x400;

			if( p )
				fp.Seek( fp.Tell() + (0x400 - p) );

			if( 0 )
			{
				CWQSG_File FP;
				if( FP.OpenFile( L"D:\\WQSG\\ICON.BIN" , 4 , 3 ) )
				{
					FP.Write( &buff[0] , head1.size );
					FP.Close();
				}
			}
		}
	}

	return true;
}

void CPs2MemoryCardDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open dlg1( L"*.psu|*.psu||" );
	if( IDOK != dlg1.DoModal() )
		return;

	static CWQSGFileDialog_Open dlg2( L"*.psu|*.psu||" );
	if( IDOK != dlg2.DoModal() )
		return;

	SPsuData psu1;
	SPsuData psu2;

	if( !Load_Psu( dlg1.GetPathName() , psu1 ) )
	{
		MessageBox( L"读取失败" , dlg1.GetPathName() );
		return;
	}

	if( !Load_Psu( dlg2.GetPathName() , psu2 ) )
	{
		MessageBox( L"读取失败" , dlg2.GetPathName() );
		return;
	}

	if( psu1.m_strName != psu2.m_strName )
	{
		MessageBox( L"存档不同" );
		return;
	}

	if( psu1.m_files.size() != psu2.m_files.size() )
	{
		MessageBox( L"文件数不等" );
		return;
	}

	for( TPsuFile::const_iterator it1 = psu1.m_files.begin() ;
		it1 != psu1.m_files.end() ; ++it1 )
	{
		TPsuFile::const_iterator it2 = psu2.m_files.find(it1->first);
		if( it2 == psu2.m_files.end() )
		{
			MessageBox( L"文件名不等" );
			return;
		}

		if( it1->second.size() != it2->second.size() )
		{
			CString str;
			str = it1->first.c_str();
			MessageBox( L"数据长度不等" , str );
			return;
		}

		if( memcmp( &(it1->second[0]) , &(it2->second[0]) , it2->second.size() ) )
		{
			CString str;
			str = it1->first.c_str();
			MessageBox( L"数据不等" , str );
			return;
		}
	}

	MessageBox( L"相等" );
}

void CPs2MemoryCardDlg::OnBnClickedButtonSaveBin()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !m_Mc.isOpen() )
		return ;

	static CWQSGFileDialog_Save dlg( L"*.bin|*.bin||" , L"bin" );
	if( IDOK != dlg.DoModal() )
		return;

	if( !m_Mc.SaveMcNoEcc( dlg.GetPathName() ) )
	{
		MessageBox( L"保存记忆卡失败" );
	}
}

void CPs2MemoryCardDlg::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if( phdr->iButton != 0 )
		return;

	HDITEM hi = {};
	hi.mask = HDI_FORMAT;

	CHeaderCtrl& cc = *m_cList.GetHeaderCtrl();
	cc.GetItem( phdr->iItem , &hi );
	if( hi.fmt & (HDF_SORTUP|HDF_SORTDOWN) )
	{
	}
	else
	{
		for( int i = 0 ; i < cc.GetItemCount() ; ++i )
		{
			cc.GetItem( i , &hi );

			const int oldFmt = hi.fmt & (HDF_SORTUP|HDF_SORTDOWN);
			if( oldFmt )
			{
				hi.fmt &= ~(HDF_SORTUP|HDF_SORTDOWN);
				cc.SetItem( i , &hi );
				break;
			}
		}
		cc.GetItem( phdr->iItem , &hi );
	}

	const int oldFmt = hi.fmt & (HDF_SORTUP|HDF_SORTDOWN);
	hi.fmt &= ~(HDF_SORTUP|HDF_SORTDOWN);
	if( oldFmt & HDF_SORTUP )
		hi.fmt |= HDF_SORTDOWN;
	else
		hi.fmt |= HDF_SORTUP;

	cc.SetItem( phdr->iItem , &hi );

	UISort();
}

void CPs2MemoryCardDlg::OnLvnDeleteitemList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	delete (SItemSortData*)m_cList.GetItemData( pNMLV->iItem );
}
