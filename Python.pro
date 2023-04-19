TEMPLATE = lib

QT -= gui
QT += core
CONFIG += c++17 console dll

DEFINES += PYTHON_LIBRARY

HEADERS += include/IPyFunction.h \
           src/PyFunction.h \
           src/PyObjectAdapter.h \
           src/PyClassAdapter.h \
           src/PySimpleAdapter.h \
           PythonRegistrator.h \
           PythonGlobal.h

SOURCES +=  src/PyClassAdapter.cpp \
            src/PyFunction.cpp \
            PythonRegistrator.cpp

#define your Python path here, note python%VERSION%.dll
SRC_PTYHON = B:/ProgramFiles/Python

win32:LIBS += $$SRC_PTYHON/python38.dll \
              -lACore

INCLUDEPATH += $$SRC_PTYHON/include
DEPENDPATH += $$SRC_PTYHON/include

OTHER_FILES += Python.pri

include($$ROOT_CORE/BaseConcepts/BaseConcepts.pri)
include($$ROOT_PROJECT/build.pri)

