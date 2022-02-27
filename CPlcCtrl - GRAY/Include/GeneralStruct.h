#pragma once

#include "CommHead.h"
#include "EnumStruct.h"
#include "CustomDefine.h"

#define					PI_VALUE			3.1415926535897932384626433832795

const	UINT UM_DEV_UPDATE_MSG = WM_USER + 0x118;

const CString           DEVNAME_PLC	= _T("PLC软件");
const UINT				DEVID_PLC	= 0;

const UINT				DEVID_RIGHT4 = 0xFF01;
const UINT				DEVID_RIGHT3 = 0xFF02;
const UINT				DEVID_RIGHT2 = 0xFF03;
const UINT				DEVID_RIGHT1 = 0xFF04;

const UINT				DEVID_LEFT4 = 0xFF05;
const UINT				DEVID_LEFT3 = 0xFF06;
const UINT				DEVID_LEFT2 = 0xFF07;
const UINT				DEVID_LEFT1 = 0xFF08;

//颜色
const int RED=RGB(255,0,0);
const int SLOWLV=RGB(221,255,221);
const int LAN=RGB(0,0,160);
const int YELLOW=RGB(255,255,0);
const int TEXTCOLOR	= RGB(0,0,0);
const int WHITE = RGB(255,255,255);
const int BACKCOLOR	=RGB(200,200,200);
const int GREEN	=RGB(0,255,0);
const int BLUE  =RGB(0,0,255);
const int BLACK	=RGB(0,0,0);
const int GRAY = RGB(90,90,90);
const int ERR_CL = RGB(255,100,0);
const int INIT_CL = RGB(255,255,255);
const int NORAL_CL = RGB(0,255,0);

typedef struct PLC_STATE{
	unsigned short	_D800;
	unsigned short	_D801;
	unsigned short	_D802;
}PLC_STATE;

typedef struct PLC_RECORD{
	BOOL  bChange;
	CAR_STATE   m_eCurState;
	SYSTEMTIME	m_ErrTime;
	SYSTEMTIME  m_RecoverTime;
}PLC_RECORD,*lpPLC_RECORD;

typedef struct CRASH_PARAM{
	//旋转角度，上、下
	float fRotateAng[2];
	//碰撞偏移位置
	UINT  nCrashxPos;
	//碰撞大小
	UINT  nCrashSize;
	//旋转中心，x,y
	UINT x_Rotate_Center;
	UINT y_Rotate_Center;
	//车偏移位置垂直、水平
	int v_Offset;
	int h_Offset;
}CRASH_PARAM,*lpCRASH_PARAM;