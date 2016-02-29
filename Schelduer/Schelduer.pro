TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Scheduler.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Scheduler.h

