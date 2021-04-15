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

// DirSizeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <vector>

class CItemInfo
{
public:
	CStringW m_strName;
	bool m_bDir;

	LONGLONG m_nSize;

	size_t m_Index;

	explicit CItemInfo()
		: m_strName()
		, m_bDir(false)
		, m_nSize(0)
		, m_Index(0)
	{
	}

	CItemInfo(const CItemInfo& info)
	{
		*this = info;
	}

	CItemInfo& operator=(const CItemInfo& info)
	{
		this->m_strName = info.m_strName;
		this->m_bDir = info.m_bDir;
		this->m_nSize = info.m_nSize;
		this->m_Index = info.m_Index;
		return *this;
	}

	friend bool operator<( _In_ const CItemInfo& str1, _In_ const CItemInfo& str2 )
	{
		return str1.m_nSize < str2.m_nSize;
	}
	friend bool operator>( _In_ const CItemInfo& str1, _In_ const CItemInfo& str2 )
	{
		return str1.m_nSize > str2.m_nSize;
	}
};

typedef std::vector<CItemInfo> TItemInfos;
class CDirNode
{
public:
	TItemInfos m_Infos;
	LONGLONG m_AllSize;

	explicit CDirNode()
		: m_Infos()
		, m_AllSize(0)
	{
	}

	CDirNode(const CDirNode& node)
	{
		*this = node;
	}

	CDirNode& operator=(const CDirNode& info)
	{
		this->m_Infos = info.m_Infos;
		this->m_AllSize = info.m_AllSize;
		return *this;
	}
};

class CPathNode
{
public:
	size_t m_Index;
	CString m_strName;
};

typedef std::vector<CDirNode> TDirs;
// CDirSizeDlg �Ի���
class CDirSizeDlg : public CDialog
{
	TDirs m_Dirs;
	std::vector<CPathNode> m_Path;
	CString m_strBasePath;
	CString m_strFullPath;
// ����
public:
	CDirSizeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DIRSIZE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
protected:
	virtual void OnOK();
	virtual void OnCancel();
private:
	CImageList m_imageList;
	//CListCtrl m_cList;
	CString m_LastSelDir;

	CWQSGList m_WList;

	BOOL Find( CString a_strPath , const size_t a_DirIndex );
	BOOL Open( CString a_strPath );
	void Clear();

	void UpdateUI();
public:
	afx_msg void OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDirUp();
	afx_msg void OnFileOpenDir();
	afx_msg void OnAboutAbout();
};
