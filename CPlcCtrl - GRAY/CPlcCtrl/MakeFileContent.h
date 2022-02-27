#pragma once

//生成写文本的格式 与 实际内容的CString
class CMakeFileContent
{
public:
	//PLC故障数据记录
	static CString&	GetPLCInfoTitle();
	static CString&	GetPLCInfoFormat();
private:
	static CString	_PLCInfoTitle;
	static CString  _PLCInfoFmt;
};
