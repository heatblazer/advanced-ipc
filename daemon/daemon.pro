TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    daemon.cpp \
    logwriter.cpp

HEADERS += \
    daemon.h \
    logwriter.h \
    defs.h

LIBS += -lpthread
