#ifndef PROGRAMVARIABLES_H
#define PROGRAMVARIABLES_H

#include <QString>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "../Common/Traces/Traces.h"

class ProgramVariables
{
    public:
        ProgramVariables();  

        static unsigned long long GetSecondsSinceEpoch();
        static unsigned long long GetMaxSecondsToEnd();
        static unsigned long long GetMaxMilisecondsForIa();
        static unsigned long long GetMaxTimeWaitToWorkers();
        static unsigned long long GetMaxTimeWaitToServer();
        static unsigned int GetRecconectingTime();

        static QString GetServerIP();
        static int GetServerPort();

        static void SetNumOfAnalyded(const unsigned long long val) { numOfAnalysded = val; }
        static unsigned long long GetNumOfAnalyded() { return numOfAnalysded; }
        static void IncreaseNumOfAnalyded(const unsigned long long val) { numOfAnalysded+= val; }

        static const unsigned int K4 =  4 * 1024;

        //Trace
        static bool GetTraceFlagForClass_Board();
        static bool GetTraceFlagForClass_Pawn();
        static bool GetTraceFlagForClass_PawnPos();
        static bool GetTraceFlagForClass_ThreadIASimpleBoard();
        static bool GetTraceFlagForClass_Peers();
        static bool GetTraceFlagForClass_MessageCoder();
        static bool GetTraceFlagForClass_ServerState();
        static bool GetTraceFlagForClass_CheckerArea();
        static bool GetTraceFlagForClass_TCPHandler();
        static bool GetTraceFlagForClass_PossibleMoves();

   private:
        static unsigned long long numOfAnalysded;
        static QString serverIP;
        static int serverPort;

        static bool traceFlagForClass_Board;
        static bool traceFlagForClass_Pawn;
        static bool traceFlagForClass_PawnPos;
        static bool traceFlagForClass_ThreadIASimpleBoard;
        static bool traceFlagForClass_Peers;
        static bool traceFlagForClass_MessageCoder;
        static bool traceFlagForClass_ServerState;
        static bool traceFlagForClass_CheckerArea;
        static bool traceFlagForClass_TCPHandler;
        static bool traceFlagForClass_PossibleMoves;
};

#define TRACE_FLAG_FOR_CLASS_Board if (ProgramVariables::GetTraceFlagForClass_Board())
#define TRACE_FLAG_FOR_CLASS_Pawn if (ProgramVariables::GetTraceFlagForClass_Pawn())
#define TRACE_FLAG_FOR_CLASS_PawnPos if (ProgramVariables::GetTraceFlagForClass_PawnPos())
#define TRACE_FLAG_FOR_CLASS_ThreadIASimpleBoard if (ProgramVariables::GetTraceFlagForClass_ThreadIASimpleBoard())
#define TRACE_FLAG_FOR_CLASS_Peers if (ProgramVariables::GetTraceFlagForClass_Peers())
#define TRACE_FLAG_FOR_CLASS_ServerState if (ProgramVariables::GetTraceFlagForClass_ServerState())
#define TRACE_FLAG_FOR_CLASS_MessageCoder if (ProgramVariables::GetTraceFlagForClass_MessageCoder())
#define TRACE_FLAG_FOR_CLASS_CheckerArea if (ProgramVariables::GetTraceFlagForClass_CheckerArea())
#define TRACE_FLAG_FOR_CLASS_TCPHandler if (ProgramVariables::GetTraceFlagForClass_TCPHandler())
#define TRACE_FLAG_FOR_CLASS_PossibleMoves if (ProgramVariables::GetTraceFlagForClass_PossibleMoves())


#endif // PROGRAMVARIABLES_H
