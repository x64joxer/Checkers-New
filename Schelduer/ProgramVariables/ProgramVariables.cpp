#include "ProgramVariables.h"

ProgramVariables::ProgramVariables()
{

}

unsigned int ProgramVariables::GetMaxThredForMessageQueue()
{
    return 2;
}

std::string ProgramVariables::GetPortForScheduler()
{
    return "6000";
}

unsigned int ProgramVariables::GetTimeReserveToSendBestResultToClient()
{
    return 1000;
}

unsigned int ProgramVariables::GetMaxTimeoutForMessageResponse()
{
    return 1000;
}