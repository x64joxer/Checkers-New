clear
mkdir -p ../build-Scheduler
g++ main.cpp \
    ConnectionManager/ConnectionManager.cpp \
    Scheduler/Scheduler.cpp \
    TCP/TCPConnection.cpp \
    TCP/TCPServer.cpp \
    SafeQueue/SharedPtrSet/SharedPtrSet.cpp \
    SafeQueue/SharedPtrSet/SharedPtrList.cpp \
    Traces/Traces.cpp \
    -I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Scheduler/scheduler -pthread
