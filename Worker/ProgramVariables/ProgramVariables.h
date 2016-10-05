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
        static bool GetTraceFlagForClass_MessageCode();
        static bool GetTraceFlagForClass_Worker();

    private:
        static bool traceFlagForClass_Board;
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
        static bool traceFlagForClass_MessageCode;
        static bool traceFlagForClass_Worker;
};

#endif // PROGRAMVARIABLES_H
