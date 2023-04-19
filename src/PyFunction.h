/**
 * gcc func.c $(python3-config --includes --ldflags) -o func && ./func
 */
#pragma once
#include "PythonGlobal.h"
#include "include/IPyFunction.h"

class PYTHON_EXPORT PyFunction final : public IPyFunction
{
public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    PyFunction(const QString& pyScriptPath, const QString& pyScriptName);

    ~PyFunction();

    //! Loading python interpreter and module _fileName.py into it
    bool init() override;

    //! Predefined function: func is setting string (from c++) -
    /// func is getting modified (python) string back
    QByteArray python_func_get_str(const QString& funcName, const QByteArray& val, bool* ok = nullptr) override;

    //!Getting value of type int named "val"
    qint32 python_func_get_val(const QString& varName, bool* ok = nullptr) override;

private:
    QScopedPointer<class PyObjectAdapter> _dict;
};
