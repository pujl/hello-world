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


#endif // ENUMSTRUCT_H
