QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = QTcpServer
TEMPLATE = app
DESTDIR += $${PWD}/output

SOURCES += \
    main.cpp \
    Server.cpp

FORMS += \
    Server.ui

HEADERS += \
    Server.h
