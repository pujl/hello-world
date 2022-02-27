
#pragma once

// CsutomBtn.h : header file
//��ťfactory ����
//��ť����Ϊ ��ͨ��ť�����ְ�ť�͸����������ּ���Ĭ����ͨ��ť
//��ť״̬��Ϊ ������ť�ͳ�����ť

/////////////////////////////////////////////////////////////////////////////
// CCsutomBtn window
const UINT BTN_CLICKED_MSG = WM_USER+0x104;

class CCsutomBtn : public CButton
{
// Construction
public:
	CCsutomBtn();

// Attributes
private:
	UINT m_BtnId;
	BOOL m_bDown;//�Ƿ��ڳ���״̬

// Operations
private:
	CBitmap m_Bitmap[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCsutomBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//BOOL IsJamNum();//�Ƿ��Ǹ������ּ�
	BOOL GetPressDown();
	void SetPressDown(BOOL AlwaysDown);//����״̬��ť
	UINT GetBtnId();
	BTN_STYLE GetBtnType();
	UINT GetCmdId();
	void SetBtnId(UINT nId);
	void SetBitmaps(UINT nBitmapIn, UINT nBitmapOut,UINT nBitmapDisable);
	virtual ~CCsutomBtn();

	// Generated message map functions
protected:
	BOOL m_bPress;		//�Ƿ񱻰���

protected:
	//{{AFX_MSG(CCsutomBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


