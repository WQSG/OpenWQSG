/*  OpenWQSG - WQSG µ¼³ö(µ¼Èë)
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

#include "stdafx.h"
#include "WQSG_cfg.h"
#if USE_XML
static TiXmlDocument	g_xmlConfig;
static CString			g_strConfigSavePathName;

static CWQSG_CriticalSection g_lock;

static inline void Xml1_0_to_2_0( TiXmlDocument& a_XmlDoc );

void InitConfig2()
{
	g_xmlConfig.Clear();
	g_xmlConfig.InsertEndChild( TiXmlDeclaration("1.0","utf-8","0") );
	g_xmlConfig.LinkEndChild( new TiXmlElement( Utf16le2Utf8( L"WQSGºº»¯ÅäÖÃ2.0" ) ) );
}

BOOL InitConfig()
{
	CConfigLockGuard guard = LockConfig();

	FILE* fileXml = NULL;
	BOOL bRt = FALSE;
	do
	{
		WCHAR exePathName[ MAX_PATH * 2 ];
		if( 0 == GetModuleFileNameW( NULL , exePathName , MAX_PATH * 2 ) )
			break;

		g_strConfigSavePathName = exePathName;
		g_strConfigSavePathName = g_strConfigSavePathName.Left( g_strConfigSavePathName.ReverseFind( L'\\' ) + 1 ) + L"WQSGºº»¯ÅäÖÃ.XML";
		//==============================================
		fileXml = _wfopen( g_strConfigSavePathName.GetString() , L"rb" );
		if( NULL == fileXml )
		{
			InitConfig2();
			bRt = TRUE;
			break;
		}

		if( !g_xmlConfig.LoadFile( fileXml , TIXML_ENCODING_UTF8 ) )
		{
			InitConfig2();
			break;
		}

		const TiXmlElement* pRoot = g_xmlConfig.FirstChildElement();
		if( !pRoot )
		{
			InitConfig2();
			break;
		}

		if( pRoot->Value() != Utf16le2Utf8( L"WQSGºº»¯ÅäÖÃ2.0"  ) )
		{
			if( pRoot->Value() == Utf16le2Utf8( L"WQSGºº»¯ÅäÖÃ" ) )
			{
				Xml1_0_to_2_0( g_xmlConfig );
				//ok
			}
			else
			{
				InitConfig2();
				//ok
			}
		}

		bRt = TRUE;
	}while(0);

	if( fileXml )
		fclose(fileXml);

	return bRt;
}

CConfigLockGuard LockConfig()
{
	return CConfigLockGuard( g_lock , *g_xmlConfig.FirstChildElement() );
}

BOOL SaveConfig()
{
	CConfigLockGuard guard = LockConfig();

	FILE* fileXml = _wfopen( g_strConfigSavePathName.GetString() , L"wb" );
	if( NULL == fileXml )
	{
		return FALSE;
	}

	const BOOL bOk = g_xmlConfig.SaveFile( fileXml );
	fclose( fileXml );

	return bOk;
}

static inline void Xml1_0_to_2_0( TiXmlDocument& a_XmlDoc )
{
	const TiXmlElement* pRoot = a_XmlDoc.FirstChildElement();

	TiXmlElement* pNewRoot = new TiXmlElement( Utf16le2Utf8( L"WQSGºº»¯ÅäÖÃ2.0" ) );

	const TiXmlElement* pNode;
#define Def_Fn_CopyAttribute( name ) if( pNode->Attribute( name ) ) newItem->SetAttribute( name , pNode->Attribute( name ) )

	pNode = pRoot->FirstChildElement( "Config" );
	while( pNode )
	{
		const char* pAttribute = pNode->Attribute( "ConfigName" );
		if( 0 == strcmp( pAttribute , "export" ) )
		{
			TiXmlElement* pNewExport = pNewRoot->LinkEndChild( new TiXmlElement( Utf16le2Utf8( L"Export" ) ) )->ToElement();

			pNode = pNode->FirstChildElement();

			while( pNode )
			{
				TiXmlElement* newItem = pNewExport->LinkEndChild( new TiXmlElement( "ExportItem") )->ToElement();

				newItem->SetAttribute( "Name" , pNode->Value() );
				Def_Fn_CopyAttribute( "ROMPath" );
				Def_Fn_CopyAttribute( "TXTPath" );
				Def_Fn_CopyAttribute( "TBLPathName" );
				Def_Fn_CopyAttribute( "TBL2PathName" );
				Def_Fn_CopyAttribute( "ExtName" );
				Def_Fn_CopyAttribute( "SegmentAddr" );
				Def_Fn_CopyAttribute( "BeginOffset" );
				Def_Fn_CopyAttribute( "EndOffset" );
				Def_Fn_CopyAttribute( "MinLen" );
				Def_Fn_CopyAttribute( "MaxLen" );
				Def_Fn_CopyAttribute( "UseDirectory" );
				Def_Fn_CopyAttribute( "CheckTblOverlap" );
				Def_Fn_CopyAttribute( "UseTBL2" );
				Def_Fn_CopyAttribute( "TxtDirDefault" );
				Def_Fn_CopyAttribute( "SubDir" );

				pNode = pNode->NextSiblingElement();
			}

			break;
		}

		pNode = pNode->NextSiblingElement( "Config" );
	}

	pNode = pRoot->FirstChildElement( "Config" );
	while( pNode )
	{
		const char* pAttribute = pNode->Attribute( "ConfigName" );
		if( 0 == strcmp( pAttribute , "import" ) )
		{
			TiXmlElement* pNewImport = pNewRoot->LinkEndChild( new TiXmlElement( Utf16le2Utf8( L"Import" ) ) )->ToElement();

			pNode = pNode->FirstChildElement();

			while( pNode )
			{
				TiXmlElement* newItem = pNewImport->LinkEndChild( new TiXmlElement("ImportItem") )->ToElement();

				newItem->SetAttribute( "Name" , pNode->Value() );
				// 				Def_Fn_CopyAttribute( "ROMPath" );
				// 				Def_Fn_CopyAttribute( "TXTPath" );
				// 				Def_Fn_CopyAttribute( "TBLPathName" );
				// 				Def_Fn_CopyAttribute( "TBL2PathName" );
				// 				Def_Fn_CopyAttribute( "ExtName" );
				// 				Def_Fn_CopyAttribute( "SegmentAddr" );
				// 				Def_Fn_CopyAttribute( "BeginOffset" );
				// 				Def_Fn_CopyAttribute( "EndOffset" );
				// 				Def_Fn_CopyAttribute( "MinLen" );
				// 				Def_Fn_CopyAttribute( "MaxLen" );
				// 				Def_Fn_CopyAttribute( "UseDirectory" );
				// 				Def_Fn_CopyAttribute( "CheckTblOverlap" );
				// 				Def_Fn_CopyAttribute( "UseTBL2" );
				// 				Def_Fn_CopyAttribute( "TxtDirDefault" );
				// 				Def_Fn_CopyAttribute( "SubDir" );

				pNode = pNode->NextSiblingElement();
			}

			break;
		}

		pNode = pNode->NextSiblingElement( "Config" );
	}
#undef Def_Fn_CopyAttribute

	a_XmlDoc.Clear();
	a_XmlDoc.InsertEndChild( TiXmlDeclaration("1.0","utf-8","0") );
	a_XmlDoc.LinkEndChild( pNewRoot );
}
#endif
