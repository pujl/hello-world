#pragma once


// CExitInfoDlg 对话框
#include "ColorStatic.h"
#include "CsutomBtn.h"
#include "resource.h"

//选择对话框
//确定  取消或者退出  取消

class CExitInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitInfoDlg)

public:
	CExitInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExitInfoDlg();
public:
	void SetDlgType(SELDLG_STYLE type);
	void SetCaptionStr(LPCSTR pStr);
	void SetPromptStr(LPCSTR pStr);
private:
	CColorStatic	m_PromptContent_Edit;
	CCsutomBtn m_SelBtn[2];//设置选择按钮
	CString PromptStr;
	CString CaptionStr;
	SELDLG_STYLE m_DlgType;
	LOGFONT		m_CaplFont;

// 对话框数据
	enum { IDD = IDD_EXITINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
