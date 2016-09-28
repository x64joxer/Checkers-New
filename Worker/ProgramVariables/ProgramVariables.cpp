#include "ProgramVariables.h"

ProgramVariables::ProgramVariables()
{

}

unsigned int ProgramVariables::GetMaxThredForIa()
{
    return 4;
}

std::string ProgramVariables::GetPortForScheduler()
{
    return "6000";
}

std::string ProgramVariables::GetIpForScheduler()
{
    return "192.168.0.7";
}

unsigned int ProgramVariables::GetTimeReserveToSendBestResultToScheduler()
{
    return 1000;
}

unsigned int ProgramVariables::GetMaxTimeoutForMessageResponse()
{
    return 1000;
}
