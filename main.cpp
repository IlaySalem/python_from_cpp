//! WARNING: you have to include Python.h at the top of your file
// clang-format off
#include "src/cpp/PyFunctionAdapter.h"
#include "src/cpp/PyClassAdapter.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
// clang-format on

int f()
{
    PyFunctionAdapter py(QString("%1/src/python").arg(QDir::currentPath()), "func");

    if (!py.init())
    {
        qWarning() << "python init error";
        return -1;
    }

    bool ok = false;
    qDebug() << py.python_func_get_str("get_value", "Hello from Python!", &ok) << "ok is:" << ok;

    qDebug() << py.python_func_get_val("a", &ok) << "ok is:" << ok;
    qDebug() << py.python_func_get_val("b", &ok) << "ok is:" << ok;
    qDebug() << py.python_func_get_val("c", &ok) << "ok is:" << ok;

    return 0;
}

int f2()
{
    PyClassAdapter py(QString("%1/src/python").arg(QDir::currentPath()), "class");

    if (!py.init())
    {
        qWarning() << "python init error";
        return -1;
    }

    qDebug() << py.python_class_get_str("Hello from Python!");

    qDebug() << (py.python_class_get_bool(false) ? "true" : "false");
    qDebug() << (py.python_class_get_bool(true) ? "true" : "false");

    qDebug() << py.python_class_get_int(32);
    qDebug() << py.python_class_get_double(23.123456789);

    qDebug() << py.python_class_get_val("a");
    qDebug() << py.python_class_get_val("b");
    qDebug() << py.python_class_get_val("c");

    return 0;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    f();
    f();

    return a.exec();
}
