#pragma once

#include <QObject>

class IPyFunction : public QObject
{
    Q_OBJECT

public:
    //! @param pyScriptPath - absolute path to a script dir
    //! @param pyScriptName - script filename without ".py" extension
    IPyFunction(const QString& pyScriptPath, const QString& pyScriptName, QObject* parent = nullptr)
        : QObject(parent)
        , _filePath(pyScriptPath)
        , _fileName(pyScriptName){};

    virtual ~IPyFunction() {}

    //! Loading python interpreter and module _fileName.py into it
    virtual bool init() = 0;

    //! Predefined function: func is setting string (from c++) -
    /// func is getting modified (python) string back
    virtual QByteArray python_func_get_str(const QString& funcName, const QByteArray& val, bool* ok = nullptr) = 0;

    //!Getting value of type int named "val"
    virtual qint32 python_func_get_val(const QString& varName, bool* ok = nullptr) = 0;

protected:
    const QString _filePath;
    const QString _fileName;
};
