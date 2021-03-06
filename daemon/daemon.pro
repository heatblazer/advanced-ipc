TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    daemon.cpp \
    thread.cpp

HEADERS += \
    daemon.h \
    thread.h \
    defs.h

LIBS += -lpthread



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../message/release/ -lmessage
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../message/debug/ -lmessage
else:unix: LIBS += -L$$OUT_PWD/../message/ -lmessage

INCLUDEPATH += $$PWD/../message
DEPENDPATH += $$PWD/../message

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ring-buffer/release/ -lring-buffer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ring-buffer/debug/ -lring-buffer
else:unix: LIBS += -L$$OUT_PWD/../ring-buffer/ -lring-buffer

INCLUDEPATH += $$PWD/../ring-buffer
DEPENDPATH += $$PWD/../ring-buffer
