clear
mkdir -p ../build-Scheduler
echo Building...

g++ main.cpp \
    ConnectionManager/ConnectionManager.cpp \
    Scheduler/Scheduler.cpp \
    Scheduler/MessageCoder.cpp \
    Scheduler/Client.cpp \
    Peers/Peers.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/ThreadIASimpleBoard.cpp \
    TCP/TCPConnection.cpp \
    TCP/TCPServer.cpp \
    SafeQueue/SharedPtrSet/SharedPtrSet.cpp \
    SafeQueue/SharedPtrSet/SharedPtrList.cpp \
    Traces/Traces.cpp \
    -I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Scheduler/scheduler -pthread


echo ...end of building
