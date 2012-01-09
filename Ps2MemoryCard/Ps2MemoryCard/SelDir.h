#pragma once
#include "afxwin.h"


// CSelDir 对话框

class CSelDir : public CDialog
{
	DECLARE_DYNAMIC(CSelDir)

	CString m_strDefSel;
	CString m_strExeDir;
public:
	CSelDir( const CString& a_strDefSel , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelDir();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEL_DIR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	const CString& GetSel()const
	{
		return m_strDefSel;
	}
private:
	CListBox m_cDir;
};
