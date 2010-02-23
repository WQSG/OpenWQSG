/*  OpenWQSG - WQSG_NDS
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

// WQSG_NDSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG_NDS.h"
#include "WQSG_NDSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FSI_SUB_FILENAME	0
#define FSI_SUB_FILEID		1
#define FSI_SUB_FILEOFFSET	2
#define FSI_SUB_FILESIZE	3
#define FSI_SUB_FILEHEADER	4
#define FSI_SUB_FILEINFO	5
// CWQSG_NDSDlg 对话框


CWQSG_NDSDlg::CWQSG_NDSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWQSG_NDSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWQSG_NDSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FSILIST, m_NdsFSI);
}

BEGIN_MESSAGE_MAP(CWQSG_NDSDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_RomOpen, &CWQSG_NDSDlg::OnRomopen)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_FSILIST, &CWQSG_NDSDlg::OnLvnItemActivateFsilist)
	ON_NOTIFY(LVN_DELETEITEM, IDC_FSILIST, &CWQSG_NDSDlg::OnLvnDeleteitemFsilist)
	ON_COMMAND(ID_RomClose, &CWQSG_NDSDlg::OnRomclose)
	ON_BN_CLICKED(IDC_BUTTON_UPDIR, &CWQSG_NDSDlg::OnBnClickedButtonUpdir)
	ON_COMMAND(ID_About, &CWQSG_NDSDlg::OnAbout)
	ON_COMMAND(ID_Quit, &CWQSG_NDSDlg::OnQuit)
END_MESSAGE_MAP()// CWQSG_NDSDlg 消息处理程序
BOOL CWQSG_NDSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_NdsFSI.ModifyStyle( LVS_SORTASCENDING | LVS_SORTDESCENDING |
		LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_EDITLABELS | LVS_NOSORTHEADER ,
		LVS_REPORT | LVS_SHOWSELALWAYS/*|LVS_OWNERDRAWFIXED*/);

	m_NdsFSI.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP );

	CString header, text;
	header.LoadString(IDS_NDSFSIHEADER);

	const BYTE nFormat[] = {
		LVCFMT_LEFT , LVCFMT_RIGHT , LVCFMT_LEFT ,
		LVCFMT_RIGHT , LVCFMT_LEFT , LVCFMT_LEFT };

	const BYTE nWidth[] = {
		144 , 60 , 70 ,
		92 , 64 , 64 };

	for( int i=0 ; i < (sizeof(nWidth)/sizeof(*nWidth)) ; i++ )
	{
		AfxExtractSubString( text , header , i );
		m_NdsFSI.InsertColumn( i , text , nFormat[i] , nWidth[i] );
	}

	//CView *pView = m_pParentWnd;
	//m_pParentWnd = pView->GetParent();

#if 0
	m_hImageList = ImageList_Create(16, 16, ILC_COLOR8, 0, 0);
	DWORD dwSize;
	HANDLE hHandle = LoadRes(IDR_NDSFSI, dwSize, _T("CRYSTAL"));
	LPPICTURE pPix= LoadPic(hHandle, dwSize, TRUE);
	HBITMAP hBitMap;
	pPix->get_Handle((OLE_HANDLE*)&hBitMap);
	ImageList_AddMasked(m_hImageList, hBitMap, 0);
	pPix->Release();
#endif
	/*
	m_hImageList = ImageList_LoadBitmap(AfxGetInstanceHandle(),
	MAKEINTRESOURCE(IDB_NDSFSI),
	16, LR_LOADTRANSPARENT, 0xc0c0c0);*/
	//m_NdsFSI.SetImageList(&il, LVSIL_SMALL);
	m_NdsFSI.SendMessage( LVM_SETIMAGELIST , LVSIL_SMALL , (LPARAM)m_hImageList );

	//SetIcon(ImageList_GetIcon(m_hImageList, FSI_ICON_NDS, 0), FALSE);

	OnLoadFSI();

	OnRomclose();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
void CWQSG_NDSDlg::OnPaint()
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
HCURSOR CWQSG_NDSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWQSG_NDSDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

void CWQSG_NDSDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnOK();
}

void CWQSG_NDSDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CWQSG_NDSDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_NdsFSI.GetSafeHwnd())
	{
		CRect rc; m_NdsFSI.GetWindowRect(&rc);
		ScreenToClient(rc);
		m_NdsFSI.SetWindowPos(NULL,
			0, 0,
			cx-rc.left,
			cy-rc.top,
			SWP_NOMOVE);
	}

	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CWQSG_NDSDlg::OnLoadFSI(void)
{
	m_NdsFSI.SetRedraw(FALSE);

	m_NdsFSI.DeleteAllItems();

	if( m_Rom.IsOpen() )
	{
		SNdsFindHandle* pHandle = m_Rom.FindFile( m_strPath );

		if( pHandle )
		{
			SNdsFindData data;

			while(true)
			{
				if( !m_Rom.FindNextFile( data , pHandle /*, true*/ ) )
				{
					if( IDYES != MessageBox( m_Rom.m_strError + L"\r\n\r\n要继续吗?" , NULL , MB_YESNO ) )
					{
						m_NdsFSI.DeleteAllItems();
						break;
					}
					ASSERT(0);
				}

				if( data.m_bEmpty )
					break;

				CString str;
				str = data.m_szName;

				const int iIndex = m_NdsFSI.InsertItem( m_NdsFSI.GetItemCount() , str );
				if( iIndex == -1 )
				{
					ASSERT(iIndex != -1);
					continue;
				}

				const SNdsFindData*const pData = new SNdsFindData( data );
				m_NdsFSI.SetItemData( iIndex , (DWORD_PTR)pData );

				if( pData->m_bDir )
				{
					const SNdsFindData::SDir& dir = pData->m_Dir;
					str.Format( L"Dir(%d)" , dir.m_uDir_ID );
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEID , str );
					//m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEOFFSET , str );
					//m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILESIZE , str );
					//m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEHEADER , str );
					//m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEINFO , str );
				}
				else
				{
					const SNdsFindData::SFile& file = pData->m_File;
					str.Format( L"%d" , file.m_uFileID_InRom );
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEID , str );

					str.Format( L"%08X" , file.m_uRomOffset );
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEOFFSET , str );

					str.Format( L"%d" , file.m_uSize );
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILESIZE , str );

					//str.Format( L"%d" , );
					str = "";
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEHEADER , str );

					//str.Format( L"%d" , );
					str = "";
					m_NdsFSI.SetItemText( iIndex , FSI_SUB_FILEINFO , str );
				}
			}

			m_Rom.FindClose( pHandle );
		}
	}

	m_NdsFSI.SetRedraw();
}

void CWQSG_NDSDlg::OnRomopen()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData();

	static CWQSGFileDialog_Open dlg( L"*.nds|*.nds||" );

	if( dlg.DoModal() != IDOK )
		return;

	//m_Rom.Close();
	OnRomclose();

	m_strPath = "";

	if( !m_Rom.Open( dlg.GetPathName() ) )
	{
		MessageBox( L"打开文件失败\r\n\r\n" + m_Rom.m_strError );
	}
	else
	{
		BOOL bCanW = m_Rom.IsCanWrite();
		SetTitle( &bCanW );
	}

	m_NdsFSI.EnableWindow( TRUE );
	OnLoadFSI();

	return;
}

void CWQSG_NDSDlg::OnLvnItemActivateFsilist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	const SNdsFindData* pData = (SNdsFindData*)m_NdsFSI.GetItemData( pNMIA->iItem );
	if( !pData || !pData->m_bDir )
		return;

	m_strPath.AppendFormat( "/%s" , pData->m_szName );

	OnLoadFSI();
}

void CWQSG_NDSDlg::OnLvnDeleteitemFsilist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	const SNdsFindData* pData = (SNdsFindData*)m_NdsFSI.GetItemData( pNMLV->iItem );
	if( pData )
		delete pData;

}

BOOL CWQSG_NDSDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnRomclose();

	return CDialog::DestroyWindow();
}

void CWQSG_NDSDlg::OnRomclose()
{
	// TODO: 在此添加命令处理程序代码

	m_Rom.Close();

	m_strPath = "";
	SetTitle( NULL );

	m_NdsFSI.EnableWindow( FALSE );

	OnLoadFSI();
}

void CWQSG_NDSDlg::OnBnClickedButtonUpdir()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_strPath == "" )
		return;

	int iPos = m_strPath.ReverseFind( '/' );
	if( -1 == iPos )
		m_strPath = "";
	else
		m_strPath = m_strPath.Left( iPos );

	OnLoadFSI();
}


BOOL CWQSG_NDSDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( WM_DROPFILES == pMsg->message )
	{
		const HDROP hDrop = (HDROP)pMsg->wParam;
		if( pMsg->hwnd == m_NdsFSI.m_hWnd )
		{
			if( m_Rom.IsCanWrite() )
			{
				WCHAR strPathName[MAX_PATH*2];

				const int fileCount = DragQueryFile( hDrop , (UINT)-1 , NULL , 0 );

				for( int i = 0 ; (i>=0) && (i<fileCount) && ( DragQueryFile( hDrop , i , strPathName , MAX_PATH*2 ) != (UINT)-1 ) ; ++i )
				{
					if( WQSG_IsDir( strPathName ) )
					{
						WDir( strPathName , m_strPath );
					}
					else if( WQSG_IsFile( strPathName ) )
					{
						SNdsFindData dirData;
						if( m_Rom.GetPath( dirData , m_strPath ) )
						{
							if( !WFile( strPathName , dirData , "" ) )
							{
								//MessageBox( m_Rom.GetErrStr() );
								//break;
							}
						}
					}
				}
				OnLoadFSI();
				//UpDataLbaData();
			}
		}
		DragFinish( hDrop );
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CWQSG_NDSDlg::WFile( CString a_strPathFile , const SNdsFindData& a_DirData , CStringA a_strFile )
{
	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathFile.GetString() , 1 , 4 ) )
		return false;

	if( a_strFile.GetLength() == 0 )
	{
		const int pos = a_strPathFile.ReverseFind( L'\\' );
		if( pos == -1 )
		{
			ASSERT(pos != -1);
			return false;
		}

		a_strFile = a_strPathFile.Mid( pos + 1 );
	}

	return m_Rom.ImportFile( a_DirData , fp , a_strFile );
}

bool CWQSG_NDSDlg::WDir( CString a_strPathFile , const CStringA& a_strDirPath )
{
	SNdsFindData dirData;
	if( m_Rom.GetPath( dirData , a_strDirPath ) )
		return false;

	if( a_strPathFile.Right(1) != L'\\' )
		a_strPathFile += L'\\';

	WIN32_FIND_DATAW data;

	const HANDLE handle = FindFirstFile( a_strPathFile + L'*' , &data );
	if( handle == INVALID_HANDLE_VALUE )
		return true;

	do 
	{
		if( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CStringA strNameA;
			strNameA = data.cFileName;

			WDir( a_strPathFile + data.cFileName , a_strDirPath + "/" + strNameA );
		}
		else
		{
			CStringA strNameA;
			strNameA = data.cFileName;

			if( !WFile( a_strPathFile + data.cFileName , dirData , strNameA ) )
			{
				if( IDYES != MessageBox( m_Rom.m_strError + L"\r\n\r\n要继续吗?" , NULL , MB_YESNO ) )
				{
					break;
				}
			}
		}
	}
	while ( FindNextFile( handle , &data ));

	FindClose(handle);
	return true;
}

void CWQSG_NDSDlg::SetTitle(BOOL* a_bCanWrite)
{
	CString str1,str2,str3;
	str1.LoadString( IDS_APP_NAME );
	str2.LoadString( IDS_APP_VER );

	CString strTitle( str1 + L" v" + str2 ); 
	if( a_bCanWrite )
	{
		if( *a_bCanWrite )
		{
			strTitle += L" [写模式]";
		}
		else	
		{
			strTitle += L" [只读模式]";
		}
	}

	SetWindowText( strTitle );
}

void CWQSG_NDSDlg::OnAbout()
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

void CWQSG_NDSDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	EndDialog(IDCANCEL);
}
