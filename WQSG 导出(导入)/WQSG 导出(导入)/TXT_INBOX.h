/*  OpenWQSG - WQSG ����(����)
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
#include "afxwin.h"


// CTXT_INBOX �Ի���

struct SImportDataEx
{
	HWND		m_hWnd;
	std::vector<CStringW> m_Files;

	BOOL		m_���ֽ�;
	u8			m_SP1;
	BOOL		m_˫�ֽ�;
	u16			m_SP2;

	//----------------------------
	CString		m_strROMPath;
	CString		m_strTXTPath;
	CString		m_strTBLPathName;
	CString		m_strTBL2PathName;
	CString		m_strExtName;
	BOOL		m_bCheckTblOverlap;
	BOOL		m_bSubDir;
	BOOL		m_bLenOverStop;

	SImportDataEx()
	{
	}
};

class CTXT_INBOX : public CBaseDialog
{
	DECLARE_DYNAMIC(CTXT_INBOX)

	SImportDataEx m_Data;

	CString m_EDIT_ROMPath;
	CString m_EDIT_TXTPath;
	CString m_EDIT_TBLPathName;
	CString m_EDIT_TBL2PathName;
	CButton m_cCheckTblOverlap;
	CButton m_cUseDirectory;
	CButton m_cLenOverStop;
	CComboBox m_cFill;
	CString m_EDIT_SP1;
	CString m_EDIT_SP2;
	CButton m_cUseTBL2;
	CString m_EDIT_ExtName;
	CString m_LOG;
	CEdit m_CEDIT_LOG;
	CString m_NodeName;
	CListBox m_CList;
	CButton m_C�ı���ͬĿ¼;
	CButton m_cSubDir;

public:
	CTXT_INBOX(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTXT_INBOX();

// �Ի�������
	enum { IDD = IDD_TXT_I };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	afx_msg LRESULT �߳���Ϣ( WPARAM ���� , LPARAM �ı� );
	afx_msg LRESULT �߳�LOG( WPARAM ���� , LPARAM �ı� );

	void AppLog(CString str);
	void UpdateImportData(SImportData& a_data);

	void LoadFromData();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButtonRom();
	afx_msg void OnBnClickedButtonTxtDir();
	afx_msg void OnBnClickedButtonTbl();
	afx_msg void OnBnClickedButtonTbl2();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnEnChangeEditSp1();
	afx_msg void OnEnChangeEditSp2();
	afx_msg void OnEnKillfocusEditSp1();
	afx_msg void OnEnKillfocusEditSp2();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnChangeEditExt();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnEnKillfocusEditName();
	afx_msg void OnBnClickedCheck3();
};
