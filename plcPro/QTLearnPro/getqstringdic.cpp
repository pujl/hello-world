#include "getqstringdic.h"


QString getQStringDic::GetDevName(unsigned int nId)
{
    QString str;

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
