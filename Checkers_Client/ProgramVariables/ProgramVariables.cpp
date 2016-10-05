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
    return "192.168.0.7";
}

int ProgramVariables::GetServerPort()
{
   return 6000;
}

unsigned int ProgramVariables::GetRecconectingTime()
{
    return 5000;
}

unsigned long long ProgramVariables::GetMaxSecondsToEnd()
{
    return 3;
}

unsigned long long ProgramVariables::GetMaxMilisecondsForIa()
{    
    return 5000;
}

//Trace flags

bool ProgramVariables::GetTraceFlagForClass_Board()
{
    return traceFlagForClass_Board;
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

bool ProgramVariables::GetTraceFlagForClass_Peers()
{
    return traceFlagForClass_Peers;
}

bool ProgramVariables::GetTraceFlagForClass_MessageCoder()
{
    return traceFlagForClass_MessageCoder;
}

bool ProgramVariables::GetTraceFlagForClass_ServerState()
{
    return traceFlagForClass_ServerState;
}

bool ProgramVariables::GetTraceFlagForClass_CheckerArea()
{
    return traceFlagForClass_CheckerArea;
}

bool ProgramVariables::GetTraceFlagForClass_TCPHandler()
{
    return traceFlagForClass_TCPHandler;
}

bool ProgramVariables::GetTraceFlagForClass_PossibleMoves()
{
    return traceFlagForClass_PossibleMoves;
}

bool ProgramVariables::traceFlagForClass_Board = false;
bool ProgramVariables::traceFlagForClass_Pawn = false;
bool ProgramVariables::traceFlagForClass_PawnPos = false;
bool ProgramVariables::traceFlagForClass_ThreadIASimpleBoard = false;
bool ProgramVariables::traceFlagForClass_Peers = false;
bool ProgramVariables::traceFlagForClass_MessageCoder = false;
bool ProgramVariables::traceFlagForClass_ServerState = false;
bool ProgramVariables::traceFlagForClass_CheckerArea = false;
bool ProgramVariables::traceFlagForClass_TCPHandler = false;
bool ProgramVariables::traceFlagForClass_PossibleMoves = false;


unsigned long long ProgramVariables::numOfAnalysded = 0;
