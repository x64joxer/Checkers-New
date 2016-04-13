clear
mkdir -p ../build-Worker
echo Building...

g++ main.cpp \
    TCP/TCPSocket.cpp \
    Worker/MessageCoder.cpp \
    Peers/Peers.cpp \
    Board/Board.cpp \
    Board/Pawn.cpp \
    Board/PawnPos.cpp \
    Board/ThreadIASimpleBoard.cpp \
    Traces/Traces.cpp	-I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Worker/worker -pthread -lboost_thread 

echo ...end of building
