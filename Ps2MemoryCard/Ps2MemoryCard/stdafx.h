
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


#pragma pack(push,1)
struct SPs2MemoryCardHead
{
	u8	magic[28];
	u8	version[12];
	u16	page_size;//Size in bytes of a memory card page. 
	u16	pages_per_cluster;//The number of pages in a cluster. 
	u16	pages_per_block;//The number of pages in an erase block. 
	u16	unused0;//0xFF00	Doesn't seem to be used
	u32	clusters_per_card;//8192	The total size of the card in clusters. 
	u32	first_allocatable;//41	Cluster offset of the first allocatable cluster. Cluster values in the FAT and directory entries are relative to this. 
	//This is the cluster immediately after the FAT
	u32	alloc_end;//8135	The cluster after the highest allocatable cluster. Relative to alloc_offset. Not used.
	u32	rootdir_cluster;//0	First cluster of the root directory. Relative to alloc_offset. Must be zero.
	u32	backup_block1;//	word	1023	Erase block used as a backup area during programming. 
	//Normally the the last block on the card, it may have a different value if that block was found to be bad.
	u32	backup_block2;//1022	This block should be erased to all ones. Normally the the second last block on the card.
	u8	unused1[8];
	u32	indir_fat_clusters[32];//8	List of indirect FAT clusters. On a standard 8M card there's only one indirect FAT cluster.
	u32	bad_block_list[32];//	-1	List of erase blocks that have errors and shouldn't be used. 
	u8	card_type;//2	Memory card type. Must be 2, indicating that this is a PS2 memory card.
	u8	card_flags;//0x52	Physical characteristics of the memory card.
	u16	unused2;                  //  zero
	u32	cluster_size;             //  1024 always, 0x400
	u32	unused3;                  //  0x100
	u32	size_in_megs;             //  size in megabytes
	u32	unused4;                  //  0xffffffff
	u8	unused5[ 12 ];            //  zero
	u32	max_used;                 //  97%of total clusters
	u8	unused6[ 8 ];             //  zero
	u32	unused7;                  //  0xffffffff
};

struct SPs2DateTime
{
	u8	unused0;
	u8	sec;//	byte	seconds
	u8	min;//	byte	minutes
	u8	hour;//	byte	hours
	u8	day;//	byte	day of the month
	u8	month;//	byte	month (1-12)
	u16	year;//	word	year
};

struct SPs2DirEntry
{
	u16	mode;//See directory mode table. 
	u16 unused0;
	u32	length;//Length in bytes if a file, or entries if a directory. 
	SPs2DateTime created;//Creation time. 
	u32	cluster;//First cluster of the file, or 0xFFFFFFFF for an empty file. In "." entries this the first cluster of this directory's parent directory instead. Relative to alloc_offset. 
	u32	dir_entry;//Only in "." entries. Entry of this directory in its parent's directory. 
	SPs2DateTime modified;
	u32	attr;//User defined attribute 
	u8  unused1[ 28 ];   //  - 
	char	name[32];//	Zero terminated name for this directory entry.
	u8	unused2[ 416 ];
};

struct SPsu_header
{                  //Offs:  Example content
	u16 attr;                       //0x00:  0x8427  (=normal folder, 8497 for normal file)
	u16 unknown_1_u16;              //0x02:  2 zero bytes
	u32 size;                       //0x04:  header_count-1, file size, 0 for pseudo
	SPs2DateTime	cTime;                  //0x08:  8 bytes creation timestamp (struct above)
	u64 EMS_used_u64;               //0x10:  8 zero bytes (but used by EMS)
	SPs2DateTime mTime;                  //0x18:  8 bytes modification timestamp (struct above)
	u64 unknown_2_u64;              //0x20:  8 bytes from mcTable
	u8  unknown_3_24_bytes[24];     //0x28:  24 zero bytes
	u8  name[32];                   //0x40:  32 name bytes, padded with zeroes
	u8  unknown_4_416_bytes[0x1A0]; //0x60:  zero byte padding to reach 0x200 size
};                     //0x200: End of psu_header struct

#pragma pack(pop)

//  Directory Entry Mode Flags
#define DF_READ          0x0001  //     Read permission. 
#define DF_WRITE         0x0002  //     Write permission. 
#define DF_EXECUTE       0x0004  //     Execute permission. 
#define DF_PROTECTED     0x0008  //     Directory is copy protected. 
#define DF_FILE          0x0010  //     Regular file. 
#define DF_DIRECTORY     0x0020  //     Directory. 
#define DF_040           0x0040  //     Used internally to create directories. 
#define DF_080           0x0080  //     Copied? 
#define DF_0100          0x0100  //     - 
#define O_CREAT          0x0200  //     Used to create files. 
#define DF_0400          0x0400  //     Set when files and directories are created, otherwise ignored.
#define DF_POCKETSTN     0x0800  //     PocketStation application save file. 
#define DF_PSX           0x1000  //     PlayStation save file. 
#define DF_HIDDEN        0x2000  //     File is hidden. 
#define DF_04000         0x4000  //     - 
#define DF_EXISTS        0x8000  //     This entry is in use. If this flag is clear, then the file or directory has been deleted. 
