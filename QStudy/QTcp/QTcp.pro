isEmpty(no_project) {

TEMPLATE = subdirs
CONFIG   = ordered
SUBDIRS += \
    QTcpClient \
    QTcpServer

} else:TEMPLATE = aux
