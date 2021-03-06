TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Scheduler.cpp \
    TCP/TCPConnection.cpp \
    TCP/TCPServer.cpp \
    Traces/Traces.cpp \
    Traces/Traces.cpp \
    SafeQueue/SharedPtrSet/SharedPtrSet.cpp \
    SafeQueue/SharedPtrSet/SharedPtrList.cpp \
    ConnectionManager.cpp \
    ConnectionManager/ConnectionManager.cpp \
    Scheduler/Scheduler.cpp \
    Scheduler/Message.cpp \
    Scheduler/MessageCoder.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/ThreadIASimpleBoard.cpp \
    Peers/Peers.cpp \
    Scheduler/Client.cpp \
    SafeQueue/SharedMap/SharedMap.cpp \
    ServerState/ServerState.cpp \
    Scheduler/Worker.cpp \
    QueueTimer/QueueTimer.cpp \
    QueueTimer/QueueTimerList.cpp \
    ProgramVariables/ProgramVariables.cpp \
    ArgumentParser/ArgumentParser.cpp \
    Counters/Counters.cpp \
    JobList/JobList.cpp \
    JobList/JobInfo.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Scheduler.h \
    TCP/TCPConnection.h \
    TCP/TCPServer.h \
    Traces/Traces.h \
    Traces/Traces.h \
    SafeQueue/SharedPtrSet/SharedPtrSet.h \
    SafeQueue/SharedPtrSet/SharedPtrList.h \
    ConnectionManager/ConnectionManager.h \
    Scheduler/Scheduler.h \
    Scheduler/Message.h \
    Scheduler/MessageCoder.h \
    Board/Board.h \
    Board/Pawn.h \
    Board/PawnPos.h \
    Board/ThreadIASimpleBoard.h \
    Peers/Peers.h \
    Scheduler/Client.h \
    SafeQueue/SharedMap/SharedMap.h \
    ServerState/ServerState.h \
    Scheduler/Worker.h \
    QueueTimer/QueueTimer.h \
    QueueTimer/QueueTimerList.h \
    ProgramVariables/ProgramVariables.h \
    ArgumentParser/ArgumentParser.h \
    Counters/Counters.h \
    JobList/JobList.h \
    JobList/JobInfo.h

