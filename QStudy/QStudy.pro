isEmpty(no_project) {

TEMPLATE = subdirs
CONFIG   = ordered
SUBDIRS += \
    QThread

} else:TEMPLATE = aux
