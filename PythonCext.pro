QT -= gui
QT += core
CONFIG += c++17 console
CONFIG -= app_bundle

HEADERS += src/cpp/PyObjectAdapter.h \
           src/cpp/PyClassAdapter.h \
           src/cpp/PyFunctionAdapter.h \
           src/cpp/PySimpleAdapter.h

SOURCES +=  src/cpp/PyClassAdapter.cpp \
            src/cpp/PyFunctionAdapter.cpp \

#define your Python path here, note python%VERSION%.dll
ROOT_PTYHON = B:/ProgramFiles/Python

win32:LIBS += $$ROOT_PTYHON/python38.dll

INCLUDEPATH += $$ROOT_PTYHON/include
DEPENDPATH += $$ROOT_PTYHON/include

SOURCES += \
        main.cpp \

