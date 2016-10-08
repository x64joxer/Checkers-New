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

//Traces

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

bool ProgramVariables::GetTraceFlagForClass_PossibleMoves()
{
    return traceFlagForClass_PossibleMoves;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIASimpleBoard()
{
    return traceFlagForClass_ThreadIASimpleBoard;
}

bool ProgramVariables::GetTraceFlagForClass_IAPossibleMoves()
{
    return traceFlagForClass_IAPossibleMoves;
}

bool ProgramVariables::GetTraceFlagForClass_Peers()
{
    return traceFlagForClass_Peers;
}

bool ProgramVariables::GetTraceFlagForClass_QueueTimer()
{
    return traceFlagForClass_QueueTimer;
}

bool ProgramVariables::GetTraceFlagForClass_SharedMap()
{
    return traceFlagForClass_SharedMap;
}

bool ProgramVariables::GetTraceFlagForClass_SharedPtrSet()
{
    return traceFlagForClass_SharedPtrSet;
}

bool ProgramVariables::GetTraceFlagForClass_SharedPtrList()
{
    return traceFlagForClass_SharedPtrList;
}

bool ProgramVariables::GetTraceFlagForClass_ServerState()
{
    return traceFlagForClass_ServerState;
}

bool ProgramVariables::GetTraceFlagForClass_TCPSocket()
{
    return traceFlagForClass_TCPSocket;
}

bool ProgramVariables::GetTraceFlagForClass_TCPSocketBody()
{
    return traceFlagForClass_TCPSocketBody;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIA()
{
    return traceFlagForClass_ThreadIA;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIABoardQueue()
{
    return traceFlagForClass_ThreadIABoardQueue;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIAMove()
{
    return traceFlagForClass_ThreadIAMove;
}

bool ProgramVariables::GetTraceFlagForClass_ThreadIATreeExpander()
{
    return traceFlagForClass_ThreadIATreeExpander;
}

bool ProgramVariables::GetTraceFlagForClass_Message()
{
    return traceFlagForClass_Message;
}

bool ProgramVariables::GetTraceFlagForClass_MessageCoder()
{
    return traceFlagForClass_MessageCoder;
}

bool ProgramVariables::GetTraceFlagForClass_Worker()
{
    return traceFlagForClass_Worker;
}

bool ProgramVariables::traceFlagForClass_Board = false;
bool ProgramVariables::traceFlagForClass_Counters = false;
bool ProgramVariables::traceFlagForClass_Pawn = false;
bool ProgramVariables::traceFlagForClass_PawnPos = false;
bool ProgramVariables::traceFlagForClass_PossibleMoves = false;
bool ProgramVariables::traceFlagForClass_ThreadIASimpleBoard = false;
bool ProgramVariables::traceFlagForClass_IAPossibleMoves = false;
bool ProgramVariables::traceFlagForClass_Peers = false;
bool ProgramVariables::traceFlagForClass_QueueTimer = false;
bool ProgramVariables::traceFlagForClass_SharedMap = false;
bool ProgramVariables::traceFlagForClass_SharedPtrSet = false;
bool ProgramVariables::traceFlagForClass_SharedPtrList = false;
bool ProgramVariables::traceFlagForClass_ServerState = false;
bool ProgramVariables::traceFlagForClass_TCPSocket = false;
bool ProgramVariables::traceFlagForClass_TCPSocketBody = false;
bool ProgramVariables::traceFlagForClass_ThreadIA = false;
bool ProgramVariables::traceFlagForClass_ThreadIABoardQueue = false;
bool ProgramVariables::traceFlagForClass_ThreadIAMove = false;
bool ProgramVariables::traceFlagForClass_ThreadIATreeExpander = false;
bool ProgramVariables::traceFlagForClass_Message = false;
bool ProgramVariables::traceFlagForClass_MessageCoder = false;
bool ProgramVariables::traceFlagForClass_Worker = false;
