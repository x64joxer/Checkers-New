clear
mkdir -p ../build-Scheduler
echo Building...

g++ main.cpp \
    ArgumentParser/ArgumentParser.cpp \
    Scheduler/Message.cpp \
    ProgramVariables/ProgramVariables.cpp \
    ConnectionManager/ConnectionManager.cpp \
    Scheduler/Scheduler.cpp \
    ServerState/ServerState.cpp \
    Scheduler/MessageCoder.cpp \
    Scheduler/Client.cpp \
    Scheduler/Worker.cpp \
    Peers/Peers.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/ThreadIASimpleBoard.cpp \
    TCP/TCPConnection.cpp \
    TCP/TCPServer.cpp \
    SafeQueue/SharedMap/SharedMap.cpp \
    SafeQueue/SharedPtrSet/SharedPtrSet.cpp \
    SafeQueue/SharedPtrSet/SharedPtrList.cpp \
    QueueTimer/QueueTimer.cpp \
    QueueTimer/QueueTimerList.cpp \
    Traces/Traces.cpp \
    -I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Scheduler/scheduler -pthread -lboost_program_options


echo ...end of building
