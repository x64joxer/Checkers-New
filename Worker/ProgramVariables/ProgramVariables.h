#ifndef PROGRAMVARIABLES_H
#define PROGRAMVARIABLES_H

#include <string>

class ProgramVariables
{
    public:
        ProgramVariables();

        static unsigned int GetMaxThredForIa();
        static std::string GetPortForScheduler();
        static std::string GetIpForScheduler();
        static unsigned int GetTimeReserveToSendBestResultToScheduler();
        static unsigned int GetMaxTimeoutForMessageResponse();

        //Traces flags
        static bool GetTraceFlagForClass_Board();
        static bool GetTraceFlagForClass_Counters();
        static bool GetTraceFlagForClass_Pawn();
        static bool GetTraceFlagForClass_PawnPos();
        static bool GetTraceFlagForClass_PossibleMoves();
        static bool GetTraceFlagForClass_ThreadIASimpleBoard();
        static bool GetTraceFlagForClass_ThreadIABoardQueue();
        static bool GetTraceFlagForClass_IAPossibleMoves();
        static bool GetTraceFlagForClass_Peers();
        static bool GetTraceFlagForClass_QueueTimer();
        static bool GetTraceFlagForClass_SharedMap();
        static bool GetTraceFlagForClass_SharedPtrSet();
        static bool GetTraceFlagForClass_SharedPtrList();
        static bool GetTraceFlagForClass_ServerState();
        static bool GetTraceFlagForClass_TCPSocket();
        static bool GetTraceFlagForClass_TCPSocketBody();
        static bool GetTraceFlagForClass_ThreadIA();
        static bool GetTraceFlagForClass_ThreadIAMove();
        static bool GetTraceFlagForClass_ThreadIATreeExpander();
        static bool GetTraceFlagForClass_Message();
        static bool GetTraceFlagForClass_MessageCoder();
        static bool GetTraceFlagForClass_Worker();

    private:
        static bool traceFlagForClass_Board;
        static bool traceFlagForClass_Counters;
        static bool traceFlagForClass_Pawn;
        static bool traceFlagForClass_PawnPos;
        static bool traceFlagForClass_PossibleMoves;
        static bool traceFlagForClass_ThreadIASimpleBoard;
        static bool traceFlagForClass_IAPossibleMoves;
        static bool traceFlagForClass_Peers;
        static bool traceFlagForClass_QueueTimer;
        static bool traceFlagForClass_SharedMap;
        static bool traceFlagForClass_SharedPtrList;
        static bool traceFlagForClass_SharedPtrSet;
        static bool traceFlagForClass_ServerState;
        static bool traceFlagForClass_TCPSocket;
        static bool traceFlagForClass_TCPSocketBody;
        static bool traceFlagForClass_ThreadIA;
        static bool traceFlagForClass_ThreadIABoardQueue;
        static bool traceFlagForClass_ThreadIAMove;
        static bool traceFlagForClass_ThreadIATreeExpander;
        static bool traceFlagForClass_Message;
        static bool traceFlagForClass_MessageCoder;
        static bool traceFlagForClass_Worker;
};

#define TRACE_FLAG_FOR_CLASS_Board if (ProgramVariables::GetTraceFlagForClass_Board())
#define TRACE_FLAG_FOR_CLASS_Counters if (ProgramVariables::GetTraceFlagForClass_Counters())
#define TRACE_FLAG_FOR_CLASS_Pawn if (ProgramVariables::GetTraceFlagForClass_Pawn())
#define TRACE_FLAG_FOR_CLASS_PawnPos if (ProgramVariables::GetTraceFlagForClass_PawnPos())
#define TRACE_FLAG_FOR_CLASS_PossibleMoves if (ProgramVariables::GetTraceFlagForClass_PossibleMoves())
#define TRACE_FLAG_FOR_CLASS_ThreadIASimpleBoard if (ProgramVariables::GetTraceFlagForClass_ThreadIASimpleBoard())
#define TRACE_FLAG_FOR_CLASS_IAPossibleMoves if (ProgramVariables::GetTraceFlagForClass_IAPossibleMoves())
#define TRACE_FLAG_FOR_CLASS_Peers if (ProgramVariables::GetTraceFlagForClass_Peers())
#define TRACE_FLAG_FOR_CLASS_QueueTimer if (ProgramVariables::GetTraceFlagForClass_QueueTimer())
#define TRACE_FLAG_FOR_CLASS_SharedMap if (ProgramVariables::GetTraceFlagForClass_SharedMap())
#define TRACE_FLAG_FOR_CLASS_SharedPtrList if (ProgramVariables::GetTraceFlagForClass_SharedPtrList())
#define TRACE_FLAG_FOR_CLASS_SharedPtrSet if (ProgramVariables::GetTraceFlagForClass_SharedPtrSet())
#define TRACE_FLAG_FOR_CLASS_ServerState if (ProgramVariables::GetTraceFlagForClass_ServerState())
#define TRACE_FLAG_FOR_CLASS_TCPSocket if (ProgramVariables::GetTraceFlagForClass_TCPSocket())
#define TRACE_FLAG_FOR_CLASS_TCPSocketBody if (ProgramVariables::GetTraceFlagForClass_TCPSocketBody())
#define TRACE_FLAG_FOR_CLASS_ThreadIA if (ProgramVariables::GetTraceFlagForClass_ThreadIA())
#define TRACE_FLAG_FOR_CLASS_ThreadIABoardQueue if (ProgramVariables::GetTraceFlagForClass_ThreadIABoardQueue())
#define TRACE_FLAG_FOR_CLASS_ThreadIAMove if (ProgramVariables::GetTraceFlagForClass_ThreadIAMove())
#define TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander if (ProgramVariables::GetTraceFlagForClass_ThreadIATreeExpander())
#define TRACE_FLAG_FOR_CLASS_Message if (ProgramVariables::GetTraceFlagForClass_Message())
#define TRACE_FLAG_FOR_CLASS_MessageCoder if (ProgramVariables::GetTraceFlagForClass_MessageCoder())
#define TRACE_FLAG_FOR_CLASS_Worker if (ProgramVariables::GetTraceFlagForClass_Worker())

#endif // PROGRAMVARIABLES_H
