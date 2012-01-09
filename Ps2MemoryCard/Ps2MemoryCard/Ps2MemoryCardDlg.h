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

// Ps2MemoryCardDlg.h : 头文件
//

#pragma once

#include "VPscMc.h"
#include "afxcmn.h"
#include <hash_map>
#include <string>

typedef stdext::hash_map<std::string,std::vector<u8>> TPsuFile;

struct SPsuData
{
	CStringA m_strName;
	TPsuFile m_files;
};

struct SItemSortData
{
	CStringW m_strTitleName;
	n32 m_nSize;
	CStringW m_strFileName;
};

// CPs2MemoryCardDlg 对话框
class CPs2MemoryCardDlg : public CDialog
{
// 构造
public:
	CPs2MemoryCardDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PS2MEMORYCARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CVPscMc m_Mc1;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonOpenMc1();
	afx_msg void OnBnClickedButtonCreateMc1();
	afx_msg void OnBnClickedButtonSaveMc1();
	afx_msg void OnBnClickedButtonSaveBin1();
	afx_msg void OnBnClickedButtonImportPsu1();
	afx_msg void OnBnClickedButtonExportPsu1();

	afx_msg void OnBnClickedButtonSelPsuDir();

	void UpdateMcUI();
	void UpdateDirUI();
	void UIInsert( CListCtrl& a_cList , const SItemSortData& a_Data );
	void UISort( CListCtrl& a_cList );
private:
	CListCtrl m_cList1;
	CListCtrl m_cList2;
public:
	afx_msg void OnBnClickedButtonAbout();

	bool Load_Psu( const CStringW& a_strFile , SPsuData& a_Files );
	afx_msg void OnBnClickedButton1();
	afx_msg void OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitemList(NMHDR *pNMHDR, LRESULT *pResult);

};
