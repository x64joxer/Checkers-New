#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T06:25:20
#
#-------------------------------------------------

QT       += core gui
QT       += network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Checkers_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GUI/CheckerArea.cpp \
    ../Common/Board/Board.cpp \
    ../Common/Board/Pawn.cpp \
    ../Common/Board/PawnPos.cpp \
    ../Common/Board/PossibleMoves.cpp \
    ../Common/Board/ThreadIASimpleBoard.cpp \
    ../Common/Traces/Traces.cpp

HEADERS  += mainwindow.h \
    GUI/CheckerArea.h \
    ../Common/Board/Board.h \
    ../Common/Board/Pawn.h \
    ../Common/Board/PawnPos.h \
    ../Common/Board/PossibleMoves.h \
    ../Common/Board/ThreadIASimpleBoard.h \
    ../Common/Traces/Traces.h

FORMS    += mainwindow.ui \
    GUI/CheckerArea.ui
