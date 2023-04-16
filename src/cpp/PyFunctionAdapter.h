/**
 * gcc func.c $(python3-config --includes --ldflags) -o func && ./func
 */
#pragma once

#include <Python.h>

#include <QDebug>
#include <QFile>

#define PY_OBJECT(macro) \
    QScopedPointer<PyObjectAdapter>(new PyObjectAdapter(macro));

class PyObjectAdapter
{
public:
    PyObjectAdapter(PyObject* obj)
        : _obj(obj)
    {
        //TODO: implement check in a soft way (allow uninitialized objects?)
        Q_ASSERT_X(_obj, "PyObjectAdapter", "python object is NULL");
    };
    ~PyObjectAdapter()
    {
        // Return resources to the system
        Py_XDECREF(_obj);
    }

    PyObject* obj() const
    {
        return _obj;
    }

private:
    PyObject* _obj = nullptr;
};

class PyFunctionAdapter
{
public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    PyFunctionAdapter(const QString& pyScriptPath, const QString& pyScriptName)
        : _filePath(pyScriptPath)
        , _fileName(pyScriptName){};

    ~PyFunctionAdapter()
    {
        Py_Finalize();
    }

    /*
     * Loading python interpreter and module _fileName.py into it
     */
    bool init()
    {
        if (!QFile(QString("%1/%2.py").arg(_filePath, _fileName)).exists())
        {
            qWarning() << "file doesn't exist";
            return false;
        }

        // Initialize Python interpreter
        Py_Initialize();

        // Loading sys module
        auto sys     = PY_OBJECT(PyImport_ImportModule("sys"));
        auto sysPath = PY_OBJECT(PyObject_GetAttrString(sys->obj(), "path"));
        // Our python sources
        auto folderPath = PY_OBJECT(PyUnicode_FromString(_filePath.toStdString().c_str()));
        PyList_Append(sysPath->obj(), folderPath->obj());

        // Creating a Unicode object from UTF-8 string
        auto name = PY_OBJECT(PyUnicode_FromString(_fileName.toStdString().c_str()));
        if (!name->obj())
        {
            // PyErr_Print(); - you should catch the stream if you want to use it
            qWarning() << "name is nullptr";
            return false;
        }

        // Loading module _fileName
        auto module = PY_OBJECT(PyImport_Import(name->obj()));
        if (!module->obj())
        {
            qWarning() << "module is nullptr";
            return false;
        }

        // Creating a dictionary of objects contained in a module
        _dict.reset(new PyObjectAdapter(PyModule_GetDict(module->obj())));
        if (!_dict->obj())
            qWarning() << "_dict is nullptr";

        return _dict->obj();
    }

    /**
   * Predefined function: func is setting string (from c++) -
   * func is getting modified (python) string back
   */
    QByteArray python_func_get_str(const QString& funcName, const QByteArray& val)
    {
        QByteArray ret;

        // Loading object "funcName" from module _fileName
        auto objct = PY_OBJECT(PyDict_GetItemString(_dict->obj(), funcName.toStdString().c_str()));
        if (!objct)
            return ret;

        // Check if pObjct is valid.
        if (!PyCallable_Check(objct->obj()))
            return ret;

        auto tempValue = PY_OBJECT(PyObject_CallFunction(objct->obj(), (char*)"(s)", val.data()));
        if (tempValue)
        {
            auto pResultRepr = PY_OBJECT(PyObject_Repr(tempValue->obj()));

            // If the received string is not copied, then after clearing the resources
            // python won't have it. To start, pResultRepr needs to be cast to an array of
            // bytes.
            ret = strdup(PyBytes_AS_STRING(
                PyUnicode_AsEncodedString(pResultRepr->obj(), "utf-8", "ERROR")));
        }
        else
            PyErr_Print();

        return ret;
    }

    /**
    * Getting value of type int named "val"
    */
    qint32 python_func_get_val(const QString& varName)
    {
        qint32 ret = -1;

        // Get object with name "varName"
        PyObject* tempValue = PyDict_GetItemString(_dict->obj(), varName.toStdString().c_str());

        // Check variable for "long" type
        if (tempValue && PyLong_Check(tempValue))
            ret = _PyLong_AsInt(tempValue);
        else
            PyErr_Print();

        return ret;
    }

private:
    const QString _filePath;
    const QString _fileName;

    QScopedPointer<PyObjectAdapter> _dict;
};
