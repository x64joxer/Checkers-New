#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include "ProgramVariables/ProgramVariables.h"
#include "QueueTimer/QueueTimer.h"
#include "Worker/Worker.h"
#include <mutex>

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");
    Worker worker;

    worker.Start();

    return 0;
}



