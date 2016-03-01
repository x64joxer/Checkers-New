TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Scheduler.cpp \
    ../Common/TCP/TCPConnection.cpp \
    ../Common/TCP/TCPServer.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Scheduler.h \
    ../Common/TCP/TCPConnection.h \
    ../Common/TCP/TCPServer.h

