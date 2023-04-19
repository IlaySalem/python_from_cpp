/**
 * gcc class.c $(python3-config --includes --ldflags) -o class && ./class
 * https://docs.python.org/3/c-api/arg.html#c.Py_BuildValue Building values
 */
#pragma once
// clang-format off
#include "PyObjectAdapter.h"
// clang-format on

#include <stdbool.h>
#include <stdio.h>

#include <QDebug>
#include <QFile>

class PyClassAdapter
{
public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    PyClassAdapter(const QString& pyScriptPath, const QString& pyScriptName);

    ~PyClassAdapter();

    /**
     * Загрузка интерпритатора python и модуля class.py в него.
     */
    bool init();

    /**
     * Передача строки в качестве аргумента и получение строки назад
     */
    char* python_class_get_str(char* val);

    /**
     * Передача bool в качестве аргумента и получение bool назад
     */
    bool python_class_get_bool(bool val);

    /**
     * Передача int в качестве аргумента и получение int назад
     */
    int python_class_get_int(int val);

    /**
     * Передача double в качестве аргумента и получение double назад
     */
    double python_class_get_double(double val);

    /**
     * Получение значения переменной содержащей значение типа int
     */
    int python_class_get_val(char* val);

private:
    const QString _filePath;
    const QString _fileName;

    PyObject *pName = NULL, *pModule = NULL, *pInstance = NULL;
    PyObject *pDict = NULL, *pClass = NULL, *pVal = NULL;
    PyObject* sys         = NULL;
    PyObject* sys_path    = NULL;
    PyObject* folder_path = NULL;
};
