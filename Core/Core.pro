#-------------------------------------------------
#
# Project created by QtCreator 2015-10-17T18:52:49
#
#-------------------------------------------------

QT       -= gui

TARGET = Core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

SOURCES += core.cpp \
    global.cpp \
    profile.cpp \
    serial.cpp

HEADERS += core.h\
        core_global.h \
    global.h \
    profile.h \
    serial.h \
    stable.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
