TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    TCP/TCPSocket.cpp \
    Traces/Traces.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    TCP/TCPSocket.h \
    Traces/Traces.h

