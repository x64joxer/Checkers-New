TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    TCP/TCPSocket.cpp \
    Traces/Traces.cpp \
    Worker/MessageCoder.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/PossibleMoves.cpp \
    Board/ThreadIASimpleBoard.cpp \
    Peers/Peers.cpp \
    ServerState/ServerState.cpp \
    Worker/Message.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    TCP/TCPSocket.h \
    Traces/Traces.h \
    Worker/MessageCoder.h \
    Board/Board.h \
    Board/Pawn.h \
    Board/PawnPos.h \
    Board/PossibleMoves.h \
    Board/ThreadIASimpleBoard.h \
    Peers/Peers.h \
    ServerState/ServerState.h \
    Worker/Message.h
