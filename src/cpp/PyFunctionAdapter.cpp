#include "PyFunctionAdapter.h"

PyFunctionAdapter::PyFunctionAdapter(const QString& pyScriptPath, const QString& pyScriptName)
    : _filePath(pyScriptPath)
    , _fileName(pyScriptName)
{
}

PyFunctionAdapter::~PyFunctionAdapter()
{
    Py_Finalize();
}

bool PyFunctionAdapter::init()
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

QByteArray PyFunctionAdapter::python_func_get_str(const QString& funcName, const QByteArray& val, bool* ok)
{
    QByteArray ret;
    if (ok)
        *ok = false;

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
        if (ok)
            *ok = true;

        // If the received string is not copied, then after clearing the resources
        // python won't have it. To start, pResultRepr needs to be cast to an array of
        // bytes.
        ret = strdup(PyBytes_AS_STRING(
            PyUnicode_AsEncodedString(pResultRepr->obj(), "utf-8", "ERROR")));
    }

    return ret;
}

qint32 PyFunctionAdapter::python_func_get_val(const QString& varName, bool* ok)
{
    qint32 ret = 0;
    if (ok)
        *ok = false;

    // Get object with name "varName"
    PyObject* tempValue = PyDict_GetItemString(_dict->obj(), varName.toStdString().c_str());

    // Check variable for "long" type
    if (tempValue && PyLong_Check(tempValue))
    {
        if (ok)
            *ok = true;
        ret = _PyLong_AsInt(tempValue);
    }

    return ret;
}
