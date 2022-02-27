#if !defined(AFX_HEADERCTRLCL_H__4711C52D_1D82_4412_A5EC_52A4D5F44B1D__INCLUDED_)
#define AFX_HEADERCTRLCL_H__4711C52D_1D82_4412_A5EC_52A4D5F44B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlCl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlCl window

class CHeaderCtrlCl : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlCl();
	DECLARE_DYNAMIC(CHeaderCtrlCl)
// Attributes
public:
	int m_R;
	int m_G;
	int m_B;
	int m_Gradient;	// �����屳��������ϵ��
	float m_Height;  //��ͷ�߶ȣ����Ǳ���,
	int m_fontHeight; //����߶�
	int m_fontWith;   //������
	COLORREF m_color;
	CStringArray m_HChar;
	CString m_Format; //��ʾ�������͵���������,0��ʾ����룬1��ʾ�м���룬2��ʾ�Ҷ���

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlCl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeaderCtrlCl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlCl)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLCL_H__4711C52D_1D82_4412_A5EC_52A4D5F44B1D__INCLUDED_)
