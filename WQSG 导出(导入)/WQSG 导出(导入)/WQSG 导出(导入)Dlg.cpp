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
// MFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WQSG 导出(导入)Dlg.h"
#include "page_TXT.h"
#include "page_PTXT.h"
#include "导出导入批处理.h"

#include "page_txt.h"
#include "page_ptxt.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTabCtrl W_TabC;
static Cpage_TXT_OUT	W_page_TXT_OUT;
static Cpage_TXT_IN		W_page_TXT_IN;
static Cpage_PTXT_OUT	W_page_PTXT_OUT;
static Cpage_PTXT_IN	W_page_PTXT_IN;
static CString			W_标题;

#define WM_WQSG_设置前端内容	(WM_USER + 1)
#define WM_WQSG_取前端窗口		(WM_WQSG_设置前端内容 + 1)

LRESULT CWQSG导出导入Dlg::设置前端内容( WPARAM 保留 , LPARAM 内容 )
{
	BOOL 是否禁用 = ( 内容 != 0 );
	if( 是否禁用 )
	{
		if( m_最前端对话框.m_hWnd == NULL )
		{
			m_最前端对话框.Create(IDD_AB);
			m_最前端对话框.ShowWindow(TRUE);
			m_最前端对话框.SetWindowText( W_标题 );
			m_最前端对话框.设置内容( (WCHAR*)内容 );
		}
		else
		{
			m_最前端对话框.设置内容( (WCHAR*)内容 );
		}
	}
	else
	{
		m_最前端对话框.DestroyWindow();
	}
	ShowWindow( !是否禁用 );
	return 0;
}

LRESULT CWQSG导出导入Dlg::取前端窗口( WPARAM 保留 , LPARAM 窗口指针 )
{
	*(HWND*)窗口指针 = m_最前端对话框.m_hWnd;
	return 0;
}
//////////////////////////
DWORD WINAPI CWQSG导出导入Dlg::普通导出(LPVOID lpParameter)
{
	参数结构*参数表 = (参数结构*)lpParameter;
	U64 开始地址 = 0,结束地址 = 0;
	UINT xyz;
	_stscanf_s(W_page_TXT_OUT.W_KS2.GetBuffer(),_T("%x"),&xyz);
	开始地址 |= xyz;
	结束地址 |= xyz;
	开始地址 <<= 32;
	结束地址 <<= 32;
	////////////////////////////////////////
	_stscanf_s(W_page_TXT_OUT.W_KS.GetBuffer(),_T("%x"),&xyz);
	开始地址 |= xyz;
	_stscanf_s(W_page_TXT_OUT.W_JS.GetBuffer(),_T("%x"),&xyz);
	结束地址 |= xyz;

	HWND 前端窗口 = NULL;
	::SendMessage( 参数表->hwnd , WM_WQSG_取前端窗口 , 0 , (LPARAM)&前端窗口 );

	::WQSG_TXT_OUT WQSG( 前端窗口 , L"WQSG 导出程序" );

	if(WQSG.OUT_TXT(参数表->ROM.GetBuffer(),参数表->TBL.GetBuffer(),参数表->TBL2.GetBuffer(),参数表->TXT.GetBuffer(),
		开始地址,结束地址,参数表->验证,
		_tstoi(W_page_TXT_OUT.W_MIN),_tstoi(W_page_TXT_OUT.W_MAX)))
	{
		::MessageBox( 前端窗口,_T("恭喜,导出成功"),_T("WQSG 导出程序"),MB_OK);
	}

	::SendMessage( 参数表->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
	return 0;
}

DWORD WINAPI CWQSG导出导入Dlg::普通导入(LPVOID lpParameter){
	//W_page_TXT_IN.UpdateData();
	参数结构*参数表 = (参数结构*)lpParameter;

	HWND 前端窗口 = NULL;
	::SendMessage( 参数表->hwnd , WM_WQSG_取前端窗口 , 0 , (LPARAM)&前端窗口 );

	U8 单=0,双_1=0,双_2=0;
	int kk;
	CString tmp;
	tmp = W_page_TXT_IN.单字节填充;
	if(W_page_TXT_IN.填充类型 & 1){
		if(0 == (tmp.GetLength())){
			if(IDNO == ::MessageBox( 前端窗口 , _T("单字节填充 为空,程序自动以 00 填充\n要继续吗?"),_T("WQSG 导入程序"),MB_OK)){
				::SendMessage( 参数表->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
				return 0;
			}
		}
		while(2 != (tmp.GetLength())){
			tmp.Insert(0,_T('0'));
		}
		::_stscanf_s(tmp.GetBuffer(),_T("%2x"),&kk);
		单 = kk;
	}
	tmp = W_page_TXT_IN.双字节填充;
	if(W_page_TXT_IN.填充类型 & 2){
		if(0 == (tmp.GetLength())){
			if(IDNO == ::MessageBox( 前端窗口 , _T("双字节填充 为空,程序自动以 0000 填充\n要继续吗?"),_T("WQSG 导入程序"),MB_OK)){
				::SendMessage( 参数表->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
				return 0;
			}
		}
		while( 4 != (tmp.GetLength())){
			tmp.Insert(0,_T('0'));
		}
		::_stscanf_s(tmp.GetBuffer(),_T("%2x"),&kk);
		双_1 = kk;
		::_stscanf_s(tmp.GetBuffer() + 2,_T("%2x"),&kk);
		双_2 = kk;
	}
	INT 长度不足提示 = (W_page_TXT_IN.W_长度不足提示)?1:0;

	::WQSG_TXT_IN WQSG( 前端窗口 ,_T("WQSG 导入程序"));
	if(WQSG.IN_TXT(参数表->ROM.GetBuffer(),参数表->TBL.GetBuffer(),参数表->TBL2.GetBuffer(),参数表->TXT.GetBuffer(),
		W_page_TXT_IN.填充类型,单,双_1,双_2,
		参数表->验证,
		长度不足提示
		))
	{
		::MessageBox( 前端窗口 , _T("恭喜,导入成功"),_T("WQSG 导入程序"),MB_OK);
	}

	::SendMessage( 参数表->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
	return 0;
}
DWORD WINAPI CWQSG导出导入Dlg::地址_指针导出(LPVOID lpParameter)
{
	参数结构*参数表 = (参数结构*)lpParameter;
	U64 首指针地址 = 0,最后指针地址 = 0;
	U32 文本基础地址 = 0;
	U8 指针间隔,指针实长,指针倍率;

	UINT xyz = 0;	
	_stscanf_s(W_page_PTXT_OUT.W_pKS.GetBuffer(),_T("%x"),&xyz);
	首指针地址 = (U64)xyz;

	xyz = 0;
	_stscanf_s(W_page_PTXT_OUT.W_pJS.GetBuffer(),_T("%x"),&xyz);
	最后指针地址 = (U64)xyz;

	xyz = 0;
	_stscanf_s(W_page_PTXT_OUT.W_p文本基础.GetBuffer(),_T("%x"),&xyz);
	文本基础地址 = (U32)xyz;

	指针间隔 = (U8)::_ttoi(W_page_PTXT_OUT.W_p指针间距);
	指针实长 = (U8)::_ttoi(W_page_PTXT_OUT.W_p实际指针长度);
	指针倍率 = (U8)::_ttoi(W_page_PTXT_OUT.W_p指针倍数);

	HWND 前端窗口 = NULL;
	::SendMessage( 参数表->hwnd , WM_WQSG_取前端窗口 , 0 , (LPARAM)&前端窗口 );

	::WQSG_pTXT_OUT WQSG( 前端窗口 , L"WQSG 导出程序");
	WQSG.地址_指针OUT(参数表->ROM.GetBuffer(),参数表->TBL.GetBuffer(),参数表->TBL2.GetBuffer(),参数表->TXT.GetBuffer(),
		参数表->验证,首指针地址,最后指针地址,指针间隔,
		指针实长,文本基础地址,指针倍率);

	::SendMessage( 参数表->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
	return 0;
}
DWORD WINAPI CWQSG导出导入Dlg::地址_指针导入(LPVOID lpParameter){
	参数结构*参数表 = (参数结构*)lpParameter;
	U64 文本区开始地址,文本区界限地址;

	
	UINT X = 0;
	::swscanf_s(W_page_PTXT_IN.m_p文本区开始地址.GetBuffer(),L"%x",&X);
	文本区开始地址 = X;

	X = 0;
	::swscanf_s(W_page_PTXT_IN.m_p文本区界限地址.GetBuffer(),L"%x",&X);
	文本区界限地址 = X;

	HWND 前端窗口 = NULL;
	::SendMessage( 参数表->hwnd , WM_WQSG_取前端窗口 , 0 , (LPARAM)&前端窗口 );

	::WQSG_pTXT_I WQSG(前端窗口,L"WQSG 指针导入程序");
	if(WQSG.载入码表(参数表->TBL.GetBuffer(),参数表->TBL2.GetBuffer(),参数表->验证)){
		if(WQSG.导入文本(参数表->ROM.GetBuffer(),参数表->TXT.GetBuffer(),文本区开始地址,文本区界限地址)){
			::MessageBox(前端窗口,_T("恭喜,导入成功"),_T("WQSG 导入程序"),MB_OK);
		}
	}
//	::MessageBox(参数表->hwnd,_T("暂时不支持"),_T("WQSG 指针导入程序"),MB_OK);
	::SendMessage( 参数表 ->hwnd , WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
	return 0;
}
// CMFCDlg 对话框
void CWQSG导出导入Dlg::写配置(void)
{
	W_page_TXT_OUT .UpdateData();
	
	WritePrivateProfileString(m_NAME,_T("地址段"),W_page_TXT_OUT.W_KS2,配置文件);
	WritePrivateProfileString(m_NAME,_T("开始地址"),W_page_TXT_OUT.W_KS,配置文件);
	WritePrivateProfileString(m_NAME,_T("结束地址"),W_page_TXT_OUT.W_JS,配置文件);
	WritePrivateProfileString(m_NAME,_T("过滤小"),W_page_TXT_OUT.W_MIN,配置文件);
	WritePrivateProfileString(m_NAME,_T("过滤大"),W_page_TXT_OUT.W_MAX,配置文件);


	W_page_TXT_IN.UpdateData();
	
	CString tmp;
	tmp.Format(_T("%d"),W_page_TXT_IN.填充类型);
	WritePrivateProfileString(m_NAME,_T("填充类型"),tmp,配置文件);
	WritePrivateProfileString(m_NAME,_T("单字节填充"),W_page_TXT_IN.单字节填充,配置文件);
	WritePrivateProfileString(m_NAME,_T("双字节填充"),W_page_TXT_IN.双字节填充,配置文件);

	W_page_PTXT_OUT.UpdateData();
	
	WritePrivateProfileString(m_NAME,_T("p开始地址"),W_page_PTXT_OUT.W_pKS,配置文件);
	WritePrivateProfileString(m_NAME,_T("p结束地址"),W_page_PTXT_OUT.W_pJS,配置文件);
	WritePrivateProfileString(m_NAME,_T("p文本基础地址"),W_page_PTXT_OUT.W_p文本基础,配置文件);
	WritePrivateProfileString(m_NAME,_T("p指针间距"),W_page_PTXT_OUT.W_p指针间距,配置文件);
	WritePrivateProfileString(m_NAME,_T("p实际指针长度"),W_page_PTXT_OUT.W_p实际指针长度,配置文件);
	WritePrivateProfileString(m_NAME,_T("p指针倍数"),W_page_PTXT_OUT.W_p指针倍数,配置文件);
	WritePrivateProfileString(m_NAME,_T("p文本长度"),W_page_PTXT_OUT.W_文本长度,配置文件);

	tmp.Format(_T("%d"),W_page_PTXT_OUT.W_p指针格式);
	WritePrivateProfileString(m_NAME,_T("p指针格式"),tmp,配置文件);
	WritePrivateProfileString(m_NAME,_T("p长度块倍数"),W_page_PTXT_OUT.W_p长度块倍数,配置文件);

	W_page_PTXT_IN.UpdateData();

	WritePrivateProfileString(m_NAME,_T("p文本区界限地址"),W_page_PTXT_IN.m_p文本区界限地址,配置文件);
	WritePrivateProfileString(m_NAME,_T("p文本区开始地址"),W_page_PTXT_IN.m_p文本区开始地址,配置文件);
}
void CWQSG导出导入Dlg::写列表(CListBox * lb_tmp){
	::CWQSG_File wfile;
	if(wfile.OpenFile(配置列表文件,4,3)){

		wfile.Write("\377\376",2);

		for( int k = lb_tmp->GetCount(),i = 0;i < k;i++){
			CString tmp;
			lb_tmp->GetText(i,tmp);
			tmp += _T("\r\n");
			wfile.Write(tmp.GetBuffer(),(::WQSG_strlen(tmp.GetBuffer())<<1));
		}
	}
}
void CWQSG导出导入Dlg::改变选项(void){
	CButton*tmp = (CButton*)GetDlgItem(IDC_CHECK_PTXT);
	switch(W_TabC.GetCurSel()){
		case 0:
			W_page_TXT_IN.ShowWindow(FALSE);
			W_page_PTXT_IN.ShowWindow(FALSE);
			if(tmp->GetCheck()){
				W_page_TXT_OUT.ShowWindow(FALSE);
				W_page_PTXT_OUT.ShowWindow(TRUE);
			}
			else{
				W_page_TXT_OUT.ShowWindow(TRUE);
				W_page_PTXT_OUT.ShowWindow(FALSE);
			}			
			break;
		case 1:
			W_page_TXT_OUT.ShowWindow(FALSE);
			W_page_PTXT_OUT.ShowWindow(FALSE);
			if(tmp->GetCheck()){
				W_page_TXT_IN.ShowWindow(FALSE);
				W_page_PTXT_IN.ShowWindow(TRUE);
			}
			else{
				W_page_TXT_IN.ShowWindow(TRUE);
				W_page_PTXT_IN.ShowWindow(FALSE);
			}
			break;
		default:
			;
	}
}

void CWQSG导出导入Dlg::创建目录(void)
{
	CString tmp = W_运行目录;
	tmp += _T("\\DATA");
	CreateDirectory(tmp,NULL);
	tmp += _T("\\WQSG 导出(导入)");
	CreateDirectory(tmp,NULL);
}
CWQSG导出导入Dlg::CWQSG导出导入Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWQSG导出导入Dlg::IDD, pParent)
	, W_NAME(_T(""))
	, W_ROM(_T(""))
	, W_TBL(_T(""))
	, W_TBL2(_T(""))
	, m_EDIT_ROM(_T(""))
{
	W_标题.LoadStringW(IDS_TXTIO);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_WQSG);

}


void CWQSG导出导入Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, W_NAME);
	DDV_MaxChars(pDX, W_NAME, 32);
	DDX_Control(pDX, IDC_TAB1, W_TabC);
	//	DDX_Control(pDX, IDC_LIST2, W_LIST);
	DDX_Text(pDX, IDC_EDIT_TXT_DIR, m_EDIT_ROM);
	DDV_MaxChars(pDX, m_EDIT_TXT, 260);
}

BEGIN_MESSAGE_MAP(CWQSG导出导入Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Add, &CWQSG导出导入Dlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Edit, &CWQSG导出导入Dlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_Del, &CWQSG导出导入Dlg::OnBnClickedButtonDel)
	__i__
	ON_BN_CLICKED(IDC_BUTTON_ROM, &CWQSG导出导入Dlg::OnBnClickedButtonRom)
	ON_BN_CLICKED(IDC_BUTTON_TBL, &CWQSG导出导入Dlg::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL2, &CWQSG导出导入Dlg::OnBnClickedButtonTbl2)
	__i__
	ON_EN_CHANGE(IDC_EDIT_ROM, &CWQSG导出导入Dlg::OnEnChangeEditRom)
	ON_EN_CHANGE(IDC_EDIT_TBL, &CWQSG导出导入Dlg::OnEnChangeEditTbl)
	ON_EN_CHANGE(IDC_EDIT_TBL2, &CWQSG导出导入Dlg::OnEnChangeEditTbl2)
	__i__
	ON_EN_KILLFOCUS(IDC_EDIT_ROM, &CWQSG导出导入Dlg::OnEnKillfocusEditRom)
	__i__
	ON_BN_CLICKED(IDC_BUTTON_START, &CWQSG导出导入Dlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_BAT, &CWQSG导出导入Dlg::OnBnClickedButtonBat)
	__i__
	ON_EN_UPDATE(IDC_EDIT_NAME, &CWQSG导出导入Dlg::OnEnUpdateEditName)
	ON_LBN_SELCHANGE(IDC_LIST2, &CWQSG导出导入Dlg::OnLbnSelchangeList2)
	__i__
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CWQSG导出导入Dlg::OnEnKillfocusEditName)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWQSG导出导入Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_CHECK_PTXT, &CWQSG导出导入Dlg::OnBnClickedCheckPtxt)
	__i__
	ON_WM_CLOSE()
	__i__
	ON_MESSAGE( WM_WQSG_设置前端内容 , &CWQSG导出导入Dlg::设置前端内容 )
	ON_MESSAGE( WM_WQSG_取前端窗口 , &CWQSG导出导入Dlg::取前端窗口 )
END_MESSAGE_MAP()
BOOL CWQSG导出导入Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowTextW(W_标题);


	((CButton*)GetDlgItem(IDC_CHECK_YZ))->SetCheck(TRUE);

	W_TabC.InsertItem(0,_T("导出"));
	W_TabC.InsertItem(1,_T("导入"));

	W_page_TXT_OUT.Create(IDD_TXT_OUT,&W_TabC);
	W_page_TXT_IN.Create(IDD_TXT_IN,&W_TabC);
	W_page_PTXT_OUT.Create(IDD_PTXT_OUT,&W_TabC);
	W_page_PTXT_IN.Create(IDD_PTXT_IN,&W_TabC);

	CRect rs,rs2;
	W_TabC.GetClientRect(&rs);
	W_TabC.GetItemRect(0,&rs2);
	rs.top += rs2.bottom + 4;
	rs.bottom -= 8;
	rs.left += 4;
	rs.right -= 8;
	W_page_TXT_OUT.MoveWindow(rs);
	W_page_TXT_IN.MoveWindow(rs);
	W_page_PTXT_OUT.MoveWindow(rs);
	W_page_PTXT_IN.MoveWindow(rs);
	W_page_TXT_OUT.ShowWindow(TRUE);
	
	W_TabC.SetCurSel(0);

	::CWQSG_File wfile;
	if(wfile.OpenFile(配置列表文件,1,3)){
		CListBox
			*lb_tmp = ((CListBox*)GetDlgItem(IDC_LIST2));

		WCHAR * 全文 = NULL;
		U64 SIZE;
		wfile.GetFileSize(&SIZE);
		if(SIZE <= 1048576){

			switch(::WQSG_A_U_X(wfile.GetFileHANDLE())){
				case 1:
					wfile.Seek(0);
					{
						U8 * tmpU = new U8[(UINT)SIZE+1];
						tmpU[(UINT)SIZE] = '\0';

						wfile.Read(tmpU,(UINT)SIZE);

						全文 = ::WQSG_char_W( (char*)tmpU);
						delete[]tmpU;
					}
					break;
				case 2:
					wfile.Seek(2);
					全文 = new WCHAR[((UINT)SIZE)>>1];
					SIZE -= 2;
					全文[((UINT)SIZE)>>1] = L'\0';
					wfile.Read(全文,(UINT)SIZE);
					break;
				default:
					break;
			}
			if(全文 != NULL){
				WCHAR * const 全文_del = 全文;
				while(WCHAR * const 一行 = ::WQSG_GetTXT_Line(&全文)){
					lb_tmp->AddString(一行);
					delete[]一行;
				}
				delete[]全文_del;
			}
		}
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWQSG导出导入Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
HCURSOR CWQSG导出导入Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWQSG导出导入Dlg::OnOK(){}
void CWQSG导出导入Dlg::OnCancel(){}
int CWQSG导出导入Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	TCHAR exeFullPath[MAX_PATH]; 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	W_运行目录 = exeFullPath;
	W_运行目录 = W_运行目录.Left(W_运行目录.ReverseFind(_T('\\')));
	配置列表文件 = 配置文件 = W_运行目录;
	创建目录();
	配置文件 += _T("\\DATA\\WQSG 导出(导入)\\配置.CFG");
	配置列表文件 += _T("\\DATA\\WQSG 导出(导入)\\配置列表.CFG");
	
	return 0;
}
void CWQSG导出导入Dlg::OnBnClickedButtonRom()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open fopendlg(_T("ROM文件(*.*)|*.*||"));

	fopendlg.SetWindowTitle( _T("选择一个ROM...") );

	if(IDOK == fopendlg.DoModal())
	{
		W_page_TXT_OUT.W_ROM地址 = W_ROM = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}
void CWQSG导出导入Dlg::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open fopendlg(_T("码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||"));
	fopendlg.SetWindowTitle( _T("选择一个码表文件...") );

	if(IDOK == fopendlg.DoModal())
	{
		W_TBL = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}

void CWQSG导出导入Dlg::OnBnClickedButtonTbl2()
{
	// TODO: 在此添加控件通知处理程序代码
	static CWQSGFileDialog_Open fopendlg(_T("码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||"));
	fopendlg.SetWindowTitle( _T("选择一个控制码表...") );

	if(IDOK == fopendlg.DoModal())
	{
		W_TBL2 = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}

void CWQSG导出导入Dlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if( _T("") == W_NAME )
	{
		MessageBox(_T("文件名不能为空"));
		return;
	}
		
	CListBox
		* lb_tmp = ((CListBox*)GetDlgItem(IDC_LIST2));
	if(LB_ERR != lb_tmp->FindString(0,W_NAME))
	{
		MessageBox(_T("文件名重复"));
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetSel(0,-1);
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetFocus();
		return ;
	}
	lb_tmp->SetCurSel(lb_tmp->AddString(W_NAME));
	lb_tmp->SetFocus();
	UpdateData(FALSE);
	写配置();

	写列表(lb_tmp);
}
void CWQSG导出导入Dlg::OnEnUpdateEditName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数，将 EM_SETEVENTMASK 消息发送到控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_NAME.Left(1))
		|| (_T('　') == W_NAME.Left(1))
		)
	{
	W_NAME.TrimLeft(_T(' '));
	W_NAME.TrimLeft(_T('　'));
	}
	UpdateData(FALSE);
}
void CWQSG导出导入Dlg::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox
		*lb_tmp = ((CListBox*)GetDlgItem(IDC_LIST2));
	INT
		j = lb_tmp->GetCurSel();
	if(-1 != j){
		lb_tmp->GetText(j,W_NAME);
		UpdateData(FALSE);

		TCHAR tmp [1024];
		GetPrivateProfileString(W_NAME,_T("地址段"),NULL,tmp,1024,配置文件);
		if(0 == *tmp){
			W_page_TXT_OUT.W_KS2 = _T("0");
		}
		else{
			W_page_TXT_OUT.W_KS2 = tmp;
		}
		GetPrivateProfileString(W_NAME,_T("开始地址"),NULL,tmp,1024,配置文件);
		W_page_TXT_OUT.W_KS = tmp;
		GetPrivateProfileString(W_NAME,_T("结束地址"),NULL,tmp,1024,配置文件);
		W_page_TXT_OUT.W_JS = tmp;
		GetPrivateProfileString(W_NAME,_T("过滤小"),_T("0"),tmp,1024,配置文件);
		W_page_TXT_OUT.W_MIN = tmp;
		GetPrivateProfileString(W_NAME,_T("过滤大"),_T("99999"),tmp,1024,配置文件);
		W_page_TXT_OUT.W_MAX = tmp;
		W_page_TXT_OUT .UpdateData(FALSE);


		GetPrivateProfileString(W_NAME,_T("填充类型"),_T("0"),tmp,1024,配置文件);
		W_page_TXT_IN.填充类型 = _tstoi(tmp);
		GetPrivateProfileString(W_NAME,_T("单字节填充"),NULL,tmp,1024,配置文件);
		W_page_TXT_IN.单字节填充 = tmp;
		GetPrivateProfileString(W_NAME,_T("双字节填充"),NULL,tmp,1024,配置文件);
		W_page_TXT_IN.双字节填充 = tmp;	
		W_page_TXT_IN.UpdateData(FALSE);
		W_page_TXT_IN.OnCbnSelchangeCombo1();

		GetPrivateProfileString(W_NAME,_T("p开始地址"),NULL,tmp,1024,配置文件);
		W_page_PTXT_OUT.W_pKS = tmp;
		GetPrivateProfileString(W_NAME,_T("p结束地址"),NULL,tmp,1024,配置文件);
		W_page_PTXT_OUT.W_pJS = tmp;
		GetPrivateProfileString(W_NAME,_T("p文本基础地址"),NULL,tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p文本基础 = tmp;
		GetPrivateProfileString(W_NAME,_T("p指针间距"),_T("2"),tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p指针间距 = tmp;
		GetPrivateProfileString(W_NAME,_T("p实际指针长度"),_T("2"),tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p实际指针长度 = tmp;
		GetPrivateProfileString(W_NAME,_T("p指针倍数"),_T("1"),tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p指针倍数 = tmp;
		GetPrivateProfileString(W_NAME,_T("p文本长度"),_T("1"),tmp,1024,配置文件);
		W_page_PTXT_OUT.W_文本长度 = tmp;


		GetPrivateProfileString(W_NAME,_T("p指针格式"),_T("0"),tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p指针格式 = _wtoi(tmp);
		GetPrivateProfileString(W_NAME,_T("p长度块倍数"),NULL,tmp,1024,配置文件);
		W_page_PTXT_OUT.W_p长度块倍数 = tmp;

		W_page_PTXT_OUT.UpdateData(FALSE);
		W_page_PTXT_OUT.OnCbnSelchangeCombo5();


		GetPrivateProfileString(W_NAME,_T("p文本区界限地址"),NULL,tmp,1024,配置文件);
		W_page_PTXT_IN.m_p文本区界限地址 = tmp;
		GetPrivateProfileString(W_NAME,_T("p文本区开始地址"),NULL,tmp,1024,配置文件);
		W_page_PTXT_IN.m_p文本区开始地址 = tmp;

		W_page_PTXT_IN.UpdateData(FALSE);
	}
}
void CWQSG导出导入Dlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox
		*lb_tmp = ((CListBox*)GetDlgItem(IDC_LIST2));
	INT
		j = lb_tmp->GetCurSel();
	if(-1 == j)
	{
		MessageBox(_T("请先选择一个项目"));
		return;
	}
	if(
		(LB_ERR != lb_tmp->FindString(0,W_NAME))
		&& (j != lb_tmp->FindString(0,W_NAME))
		)
	{ 
		MessageBox(_T("文件名重复"));
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetSel(0,-1);
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetFocus();
		return ;
	}
	CString tmp;
	lb_tmp->GetText(j,tmp);
	WritePrivateProfileString(tmp,NULL,NULL,配置文件);
	lb_tmp->DeleteString(j);
	lb_tmp->SetCurSel(lb_tmp->InsertString(j,W_NAME));
	写配置();
	lb_tmp->SetFocus();	

	写列表(lb_tmp);
}
void CWQSG导出导入Dlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox
		*lb_tmp = ((CListBox*)GetDlgItem(IDC_LIST2));
	INT
		j = lb_tmp->GetCurSel();
	if(-1 == j)
	{
		MessageBox(_T("请选择一个项目"));
		return;
	}
	lb_tmp->GetText(j,W_NAME);
	WritePrivateProfileString(W_NAME,NULL,NULL,配置文件);
	W_NAME.Empty();
	lb_tmp->DeleteString(j);
	if(lb_tmp->GetCount() > j)
	{
		lb_tmp->SetCurSel(j);
	}
	else
	{
		lb_tmp->SetCurSel(--j);
	}
	OnLbnSelchangeList2();
	lb_tmp->SetFocus();
	UpdateData(FALSE);

	写列表(lb_tmp);
}
void CWQSG导出导入Dlg::OnEnKillfocusEditName()
	{
	// TODO: 在此添加控件通知处理程序代码
		////测试是否有非
	if(
		(-1 != W_NAME.Find(_T('\\')))
		|| (-1 != W_NAME.Find(_T('/')))
		|| (-1 != W_NAME.Find(_T(':')))
		|| (-1 != W_NAME.Find(_T('*')))
		|| (-1 != W_NAME.Find(_T('?')))
		|| (-1 != W_NAME.Find(_T('\"')))
		|| (-1 != W_NAME.Find(_T('<')))
		|| (-1 != W_NAME.Find(_T('>')))
		|| (-1 != W_NAME.Find(_T('|')))
		|| (-1 != W_NAME.Find(_T('[')))
		|| (-1 != W_NAME.Find(_T(']')))
		)
		{
		MessageBox(_T("文件名不能有以下符号\n \\ / : * ? \" < > | [ ]"));
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetSel(0,-1);
		((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetFocus();
		return ;
		}
	}

void CWQSG导出导入Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: 在此添加控件通知处理程序代码
	改变选项();
	*pResult = 0;
}
void CWQSG导出导入Dlg::OnBnClickedCheckPtxt(){
	// TODO: 在此添加控件通知处理程序代码
	改变选项();
}

void CWQSG导出导入Dlg::OnEnChangeEditRom()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_ROM.Left(1))
		|| (_T('　') == W_ROM.Left(1))
		)
		{
			W_ROM.TrimLeft(_T(" "));
			W_ROM.TrimLeft(_T("　"));
		}
	UpdateData(FALSE);
}

void CWQSG导出导入Dlg::OnEnChangeEditTbl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_TBL.Left(1))
		|| (_T('　') == W_TBL.Left(1))
		)
		{
			W_TBL.TrimLeft(_T(" "));
			W_TBL.TrimLeft(_T("　"));
		}
	UpdateData(FALSE);
}

void CWQSG导出导入Dlg::OnEnChangeEditTbl2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	while(
		(_T(' ') == W_TBL2.Left(1))
		|| (_T('　') == W_TBL2.Left(1))
		)
		{
			W_TBL2.TrimLeft(_T(" "));
			W_TBL2.TrimLeft(_T("　"));
		}
	UpdateData(FALSE);
}

void CWQSG导出导入Dlg::OnBnClickedButtonBat()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton*tmp = (CButton*)GetDlgItem(IDC_CHECK_PTXT);
	switch(W_TabC.GetCurSel()){
		case 0:
			if(tmp->GetCheck()){
			}
			else{
				ShowWindow(FALSE);
				导出批处理 BAT;
				BAT.DoModal();
				ShowWindow(TRUE);
			}			
			break;
		case 1:
			if(tmp->GetCheck()){
			}
			else{
			}
			break;
		default:
			;
	}
}
void CWQSG导出导入Dlg::OnBnClickedButtonStart(){
	// TODO: 在此添加控件通知处理程序代码
	CString err;

	CButton* tmp = (CButton*)GetDlgItem(IDC_CHECK_PTXT);

	UpdateData();
	m_参数表.hwnd = m_hWnd;
	m_参数表.ROM = W_ROM;
	m_参数表.TBL = W_TBL;
	m_参数表.TBL2 = W_TBL2;
	m_参数表.验证 = ((CButton*)GetDlgItem(IDC_CHECK_YZ))->GetCheck()?1:0;

	if(0 == m_参数表.ROM.GetLength()){
		err = L"ROM 地址不能为空";
		goto __gt出错退出;
	}
	if(0 == m_参数表.TBL.GetLength()){
		err = L"码表 地址不能为空";
		goto __gt出错退出;
	}
/*
	if(0 == m_参数表.TBL2.GetLength()){
		if(IDNO == MessageBox(_T("控制码表 地址为空,要继续吗?"),NULL,MB_YESNO)){
			err = L"";
			goto __gt出错退出;
		}
	}
*/
//	SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)L"Start" );
	switch(W_TabC.GetCurSel())
	{
		case 0://导出
			{
				static CWQSGFileDialog_Save fopendlg( _T("文本文件(*.TXT)|*.TXT||") , _T("TXT") , W_NAME );
				fopendlg.SetWindowTitle( _T("文本保存到...") );

				HANDLE handle;
				if( IDOK == fopendlg.DoModal() )
				{
					m_参数表.TXT = fopendlg.GetPathName();

					if(tmp->GetCheck() != 0)//指针
					{
						W_page_PTXT_OUT.UpdateData();

						if(0 == W_page_PTXT_OUT.W_pKS.GetLength())
						{
							err = L"首指针地址 不能为空";
							goto __gt出错退出;
						}
						if(0 == W_page_PTXT_OUT.W_pJS.GetLength())
						{
							err = L"最后指针地址 不能为空";
							goto __gt出错退出;
						}
						if(0 == W_page_PTXT_OUT.W_p文本基础.GetLength())
						{
							err = L"文本基础地址 不能为空";
							goto __gt出错退出;
						}

						switch( W_page_PTXT_OUT.W_p指针格式 )
						{
						case 0:
							SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)L"正在导出..." );
							handle = ::CreateThread(NULL,NULL,地址_指针导出,&m_参数表,NULL,NULL);
							::CloseHandle(handle);
							return;
						case 1:
						case 2:
							err = L"暂时不支持此模式";
							goto __gt出错退出;
							break;
						default:
							err = L"未知格式";
							goto __gt出错退出;
						}
					}
					else//非指针
					{
						W_page_TXT_OUT.UpdateData();

						if(0 == W_page_TXT_OUT.W_KS.GetLength())
						{
							err = L"开始地址 不能为空";
							goto __gt出错退出;
						}
						if(0 == W_page_TXT_OUT.W_JS.GetLength())
						{
							err = L"结束地址 不能为空";
							goto __gt出错退出;
						}

						SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)L"正在导出..." );
						handle = ::CreateThread(NULL,NULL,普通导出,&m_参数表,NULL,NULL);
						::CloseHandle(handle);
						return;
					}
				}
			}
			break;
		case 1://导入
			{
				static CWQSGFileDialog_Open fopendlg(_T("文本文件(*.TXT)|*.TXT||"),W_NAME);
				fopendlg.SetWindowTitle( _T("请选择要导入的文本...") );

				if(IDOK == fopendlg.DoModal())
				{
					m_参数表.TXT = fopendlg.GetPathName();

					HANDLE handle;
					if(tmp->GetCheck()!=0)
					{
						W_page_PTXT_IN.UpdateData();

						if(W_page_PTXT_IN.m_p文本区开始地址.GetLength() == 0){
							err = L"文本区开始地址不能为空";
							goto __gt出错退出;
						}
						if(W_page_PTXT_IN.m_p文本区界限地址.GetLength() == 0){
							err = L"文本区界限地址不能为空";
							goto __gt出错退出;
						}
						SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)L"正在导入..." );
						handle = ::CreateThread(NULL,NULL,地址_指针导入,&m_参数表,NULL,NULL);
						::CloseHandle(handle);
						return;
					}
					else
					{
						W_page_TXT_IN.UpdateData();

						SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)L"正在导入..." );
						handle = ::CreateThread(NULL,NULL,普通导入,&m_参数表,NULL,NULL);
						::CloseHandle(handle);
						return;
					}
				}
			}
			break;
		default:
			break;
	}
__gt出错退出:
	if(err.GetLength() > 0)
		MessageBox( err );
//	SendMessage( WM_WQSG_设置前端内容 , 0 , (LPARAM)0 );
}

void CWQSG导出导入Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CWQSG导出导入Dlg::OnEnKillfocusEditRom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	W_page_TXT_OUT.W_ROM地址 = W_ROM;
}

void CWQSG导出导入Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
	if(IDOK == MessageBox(_T("是否要退出?"),NULL,MB_OKCANCEL) )
	{
		CDialog::OnCancel();
	}
}

