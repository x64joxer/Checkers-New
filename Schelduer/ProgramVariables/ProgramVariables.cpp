#include "ProgramVariables.h"

ProgramVariables::ProgramVariables()
{

}

unsigned int ProgramVariables::GetMaxThredForMessageQueue()
{
    return 10;
}

std::string ProgramVariables::GetPortForScheduler()
{
    return serverPort;
}

unsigned int ProgramVariables::GetTimeReserveToSendBestResultToClient()
{
    return 1000;
}

unsigned int ProgramVariables::GetTimeToSendJobsToFreeWorkers()
{
    return 1500;
}

unsigned int ProgramVariables::GetMaxTimeoutForMessageResponse()
{
    return 1000;
}

void ProgramVariables::SetPorForScheduler(const std::string & port)
{
    serverPort = port;
}

//Traffic

bool ProgramVariables::GetTrafficFlag()
{
    return trafficMode;
}

void ProgramVariables::SetTrafficFlag(const bool flag)
{
    trafficMode = flag;
}

//Trace flags

bool ProgramVariables::GetTraceFlagForClass_Board()
{
    return traceFlagForClass_Board;
}

bool ProgramVariables::GetTraceFlagForClass_Counters()
{
    return traceFlagForClass_Counters;
}

bool ProgramVariables::GetTraceFlagForClass_Pawn()
{
    return traceFlagForClass_Pawn;
}

bool ProgramVariables::GetTraceFlagForClass_PawnPos()
{
    return traceFlagForClass_PawnPos;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIASimpleBoard()
{
    return traceFlagForClass_ThreadIASimpleBoard;
}

bool ProgramVariables::GetTraceFlagForClass_ConnectionManager()
{
    return traceFlagForClass_ConnectionManager;
}

bool ProgramVariables::GetTraceFlagForClass_Peers()
{
    return traceFlagForClass_Peers;
}

bool ProgramVariables::GetTraceFlagForClass_QueueTimer()
{
    return traceFlagForClass_QueueTimer;
}

bool ProgramVariables::GetTraceFlagForClass_QueueTimerList()
{
    return traceFlagForClass_QueueTimerList;
}

bool ProgramVariables::GetTraceFlagForClass_SharedMap()
{
    return traceFlagForClass_SharedMap;
}

bool ProgramVariables::GetTraceFlagForClass_SharedPtrList()
{
    return traceFlagForClass_SharedPtrList;
}

bool ProgramVariables::GetTraceFlagForClass_SharedPtrSet()
{
    return traceFlagForClass_SharedPtrSet;
}

bool ProgramVariables::GetTraceFlagForClass_Client()
{
    return traceFlagForClass_Client;
}

bool ProgramVariables::GetTraceFlagForClass_Message()
{
    return traceFlagForClass_Message;
}

bool ProgramVariables::GetTraceFlagForClass_MessageCoder()
{
    return traceFlagForClass_MessageCoder;
}

bool ProgramVariables::GetTraceFlagForClass_Scheduler()
{
    return traceFlagForClass_Scheduler;
}

bool ProgramVariables::GetTraceFlagForClass_Worker()
{
    return traceFlagForClass_Worker;
}

bool ProgramVariables::GetTraceFlagForClass_ServerState()
{
    return traceFlagForClass_ServerState;
}

bool ProgramVariables::GetTraceFlagForClass_TCPConnection()
{
    return traceFlagForClass_TCPConnection;
}

bool ProgramVariables::GetTraceFlagForClass_TCPServer()
{
    return traceFlagForClass_TCPServer;
}

std::string ProgramVariables::serverPort = "6000";

bool ProgramVariables::trafficMode = false;

bool ProgramVariables::traceFlagForClass_Board = false;
bool ProgramVariables::traceFlagForClass_Counters = false;
bool ProgramVariables::traceFlagForClass_Pawn = false;
bool ProgramVariables::traceFlagForClass_PawnPos = false;
bool ProgramVariables::traceFlagForClass_ThreadIASimpleBoard = false;
bool ProgramVariables::traceFlagForClass_ConnectionManager = false;
bool ProgramVariables::traceFlagForClass_Peers = false;
bool ProgramVariables::traceFlagForClass_QueueTimer = false;
bool ProgramVariables::traceFlagForClass_QueueTimerList = false;
bool ProgramVariables::traceFlagForClass_SharedMap = false;
bool ProgramVariables::traceFlagForClass_SharedPtrList = false;
bool ProgramVariables::traceFlagForClass_SharedPtrSet = false;
bool ProgramVariables::traceFlagForClass_Client = false;
bool ProgramVariables::traceFlagForClass_Message = false;
bool ProgramVariables::traceFlagForClass_MessageCoder = false;
bool ProgramVariables::traceFlagForClass_Scheduler = false;
bool ProgramVariables::traceFlagForClass_Worker = false;
bool ProgramVariables::traceFlagForClass_ServerState = false;
bool ProgramVariables::traceFlagForClass_TCPConnection = false;
bool ProgramVariables::traceFlagForClass_TCPServer = false;
