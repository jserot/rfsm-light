/****************************************************************************
** Meta object code from reading C++ file 'fsmEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fsmEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fsmEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FsmEditor_t {
    QByteArrayData data[22];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FsmEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FsmEditor_t qt_meta_stringdata_FsmEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FsmEditor"
QT_MOC_LITERAL(1, 10, 13), // "stateSelected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 6), // "State*"
QT_MOC_LITERAL(4, 32, 18), // "transitionSelected"
QT_MOC_LITERAL(5, 51, 11), // "Transition*"
QT_MOC_LITERAL(6, 63, 15), // "nothingSelected"
QT_MOC_LITERAL(7, 79, 11), // "fsmModified"
QT_MOC_LITERAL(8, 91, 10), // "newDiagram"
QT_MOC_LITERAL(9, 102, 4), // "save"
QT_MOC_LITERAL(10, 107, 6), // "saveAs"
QT_MOC_LITERAL(11, 114, 8), // "openFile"
QT_MOC_LITERAL(12, 123, 10), // "checkModel"
QT_MOC_LITERAL(13, 134, 13), // "withTestBench"
QT_MOC_LITERAL(14, 148, 6), // "errMsg"
QT_MOC_LITERAL(15, 155, 14), // "checkModelOnly"
QT_MOC_LITERAL(16, 170, 14), // "checkTestbench"
QT_MOC_LITERAL(17, 185, 9), // "exportDot"
QT_MOC_LITERAL(18, 195, 15), // "exportRfsmModel"
QT_MOC_LITERAL(19, 211, 19), // "exportRfsmTestbench"
QT_MOC_LITERAL(20, 231, 17), // "toolButtonClicked"
QT_MOC_LITERAL(21, 249, 2) // "id"

    },
    "FsmEditor\0stateSelected\0\0State*\0"
    "transitionSelected\0Transition*\0"
    "nothingSelected\0fsmModified\0newDiagram\0"
    "save\0saveAs\0openFile\0checkModel\0"
    "withTestBench\0errMsg\0checkModelOnly\0"
    "checkTestbench\0exportDot\0exportRfsmModel\0"
    "exportRfsmTestbench\0toolButtonClicked\0"
    "id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FsmEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x0a /* Public */,
       4,    1,   97,    2, 0x0a /* Public */,
       6,    0,  100,    2, 0x0a /* Public */,
       7,    0,  101,    2, 0x0a /* Public */,
       8,    0,  102,    2, 0x0a /* Public */,
       9,    0,  103,    2, 0x0a /* Public */,
      10,    0,  104,    2, 0x0a /* Public */,
      11,    0,  105,    2, 0x0a /* Public */,
      12,    2,  106,    2, 0x0a /* Public */,
      12,    1,  111,    2, 0x2a /* Public | MethodCloned */,
      15,    0,  114,    2, 0x0a /* Public */,
      16,    0,  115,    2, 0x0a /* Public */,
      17,    0,  116,    2, 0x0a /* Public */,
      18,    0,  117,    2, 0x0a /* Public */,
      19,    0,  118,    2, 0x0a /* Public */,
      20,    1,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,

       0        // eod
};

void FsmEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FsmEditor *_t = static_cast<FsmEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateSelected((*reinterpret_cast< State*(*)>(_a[1]))); break;
        case 1: _t->transitionSelected((*reinterpret_cast< Transition*(*)>(_a[1]))); break;
        case 2: _t->nothingSelected(); break;
        case 3: _t->fsmModified(); break;
        case 4: _t->newDiagram(); break;
        case 5: _t->save(); break;
        case 6: _t->saveAs(); break;
        case 7: _t->openFile(); break;
        case 8: _t->checkModel((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->checkModel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->checkModelOnly(); break;
        case 11: _t->checkTestbench(); break;
        case 12: _t->exportDot(); break;
        case 13: _t->exportRfsmModel(); break;
        case 14: _t->exportRfsmTestbench(); break;
        case 15: _t->toolButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FsmEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FsmEditor.data,
      qt_meta_data_FsmEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FsmEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FsmEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FsmEditor.stringdata0))
        return static_cast<void*>(const_cast< FsmEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int FsmEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
