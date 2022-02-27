// CMainView.h : CMainView ��Ľӿ�
//


#pragma once
#include "CsutomBtn.h"
#include "ColorStatic.h"
#include "RectWnd.h"
#include "CPlcCtrlDoc.h"
#include "ListCtrlCl.h"
#include "TsWnd.h"

const UINT  MAX_NUM = 8;

class CMainView : public CFormView
{
protected: // �������л�����
	CMainView();
	DECLARE_DYNCREATE(CMainView)

public:
	enum{ IDD = IDD_CPLCCTRL_FORM };

// ����
public:
	CCPlcCtrlDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CMainView();

	void DevUpdate(UINT nDevCode,UINT nUpdateId);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	CCsutomBtn		m_ExitBtn;
	CCsutomBtn		m_ConnectBtn;
	CColorStatic	m_TimeStatic;
	CColorStatic	m_CapStatic;
	CColorStatic	m_ShowStatic1;
	CColorStatic	m_ShowStatic2;
	CColorStatic	m_ShowStatic3;
	HBITMAP			m_RedBMP;
	HBITMAP			m_YellowBMP;
	HBITMAP			m_OrangeBMP;
	HBITMAP			m_GrayBMP;
	HBITMAP			m_GreenBMP;

	CRectWnd		m_ShowWnd[3];
	CListCtrlCl		m_MsgList;
	CColorStatic	m_MsgCap;
	LOGFONT			m_lNormalFont;
	LOGFONT			m_lShowFont;
	LOGFONT			m_lTimeFont;
	LOGFONT			m_lCapFont;
	CPen			penSlow;
	CPen			penBlack;

	UINT			m_ErrNum;
	CString			m_strRcdFormat;
	//ͨ��״̬
	CColorStatic	m_TxCap;
	CStatic			m_TxStatic;
	//�ۺ��쳣
	CColorStatic	m_ZhCap;
	CStatic			m_ZhStatic;
	//������
	CColorStatic	m_LockCap;
	CStatic			m_LockStatic;
	//PLC����
	CColorStatic	m_PLCRunCap;
	CStatic			m_PLCRunStatic;

	CTsWnd			m_TsWnd;
	UINT			m_PlaySoundFlag;
	UINT			m_CapFtSize;
	UINT			m_TimeFtSize;
	UINT			m_NormalFtSize;
	UINT			m_ShowFtSize;
	CString			m_StrTitle;
	int				m_TitleWidth;
	int 			m_TimeWidth;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBtnExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnConnect();
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // CMainView.cpp �еĵ��԰汾
inline CCPlcCtrlDoc* CMainView::GetDocument() const
   { return reinterpret_cast<CCPlcCtrlDoc*>(m_pDocument); }
#endif

