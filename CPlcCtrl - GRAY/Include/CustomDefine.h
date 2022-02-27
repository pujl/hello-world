#pragma once

//菜单命令号
//不分视图，分按钮类别+按钮操作和命令号
//按钮类别分为普通按钮(1)、数字键盘按钮(2)和干扰引导数字按钮（3）三种
//按钮操作分为按完弹出界面类型（1）和操作命令（2）按钮
//按钮依次添加，在过程中根据实际使用情况再添加即可

const UINT  BTNID_EXIT_SYS			= 0x01020012;
const UINT  BTNID_CONN_SYS			= 0x01020013;
//退出按钮
const UINT  BTNID_EXIT_CONFIRM		= 0x01021000;
const UINT  BTNID_CANCER_CONFIRM	= 0x01022000;
const UINT  BTNID_ENSUER_CONFIRM	= 0x01023000;

//掩码定义
const UINT BTN_CATEGORY_MASK			= 0xFF000000;
const UINT BTN_OPR_MASK					= 0x00FF0000;
const UINT BTN_CMD_MASK					= 0x0000FFFF;
const UINT BTN_GUIDETGT_NUMMASK			= 0x00FFFFFF;

//常量定义
const UINT BTNTY_NORMAL_STYLE			= 0x01000000;
const UINT BTNTY_NUM_KEYBOARD			= 0x02000000;
const UINT BTNTY_NUM_GUIDE				= 0x03000000;
const UINT BTNTY_SWITCH_VIEW			= 0x00010000;
const UINT BTNTY_SEND_ORDER				= 0x00020000;

const UINT INVALID_ID					= 0xFFFFFFFF;


