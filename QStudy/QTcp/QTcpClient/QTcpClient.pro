QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = QTcpClient
TEMPLATE = app
DESTDIR += $${PWD}/output

SOURCES += \
    main.cpp \
    Client.cpp

FORMS += \
    Client.ui

HEADERS += \
    Client.h
