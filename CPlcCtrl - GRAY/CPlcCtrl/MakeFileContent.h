#pragma once

//����д�ı��ĸ�ʽ �� ʵ�����ݵ�CString
class CMakeFileContent
{
public:
	//PLC�������ݼ�¼
	static CString&	GetPLCInfoTitle();
	static CString&	GetPLCInfoFormat();
private:
	static CString	_PLCInfoTitle;
	static CString  _PLCInfoFmt;
};
