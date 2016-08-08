clear
mkdir -p ../build-Worker
echo Building...

g++ -g main.cpp \
    Worker/Worker.cpp \
    TCP/TCPSocket.cpp \
    TCP/TCPSocketBody.cpp \
    QueueTimer/QueueTimer.cpp \
    Worker/MessageCoder.cpp \
    Peers/Peers.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/ThreadIASimpleBoard.cpp \
    SafeQueue/SharedPtrSet/SharedPtrList.cpp \
    Traces/Traces.cpp	-I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Worker/worker -pthread -lboost_thread 

echo ...end of building
