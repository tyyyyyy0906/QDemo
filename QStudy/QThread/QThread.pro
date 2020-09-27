QT += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = QThread
TEMPLATE = app
DESTDIR += $${PWD}/output

SOURCES += \
    main.cpp     \
    Widget.cpp   \
    Worker.cpp   \
    TaskPool.cpp \
    Concurrent.cpp

HEADERS += \
    Widget.h   \
    Worker.h   \
    TaskPool.h \
    Concurrent.h

FORMS += \
    Widget.ui
