/****************************************************************************
** Meta object code from reading C++ file 'nettcpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "nettcpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nettcpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetTcpServer_t {
    QByteArrayData data[18];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetTcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetTcpServer_t qt_meta_stringdata_NetTcpServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "NetTcpServer"
QT_MOC_LITERAL(1, 13, 17), // "RecvNetDataSignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 11), // "const char*"
QT_MOC_LITERAL(4, 44, 4), // "name"
QT_MOC_LITERAL(5, 49, 7), // "devCode"
QT_MOC_LITERAL(6, 57, 14), // "unsigned char*"
QT_MOC_LITERAL(7, 72, 4), // "info"
QT_MOC_LITERAL(8, 77, 3), // "len"
QT_MOC_LITERAL(9, 81, 4), // "type"
QT_MOC_LITERAL(10, 86, 17), // "SendNetDataSignal"
QT_MOC_LITERAL(11, 104, 7), // "sendlen"
QT_MOC_LITERAL(12, 112, 18), // "SendNetStateSignal"
QT_MOC_LITERAL(13, 131, 11), // "GetOneFrame"
QT_MOC_LITERAL(14, 143, 12), // "SendOneFrame"
QT_MOC_LITERAL(15, 156, 7), // "sendLen"
QT_MOC_LITERAL(16, 164, 12), // "SendNetState"
QT_MOC_LITERAL(17, 177, 16) // "disconnectclient"

    },
    "NetTcpServer\0RecvNetDataSignal\0\0"
    "const char*\0name\0devCode\0unsigned char*\0"
    "info\0len\0type\0SendNetDataSignal\0sendlen\0"
    "SendNetStateSignal\0GetOneFrame\0"
    "SendOneFrame\0sendLen\0SendNetState\0"
    "disconnectclient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetTcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   59,    2, 0x06 /* Public */,
       1,    4,   70,    2, 0x26 /* Public | MethodCloned */,
      10,    5,   79,    2, 0x06 /* Public */,
      10,    4,   90,    2, 0x26 /* Public | MethodCloned */,
      12,    0,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  100,    2, 0x0a /* Public */,
      14,    2,  101,    2, 0x0a /* Public */,
      16,    0,  106,    2, 0x0a /* Public */,
      17,    0,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 6, QMetaType::ULong, QMetaType::UChar,    4,    5,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 6, QMetaType::ULong,    4,    5,    7,    8,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 6, QMetaType::ULong, QMetaType::UChar,    4,    5,    7,   11,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, 0x80000000 | 6, QMetaType::ULong,    4,    5,    7,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::ULong,    7,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetTcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetTcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RecvNetDataSignal((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4])),(*reinterpret_cast< unsigned char(*)>(_a[5]))); break;
        case 1: _t->RecvNetDataSignal((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4]))); break;
        case 2: _t->SendNetDataSignal((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4])),(*reinterpret_cast< unsigned char(*)>(_a[5]))); break;
        case 3: _t->SendNetDataSignal((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4]))); break;
        case 4: _t->SendNetStateSignal(); break;
        case 5: _t->GetOneFrame(); break;
        case 6: _t->SendOneFrame((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2]))); break;
        case 7: _t->SendNetState(); break;
        case 8: _t->disconnectclient(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetTcpServer::*)(const char * , int , unsigned char * , unsigned long , unsigned char );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetTcpServer::RecvNetDataSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetTcpServer::*)(const char * , int , unsigned char * , unsigned long , unsigned char );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetTcpServer::SendNetDataSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetTcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetTcpServer::SendNetStateSignal)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetTcpServer::staticMetaObject = { {
    &QTcpServer::staticMetaObject,
    qt_meta_stringdata_NetTcpServer.data,
    qt_meta_data_NetTcpServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetTcpServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int NetTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void NetTcpServer::RecvNetDataSignal(const char * _t1, int _t2, unsigned char * _t3, unsigned long _t4, unsigned char _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void NetTcpServer::SendNetDataSignal(const char * _t1, int _t2, unsigned char * _t3, unsigned long _t4, unsigned char _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 4
void NetTcpServer::SendNetStateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
