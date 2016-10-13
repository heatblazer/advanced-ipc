TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    daemon.cpp \
    msg.cpp

HEADERS += \
    daemon.h \
    msg.h
