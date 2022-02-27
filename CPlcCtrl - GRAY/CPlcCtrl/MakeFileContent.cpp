#include "StdAfx.h"
#include "MakeFileContent.h"

//红外告警数据头
CString	CMakeFileContent::_PLCInfoTitle = _T("序号\t设备名称\t异常时间\t恢复时间\r\n");
CString	CMakeFileContent::_PLCInfoFmt   = _T("%d\t%s\t\t%02d:%02d:%02d\t%02d:%02d:%02d\t\t\r\n");

CString&	CMakeFileContent::GetPLCInfoTitle()
{
	return _PLCInfoTitle;
}

CString&	CMakeFileContent::GetPLCInfoFormat()
{
	return _PLCInfoFmt;
}