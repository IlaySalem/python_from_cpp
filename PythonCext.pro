QT -= gui
QT += core
CONFIG += c++17 console
CONFIG -= app_bundle

HEADERS += src/cpp/PyClassAdapter.h \
           src/cpp/PyFunctionAdapter.h \
           src/cpp/PySimpleAdapter.h

#DEFINES += __cplusplus
win32:LIBS += B:/ProgramFiles/Python/python38.dll

INCLUDEPATH += B:/ProgramFiles/Python/include
DEPENDPATH += B:/ProgramFiles/Python/include

SOURCES += \
        main.cpp \

