/**
 * gcc func.c $(python3-config --includes --ldflags) -o func && ./func
 */
#pragma once

#include "PyObjectAdapter.h"

class PyFunctionAdapter
{
public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    PyFunctionAdapter(const QString& pyScriptPath, const QString& pyScriptName);

    ~PyFunctionAdapter();

    /**
     * Loading python interpreter and module _fileName.py into it
     */
    bool init();

    /**
   * Predefined function: func is setting string (from c++) -
   * func is getting modified (python) string back
   */
    QByteArray python_func_get_str(const QString& funcName, const QByteArray& val, bool* ok = nullptr);

    /**
    * Getting value of type int named "val"
    */
    qint32 python_func_get_val(const QString& varName, bool* ok = nullptr);

private:
    const QString _filePath;
    const QString _fileName;

    QScopedPointer<PyObjectAdapter> _dict;
};
