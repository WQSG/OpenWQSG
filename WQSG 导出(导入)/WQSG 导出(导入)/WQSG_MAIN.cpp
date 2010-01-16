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
#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WQSG_MAIN.h"
// CWQSG_MAIN 对话框
CWnd*				WQSG_MAIN_CWND = NULL;
CWQSG_INI_XML		WQSG_ini;
CString				WQSG_iniSavePathName;
IMPLEMENT_DYNAMIC(CWQSG_MAIN, CDialog)
CWQSG_MAIN::CWQSG_MAIN(CWnd* pParent /*=NULL*/)
	: CDialog(CWQSG_MAIN::IDD, pParent)
#if DEF_ON_TXTIO
	, m_TXTIO( L"导出文本" , L"导入文本")
#endif
#if DEF_ON_PTXTIO
	, m_PTXTIO( L"导出文本" , L"导入文本" )
#endif
#if	DEF_ON_WIPS
	, m_WIPS( L"制作补丁" , L"使用补丁")
#endif
	, m_CurToolID( WT_ID_MAX )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_WQSG);
	WQSG_MAIN_CWND = this;

	for( int i = 0; i < WT_ID_MAX ; ++i )
		m_Tool_CWND[i] = NULL;
}

CWQSG_MAIN::~CWQSG_MAIN()
{
}

void CWQSG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TAB);
	DDX_Control(pDX, IDC_EDIT_LOG, m_CEDIT_LOG);
}


BEGIN_MESSAGE_MAP(CWQSG_MAIN, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWQSG_MAIN::OnTcnSelchangeTab1)
	ON_MESSAGE( WM_WQSG_设置文本 , &CWQSG_MAIN::设置文本 )
	ON_MESSAGE( WM_WQSG_设置LOG文本 , &CWQSG_MAIN::设置LOG文本 )
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()
void CWQSG_MAIN::OnOK(){}
void CWQSG_MAIN::OnCancel(){}//	
BOOL CWQSG_MAIN::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	{
		CString strName;strName.LoadString( IDS_APP_NAME );
		CString strVer;strVer.LoadString( IDS_APP_VER );
		CString strWQSG;strWQSG.LoadString( IDS_APP_WQSG );
		SetWindowTextW( strName + L" " + strVer + L"    " + strWQSG );
	}
	//--------------------------------------
	WCHAR exePathName[ MAX_PATH * 2 ];
	if( 0 == GetModuleFileNameW( NULL , exePathName , MAX_PATH * 2 ) )
		goto __gt出错退出;

	WQSG_iniSavePathName = exePathName;
	WQSG_iniSavePathName = WQSG_iniSavePathName.Left( WQSG_iniSavePathName.ReverseFind( L'\\' ) + 1 ) + L"WQSG汉化配置.XML";
	if( !WQSG_ini.Load( WQSG_iniSavePathName , L"WQSG汉化配置" , FALSE ) )
		goto __gt出错退出;

	{
		CRect rect , rectTAB;
		m_TAB.GetClientRect( rect );

		m_TAB.GetItemRect( 0 , rectTAB );
		rectTAB.top = rectTAB.bottom + 4;
		rectTAB.left = 2;
		rectTAB.right = rect.Width() - 4;
		rectTAB.bottom = rect.Height() - 5;// - rectTAB.top;

		int __nID = 0;
#define DEF_ADD( __DEF_X , __DEF_Y , __DEF_TYPE ) \
		if( !__DEF_X.Create( (UINT)__DEF_X.IDD , &m_TAB ) )\
		goto __gt出错退出;\
		m_TAB.InsertItem( __nID , __DEF_Y );\
		m_Tool_CWND[ __nID++ ] = &__DEF_X;\
		__DEF_X.MoveWindow( rectTAB );

#if DEF_ON_TXTIO
		DEF_ADD( m_TXTIO , L"导文本(普通)" , WT_ID_TXTIO )
#endif
#if DEF_ON_PTXTIO
			DEF_ADD( m_PTXTIO , L"导文本(指针)" , WT_ID_PTXTIO )
#endif
#if DEF_ON_WIPS
			DEF_ADD( m_WIPS , L"WIPS补丁" , WT_ID_WIPS )
#endif
#if DEF_ON_TBL
			DEF_ADD( m_TBLtool , L"码表工具" , WT_ID_TBL )	
#endif
#if DEF_ON_文本替换
		DEF_ADD( m_批量文本替换 , L"文本替换" , WT_ID_文本替换 )
#endif
#if DEF_ON_差值搜索
		DEF_ADD( m_chazhiSearch , L"差值搜索" , WT_ID_差值搜索 )
#endif

		DEF_ADD( m_dlgAbout , L"关于本软件" , WT_ID_ABOUT )

#undef DEF_ADD
	}
	选择工具();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
__gt出错退出:
	CDialog::OnCancel();
	return FALSE;
}
void CWQSG_MAIN::OnClose()
{
	
	if( m_窗口消息.GetLength() > 0 )
	{
		if( IDYES != MessageBox( L"任务还没结束,确定要关闭么?" , NULL , MB_YESNO ) )
		{
			return;
		}
	}
	CDialog::OnClose();
	CDialog::OnCancel();
}

void CWQSG_MAIN::选择工具(void)
{
	int nID = m_TAB.GetCurSel();
	if( m_CurToolID != nID )
	{
		if( (m_CurToolID >= 0) && (m_CurToolID < WT_ID_MAX) )
		{
			if( m_Tool_CWND[ m_CurToolID ] )
			{
				m_Tool_CWND[ m_CurToolID ]->ShowWindow( FALSE );
			}
		}
		m_CurToolID = WT_ID_MAX;
		if( (nID >= 0) && (nID < WT_ID_MAX) )
		{
			m_CurToolID = nID;
			if( m_Tool_CWND[ m_CurToolID ] )
			{
				m_Tool_CWND[ m_CurToolID ]->ShowWindow( TRUE );
			}
		}
	}
}

void CWQSG_MAIN::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	选择工具();
	*pResult = 0;
}

LRESULT CWQSG_MAIN::设置文本( WPARAM 保留 , LPARAM 文本 )
{
	if( 文本 )
	{
		m_窗口消息 = (WCHAR*)文本 ;
		m_TAB.EnableWindow( FALSE );
		m_TAB.ShowWindow( SW_HIDE );
		m_CEDIT_LOG.ShowWindow( SW_NORMAL );
		Invalidate();
	}
	else
	{
		m_窗口消息 = L"";
		m_TAB.EnableWindow( TRUE );
		m_TAB.ShowWindow( SW_NORMAL );
		m_CEDIT_LOG.ShowWindow( SW_HIDE );
		m_CEDIT_LOG.SetWindowTextW( L"" );
		Invalidate();
	}
	return 0;
}
LRESULT CWQSG_MAIN::设置LOG文本( WPARAM 保留 , LPARAM 文本 )
{
	m_CEDIT_LOG.SetWindowTextW( (WCHAR*)文本 );
	return 0;
}
BOOL CWQSG_MAIN::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::CBrush hbr( RGB(255,255,255) );
	CRect rect;
	GetClientRect( rect );
	pDC->FillRect( rect , &hbr );

	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CWQSG_MAIN::OnPaint()
{
	CPaintDC dc(this); // 不为绘图消息调用 CDialog::OnPaint()

	if( m_窗口消息.GetLength() )
	{
		CRect rect;
		GetClientRect( rect );
		rect.top = rect.bottom ;//>> 2;
//		rect.bottom = rect.top;
		COLORREF old = dc.SetTextColor( RGB(255,0,0) );
		dc.DrawTextW( m_窗口消息 , rect , DT_CENTER );
		dc.SetTextColor( old );
	}
}
