
#pragma once

// CsutomBtn.h : header file
//按钮factory 介绍
//按钮类别分为 普通按钮和数字按钮和干扰引导数字键，默认普通按钮
//按钮状态分为 正常按钮和常亮按钮

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
	BOOL m_bDown;//是否处于常按状态

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
	//BOOL IsJamNum();//是否是干扰数字键
	BOOL GetPressDown();
	void SetPressDown(BOOL AlwaysDown);//常按状态按钮
	UINT GetBtnId();
	BTN_STYLE GetBtnType();
	UINT GetCmdId();
	void SetBtnId(UINT nId);
	void SetBitmaps(UINT nBitmapIn, UINT nBitmapOut,UINT nBitmapDisable);
	virtual ~CCsutomBtn();

	// Generated message map functions
protected:
	BOOL m_bPress;		//是否被按下

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


