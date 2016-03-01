#include "Scheduler.h"

Scheduler::Scheduler()
{

}

void Scheduler::Init()
{
    std::cout << "Scheluder" << std::endl;
    server = new TCPServer("0.0.0.0", "6000");
    server->Run();
}
