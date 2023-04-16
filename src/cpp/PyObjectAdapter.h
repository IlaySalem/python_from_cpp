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
