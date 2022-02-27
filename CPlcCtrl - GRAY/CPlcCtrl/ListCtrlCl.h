#if !defined(AFX_LISTCTRLCL_H__8D433443_A56A_4112_BC69_7E9504D5012D__INCLUDED_)
#define AFX_LISTCTRLCL_H__8D433443_A56A_4112_BC69_7E9504D5012D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlCl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCl window
#include "HeaderCtrlCl.h"
class CListCtrlCl : public CListCtrl
{
// Construction
	DECLARE_DYNAMIC(CListCtrlCl)
public:
	CListCtrlCl();

// Attributes
public:
	CPtrList m_ptrListCol;  //保存列颜色
	CPtrList m_ptrListItem; //保存Item颜色表
	CPtrList m_colTextColor; //保存列字体颜色
	CPtrList m_ItemTextColor; //保存单元格字体颜色
	COLORREF m_color;
	int m_nRowHeight;
	// 字体高度
	int m_fontHeight;
	// 字体宽度
	int m_fontWith;
	CHeaderCtrlCl m_Header;

// Operations
public:
	BOOL SetTextColor(COLORREF cr);
	// 设置表头高度
	void SetHeaderHeight(float Height);
	// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);
	bool FindColColor(int col ,COLORREF &color); //查找列颜色
	bool FindItemColor(int col,int row,COLORREF &color);
	bool FindColTextColor(int col,COLORREF &color); //查找列字体颜色
	bool FindItemTextColor(int col,int row,COLORREF &color);
	void SetColColor(int col,COLORREF color);  //设置列颜色
	void SetItemColor(int col,int row,COLORREF color); //设置Item颜色
	void SetColTextColor(int col,COLORREF color);   //设置列文本颜色
	void SetItemTextColor(int col,int row,COLORREF color);
	void SetRowHeigt(int nHeight); //设置行高
	void SetHeaderFontHW(int nHeight,int nWith); //设置表头字体大小
	void SetHeaderTextColor(COLORREF color);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	void RemoveAllPtr(CPtrList *pList);
	void SetFontHW(int nHeight,int nWith);  //设置字体的高和宽

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlCl)
	public:
	protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
// Implementation
public:
	void SetListSelect(BOOL bSelect=FALSE);
	BOOL m_bUpdate;
	BOOL m_bSelect;
	void SetListUpdate(BOOL bUpdate=FALSE);
	virtual ~CListCtrlCl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlCl)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLCL_H__8D433443_A56A_4112_BC69_7E9504D5012D__INCLUDED_)
