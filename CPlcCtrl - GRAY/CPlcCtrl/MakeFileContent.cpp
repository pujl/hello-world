#include "StdAfx.h"
#include "MakeFileContent.h"

//����澯����ͷ
CString	CMakeFileContent::_PLCInfoTitle = _T("���\t�豸����\t�쳣ʱ��\t�ָ�ʱ��\r\n");
CString	CMakeFileContent::_PLCInfoFmt   = _T("%d\t%s\t\t%02d:%02d:%02d\t%02d:%02d:%02d\t\t\r\n");

CString&	CMakeFileContent::GetPLCInfoTitle()
{
	return _PLCInfoTitle;
}

CString&	CMakeFileContent::GetPLCInfoFormat()
{
	return _PLCInfoFmt;
}