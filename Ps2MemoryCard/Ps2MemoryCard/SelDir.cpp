// SelDir.cpp : 实现文件
//

#include "stdafx.h"
#include "Ps2MemoryCard.h"
#include "SelDir.h"


// CSelDir 对话框

IMPLEMENT_DYNAMIC(CSelDir, CDialog)

CSelDir::CSelDir(const CString& a_strDefSel , CWnd* pParent /*=NULL*/)
	: CDialog(CSelDir::IDD, pParent)
	, m_strDefSel(a_strDefSel)
{
	WCHAR exePath[ MAX_PATH ];
	if( WQSG_GetExePath( exePath , MAX_PATH ) )
	{
		m_strExeDir = exePath;
		m_strExeDir += L"\\";
	}
}

CSelDir::~CSelDir()
{
}

void CSelDir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cDir);
}


BEGIN_MESSAGE_MAP(CSelDir, CDialog)
END_MESSAGE_MAP()


// CSelDir 消息处理程序

void CSelDir::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	const int iSel = m_cDir.GetCurSel();
	if( iSel == LB_ERR )
		return;

	m_cDir.GetText( iSel , m_strDefSel );
	m_strDefSel.Insert( 0 , m_strExeDir );

	CDialog::OnOK();
}

BOOL CSelDir::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if( m_strExeDir.GetLength() == 0 )
	{
		EndDialog( IDCANCEL );
		return FALSE;
	}

	SetDlgItemText( IDC_STATIC1 ,
		L"目录命名规则为:\"MC_XYY\"\r\n"
		L"其中X可选字母为A或B\r\n"
		L"其中YY可选数字为00到99" );

	CString strDefSel;
	for ( int n = 1 ; n < 100 ; n++ )
	{
		CString str;
		str.Format( L"MC_A%02d" , n );

		const CString strFull = m_strExeDir + str;

		if( WQSG_IsDir( strFull ) )
		{
			m_cDir.AddString( str );
			if( strFull == m_strDefSel )
				strDefSel = str;
		}
	}

	for ( int n = 1 ; n < 100 ; n++ )
	{
		CString str;
		str.Format( L"MC_B%02d" , n );

		const CString strFull = m_strExeDir + str;

		if( WQSG_IsDir( strFull ) )
		{
			m_cDir.AddString( str );
			if( strFull == m_strDefSel )
				strDefSel = str;
		}
	}

	m_cDir.SelectString( 0 , strDefSel );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
