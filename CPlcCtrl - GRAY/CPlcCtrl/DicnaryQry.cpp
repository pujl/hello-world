#include "StdAfx.h"
#include "DicnaryQry.h"

CString	CDicnaryQry::GetBtnName(UINT nId)
{
	CString str;

	switch(nId)
	{
	case BTNID_EXIT_SYS:
		str = "�˳�\nϵͳ";
		break;
	case BTNID_CONN_SYS:
		str = "����\nPLC";
		break;
	case BTNID_EXIT_CONFIRM:
		str = "�˳�";
		break;
	case BTNID_CANCER_CONFIRM:
		str = "ȡ��";
		break;
	case BTNID_ENSUER_CONFIRM:
		str = "ȷ��";
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
		str = "��һ";
		break;
	case DEVID_LEFT2:
		str = "���";
		break;
	case DEVID_LEFT3:
		str = "����";
		break;
	case DEVID_LEFT4:
		str = "����";
		break;
	case DEVID_RIGHT1:
		str = "��һ";
		break;
	case DEVID_RIGHT2:
		str = "�Ҷ�";
		break;
	case DEVID_RIGHT3:
		str = "����";
		break;
	case DEVID_RIGHT4:
		str = "����";
		break;
	default:
		break;
	}

	return str;
}