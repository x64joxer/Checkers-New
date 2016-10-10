#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include "ArgumentParser/ArgumentParser.h"
#include "ProgramVariables/ProgramVariables.h"
#include "QueueTimer/QueueTimer.h"
#include "Worker/Worker.h"
#include <mutex>

using namespace std;

int main(int argc, char* argv[])
{
    ArgumentParser::Parse(argc, argv);

    Traces::SetTraceFolder("trace");
    Worker worker;

    worker.Start();

    return 0;
}



