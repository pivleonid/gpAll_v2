/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 8), // "openBoms"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 9), // "openSklad"
QT_MOC_LITERAL(4, 31, 5), // "clear"
QT_MOC_LITERAL(5, 37, 8), // "generate"
QT_MOC_LITERAL(6, 46, 9), // "openAbout"
QT_MOC_LITERAL(7, 56, 18), // "operationSumRefDez"
QT_MOC_LITERAL(8, 75, 34), // "QMap<QString,QList<QStringLis..."
QT_MOC_LITERAL(9, 110, 14), // "mapVarLisrCont"
QT_MOC_LITERAL(10, 125, 14), // "operationSklad"
QT_MOC_LITERAL(11, 140, 15), // "listStringInt_t"
QT_MOC_LITERAL(12, 156, 15), // "operationSearch"
QT_MOC_LITERAL(13, 172, 18), // "QList<QStringList>"
QT_MOC_LITERAL(14, 191, 15), // "dataSkladAndSum"
QT_MOC_LITERAL(15, 207, 16), // "listStringInt_t&"
QT_MOC_LITERAL(16, 224, 9) // "dataSklad"

    },
    "MainWindow\0openBoms\0\0openSklad\0clear\0"
    "generate\0openAbout\0operationSumRefDez\0"
    "QMap<QString,QList<QStringList> >&\0"
    "mapVarLisrCont\0operationSklad\0"
    "listStringInt_t\0operationSearch\0"
    "QList<QStringList>\0dataSkladAndSum\0"
    "listStringInt_t&\0dataSklad"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    1,   59,    2, 0x08 /* Private */,
      10,    0,   62,    2, 0x08 /* Private */,
      12,    2,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    0x80000000 | 11,
    0x80000000 | 13, 0x80000000 | 8, 0x80000000 | 15,   14,   16,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openBoms(); break;
        case 1: _t->openSklad(); break;
        case 2: _t->clear(); break;
        case 3: _t->generate(); break;
        case 4: _t->openAbout(); break;
        case 5: _t->operationSumRefDez((*reinterpret_cast< QMap<QString,QList<QStringList> >(*)>(_a[1]))); break;
        case 6: { listStringInt_t _r = _t->operationSklad();
            if (_a[0]) *reinterpret_cast< listStringInt_t*>(_a[0]) = std::move(_r); }  break;
        case 7: { QList<QStringList> _r = _t->operationSearch((*reinterpret_cast< QMap<QString,QList<QStringList> >(*)>(_a[1])),(*reinterpret_cast< listStringInt_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QList<QStringList>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
