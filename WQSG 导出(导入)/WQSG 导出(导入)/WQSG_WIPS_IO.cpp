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
// WQSG_IPS_IO.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG 导出(导入).h"
#include "WQSG_WIPS_IO.h"


// CWQSG_IPS_IO 对话框

IMPLEMENT_DYNAMIC(CWQSG_WIPS_IO, CDialog)

CWQSG_WIPS_IO::CWQSG_WIPS_IO(CWnd* pParent /*=NULL*/)
	: CDialog(CWQSG_WIPS_IO::IDD, pParent)
{

}

CWQSG_WIPS_IO::~CWQSG_WIPS_IO()
{
}

void CWQSG_WIPS_IO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWQSG_WIPS_IO, CDialog)
END_MESSAGE_MAP()


// CWQSG_IPS_IO 消息处理程序
