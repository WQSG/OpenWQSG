/*  OpenWQSG - Ps2MemoryCard
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

#include "StdAfx.h"
#include "VPscMc.h"

CVPscMc::CVPscMc(void)
: m_bOpen(false)
, m_pHead(NULL)
, m_pBuf(NULL)
{
}

CVPscMc::~CVPscMc(void)
{
}

bool CVPscMc::LoadMc( const CStringW& a_strPathName )
{
	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathName.GetString() , 1 , 3 ) )
		return false;

	n64 nSize = fp.GetFileSize();

	if( nSize != 0x00800000 && nSize != 0x00840000 )
		return false;

	m_Bufs.resize( 0x00840000 , 0 );
	m_pBuf = &m_Bufs[0];
	m_pHead = (SPs2MemoryCardHead*)m_pBuf;

	if( nSize != 0x00840000 )
	{
		u8*const pEnd = m_pBuf + 0x00840000;
		for ( u8* pBuf = m_pBuf ; pBuf < pEnd ; pBuf += 0x210 )
		{
			if( 0x200 != fp.Read( pBuf , 0x200 ) )
				return false;

			UpdateEcc( (char*)pBuf , (char*)pBuf + 0x200 );
		}
	}
	else if( nSize != fp.Read( m_pBuf , nSize ) )
		return false;

	m_bOpen = true;
	return true;
}

bool CVPscMc::SaveMc( const CStringW& a_strPathName ) const
{
	if( !isOpen() )
		return false;

	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathName.GetString() , 4 , 3 ) )
		return false;

	return ( m_Bufs.size() == fp.Write( m_pBuf , m_Bufs.size() ) );
}

bool CVPscMc::SaveMcNoEcc( const CStringW& a_strPathName ) const
{
	if( !isOpen() )
		return false;

	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathName.GetString() , 4 , 3 ) )
		return false;

	u8*const pEnd = m_pBuf + 0x00840000;
	for ( u8* pBuf = m_pBuf ; pBuf < pEnd ; pBuf += 0x210 )
	{
		if( 0x200 != fp.Write( pBuf , 0x200 ) )
			return false;
	}

	return true;
}

bool CVPscMc::Bak()
{
	if( isOpen() )
	{
		m_BakBufs = m_Bufs;
		return true;
	}
	return false;
}
bool CVPscMc::UnBak()
{
	if( isOpen() )
	{
		m_Bufs = m_BakBufs;
		return true;
	}
	return false;
}

bool CVPscMc::Import_Psu( const CString& a_strPathName )
{
	if( !isOpen() )
		return false;

	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathName.GetString() , 1 , 3 ) )
		return false;

	SPsu_header psu_head = {};
	if( sizeof(psu_head) != fp.Read( &psu_head , sizeof(psu_head) ) )
		return false;

	if( !(psu_head.attr & DF_DIRECTORY) )
		return false;

	if( !(psu_head.attr & DF_EXISTS) )
		return false;

	{
		static const SPsu_header psu_x = {};

		if( psu_head.unknown_1_u16 != 0 || psu_head.unknown_2_u64 != 0 || psu_head.EMS_used_u64 != 0 )
			return false;

		if( memcmp( psu_head.unknown_3_24_bytes , psu_x.unknown_3_24_bytes , sizeof(psu_x.unknown_3_24_bytes) ) != 0 ||
			memcmp( psu_head.unknown_4_416_bytes , psu_x.unknown_4_416_bytes , sizeof(psu_x.unknown_4_416_bytes) ) != 0)
			return false;
	}


	if( !_Vmc_Mkdir( "" , (const char*)psu_head.name , &psu_head.cTime , &psu_head.mTime , &psu_head.attr ) )
		return false;

	for( u32 i = 0 ; i < psu_head.size ; ++i )
	{
		SPsu_header head1 = {};
		if( sizeof(head1) != fp.Read( &head1 , sizeof(head1) ) )
			return false;

		{
			static const SPsu_header psu_x = {};

			if( head1.unknown_1_u16 != 0 || head1.unknown_2_u64 != 0 || head1.EMS_used_u64 != 0 )
				return false;

			if( memcmp( head1.unknown_3_24_bytes , psu_x.unknown_3_24_bytes , sizeof(psu_x.unknown_3_24_bytes) ) != 0 ||
				memcmp( head1.unknown_4_416_bytes , psu_x.unknown_4_416_bytes , sizeof(psu_x.unknown_4_416_bytes) ) != 0)
				return false;
		}

		if( head1.attr & DF_DIRECTORY )
		{
			if( head1.size == 0 )
				continue;

			return false;
		}

		if( !(head1.attr & DF_EXISTS) )
			return false;

		if( !_Vmc_WriteFile( fp , head1.size , (const char*)psu_head.name , (const char*)head1.name , &head1.cTime , &head1.mTime , &head1.attr ) )
			return false;

		const u32 p = head1.size % 0x400;

		if( p )
			fp.Seek( fp.Tell() + (0x400 - p) );
	}

	return true;
}

bool CVPscMc::Export_Psu( const CString& a_strPathName , const CStringA a_strDirName ) const
{
	if( !isOpen() )
		return false;

	SPs2DirEntry dirent_root = {};
	if( !getDirentryFromPath( dirent_root , "" , true ) )
		return false;

	SPs2DirEntry dirent_path = {};
	u32 uEntryIndex;
	if( !FindDirentryFromDirentry( dirent_path , uEntryIndex , dirent_root , a_strDirName , true ) )
		return false;

	if( !( dirent_path.mode & DF_DIRECTORY ) )
		return false;

	SPsu_header psu_head = {};

	psu_head.attr = dirent_path.mode;
	WQSG_strcpy( dirent_path.name , (char*)psu_head.name );
	psu_head.cTime = dirent_path.created;
	psu_head.mTime = dirent_path.modified;

	CWQSG_File fp;
	if( !fp.OpenFile( a_strPathName.GetString() , 4 , 3 ) )
		return false;

	if( sizeof(psu_head) != fp.Write( &psu_head , sizeof(psu_head) ) )
		return false;
	//------------------------------------------
	{
		SPsu_header psu_head0 = {};
		psu_head0.attr = 0x8427;  //Standard folder attr, for pseudo "." and ".."
		psu_head0.cTime = psu_head.cTime;
		psu_head0.mTime = psu_head.mTime;
		psu_head0.name[0] = '.';  //Set name entry to "."
		if( sizeof(psu_head0) != fp.Write( &psu_head0 , sizeof(psu_head0) ) )
			return false;
		psu_head0.name[1] = '.';  //Change name entry to ".."
		if( sizeof(psu_head0) != fp.Write( &psu_head0 , sizeof(psu_head0) ) )
			return false;
	}
	//------------------------------------------

	psu_head.size = 2;

	std::vector<SFileInfo> datas;
	if( !GetFiles( datas , a_strDirName ) )
		return false;

	std::vector<u8> pack;
	pack.resize( 0x400 , 0xFF );

	for( std::vector<SFileInfo>::iterator it = datas.begin() ;
		it != datas.end() ; ++it )
	{
		const SFileInfo& info = *it;

		if( info.bDir )
		{
			if( info.uSize != 0 )
				return false;
			//
			continue;
		}

		psu_head.size++;

		SPsu_header psu_file = {};

		//psu_file.attr = dirent_path.mode;
		WQSG_strcpy( info.szName , (char*)psu_file.name );
		//psu_file.cTime = dirent_path.created;
		//psu_file.mTime = dirent_path.modified;

		CWQSG_memFile mf;
		if( !_Vmc_ReadFile( &mf , a_strDirName , info.szName , &psu_file.cTime , &psu_file.mTime , &psu_file.attr ) )
			return false;

		psu_file.size = info.uSize;

		if( sizeof(psu_file) != fp.Write( &psu_file , sizeof(psu_file) ) )
			return false;

		ASSERT( mf.GetFileSize() == (n64)info.uSize );

		if( info.uSize != fp.Write( mf.GetBuf() , info.uSize ) )
			return false;

		const u32 p = info.uSize % 0x400;

		if( p )
		{
			if( (0x400 - p) != fp.Write( &pack[0] , (0x400 - p) ) )
				return false;
		}
	}

	//------------------------------------------
	fp.Seek(0);
	if( sizeof(psu_head) != fp.Write( &psu_head , sizeof(psu_head) ) )
		return false;
	//------------------------------------------

	return true;
}

bool CVPscMc::GetFiles( std::vector<SFileInfo>& a_Datas , const CStringA& a_strPath ) const
{
	a_Datas.clear();

	SPs2DirEntry dirent_root = {};
	if( !getDirentryFromPath( dirent_root , a_strPath , true ) )
		return false;

	if( !( dirent_root.mode & DF_DIRECTORY ) )
		return false;

	for ( u32 uDirEntIndex = 2 ; uDirEntIndex < dirent_root.length ; ++uDirEntIndex )
	{
		SPs2DirEntry dirEnt;
		if( !getDirentryFromDirentry ( dirEnt , dirent_root , uDirEntIndex ) )
			return false;

		if( dirEnt.mode & DF_EXISTS )
		{
			SFileInfo info;
			info.bDir = (dirEnt.mode & DF_DIRECTORY) != 0;
			WQSG_strcpy( dirEnt.name , info.szName );
			info.uSize = dirEnt.length;

			a_Datas.push_back( info );
		}
	}

	return true;
}
