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

#pragma once
#include <vector>

#ifdef DEBUG
#define DEBUGPRINT( level, args , ... ) \
	/*if( DEBUG >= level ) */printf( args , __VA_ARGS__ )
#else
#define DEBUGPRINT( args , ... )
#endif

#define FREE_CLUSTER	0x7FFFFFFF
#define ERROR_CLUSTER	0xFFFFFFFF
#define MASK_CLUSTER	0x80000000


class CVPscMc
{
	SPs2MemoryCardHead* m_pHead;
	u8* m_pBuf;

	std::vector<u8> m_Bufs;
	bool m_bOpen;

	std::vector<u8> m_BakBufs;
public:
	struct SFileInfo
	{
		char szName[32];
		bool bDir;
		u32 uSize;
	};
public:
	CVPscMc(void);
	virtual ~CVPscMc(void);

	bool LoadMc( const CStringW& a_strPathName );
	bool SaveMc( const CStringW& a_strPathName );

	bool SaveMcNoEcc( const CStringW& a_strPathName );

	bool isOpen()const{return m_bOpen;}
	//-----------------------
	bool Bak();
	bool UnBak();
protected:
	void UpdateEcc( char* Page_Data, char* ECC_Data );
	bool ReadPage( void* a_OutBuf , n32 a_nPageIndex );
	bool WritePage( const void* a_OutBuf , n32 a_nPageIndex );
	bool readCluster( void* a_pOutCluster, u32 a_uClusterIndex );
	bool writeCluster( const void* a_pOutCluster, u32 a_uClusterIndex );
	//---------------------------------------------
	typedef enum {
		FAT_RESET,
		FAT_SET
	} SetFat_Mode;

	bool getFatEntry ( u32& a_uNextCluster , u32 a_uCluster );
	bool setFatEntry ( u32 a_uCluster, u32 a_uValue , SetFat_Mode a_eMode );

	u32 getFreeCluster();
	//---------------------------------------------
	bool FindDirentryFromDirentry ( SPs2DirEntry& a_DirEnt , u32& a_uEntryIndex , const SPs2DirEntry& a_BaseDirEnt , const CStringA& a_strName , bool a_bMustExists );

	bool getDirentryFromPath ( SPs2DirEntry& a_DirEnt , const CStringA& a_strPath , bool a_bMustExists );

	bool getDirentryFromDirentry ( SPs2DirEntry& a_DirEnt , const SPs2DirEntry& a_BaseDirEnt , u32 a_uDirEntIndex );
	bool setDirentryFromDirentry ( const SPs2DirEntry& a_DirEnt , const SPs2DirEntry& a_BaseDirEnt , u32 a_uDirEntIndex );

	bool GetClusterIndex_ByEntryIndex( u32& a_OutClusterIndex , u32& a_OutPageOffset , const SPs2DirEntry& a_DirEnt , u32 a_uEntryIndex );

	bool _Vmc_Mkdir( SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode );

	bool _Vmc_DeleteFile( const SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName );
	bool _Vmc_DeleteFile( const SPs2DirEntry& a_DirEnt_Path , SPs2DirEntry& a_DirEnt_File , u32 a_uEntryIndex );

	bool _Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode );

	bool writeDirBase( u32 a_uPageStart , const SPs2DirEntry& a_Parent , u32 a_uEntryIndex );

	bool addObject( u32& a_uEntryIndex , SPs2DirEntry& a_parent , SPs2DirEntry& a_dirent , bool a_bUseOldName );
	bool removeObject( const SPs2DirEntry& a_parent , u32 a_uEntryIndex );

	bool _Vmc_Mkdir( const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode );
	bool _Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified , const u16* a_puMode );
	bool _Vmc_ReadFile( CWQSG_xFile* a_pOutFp , const CStringA& a_strPath , const CStringA& a_strName , SPs2DateTime* a_pCreated , SPs2DateTime* a_pModified , u16* a_puMode );
	bool _Vmc_ReadFile( CWQSG_xFile* a_pOutFp , const SPs2DirEntry& a_DirEnt_Path , const CStringA& a_strName , SPs2DateTime* a_pCreated , SPs2DateTime* a_pModified , u16* a_puMode );
public:
	bool Vmc_Mkdir( const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified );
	bool Vmc_WriteFile( CWQSG_xFile& a_InFp , u32 a_uSize , const CStringA& a_strPath , const CStringA& a_strName , const SPs2DateTime* a_pCreated , const SPs2DateTime* a_pModified );
	bool Vmc_ReadFile( CWQSG_xFile& a_OutFp , const CStringA& a_strPath , const CStringA& a_strName );
	bool Vmc_DeleteFile( const CStringA& a_strPath , const CStringA& a_strName );

	bool Import_Psu( const CString& a_strPathName );
	bool Export_Psu( const CString& a_strPathName , const CStringA a_strDirName );

	bool GetFiles( std::vector<SFileInfo>& a_Datas , const CStringA& a_strPath );
};
