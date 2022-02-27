#pragma once


//选择对话框类别
enum SELDLG_STYLE
{
	EXIT_DLG,
	CANCEL_DLG,
	TIP_DLG
};

//按钮类别定义
enum BTN_STYLE
{
	DEFAULT_BTN,
	NORMAL_BTN,
	NUM_BTN,
	NUM_GUIDE_BTN,
};

// CRectWnd
//白色代表计划停止
//绿色代表正常
//红色代表异常
enum CAR_STATE {
	INIT_STATE,
	NORAML_STATE,
	ERROR_STATE,
};

//定义上下故障条
enum STRIP_POS
{
	TOP_STRIP = 0,
	BOTTOM_STRIP = 1,
};