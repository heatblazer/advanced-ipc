TEMPLATE = subdirs

SUBDIRS += \
    daemon \
    receiver \
    message

daemon.subdir = daemon
message.subdir = message

daemon.depends = message
