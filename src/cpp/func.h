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
    PyObjectAdapter(PyObject* obj, const QString& name = "tempObject")
        : _obj(obj)
        , _name(name)
    {
        //TODO: implement check in a soft way (allow uninitialized objects?)
        Q_ASSERT_X(_obj, "PyObjectAdapter", "python object is NULL");
    }

    ~PyObjectAdapter()
    {
        // Return resources to the system
        Py_XDECREF(_obj);
        qDebug() << "destructor: " << _name;
    }

    PyObject* obj() const
    {
        return _obj;
    }

private:
    PyObject*     _obj = nullptr;
    const QString _name;
};

class PyAdapter
{
public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    PyAdapter(const QString& pyScriptPath, const QString& pyScriptName)
        : _filePath(pyScriptPath)
        , _fileName(pyScriptName){};

    ~PyAdapter()
    {
        //WARNING: destruction of this pointer causes memory curroption
        //Py_XDECREF(_tempValue);
        Py_XDECREF(_dict);
        Py_XDECREF(_module);
        Py_XDECREF(_name);
        Py_XDECREF(_folderPath);
        Py_XDECREF(_sysPath);
        Py_XDECREF(_sys);

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
        _sys     = PyImport_ImportModule("sys");
        _sysPath = PyObject_GetAttrString(_sys, "path");
        // Our python sources
        _folderPath = PyUnicode_FromString(_filePath.toStdString().c_str());
        PyList_Append(_sysPath, _folderPath);

        // Creating a Unicode object from UTF-8 string
        _name = PyUnicode_FromString(_fileName.toStdString().c_str());
        if (!_name)
        {
            // PyErr_Print(); - you should catch the stream if you want to use it
            qWarning() << "name is nullptr";
            return false;
        }

        // Loading module _fileName
        _module = PyImport_Import(_name);
        if (!_module)
        {
            qWarning() << "module is nullptr";
            return false;
        }

        // Creating a dictionary of objects contained in a module
        _dict = PyModule_GetDict(_module);
        if (!_dict)
            qWarning() << "_dict is nullptr";

        return _dict;
    }

    /**
   * Predefined function: func is setting string (from c++) -
   * func is getting modified (python) string back
   */
    QByteArray python_func_get_str(const QString& funcName, const QByteArray& val)
    {
        QByteArray ret;

        // Loading object "funcName" from module _fileName
        auto objct = PY_OBJECT(PyDict_GetItemString(_dict, funcName.toStdString().c_str()));
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
        _tempValue = PyDict_GetItemString(_dict, varName.toStdString().c_str());

        // Check variable for "long" type
        if (_tempValue && PyLong_Check(_tempValue))
            ret = _PyLong_AsInt(_tempValue);
        else
            PyErr_Print();

        return ret;
    }

private:
    const QString _filePath;
    const QString _fileName;

    PyObject *_dict       = nullptr,
             *_sys        = nullptr,
             *_sysPath    = nullptr,
             *_folderPath = nullptr,
             *_name       = nullptr,
             *_module     = nullptr,
             *_tempValue  = nullptr;
};
