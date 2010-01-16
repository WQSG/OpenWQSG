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
// MFC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WQSG_MAIN.h"


#include <stdio.h>
#include <tchar.h>
#include <locale.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void printCmd()
{
#if 1
	printf("\n-------------------------------\n");
	printf("格式:WQSG 控制码 参数 参数 ...\n");
	printf("-----------------------------------控制码表\n");
	printf(" e 导出文本\n");
	printf(" i 导入文本\n");
	printf("-----------------------------------参数表\n");
	printf(" -tbl \"%%s\"     必选  码表路径\n");
	printf(" -tbl2 \"%%s\"    可选  控制码表路径            默认:无\n");
	printf(" -checktbl     可选  检测码表错误            默认:不检测\n");
	printf(" -rom \"%%s\"     必选  rom路径\n");
	printf(" -text \"%%s\"    必选  文本路径\n");
	printf(" -start %%x     可选  导出用     开始地址     默认:0\n");
	printf(" -end %%x       可选  导出用     结束地址     默认:FFFFFFFF\n");
	printf(" -min %%u       可选  导出用     最小界限     默认:2\n");
	printf(" -max %%u       可选  导出用     最大界限     默认:9999\n");
	printf(" -sp1 %%2X      可选  导入用     单字节填充   默认:无\n");
 	printf(" -sp2 %%4X      可选  导入用     双字节填充   默认:无\n");
 #else
// 	printf("\n-------------------------------\n");
// 	printf("格式:WQSG 控制码 参数 参数 ...\n");
// 	printf("\n-----------------------------------控制码表\n");
// 	printf(" e 导出文本\n");
// 	printf(" i 导入文本\n");
// 	printf("\n-----------------------------------参数表\n");
// 	printf(" -tbl \"%s\"     必选  码表路径\n");
// 	printf(" -tbl2 \"%s\"    可选  控制码表路径            默认:无\n");
// 	printf(" -checktbl   可选  检测码表错误            默认:不检测\n");
//  	printf(" -rom \"%s\"     必选  rom路径\n");
//  	printf(" -text \"%s\"    必选  文本路径\n");
 //	printf(" -start %x   可选  导出用     开始地址     默认:0\n");
 //	printf(" -end %x     可选  导出用     结束地址     默认:FFFFFFFF\n");
 //	printf(" -min %u     可选  导出用     最小界限     默认:2\n");
 //	printf(" -max %u     可选  导出用     最大界限     默认:9999\n");
 //	printf(" -sp1 %2X    可选  导入用     单字节填充   默认:无\n");
//	printf(" -sp2 %4X    可选  导入用     双字节填充   默认:无\n");
#endif
}

static int WQSG_Main( int nArgs , TCHAR* szArglist[] )
{
	if( nArgs < 8 )
	{
		printCmd();
		return -1;
	}

	//------------------------------公共部分
	const TCHAR* pTbl = NULL;
	const TCHAR* pTbl2 = NULL;
	BOOL bCheckTbl = FALSE;

	const TCHAR* pRom = NULL;
	const TCHAR* pTxt = NULL;
	//------------------------------导出部分
	s64 nStartPos = 0;
	s64 nEndPos = 0xFFFFFFFF;

	UINT uMin = 2;
	UINT uMax = 9999;
	//------------------------------导入部分
	const u8* pSP1 = NULL;
	const u16* pSP2 = NULL;
	u8 SP1;
	u16 SP2;

	for( int n = 2 ; n < nArgs ; ++n )
	{
		const TCHAR*const pArg =  szArglist[n];
		if( pArg[n] == _T('-') )
		{
			if( _tcsicmp( pArg , _T("-tbl") ) == 0 )
			{
				if( (n+1) < nArgs )
					pTbl = szArglist[++n];
			}
			else if( _tcsicmp( pArg , _T("-tbl2") ) == 0 )
			{
				if( (n+1) < nArgs )
					pTbl2 = szArglist[++n];
			}
			else if( _tcsicmp( pArg , _T("-checktbl") ) == 0 )
			{
				bCheckTbl = TRUE;
			}
			else if( _tcsicmp( pArg , _T("-rom") ) == 0 )
			{
				if( (n+1) < nArgs )
					pRom = szArglist[++n];
			}
			else if( _tcsicmp( pArg , _T("-text") ) == 0 )
			{
				if( (n+1) < nArgs )
					pTxt = szArglist[++n];
			}
			else if( _tcsicmp( pArg , _T("-start") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					nStartPos = 0; 
					if( 1 != _stscanf( szArglist[++n] , _T("%X") , &nStartPos ) )
					{
						return -1;
					}
				}
			}
			else if( _tcsicmp( pArg , _T("-end") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					nEndPos = 0; 
					if( 1 != _stscanf( szArglist[++n] , _T("%X") , &nEndPos ) )
					{
						return -1;
					}
				}
			}
			else if( _tcsicmp( pArg , _T("-min") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					uMin = 0; 
					if( 1 != _stscanf( szArglist[++n] , _T("%u") , &uMin ) )
					{
						return -1;
					}
				}
			}
			else if( _tcsicmp( pArg , _T("-max") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					uMax = 0; 
					if( 1 != _stscanf( szArglist[++n] , _T("%u") , &uMax ) )
					{
						return -1;
					}
				}
			}
			else if( _tcsicmp( pArg , _T("-sp1") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					int val = 0;
					if( 1 != _stscanf( szArglist[++n] , _T("%X") , &val ) )
					{
						return -1;
					}
					if( val < 0 || val > 0xFF )
					{
						return -1;
					}
					SP1 = (u8)val;
					pSP1 = &SP1;
				}
			}
			else if( _tcsicmp( pArg , _T("-sp2") ) == 0 )
			{
				if( (n+1) < nArgs )
				{
					int val = 0;
					if( 1 != _stscanf( szArglist[++n] , _T("%X") , &val ) )
					{
						return -1;
					}
					if( val < 0 || val > 0xFFFF )
					{
						return -1;
					}
					SP2 = (u16)val;
					pSP2 = &SP2;
				}
			}
		}
	}

	if( _tcsicmp( szArglist[1] , _T("I") ) == 0 )
	{
		printf( "导入模式\n" );

		static WQSG_TXT_I WQSG;
		if( !WQSG.LoadTbl( pTbl , pTbl2 , bCheckTbl ) )
			return -1;

		if( !WQSG.导入文本( pRom , pTxt , pSP1 , pSP2 ) )
		{
			return -1;
		}
	}
	else if( _tcsicmp( szArglist[1] , _T("E") ) == 0 )
	{
		printf( "导出模式\n" );

		static CWQSG_TXT_O WQSG;
		if( !WQSG.LoadTbl( pTbl , pTbl2 , bCheckTbl ) )
			return -1;

		if( !WQSG.导出文本( pRom , pTxt , nStartPos , nEndPos , uMin , uMax ) )
		{
			return -1;
		}
	}
	else
	{
		printCmd();
	}

	return 0;
}

#if 1

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	setlocale( 0 , "" );
	int nRetCode = 0;

	if( argc > 1 )
	{
		nRetCode = WQSG_Main( argc , argv );
	}
	else
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
			FreeConsole();

			//////////////////////////////////////////
			TCHAR exeFullPath[MAX_PATH]; 
			GetModuleFileName(NULL,exeFullPath,MAX_PATH);
			CString 当前目录 = exeFullPath;
			当前目录 = 当前目录.Left(当前目录.ReverseFind(_T('\\')));
			当前目录.Replace(_T('\\'),_T('?'));	
			const HANDLE WQSG_Mutex = ::CreateMutex(NULL,TRUE,当前目录);

			if(WQSG_Mutex)
			{
				if(ERROR_ALREADY_EXISTS == ::GetLastError())
				{
					AfxMessageBox(_T("同一个目录下,不能同时运行两个程序"));
					::ReleaseMutex(WQSG_Mutex);
					return FALSE;
				}
			}
			else
			{
				DWORD II = ::GetLastError();
				AfxMessageBox(_T("生成互斥失败"));
				return FALSE;
			}
			/////////////////////////////////////////
			INITCOMMONCONTROLSEX InitCtrls;
			InitCtrls.dwSize = sizeof(InitCtrls);
			// 将它设置为包括所有要在应用程序中使用的
			// 公共控件类。
			InitCtrls.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&InitCtrls);
			//	AfxEnableControlContainer();

			// 标准初始化
			// 如果未使用这些功能并希望减小
			// 最终可执行文件的大小，则应移除下列
			// 不需要的特定初始化例程
			// 更改用于存储设置的注册表项
			// TODO: 应适当修改该字符串，
			// 例如修改为公司或组织名

			//	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

			CWQSG_MAIN dlg;
			INT_PTR nResponse = dlg.DoModal();
			if (nResponse == IDOK)
			{
				// TODO: 在此处放置处理何时用“确定”来关闭
				//  对话框的代码
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: 在此放置处理何时用“取消”来关闭
				//  对话框的代码
			}
			ReleaseMutex(WQSG_Mutex);
		}
	}

	return nRetCode;
}
#else

// CMFCApp
BEGIN_MESSAGE_MAP(CWQSG导出导入App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApp 构造
CWQSG导出导入App::CWQSG导出导入App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMFCApp 对象

CWQSG导出导入App theApp;


// CMFCApp 初始化


BOOL CWQSG导出导入App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	m_bCmdMode = FALSE;
	m_CmdR = 0;

	int nArgs = 0;
	LPWSTR* szArglist = CommandLineToArgvW( GetCommandLineW() , &nArgs );
	if( szArglist )
	{
		if( nArgs > 1 )
		{
			m_bCmdMode = TRUE;


			FILE* fp_out;
			FILE* fp_in;
			FILE* fp_err;
			//AllocConsole();

			AttachConsole( ATTACH_PARENT_PROCESS );
			freopen_s( &fp_out , "CONOUT$" , "w+t", stdout );
			freopen_s( &fp_in , "CONIN$" , "r+t" , stdin );
			freopen_s( &fp_err , "CONERR$" , "w+t" , stderr );


			m_CmdR = WQSG_Main( szArglist , nArgs );


			fclose(fp_out);
			fclose(fp_in);
			fclose(fp_err);
 			FreeConsole();
		}
		else
		{
			//////////////////////////////////////////
			TCHAR exeFullPath[MAX_PATH]; 
			GetModuleFileName(NULL,exeFullPath,MAX_PATH);
			CString 当前目录 = exeFullPath;
			当前目录 = 当前目录.Left(当前目录.ReverseFind(_T('\\')));
			当前目录.Replace(_T('\\'),_T('?'));	
			const HANDLE WQSG_Mutex = ::CreateMutex(NULL,TRUE,当前目录);

			if(WQSG_Mutex)
			{
				if(ERROR_ALREADY_EXISTS == ::GetLastError())
				{
					AfxMessageBox(_T("同一个目录下,不能同时运行两个程序"));
					::ReleaseMutex(WQSG_Mutex);
					return FALSE;
				}
			}
			else
			{
				DWORD II = ::GetLastError();
				AfxMessageBox(_T("生成互斥失败"));
				return FALSE;
			}
			/////////////////////////////////////////
			INITCOMMONCONTROLSEX InitCtrls;
			InitCtrls.dwSize = sizeof(InitCtrls);
			// 将它设置为包括所有要在应用程序中使用的
			// 公共控件类。
			InitCtrls.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&InitCtrls);

			CWinApp::InitInstance();

			//	AfxEnableControlContainer();

			// 标准初始化
			// 如果未使用这些功能并希望减小
			// 最终可执行文件的大小，则应移除下列
			// 不需要的特定初始化例程
			// 更改用于存储设置的注册表项
			// TODO: 应适当修改该字符串，
			// 例如修改为公司或组织名

			//	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

			CWQSG_MAIN dlg;
			m_pMainWnd = &dlg;
			INT_PTR nResponse = dlg.DoModal();
			if (nResponse == IDOK)
			{
				// TODO: 在此处放置处理何时用“确定”来关闭
				//  对话框的代码
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: 在此放置处理何时用“取消”来关闭
				//  对话框的代码
			}
			ReleaseMutex(WQSG_Mutex);
		}
		LocalFree( szArglist );
	}
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CWQSG导出导入App::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	int rt = CWinApp::ExitInstance();

	if( m_bCmdMode )
		rt = m_CmdR;

	return rt;
}

#endif

