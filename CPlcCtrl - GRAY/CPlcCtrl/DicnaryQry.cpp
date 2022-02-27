#include "StdAfx.h"
#include "DicnaryQry.h"

CString	CDicnaryQry::GetBtnName(UINT nId)
{
	CString str;

	switch(nId)
	{
	case BTNID_EXIT_SYS:
		str = "退出\n系统";
		break;
	case BTNID_CONN_SYS:
		str = "连接\nPLC";
		break;
	case BTNID_EXIT_CONFIRM:
		str = "退出";
		break;
	case BTNID_CANCER_CONFIRM:
		str = "取消";
		break;
	case BTNID_ENSUER_CONFIRM:
		str = "确定";
		break;
	default:
		break;
	}

	return str;
}

CString CDicnaryQry::GetDevName(UINT nId)
{
	CString str;

	switch(nId)
	{
	case DEVID_LEFT1:
		str = "左一";
		break;
	case DEVID_LEFT2:
		str = "左二";
		break;
	case DEVID_LEFT3:
		str = "左三";
		break;
	case DEVID_LEFT4:
		str = "左四";
		break;
	case DEVID_RIGHT1:
		str = "右一";
		break;
	case DEVID_RIGHT2:
		str = "右二";
		break;
	case DEVID_RIGHT3:
		str = "右三";
		break;
	case DEVID_RIGHT4:
		str = "右四";
		break;
	default:
		break;
	}

	return str;
}