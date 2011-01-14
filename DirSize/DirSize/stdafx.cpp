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

// stdafx.cpp : 只包括标准包含文件的源文件
// DirSize.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <Imm.h>

IMPLEMENT_DYNAMIC(CWQSGList,CWnd)

CWQSGList::CWQSGList()
: m_hImageList(NULL)
, m_hParentWnd(NULL)
, m_iTitleHeight(0)
, m_iLineHeight(0)
, m_iCount(0)
, m_iFirstLine(-1)
, m_iHotLine(-1)
, m_bPaint(FALSE)
{
}

CWQSGList::~CWQSGList()
{
}

BEGIN_MESSAGE_MAP(CWQSGList, CWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BOOL CWQSGList::Init( HWND a_hParentWnd , CRect& a_Rect , UINT a_nID )
{
	ASSERT( m_hWnd == NULL && m_hParentWnd == NULL );

	CWnd*const pParentWnd = CWnd::FromHandle( a_hParentWnd );
	if( NULL == pParentWnd )
		return FALSE;

	//HIMC f = ImmAssociateContext( m_hWnd , 0 );

	const DWORD dwDefaultStyle = DS_SETFONT | DS_FIXEDSYS | WS_CHILD | WS_TABSTOP;

	if( !Create( NULL , L"" , dwDefaultStyle , a_Rect , pParentWnd , a_nID ) )
		return FALSE;

	LOGFONT logFont = {};
	pParentWnd->GetFont()->GetLogFont( &logFont );

//	m_bRedraw = TRUE;
//	m_bInvalidate = TRUE;

	m_Font.DeleteObject();
	if( !m_Font.CreateFontIndirect( &logFont ) )
		goto __gtInitErr;

	{
		CClientDC dc( NULL );

		CFont*const pFontOld = dc.SelectObject( &m_Font );
		Format( dc );
		dc.SelectObject( pFontOld );
	}

	ShowWindow( SW_SHOW );
	UpdateData();

	GetClientRect( &m_Rect );
	m_hParentWnd = a_hParentWnd;
	return TRUE;
__gtInitErr:
	if( m_hWnd )
		DestroyWindow();

	if( a_hParentWnd )
		a_hParentWnd = NULL;

	return FALSE;
}

CImageList* CWQSGList::SetImageList(_In_ CImageList* a_pImageList, _In_ int a_nImageList)
{
	const HIMAGELIST hOld = m_hImageList;
	m_hImageList = a_pImageList?a_pImageList->GetSafeHandle():NULL;
	m_nImageList = a_nImageList;

	Invalidate();

	return CImageList::FromHandlePermanent( hOld );
}

int CWQSGList::InsertColumn(_In_ int a_nCol, _In_z_ LPCTSTR a_lpszColumnHeading,
							_In_ EColumnType a_pType /*= E_CT_Text*/, _In_ int a_nWidth /*= -1*/, ETextAlign a_eAlign /*= E_TA_LEFT*/ )
{
	const int iIndex = (int)m_Columns.size();
	if( iIndex < 0 )
		return -1;

	SColumnInfo info;
	info.m_iIndex = Alloc( a_pType );
	if( info.m_iIndex < 0 )
		return -1;

	info.m_eType = a_pType;
	info.m_iWidth = a_nWidth;
	info.m_eTextAlign = a_eAlign;
	info.m_strName = a_lpszColumnHeading;

	m_Columns.push_back( info );

	if( m_iFirstLine == -1 )
		m_iFirstLine = 0;

	Invalidate();

	return iIndex;
}

int CWQSGList::InsertItem( _In_ int a_nItem, _In_ int a_nImage )
{
	ASSERT(m_iCount >= 0);
	if( m_Columns.size() == 0 || (m_iCount+1) < 0 )
		return -1;

	if( a_nItem > m_iCount )
		a_nItem = m_iCount;

	ItemInsert( a_nItem );
	SetImage( a_nItem , a_nImage );

	//Invalidate();

	return a_nItem;
}

BOOL CWQSGList::SetItemText(_In_ int a_nItem, _In_ int a_nSubItem, _In_z_ LPCTSTR a_lpszText)
{
	if( a_nItem < 0 || a_nItem >= m_iCount ||
		a_nSubItem < 0 || m_Columns.size() <= size_t(a_nSubItem) ||
		ColumnType(a_nSubItem) != E_CT_Text )
		return FALSE;

	ItemGetText( a_nItem , a_nSubItem ) = a_lpszText;

	Invalidate();

	return TRUE;
}

BOOL CWQSGList::SetItem100(_In_ int a_nItem, _In_ int a_nSubItem, _In_z_ float a_1)
{
	if( a_nItem < 0 || a_nItem >= m_iCount ||
		a_nSubItem < 0 || m_Columns.size() <= size_t(a_nSubItem) ||
		ColumnType(a_nSubItem) != E_CT_Progress )
		return FALSE;

	ItemGet100( a_nItem , a_nSubItem ) = a_1;

	Invalidate();

	return TRUE;
}

BOOL CWQSGList::SetImage(_In_ int a_nItem, _In_ int a_nImage )
{
	ASSERT( m_ItemInfos.size() == (size_t)m_iCount );
	if( a_nItem < 0 || a_nItem >= m_iCount )
		return FALSE;

	m_ItemInfos[a_nItem].m_nImage = a_nImage;
	Invalidate();
	return TRUE;
}

int CWQSGList::GetImage(_In_ int a_nItem )const
{
	ASSERT( m_ItemInfos.size() == (size_t)m_iCount );
	if( a_nItem < 0 || a_nItem >= m_iCount )
		return 0;

	return m_ItemInfos[a_nItem].m_nImage;
}

BOOL CWQSGList::SetData(_In_ int a_nItem, _In_ LPVOID a_pPtr)
{
	ASSERT( m_ItemInfos.size() == (size_t)m_iCount );
	if( a_nItem < 0 || a_nItem >= m_iCount )
		return FALSE;

	m_ItemInfos[a_nItem].m_pPtr = a_pPtr;

	return TRUE;
}

LPVOID CWQSGList::GetData(_In_ int a_nItem )const
{
	ASSERT( m_ItemInfos.size() == (size_t)m_iCount );
	if( a_nItem < 0 || a_nItem >= m_iCount )
		return NULL;

	return m_ItemInfos[a_nItem].m_pPtr;
}

BOOL CWQSGList::DeleteItem( _In_ int a_nItem )
{
	if( a_nItem < 0 || a_nItem >= m_iCount || m_Columns.size() == 0 )
		return FALSE;

	ItemDelete( a_nItem );

	Invalidate();

	if( m_iFirstLine >= m_iCount )
		m_iFirstLine = m_iCount - 1;

	if( m_iHotLine == a_nItem )
		m_iHotLine = -1;
	else if( m_iHotLine > a_nItem )
		m_iHotLine--;

	return TRUE;
}

BOOL CWQSGList::DeleteAllItems()
{
	for( int count = m_iCount ; count ; count-- )
		ItemDelete( 0 );

	return TRUE;
}

int CWQSGList::HitTest( CPoint a_pt ) const
{
	if( !m_Rect.PtInRect( a_pt ) )
		return -1;

	a_pt.x -= m_Rect.left;
	a_pt.y -= m_Rect.top;

	if( a_pt.y < m_iTitleHeight )
		return -1;

	a_pt.y -= (m_iTitleHeight+1);

	const int nItem = a_pt.y / (m_iLineHeight+1) + m_iFirstLine;
	if( nItem >= m_iCount )
		return -1;

	return nItem;
}

int CWQSGList::Alloc( enum EColumnType a_eType )
{
	int iRt = -1;
	switch ( a_eType )
	{
	case E_CT_Text:
		iRt = m_Texts.Alloc();
		break;
	case E_CT_Progress:
		iRt = m_1s.Alloc();
		break;
	default:
		ASSERT(0);
	}

	return iRt;
}

void CWQSGList::ItemInsert(_In_ int a_nItem)
{
	ASSERT( a_nItem >= 0 && a_nItem <= m_iCount );

	for ( std::vector<SColumnInfo>::iterator it = m_Columns.begin() ;
		it != m_Columns.end() ; ++it )
	{
		const SColumnInfo& col = *it;

		switch ( col.m_eType )
		{
		case E_CT_Text:
			{
				TVTexts& item = m_Texts.Get( col.m_iIndex );
				ASSERT( item.size() == m_iCount );

				item.insert( item.begin() + a_nItem , L"" );
			}
			break;
		case E_CT_Progress:
			{
				TV1s& item = m_1s.Get( col.m_iIndex );
				ASSERT( item.size() == m_iCount );

				item.insert( item.begin() + a_nItem , 0.0f );
			}
			break;
		default:
			ASSERT(0);
		}
	}

	ASSERT( m_ItemInfos.size() == m_iCount );
	static SItemInfo itenInfo = {};
	m_ItemInfos.insert( m_ItemInfos.begin() + a_nItem , itenInfo );

	m_iCount++;
}

void CWQSGList::ItemDelete(_In_ int a_nItem)
{
	ASSERT( a_nItem >= 0 && a_nItem < m_iCount );

	for ( std::vector<SColumnInfo>::iterator it = m_Columns.begin() ;
		it != m_Columns.end() ; ++it )
	{
		const SColumnInfo& col = *it;

		switch ( col.m_eType )
		{
		case E_CT_Text:
			{
				TVTexts& item = m_Texts.Get( col.m_iIndex );
				ASSERT( item.size() == m_iCount );

				item.erase( item.begin() + a_nItem );
			}
			break;
		case E_CT_Progress:
			{
				TV1s& item = m_1s.Get( col.m_iIndex );
				ASSERT( item.size() == m_iCount );

				item.erase( item.begin() + a_nItem );
			}
			break;
		default:
			ASSERT(0);
		}
	}

	ASSERT( m_ItemInfos.size() == m_iCount );
	m_ItemInfos.erase( m_ItemInfos.begin() + a_nItem );

	--m_iCount;
}

// void CWQSGList::Invalidate()
// {
// 	ASSERT( !m_bPaint );
// 
// 	m_bInvalidate = TRUE;
// 
// 	if( m_bRedraw )
// 	{
// 		CWnd* pWnd = CWnd::FromHandlePermanent(m_hParentWnd);
// 		if( pWnd )
// 			pWnd->InvalidateRect( &m_Rect , FALSE );
// 	}
// }


COLORREF g_clBg = RGB( 255 , 255 , 255 );

COLORREF g_clTitleBg = RGB( 128 , 128 , 128 );
COLORREF g_clSelItem = RGB( 200 , 200 , 200 );

COLORREF g_cl100A = RGB( 255 , 128 , 128 );
COLORREF g_cl100B = RGB( 200 , 200 , 255 );

void CWQSGList::DrawTitle( CDC& a_Dc , const CRect a_RectAll )
{
	if( a_RectAll.bottom < a_RectAll.top )
		return;

	if( a_RectAll.bottom < a_RectAll.left )
		return;

	CBrush brushBg( g_clTitleBg );

	CRect rectX( a_RectAll );
	a_Dc.FillRect( &rectX , &brushBg );

	{
		CPen pen( BS_SOLID , 1 , RGB(255,0,0) );
		CPen* pPen = a_Dc.SelectObject( &pen );

		a_Dc.MoveTo( a_RectAll.left , a_RectAll.bottom );
		a_Dc.LineTo( a_RectAll.right , a_RectAll.bottom );
		a_Dc.SelectObject( pPen );
	}

	rectX.top += 2;
	rectX.bottom -= 2;
	if( rectX.top >= rectX.bottom )
		return;

	CRect rect( rectX );

	for ( std::vector<SColumnInfo>::iterator it = m_Columns.begin() ;
		it != m_Columns.end() ; ++it )
	{
		const SColumnInfo& info = *it;
		if(info.m_iWidth <= 0)
			continue;

		rect.right = rect.left + info.m_iWidth;

		if( rect.left > rectX.right )
			break;

		if( rect.right <= rectX.right )
		{
			a_Dc.MoveTo( rect.right , a_RectAll.top );
			a_Dc.LineTo( rect.right , a_RectAll.bottom );
		}
		else
			rect.right = rectX.right;

		rect.left += 2;
		rect.right -= 2;
		DrawText( a_Dc , rect , info.m_strName , info.m_eTextAlign );
		//---------------------------------------
		rect.left = (rect.right + 2);
	}
}

void CWQSGList::DrawItem( CDC& a_Dc , const CRect& a_Rect , int a_nItem )
{
	ASSERT( a_nItem >= 0 && a_nItem < m_iCount );

	if( a_Rect.bottom < a_Rect.top )
		return;

	CRect rect( a_Rect );

	rect.top += 1;
	rect.bottom -= 1;
	if( rect.top >= rect.bottom )
		return;

	for ( std::vector<SColumnInfo>::iterator it = m_Columns.begin() ;
		it != m_Columns.end() ; ++it , rect.left = rect.right + 1 )
	{
		const SColumnInfo& col = *it;

		rect.right = rect.left + col.m_iWidth - 1;

		if( rect.left > a_Rect.right )
			break;

		if( rect.right <= a_Rect.right )
		{
			a_Dc.MoveTo( rect.right , a_Rect.top );
			a_Dc.LineTo( rect.right , a_Rect.bottom );
		}
		else
			rect.right = a_Rect.right;

		if( a_nItem == m_iHotLine )
		{
			CRect rectX( a_Rect );

			rectX.left = (it == m_Columns.begin())?rect.left:(rect.left + 1);
			rectX.right = rect.right;

			CBrush brushBg( g_clSelItem );
			a_Dc.FillRect( &rectX , &brushBg );
		}

		a_Dc.MoveTo( rect.left , a_Rect.bottom );
		a_Dc.LineTo( rect.right + 1 , a_Rect.bottom );

		CRect rectText( rect );
		rectText.left += 1;
		rectText.right -= 1;

		if( rectText.left >= rectText.right )
			continue;

		if( it == m_Columns.begin() )
		{
			CImageList* pImage = CImageList::FromHandle( m_hImageList );
			if( pImage )
			{
				IMAGEINFO info = {};
				if( pImage->GetImageInfo( GetImage(a_nItem) , &info ) )
				{
					CPoint pot( rectText.left , rectText.top );
					CSize size( min( rectText.Width() , info.rcImage.right - info.rcImage.left ) , min(rectText.Height() , info.rcImage.right - info.rcImage.left ) );

					pImage->DrawIndirect( &a_Dc , GetImage(a_nItem) , pot , size , CPoint( 0 , 0 ) );

					rectText.left += (size.cx + 1);
				}
			}
		}

		if( rectText.left >= rectText.right )
			continue;

		switch ( col.m_eType )
		{
		case E_CT_Text:
			{
				TVTexts& item = m_Texts.Get( col.m_iIndex );
				ASSERT( item.size() == (size_t)m_iCount );

				DrawText( a_Dc , rectText , item[a_nItem] , col.m_eTextAlign );
			}
			break;
		case E_CT_Progress:
			{
				TV1s& item = m_1s.Get( col.m_iIndex );
				ASSERT( item.size() == (size_t)m_iCount );

				Draw100( a_Dc , rectText , item[a_nItem] );
			}
			break;
		default:
			ASSERT(0);
		}
		//---------------------------------------
	}
}

void CWQSGList::DrawText( CDC& a_Dc , const CRect& a_Rect , const CStringW& a_strText , ETextAlign a_eAlign )
{
	UINT uFonmat = 0;

	switch (a_eAlign)
	{
	case E_TA_RIGHT:
		uFonmat = DT_RIGHT | DT_VCENTER;
		break;
	case E_TA_CENTER:
		uFonmat = DT_CENTER | DT_VCENTER;
		break;
	case E_TA_LEFT:
	default:
		uFonmat |= DT_VCENTER;
		break;
	}

	a_Dc.DrawText( a_strText , (CRect*)&a_Rect , uFonmat );
}

void CWQSGList::Draw100( CDC& a_Dc , CRect a_Rect , float a_Val )
{
	ASSERT( a_Val >= 0.0f && a_Val <= 1.0f );

	CBrush brushBg1( g_cl100A );
	CBrush brushBg2( g_cl100B );

	CRect rect( a_Rect );
	a_Dc.FillRect( &rect , &brushBg1 );
	rect.right = rect.left + int(rect.Width() * a_Val);
	a_Dc.FillRect( &rect , &brushBg2 );

	CString str;
	str.Format( L"%.2f%%" , a_Val * 100.0f );

	DrawText( a_Dc , a_Rect , str , E_TA_CENTER );
}

CStringW& CWQSGList::ItemGetText(_In_ int a_nItem , _In_ int a_nSubItem)
{
	ASSERT( a_nItem >= 0 && a_nItem < m_iCount );
	ASSERT( a_nSubItem >= 0 && (size_t)a_nSubItem < m_Columns.size() );

	const SColumnInfo& col = m_Columns[a_nSubItem];

	ASSERT( col.m_eType == E_CT_Text );

	TVTexts& item = m_Texts.Get( col.m_iIndex );

	ASSERT( (size_t)m_iCount == item.size() );

	return item[a_nItem];
}

float& CWQSGList::ItemGet100(_In_ int a_nItem , _In_ int a_nSubItem)
{
	ASSERT( a_nItem >= 0 && a_nItem < m_iCount );
	ASSERT( a_nSubItem >= 0 && (size_t)a_nSubItem < m_Columns.size() );

	const SColumnInfo& col = m_Columns[a_nSubItem];

	ASSERT( col.m_eType == E_CT_Progress );

	TV1s& item = m_1s.Get( col.m_iIndex );

	ASSERT( (size_t)m_iCount == item.size() );

	return item[a_nItem];
}

CWQSGList::EColumnType CWQSGList::ColumnType(_In_ int a_nCol)const
{
	ASSERT( a_nCol >= 0 && (size_t)a_nCol < m_Columns.size() );

	const SColumnInfo& col = m_Columns[a_nCol];

	return col.m_eType;
}

void CWQSGList::Format( CDC& a_Dc )
{
	const CSize size = a_Dc.GetTextExtent( L" " );

	m_iTitleHeight = size.cy + 3;

	CImageList* pImage = CImageList::FromHandle( m_hImageList );
	if( pImage )
	{
		IMAGEINFO info = {};
		pImage->GetImageInfo( 0 , &info );
		m_iLineHeight = max( info.rcImage.bottom - info.rcImage.top , size.cy ) + 2;
	}
	else
	{
		m_iLineHeight = size.cy + 2;
	}
}

BOOL CWQSGList::GetItemRect( _In_ int a_nItem , CRect& a_rect )
{
	if( a_nItem < 0 || a_nItem >= m_iCount ||
		a_nItem < m_iFirstLine )
		return FALSE;

	a_rect.left = m_Rect.left + 1;
	a_rect.right = m_Rect.right - 1;

	a_rect.top = m_Rect.top + 2 + m_iTitleHeight + (m_iLineHeight + 1) * a_nItem;
	a_rect.bottom = a_rect.top + m_iLineHeight;

	if( a_rect.top > m_Rect.bottom )
		return FALSE;

	if( a_rect.bottom > m_Rect.bottom )
		a_rect.bottom = m_Rect.bottom;

	return TRUE;
}

void CWQSGList::OnPaint()
{
//	if( !m_bRedraw /*|| !m_bInvalidate*/ )
//		return;

//	m_bInvalidate = FALSE;
	m_bPaint = TRUE;

	CPaintDC dc(this);

	CFont*const pFontOld = dc.SelectObject( &m_Font );
	const COLORREF textColorOld = dc.SetTextColor( RGB(0,0,0) );
	const int nBkMode = dc.SetBkMode( TRANSPARENT );
	CBrush brushNew( g_clBg );
	//================================
	Format( dc );
	CRect WinRect( m_Rect );
	//------------------------------
	dc.FillRect( &WinRect , &brushNew );
	dc.Draw3dRect( WinRect , RGB(0,0,0) , RGB(0,0,0) );

	{
		CRect rectTemp( WinRect );
		rectTemp.top++;
		rectTemp.bottom = rectTemp.top + m_iTitleHeight;
		if( rectTemp.bottom >= (WinRect.bottom-1) )
			rectTemp.bottom = WinRect.bottom - 2;

		rectTemp.left++;
		rectTemp.right--;

		DrawTitle( dc , rectTemp );
		WinRect.top = rectTemp.bottom + 2;
	}
	//
	for ( int iIndex = m_iFirstLine ; iIndex < m_iCount ; ++iIndex )
	{
		CRect rectTemp;
		if( !GetItemRect( iIndex , rectTemp ) )
			break;

		DrawItem( dc , rectTemp , iIndex );

		rectTemp.top = rectTemp.bottom;
	}
	//================================
	dc.SelectObject( pFontOld );
	dc.SetTextColor( textColorOld );
	dc.SetBkMode( nBkMode );

	m_bPaint = FALSE;
}

void CWQSGList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus();
	SetActiveWindow();

	if( !m_Rect.PtInRect( point ) )
		return;

	const int nItem = HitTest( point );
	if( nItem == -1 )
		return;

	if( m_iHotLine != nItem )
	{
		m_iHotLine = nItem;
		Invalidate();
	}
}

void CWQSGList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);

	if( !m_Rect.PtInRect( point ) )
		return;

	const int nItem = HitTest( point );
	if( nItem == -1 )
		return;

	NMITEMACTIVATE item = {};
	item.hdr.code = LVN_ITEMACTIVATE;
	item.hdr.idFrom = GetDlgCtrlID();
	item.hdr.hwndFrom = m_hWnd;

	item.iItem = nItem;

	::SendNotifyMessage( m_hParentWnd , WM_NOTIFY , (WPARAM)m_hWnd , (LPARAM)&item );
}
