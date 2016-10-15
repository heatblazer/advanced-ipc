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



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../message/release/ -lmessage
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../message/debug/ -lmessage
else:unix: LIBS += -L$$OUT_PWD/../message/ -lmessage

INCLUDEPATH += $$PWD/../message
DEPENDPATH += $$PWD/../message
