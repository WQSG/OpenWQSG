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


#include "批量文本替换Dlg.h"
#include "chazhiSearch.h"
#include "WQSG_TXT_IO.h"

#if DEF_ON_TBL
#include "统计字频.h"
#endif

#include "CDlgAbout.h"
// CWQSG_MAIN 对话框
CWnd*			g_pMAIN_CWND = NULL;

IMPLEMENT_DYNAMIC(CWQSG_MAIN, CDialog)
CWQSG_MAIN::CWQSG_MAIN(CWnd* pParent /*=NULL*/)
	: CDialog(CWQSG_MAIN::IDD, pParent)
	, m_SelWnd( NULL )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_WQSG);
	g_pMAIN_CWND = this;
}

CWQSG_MAIN::~CWQSG_MAIN()
{
	for( std::vector<CBaseDialog*>::iterator it = m_TabWnd.begin() ; 
		it != m_TabWnd.end() ; ++it )
	{
		delete *it;
	}
	m_TabWnd.clear();
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
	ON_MESSAGE( WM_WQSG_SetText , &CWQSG_MAIN::SetText )
	ON_MESSAGE( WM_WQSG_SetLogText , &CWQSG_MAIN::SetLogText )
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

template< typename BaseType >
BOOL CWQSG_MAIN::AddDlg( BaseType* a_pDlg , const CString& a_strTabName , const CRect& a_rect )
{
	if( !a_pDlg )
		return FALSE;

	if( !a_pDlg->Create( (UINT)a_pDlg->IDD , &m_TAB ) )
	{
		delete a_pDlg;
		return FALSE;
	}

	const LONG nIndex = m_TAB.GetItemCount();
	if( nIndex != m_TAB.InsertItem( nIndex , a_strTabName ) )
	{
		delete a_pDlg;
		return FALSE;
	}

	m_TabWnd.push_back( a_pDlg );
	a_pDlg->MoveWindow( a_rect );
	a_pDlg->ShowWindow( FALSE );

	return TRUE;
}

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
		goto __gtErrExit;

	if( !InitConfig() )
		goto __gtErrExit;

	{
		CRect rect , rectTAB;
		m_TAB.GetClientRect( rect );

		m_TAB.GetItemRect( 0 , rectTAB );
		rectTAB.top = rectTAB.bottom + 4;
		rectTAB.left = 2;
		rectTAB.right = rect.Width() - 4;
		rectTAB.bottom = rect.Height() - 5;// - rectTAB.top;
///////////////////////
		m_SelWnd = NULL;

#if DEF_ON_TXTIO
	if( !AddDlg( new CWQSG_TXT_IO( L"导出文本" , L"导入文本" ) , L"导文本(普通)" , rectTAB ) )
		goto __gtErrExit;
#endif

#if DEF_ON_PTXTIO
	if( !AddDlg( new CWQSG_PTXT_IO( L"导出文本" , L"导入文本" ) , L"导文本(指针)" , rectTAB ) )
		goto __gtErrExit;
#endif

#if DEF_ON_WIPS
	if( !AddDlg( new CWQSG_IPS_IO( L"制作补丁" , L"使用补丁" ) , L"WIPS补丁" , rectTAB ) )
		goto __gtErrExit;
#endif

#if DEF_ON_TBL
	if( !AddDlg( new C统计字频 , L"码表工具" , rectTAB ) )
		goto __gtErrExit;
#endif

#if DEF_ON_文本替换
	if( !AddDlg( new C批量文本替换Dlg , L"文本替换" , rectTAB ) )
		goto __gtErrExit;
#endif

#if DEF_ON_差值搜索
	if( !AddDlg( new CchazhiSearch , L"差值搜索" , rectTAB ) )
		goto __gtErrExit;
#endif

	if( !AddDlg( new CCDlgAbout , L"关于本软件" , rectTAB ) )
		goto __gtErrExit;
	}

	SelTab();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
__gtErrExit:
	CDialog::OnCancel();
	return FALSE;
}
void CWQSG_MAIN::OnClose()
{
	
	if( m_strWindowMsg.GetLength() > 0 )
	{
		if( IDYES != MessageBox( L"任务还没结束,确定要关闭么?" , NULL , MB_YESNO ) )
		{
			return;
		}
	}
	CDialog::OnClose();
	CDialog::OnCancel();
}

void CWQSG_MAIN::SelTab(void)
{
	const int nID = m_TAB.GetCurSel();
	ASSERT( nID >= 0 && (size_t)nID < m_TabWnd.size() );

	if( m_SelWnd != m_TabWnd[nID] )
	{
		if( m_SelWnd )
			m_SelWnd->ShowWindow( FALSE );

		m_SelWnd = m_TabWnd[nID];
		m_SelWnd->ShowWindow( TRUE );
	}
}

void CWQSG_MAIN::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SelTab();
}

LRESULT CWQSG_MAIN::SetText( WPARAM 保留 , LPARAM 文本 )
{
	if( 文本 )
	{
		m_strWindowMsg = (const WCHAR*)文本 ;
		m_TAB.EnableWindow( FALSE );
		m_TAB.ShowWindow( SW_HIDE );
		m_CEDIT_LOG.ShowWindow( SW_NORMAL );
		Invalidate();
	}
	else
	{
		m_strWindowMsg = L"";
		m_TAB.EnableWindow( TRUE );
		m_TAB.ShowWindow( SW_NORMAL );
		m_CEDIT_LOG.ShowWindow( SW_HIDE );
		m_CEDIT_LOG.SetWindowTextW( L"" );
		Invalidate();
	}
	return 0;
}
LRESULT CWQSG_MAIN::SetLogText( WPARAM 保留 , LPARAM 文本 )
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

	if( m_strWindowMsg.GetLength() )
	{
		CRect rect;
		GetClientRect( rect );
		rect.top = rect.bottom ;//>> 2;
//		rect.bottom = rect.top;
		const COLORREF old = dc.SetTextColor( RGB(255,0,0) );
		dc.DrawTextW( m_strWindowMsg , rect , DT_CENTER );
		dc.SetTextColor( old );
	}
}
