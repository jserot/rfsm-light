/****************************************************************************
** Meta object code from reading C++ file 'fsd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fsd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fsd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Fsd_t {
    QByteArrayData data[11];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fsd_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fsd_t qt_meta_stringdata_Fsd = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Fsd"
QT_MOC_LITERAL(1, 4, 11), // "fsdModified"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 13), // "stateSelected"
QT_MOC_LITERAL(4, 31, 6), // "State*"
QT_MOC_LITERAL(5, 38, 4), // "item"
QT_MOC_LITERAL(6, 43, 18), // "transitionSelected"
QT_MOC_LITERAL(7, 62, 11), // "Transition*"
QT_MOC_LITERAL(8, 74, 7), // "setMode"
QT_MOC_LITERAL(9, 82, 4), // "Mode"
QT_MOC_LITERAL(10, 87, 4) // "mode"

    },
    "Fsd\0fsdModified\0\0stateSelected\0State*\0"
    "item\0transitionSelected\0Transition*\0"
    "setMode\0Mode\0mode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fsd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       6,    1,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void Fsd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Fsd *_t = static_cast<Fsd *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fsdModified(); break;
        case 1: _t->stateSelected((*reinterpret_cast< State*(*)>(_a[1]))); break;
        case 2: _t->transitionSelected((*reinterpret_cast< Transition*(*)>(_a[1]))); break;
        case 3: _t->setMode((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Fsd::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Fsd::fsdModified)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Fsd::*_t)(State * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Fsd::stateSelected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Fsd::*_t)(Transition * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Fsd::transitionSelected)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Fsd::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_Fsd.data,
      qt_meta_data_Fsd,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fsd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fsd::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fsd.stringdata0))
        return static_cast<void*>(const_cast< Fsd*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int Fsd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Fsd::fsdModified()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fsd::stateSelected(State * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Fsd::transitionSelected(Transition * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
