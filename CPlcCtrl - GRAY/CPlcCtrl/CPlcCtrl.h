// CPlcCtrl.h : CPlcCtrl Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCPlcCtrlApp:
// �йش����ʵ�֣������ CPlcCtrl.cpp
//

class CCPlcCtrlApp : public CWinApp
{
public:
	CCPlcCtrlApp();

// ��д
public:
	virtual BOOL InitInstance();
private:
	void InitProfile();
	BOOL IsRunOnce();

private:
	HANDLE m_MutexInstance;

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCPlcCtrlApp theApp;