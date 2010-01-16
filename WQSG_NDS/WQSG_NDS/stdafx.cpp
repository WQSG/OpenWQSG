/*  OpenWQSG - WQSG_NDS
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
// WQSG_NDS.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <vector>
static const u16 g_crc16tab[] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

__inline static u16 CalcCrc16( const u8* a_pData , u32 a_uLength , u16 a_crc = ~u16(0) )
{
	while ( a_uLength-- )
		a_crc = (a_crc >> 8) ^ g_crc16tab[(a_crc ^ *(a_pData++)) & 0xFF];

	return a_crc;
}

struct SNdsFindHandle
{
	SNdsFindData m_Data;
	u16 m_uFileId;
	s32 m_nOffset;
};

CNdsRom::CNdsRom()
: m_uRomSize(0)
, m_pLinkBlock(NULL)
, m_NdsHeader()
{
	ASSERT( sizeof(m_NdsHeader) == 512 );
	ASSERT( sizeof(SNdsDirRec) == 8 );
	ASSERT( sizeof(SNdsFileRec) == 8 );

	ASSERT( (s32)u16(0xFFFF) >= 0);
}

CNdsRom::~CNdsRom()
{
	Close();
}

__inline bool CNdsRom::zzz_Alloc( const CStringA& a_strPath )
{
	SNdsFindHandle* pHandle = FindFile( a_strPath );
	if( !pHandle )
	{
		m_strError.Format( L"内存不足\r\nSNdsFindHandle* pHandle = FindFile( a_strPath )" );
		return false;
	}

	SNdsFindData data;
	while(true)
	{
		if( !FindNextFile( data , pHandle ) )
		{
			ASSERT(0);
			delete pHandle;
			return false;
		}

		if( data.m_bEmpty )
			break;

		if( data.m_bDir )
		{
			if( !zzz_Alloc( a_strPath + "/" + data.m_szName ) )
			{
				delete pHandle;
				return false;
			}
		}
		else if(data.m_File.m_uSize)
		{
			if( !m_pLinkBlock->AllocPos( data.m_File.m_uRomOffset , data.m_File.m_uSize ) )
			{
				m_strError.Format( L"Rom空间不足?\r\n这Rom有问题吧" );
				ASSERT(0);
				delete pHandle;
				return false;
			}
		}
	}

	delete pHandle;
	return true;
}

__inline bool CNdsRom::zzz_AllocSize( u32& a_uNewPos , const u32 a_uNewSize , const u32 a_uAlign ,
									 const u32 a_uOldPos , const u32 a_uOldSize )
{
	if( !m_pLinkBlock->Free( a_uOldPos ) )
	{
		m_strError.Format( L"%hs : !m_pLinkBlock->Free( a_uOldPos ),一定是bug", __FUNCDNAME__  );
		ASSERT(0);
		return false;//
	}

	a_uNewPos = a_uOldPos;

	if( !m_pLinkBlock->AllocPos( a_uNewPos , a_uNewSize ) )
	{
		const s32 startFind = ( a_uOldPos < m_NdsHeader.Banner_Offset )?0:m_NdsHeader.Banner_Offset;

		s32 nNewOffset = m_pLinkBlock->Alloc( a_uNewSize , a_uAlign , startFind );
		if( nNewOffset == -1 )
		{
			m_strError.Format( L"%hs : Rom的空间不足", __FUNCDNAME__  );
			//恢复现场
			const bool bOk = m_pLinkBlock->AllocPos( a_uOldPos , a_uOldSize );
			ASSERT(bOk);
			return false;
		}

		a_uNewPos = nNewOffset;
	}

	return true;
}

__inline bool CNdsRom::zzz_UdateRec( const SNdsFindData& a_DirData , const SNdsFindData& a_FileData )
{
	if( !a_DirData.m_bDir )
	{
		m_strError.Format( L"%hs : !a_DirData.m_bDir,一定是bug", __FUNCDNAME__  );
		ASSERT(a_DirData.m_bDir);
		return false;
	}

	if( a_FileData.m_bDir )
	{
		m_strError.Format( L"%hs : a_FileData.m_bDir,一定是bug", __FUNCDNAME__  );
		ASSERT(!a_FileData.m_bDir);
		return false;
	}

	switch ( a_DirData.m_Dir.m_eType )
	{
	case E_NFHT_ROOT:
		switch( a_FileData.m_File.m_uFileID_InDir )
		{
		case 0://arm9
			if( m_NdsHeader.Arm9_Rom_Offset != a_FileData.m_File.m_uRomOffset ||
				m_NdsHeader.Arm9_Size != a_FileData.m_File.m_uSize )
			{
				m_NdsHeader.Arm9_Rom_Offset = a_FileData.m_File.m_uRomOffset;
				m_NdsHeader.Arm9_Size = a_FileData.m_File.m_uSize;
				//修复header
				m_NdsHeader.Header_CRC = CalcCrc16( (unsigned char *)&m_NdsHeader , 0x15E );

				m_hRom.Seek( 0 );
				if( sizeof(m_NdsHeader) != m_hRom.Write( &m_NdsHeader , sizeof(m_NdsHeader) ) )
				{
					m_strError.Format( L"%hs : arm9 写 Rom 失败", __FUNCDNAME__  );
					return false;
				}
			}
			break;
		case 1://arm7
			if( m_NdsHeader.Arm7_Rom_Offset != a_FileData.m_File.m_uRomOffset ||
				m_NdsHeader.Arm7_Size != a_FileData.m_File.m_uSize )
			{
				m_NdsHeader.Arm7_Rom_Offset = a_FileData.m_File.m_uRomOffset;
				m_NdsHeader.Arm7_Size = a_FileData.m_File.m_uSize;
				//修复header
				m_NdsHeader.Header_CRC = CalcCrc16( (unsigned char *)&m_NdsHeader , 0x15E );

				m_hRom.Seek( 0 );
				if( sizeof(m_NdsHeader) != m_hRom.Write( &m_NdsHeader , sizeof(m_NdsHeader) ) )
				{
					m_strError.Format( L"%hs : arm7 写 Rom 失败", __FUNCDNAME__  );
					return false;
				}
			}
			break;
		default:
			m_strError.Format( L"%hs : 根目录只能导入arm9好人arm7,一定是bug", __FUNCDNAME__  );
			ASSERT(0);
			return false;
		}
		break;

	case E_NFHT_OVERLAY:
		{
			u16 uFileID_InDir = a_FileData.m_File.m_uFileID_InDir;

			const size_t count9 = m_NdsHeader.Arm9_Overlay_Size / sizeof(OVERLAYENTRY);

			s64 offset;

			if( uFileID_InDir >= count9 )
			{
				uFileID_InDir -= (u16)count9;

				const size_t count7 = m_NdsHeader.Arm7_Overlay_Size / sizeof(OVERLAYENTRY);
				if( uFileID_InDir >= count7 )
				{
					m_strError.Format( L"%hs : auFileID_InDir >= count7" , __FUNCDNAME__  );
					ASSERT( uFileID_InDir < count7 );
					return false;
				}

				offset = m_NdsHeader.Arm7_Overlay_Offset;
			}
			else
			{
				offset = m_NdsHeader.Arm9_Overlay_Offset;
			}

			m_hRom.Seek( offset + uFileID_InDir * sizeof(OVERLAYENTRY) );

			OVERLAYENTRY overlay;
			if( sizeof(overlay) != m_hRom.Read( &overlay , sizeof(overlay) ) )
			{
				m_strError.Format( L"%hs : 读 OVERLAYENTRY 失败" , __FUNCDNAME__  );
				return false;
			}

			offset = m_NdsHeader.Fat_Offset + overlay.file_id * sizeof(SNdsFileRec);

			m_hRom.Seek( offset );

			SNdsFileRec fileRec;
			if( sizeof(fileRec) != m_hRom.Read( &fileRec , sizeof(fileRec) ) )
			{
				m_strError.Format( L"%hs : 读 SNdsFileRec 失败" , __FUNCDNAME__  );
				return false;
			}

			const u32 uTop = a_FileData.m_File.m_uRomOffset;

			const s64 sBottom = uTop + a_FileData.m_File.m_uSize;
			if( sBottom > 0xFFFFFFFF )
			{
				m_strError.Format( L"%hs : sBottom > 0xFFFFFFFF" , __FUNCDNAME__  );
				ASSERT( sBottom <= 0xFFFFFFFF );
				return false;
			}

			const u32 uBottom = (u32)sBottom;

			if( fileRec.top != uTop || fileRec.bottom != uBottom )
			{
				fileRec.top = uTop;
				fileRec.bottom = uBottom;

				m_hRom.Seek( offset );

				if( sizeof(fileRec) != m_hRom.Write( &fileRec , sizeof(fileRec) ) )
				{
					m_strError.Format( L"%hs : OVERLAYENTRY 写 Rom 失败" , __FUNCDNAME__  );
					return false;
				}
			}
		}
		break;

	case E_NFHT_DATA:
		{
			s64 nPos;
#if 0
			if( a_off < 0 )
				return false;

			nPos = m_NdsHeader.Fnt_Offset + ((u32)a_DirData.m_Dir.m_uDir_ID*sizeof(SNdsDirRec));

			m_hRom.Seek( nPos );

			SNdsDirRec dirRec;
			if( sizeof(dirRec) != m_hRom.Read( &dirRec , sizeof(dirRec) ) )
				return false;

			nPos = m_NdsHeader.Fnt_Offset + dirRec.entry_start /*+ (a_uFileId + dirRec.top_file_id) * sizeof(info)*/;

			
			m_hRom.Seek( nPos + a_off );

			u8 info;
			if( sizeof(info) != m_hRom.Read( &info , sizeof(info) ) )
				return false;

			const bool bEmpty = info == 0;

			if( bEmpty )
				return false;

			const bool bDir = (info & 0x80) != 0;
			if( bDir )
				return false;

			info &= 0x7F;

// 			if( info != m_hRom.Read( a_Data.m_szName , info ) )
// 				return -1;

				a_nOffset += (info + 1);


				a_Data.m_File.m_uFileID_InRom = a_uFileId + dirRec.top_file_id;
				a_Data.m_File.m_uFileID_InDir = a_uFileId;

				a_Data.m_File.m_nFatOffset = ((s32)a_FileData.m_File.m_uFileID_InRom * sizeof(SNdsFileRec));
				
#endif
			if( a_FileData.m_File.m_nFatOffset < 0 )
			{
				m_strError.Format( L"%hs : a_FileData.m_File.m_nFatOffset < 0" , __FUNCDNAME__  );
				ASSERT(a_FileData.m_File.m_nFatOffset >= 0);
				return false;
			}

			nPos = m_NdsHeader.Fat_Offset + a_FileData.m_File.m_nFatOffset;

			m_hRom.Seek( nPos );

			SNdsFileRec fileRec;
			if( sizeof(fileRec) != m_hRom.Read( &fileRec , sizeof(fileRec) ) )
			{
				m_strError.Format( L"%hs : 读文件记录" , __FUNCDNAME__  );
				return false;
			}

			const u32 uTop = a_FileData.m_File.m_uRomOffset;

			const s64 sBottom = uTop + a_FileData.m_File.m_uSize;
			if( sBottom > 0xFFFFFFFF )
			{
				m_strError.Format( L"%hs : sBottom > 0xFFFFFFFF" , __FUNCDNAME__  );
				ASSERT( sBottom <= 0xFFFFFFFF );
				return false;
			}

			const u32 uBottom = (u32)sBottom;

			if( fileRec.top != uTop || fileRec.bottom != uBottom )
			{
				fileRec.top = uTop;
				fileRec.bottom = uBottom;

				m_hRom.Seek( nPos );

				if( sizeof(fileRec) != m_hRom.Write( &fileRec , sizeof(fileRec) ) )
				{
					m_strError.Format( L"%hs : 写文件记录失败" , __FUNCDNAME__  );
					return false;
				}
			}
		}
		break;

	default:
		m_strError.Format( L"%hs : 未知目录类型,一定是bug", __FUNCDNAME__  );
		ASSERT(0);
		return false;
	}

	return true;
}

bool CNdsRom::Open( const CStringW& a_strFile )
{
	Close();

	if( !m_hRom.OpenFile( a_strFile.GetString() , 3 , 3 ) )
	{
		if( !m_hRom.OpenFile( a_strFile.GetString() , 1 , 3 ) )
		{
			m_strError = L"打开文件失败";
			return false;
		}
	}

	const n64 nSile = m_hRom.GetFileSize();

	if( sizeof(m_NdsHeader) != m_hRom.Read( &m_NdsHeader , sizeof(m_NdsHeader) ) )
	{
		m_strError.Format( L"读文件头失败,确定这是NDS Rom?" );
		goto __gtOpenErr;
	}

	if( nSile > (1024*1024*1024) )
	{
		m_strError.Format( L"Rom文件大于1GB了..." );
		goto __gtOpenErr;
	}

	m_uRomSize = (u32)nSile;

	if( m_NdsHeader.Arm9_Rom_Offset % 4096 )
	{
		m_strError.Format( L"Rom校验失败\r\nm_NdsHeader.Arm9_Rom_Offset % 4096\r\nm_NdsHeader.Arm9_Rom_Offset = %08X" , m_NdsHeader.Arm9_Rom_Offset );
		goto __gtOpenErr;
	}

	if( m_NdsHeader.Arm7_Rom_Offset % 512 )
	{
		m_strError.Format( L"Rom校验失败\r\nm_NdsHeader.Arm7_Rom_Offset % 512\r\nm_NdsHeader.Arm7_Rom_Offset = %08X" , m_NdsHeader.Arm7_Rom_Offset );
		goto __gtOpenErr;
	}

// 	if( m_NdsHeader.Arm9_Overlay_Offset % 512 )
// 		goto __gtOpenErr;
// 
// 	if( m_NdsHeader.Arm7_Overlay_Offset % 512 )
// 		goto __gtOpenErr;

	if( m_NdsHeader.Banner_Offset % 512 )
	{
		m_strError.Format( L"Rom校验失败\r\nm_NdsHeader.Banner_Offset % 512\r\nm_NdsHeader.Banner_Offset = %08X" , m_NdsHeader.Banner_Offset );
		goto __gtOpenErr;
	}

	if( m_NdsHeader.Fnt_Offset % 4/*512*/ )
	{
		m_strError.Format( L"Rom校验失败\r\nm_NdsHeader.Fnt_Offset % 4\r\nm_NdsHeader.Fnt_Offset = %08X" , m_NdsHeader.Fnt_Offset );
		goto __gtOpenErr;
	}

	if( m_NdsHeader.Fat_Offset % 4/*512*/ )
	{
		m_strError.Format( L"Rom校验失败\r\nm_NdsHeader.Fat_Offset % 4\r\nm_NdsHeader.Fat_Offset = %08X" , m_NdsHeader.Fat_Offset );
		goto __gtOpenErr;
	}

	m_pLinkBlock = new CWQSG_PartitionList( m_uRomSize );
	if( !m_pLinkBlock )
	{
		m_strError.Format( L"内存不足\r\nm_pLinkBlock = new CWQSG_PartitionList( m_uRomSize )" );
		goto __gtOpenErr;
	}

	if( !m_pLinkBlock->AllocPos( 0 , 0x4000 ) )
	{
		m_strError.Format( L"Rom空间不足\r\nm_pLinkBlock->AllocPos( 0 , 0x4000 )" );
		goto __gtOpenErr;
	}

// 	if( !m_pLinkBlock->AllocPos( m_NdsHeader.Arm9_Rom_Offset , m_NdsHeader.Arm9_Size ) )
// 		goto __gtOpenErr;
// 
// 	if( !m_pLinkBlock->AllocPos( m_NdsHeader.Arm7_Rom_Offset , m_NdsHeader.Arm7_Size ) )
// 		goto __gtOpenErr;

// 	if( m_NdsHeader.Arm9_Overlay_Offset && m_NdsHeader.Arm9_Overlay_Size )
// 		if( !m_pLinkBlock->AllocPos( m_NdsHeader.Arm9_Overlay_Offset , m_NdsHeader.Arm9_Overlay_Size ) )
// 			goto __gtOpenErr;
// 
// 	if( m_NdsHeader.Arm7_Overlay_Offset && m_NdsHeader.Arm7_Overlay_Size )
// 		if( !m_pLinkBlock->AllocPos( m_NdsHeader.Arm7_Overlay_Offset , m_NdsHeader.Arm7_Overlay_Size ) )
// 			goto __gtOpenErr;

	if( !m_pLinkBlock->AllocPos( m_NdsHeader.Fnt_Offset , m_NdsHeader.Fnt_Size ) )
	{
		m_strError.Format( L"Rom空间不足\r\nFnt" );
		goto __gtOpenErr;
	}

	if( !m_pLinkBlock->AllocPos( m_NdsHeader.Fat_Offset , m_NdsHeader.Fat_Size ) )
	{
		m_strError.Format( L"Rom空间不足\r\nFat" );
		goto __gtOpenErr;
	}

	if( !m_pLinkBlock->AllocPos( m_NdsHeader.Banner_Offset , 0x840 ) )
	{
		m_strError.Format( L"Rom空间不足\r\nBanner" );
		goto __gtOpenErr;
	}

	if( !zzz_Alloc( "" ) )
		goto __gtOpenErr;

	return true;
__gtOpenErr:
	Close();
	return false;
}

void CNdsRom::Close()
{
	m_hRom.Close();

	m_uRomSize = 0;

	if( m_pLinkBlock )
	{
		delete m_pLinkBlock;
		m_pLinkBlock = NULL;
	}
}

CStringA GetPathDir( CStringA& a_strPath )
{
	CStringA rt;

	while( a_strPath[0] == '/' )
		a_strPath.Delete(0);

	int pos = a_strPath.Find( '/' );
	if( pos == -1 )
	{
		rt = a_strPath;
		a_strPath = "";
	}
	else
	{
		rt = a_strPath.Left( pos );
		a_strPath.Delete( 0 , pos );

		while( a_strPath[0] == '/' )
			a_strPath.Delete(0);
	}

	return rt;
}

SNdsFindHandle* CNdsRom::FindFile( const CStringA& a_strPath )
{
	SNdsFindData dirData;

	GetPath( dirData , a_strPath );

	SNdsFindHandle* pRt = new SNdsFindHandle;
	if( pRt )
	{
		memset( pRt , 0 , sizeof(SNdsFindHandle) );
		pRt->m_Data = dirData;
	}

	return pRt;
}

void CNdsRom::FindClose( SNdsFindHandle* a_pHandle )
{
	if( a_pHandle )
		delete a_pHandle;
}

bool CNdsRom::GetPath( SNdsFindData& a_Data , CStringA a_strPath )
{
	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug", __FUNCDNAME__  );
		return false;
	}

	CStringA strDirName;

	memset( &a_Data , 0 , sizeof(a_Data) );
	a_Data.m_bDir = true;
	a_Data.m_Dir.m_eType = E_NFHT_ROOT;

	SNdsFindData data;
	while( !a_strPath.IsEmpty() )
	{
		strDirName = GetPathDir( a_strPath );

		if( strDirName.IsEmpty() )
			break;

		if( !zzz_FindFile( data , a_Data , strDirName ) )
			return false;

		if( !data.m_bDir )
		{
			m_strError.Format( L"%hs : 路径居然包含了文件,一定是bug", __FUNCDNAME__  );
			return false;
		}

		a_Data = data;
	}

	return true;
}

bool CNdsRom::FindNextFile( SNdsFindData& a_Data , SNdsFindHandle* a_pHandle /*, bool a_bFindNext */)
{
	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug", __FUNCDNAME__  );
		return false;
	}

	const s32 nNewOffset = zzz_FindNextFile( a_Data , a_pHandle->m_Data , a_pHandle->m_nOffset , a_pHandle->m_uFileId );
	if( nNewOffset < 0 )
		return false;

	if( !a_Data.m_bEmpty && !a_Data.m_bDir )
		a_pHandle->m_uFileId++;

	a_pHandle->m_nOffset = nNewOffset;

	return true;
}

bool CNdsRom::GetRec_Root( SNdsFindData& a_Data , s32 a_nFileId )
{
	memset( &a_Data , 0 , sizeof(a_Data) );

	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug" , __FUNCDNAME__ );
		return false;
	}

	if( a_nFileId < 0 )
	{
		m_strError.Format( L"%hs : 参数错误\r\a_nFileId < 0" , __FUNCDNAME__ );
		return false;
	}

	switch( a_nFileId )
	{
	case 0:
		strcpy( a_Data.m_szName , "arm9.bin" );

		a_Data.m_bDir = false;

		a_Data.m_File.m_uRomOffset = m_NdsHeader.Arm9_Rom_Offset;
		a_Data.m_File.m_uSize = m_NdsHeader.Arm9_Size;
		//a_Data.m_File.m_uFile_ID = (u16)a_nFileId;
		a_Data.m_File.m_uFileID_InDir = (u16)a_nFileId;

		break;
	case 1:
		strcpy( a_Data.m_szName , "arm7.bin" );

		a_Data.m_bDir = false;

		a_Data.m_File.m_uRomOffset = m_NdsHeader.Arm7_Rom_Offset;
		a_Data.m_File.m_uSize = m_NdsHeader.Arm7_Size;
		//a_Data.m_File.m_uFile_ID = (u16)a_nFileId;
		a_Data.m_File.m_uFileID_InDir = (u16)a_nFileId;
		
		break;
	case 2:
		strcpy( a_Data.m_szName , DEF_Overlay );

		a_Data.m_bDir = true;

		a_Data.m_Dir.m_eType = E_NFHT_OVERLAY;
		a_Data.m_Dir.m_uDir_ID = 0;

		break;
	case 3:
		strcpy( a_Data.m_szName , "data" );

		a_Data.m_bDir = true;

		a_Data.m_Dir.m_eType = E_NFHT_DATA;
		a_Data.m_Dir.m_uDir_ID = 0;

		break;
	default:
		a_Data.m_bEmpty = true;
	}

	return true;
}

bool CNdsRom::GetRec_Overlay( SNdsFindData& a_Data , s32 a_nFileId )
{
	memset( &a_Data , 0 , sizeof(a_Data) );

	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug" , __FUNCDNAME__ );
		return false;
	}

	if( a_nFileId < 0 )
	{
		m_strError.Format( L"%hs : 参数错误\r\a_nFileId < 0" , __FUNCDNAME__ );
		return false;
	}

	const s32 nCount9 = m_NdsHeader.Arm9_Overlay_Size / sizeof(OVERLAYENTRY);

	s64 offset;
	const n32 nIndex = a_nFileId;

	if( a_nFileId >= nCount9 )
	{
		a_nFileId -= nCount9;

		const s32 nCount7 = m_NdsHeader.Arm7_Overlay_Size / sizeof(OVERLAYENTRY);
		if( a_nFileId >= nCount7 )
		{
			a_Data.m_bEmpty = true;
			return true;
		}

		offset = m_NdsHeader.Arm7_Overlay_Offset;
	}
	else
	{
		offset = m_NdsHeader.Arm9_Overlay_Offset;
	}

	m_hRom.Seek( offset + a_nFileId * sizeof(OVERLAYENTRY) );

	OVERLAYENTRY overlay;
	if( sizeof(overlay) != m_hRom.Read( &overlay , sizeof(overlay) ) )
	{
		m_strError.Format( L"%hs : 读OVERLAYENTRY失败" , __FUNCDNAME__ );
		return false;
	}

	offset = m_NdsHeader.Fat_Offset + overlay.file_id * sizeof(SNdsFileRec);

	m_hRom.Seek( offset );

	SNdsFileRec fileRec;
	if( sizeof(fileRec) != m_hRom.Read( &fileRec , sizeof(fileRec) ) )
	{
		m_strError.Format( L"%hs : 读SNdsFileRec失败" , __FUNCDNAME__ );
		return false;
	}

	sprintf( a_Data.m_szName , "overlay_%04d.bin" , nIndex );
	a_Data.m_bDir = false;

	a_Data.m_File.m_nFatOffset = (s32)offset;
	ASSERT(a_Data.m_File.m_nFatOffset >= 0);

	a_Data.m_File.m_uFileID_InRom = overlay.file_id;
	a_Data.m_File.m_uFileID_InDir = (u16)nIndex;

	if( fileRec.top % 4 )
	{
		m_strError.Format( L"%hs : 文件校验失败\r\nfileRec.top % 4\r\nfileRec.top = %08X" , __FUNCDNAME__ , fileRec.top );
		return false;
	}

	if( fileRec.bottom < fileRec.top )
	{
		m_strError.Format( L"%hs : 文件校验失败\r\fileRec.bottom < fileRec.top\r\nfileRec.top = %08X , fileRec.bottom = %08X" , __FUNCDNAME__ , fileRec.top , fileRec.bottom );
		return false;
	}

	a_Data.m_File.m_uRomOffset = fileRec.top;
	a_Data.m_File.m_uSize = fileRec.bottom - fileRec.top;

	return true;
}

s32 CNdsRom::GetRec_Data( SNdsFindData& a_Data , u16 a_uDirID , u16 a_uFileId , s32 a_nOffset )
{
	memset( &a_Data , 0 , sizeof(a_Data) );

	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug", __FUNCDNAME__  );
		return -1;
	}

	if( a_nOffset < 0 )
	{
		m_strError.Format( L"%hs : 参数错误\r\na_nOffset < 0", __FUNCDNAME__  );
		return -1;
	}

	s64 nPos = m_NdsHeader.Fnt_Offset + ((u32)a_uDirID*sizeof(SNdsDirRec));

	m_hRom.Seek( nPos );

	SNdsDirRec dirRec;
	if( sizeof(dirRec) != m_hRom.Read( &dirRec , sizeof(dirRec) ) )
	{
		m_strError.Format( L"%hs : 读Rom记录失败", __FUNCDNAME__  );
		return -1;
	}

	nPos = m_NdsHeader.Fnt_Offset + dirRec.entry_start /*+ (a_uFileId + dirRec.top_file_id) * sizeof(info)*/;

	m_hRom.Seek( nPos + a_nOffset );

	u8 info;
	if( sizeof(info) != m_hRom.Read( &info , sizeof(info) ) )
	{
		m_strError.Format( L"%hs : 读Rom信息失败", __FUNCDNAME__  );
		return -1;
	}

	a_Data.m_nFntOffset = a_nOffset;

	a_Data.m_bEmpty = info == 0;

	if( !a_Data.m_bEmpty )
	{
		a_Data.m_bDir = (info & 0x80) != 0;

		info &= 0x7F;

		if( info != m_hRom.Read( a_Data.m_szName , info ) )
		{
			m_strError.Format( L"%hs : 读Rom信息失败2", __FUNCDNAME__  );
			return -1;
		}

		a_nOffset += (info + 1);

		if( a_Data.m_bDir )
		{
			if( sizeof(a_Data.m_Dir.m_uDir_ID) != m_hRom.Read( &a_Data.m_Dir.m_uDir_ID , sizeof(a_Data.m_Dir.m_uDir_ID) ) )
			{
				m_strError.Format( L"%hs : 读目录ID失败", __FUNCDNAME__  );
				return -1;
			}

			a_Data.m_Dir.m_uDir_ID &= 0xFFF;

			a_nOffset += sizeof(a_Data.m_Dir.m_uDir_ID);

			a_Data.m_Dir.m_eType = E_NFHT_DATA;
		}
		else
		{
			a_Data.m_File.m_uFileID_InRom = a_uFileId + dirRec.top_file_id;
			a_Data.m_File.m_uFileID_InDir = a_uFileId;

			a_Data.m_File.m_nFatOffset = ((s32)a_Data.m_File.m_uFileID_InRom * sizeof(SNdsFileRec));
			ASSERT(a_Data.m_File.m_nFatOffset >= 0);

			nPos = m_NdsHeader.Fat_Offset + a_Data.m_File.m_nFatOffset;

			m_hRom.Seek( nPos );

			SNdsFileRec fileRec;
			if( sizeof(fileRec) != m_hRom.Read( &fileRec , sizeof(fileRec) ) )
			{
				m_strError.Format( L"%hs : 读Rom文件记录失败" , __FUNCDNAME__ );
				return -1;
			}

			if( fileRec.top % 4 )
			{
				m_strError.Format( L"%hs : 文件校验失败\r\nfileRec.top % 4\r\nfileRec.top = %08X" , __FUNCDNAME__ , fileRec.top );
				return -1;
			}

			if( fileRec.bottom < fileRec.top )
			{
				m_strError.Format( L"%hs : 文件校验失败\r\fileRec.bottom < fileRec.top\r\nfileRec.top = %08X , fileRec.bottom = %08X" , __FUNCDNAME__ , fileRec.top , fileRec.bottom );
				return -1;
			}

			a_Data.m_File.m_uRomOffset = fileRec.top;
			a_Data.m_File.m_uSize = fileRec.bottom - fileRec.top;
		}
	}

	return a_nOffset;
}

bool CNdsRom::zzz_FindFile( SNdsFindData& a_FileData , const SNdsFindData& a_DirData , CStringA a_strName )
{
	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug" , __FUNCDNAME__ );
		return false;
	}

	a_strName.MakeLower();

	n32 nOffset = 0;
	u16 nFileID = 0;

	while( true )
	{
		const n32 nNextOffset = zzz_FindNextFile( a_FileData , a_DirData , nOffset , nFileID );
		if( nNextOffset < 0 )
			return false;

		nOffset = nNextOffset;

		if( a_FileData.m_bEmpty )
		{
			m_strError.Format( L"%hs : 忘了什么问题,算是bug?" , __FUNCDNAME__ );
			return false;
		}

		CStringA strName( a_FileData.m_szName );

		if( strName.MakeLower() == a_strName )
			return true;

		if( !a_FileData.m_bDir )
			nFileID++;
	}

	m_strError.Format( L"%hs : 不可能执行这段?程序一定有bug" , __FUNCDNAME__ );
	return false;
}

s32 CNdsRom::zzz_FindNextFile( SNdsFindData& a_FileData , const SNdsFindData& a_DirData , s32 a_nOffset , u16 a_uFileID )
{
	if( !IsOpen() )
	{
		m_strError.Format( L"%hs : 还没打开Rom?程序有bug" , __FUNCDNAME__ );
		return -1;
	}

	if( a_DirData.m_bDir )
	{
		switch ( a_DirData.m_Dir.m_eType )
		{
		case E_NFHT_ROOT:
			if( GetRec_Root( a_FileData , a_nOffset ) )
			{
				return a_nOffset + 1;
			}
			break;
		case E_NFHT_OVERLAY:
			if( GetRec_Overlay( a_FileData , a_nOffset ) )
			{
				return a_nOffset + 1;
			}
			break;
		case E_NFHT_DATA:
			{
				const s32 nNextOffset = GetRec_Data( a_FileData , a_DirData.m_Dir.m_uDir_ID , a_uFileID , a_nOffset );
				return nNextOffset;
			}
			break;
		default:
			m_strError.Format( L"%hs : 未知的目录类型?程序有bug" , __FUNCDNAME__ );
		}
	}
	else
		m_strError.Format( L"%hs : 谁把文件当目录了?程序有bug" , __FUNCDNAME__ );

	return -1;
}

bool CNdsRom::ImportFile( const SNdsFindData& a_DirData , CWQSG_xFile& a_InFile , const CStringA& a_strName )
{
	if( !IsCanWrite() )
	{
		m_strError.Format( L"%hs : Rom不是写模式?程序有bug" , __FUNCDNAME__ );
		return false;
	}

	if( a_DirData.m_bDir )
	{
		SNdsFindData fileData;
		bool bFind = zzz_FindFile( fileData , a_DirData , a_strName );

		if( !bFind )
		{
			m_strError.Format( L"%hs : 暂时不支持添加新文件" , __FUNCDNAME__ );
			return false;//不能创建新文件
		}

		if( bFind && fileData.m_bDir )
		{
			m_strError.Format( L"%hs : 已经存在同名目录" , __FUNCDNAME__ );
			return false;//不能写目录
		}

		const s64 sInMaxSize = a_InFile.GetFileSize();
		if( sInMaxSize > (1024*1024*1024) )
		{
			m_strError.Format( L"%hs : 不能导入大于1G的文件" , __FUNCDNAME__ );
			return false;
		}

		u32 uNewPos;
		const u32 uNewSize = (u32)sInMaxSize;

		switch ( a_DirData.m_Dir.m_eType )
		{
		case E_NFHT_ROOT:
			if( !bFind )
			{
				m_strError.Format( L"%hs : 不能在根目录添加文件" , __FUNCDNAME__ );
				return false;//此目录不能创建新文件
			}
			else
			{
				if ( fileData.m_File.m_uFileID_InDir == 0 )
				{
					if( !zzz_AllocSize( uNewPos , uNewSize , 4096 , fileData.m_File.m_uRomOffset , fileData.m_File.m_uSize ) )
						return false;
				}
				else if( !zzz_AllocSize( uNewPos , uNewSize , 512 , fileData.m_File.m_uRomOffset , fileData.m_File.m_uSize ) )
					return false;
			}

			break;

		case E_NFHT_OVERLAY:
			if( !bFind )
			{
				m_strError.Format( L"%hs : 不能在OVERLAY目录添加文件" , __FUNCDNAME__ );
				return false;//此目录不能创建新文件
			}
			else if( !zzz_AllocSize( uNewPos , uNewSize , 4 , fileData.m_File.m_uRomOffset , fileData.m_File.m_uSize ) )
				return false;

			break;

		case E_NFHT_DATA:
			if( !bFind )
				return false;//此目录暂时不支持创建新文件
			else if( !zzz_AllocSize( uNewPos , uNewSize , 4 , fileData.m_File.m_uRomOffset , fileData.m_File.m_uSize ) )
				return false;

			break;

		default:
			ASSERT(0);
			return false;
		}

		const u32 uBufSize = 1024*1024*1;
		std::vector<u8> Buf;
		Buf.resize( uBufSize , 0 );

		u8*const pBuf = &(Buf[0]);

		m_hRom.Seek( fileData.m_File.m_uRomOffset = uNewPos );
		u32 uRead = fileData.m_File.m_uSize = uNewSize;

		while( uRead > 0 )
		{
			const u32 uR = (uRead > uBufSize)?uBufSize:uRead;
			uRead -= uR;

			if( uR != a_InFile.Read( pBuf , uR ) )
			{
				m_strError.Format( L"%hs : 文件读取失败" , __FUNCDNAME__ );
				return false;
			}

			if( uR != m_hRom.Write( pBuf , uR ) )
			{
				m_strError.Format( L"%hs : Rom写入数据失败" , __FUNCDNAME__ );
				return false;
			}
		}

		return( zzz_UdateRec( a_DirData , fileData ) );
	}

	m_strError.Format( L"%hs : 谁在导入目录?程序有bug" , __FUNCDNAME__ );
	ASSERT(0);
	return false;
}
