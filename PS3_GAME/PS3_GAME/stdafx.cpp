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
// stdafx.cpp : source file that includes just the standard includes
//	PS3_GAME.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)


struct SSfoPsfHead
{
	const static u32 ms_uMagic = u32('FSP\0');
	u32 m_uMagic;
	u32 m_VerX;
	n32 m_nNameBaseOffset;
	n32 m_nDateBaseOffset;
	n32 m_nCount;
};

struct SSfoPsfItem
{
	n16 m_nNameOffset;
	n16 m_nX1;
	n32 m_nDateLen;
	n32 m_nDateSize;
	n32 m_nDataOffset;
};


inline bool CheckDirX( CString a_strPath )
{
	if( a_strPath.Right(1) != L'\\' && a_strPath.Right(1) != L'/' )
		a_strPath += L'\\';

	WIN32_FIND_DATA data;
	const HANDLE hFind = FindFirstFile( a_strPath + L"*" , &data );
	if( hFind == INVALID_HANDLE_VALUE )
		return false;

	do 
	{
		const CString strName = data.cFileName;

		if( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( strName == L"." || strName == L".." )
				continue;

			if( !CheckDirX( a_strPath + strName ) )
			{
				FindClose(hFind);
				return false;
			}
		}
		else
		{
			if( data.nFileSizeHigh )
			{
				FindClose(hFind);
				return false;
			}
		}
	}
	while ( FindNextFile( hFind , &data ) );
	FindClose(hFind);

	return true;
}
// CPs3_GameDlg ¶Ô»°¿ò
BOOL CPs3GameInfo::LoadInfo( CString a_strPath , const CString& a_strDirName )
{
	ATLASSERT( sizeof(SSfoPsfHead) == 20 );

	m_bCheckBigFile = FALSE;

	if( a_strPath.Right(1) != L'\\' && a_strPath.Right(1) != L'/' )
		a_strPath += L'\\';

	m_strPath = a_strPath;
	m_strDirName = a_strDirName;
	do
	{
		CWQSG_File fp;
		if( !fp.OpenFile( a_strPath + a_strDirName + L"\\PS3_GAME\\PARAM.SFO" , 1 , 3 ) )
			break;

		SSfoPsfHead head;
		if( sizeof(head) != fp.Read( &head , sizeof(head) ) )
			break;

		if( head.ms_uMagic != head.m_uMagic )
			break;

		if( head.m_nNameBaseOffset < sizeof(head) )
			break;

		if( head.m_nDateBaseOffset < sizeof(head) )
			break;

		const n32 nNameSize = head.m_nDateBaseOffset - head.m_nNameBaseOffset;
		if( nNameSize < 1 )
			break;

		if( head.m_nCount <= 0 || head.m_nCount > 50 )
			break;

		std::vector< u8 > names;
		names.resize( nNameSize );
		const char*const pNames = (char*)&names[0];

		fp.Seek( head.m_nNameBaseOffset );

		if( nNameSize != fp.Read( &names[0] , nNameSize ) )
			break;

		if( pNames[nNameSize-1] != L'\0' )
			break;

		std::vector< SSfoPsfItem > items;
		items.resize( head.m_nCount );

		fp.Seek( sizeof(SSfoPsfHead) );
		if( (head.m_nCount * sizeof(SSfoPsfItem)) != fp.Read( &items[0] , head.m_nCount * sizeof(SSfoPsfItem) ) )
			break;

		for( std::vector< SSfoPsfItem >::iterator it = items.begin() ;
			it != items.end() ; ++it )
		{
			const SSfoPsfItem& item = *it;

			if( item.m_nNameOffset < 0 || item.m_nNameOffset >= nNameSize )
				continue;

			if( item.m_nDataOffset < 0 || item.m_nDateSize < item.m_nDateLen )
				continue;

			const char*const pName = pNames + item.m_nNameOffset;

			if( WQSG_strcmp( pName , "TITLE" ) == 0 )
			{
				std::vector< u8 > data;
				data.resize( item.m_nDateSize );
	
				fp.Seek( head.m_nDateBaseOffset + item.m_nDataOffset );

				if( item.m_nDateSize != fp.Read( &data[0] , item.m_nDateSize ) )
					continue;

				WCHAR* pBuf = WQSG_UTF8_W( (char*)&data[0] );
				m_strGameName = pBuf;
				delete[]pBuf;
			}
			//else if( WQSG_strcmp( pName , "TITLE" ) == 0 )
			//{
			//}
		}
	}
	while(false);

	CxImage* pImage = new CxImage;
	if( !pImage->Load( a_strPath + a_strDirName + L"\\PS3_GAME\\ICON0.PNG" , CXIMAGE_FORMAT_PNG ) )
	{
		delete pImage;
		pImage = NULL;
	}

	m_pLogo = pImage ;
	return TRUE;
}

void CPs3GameInfo::CheckBigFile()
{
	m_bHasBigFile = !CheckDirX( m_strPath + m_strDirName );
	m_bCheckBigFile = TRUE;
}
