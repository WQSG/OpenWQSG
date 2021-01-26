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
#include "stdafx.h"
#include "xmlStorage.h"

#if USE_XML
#include "../3rd/rapidxml/rapidxml.hpp"
#include "../3rd/rapidxml/rapidxml_print.hpp"
#endif //USE_XML


#if USE_XML
static const CStringW g_strConfigRootName = L"WQSG汉化配置2.0";

class CXmlAllocator
{
	rapidxml::xml_document<>& m_doc;
public:
	CXmlAllocator(rapidxml::xml_document<>& a_doc);

	rapidxml::xml_node<>* CreateNode(const WCHAR* a_pName);
	rapidxml::xml_attribute<>* CreateAttribute(const WCHAR* a_pName, const WCHAR* a_pVal);
};

static const char* GetXmlAttribute(const rapidxml::xml_node<>* a_pNode, const char* a_pName)
{
	const rapidxml::xml_attribute<>* pAttr = a_pNode->first_attribute( a_pName );
	return pAttr ? pAttr->value() : NULL;
}

CXmlAllocator::CXmlAllocator(rapidxml::xml_document<>& a_doc)
: m_doc(a_doc)
{
}

rapidxml::xml_node<>*
CXmlAllocator::CreateNode(const WCHAR* a_pName)
{
	const CStringA strName = Utf16le2Utf8(a_pName);
	return m_doc.allocate_node(rapidxml::node_type::node_element, m_doc.allocate_string( strName.GetString() ));
}

rapidxml::xml_attribute<>*
CXmlAllocator::CreateAttribute(const WCHAR* a_pName, const WCHAR* a_pVal)
{
	const CStringA strName = Utf16le2Utf8(a_pName);
	const CStringA strVal = Utf16le2Utf8(a_pVal);
	return m_doc.allocate_attribute(m_doc.allocate_string( strName.GetString() ), m_doc.allocate_string( strVal.GetString() ));
}

class CXmlStorageVersion
{
private:
	CXmlStorageVersion();

	static void _CopyAttribute(rapidxml::xml_node<>& a_DstNode, const rapidxml::xml_node<>& a_SrcNode, CXmlAllocator& a_xml, const WCHAR* a_pName);
public:
	static void Ver1d0To2d0(rapidxml::xml_document<>& a_XmlDoc);
};

void CXmlStorageVersion::_CopyAttribute(rapidxml::xml_node<>& a_DstNode, const rapidxml::xml_node<>& a_SrcNode, CXmlAllocator& a_xml, const WCHAR* a_pName)
{
	const char* pAttrVal = GetXmlAttribute(&a_SrcNode, Utf16le2Utf8(a_pName));
	if(pAttrVal)
	{
		rapidxml::xml_attribute<>* pAttr = a_xml.CreateAttribute(a_pName, Utf82Utf16le(pAttrVal));
		a_DstNode.append_attribute(pAttr);
	}
}

void CXmlStorageVersion::Ver1d0To2d0( rapidxml::xml_document<>& a_XmlDoc )
{
	rapidxml::xml_node<>*const pOldRoot = a_XmlDoc.first_node();
	CXmlAllocator xml(a_XmlDoc);

	rapidxml::xml_node<>* pNewRoot = xml.CreateNode(g_strConfigRootName);

	const rapidxml::xml_node<>* pNode;

	pNode = pOldRoot->first_node( "Config" );
	while( pNode )
	{
		const char* pAttribute = GetXmlAttribute(pNode, "ConfigName" );
		if( pAttribute && 0 == strcmp( pAttribute , "export" ) )
		{
			rapidxml::xml_node<>*const pNewExport = xml.CreateNode(L"Export");
			pNewRoot->append_node(pNewExport);

			pNode = pNode->first_node();

			while( pNode )
			{
				rapidxml::xml_node<>*const newItem = xml.CreateNode(L"ExportItem");
				pNewExport->append_node(newItem);

				newItem->append_attribute( xml.CreateAttribute(L"Name", Utf82Utf16le(pNode->value())) );
				_CopyAttribute(*newItem, *pNode, xml, L"ROMPath" );
				_CopyAttribute(*newItem, *pNode, xml, L"TXTPath" );
				_CopyAttribute(*newItem, *pNode, xml, L"TBLPathName" );
				_CopyAttribute(*newItem, *pNode, xml, L"TBL2PathName" );
				_CopyAttribute(*newItem, *pNode, xml, L"ExtName" );
				_CopyAttribute(*newItem, *pNode, xml, L"SegmentAddr" );
				_CopyAttribute(*newItem, *pNode, xml, L"BeginOffset" );
				_CopyAttribute(*newItem, *pNode, xml, L"EndOffset" );
				_CopyAttribute(*newItem, *pNode, xml, L"MinLen" );
				_CopyAttribute(*newItem, *pNode, xml, L"MaxLen" );
				_CopyAttribute(*newItem, *pNode, xml, L"UseDirectory" );
				_CopyAttribute(*newItem, *pNode, xml, L"CheckTblOverlap" );
				_CopyAttribute(*newItem, *pNode, xml, L"UseTBL2" );
				_CopyAttribute(*newItem, *pNode, xml, L"TxtDirDefault" );
				_CopyAttribute(*newItem, *pNode, xml, L"SubDir" );

				pNode = pNode->next_sibling();
			}

			break;
		}

		pNode = pNode->next_sibling( "Config" );
	}

	pNode = pOldRoot->first_node( "Config" );
	while( pNode )
	{
		const char* pAttribute = GetXmlAttribute(pNode, "ConfigName");
		if( pAttribute && 0 == strcmp( pAttribute , "import" ) )
		{
			rapidxml::xml_node<>*const pNewImport = xml.CreateNode(L"Import");
			pNewRoot->append_node(pNewImport);

			pNode = pNode->first_node();
#if 0
			while( pNode )
			{
				const rapidxml::xml_node<>* newItem = xml.CreateNode(L"ImportItem");
				pNewImport->append_node(newItem);

				newItem->SetAttribute( "Name" , pNode->Value() );
				// 				_CopyAttribute(*newItem, *pNode, xml, "ROMPath" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "TXTPath" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "TBLPathName" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "TBL2PathName" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "ExtName" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "SegmentAddr" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "BeginOffset" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "EndOffset" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "MinLen" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "MaxLen" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "UseDirectory" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "CheckTblOverlap" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "UseTBL2" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "TxtDirDefault" );
				// 				_CopyAttribute(*newItem, *pNode, xml, "SubDir" );

				pNode = pNode->NextSiblingElement();
			}
#endif
			break;
		}

		pNode = pNode->next_sibling( "Config" );
	}

	a_XmlDoc.remove_node(pOldRoot);
	a_XmlDoc.append_node( pNewRoot );
}
//-----------------------------------
static CStringW SafeGetAttr(const rapidxml::xml_node<>* a_pNode, const WCHAR* a_pName, const WCHAR* a_pDefault)
{
	const char* pUtf8 = GetXmlAttribute(a_pNode, Utf16le2Utf8(a_pName));
	return pUtf8 ? Utf82Utf16le(pUtf8) : a_pDefault;
}
//-----------------------------------
static void _LoadImportData(rapidxml::xml_node<>& a_Root)
{
	CGlobalData::m_ImportDatas.clear();

	const rapidxml::xml_node<>* pNode;

	pNode = a_Root.first_node( "Import" );
	if( !pNode )
		return;

	for( pNode = pNode->first_node( "ImportItem" ) ; pNode ; pNode = pNode->next_sibling( "ImportItem" ) )
	{
		const char* pNameUtf8 = GetXmlAttribute(pNode, "Name");
		if( !pNameUtf8 )
			continue;

		SImportData data;
		data.m_strItemName = Utf82Utf16le( pNameUtf8 );

		data.m_strROMPath = SafeGetAttr(pNode, L"ROMPath" , L"" );
		data.m_strTXTPath = SafeGetAttr(pNode, L"TXTPath" , L"" );
		data.m_strTBLPathName = SafeGetAttr(pNode, L"TBLPathName" , L"" );
		data.m_strTBL2PathName = SafeGetAttr(pNode, L"TBL2PathName" , L"" );
		data.m_strExtName = SafeGetAttr(pNode, L"ExtName" , L"" );
		data.m_bUseDirectory = 0 != _wtoi(SafeGetAttr(pNode, L"UseDirectory" , L"0" ));
		data.m_bCheckTblOverlap = 0 != _wtoi(SafeGetAttr(pNode, L"CheckTblOverlap" , L"1" ));
		data.m_bUseTBL2 = 0 != _wtoi(SafeGetAttr(pNode, L"UseTBL2" , L"0" ));
		data.m_bTxtDirDefault = 0 != _wtoi(SafeGetAttr(pNode, L"TxtDirDefault" , L"0" ));
		data.m_bSubDir = 0 != _wtoi(SafeGetAttr(pNode, L"SubDir" , L"0" ));

		data.m_bLenOverStop = 0 != _wtoi(SafeGetAttr(pNode, L"LenOverStop" , L"0" ));
		data.m_uFill = _wtoi(SafeGetAttr(pNode, L"Fill" , L"0" ));
		data.m_strFillByte = SafeGetAttr(pNode, L"FillByte" , L"20" );
		data.m_strFillWord = SafeGetAttr(pNode, L"FillWord" , L"8140" );

		CGlobalData::m_ImportDatas.push_back( data );
	}
}
//-----------------------------------
static void _LoadExportData(rapidxml::xml_node<>& a_Root)
{
	CGlobalData::m_ExportDatas.clear();

	const rapidxml::xml_node<>* pNode;

	pNode = a_Root.first_node( "Export" );
	if( !pNode )
		return;

	for( pNode = pNode->first_node( "ExportItem" ) ; pNode ; pNode = pNode->next_sibling( "ExportItem" ) )
	{
		const char* pNameUtf8 = GetXmlAttribute(pNode, "Name");
		if( !pNameUtf8 )
			continue;

		SExportData data;
		data.m_strItemName = Utf82Utf16le( pNameUtf8 );

		data.m_strROMPath = SafeGetAttr(pNode, L"ROMPath" , L"" );
		data.m_strTXTPath = SafeGetAttr(pNode, L"TXTPath" , L"" );
		data.m_strTBLPathName = SafeGetAttr(pNode, L"TBLPathName" , L"" );
		data.m_strTBL2PathName = SafeGetAttr(pNode, L"TBL2PathName" , L"" );
		data.m_strExtName = SafeGetAttr(pNode, L"ExtName" , L"" );
		data.m_uSegmentAddr = _wtoi(SafeGetAttr(pNode, L"SegmentAddr" , L"0" ));
		data.m_uBeginOffset = _wtoi(SafeGetAttr(pNode, L"BeginOffset" , L"" ));
		data.m_uEndOffset = _wtoi(SafeGetAttr(pNode, L"EndOffset" , L"" ));
		data.m_uMinLen = _wtoi(SafeGetAttr(pNode, L"MinLen" , L"2" ));
		data.m_uMaxLen = _wtoi(SafeGetAttr(pNode, L"MaxLen" , L"9999" ));
		data.m_bUseDirectory = 0 != _wtoi(SafeGetAttr(pNode, L"UseDirectory" , L"0" ));
		data.m_bCheckTblOverlap = 0 != _wtoi(SafeGetAttr(pNode, L"CheckTblOverlap" , L"1" ));
		data.m_bUseTBL2 = 0 != _wtoi(SafeGetAttr(pNode, L"UseTBL2" , L"0" ));
		data.m_bTxtDirDefault = 0 != _wtoi(SafeGetAttr(pNode, L"TxtDirDefault" , L"0" ));
		data.m_bSubDir = 0 != _wtoi(SafeGetAttr(pNode, L"SubDir" , L"0" ));

		CGlobalData::m_ExportDatas.push_back( data );
	}
}
//-----------------------------------
static void _SaveImportData(CXmlAllocator& a_xml, rapidxml::xml_node<>& a_Root)
{
	CXmlAllocator& xml = a_xml;

	rapidxml::xml_node<>*const pNewNode = xml.CreateNode(L"Import");
	a_Root.append_node( pNewNode );

	std::vector<SImportData>::iterator it = CGlobalData::m_ImportDatas.begin();

	for ( ; it != CGlobalData::m_ImportDatas.end() ; ++it )
	{
		const SImportData& data = *it;
		rapidxml::xml_node<>*const pNode = xml.CreateNode(L"ImportItem");
		pNewNode->append_node(pNode);

		pNode->append_attribute( xml.CreateAttribute(L"Name" , data.m_strItemName ) );
		pNode->append_attribute( xml.CreateAttribute(L"ROMPath" , data.m_strROMPath ) );
		pNode->append_attribute( xml.CreateAttribute(L"TXTPath" , data.m_strTXTPath ) );
		pNode->append_attribute( xml.CreateAttribute(L"TBLPathName" , data.m_strTBLPathName ) );
		pNode->append_attribute( xml.CreateAttribute(L"TBL2PathName" , data.m_strTBL2PathName ) );
		pNode->append_attribute( xml.CreateAttribute(L"ExtName" , data.m_strExtName ) );
		pNode->append_attribute( xml.CreateAttribute(L"UseDirectory" , X2Str( data.m_bUseDirectory )) );
		pNode->append_attribute( xml.CreateAttribute(L"CheckTblOverlap" , X2Str( data.m_bCheckTblOverlap )) );
		pNode->append_attribute( xml.CreateAttribute(L"UseTBL2" , X2Str( data.m_bUseTBL2 )) );
		pNode->append_attribute( xml.CreateAttribute(L"TxtDirDefault" , X2Str( data.m_bTxtDirDefault )) );
		pNode->append_attribute( xml.CreateAttribute(L"SubDir" , X2Str( data.m_bSubDir )) );
		//
		pNode->append_attribute( xml.CreateAttribute(L"LenOverStop" , X2Str( data.m_bLenOverStop )) );
		pNode->append_attribute( xml.CreateAttribute(L"Fill" , X2Str( data.m_uFill )) );
		pNode->append_attribute( xml.CreateAttribute(L"FillByte" , data.m_strFillByte ) );
		pNode->append_attribute( xml.CreateAttribute(L"FillWord" , data.m_strFillWord ) );
	}
}
//-----------------------------------
static void _SaveExportData(CXmlAllocator& a_xml, rapidxml::xml_node<>& a_Root)
{
	CXmlAllocator& xml = a_xml;

	rapidxml::xml_node<>*const pNewNode = xml.CreateNode(L"Export");
	a_Root.append_node( pNewNode );

	std::vector<SExportData>::iterator it = CGlobalData::m_ExportDatas.begin();

	for ( ; it != CGlobalData::m_ExportDatas.end() ; ++it )
	{
		const SExportData& data = *it;
		rapidxml::xml_node<>*const pNode = xml.CreateNode(L"ExportItem" );
		pNewNode->append_node(pNode);

		pNode->append_attribute( xml.CreateAttribute(L"Name" , data.m_strItemName ) );
		pNode->append_attribute( xml.CreateAttribute(L"ROMPath" , data.m_strROMPath ) );
		pNode->append_attribute( xml.CreateAttribute(L"TXTPath" , data.m_strTXTPath ) );
		pNode->append_attribute( xml.CreateAttribute(L"TBLPathName" , data.m_strTBLPathName ) );
		pNode->append_attribute( xml.CreateAttribute(L"TBL2PathName" , data.m_strTBL2PathName ) );
		pNode->append_attribute( xml.CreateAttribute(L"ExtName" , data.m_strExtName ) );
		pNode->append_attribute( xml.CreateAttribute(L"SegmentAddr" , X2Str( data.m_uSegmentAddr )) );
		pNode->append_attribute( xml.CreateAttribute(L"BeginOffset" , X2Str( data.m_uBeginOffset )) );
		pNode->append_attribute( xml.CreateAttribute(L"EndOffset" , X2Str( data.m_uEndOffset )) );
		pNode->append_attribute( xml.CreateAttribute(L"MinLen" , X2Str( data.m_uMinLen )) );
		pNode->append_attribute( xml.CreateAttribute(L"MaxLen" , X2Str( data.m_uMaxLen )) );
		pNode->append_attribute( xml.CreateAttribute(L"UseDirectory" , X2Str( data.m_bUseDirectory )) );
		pNode->append_attribute( xml.CreateAttribute(L"CheckTblOverlap" , X2Str( data.m_bCheckTblOverlap )) );
		pNode->append_attribute( xml.CreateAttribute(L"UseTBL2" , X2Str( data.m_bUseTBL2 )) );
		pNode->append_attribute( xml.CreateAttribute(L"TxtDirDefault" , X2Str( data.m_bTxtDirDefault )) );
		pNode->append_attribute( xml.CreateAttribute(L"SubDir" , X2Str( data.m_bSubDir )) );
	}
}

// ----------------------------------------------------
static CStringW g_strConfigSavePathName;

BOOL LoadXml()
{
	rapidxml::xml_document<> xmlDoc;
	std::vector<char> vBuffer;
	BOOL bRt = FALSE;
	do
	{
		WCHAR exePathName[ MAX_PATH * 2 ];
		if( 0 == GetModuleFileNameW( NULL , exePathName , MAX_PATH * 2 ) )
			break;

		g_strConfigSavePathName = exePathName;
		g_strConfigSavePathName = g_strConfigSavePathName.Left( g_strConfigSavePathName.ReverseFind( L'\\' ) + 1 ) + L"WQSG汉化配置.XML";
		//==============================================
		CWQSG_File fp;
		if( !fp.OpenFile( g_strConfigSavePathName.GetString() , 1 , 3 ) )
		{
			bRt = TRUE;
			break;
		}
		const n64 nFileSize = fp.GetFileSize();
		if(nFileSize <= 0 || nFileSize >= 0x7FFFFFFFll)
		{
			bRt = nFileSize == 0;
			break;
		}
		vBuffer.resize(nFileSize);

		fp.Seek(0);
		if(fp.Read(&vBuffer[0], (u32)nFileSize) != (u32)nFileSize)
		{
			break;
		}
		vBuffer.push_back('\0');

		try
		{
			char* pText = &vBuffer[0];
			xmlDoc.parse<0>(pText);
		}
		catch(rapidxml::parse_error error)
		{
			break;
		}
		
		const rapidxml::xml_node<>* pRoot = xmlDoc.first_node();
		if( !pRoot )
		{
			break;
		}

		if( pRoot->name() != Utf16le2Utf8( g_strConfigRootName  ) )
		{
			if( pRoot->name() == Utf16le2Utf8( L"WQSG汉化配置" ) )
			{
				CXmlStorageVersion::Ver1d0To2d0( xmlDoc );
				//ok
			}
			else
			{
				xmlDoc.clear();

				CXmlAllocator xml(xmlDoc);

				xmlDoc.append_node( xml.CreateNode(g_strConfigRootName) );
				//ok
			}
		}

		// ------------------
		bRt = TRUE;
		_LoadImportData(*xmlDoc.first_node());
		_LoadExportData(*xmlDoc.first_node());
	}while(0);

	return bRt;
}

BOOL SaveXml()
{
	rapidxml::xml_document<> xmlDoc;
	CXmlAllocator xml(xmlDoc);

	rapidxml::xml_node<>* pDeclaration = xmlDoc.allocate_node(rapidxml::node_type::node_declaration);
	pDeclaration->append_attribute(xml.CreateAttribute(L"version", L"1.0"));
	pDeclaration->append_attribute(xml.CreateAttribute(L"encoding", L"utf-8"));
	pDeclaration->append_attribute(xml.CreateAttribute(L"standalone", L"0"));
	xmlDoc.append_node(pDeclaration);

	rapidxml::xml_node<>* pNode = xml.CreateNode(g_strConfigRootName);
	xmlDoc.append_node(pNode);

	// ------------------------
	_SaveExportData(xml, *pNode);
	_SaveImportData(xml, *pNode);
	// ------------------------

	FILE* fileXml = _wfopen( g_strConfigSavePathName.GetString() , L"wb" );
	if( NULL == fileXml )
	{
		return FALSE;
	}

	std::string text;
    rapidxml::print(std::back_inserter(text), xmlDoc, 0);

	const BOOL bOk = fwrite(text.c_str(), text.length(), 1, fileXml) == 1;
	fclose( fileXml );

	return bOk;
}
#endif
