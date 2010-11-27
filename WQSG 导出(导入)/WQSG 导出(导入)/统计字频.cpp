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
// 统计字频.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "统计字频.h"

#include <vector>
#include <list>
// C统计字频 对话框
class CChars
{
public:
	struct SCharInfo
	{
		WCHAR m_char;
		BOOL m_bUnTbl;
	};

	struct SChars
	{
		u32 m_uCount;
		std::vector<SCharInfo> m_Chars;
	};

	std::list<SChars> m_Chars;

	void AddCharS2B( u32 a_uCount , WCHAR a_Ch , BOOL a_bUnTbl )
	{
		std::list<SChars>::iterator it = m_Chars.begin();

		for( ; it != m_Chars.end() ; ++it )
		{
			SChars& data = *it;

			if( data.m_uCount == a_uCount )
			{
				SCharInfo tmpInfo;
				tmpInfo.m_char = a_Ch;
				tmpInfo.m_bUnTbl = a_bUnTbl;

				data.m_Chars.push_back( tmpInfo );
				return;
			}
			else if( data.m_uCount > a_uCount )
			{
				SCharInfo tmpInfo;
				tmpInfo.m_char = a_Ch;
				tmpInfo.m_bUnTbl = a_bUnTbl;

				SChars tmp;
				tmp.m_uCount = a_uCount;
				tmp.m_Chars.push_back( tmpInfo );
				m_Chars.insert( it , tmp );
				return;
			}
		}

		SCharInfo tmpInfo;
		tmpInfo.m_char = a_Ch;
		tmpInfo.m_bUnTbl = a_bUnTbl;

		SChars tmp;
		tmp.m_uCount = a_uCount;
		tmp.m_Chars.push_back( tmpInfo );
		m_Chars.push_back( tmp );
	}
	void AddCharB2S( u32 a_uCount , WCHAR a_Ch , BOOL a_bUnTbl )
	{
		std::list<SChars>::iterator it = m_Chars.begin();

		for( ; it != m_Chars.end() ; ++it )
		{
			SChars& data = *it;

			if( data.m_uCount == a_uCount )
			{
				SCharInfo tmpInfo;
				tmpInfo.m_char = a_Ch;
				tmpInfo.m_bUnTbl = a_bUnTbl;

				data.m_Chars.push_back( tmpInfo );
				return;
			}
			else if( data.m_uCount < a_uCount )
			{
				SCharInfo tmpInfo;
				tmpInfo.m_char = a_Ch;
				tmpInfo.m_bUnTbl = a_bUnTbl;

				SChars tmp;
				tmp.m_uCount = a_uCount;
				tmp.m_Chars.push_back( tmpInfo );
				m_Chars.insert( it , tmp );
				return;
			}
		}

		SCharInfo tmpInfo;
		tmpInfo.m_char = a_Ch;
		tmpInfo.m_bUnTbl = a_bUnTbl;

		SChars tmp;
		tmp.m_uCount = a_uCount;
		tmp.m_Chars.push_back( tmpInfo );
		m_Chars.push_back( tmp );
	}
};

void 取单字节编码(int*高位,int*低位,WCHAR*位置)
{
	::swprintf(位置,3,L"%02x",(*低位)++);
	if(*低位 > 255)
		*低位 = 0;
}
void 取双字节编码_低(int*高位,int*低位,WCHAR*位置)
{
	::swprintf(位置++,3,L"%02x",*高位);
	::swprintf(++位置,3,L"%02x",(*低位)++);
	if(*低位 > 255)
	{
		*低位 = 0;(*高位)++;
		if(*高位 > 255)
			*高位 = 0;
	}
}
void 取双字节编码_高(int*高位,int*低位,WCHAR*位置)
{
	::swprintf(位置++,3,L"%02x",(*高位)++);
	::swprintf(++位置,3,L"%02x",*低位);
	if(*高位 > 255)
	{
		*高位 = 0;
		(*低位)++;
		if(*低位 > 255)
			*低位 = 0;
	}
}
void 开始编码(::CWQSG_File * TBL_File,const WCHAR*文本,int 高位,int 低位,void(*取码表编码)(int*高位,int*低位,WCHAR*位置))
{
	TBL_File->Write("\xff\xfe",2);
	WCHAR 码表左边[5];
	CStringW 码表;
	while(*文本)
	{
		if(0x20 > *文本)
		{
			文本++;
			continue;
		}
		取码表编码(&高位,&低位,码表左边);
		码表 = 码表左边;
		码表 += L'=';
		码表 += *(文本++);
		码表 += L"\15\12";
		TBL_File->Write(码表.GetString(),(码表.GetLength()<<1));
	}
}
IMPLEMENT_DYNAMIC(C统计字频, CBaseDialog)

void C统计字频::清零()
{
	for( int i = 0 ; i < (sizeof(m_Info)/sizeof(*m_Info)) ; i++ )
		m_Info[i].Clean();
}
void C统计字频::统计一句字频_过滤控制符(const WCHAR* TXT)
{
	while(*TXT)
	{
		if(W_KZF_L == *TXT)
		{
			UINT 左括号数量 = 1,右边括号数量 = 0;
			TXT++;
			while(左括号数量 != 右边括号数量
				&& NULL != *TXT)
			{
				if(W_KZF_L == *TXT)
				{
					左括号数量++;
				}
				else if(W_KZF_R == *TXT)
				{
					右边括号数量++;
				}
				TXT++;
			}
		}
		else if(W_KZF_R == *TXT)
		{
			TXT++;
		}
		else if(0x20 > *TXT)
		{
			TXT++;
		}
		else
		{
			m_Info[*TXT].m_uCount++;

			TXT++;
		}
	}
}
void C统计字频::统计一句字频_纯文本(const WCHAR* TXT)
{
	while(*TXT)
	{
		if(0x20 > *TXT)
		{
			TXT++;
		}
		else
		{
			m_Info[*TXT].m_uCount++;

			TXT++;
		}
	}
}
BOOL C统计字频::载入码表()
{
	::CMemTextW WQSG;
	if(FALSE == WQSG.Load(W_码表地址.GetString(),(DWORD)10485760))
	{
		MessageBoxW( W_码表地址 + L"\r\n" + WQSG.GetErrTXT() );
		return FALSE;
	}

	while(const WCHAR* _line = WQSG.GetLine() )
	{
		WCHAR* tmp = ::WQSG_DelSP_L(_line);
		delete[]_line;
		if(0 == *tmp)
		{
			delete[]tmp;
			continue;
		}
		/////////////////////////////
		INT I = ::WQSG_strchr(tmp,L'=');
		if(-1 == I)
		{
			CStringW message(L"错误,找不到 =\n\n");
			message += tmp;
			delete[]tmp;
			MessageBoxW(message);
			return FALSE;
		}
		//////////////////////////////////
		_line = ::WQSG_getstrL(tmp + I + 1,-1);
		if(0 == ::WQSG_strlen(_line))
		{
			CStringW message(L"错误,码表右边只能有一个字\n\n");
			message += tmp;
			delete[]tmp;
			MessageBoxW(message);
			return FALSE;
		}
		delete[]tmp;
		////////////////////////////////
		m_Info[*_line].m_bUnTbl = FALSE;
		delete[]_line;
	}
	return TRUE;
}
void C统计字频::OnOK()
{
	// TODO: 在此添加控件通知处理程序代码
}
void C统计字频::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}

INT_PTR C统计字频::DoModal()
{
	if(0 == W_窗口类型)
	{
		W_窗口类型 = 1;
		return CBaseDialog::DoModal();
	}
	return IDCANCEL;
}
BOOL C统计字频::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	if(0 == W_窗口类型)
	{
		W_窗口类型 = 2;
		return CBaseDialog::Create(nIDTemplate,pParentWnd);
	}
	return FALSE;
}
C统计字频::C统计字频(CWnd* pParent /*=NULL*/)
	: CBaseDialog(C统计字频::IDD, pParent)
	, W_码表地址(_T(""))
	, W_显示框(_T(""))
	, W_控制符类型(0)
	, W_对照码表(FALSE)
	, W_统计模式(FALSE)
	, W_等待编码的字(_T(""))
	, W_第一个编码(_T(""))
	, W_高低交换(FALSE)
	, W_窗口类型(0)
	, W_编码表(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_WQSG);
	清零();
}

C统计字频::~C统计字频()
{
	switch(W_窗口类型)
	{
	case 1://模态
		W_窗口类型 = 0;
		CBaseDialog::OnCancel();
		break;
	case 2://非模态
		W_窗口类型 = 0;
		DestroyWindow();
		break;
	default://不存在
		break;
	}
}

void C统计字频::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TBL_TBL, W_码表地址);
	DDV_MaxChars(pDX, W_码表地址, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TBL_1, W_显示框);
	DDX_CBIndex(pDX, IDC_COMBO_TBL_1, W_控制符类型);
	DDX_Check(pDX, IDC_CHECK_TBL_TBL, W_对照码表);
	DDX_Radio(pDX, IDC_RADIO_TBL_1, W_统计模式);
	DDX_Text(pDX, IDC_EDIT_TBL_2, W_等待编码的字);
	DDX_Text(pDX, IDC_EDIT_TBL_3, W_第一个编码);
	DDV_MaxChars(pDX, W_第一个编码, 4);
	DDX_Check(pDX, IDC_CHECK_TBL_1, W_高低交换);
	DDX_Text(pDX, IDC_EDIT1, W_编码表);
	DDV_MaxChars(pDX, W_编码表, MAX_PATH);
	DDX_Control(pDX, IDC_COMBO_OUT, m_OutType);
}


BEGIN_MESSAGE_MAP(C统计字频, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_TBL_TBL, &C统计字频::OnBnClickedButtonTbl)
	ON_BN_CLICKED(IDC_BUTTON_TBL_2, &C统计字频::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK_TBL_TBL, &C统计字频::OnBnClickedCheckTbl)
	ON_EN_CHANGE(IDC_EDIT_TBL_3, &C统计字频::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK1, &C统计字频::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON_TBL_3, &C统计字频::OnBnClickedButtonTbl3)
	ON_BN_CLICKED(IDC_BUTTON3, &C统计字频::OnBnClickedButton3)
	ON_WM_LBUTTONDOWN()
	ON_EN_KILLFOCUS(IDC_EDIT_TBL_3, &C统计字频::OnEnKillfocusEditTbl3)
	ON_WM_CLOSE()
END_MESSAGE_MAP()// C统计字频 消息处理程序
BOOL C统计字频::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	while( m_OutType.GetCount() )
		m_OutType.DeleteString( 0 );
	m_OutType.AddString( L"Unicode 编码顺序 [默认]" );
	m_OutType.AddString( L"字频降序" );
	m_OutType.AddString( L"字频升序" );
	m_OutType.SetCurSel( 0 );


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

void C统计字频::OnPaint()
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
		CBaseDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR C统计字频::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C统计字频::OnBnClickedButtonTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CWQSGFileDialog_Save fopendlg( _T("码表文件(*.TBL,*.TXT)|*.TBL;*.TXT||") );
	fopendlg.SetWindowTitle( _T("选择一个码表...") );

	if(IDOK == fopendlg.DoModal())
	{
		W_码表地址 = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}
void C统计字频::OnBnClickedCheckTbl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	(GetDlgItem(IDC_EDIT_TBL_TBL))->EnableWindow( W_对照码表 );
}

void C统计字频::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	W_等待编码的字 = W_显示框 = _T("");
	UpdateData(FALSE);
	清零();
	if(W_对照码表)
	{
		if( !载入码表() )
			return;
	}

	static CWQSGFileDialog_OpenS fopendlg( _T("文本文件(*.TXT)|*.TXT||") );
	fopendlg.SetWindowTitle( _T("选择要统计的文本...") );

	if(IDOK == fopendlg.DoModal())
	{
		POSITION pos = fopendlg.GetStartPosition();
		UINT 数量 = 0;
		CString 文件;
		while(fopendlg.GetNextPathName(文件,pos))
		{
			::CMemTextW WQSG;
			if(FALSE == WQSG.Load(文件.GetString(),(DWORD)10485760))
			{
				MessageBoxW(文件 + L"\r\n" + WQSG.GetErrTXT() );
				return;
			}
			if(W_控制符类型)
			{
				W_KZF_L = L'[';W_KZF_R = L']';
			}
			else
			{
				W_KZF_L = L'{';W_KZF_R = L'}';
			}
			数量++;
			switch(W_统计模式)
			{
			case 0:
				while(const WCHAR * _line = WQSG.GetLine())
				{
					///////////////////////////////////
					WCHAR* tmp = ::WQSG_DelSP_L(_line);
					delete[]_line;
					if(0 == *tmp)
					{
						delete[]tmp;
						continue;
					}
					_line = tmp;
					////////////////////////////////
					UINT k = ::WQSG_strchr(tmp,L',');
					if(-1 == k)
					{
						文件 += L"\n错误,找不到第一个逗号,\n\n";
						文件 += _line;
						delete[]_line;
						MessageBoxW(文件);
						return;
					}
					tmp += (k + 1);
					//////////////////////////////////////////
					k = ::WQSG_strchr(tmp,L',');
					if(-1 == k)
					{
						文件 += L"\n错误,找不到第二个逗号,\n\n";
						文件 += _line;
						delete[]_line;
						MessageBoxW(文件);
						return;
					}
					tmp += (k + 1);
					///////////////////////////////////
					tmp = ::WQSG_getstrL(tmp,-1);
					delete[]_line;
					统计一句字频_过滤控制符(tmp);
					delete[]tmp;
				}
				break;
			case 1:
				while(const WCHAR* _line = WQSG.GetLine())
				{
					统计一句字频_纯文本(_line);
					delete[]_line;
				}
				break;
			case 2:
				MessageBox(_T("暂时不支持此统计模式"));
				return;
				break;
			default:
				MessageBox(_T("???"));;
			}
		}
		//////////////////////////// 统计收尾
		CStringW 未编码,字频率;
		UINT 字的个数 = 0,未编码的个数 = 0;
		switch ( m_OutType.GetCurSel() )
		{
		case 1:
		case 2:
			{
				CChars info;

				if( m_OutType.GetCurSel() == 1 )
				{
					for( int i = 0x0 ; i < (sizeof(m_Info)/sizeof(*m_Info)) ; i++ )
						if( m_Info[i].m_uCount )
							info.AddCharB2S( m_Info[i].m_uCount , (WCHAR)i , m_Info[i].m_bUnTbl );
				}
				else
				{
					for( int i = 0x0 ; i < (sizeof(m_Info)/sizeof(*m_Info)) ; i++ )
						if( m_Info[i].m_uCount )
							info.AddCharS2B( m_Info[i].m_uCount , (WCHAR)i , m_Info[i].m_bUnTbl );
				}
				//=====================================================================
				 for( std::list<CChars::SChars>::iterator it = info.m_Chars.begin() ;
					 it != info.m_Chars.end() ; it++ )
				{
					const CChars::SChars& chars = *it;
					const std::vector<CChars::SCharInfo>& charInfo = chars.m_Chars;

					for( std::vector<CChars::SCharInfo>::const_iterator itInfo = charInfo.begin() ;
						itInfo != charInfo.end() ; ++itInfo )
					{
						const CChars::SCharInfo& tmpInfo = *itInfo;

						if( tmpInfo.m_bUnTbl )
						{
							未编码 += tmpInfo.m_char;
							未编码的个数++;
						}

						字频率.AppendFormat( L"%c     %u\r\n" , tmpInfo.m_char , chars.m_uCount );
						字的个数++;
					}
				}
			}
			break;
		default:
			for( int i = 0x0 ; i < (sizeof(m_Info)/sizeof(*m_Info)) ; i++ )
			{
				if( m_Info[i].m_uCount )
				{
					if( m_Info[i].m_bUnTbl )
					{
						未编码 += (WCHAR)i;
						未编码的个数++;
					}
					字频率.AppendFormat( L"%c     %u\r\n" , (WCHAR)i , m_Info[i].m_uCount );
					字的个数++;
				}
			}
		}
		///////////////////// 输出字频
		W_等待编码的字 += 未编码;
		W_显示框.Format( L"****** 其他的统计 ******\15\12字的总数:  %u\15\12未编码的字数:  %u" , 字的个数 , 未编码的个数 );
		W_显示框 += _T("\15\12****** 出现的频率 ******\15\12");	
		W_显示框 += 字频率;
		字频率 = L"";
		UpdateData(FALSE);
		未编码.Format( _T("统计完毕\n共统计%u个文件") , 数量 );
		MessageBox( 未编码 );
	}
}

void C统计字频::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	EditCheckHaxStr(W_第一个编码,(CEdit*)GetDlgItem(IDC_EDIT_TBL_3),(CWnd*)this,0);
}

void C统计字频::OnEnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(W_第一个编码.GetLength()&1)
	{
		W_第一个编码 = _T("0") + W_第一个编码;
		UpdateData(FALSE);
		//MessageBox(_T("HEX 的位数必须为 2 的倍数"));
		//(CEdit*)GetDlgItem(IDC_EDIT_TBL_3)->SetFocus();
	}
}


void C统计字频::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL 选中 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT_TBL_3)->EnableWindow(!选中);
	GetDlgItem(IDC_CHECK_TBL_1)->EnableWindow(!选中);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(选中);
	GetDlgItem(IDC_EDIT1)->EnableWindow(选中);
}

void C统计字频::OnBnClickedButtonTbl3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	BOOL 编码表 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();

	INT LLLEN;
	if(编码表)
	{
		if(W_编码表.GetLength() == 0)
		{
			MessageBox(_T("编码表 不能为空"));
			return;
		}
	}
	else
	{
		if(0 == (LLLEN = W_第一个编码.GetLength()))
		{
			MessageBox(_T("首编码 不能为空"));
			return;
		}
	}
	///////////////////////////
	static CWQSGFileDialog_Save fopendlg( _T("码表文件(*.TBL)|*.TBL||") , _T("TBL") );
	fopendlg.SetWindowTitle( _T("码表保存到...") );
 
	if(IDOK == fopendlg.DoModal())
	{
		::CWQSG_File TBL_File;
		if(! TBL_File.OpenFile(fopendlg.GetPathName().GetString() , 4 , 3 ))
		{
			MessageBox(_T("生成码表失败"));
			return;
		}
		if(编码表)
		{
			if(!编码表编码(&TBL_File))
			{
				return;
			}
		}
		else
		{
			int 高位,低位;
			const TCHAR* s1 = W_第一个编码.GetString();
			switch(LLLEN)
			{
			case 2:
				::_stscanf_s(s1,_T("%2x"),&低位);
				高位 = 0;
				开始编码(&TBL_File,W_等待编码的字.GetString(),高位,低位,&::取单字节编码);
				break;
			case 4:
				::_stscanf_s(s1++,_T("%2x"),&高位);
				::_stscanf_s(++s1,_T("%2x"),&低位);
				if(W_高低交换)
					开始编码(&TBL_File,W_等待编码的字.GetString(),高位,低位,&::取双字节编码_高);
				else
					开始编码(&TBL_File,W_等待编码的字.GetString(),高位,低位,&::取双字节编码_低);
				break;
			default:
				MessageBox(_T("??? 2"));
				return;
			}
		}
		MessageBox(_T("完毕"));
	}
	UpdateData(FALSE);
}

void C统计字频::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	static CWQSGFileDialog_Open fopendlg( _T("编码表(*.TBL,*.TXT)|*.TBL;*.TXT||") );
	fopendlg.SetWindowTitle( _T("选择一个编码表表...") );

	if(IDOK == fopendlg.DoModal())
	{
		W_编码表 = fopendlg.GetPathName();
	}
	UpdateData(FALSE);
}

BOOL C统计字频::编码表编码(::CWQSG_File * TXT_File)
{
	UpdateData();
	if(W_编码表.GetLength() == 0)
	{
		MessageBox(_T("请先选择一个编码表."));
		return FALSE;
	}
	::CMemTextW WQSG;
	if( !WQSG.Load( W_编码表.GetString() , 33554432 ) )
	{
		MessageBox(W_编码表 + L"\r\n" + WQSG.GetErrTXT() ,L"编码表编码");
		return FALSE;
	}
	::CWQSG_MSG_W 对话框(m_hWnd,L"编码表编码");
	const WCHAR * 字符串 = W_等待编码的字.GetString();

	const WCHAR* pLine;
	WCHAR * tmp, * tmp2;
	CString 文本;
	TXT_File->Write("\377\376",2);
	while(*字符串)
	{
		if(*字符串 < 0x20)
		{
			字符串++;
			continue;
		}
		pLine = WQSG.GetLine();
		if(NULL == pLine)
		{
			文本 = *字符串;
			对话框.show( 文本.GetString() , L"编码数不够用" );
			return FALSE;
		}

		tmp = ::WQSG_DelSP_L(pLine);
		if(0 == *tmp)
		{
			delete[]pLine;
			delete[]tmp;
			continue;
		}
		UINT 位置 = ::WQSG_strchr(tmp,L'=');
		if(-1 == 位置)
		{
			delete[]tmp;
			if(IDOK != 对话框.show(pLine,L"找不到等号,此行作废,要继续吗?",MB_YESNO))
			{
				delete[]pLine;
				return FALSE;
			}
			delete[]pLine;
			continue;
		}
		tmp2 = ::WQSG_getstrL(tmp,位置);
		delete[]tmp;
		位置 = ::WQSG_strlen(tmp2);
		if( 位置 & 1//为单数
			|| 0 == 位置
			|| !::WQSG_IsHexText(tmp2)
		)
		{
			delete[]tmp2;
			if(IDOK != 对话框.show( pLine , L"等号左边不是有效的十六进制文本,此行作废,要继续吗?" , MB_YESNO) )
			{
				delete[]pLine;
				return FALSE;
			}
			delete[]pLine;
			continue;
		}
		文本 = tmp2;
		文本 += L'=';
		文本 += *(字符串++);
		文本 += L"\r\n";
		TXT_File->Write( 文本.GetString() , 文本.GetLength()*sizeof(*文本.GetString()) );
		delete[]pLine;
		delete[]tmp2;
	}
	return FALSE;
}
void C统计字频::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CBaseDialog::OnLButtonDown(nFlags, point);
}

void C统计字频::OnEnKillfocusEditTbl3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(W_第一个编码.GetLength()&1)
	{
		W_第一个编码.Insert(0,L'0');
		UpdateData(FALSE);
	}
}

void C统计字频::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseDialog::OnClose();
//	if( IDOK == MessageBox(_T("是否要退出?"),NULL,MB_OKCANCEL) )
//	{
		//ReleaseMutex(WQSG_Mutex);
		switch(W_窗口类型)
		{
		case 1://模态
			W_窗口类型 = 0;
			CBaseDialog::OnCancel();
			break;
		case 2://非模态
			W_窗口类型 = 0;
			DestroyWindow();
			break;
		default://不存在
			break;
		}
//	}
}
