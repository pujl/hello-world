#pragma once


// CExitInfoDlg �Ի���
#include "ColorStatic.h"
#include "CsutomBtn.h"
#include "resource.h"

//ѡ��Ի���
//ȷ��  ȡ�������˳�  ȡ��

class CExitInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitInfoDlg)

public:
	CExitInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExitInfoDlg();
public:
	void SetDlgType(SELDLG_STYLE type);
	void SetCaptionStr(LPCSTR pStr);
	void SetPromptStr(LPCSTR pStr);
private:
	CColorStatic	m_PromptContent_Edit;
	CCsutomBtn m_SelBtn[2];//����ѡ��ť
	CString PromptStr;
	CString CaptionStr;
	SELDLG_STYLE m_DlgType;
	LOGFONT		m_CaplFont;

// �Ի�������
	enum { IDD = IDD_EXITINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
