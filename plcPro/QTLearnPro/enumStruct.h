#ifndef ENUMSTRUCT_H
#define ENUMSTRUCT_H

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

//设备连接消息，未连接、连接失败、连接成功
enum DEV_LINK_STATE
{
    NO_LINK = 0x0F,
    LINK_FAILED = 0x00,
    LINK_SUCC = 0x01,
};


#endif // ENUMSTRUCT_H
