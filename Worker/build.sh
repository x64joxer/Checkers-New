clear
mkdir -p ../build-Worker
echo Building...

g++ main.cpp \
    TCP/TCPSocket.cpp -I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Worker/worker -pthread

echo ...end of building
