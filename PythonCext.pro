QT -= gui
QT += core
CONFIG += c++17 console
CONFIG -= app_bundle

HEADERS += src/cpp/class.h \
           src/cpp/func.h \
           src/cpp/simple.h

#DEFINES += __cplusplus
win32:LIBS += B:/ProgramFiles/Python/python38.dll

INCLUDEPATH += B:/ProgramFiles/Python/include
DEPENDPATH += B:/ProgramFiles/Python/include

SOURCES += \
        main.cpp \

