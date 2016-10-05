#include <iostream>
#include <boost/filesystem/operations.hpp>

#include "ConnectionManager/ConnectionManager.h"
#include "TCP/TCPServer.h"
#include "Traces/Traces.h"
#include "Scheduler/Scheduler.h"

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");

    ConnectionManager connectionMnager;

    //Start scheduler
    Scheduler scheduler;
    scheduler.SetConnectionManager(&connectionMnager);
    scheduler.Start(ProgramVariables::GetMaxThredForMessageQueue());

    //Start TCP server
    TCPServer *server;
    server = new TCPServer("0.0.0.0", ProgramVariables::GetPortForScheduler());
    server->SetConnectionManager(&connectionMnager);
    server->Run();

    return 0;
}

