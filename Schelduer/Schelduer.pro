TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Scheduler.cpp \
    TCP/TCPConnection.cpp \
    TCP/TCPServer.cpp \
    Traces/Traces.cpp \
    Traces/Traces.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Scheduler.h \
    TCP/TCPConnection.h \
    TCP/TCPServer.h \
    Traces/Traces.h \
    Traces/Traces.h

