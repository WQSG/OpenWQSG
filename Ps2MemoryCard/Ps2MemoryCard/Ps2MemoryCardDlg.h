
// Ps2MemoryCardDlg.h : 头文件
//

#pragma once

#include "VPscMc.h"
#include "afxcmn.h"

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
	CVPscMc m_Mc;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonOpenMc();
	afx_msg void OnBnClickedButtonSaveMc();
	afx_msg void OnBnClickedButtonImportPsu();

	void UpdateUI();
private:
	CListCtrl m_cList;
public:
	afx_msg void OnBnClickedButtonExportPsu();
};
