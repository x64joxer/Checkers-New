#include <iostream>
#include <boost/filesystem/operations.hpp>

#include "Scheduler.h"
#include "TCP/TCPServer.h"

using namespace std;

int main()
{
    cout << "Main";

    Scheduler workScheduler;

    TCPServer *server;
    server = new TCPServer("0.0.0.0", "6000");
    server->SetScheduler(&workScheduler);
    server->Run();

    return 0;
}

