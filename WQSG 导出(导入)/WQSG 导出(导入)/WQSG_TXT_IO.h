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

template <class TWindow1 , class TWindow2>
class CWQSG_IO : public CBaseDialog
{
//	DECLARE_DYNAMIC(CWQSG_IO)
	TWindow1		m_Window_Out;
	TWindow2		m_Window_In;
	CWnd*			m_CulWD;
	CTabCtrl		m_CTAB;
	CString			m_strTitle1;
	CString			m_strTitle2;

public:
	CWQSG_IO( CString a_strTitle1 , CString a_strTitle2 , CWnd* pParent = NULL)
		: CBaseDialog(CWQSG_IO::IDD, pParent)
		, m_strTitle1(a_strTitle1)
		, m_strTitle2(a_strTitle2)
		, m_CulWD(NULL)
	{
	}

	virtual ~CWQSG_IO()
	{
	}

// 对话框数据
	enum { IDD = IDD_TXT_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CBaseDialog::DoDataExchange(pDX);
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
				TabChange();
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

				m_Window_Out.MoveWindow( rectTAB );
				m_Window_In.MoveWindow( rectTAB );
			}
		}

		return CBaseDialog::WindowProc(message, wParam, lParam);
	}
public:
	virtual BOOL OnInitDialog()
	{
		CBaseDialog::OnInitDialog();

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
		goto __gtErrExit;\
		m_CTAB.InsertItem( iID++ , __DEF_Y );\
		__DEF_X.MoveWindow( rectTAB );

		DEF_ADD( m_Window_Out , m_strTitle1 )
		DEF_ADD( m_Window_In , m_strTitle2)

	#undef DEF_ADD

		TabChange();

		return TRUE;  	// 异常: OCX 属性页应返回 FALSE
__gtErrExit:
		CBaseDialog::OnCancel();
		return FALSE;
	}
private:
	void TabChange(void)
	{
		CWnd* newCW = NULL;

		int iID = m_CTAB.GetCurSel();
		switch( iID )
		{
		case 0:
			newCW = &m_Window_Out;
			break;
		case 1:
			newCW = &m_Window_In;
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
