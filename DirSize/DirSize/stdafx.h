/*  OpenWQSG - DirSize
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

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <WQSG_Lib.h>

#include <vector>
#include <map>

template< typename T >
class CAlone
{
	typedef std::map<int,T> TType;

	int m_iIndex;
	TType m_Map;

public:
	inline CAlone()
		: m_iIndex(0){}

	inline int Alloc()
	{
		const int nBase = m_iIndex;

		while ( m_Map.end() != m_Map.find(m_iIndex) )
		{
			if( ++m_iIndex < 0 )
				m_iIndex = 0;

			if( nBase == m_iIndex )
				return -1;
		}

		m_Map[m_iIndex] = T();

		return m_iIndex;
	}

	inline bool Free( int a_iIndex )
	{
		TType::iterator it = m_Map.find( a_iIndex );
		if( it == m_Map.end() )
			return false;

		m_Map.erase( it );

		return true;
	}

	inline T& Get( int a_iIndex )
	{
		TType::iterator it = m_Map.find( a_iIndex );
		ASSERT( it != m_Map.end() );
		return it->second;
	}
};

class CWQSGList : public CWnd
{
	DECLARE_DYNAMIC(CWQSGList)
public:
	enum EColumnType
	{
		E_CT_Text ,
		E_CT_Progress ,

		E_CT_MAX ,
	};

	enum ETextAlign
	{
		E_TA_LEFT ,
		E_TA_RIGHT ,
		E_TA_CENTER ,

		E_TA_MAX ,
	};
public:
	CWQSGList();

	virtual ~CWQSGList();

	BOOL Init( HWND a_hParentWnd , CRect& a_Rect , UINT a_nID );

	CImageList* SetImageList(_In_ CImageList* a_pImageList, _In_ int a_nImageList);

	int InsertColumn(_In_ int a_nCol, _In_z_ LPCTSTR a_lpszColumnHeading,
		_In_ EColumnType a_pType = E_CT_Text, _In_ int a_nWidth = -1, _In_ ETextAlign a_eAlign = E_TA_LEFT );

	int InsertItem( _In_ int a_nItem, _In_ int a_nImage = 0 );
	int GetItemCount()const{return m_iCount;}

	BOOL SetItemText(_In_ int a_nItem, _In_ int a_nSubItem, _In_z_ LPCTSTR a_lpszText);
	BOOL SetItem100(_In_ int a_nItem, _In_ int a_nSubItem, _In_z_ float a_1);

	BOOL SetImage(_In_ int a_nItem, _In_ int a_nImage );
	int GetImage(_In_ int a_nItem )const;

	BOOL SetData(_In_ int a_nItem, _In_ LPVOID a_pPtr);
	LPVOID GetData(_In_ int a_nItem )const;

	BOOL DeleteItem( _In_ int a_nItem );
	BOOL DeleteAllItems();

	int HitTest( CPoint a_pt ) const;

private:
	int Alloc( EColumnType a_eType );
	int Free( EColumnType a_eType );
	//---------------------------------------
	void ItemInsert(_In_ int a_nItem);
	void ItemDelete(_In_ int a_nItem);
	//---------------------------------------
	//void Invalidate();
	//---------------------------------------
	void DrawTitle( CDC& a_Dc , const CRect a_RectAll );
	void DrawItem( CDC& a_Dc , const CRect& a_Rect , int a_nItem );

	void DrawText( CDC& a_Dc , const CRect& a_Rect , const CStringW& a_strText , ETextAlign a_eAlign );
	void Draw100( CDC& a_Dc , CRect a_Rect , float a_Val );

	CStringW& ItemGetText(_In_ int a_nItem , _In_ int a_nSubItem);
	float& ItemGet100(_In_ int a_nItem , _In_ int a_nSubItem);

	EColumnType ColumnType(_In_ int a_nCol)const;

	void Format( CDC& a_Dc );

	BOOL GetItemRect( _In_ int a_nItem , CRect& a_rect );

	void UpdateScroll();
protected:
	// 生成的消息映射函数
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
	HWND m_hParentWnd;

	HIMAGELIST m_hImageList;
	int m_nImageList;

	CFont m_Font;
	//BOOL m_bInvalidate;
	//BOOL m_bRedraw;
	CRect m_Rect;
	int m_iTitleHeight;
	int m_iLineHeight;
	int m_iFirstLine;
	int m_iHotLine;
	BOOL m_bPaint;
	//------------------------

	struct SColumnInfo
	{
		CStringW m_strName;
		ETextAlign m_eTextAlign;
		EColumnType m_eType;
		int m_iWidth;

		int m_iIndex;
	};

	std::vector<SColumnInfo> m_Columns;

	struct SItemInfo
	{
		int m_nImage;
		LPVOID m_pPtr;
	};

	int m_iCount;
	std::vector<SItemInfo> m_ItemInfos;

	typedef std::vector<CStringW> TVTexts;
	typedef std::vector<float> TV1s;

	CAlone<TVTexts> m_Texts;
	CAlone<TV1s> m_1s;
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
