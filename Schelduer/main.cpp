#include <iostream>
#include <boost/filesystem/operations.hpp>

#include "ConnectionManager/ConnectionManager.h"
#include "TCP/TCPServer.h"
#include "Traces/Traces.h"

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");

    ConnectionManager workScheduler;

    TCPServer *server;
    server = new TCPServer("0.0.0.0", "6000");
    server->SetConnectionManager(&workScheduler);
    server->Run();

    return 0;
}

