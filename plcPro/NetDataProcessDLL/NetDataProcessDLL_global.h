#ifndef NETDATAPROCESSDLL_GLOBAL_H
#define NETDATAPROCESSDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETDATAPROCESSDLL_LIBRARY)
#  define NETDATAPROCESSDLL_EXPORT Q_DECL_EXPORT
#else
#  define NETDATAPROCESSDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // NETDATAPROCESSDLL_GLOBAL_H
