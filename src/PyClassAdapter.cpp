#include "PyClassAdapter.h"

PyClassAdapter::PyClassAdapter(const QString& pyScriptPath, const QString& pyScriptName)
    : _filePath(pyScriptPath)
    , _fileName(pyScriptName)
{
}

PyClassAdapter::~PyClassAdapter()
{
    // Вернуть ресурсы системе
    Py_XDECREF(pInstance);
    Py_XDECREF(pDict);

    Py_XDECREF(pModule);
    Py_XDECREF(pName);

    Py_XDECREF(folder_path);
    Py_XDECREF(sys_path);
    Py_XDECREF(sys);

    // Выгрузка интерпритатора Python
    Py_Finalize();
}

bool PyClassAdapter::init()
{
    if (!QFile(QString("%1/%2.py").arg(_filePath, _fileName)).exists())
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Инициализировать интерпретатор Python
    Py_Initialize();

    // Загрузка модуля sys
    sys      = PyImport_ImportModule("sys");
    sys_path = PyObject_GetAttrString(sys, "path");
    // Путь до наших исходников python
    folder_path = PyUnicode_FromString(_filePath.toStdString().c_str());
    PyList_Append(sys_path, folder_path);

    // Создание Unicode объекта из UTF-8 строки
    pName = PyUnicode_FromString(_fileName.toStdString().c_str());
    if (!pName)
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Загрузить модуль class
    pModule = PyImport_Import(pName);
    if (!pModule)
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Словарь объектов содержащихся в модуле
    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Загрузка объекта Class из class.py
    pClass = PyDict_GetItemString(pDict, (const char*)"Class");
    if (!pClass)
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Проверка pClass на годность.
    if (!PyCallable_Check(pClass))
    {
        qWarning() << "file doesn't exist";
        return false;
    }

    // Указатель на Class
    pInstance = PyObject_CallObject(pClass, NULL);

    return pInstance;
}

char* PyClassAdapter::python_class_get_str(char* val)
{
    char* ret = NULL;

    pVal =
        PyObject_CallMethod(pInstance, (char*)"get_value", (char*)"(s)", val);
    if (pVal != NULL)
    {
        PyObject* pResultRepr = PyObject_Repr(pVal);

        // Если полученную строку не скопировать, то после очистки ресурсов python
        // её не будет.
        ret = strdup(PyBytes_AS_STRING(
            PyUnicode_AsEncodedString(pResultRepr, "utf-8", "ERROR")));

        Py_XDECREF(pResultRepr);
        Py_XDECREF(pVal);
    }
    else
    {
        PyErr_Print();
    }

    return ret;
}

bool PyClassAdapter::python_class_get_bool(bool val)
{
    bool ret = false;

    // printf("val %d\n", val);
    pVal = PyObject_CallMethod(pInstance, (char*)"get_bool", (char*)"(i)", val);
    if (pVal != NULL)
    {
        if (PyBool_Check(pVal))
        {
            ret = PyObject_IsTrue(pVal);
        }

        Py_XDECREF(pVal);
    }
    else
    {
        PyErr_Print();
    }

    return ret;
}

int PyClassAdapter::python_class_get_int(int val)
{
    int ret = 0;

    pVal =
        PyObject_CallMethod(pInstance, (char*)"get_value", (char*)"(i)", val);
    if (pVal != NULL)
    {
        if (PyLong_Check(pVal))
        {
            ret = _PyLong_AsInt(pVal);
        }

        Py_XDECREF(pVal);
    }
    else
    {
        PyErr_Print();
    }

    return ret;
}

double PyClassAdapter::python_class_get_double(double val)
{
    double ret = 0.0;

    pVal =
        PyObject_CallMethod(pInstance, (char*)"get_value", (char*)"(f)", val);
    if (pVal != NULL)
    {
        if (PyFloat_Check(pVal))
        {
            ret = PyFloat_AS_DOUBLE(pVal);
        }

        Py_XDECREF(pVal);
    }
    else
    {
        PyErr_Print();
    }

    return ret;
}

int PyClassAdapter::python_class_get_val(char* val)
{
    int ret = 0;

    pVal = PyObject_GetAttrString(pInstance, (char*)val);
    if (pVal != NULL)
    {
        if (PyLong_Check(pVal))
        {
            ret = _PyLong_AsInt(pVal);
        }

        Py_XDECREF(pVal);
    }
    else
    {
        PyErr_Print();
    }

    return ret;
}
