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
	CPtrList m_ptrListCol;  //��������ɫ
	CPtrList m_ptrListItem; //����Item��ɫ��
	CPtrList m_colTextColor; //������������ɫ
	CPtrList m_ItemTextColor; //���浥Ԫ��������ɫ
	COLORREF m_color;
	int m_nRowHeight;
	// ����߶�
	int m_fontHeight;
	// ������
	int m_fontWith;
	CHeaderCtrlCl m_Header;

// Operations
public:
	BOOL SetTextColor(COLORREF cr);
	// ���ñ�ͷ�߶�
	void SetHeaderHeight(float Height);
	// Gradient - ����ϵ�������屳����,���ý�����Ϊ0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);
	bool FindColColor(int col ,COLORREF &color); //��������ɫ
	bool FindItemColor(int col,int row,COLORREF &color);
	bool FindColTextColor(int col,COLORREF &color); //������������ɫ
	bool FindItemTextColor(int col,int row,COLORREF &color);
	void SetColColor(int col,COLORREF color);  //��������ɫ
	void SetItemColor(int col,int row,COLORREF color); //����Item��ɫ
	void SetColTextColor(int col,COLORREF color);   //�������ı���ɫ
	void SetItemTextColor(int col,int row,COLORREF color);
	void SetRowHeigt(int nHeight); //�����и�
	void SetHeaderFontHW(int nHeight,int nWith); //���ñ�ͷ�����С
	void SetHeaderTextColor(COLORREF color);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	void RemoveAllPtr(CPtrList *pList);
	void SetFontHW(int nHeight,int nWith);  //��������ĸߺͿ�

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
