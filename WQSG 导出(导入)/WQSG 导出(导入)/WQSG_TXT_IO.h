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
#pragma once


// CWQSG_TXT_IO 对话框
#include "afxcmn.h"
#include "WQSG_cfg.h"

template <class 窗口_1 , class 窗口_2>
class CWQSG_IO : public CDialog
{
//	DECLARE_DYNAMIC(CWQSG_IO)
	窗口_1			m_窗口_Out;
	窗口_2			m_窗口_In;
	CWnd*			m_CulWD;
	CTabCtrl		m_CTAB;
	CString			m_标题1;
	CString			m_标题2;

public:
	CWQSG_IO( CString 标题1 , CString 标题2 , CWnd* pParent = NULL)
		: CDialog(CWQSG_IO::IDD, pParent)
		, m_标题1(标题1)
		, m_标题2(标题2)
		, m_CulWD(NULL)
	{
	}
	~CWQSG_IO()
	{
	}

// 对话框数据
	enum { IDD = IDD_TXT_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_TAB1, m_CTAB);
	}

	//DECLARE_MESSAGE_MAP()
	
	virtual void OnOK(){}
	virtual void OnCancel(){}
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		// TODO: 在此添加专用代码和/或调用基类
		if( message == WM_NOTIFY )
		{
			NMHDR* pNMHDR = (NMHDR*)lParam;
			HWND hWndCtrl = pNMHDR->hwndFrom;
			if( hWndCtrl == m_CTAB.m_hWnd && pNMHDR->code == TCN_SELCHANGE )
			{
				TAB选项改变();
			}
		}
		else if( message == WM_SIZE )
		{
			if( m_CTAB.m_hWnd )
			{
				CRect rect( 0 , 0 , LOWORD(lParam), HIWORD(lParam) ) , rectTAB;

				m_CTAB.MoveWindow( rect );

				m_CTAB.GetClientRect( rect );

				m_CTAB.GetItemRect( 0 , rectTAB );
				rectTAB.top = rectTAB.bottom + 4;
				rectTAB.left = 2;
				rectTAB.right = rect.Width() - 4;
				rectTAB.bottom = rect.Height() - 5;// - rectTAB.top;

				m_窗口_Out.MoveWindow( rectTAB );
				m_窗口_In.MoveWindow( rectTAB );
			}
		}

		return CDialog::WindowProc(message, wParam, lParam);
	}
public:
	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		// TODO:  在此添加额外的初始化
		CRect rect , rectTAB;
		m_CTAB.GetClientRect( rect );

		m_CTAB.GetItemRect( 0 , rectTAB );
		rectTAB.top = rectTAB.bottom + 4;
		rectTAB.left = 2;
		rectTAB.right = rect.Width() - 4;
		rectTAB.bottom = rect.Height() - 5;// - rectTAB.top;

		int iID = 0;

	#define DEF_ADD( __DEF_X , __DEF_Y ) \
		if( !__DEF_X.Create( (UINT)__DEF_X.IDD , &m_CTAB ) )\
		goto __gt出错退出;\
		m_CTAB.InsertItem( iID++ , __DEF_Y );\
		__DEF_X.MoveWindow( rectTAB );

		DEF_ADD( m_窗口_Out , m_标题1 )
		DEF_ADD( m_窗口_In , m_标题2)

	#undef DEF_ADD

		TAB选项改变();

		return TRUE;  	// 异常: OCX 属性页应返回 FALSE
__gt出错退出:
		CDialog::OnCancel();
		return FALSE;
	}
private:
	void TAB选项改变(void)
	{
		CWnd* newCW = NULL;

		int iID = m_CTAB.GetCurSel();
		switch( iID )
		{
		case 0:
			newCW = &m_窗口_Out;
			break;
		case 1:
			newCW = &m_窗口_In;
			break;
		default:
			break;
		}
		if( newCW != m_CulWD )
		{
			if( m_CulWD )
				m_CulWD->ShowWindow( SW_HIDE );

			m_CulWD = newCW;
			m_CulWD->ShowWindow( SW_NORMAL );
		}
	}
};


#if DEF_ON_TXTIO
#include "TXT_OutBox.h"
#include "TXT_INBOX.h"
typedef CWQSG_IO< CTXT_OutBox , CTXT_INBOX > CWQSG_TXT_IO;
#endif
#if DEF_ON_WIPS
#include "WIPS.h"
typedef CWQSG_IO< CWIPS_O_Dlg , CWIPS_I_Dlg > CWQSG_IPS_IO;
#endif
#if DEF_ON_PTXTIO
#include "PTXT_OutDlg.h"
#include "PTXT_InDlg.h"
typedef CWQSG_IO< CPTXT_OutDlg , CPTXT_InDlg > CWQSG_PTXT_IO;
#endif
