//! WARNING: you have to include Python.h at the top of your file
// clang-format off
#include "src/cpp/func.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
// clang-format on

int f()
{
    PyAdapter py(QString("%1/src/python").arg(QDir::currentPath()), "func");

    if (!py.init())
    {
        qWarning() << "python init error";
        return -1;
    }

    qDebug() << py.python_func_get_str("get_value", "Hello from Python!");

    qDebug() << py.python_func_get_val("a");
    qDebug() << py.python_func_get_val("b");
    qDebug() << py.python_func_get_val("c");

    return 0;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    f();
    f();

    return a.exec();
}
