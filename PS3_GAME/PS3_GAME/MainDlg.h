/*  OpenWQSG - PS3_GAME
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
// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlddx.h>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler , public CWinDataExchange<CMainDlg>
{
public:
	CListViewCtrl	m_cList;
	enum { IDD = IDD_MAINDLG };

	BEGIN_DDX_MAP(CMainDlg)
		DDX_CONTROL_HANDLE(IDC_LIST1, m_cList)
	END_DDX_MAP();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_ID_HANDLER(ID_32775, On32775)
		NOTIFY_HANDLER(IDC_LIST1, LVN_ITEMCHANGED, OnLvnItemchangedList1)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		//COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		DoDataExchange(false);

		m_cList.SetExtendedListViewStyle( m_cList.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

		m_cList.InsertColumn( 0 , L"文件夹" , 0 , 200 );
		m_cList.InsertColumn( 1 , L"游戏名" , 0 , 300 );
		m_cList.InsertColumn( 2 , L"大文件" , 0 , 50 );

#if _DEBUG
		m_strRootDir = L"F:\\ps3\\Game";
#else
		WQSG_GetExePath( m_strRootDir.GetBuffer(1000) , 1000 );
		m_strRootDir.ReleaseBuffer();
#endif
		Clear();
		FindGame( m_GameInfo );
		UpdateUI();

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		Clear();
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	//LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	CAboutDlg dlg;
	//	dlg.DoModal();
	//	return 0;
	//}

	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	// TODO: Add validation code 
	//	CloseDialog(wID);
	//	return 0;
	//}

	//LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	CloseDialog(wID);
	//	return 0;
	//}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CloseDialog(IDCANCEL);
		return 0;
	}
	
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);

		const int iIndex = m_cList.GetSelectedIndex();
		if( iIndex != -1 )
		{
			//CDCHandle dc = (HDC)wParam;

			RECT rect;
			GetClientRect( &rect );

			RECT rect2;
			rect2.left = 10;
			rect2.top = rect.bottom - 186;
			rect2.right = rect2.left + 320;
			rect2.bottom = rect2.top + 176;

			m_GameInfo[iIndex]->Draw( dc , rect2 );

			::OutputDebugString( L"d\n" );
		}

		return 0;
	}
	LRESULT OnLvnItemchangedList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		Invalidate();
		return 0;
	}
	
	LRESULT On32775(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CWQSG_DirDlg dlg( m_hWnd , L"选择路径..." );

		WCHAR path[MAX_PATH*2] = {};
		if( dlg.GetPath( path ) )
		{
			m_strRootDir = path;
			Clear();
			FindGame( m_GameInfo );
			UpdateUI();
		}

		return 0;
	}

private:
	CString m_strRootDir;
	std::vector<CPs3GameInfo*> m_GameInfo;

	void FindGame( std::vector<CPs3GameInfo*>& a_GameInfo )
	{
		CString strDir = m_strRootDir;
		if( strDir.Right(1) != L'\\' && strDir.Right(1) != L'/' )
			strDir += L'\\';

		WIN32_FIND_DATA data;
		const HANDLE handle = FindFirstFile( strDir + L"*." , &data );

		if( handle == INVALID_HANDLE_VALUE )
			return ;

		do
		{
			if( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				const CString strFile = data.cFileName;
				if( strFile == L"." || strFile == L".." )
					continue;

				CPs3GameInfo* pInfo = new CPs3GameInfo;

				const BOOL bRt = pInfo->LoadInfo( strDir , strFile );
				//if( !bRt )
				//	continue;

				a_GameInfo.push_back( pInfo );
			}
		}
		while( FindNextFile( handle , &data ) );

		FindClose( handle );
	}

	void Clear()
	{
		std::vector<CPs3GameInfo*>::iterator it = m_GameInfo.begin();
		for( ; it != m_GameInfo.end() ; ++it )
		{
			delete *it;
		}
		m_GameInfo.clear();
	}

	void UpdateUI()
	{
		m_cList.DeleteAllItems();
		m_cList.SetRedraw( FALSE );

		std::vector<CPs3GameInfo*>::iterator it = m_GameInfo.begin();
		for( ; it != m_GameInfo.end() ; ++it )
		{
			const CPs3GameInfo* pInfo = *it;

			const int iIndex = m_cList.InsertItem( m_cList.GetItemCount() , pInfo->GetDirName() );
			m_cList.SetItemText( iIndex , 1 , pInfo->GetGameName() );
			m_cList.SetItemText( iIndex , 2 , pInfo->IsHasBigFile()?L"O":L"X" );
		}

		m_cList.SetRedraw( TRUE );
	}
};
