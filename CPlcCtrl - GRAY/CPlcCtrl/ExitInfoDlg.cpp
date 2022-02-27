// ExitInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExitInfoDlg.h"
#include "MainFrm.h"

// CExitInfoDlg 对话框

IMPLEMENT_DYNAMIC(CExitInfoDlg, CDialog)

CExitInfoDlg::CExitInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitInfoDlg::IDD, pParent)
{
	PromptStr.Empty();
	m_DlgType = (SELDLG_STYLE)0;

	m_CaplFont.lfHeight = -18; //10号字
	m_CaplFont.lfWidth = 0; 
	m_CaplFont.lfEscapement = 0; 
	m_CaplFont.lfOrientation = 0; 
	m_CaplFont.lfWeight = 0; 
	m_CaplFont.lfItalic = 0; 
	m_CaplFont.lfUnderline = 0; 
	m_CaplFont.lfStrikeOut = 0; 
	m_CaplFont.lfCharSet = 134; 
	m_CaplFont.lfOutPrecision = 3; 
	m_CaplFont.lfClipPrecision = 2; 
	m_CaplFont.lfQuality = 1; 
	m_CaplFont.lfPitchAndFamily = 2; 
	memset(m_CaplFont.lfFaceName,0,sizeof(m_CaplFont.lfFaceName));
	wsprintf(m_CaplFont.lfFaceName,"%s",_T("黑体"));
}

CExitInfoDlg::~CExitInfoDlg()
{
}

void CExitInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_SelBtn[1]);
	DDX_Control(pDX, IDOK, m_SelBtn[0]);
	DDX_Control(pDX, IDC_PROMPTCONTENT_EDIT, m_PromptContent_Edit);
}


BEGIN_MESSAGE_MAP(CExitInfoDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CExitInfoDlg 消息处理程序

HBRUSH CExitInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	CMainFrame *pFrame =(CMainFrame *)AfxGetMainWnd();
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)pFrame->GetBackBrush().GetSafeHandle ();	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CExitInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CMainFrame *pFrame =(CMainFrame *)AfxGetMainWnd();

	m_PromptContent_Edit.SetBackColor(BACKCOLOR);
	m_PromptContent_Edit.SetTextColor(TEXTCOLOR);
	m_PromptContent_Edit.SetTextFont(m_CaplFont);
	m_PromptContent_Edit.SetAlignment(DT_CENTER);
	m_PromptContent_Edit.SetCaption(PromptStr);

	for(int i = 0;i<2;i++)
	{
		m_SelBtn[i].SetBitmaps(IDB_BITMAP1,IDB_BITMAP2,IDB_BITMAP3);
	}
	if(EXIT_DLG == m_DlgType)
	{
		m_SelBtn[0].SetBtnId(BTNID_EXIT_CONFIRM);
	}else if(CANCEL_DLG == m_DlgType)
	{
		m_SelBtn[0].SetBtnId(BTNID_ENSUER_CONFIRM);
	}else
	{
		m_SelBtn[0].ShowWindow(SW_HIDE);
		m_SelBtn[1].SetWindowPos(NULL,130,70,0,0,SWP_NOSIZE);
	}
	m_SelBtn[1].SetBtnId(BTNID_CANCER_CONFIRM);

	SetWindowText(CaptionStr);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CExitInfoDlg::SetPromptStr(LPCSTR pStr)
{
	PromptStr.Format("%s",pStr);
}

void CExitInfoDlg::SetCaptionStr(LPCSTR pStr)
{
	CaptionStr.Format("%s",pStr);
}

void CExitInfoDlg::SetDlgType(SELDLG_STYLE type)
{
	m_DlgType=type;
}
