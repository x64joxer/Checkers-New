TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    TCP/TCPSocket.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    TCP/TCPSocket.h

