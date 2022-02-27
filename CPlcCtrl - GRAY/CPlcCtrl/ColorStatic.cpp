// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_TextColor= GREEN;
	m_BackColor= BACKCOLOR;
	m_strCaption=_T("");
	memset(&m_lFont,0,sizeof(m_lFont));
	m_Alignment=DT_LEFT;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers



int CColorStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	

	return 0;
}

void CColorStatic::OnPaint() 
{
/*	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	
	// TODO: Add your message handler code here
	CRect rect;

	GetClientRect(&rect);
	dc.SetBkColor(m_BackColor);
	dc.SetBkMode(TRANSPARENT);
	CFont *pFont=NULL;
	if(m_lFont.lfHeight==0)
		pFont=GetParent()->GetFont();
	else
	{
		pFont= new CFont;
		pFont->CreateFontIndirect(&m_lFont);
	}
	if(m_strCaption.Compare("")==0)
	{
		this->GetWindowText(m_strCaption);
	}
	CFont *pOldFont;
	pOldFont=dc.SelectObject(pFont);
	dc.SetTextColor(m_TextColor);
	dc.DrawText(m_strCaption,&rect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	dc.SelectObject(pOldFont);
	if (pFont)
	{
		delete pFont;
		pFont=NULL;

	}*/
	
	CPaintDC dc(this); // device context for painting
	CDC MemDC;
	CBitmap MemBitmap;
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	MemDC.CreateCompatibleDC(&dc);

	CBitmap *pOldBitmap=MemDC.SelectObject(&MemBitmap);
	MemDC.FillRect(&rect,&CBrush(m_BackColor));
	MemDC.SetBkColor(m_BackColor);
	MemDC.SetBkMode(TRANSPARENT);
	CFont *pFont=NULL;
	if(m_lFont.lfHeight==0)
		pFont=GetParent()->GetFont();
	else
	{
		pFont= new CFont;
		pFont->CreateFontIndirect(&m_lFont);
	}
	if(m_strCaption.Compare("")==0)
	{
		this->GetWindowText(m_strCaption);
	}
	CFont *pOldFont;
	pOldFont=MemDC.SelectObject(pFont);
	MemDC.SetTextColor(m_TextColor);
	MemDC.DrawText(m_strCaption,&rect,m_Alignment|DT_SINGLELINE|DT_VCENTER);
	dc.BitBlt(0,0,rect.Width(),	rect.Height(),&MemDC,0,0,SRCCOPY);
	MemDC.SelectObject(pOldBitmap); 
	MemDC.SelectObject(pOldFont);
	
	if(m_lFont.lfHeight!=0)
	{
		delete pFont;
		pFont = NULL;
		
	}

	// Do not call CStatic::OnPaint() for painting messages
}

void CColorStatic::Create(CString strCaption, COLORREF BackColor)
{
	
	
}

void CColorStatic::SetCaption(CString strCaption)
{
	m_strCaption=strCaption;
	Invalidate();
}

void CColorStatic::SetBackColor(COLORREF BackColor)
{
	m_BackColor=BackColor;
}

void CColorStatic::SetTextColor(COLORREF TextColor)
{
	m_TextColor=TextColor;
	Invalidate();
}

void CColorStatic::SetTextFont(LOGFONT lFont)
{
	memcpy(&m_lFont,&lFont,sizeof(lFont));
}

void CColorStatic::SetAlignment(UINT type)
{
	m_Alignment=type;

}
