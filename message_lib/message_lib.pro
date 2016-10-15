#-------------------------------------------------
#
# Project created by QtCreator 2016-10-15T13:12:30
#
#-------------------------------------------------

QT       -= core gui

TARGET = message_lib
TEMPLATE = lib

DEFINES += MESSAGE_LIB_LIBRARY

SOURCES += message_lib.cpp

HEADERS += message_lib.h\
        message_lib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
