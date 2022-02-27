// TsWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CPlcCtrl.h"
#include "TsWnd.h"
#include "MainFrm.h"
#include "DicnaryQry.h"
#include <math.h>

// CTsWnd

IMPLEMENT_DYNAMIC(CTsWnd, CWnd)

CTsWnd::CTsWnd()
{
	ReadIniFile();
	penWhite.CreatePen(PS_SOLID,1,WHITE);
	penRed.CreatePen(PS_SOLID,1,RED);
	penBlue.CreatePen(PS_SOLID,1,BACKCOLOR);
	penYellow.CreatePen(PS_SOLID,1,YELLOW);
	penSlow.CreatePen(PS_SOLID,1,SLOWLV);
	penBlack.CreatePen(PS_SOLID,1,RGB(0,0,0));
	penGreen.CreatePen(PS_SOLID,1,GREEN);

	m_Font.CreateFont(0-m_StripFtSize,0,0,0,FW_HEAVY,FALSE,FALSE,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,VARIABLE_PITCH|FF_ROMAN,"黑体");
	m_bPlanStop = TRUE;

	for(int i =0;i<8;i++)
	{
		m_eCarStates[i] = INIT_STATE;
	}
	m_nStripWidth = 0;
	m_nStripHigh = 0;
}

CTsWnd::~CTsWnd()
{
}

BEGIN_MESSAGE_MAP(CTsWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTsWnd 消息处理程序
void CTsWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	DrawCS(&dc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CTsWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	//记录绘制区域大小
	m_WndWidth = cx;
	m_WndHigh = cy;
	// TODO: Add your message handler code here
}

void CTsWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnTimer(nIDEvent);
}

int CTsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTsWnd::ReadIniFile()
{
	m_StripFtSize = AfxGetApp()->GetProfileInt("FontSize","Strip",60);
	m_CarLeft = AfxGetApp()->GetProfileInt("CarDir","left",1);

	CString strDir;
	strDir = (m_CarLeft == 1)?"CrashParamLeft":"CrashParamRight";

	CString str;
	str= AfxGetApp()->GetProfileString(strDir,"fRotateTop","30");
	m_CrashParam.fRotateAng[TOP_STRIP] = atof(str);
	str= AfxGetApp()->GetProfileString(strDir,"fRotateBottom","330");
	m_CrashParam.fRotateAng[BOTTOM_STRIP] = atof(str);
	m_CrashParam.nCrashxPos = AfxGetApp()->GetProfileInt(strDir,"nCrashxPos",50);
	m_CrashParam.nCrashSize = AfxGetApp()->GetProfileInt(strDir,"nCrashSize",133);
	m_CrashParam.x_Rotate_Center = AfxGetApp()->GetProfileInt(strDir,"x_Rotate_Center",180);
	m_CrashParam.y_Rotate_Center = AfxGetApp()->GetProfileInt(strDir,"y_Rotate_Center ",220);
	str= AfxGetApp()->GetProfileString(strDir,"v_Offset","30");
	m_CrashParam.v_Offset = atoi(str);
	str= AfxGetApp()->GetProfileString(strDir,"h_Offset","8");
	m_CrashParam.h_Offset = atoi(str);

}

//负责整个区域的绘制
void CTsWnd::DrawCS(CDC* pDC)
{
	CRect TsWndRect;
	GetClientRect(&TsWndRect);
	CDC  dcCompatible;
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(!dcCompatible.m_hDC)
	{
		BOOL bRet=dcCompatible.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC,TsWndRect.Width(),TsWndRect.Height());  
		dcCompatible.SelectObject(&bitmap);
	}
	dcCompatible.SetBkMode(TRANSPARENT);
	dcCompatible.SelectObject(&pFrame->GetBackBrush());
	dcCompatible.SelectObject(&penBlue);
	dcCompatible.Rectangle(TsWndRect);

	//绘制8个防撞条,计算每个防撞条位置
	m_nStripWidth = m_WndWidth*3/4/4+40;
	m_nStripHigh = 80;
	UINT MidSpace = 10;
	//上下边距
	UINT nSideMargin = 60;
	//左右边距
	UINT SideSpace = (m_WndWidth-m_nStripWidth*4-MidSpace*3)/2;
	CRect rect;
	for(int i =0;i<8;i++)
	{
		int x = SideSpace + m_nStripWidth * (i%4) + MidSpace * (i%4);
		int y = i>=4?(m_WndHigh-nSideMargin-m_nStripHigh):nSideMargin;
		rect.SetRect(x,y,x+m_nStripWidth,y+m_nStripHigh);
		DrawStrips(&dcCompatible,i,rect);

		//绘制文字
		dcCompatible.SetTextColor(TEXTCOLOR);
		dcCompatible.SelectObject(&m_Font);
		int nTemp = i<4?0:(y+m_nStripHigh);
		rect.SetRect(x,nTemp,x+m_nStripWidth,nTemp+60);
		dcCompatible.DrawText(CDicnaryQry::GetDevName(i+0xFF01),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
	//绘制车

	if(!m_bPlanStop)
	{
		for(int i = 0;i<4;i++)
		{
			if(m_eCarStates[i] == ERROR_STATE)
			{
				rect.SetRect(SideSpace+m_nStripWidth*i+MidSpace *i+m_CrashParam.h_Offset,m_WndHigh/2-m_nStripWidth/2-m_CrashParam.v_Offset,SideSpace+m_nStripWidth*(i+1)+m_CrashParam.h_Offset,m_WndHigh/2+m_nStripWidth/2-m_CrashParam.v_Offset);
				DrawCar(&dcCompatible,rect,m_CrashParam.fRotateAng[TOP_STRIP]);
			}else if(m_eCarStates[i+4] == ERROR_STATE)
			{
				rect.SetRect(SideSpace+m_nStripWidth*i+MidSpace *i+m_CrashParam.h_Offset,m_WndHigh/2-m_nStripWidth/2+m_CrashParam.v_Offset,SideSpace+m_nStripWidth*(i+1)+m_CrashParam.h_Offset,m_WndHigh/2+m_nStripWidth/2+m_CrashParam.v_Offset);
				DrawCar(&dcCompatible,rect,m_CrashParam.fRotateAng[BOTTOM_STRIP]);
			}else
			{
				rect.SetRect(SideSpace+m_nStripWidth*i+MidSpace *i+m_CrashParam.h_Offset,m_WndHigh/2-m_nStripWidth/2,SideSpace+m_nStripWidth*(i+1)+m_CrashParam.h_Offset,m_WndHigh/2+m_nStripWidth/2);
				DrawCar(&dcCompatible,rect,0);
			}
		}
	}
	//碰撞效果
	for(int i =0;i<8;i++)
	{
		if(m_eCarStates[i] ==ERROR_STATE)
		{
			int x = SideSpace + m_nStripWidth * (i%4) + MidSpace * (i%4);
			int y = i>=4?(m_WndHigh-nSideMargin-m_nStripHigh):nSideMargin;
			rect.SetRect(x,y,x+m_nStripWidth,y+m_nStripHigh);
			DrawCrashFlash(&dcCompatible,rect);
		}
	}

	pDC->BitBlt(0,0,TsWndRect.Width(),TsWndRect.Height(),&dcCompatible,0,0,SRCCOPY);
}
//绘制8个防撞条
void CTsWnd::DrawStrips(CDC* pDC,int nIdx,CRect &rect)
{
	CBrush   bRush;
	switch(m_eCarStates[nIdx])
	{
	case INIT_STATE:
		bRush.CreateSolidBrush(INIT_CL);
		break;
	case NORAML_STATE:
		bRush.CreateSolidBrush(NORAL_CL);
		break;
	case ERROR_STATE:
		bRush.CreateSolidBrush(ERR_CL);
		break;
	default:
		bRush.CreateSolidBrush(BLACK);
		break;
	}
	pDC->FillRect(&rect,&bRush);
}
//绘制车
void CTsWnd::DrawCar(CDC* pDC,CRect &rect,float fRotateAng)
{
	HBITMAP hBitmap1, hBitmap2;
	COLORREF clrBack = RGB(255,255,255);
	float rad = 3.1415926 *fRotateAng / 180;
	if(fRotateAng != 0)
	{
		//加载位图
		if(m_CarLeft == 1)
		{
			hBitmap1 = (HBITMAP)::LoadBitmapW(AfxGetInstanceHandle(), MAKEINTRESOURCEW(IDB_CARLEFT));
		}else
		{
			hBitmap1 = (HBITMAP)::LoadBitmapW(AfxGetInstanceHandle(), MAKEINTRESOURCEW(IDB_CARRIGH));
		}

		//旋转位图
		hBitmap2 = GetRotatedBitmap(hBitmap1, rad, clrBack,m_CrashParam.x_Rotate_Center,m_CrashParam.y_Rotate_Center);

		m_bmp.Attach(hBitmap2); //关联位图对象
	}else
	{
		if(m_CarLeft == 1)
		{
			m_bmp.LoadBitmap(IDB_CARLEFT);
		}else
		{
			m_bmp.LoadBitmap(IDB_CARRIGH);
		}
	}

	m_bmp.GetObject(sizeof(BITMAP),&m_bm);
	m_Cbm.CreateCompatibleDC(pDC);
	m_Cbm.SelectObject(&m_bmp);
	TransparentBlt(pDC->GetSafeHdc(),
		rect.left,rect.top,rect.Width(),rect.Height(),
		m_Cbm.GetSafeHdc(),
		0,0,m_bm.bmWidth,m_bm.bmHeight,
		RGB(255,255,255));

	m_Cbm.DeleteDC();
	m_bmp.DeleteObject();
	if(fRotateAng != 0)
	{
		DeleteObject(hBitmap1);
		DeleteObject(hBitmap2);
	}
}
//绘制碰撞效果
void CTsWnd::DrawCrashFlash(CDC* pDC,CRect &rect)
{
	//rect为防撞条的大小
	m_bmp.LoadBitmap(IDB_CRASH);
	//获取位图信息
	m_bmp.GetObject(sizeof(BITMAP),&m_bm);
	//创建图片dc，用于显示
	m_Cbm.CreateCompatibleDC(pDC);
	m_Cbm.SelectObject(&m_bmp);
	UINT nMargin = m_CrashParam.nCrashSize;
	//将m_Cbm显示出来
	TransparentBlt(pDC->GetSafeHdc(),
		rect.left + m_CrashParam.nCrashxPos,rect.top+m_nStripHigh/2-nMargin/2,nMargin,nMargin,
		m_Cbm.GetSafeHdc(),
		0,0,m_bm.bmWidth,m_bm.bmHeight,
		RGB(255,255,255));

	m_Cbm.DeleteDC();
	m_bmp.DeleteObject();
}

void CTsWnd::SetCollisionStripsState(BYTE nId,CAR_STATE eState)
{
	if(m_bPlanStop)
	{
		m_bPlanStop = FALSE;
	}
	if(m_eCarStates[nId] != eState)
	{	
		m_eCarStates[nId] = eState;
		Invalidate();
	}
}

void CTsWnd::SetIsPlanStop(BOOL bStop)
{
	if(m_bPlanStop != bStop)
	{
		m_bPlanStop = bStop;
		if(m_bPlanStop)
		{
			for(int i = 0;i<8;i++)
			{
				m_eCarStates[i] = INIT_STATE;
			}
		}
		Invalidate();
	}
}

//hBitmap：要旋转位图的句柄
//radians   : 旋转的角度（弧度）
//clrBack   :旋转时的背景颜色
//旋转图片函数
HBITMAP CTsWnd::GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack,int x_mid,int y_mid)
{
	// Create a memory DC compatible with the display
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC(NULL);
	destDC.CreateCompatibleDC(NULL);

	// Get logical coordinates
	BITMAP bm;
	::GetObject(hBitmap, sizeof(bm), &bm);
	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(sourceDC.m_hDC, hBitmap);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(destDC.m_hDC, hbmResult);
	// Draw the background color before we change mapping mode
	HBRUSH hbrBack = CreateSolidBrush(clrBack);
	HBRUSH hbrOld = (HBRUSH)::SelectObject(destDC.m_hDC, hbrBack);
	destDC.PatBlt(0, 0, w, h, PATCOPY);
	::DeleteObject(::SelectObject(destDC.m_hDC, hbrOld));

	// We will use world transform to rotate the bitmap
	SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = sine;
	xform.eM21 = -sine;
	xform.eM22 = cosine;
	xform.eDx = (float)(x_mid - cosine*x_mid + sine*x_mid);
	xform.eDy = (float)(y_mid - cosine*y_mid - sine*y_mid);

	SetWorldTransform(destDC.m_hDC, &xform);
	// Now do the actual rotating - a pixel at a time
	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY);

	// Restore DCs
	::SelectObject(sourceDC.m_hDC, hbmOldSource);
	::SelectObject(destDC.m_hDC, hbmOldDest);
	return hbmResult;
}
