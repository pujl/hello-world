// CPlcCtrlDoc.cpp : CCPlcCtrlDoc 类的实现
//

#include "stdafx.h"
#include "CPlcCtrl.h"

#include "CPlcCtrlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCPlcCtrlDoc

IMPLEMENT_DYNCREATE(CCPlcCtrlDoc, CDocument)

BEGIN_MESSAGE_MAP(CCPlcCtrlDoc, CDocument)
END_MESSAGE_MAP()


// CCPlcCtrlDoc 构造/析构

CCPlcCtrlDoc::CCPlcCtrlDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCPlcCtrlDoc::~CCPlcCtrlDoc()
{
}

BOOL CCPlcCtrlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CCPlcCtrlDoc 序列化

void CCPlcCtrlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CCPlcCtrlDoc 诊断

#ifdef _DEBUG
void CCPlcCtrlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCPlcCtrlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCPlcCtrlDoc 命令
