#include "ProgramVariables.h"

ProgramVariables::ProgramVariables()
{

}

unsigned long long ProgramVariables::GetSecondsSinceEpoch()
{
    std::time_t result = std::time(nullptr);

    return result;
}

unsigned long long ProgramVariables::GetMaxTimeWaitToWorkers()
{
    return 5;
}

unsigned long long ProgramVariables::GetMaxTimeWaitToServer()
{
    return 5000;
}

QString ProgramVariables::GetServerIP()
{
    return "192.168.0.6";
}

int ProgramVariables::GetServerPort()
{
   return 6000;
}

unsigned int ProgramVariables::GetRecconectingTime()
{
    return 20000;
}

unsigned long long ProgramVariables::GetMaxSecondsToEnd()
{
    return 3;
}

unsigned long long ProgramVariables::GetMaxTimeForIa()
{
    return 5;
}

unsigned long long ProgramVariables::numOfAnalysded = 0;
