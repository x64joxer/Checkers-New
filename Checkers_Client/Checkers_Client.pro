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
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/PossibleMoves.cpp \
    Board/ThreadIASimpleBoard.cpp \
    TCP/TCPHandler.cpp \
    ProgramVariables.cpp \
    Traces/Traces.cpp \
    MessageCoder/MessageCoder.cpp \
    Peers/Peers.cpp
HEADERS  += mainwindow.h \
    GUI/CheckerArea.h \
    Board/Board.h \
    Board/Pawn.h \
    Board/PawnPos.h \
    Board/PossibleMoves.h \
    Board/ThreadIASimpleBoard.h \
    TCP/TCPHandler.h \
    ProgramVariables.h \
    Traces/Traces.h \
    MessageCoder/MessageCoder.h \
    Peers/Peers.h

FORMS    += mainwindow.ui \
    GUI/CheckerArea.ui
