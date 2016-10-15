#
# Common settings for all supprojects.
# You must inclue this in all .pro files.
#


INCLUDEPATH += \
    $$top_srcdir/lib

LIBS += \
    -L$$top_builddir/message -lmessage
