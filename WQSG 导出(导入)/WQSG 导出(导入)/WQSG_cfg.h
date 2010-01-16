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
#ifndef __WQSG_CFG_H__
#define __WQSG_CFG_H__

#define	DEF_ON_TXTIO		1
#define	DEF_ON_PTXTIO		0
#define	DEF_ON_WIPS			1
#define	DEF_ON_TBL			1
#define DEF_ON_文本替换		1
#define DEF_ON_差值搜索		1


enum
{
	WT_ID_MIN,
#if DEF_ON_TXTIO
	WT_ID_TXTIO,
#endif
#if DEF_ON_PTXTIO
	WT_ID_PTXTIO,
#endif
#if DEF_ON_WIPS
	WT_ID_WIPS,
#endif
#if DEF_ON_TBL
	WT_ID_TBL,
#endif
#if DEF_ON_文本替换
	WT_ID_文本替换,
#endif
#if DEF_ON_差值搜索
	WT_ID_差值搜索,
#endif

	WT_ID_ABOUT,

	WT_ID_MAX,
};

#endif
