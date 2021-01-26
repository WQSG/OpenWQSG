/*  OpenWQSG - CodePageConversioner
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
// CodePageConversionerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodePageConversioner.h"
#include "CodePageConversionerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
struct __TgCPCP_IN
{
	WCHAR const*const	name;
	const INT			codePage;
};
struct __TgCPCP_OUT
{
	WCHAR const*const	name;
	const BOOL			ANSI_type;
	const INT			codePage;
};
__TgCPCP_IN		WQSG_CP_in[] = {
	{ L"��������"	,	936 },
	{ L"��������"	,	950 },
	{ L"����SJ"		,	932 },
};
__TgCPCP_OUT		WQSG_CP_out[] = {
	{ L"UNICODE"	,	FALSE	,	0 },
	{ L"UTF-8"		,	TRUE	,	CP_UTF8 },
	{ L"��������"	,	TRUE	,	936 },
	{ L"����SJ"		,	TRUE	,	932 },
};
// CCodePageConversionerDlg �Ի���
CCodePageConversionerDlg::CCodePageConversionerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodePageConversionerDlg::IDD, pParent)
{
	m_hIcon = NULL;//AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CCodePageConversionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cCP);
	DDX_Control(pDX, IDC_COMBO2, m_cCP2);
}
BEGIN_MESSAGE_MAP(CCodePageConversionerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CCodePageConversionerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()// CCodePageConversionerDlg ��Ϣ�������
BOOL CCodePageConversionerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	for( int i = 0 ; ( i >= 0 ) && (i < (sizeof(WQSG_CP_in)/sizeof(*WQSG_CP_in))) ; ++i )
	{
		int index = m_cCP.AddString( WQSG_CP_in[i].name );
		if( index < 0 )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}
		m_cCP.SetItemDataPtr( index , &WQSG_CP_in[i] );
	}
	m_cCP.SetCurSel( 0 );
	for( int i = 0 ; ( i >= 0 ) && (i < (sizeof(WQSG_CP_out)/sizeof(*WQSG_CP_out))) ; ++i )
	{
		int index = m_cCP2.AddString( WQSG_CP_out[i].name );
		if( index < 0 )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}
		m_cCP2.SetItemDataPtr( index , &WQSG_CP_out[i] );
	}
	m_cCP2.SetCurSel( 0 );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// �����Ի��������С����ť������Ҫ����Ĵ��������Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó����⽫�ɿ���Զ���ɡ�
void CCodePageConversionerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CCodePageConversionerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CCodePageConversionerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();
	CDialog::OnCancel();
}
void CCodePageConversionerDlg::OnOK(){}
void CCodePageConversionerDlg::OnCancel(){}
void CCodePageConversionerDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	const int iSel1 = m_cCP.GetCurSel();
	if( iSel1 < 0 )
	{
		MessageBox( L"�������ҳûѡ��" );
		return;
	}
	const int iSel2 = m_cCP2.GetCurSel();
	if( iSel2 < 0 )
	{
		MessageBox( L"�������ҳûѡ��" );
		return;
	}

	__TgCPCP_IN*const cp_in = (__TgCPCP_IN*)m_cCP.GetItemDataPtr( iSel1 );
	if( NULL == cp_in )
	{
		MessageBox( L"�������ҳָ��Ϊ��" );
		return;
	}

	__TgCPCP_OUT*const cp_out = (__TgCPCP_OUT*)m_cCP2.GetItemDataPtr( iSel2 );
	if( NULL == cp_out )
	{
		MessageBox( L"�������ҳָ��Ϊ��" );
		return;
	}

	static CWQSGFileDialog_OpenS dlg( L"*.txt|*.txt||" );

	if( dlg.DoModal() != IDOK )
		return;

	int count = 0;
	POSITION pos = dlg.GetStartPosition();
	CString strName;
	while( dlg.GetNextPathName( strName , pos ) )
	{
		BOOL ת���ļ�( HWND hWnd , CStringW src , __TgCPCP_IN& cp_in , __TgCPCP_OUT& cp_out );

		if( !ת���ļ�( m_hWnd , strName.GetString() , *cp_in , *cp_out ) )
			break;

		++count;
	}

	if( count > 0 )
	{
		CStringW str;
		str.Format( L"�㶨 %u ��" , count );
		MessageBox( str );
	}
}
BOOL ת���ļ�( HWND hWnd , CStringW src , __TgCPCP_IN& cp_in , __TgCPCP_OUT& cp_out )
{
	CMemTextW tfp;
	if( !tfp.Load( src.GetString() , 1024*1024*8 , cp_in.codePage ) )
	{
		MessageBox( hWnd , tfp.GetErrTXT() , src , MB_OK );
		return FALSE;
	}
	//-----------------------------------------------------------------
	if( cp_out.ANSI_type )//Ŀ����ANSI ����
	{
		if( ((cp_out.codePage == CP_UTF8) && (tfp.GetCP() == EWQSG_CodePage::E_CP_UTF8)) ||
			(( cp_in.codePage == cp_out.codePage ) && (tfp.GetCP() == EWQSG_CodePage::E_CP_ANSI) )
			)
			return TRUE;

		switch( tfp.GetCP() )
		{
		case EWQSG_CodePage::E_CP_UNICODE:
		case EWQSG_CodePage::E_CP_ANSI:
			break;
		case EWQSG_CodePage::E_CP_UNICODE_BIG:
		default:
			MessageBox( hWnd , L"��֧�ֵ�ת��ģʽ" , src , MB_OK );
			return FALSE;
		}
		//--------------
		BOOL �޷�ת�� = CP_UTF8 != cp_out.codePage;
		char* buffer;
		int	iLen = ::WideCharToMultiByte( cp_out.codePage , 0  ,(LPWSTR)tfp.GetText() , -1 , NULL , 0 , NULL ,(�޷�ת��)?(&�޷�ת��):NULL );
		if( �޷�ת�� )
		{
			if( IDYES != MessageBox( hWnd , L"���ı������޷�ת�����ַ�,Ҫ����ô?" , src , MB_YESNO ) )
				return FALSE;
		}

		if( (iLen > 0) &&
			( buffer = new char[iLen] )
			)
		{
			::WideCharToMultiByte ( cp_out.codePage , 0 , (LPWSTR)tfp.GetText() , -1 , buffer , iLen , NULL , NULL );
		}
		else
		{
			MessageBox( hWnd , L"ת��ʧ��" , src , MB_OK );
			return FALSE;
		}
		//-------------------------------------------------------
		tfp.Clear();
#if 1
		CWQSG_File	fp;
		if( !fp.OpenFile( src.GetString() , 4 , 3 ) )
		{
			MessageBox( hWnd , L"���ļ�ʧ��(new)" , src , MB_OK );
			return FALSE;
		}

		if( cp_out.codePage == CP_UTF8 )
		{
			if( 3 != fp.Write( "\xEF\xBB\xBF" , 3 ) )
			{
				MessageBox( hWnd , L"д�ı���ʶʧ��" , src , MB_OK );
				return FALSE;
			}
		}
		iLen--;
		if( iLen != fp.Write( buffer , iLen ) )
		{
			MessageBox( hWnd , L"д�ı�ʧ��" , src , MB_OK );
			return FALSE;
		}
#endif
	}
	else
	{
		switch( tfp.GetCP() )
		{
		case EWQSG_CodePage::E_CP_UNICODE:
			return TRUE;
		case EWQSG_CodePage::E_CP_UTF8:
		case EWQSG_CodePage::E_CP_ANSI:
			break;
		case EWQSG_CodePage::E_CP_UNICODE_BIG:
		default:
			MessageBox( hWnd , L"��֧�ֵ�ת��ģʽ" , src , MB_OK );
			return FALSE;
		}
		//---------
		const int	iLen = WQSG_strlen( tfp.GetText() );

		if( (iLen < 0) || ((iLen<<1) < 0 ) || ((iLen<<1) < iLen) )
			return FALSE;

		WCHAR*const buffer = new WCHAR[iLen+1];
		if( NULL == buffer )
		{
			MessageBox( hWnd , L"�����ڴ�ʧ��" , src , MB_OK );
			return FALSE;
		}
		memcpy( buffer , tfp.GetText() , iLen<<1 );
		buffer[iLen] = L'\0';
		//-------------------------------------------------------
		tfp.Clear();
#if 1
		CWQSG_File	fp;
		if( !fp.OpenFile( src.GetString() , 4 , 3 ) )
		{
			MessageBox( hWnd , L"���ļ�ʧ��(new)" , src , MB_OK );
			return FALSE;
		}
		if( 2 != fp.Write( "\xFF\xFE" , 2 ) )
		{
			MessageBox( hWnd , L"д�ı���ʶʧ��" , src , MB_OK );
			return FALSE;
		}
		if( (iLen<<1) != (int)fp.Write( buffer , iLen<<1 ) )
		{
			MessageBox( hWnd , L"д�ı�ʧ��" , src , MB_OK );
			return FALSE;
		}
#endif
	}
	return TRUE;
}
