/*  OpenWQSG - DirSize
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

// DirSizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DirSize.h"
#include "DirSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirSizeDlg 对话框




CDirSizeDlg::CDirSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirSizeDlg::IDD, pParent)
	, m_strFullPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Text(pDX, IDC_EDIT1, m_strFullPath);
}

BEGIN_MESSAGE_MAP(CDirSizeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DIR_UP, &CDirSizeDlg::OnBnClickedButtonDirUp)
	ON_COMMAND(ID_FILE_OPEN_DIR, &CDirSizeDlg::OnFileOpenDir)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1+44, &CDirSizeDlg::OnLvnItemActivateList1)
	ON_COMMAND(ID_ABOUT_ABOUT, &CDirSizeDlg::OnAboutAbout)
END_MESSAGE_MAP()

#define DEF_NAME 0
#define DEF_100 1
#define DEF_SIZE1 2
#define DEF_SIZE2 3

// CDirSizeDlg 消息处理程序

BOOL CDirSizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CRect rect;
	GetDlgItem( IDC_LIST1 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_LIST1 )->GetWindowRect( &rect );
	ScreenToClient( &rect );

	m_WList.Init( m_hWnd , rect , IDC_LIST1 + 44 );
	// TODO: 在此添加额外的初始化代码

	{
		HIMAGELIST hi = NULL;
		SHFILEINFO shfi;

		memset(&shfi, 0 ,sizeof(shfi));
		hi = (HIMAGELIST)SHGetFileInfoW( L"TESTICON1" , FILE_ATTRIBUTE_DIRECTORY , &shfi , sizeof(shfi) ,
			SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES );

		if( !hi )
			ASSERT( FALSE );

		IMAGEINFO info = {0};
		if( !CImageList::FromHandle(hi)->GetImageInfo( 0 , &info ) ||
			!m_imageList.Create( info.rcImage.right , info.rcImage.bottom , ILC_COLORDDB , 1 , 1 ) )
			ASSERT( FALSE );

		m_imageList.SetBkColor( CLR_NONE/*m_cList.GetBkColor()*/ );

		m_imageList.Add( shfi.hIcon );

		memset( &shfi , 0 , sizeof(shfi) );
		if( !SHGetFileInfoW( L"TESTICON2" , 0 , &shfi , sizeof(shfi) ,
			SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES ) )
			ASSERT( FALSE );

		m_imageList.Add( shfi.hIcon );
	}

	m_WList.SetImageList( &m_imageList , LVSIL_SMALL );
	m_WList.InsertColumn( 0 , L"Name" , m_WList.E_CT_Text , 250 );
	m_WList.InsertColumn( 1 , L"100%" , m_WList.E_CT_Progress , 70 );
	m_WList.InsertColumn( 2 , L"Size1" , m_WList.E_CT_Text , 100 , m_WList.E_TA_RIGHT );
	m_WList.InsertColumn( 3 , L"Size2" , m_WList.E_CT_Text , 100 , m_WList.E_TA_RIGHT );

#if _DEBUG
	Open( L"D:\\VM" );
#endif
	m_WList.Invalidate( FALSE );
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

void CDirSizeDlg::OnPaint()
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
HCURSOR CDirSizeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDirSizeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
	EndDialog(IDCANCEL);
}

void CDirSizeDlg::OnOK()
{
}

void CDirSizeDlg::OnCancel()
{
}

#include<algorithm>
#include<functional>

BOOL CDirSizeDlg::Find( CString a_strPath , const size_t a_DirIndex )
{
	ASSERT( a_strPath.Right(1) == L'\\' );

	LONGLONG nSizeAll = 0;

	WIN32_FIND_DATA data;

	HANDLE handle = FindFirstFile( a_strPath + L"*" , &data );
	if( handle == INVALID_HANDLE_VALUE )
		return TRUE;

	BOOL bRt = TRUE;
	do 
	{
		if( FILE_ATTRIBUTE_REPARSE_POINT & data.dwFileAttributes )
		{
			//ASSERT(0);
			continue;
		}

		SItemInfo info;
		lstrcpy( info.m_szName , data.cFileName );
		info.m_bDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		CString strFile( data.cFileName );
		if( info.m_bDir )
		{
			if( strFile == L"." || strFile == L".." )
				continue;

			info.m_Index = m_Dirs.size();

			m_Dirs.push_back( SDirNode() );

			Find( a_strPath + strFile + L'\\' , info.m_Index  );

			info.m_nSize = m_Dirs[info.m_Index].m_AllSize;
			nSizeAll += info.m_nSize;

			m_Dirs[a_DirIndex].m_Infos.push_back(info);
		}
		else
		{
			info.m_nSize = (LONGLONG(data.nFileSizeHigh) << 32) | data.nFileSizeLow;
			nSizeAll += info.m_nSize;

			m_Dirs[a_DirIndex].m_Infos.push_back(info);
		}
	}
	while ( FindNextFile( handle, &data ) );

	FindClose(handle);

	sort( m_Dirs[a_DirIndex].m_Infos.begin() , m_Dirs[a_DirIndex].m_Infos.end() , std::greater<SItemInfo>() );
	m_Dirs[a_DirIndex].m_AllSize = nSizeAll;

	return bRt;
}

BOOL CDirSizeDlg::Open( CString a_strPath )
{
	Clear();

	if( a_strPath.Right(1) != L'\\' )
		a_strPath += L'\\';

	m_Dirs.resize( 1 );

	if( Find( a_strPath , 0 ) )
	{
		m_strBasePath = a_strPath;

		UpdateUI();
		return TRUE;
	}

	Clear();
	return FALSE;
}

void CDirSizeDlg::Clear()
{
	UpdateData();
	m_Dirs.clear();
	m_Path.clear();
//	m_cList.DeleteAllItems();
	m_WList.DeleteAllItems();

	m_strFullPath = m_strBasePath = L"";
	UpdateData( FALSE );
}

void CDirSizeDlg::UpdateUI()
{
	m_WList.SetRedraw(FALSE);
	UpdateData();

//	m_cList.DeleteAllItems();
	m_WList.DeleteAllItems();

	m_strFullPath = m_strBasePath;

	if( !m_Dirs.empty() )
	{
		size_t dirIndex = 0;

		for( std::vector<SPathNode>::iterator it = m_Path.begin() ;
			it != m_Path.end() ; ++it )
		{
			const SPathNode& path = *it;
			m_strFullPath += (path.m_strName + L'\\');

			dirIndex = path.m_Index;
		}

		const SDirNode& dirNode = m_Dirs[dirIndex];

		for( TItemInfos::const_iterator it = dirNode.m_Infos.begin() ;
			it != dirNode.m_Infos.end() ; ++it )
		{
			const SItemInfo& item = *it;

			//const int iIndex = m_cList.InsertItem( m_cList.GetItemCount() , item.m_szName , (item.m_bDir?0:1) );
			const int iIndex = m_WList.InsertItem( m_WList.GetItemCount() , (item.m_bDir?0:1) );

			ASSERT( iIndex != -1 );

			m_WList.SetItemText( iIndex , DEF_NAME , item.m_szName );
			
			LONGLONG nSize;

			if( item.m_bDir )
			{
				ASSERT( item.m_Index > 0 && item.m_Index < m_Dirs.size() );
				const SDirNode& dirNodeSub = m_Dirs[item.m_Index];

				nSize = dirNodeSub.m_AllSize;

				ASSERT( nSize == item.m_nSize );
			}
			else
			{
				nSize = item.m_nSize;
			}

			CString str;
			//str.Format( L"%.2f%%" , float(nSize) / dirNode.m_AllSize * 100.0f );
			//m_cList.SetItemText( iIndex , DEF_100 , str );
			m_WList.SetItem100( iIndex , DEF_100 , float(nSize) / dirNode.m_AllSize );

			{
				LONGLONG nS = nSize;
				str = L"";

				if( nS >= (1024*1024*1024) )
				{
					const LONGLONG nH = nS / (1024*1024*1024);
					nS %= (1024*1024*1024);
					if( nS > 0 )
					{
						str.AppendFormat( L"%lld.%ld GB " , nH , long(nS / float(1024*1024*1024) * 100) );
					}
					else
					{
						str.AppendFormat( L"%lld GB " , nH );
					}
				}
				else if( nS >= (1024*1024) )
				{
					const LONGLONG nH = nS / (1024*1024);
					nS %= (1024*1024);
					if( nS > 0 )
					{
						str.AppendFormat( L"%lld.%ld MB " , nH , long(nS / float(1024*1024) * 100) );
					}
					else
					{
						str.AppendFormat( L"%lld MB " , nH );
					}
				}
				else if( nS >= (1024) )
				{
					const LONGLONG nH = nS / (1024);
					nS %= (1024);
					if( nS > 0 )
					{
						str.AppendFormat( L"%lld.%ld KB " , nH , long(nS / float(1024) * 100) );
					}
					else
					{
						str.AppendFormat( L"%lld KB " , nH );
					}
				}
				else if( nS >= 0 )
				{
					const LONGLONG nH = nS;
					nS = 0;
					if( nS > 0 )
					{
						str.AppendFormat( L"%lld.%ld B " , nH , long(nS ) );
					}
					else
					{
						str.AppendFormat( L"%lld B " , nH );
					}
				}
			}
			//m_cList.SetItemText( iIndex , DEF_SIZE1 , str );
			m_WList.SetItemText( iIndex , DEF_SIZE1 , str );

			str.Format( L"%lld" , nSize );
			//m_cList.SetItemText( iIndex , DEF_SIZE2 , str );
			m_WList.SetItemText( iIndex , DEF_SIZE2 , str );

			ASSERT( sizeof(DWORD_PTR) == sizeof(&item) );
			//m_cList.SetItemData( iIndex , (DWORD_PTR)&item );
			m_WList.SetData( iIndex , (LPVOID)&item );
		}
	}

	SetWindowText( m_strFullPath );

	UpdateData( FALSE );
	m_WList.SetRedraw(TRUE);
}

void CDirSizeDlg::OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	ASSERT( sizeof(m_WList.GetData( pNMIA->iItem )) == sizeof(const SItemInfo*) );

	const SItemInfo* pInfo = (const SItemInfo*)m_WList.GetData( pNMIA->iItem );
	ASSERT(pInfo);
	if( !pInfo->m_bDir )
		return;

	ASSERT( pInfo->m_Index > 0 && pInfo->m_Index < m_Dirs.size() );

	SPathNode path;
	path.m_Index = pInfo->m_Index;
	path.m_strName = pInfo->m_szName;

	m_Path.push_back( path );
	UpdateUI();
	m_WList.Invalidate( FALSE );
}

void CDirSizeDlg::OnBnClickedButtonDirUp()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !m_Path.empty() )
	{
		m_Path.pop_back();
		UpdateUI();
		m_WList.Invalidate( FALSE );
	}
}

void CDirSizeDlg::OnFileOpenDir()
{
	// TODO: 在此添加命令处理程序代码
	CWQSG_DirDlg dlg( m_hWnd , L"选择目录..." , m_LastSelDir.GetString() );
	WCHAR outPath[MAX_PATH*2];
	if( !dlg.GetPath( outPath ) )
		return ;

	Open( outPath );
	m_WList.Invalidate( FALSE );
}

void CDirSizeDlg::OnAboutAbout()
{
	// TODO: 在此添加命令处理程序代码

	CString strAppName;
	strAppName.LoadString( IDS_APP_NAME );

	CString strAppVer;
	strAppVer.LoadString( IDS_APP_VER );

	CString strAuthor1;
	strAuthor1.LoadString( IDS_APP_AUTHOR );

	CString strAuthor2;
	strAuthor2.LoadString( IDS_APP_AUTHOR2 );

	WQSG_About( m_hIcon , m_hWnd , L"关于本软件" , strAppName + L"\r\nv" + strAppVer ,
		L"项目svn : <A HREF=\"http://code.google.com/p/openwqsg\">http://code.google.com/p/openwqsg</A>\r\n依赖库svn : <A HREF=\"http://code.google.com/p/wqsglib\">http://code.google.com/p/wqsglib</A>\r\n                 <A HREF=\"http://wqsg.ys168.com\">http://wqsg.ys168.com</A>\r\n" ,
		strAuthor2 + L"(" + strAuthor1 + L")" );
}

