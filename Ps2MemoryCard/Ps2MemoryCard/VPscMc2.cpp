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
//----------------------------------------------------------------------------
// XOR table use for Error Correcting Code ( ECC ) calculation.
//----------------------------------------------------------------------------
const unsigned char ECC_XOR_Table[ 256 ]= {
	0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4, 
	0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00, 
	0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77, 
	0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3, 
	0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66, 
	0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2, 
	0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5, 
	0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11, 
	0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55, 
	0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1, 
	0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96, 
	0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22, 
	0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87, 
	0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33, 
	0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44, 
	0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0, 
	0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44, 
	0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0, 
	0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87, 
	0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33, 
	0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96, 
	0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22, 
	0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55, 
	0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1, 
	0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5, 
	0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11, 
	0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66, 
	0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2, 
	0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77, 
	0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3, 
	0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4, 
	0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00, 
};


inline static void calculateECC ( char* ECC_Chunk, const unsigned char* Data_Chunk ) 
{
	int i, c;

	ECC_Chunk[ 0 ]= ECC_Chunk[ 1 ]= ECC_Chunk[ 2 ]= 0;

	for ( i = 0; i < 0x80; i++ ) 
	{

		c = ECC_XOR_Table[ Data_Chunk[ i ] ];

		ECC_Chunk[ 0 ] ^= c;

		if ( c & 0x80 ) 
		{

			ECC_Chunk[ 1 ] ^= ~i;
			ECC_Chunk[ 2 ] ^= i;

		}

	}

	ECC_Chunk[ 0 ] = ~ECC_Chunk[ 0 ];
	ECC_Chunk[ 0 ] &= 0x77;
	ECC_Chunk[ 1 ] = ~ECC_Chunk[ 1 ];
	ECC_Chunk[ 1 ] &= 0x7f;
	ECC_Chunk[ 2 ] = ~ECC_Chunk[ 2 ];
	ECC_Chunk[ 2 ] &= 0x7f;
}
//----------------------------------------------------------------------------
// Build ECC from a complet page of data
//----------------------------------------------------------------------------
inline static void buildECC ( char* Page_Data, char* ECC_Data ) 
{

	u8 Data_Chunk[ 4 ][ 128 ];
	char ECC_Chunk[ 4 ][ 3 ];
	char ECC_Pad[ 4 ];

	// This is to divide the page in 128 bytes chunks
	memcpy ( Data_Chunk[ 0 ], Page_Data +   0, 128 );
	memcpy ( Data_Chunk[ 1 ], Page_Data + 128, 128 );
	memcpy ( Data_Chunk[ 2 ], Page_Data + 256, 128 );
	memcpy ( Data_Chunk[ 3 ], Page_Data + 384, 128 );

	// Ask for 128 bytes chunk ECC calculation, it returns 3 bytes per chunk
	calculateECC ( ECC_Chunk[ 0 ], Data_Chunk[ 0 ]);
	calculateECC ( ECC_Chunk[ 1 ], Data_Chunk[ 1 ]);
	calculateECC ( ECC_Chunk[ 2 ], Data_Chunk[ 2 ]);
	calculateECC ( ECC_Chunk[ 3 ], Data_Chunk[ 3 ]);

	// Prepare Padding as ECC took only 12 bytes and stand on 16 bytes
	memset ( ECC_Pad , 0 , sizeof ( ECC_Pad )  );

	// "MemCopy" our four 3 bytes ECC chunks into our 16 bytes ECC data buffer
	// Finaly "MemCopy" our 4 bytes PAD chunks into last 4 bytes of our ECC data buffer
	memcpy ( ECC_Data + 0, ECC_Chunk[ 0 ], 3 );
	memcpy ( ECC_Data + 3, ECC_Chunk[ 1 ], 3 );
	memcpy ( ECC_Data + 6, ECC_Chunk[ 2 ], 3 );
	memcpy ( ECC_Data + 9, ECC_Chunk[ 3 ], 3 );
	memcpy ( ECC_Data + 12, ECC_Pad      , 4 );
}

inline static void getPs2Time( SPs2DateTime* a_pModified )
{
	memset( a_pModified , 0 , sizeof(SPs2DateTime) );

	SYSTEMTIME timeUtc = {};
	GetSystemTime( &timeUtc );

	TIME_ZONE_INFORMATION DEFAULT_TIME_ZONE_INFORMATION = {-540};

	SYSTEMTIME time = {};
	SystemTimeToTzSpecificLocalTime( &DEFAULT_TIME_ZONE_INFORMATION , &timeUtc , &time);  

	a_pModified->year = time.wYear;
	a_pModified->month = (u8)time.wMonth;
	a_pModified->day = (u8)time.wDay;
	a_pModified->hour = (u8)time.wHour;
	a_pModified->min = (u8)time.wMinute;
	a_pModified->sec = (u8)time.wSecond;
}

//----------------------------------------------------------------------------
void CVPscMc::UpdateEcc( char* Page_Data, char* ECC_Data )
{
	buildECC( Page_Data , ECC_Data );
}

bool CVPscMc::ReadPage( void* a_OutBuf , n32 a_nPageIndex )
{
	size_t position = a_nPageIndex * (m_pHead->page_size + 0x10);

	memcpy( a_OutBuf , m_pBuf + position , m_pHead->page_size );

	return true;
}

bool CVPscMc::WritePage( const void* a_OutBuf , n32 a_nPageIndex )
{
	size_t position = a_nPageIndex * (m_pHead->page_size + 0x10);

	memcpy( m_pBuf + position , a_OutBuf , m_pHead->page_size );

	buildECC( (char*)m_pBuf + position , (char*)m_pBuf + position + m_pHead->page_size );

	return true;
}

bool CVPscMc::readCluster( void* a_pOutCluster , u32 a_uClusterIndex ) 
{
	u32 uPage_Index = a_uClusterIndex * m_pHead->pages_per_cluster;

	for( int i = 0 ; i < m_pHead->pages_per_cluster ; i++ ) 
	{
		if( !ReadPage ( (u8*)a_pOutCluster + ( i * m_pHead->page_size ) , uPage_Index + i ) )
			return false;
	}

	return true;
}

bool CVPscMc::writeCluster( const void* a_pOutCluster, u32 a_uClusterIndex )
{
	u32 uPage_Index = a_uClusterIndex * m_pHead->pages_per_cluster;

	for( int i = 0 ; i < m_pHead->pages_per_cluster ; i++ ) 
	{
		if( !WritePage( (u8*)a_pOutCluster + ( i * m_pHead->page_size ) , uPage_Index + i ) )
			return false;
	}

	return true;
}
//-----------------------------------------------------------------
bool CVPscMc::getFatEntry ( u32& a_uNextCluster , u32 a_uCluster )
{
	const u32 uMod = m_pHead->cluster_size / sizeof(u32);
	// The fat index, aka which cluster we want to know about const u32& fat_index = a_uCluster;

	// Which fat cluster that the information would be in
	const u32 uFatHigh_Tmp = a_uCluster / uMod;

	const u32 uFatHigh_Index = uFatHigh_Tmp / uMod;
	const u32 uFatHigh_Offset = uFatHigh_Tmp % uMod;

	ASSERT( uFatHigh_Index >= 0 && uFatHigh_Index < 32 );

	std::vector<u32> indirect_cluster_High2Low;
	indirect_cluster_High2Low.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );
	if( !readCluster ( &indirect_cluster_High2Low[0] , m_pHead->indir_fat_clusters[ uFatHigh_Index ] ) )
		return false;

	std::vector<u32> fat_cluster;
	fat_cluster.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );
	if( !readCluster ( &fat_cluster[0] , indirect_cluster_High2Low[ uFatHigh_Offset ] ) )
		return false;

	// The offset in the fat cluster where we can find information about the sector
	const u32 uFatLow_Offset = a_uCluster % uMod;

	// Check if the entry in the fat table corresponds to a free cluster or a eof cluster
	if ( fat_cluster[ uFatLow_Offset ] == FREE_CLUSTER || fat_cluster[ uFatLow_Offset ] == ERROR_CLUSTER ) 
	{
		a_uNextCluster = fat_cluster[ uFatLow_Offset ];
		return true;
	}

	a_uNextCluster = fat_cluster[ uFatLow_Offset ] & FREE_CLUSTER;
	// Return the fat entry, but remove the most significant bit.
	return true;
}

bool CVPscMc::setFatEntry ( u32 a_uCluster, u32 a_uValue, SetFat_Mode a_eMode )
{
	const u32 uMod = m_pHead->cluster_size / sizeof(u32);
	// The fat index, aka which cluster we want to know about const u32& fat_index = a_uCluster;

	// Which fat cluster that the information would be in
	const u32 uFatHigh_Tmp = a_uCluster / uMod;

	const u32 uFatHigh_Index = uFatHigh_Tmp / uMod;
	const u32 uFatHigh_Offset = uFatHigh_Tmp % uMod;

	ASSERT( uFatHigh_Index >= 0 && uFatHigh_Index < 32 );

	std::vector<u32> indirect_cluster_High2Low;
	indirect_cluster_High2Low.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );
	if( !readCluster ( &indirect_cluster_High2Low[0] , m_pHead->indir_fat_clusters[ uFatHigh_Index ] ) )
		return false;

	std::vector<u32> fat_cluster;
	fat_cluster.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );
	if( !readCluster ( &fat_cluster[0] , indirect_cluster_High2Low[ uFatHigh_Offset ] ) )
		return false;

	// The offset in the fat cluster where we can find information about the sector
	const u32 uFatLow_Offset = a_uCluster % uMod;

	if ( a_uValue == FREE_CLUSTER || a_uValue == ERROR_CLUSTER || a_eMode == FAT_RESET )
	{
		fat_cluster[ uFatLow_Offset ] = a_uValue;
	}
	else
	{
		fat_cluster[ uFatLow_Offset ] = a_uValue | MASK_CLUSTER;
	}

	return writeCluster ( &fat_cluster[0] , indirect_cluster_High2Low[ uFatHigh_Offset ] );
}

u32 CVPscMc::getFreeCluster()
{
	std::vector<u32> indirect_cluster_High2Low;
	indirect_cluster_High2Low.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );

	std::vector<u32> fat_cluster;
	fat_cluster.resize( m_pHead->pages_per_cluster * (m_pHead->page_size / sizeof(u32)) , ERROR_CLUSTER );

	for( u32 uFatHigh_Index = 0 ;
		uFatHigh_Index < (sizeof(m_pHead->indir_fat_clusters)/sizeof(*m_pHead->indir_fat_clusters)) ;
		++uFatHigh_Index )
	{
		if( m_pHead->indir_fat_clusters[ uFatHigh_Index ] < 8 )
			return ERROR_CLUSTER;

		if( !readCluster ( &indirect_cluster_High2Low[0] , m_pHead->indir_fat_clusters[ uFatHigh_Index ] ) )
			return ERROR_CLUSTER;

		for( std::vector<u32>::size_type uHigh2Low = 0 ; uHigh2Low < indirect_cluster_High2Low.size() ; ++uHigh2Low )
		{
			if( ERROR_CLUSTER == indirect_cluster_High2Low[uHigh2Low] )
				continue;

			if( !readCluster ( &fat_cluster[0] , indirect_cluster_High2Low[uHigh2Low] ) )
				return ERROR_CLUSTER;

			for( std::vector<u32>::size_type uFatLow = 0 ; uFatLow < fat_cluster.size() ; ++uFatLow )
			{
				const u32 free_cluster = fat_cluster[uFatLow];

				if( FREE_CLUSTER == free_cluster ||
					(ERROR_CLUSTER != free_cluster && (free_cluster & MASK_CLUSTER) != MASK_CLUSTER) )
				{
					const u32 uMod = m_pHead->cluster_size / sizeof(u32);

					const u32 uRt = uFatLow + uHigh2Low * uMod + uFatHigh_Index * uMod * uMod;

					return uRt;
				}
			}
		}
	}

	return ERROR_CLUSTER;
}

//-----------------------------------------------------------------
bool CVPscMc::getDirentryFromPath( SPs2DirEntry& a_DirEnt , const CStringA& a_strPath , bool a_bMustExists )
{
	if( !isOpen() )
		return false;

	if( !ReadPage( &a_DirEnt , (m_pHead->first_allocatable + m_pHead->rootdir_cluster) * m_pHead->pages_per_cluster ) )
		return false;

	CStringA strPath = a_strPath;

	u32 uEntryIndex = 0;

	do
	{
		strPath.TrimLeft();
		strPath.TrimLeft( '/' );
		strPath.TrimLeft( '\\' );

		if( !strPath.GetLength() )
			break;

		const int iPos1 = strPath.Find( '\\' );
		const int iPos2 = strPath.Find( '/' );

		CStringA strName;
		if( iPos1 == -1 && iPos2 == -1 )
		{
			strName = strPath;
			strPath.Delete( 0 , strPath.GetLength() );
		}
		else
		{
			int iPosX ;
			if( iPos1 != -1 && ( iPos1 < iPos2 || iPos2 == -1 ) )
			{
				iPosX = iPos1;
			}
			else if( iPos2 != -1 && ( iPos2 < iPos1 || iPos1 == -1 ) )
			{
				iPosX = iPos2;
			}
			else
			{
				ASSERT(0);
				return false;
			}

			strName = strPath.Left( iPosX );
			strPath.Delete( 0 , iPosX + 1 );
		}

		const SPs2DirEntry DirEnt_Prev = a_DirEnt;
		if( !FindDirentryFromDirentry( a_DirEnt , uEntryIndex , DirEnt_Prev , strName , a_bMustExists ) )
			return false;

		ASSERT( a_DirEnt.mode & DF_EXISTS );
	}
	while( strPath.GetLength() );

	return true;
}

bool CVPscMc::getDirentryFromDirentry( SPs2DirEntry& a_DirEnt , const SPs2DirEntry& a_BaseDirEnt , u32 a_uDirEntIndex )
{
	ASSERT( a_BaseDirEnt.mode & DF_DIRECTORY );
	ASSERT( a_BaseDirEnt.mode & DF_EXISTS );

	u32 uPageOffset;
	u32 uClusterIndex;

	if( GetClusterIndex_ByEntryIndex( uClusterIndex , uPageOffset , a_BaseDirEnt , a_uDirEntIndex ) )
		return ReadPage ( &a_DirEnt , ( m_pHead->first_allocatable + uClusterIndex ) * m_pHead->pages_per_cluster + uPageOffset );

	return false;
}

bool CVPscMc::setDirentryFromDirentry( const SPs2DirEntry& a_DirEnt , const SPs2DirEntry& a_BaseDirEnt , u32 a_uDirEntIndex )
{
	ASSERT( a_BaseDirEnt.mode & DF_DIRECTORY );
	ASSERT( a_BaseDirEnt.mode & DF_EXISTS );

	u32 uPageOffset;
	u32 uClusterIndex;

	if( GetClusterIndex_ByEntryIndex( uClusterIndex , uPageOffset , a_BaseDirEnt , a_uDirEntIndex ) )
		return WritePage( &a_DirEnt , ( m_pHead->first_allocatable + uClusterIndex ) * m_pHead->pages_per_cluster + uPageOffset );

	return false;
}

bool CVPscMc::FindDirentryFromDirentry( SPs2DirEntry& a_DirEnt , u32& a_uEntryIndex , const SPs2DirEntry& a_BaseDirEnt , const CStringA& a_strName , bool a_bMustExists )
{
	if( !(a_BaseDirEnt.mode & DF_DIRECTORY) )
	{
		ASSERT( a_BaseDirEnt.mode & DF_DIRECTORY );
		return false;
	}

	if( !(a_BaseDirEnt.mode & DF_EXISTS) )
	{
		ASSERT( a_BaseDirEnt.mode & DF_EXISTS );
		return false;
	}

	for( a_uEntryIndex = 0 ; a_uEntryIndex < a_BaseDirEnt.length ; ++a_uEntryIndex )
	{
		if( !getDirentryFromDirentry( a_DirEnt , a_BaseDirEnt , a_uEntryIndex ) )
			return false;

		if( a_bMustExists )
			if( !(a_DirEnt.mode & DF_EXISTS) )
				continue;

		if( a_DirEnt.name == a_strName )
			return true;
	}

	return false;
}

bool CVPscMc::GetClusterIndex_ByEntryIndex( u32& a_OutClusterIndex , u32& a_OutPageOffset , const SPs2DirEntry& a_BaseDirEnt , u32 a_uEntryIndex )
{
	ASSERT( a_BaseDirEnt.mode & DF_DIRECTORY );
	ASSERT( a_BaseDirEnt.mode & DF_EXISTS );

	const u32 uClustetOffset = a_uEntryIndex / m_pHead->pages_per_cluster;
	a_OutPageOffset = a_uEntryIndex % m_pHead->pages_per_cluster;

	a_OutClusterIndex = a_BaseDirEnt.cluster;
	for( u32 uIndex = 0 ; uIndex < uClustetOffset ; ++uIndex )
	{
		u32 uNext;
		if( !getFatEntry ( uNext , a_OutClusterIndex ) )
			return false;

		if( uNext == ERROR_CLUSTER )
			return false;

		a_OutClusterIndex = uNext;
	}

	return true;
}

bool CVPscMc::Vmc_Mkdir( const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified ) 
{
	if( !isOpen() )
		return false;

	return _Vmc_Mkdir( a_strPath , a_strName , a_pCreated , a_pModified , NULL );
}

bool CVPscMc::_Vmc_Mkdir( SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode )
{
	if( !isOpen() )
		return false;

	if( a_puMode )
	{
		const u16& uMode= *a_puMode;
		if( !(uMode & DF_EXISTS) ||
			!(uMode & DF_DIRECTORY) ||
			(uMode & DF_FILE) )
			return false;
	}

	//------------------------------------------------------------
	SPs2DirEntry dirent_file;
	u32 uEntryIndex;

	if( FindDirentryFromDirentry( dirent_file , uEntryIndex , a_DirEnt_Path , a_strName , true ) )
	{
		ASSERT( DF_EXISTS & dirent_file.mode );

		if( !(dirent_file.mode & DF_DIRECTORY) )
			return false;//已存在同名文件，所以不能创建同名文件夹

		if( a_pCreated || a_pModified || a_puMode )
		{
			if( a_pCreated )
				dirent_file.created = *a_pCreated;

			if( a_pModified )
				dirent_file.modified = *a_pModified;

			if( a_puMode )
				dirent_file.mode = *a_puMode;

			if( !setDirentryFromDirentry( dirent_file , a_DirEnt_Path , uEntryIndex ) )
				return false;
		}

		return true;
	}

	SPs2DirEntry dirent_new = {};

	if( a_puMode )
		dirent_new.mode = *a_puMode;
	else
		dirent_new.mode = DF_EXISTS | DF_0400 | DF_DIRECTORY | DF_READ | DF_WRITE | DF_EXECUTE; //  0x8427

	strcpy( dirent_new.name , a_strName.GetString() );

	if( a_pCreated )
		dirent_new.created = *a_pCreated;
	else
		getPs2Time( &dirent_new.created );

	if( a_pModified )
		dirent_new.modified = *a_pModified;
	else
		getPs2Time( &dirent_new.modified );
	//-----------------------------------------------
	dirent_new.cluster = getFreeCluster();
	if( dirent_new.cluster == ERROR_CLUSTER )
		return false;

	if( !setFatEntry( dirent_new.cluster , ERROR_CLUSTER , FAT_SET ) )
		return false;

	dirent_new.length = 2;

	if( !addObject( uEntryIndex , a_DirEnt_Path , dirent_new , false ) )
		return false;

	if( !writeDirBase( dirent_new.cluster , a_DirEnt_Path , uEntryIndex ) )
		return false;

	return true;
}

bool CVPscMc::Vmc_DeleteFile( const CStringA& a_strPath , const CStringA& a_strName )
{
	if( !isOpen() )
		return false;

	SPs2DirEntry dirent_path;
	if( !getDirentryFromPath( dirent_path , a_strPath , true ) )
		return false;

	SPs2DirEntry dirent_file;
	u32 uEntryIndex;

	if( FindDirentryFromDirentry( dirent_file , uEntryIndex , dirent_path , a_strName , true ) )
		return _Vmc_DeleteFile( dirent_path , dirent_file , uEntryIndex );

	return false;
}

bool CVPscMc::_Vmc_DeleteFile( const SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName )
{
	if( !isOpen() )
		return false;

	SPs2DirEntry dirent_file;
	u32 uEntryIndex;

	if( FindDirentryFromDirentry( dirent_file , uEntryIndex , a_DirEnt_Path , a_strName , true ) )
		return _Vmc_DeleteFile( a_DirEnt_Path , dirent_file , uEntryIndex );

	return false;
}

bool CVPscMc::_Vmc_DeleteFile( const SPs2DirEntry& a_DirEnt_Path , SPs2DirEntry& a_DirEnt_File , u32 a_uEntryIndex )
{
	if( !isOpen() )
		return false;

	if( !(DF_EXISTS & a_DirEnt_File.mode) )
		return false;

	if( a_DirEnt_File.mode & DF_DIRECTORY )
		return false;//存在同名目录,无法删除

	u32 uCluster = a_DirEnt_File.cluster;
	while( uCluster != ERROR_CLUSTER )
	{
		if( uCluster == FREE_CLUSTER )
		{
			ASSERT(uCluster != FREE_CLUSTER);
			return false;
		}

		u32 uNext;
		if( !getFatEntry( uNext , uCluster ) )
			return false;

		if( uNext == ERROR_CLUSTER )
		{
			if( !setFatEntry( uCluster , FREE_CLUSTER , FAT_RESET ) )
				return false;
		}
		else if( uNext == FREE_CLUSTER )
		{
			ASSERT(uCluster != FREE_CLUSTER);
			return false;
		}
		else
		{
#if 0
			if( !setFatEntry( uCluster , uNext , FAT_RESET ) )
				return false;
#else
			if( !setFatEntry( uCluster , FREE_CLUSTER , FAT_RESET ) )
				return false;
#endif
		}

		uCluster = uNext;
	}

	a_DirEnt_File.cluster = ERROR_CLUSTER;
	a_DirEnt_File.mode ^= DF_EXISTS;
	return setDirentryFromDirentry( a_DirEnt_File , a_DirEnt_Path , a_uEntryIndex );
}

bool CVPscMc::Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified )
{
	return _Vmc_WriteFile( a_InFp , a_uSize , a_strPath , a_strName , a_pCreated , a_pModified , NULL );
}

bool CVPscMc::_Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode )
{
	if( !isOpen() )
		return false;

	ASSERT( a_DirEnt_Path.mode & DF_DIRECTORY );
	ASSERT( a_DirEnt_Path.mode & DF_EXISTS );

	if( a_puMode )
	{
		if( !(*a_puMode & DF_EXISTS) ||
			!(*a_puMode & DF_FILE) ||
			(*a_puMode & DF_DIRECTORY) )
			return false;
	}

	SPs2DirEntry dirent_file;
	u32 uEntryIndex;

	const bool bHasOldFile = FindDirentryFromDirentry( dirent_file , uEntryIndex , a_DirEnt_Path , a_strName , true );
	if( bHasOldFile )
	{
		ASSERT( DF_EXISTS & dirent_file.mode );

		if( dirent_file.mode & DF_DIRECTORY )
			return false;//存在同名目录,无法创建文件
		//删除文件

		if( !_Vmc_DeleteFile( a_DirEnt_Path , dirent_file , uEntryIndex ) )
			return false;
	}

	memset( &dirent_file , 0 , sizeof(dirent_file) );

	strcpy ( dirent_file.name , a_strName.GetString() );
	dirent_file.length  = a_uSize;
	dirent_file.cluster = ERROR_CLUSTER;

	if( a_puMode )
	{
		dirent_file.mode = *a_puMode;
	}
	else
	{
		dirent_file.mode = DF_EXISTS | DF_0400 | DF_FILE | DF_READ | DF_WRITE | DF_EXECUTE; //  0x8417
	}

	if( a_pCreated )
		dirent_file.created = *a_pCreated;
	else
		getPs2Time( &dirent_file.created );

	if( a_pModified )
		dirent_file.modified = *a_pModified;
	else
		getPs2Time( &dirent_file.modified );
	//---------------------------------------------------
	std::vector<u8> buffer;
	buffer.resize( m_pHead->cluster_size , 0 );
	u32 uOldCluster = ERROR_CLUSTER;

	while( a_uSize > 0 )
	{
		const u32 uRead = (a_uSize > m_pHead->cluster_size)?m_pHead->cluster_size:a_uSize;
		a_uSize -= uRead;

		u32 uCluster = getFreeCluster();

		if( uCluster == ERROR_CLUSTER )
			return false;

		if( uOldCluster == ERROR_CLUSTER )
		{
			dirent_file.cluster = uCluster;
		}
		else
		{
			if( !setFatEntry( uOldCluster , uCluster , FAT_SET ) )
				return false;
		}

		if( !setFatEntry( uCluster , ERROR_CLUSTER , FAT_SET ) )
			return false;

		if( uRead != a_InFp.Read( &buffer[0] , uRead ) )
			return false;

		if( !writeCluster( &buffer[0] , uCluster + m_pHead->first_allocatable ) )
			return false;

		uOldCluster = uCluster;
	}
	//---------------------------------------------------
	return addObject( uOldCluster , a_DirEnt_Path , dirent_file , bHasOldFile );
}

bool CVPscMc::_Vmc_ReadFile( CWQSG_xFile* a_pOutFp , const CStringA& a_strPath , const CStringA& a_strName , SPs2DateTime* a_pCreated , SPs2DateTime* a_pModified , u16* a_puMode )
{
	if( !isOpen() )
		return false;

	SPs2DirEntry dirent_path;
	if( !getDirentryFromPath( dirent_path , a_strPath , true ) )
		return false;

	return _Vmc_ReadFile( a_pOutFp , dirent_path , a_strName , a_pCreated , a_pModified , a_puMode );
	
}

bool CVPscMc::_Vmc_ReadFile( CWQSG_xFile* a_pOutFp , const SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , SPs2DateTime* a_pCreated , SPs2DateTime* a_pModified , u16* a_puMode )
{
	if( !isOpen() )
		return false;

	ASSERT( a_DirEnt_Path.mode & DF_DIRECTORY );
	ASSERT( a_DirEnt_Path.mode & DF_EXISTS );

	SPs2DirEntry dirent_file;
	u32 uEntryIndex;

	if( !FindDirentryFromDirentry( dirent_file , uEntryIndex , a_DirEnt_Path , a_strName , true ) )
		return false;
	//---------------------------------------------------
	if( a_pOutFp )
	{
		std::vector<u8> buffer;
		buffer.resize( m_pHead->cluster_size , 0 );
		u32 uCluster = dirent_file.cluster;
		u32 uFileSize = dirent_file.length;

		while( uFileSize > 0 )
		{
			if( uCluster == ERROR_CLUSTER || uCluster == FREE_CLUSTER )
				return false;

			const u32 uRead = (uFileSize > m_pHead->cluster_size)?m_pHead->cluster_size:uFileSize;
			uFileSize -= uRead;

			if( !readCluster( &buffer[0] , uCluster + m_pHead->first_allocatable ) )
				return false;

			if( uRead != a_pOutFp->Write( &buffer[0] , uRead ) )
				return false;

			u32 uNextCluster;
			if( !getFatEntry( uNextCluster , uCluster ) )
				return false;

			uCluster = uNextCluster;
		}
	}

	if( a_puMode )
		*a_puMode = dirent_file.mode;

	if( a_pCreated )
		*a_pCreated = dirent_file.created;

	if( a_pModified )
		*a_pModified = dirent_file.modified;

	return true;
}

bool CVPscMc::writeDirBase( u32 a_uPageStart , const SPs2DirEntry& a_Parent , u32 a_uEntryIndex )
{
	if( m_pHead->pages_per_cluster < 2 )
		return false;

	SPs2DirEntry pseudo_entries = {};

	// fill pseudo entries
	strcpy ( pseudo_entries.name , "." );
	pseudo_entries.dir_entry = a_uEntryIndex;
	pseudo_entries.length    = 0;
	pseudo_entries.cluster   = a_Parent.cluster;
	pseudo_entries.mode      = DF_EXISTS | DF_0400 | DF_DIRECTORY | DF_READ | DF_WRITE | DF_EXECUTE; // 0x8427

	getPs2Time ( &pseudo_entries.created  );
	getPs2Time ( &pseudo_entries.modified );

	// write first pseudo entry
	if( !WritePage ( &pseudo_entries , ( m_pHead->first_allocatable + a_uPageStart ) * m_pHead->pages_per_cluster ) )
		return false;

	strcpy ( pseudo_entries.name, ".." );
	pseudo_entries.dir_entry = 0;
	pseudo_entries.cluster   = 0;

	pseudo_entries.created = a_Parent.created;
	pseudo_entries.modified = a_Parent.modified;

	// write second pseudo entry
	if( !WritePage ( &pseudo_entries, ( m_pHead->first_allocatable + a_uPageStart ) * m_pHead->pages_per_cluster + 1 ) )
		return false;

	return true;
}

bool CVPscMc::addObject( u32& a_uEntryIndex , SPs2DirEntry& a_parent , SPs2DirEntry& a_dirent , bool a_bUseOldName ) 
{
	if( !isOpen() )
		return false;

	ASSERT( a_parent.mode & DF_DIRECTORY );
	ASSERT( a_parent.mode & DF_EXISTS );

	bool bFindOldName = false;
	a_uEntryIndex = 2;
	if( a_bUseOldName )
	{
		SPs2DirEntry dirent_file;
		bFindOldName = FindDirentryFromDirentry( dirent_file , a_uEntryIndex , a_parent , a_dirent.name , false );

		ASSERT( !bFindOldName || !(dirent_file.mode & DF_EXISTS) );
	}

	if( !bFindOldName )
	{
		for( ; a_uEntryIndex < a_parent.length ; ++a_uEntryIndex )
		{
			SPs2DirEntry dirent;
			if( !getDirentryFromDirentry( dirent , a_parent , a_uEntryIndex ) )
				return false;

			if( !(dirent.mode & DF_EXISTS) )
				break;
		}

		if( a_uEntryIndex == a_parent.length )//没找到标记删除的
		{
			if( (a_parent.length % m_pHead->pages_per_cluster) == 0 )//簇用完,需要添加
			{
				u32 current_cluster;
				u32 uItem;
				if( !GetClusterIndex_ByEntryIndex( current_cluster , uItem , a_parent , a_uEntryIndex - 1 ) )
					return false;
				// Get a free cluster because our object require an additional cluster
				const u32 nextfree_cluster = getFreeCluster();

				if( nextfree_cluster == ERROR_CLUSTER ) 
				{
					DEBUGPRINT ( 2, "vmcfs: Not enough free space.  Aborting.\n" );
					return false;
				}

				DEBUGPRINT ( 6, "vmcfs: Added new object in fat table cluster %u ( Page %u ) \n", nextfree_cluster , nextfree_cluster * m_pHead->pages_per_cluster );

				if( !setFatEntry( nextfree_cluster , ERROR_CLUSTER , FAT_SET ) )     // set the free cluster we just found to be EOF
				{
					return false;
				}

				if( !setFatEntry ( current_cluster , nextfree_cluster , FAT_SET ) ) // update the last cluster in the directory entry list to point to our new cluster
				{
					return false;
				}
			}
		}
	}

	if( !setDirentryFromDirentry( a_dirent , a_parent , a_uEntryIndex ) )
		return false;

	if( a_uEntryIndex == a_parent.length )
	{
		SPs2DirEntry parentparent;
		if( !getDirentryFromDirentry( parentparent , a_parent , 0 ) )
			return false;

		ASSERT( strcmp( "." , parentparent.name ) == 0 );

		a_parent.length++;
		if( !setDirentryFromDirentry( a_parent , parentparent , parentparent.dir_entry ) )
			return false;
	}

	return true;
}

bool CVPscMc::removeObject( const SPs2DirEntry& a_parent , u32 a_uEntryIndex )
{
	SPs2DirEntry dirent;
	if( !getDirentryFromDirentry( dirent , a_parent , a_uEntryIndex ) )
		return false;

	if( !(dirent.mode & DF_EXISTS) )
		return false;

	u32 last_cluster    = dirent.cluster;

	DEBUGPRINT ( 3 , "vmcfs: Searching last cluster of direntry\n" );

	while ( 1 ) 
	{
		u32 current_cluster;

		if( !getFatEntry ( current_cluster , last_cluster ) )
			return false;

		if ( current_cluster == FREE_CLUSTER ) 
		{
			// FREE_CLUSTER mean nothing to remove or error, so return
			DEBUGPRINT ( 10 , "vmcfs: Testing cluster %u ... value is FREE_CLUSTER\n" , last_cluster );
			return false;
		}
		else if ( current_cluster == ERROR_CLUSTER ) 
		{
			// EOF_CLUSTER mean last cluster of the direntry is found
			DEBUGPRINT ( 3 , "vmcfs: Last cluster of direntry at %u\n" , last_cluster );
			break;
		}
		else
		{
			// Otherwise change bit mask of tested cluster
			DEBUGPRINT ( 10 , "vmcfs: Testing cluster %u ... value is %u\n", last_cluster , current_cluster );
			if( !setFatEntry ( last_cluster , current_cluster , FAT_RESET ) )
				return false;
		}

		last_cluster = current_cluster;
	}

	// Set last cluster of direntry as free.
	if( !setFatEntry ( last_cluster , FREE_CLUSTER , FAT_RESET ) )// set the last cluster of the file as free
		return false;

	// Set object as deleted. ( Remove DF_EXISTS flag )
	dirent.mode = dirent.mode ^ DF_EXISTS;
	if( !setDirentryFromDirentry( dirent , a_parent , a_uEntryIndex ) )
		return false;

	return true;
}

bool CVPscMc::_Vmc_Mkdir( const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode )
{
	if( !isOpen() )
		return false;

	DEBUGPRINT ( 1, "vmcfs: mkdir %s\n", a_strPath.GetString() );

	SPs2DirEntry dirent_dir;

	if( !getDirentryFromPath( dirent_dir , a_strPath , true ) )
		return false;

	return _Vmc_Mkdir( dirent_dir , a_strName , a_pCreated , a_pModified , a_puMode );
}

bool CVPscMc::_Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode )
{
	if( !isOpen() )
		return false;

	SPs2DirEntry dirent_path;
	if( !getDirentryFromPath( dirent_path , a_strPath , true ) )
		return false;

	return _Vmc_WriteFile( a_InFp , a_uSize , dirent_path , a_strName , a_pCreated , a_pModified , a_puMode );
}

bool CVPscMc::Vmc_ReadFile( CWQSG_xFile& a_OutFp , const CStringA& a_strPath , const CStringA& a_strName )
{
	return _Vmc_ReadFile( &a_OutFp , a_strPath , a_strName , NULL , NULL , NULL );
}
