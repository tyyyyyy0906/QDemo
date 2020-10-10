isEmpty(no_project) {

TEMPLATE = subdirs
CONFIG   = ordered
SUBDIRS += \
    QThread \
	QTcp

} else:TEMPLATE = aux
