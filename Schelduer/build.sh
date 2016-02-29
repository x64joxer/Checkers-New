mkdir -p ../build-Scheduler
rm -f ../build-Scheduler/*
g++ main.cpp  -I /opt/local/include/ -L/opt/local/lib -std=c++11 -lboost_system -Wl,-rpath,/opt/local/lib -o ../build-Scheduler/scheduler
