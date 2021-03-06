#ifndef PROGRAMVARIABLES_H
#define PROGRAMVARIABLES_H

#include <string>

class ProgramVariables
{
    public:
        ProgramVariables();

        static unsigned short GetMaxThredForMessageQueue();
        static void SetMaxThredForMessageQueue(const unsigned short val);
        static std::string GetPortForScheduler();
        static unsigned int GetTimeReserveToSendBestResultToClient();
        static unsigned int GetTimeToSendJobsToFreeWorkers();
        static unsigned int GetMaxTimeoutForMessageResponse();
        static void SetPorForScheduler(const std::string & port);

        //Traffic
        static bool GetTrafficFlag();
        static void SetTrafficFlag(const bool flag);

        //Trace
        static bool GetTraceFlagFor_Notif();

        static bool GetTraceFlagForClass_Board();
        static bool GetTraceFlagForClass_Counters();
        static bool GetTraceFlagForClass_Pawn();
        static bool GetTraceFlagForClass_PawnPos();
        static bool GetTraceFlagForClass_ThreadIASimpleBoard();
        static bool GetTraceFlagForClass_ConnectionManager();
        static bool GetTraceFlagForClass_Peers();
        static bool GetTraceFlagForClass_ProgramVariables();
        static bool GetTraceFlagForClass_QueueTimer();
        static bool GetTraceFlagForClass_QueueTimerList();
        static bool GetTraceFlagForClass_SharedMap();
        static bool GetTraceFlagForClass_SharedPtrList();
        static bool GetTraceFlagForClass_SharedPtrSet();        
        static bool GetTraceFlagForClass_Client();
        static bool GetTraceFlagForClass_JobList();
        static bool GetTraceFlagForClass_Message();
        static bool GetTraceFlagForClass_MessageCoder();
        static bool GetTraceFlagForClass_Scheduler();
        static bool GetTraceFlagForClass_Worker();
        static bool GetTraceFlagForClass_ServerState();
        static bool GetTraceFlagForClass_TCPConnection();
        static bool GetTraceFlagForClass_TCPServer();

   private:
        static std::string serverPort;
        static unsigned short maxThredForMessageQueue;

        //Traffic
        static bool trafficMode;

        //Trace flags
        static bool traceFlagFor_Notif;
        static bool traceFlagForClass_Board;
        static bool traceFlagForClass_Counters;
        static bool traceFlagForClass_Pawn;
        static bool traceFlagForClass_PawnPos;
        static bool traceFlagForClass_ThreadIASimpleBoard;
        static bool traceFlagForClass_ConnectionManager;
        static bool traceFlagForClass_Peers;
        static bool traceFlagForClass_QueueTimer;
        static bool traceFlagForClass_QueueTimerList;
        static bool traceFlagForClass_SharedMap;
        static bool traceFlagForClass_SharedPtrList;
        static bool traceFlagForClass_SharedPtrSet;
        static bool traceFlagForClass_Client;
        static bool traceFlagForClass_JobList;
        static bool traceFlagForClass_Message;
        static bool traceFlagForClass_MessageCoder;
        static bool traceFlagForClass_Scheduler;
        static bool traceFlagForClass_Worker;
        static bool traceFlagForClass_ServerState;
        static bool traceFlagForClass_TCPConnection;
        static bool traceFlagForClass_TCPServer;
};

#define TRACE_FLAG_FOR_Notif if (ProgramVariables::GetTraceFlagFor_Notif())
#define TRACE_FLAG_FOR_CLASS_Board if (ProgramVariables::GetTraceFlagForClass_Board())
#define TRACE_FLAG_FOR_CLASS_Counters if (ProgramVariables::GetTraceFlagForClass_Counters())
#define TRACE_FLAG_FOR_CLASS_Pawn if (ProgramVariables::GetTraceFlagForClass_Pawn())
#define TRACE_FLAG_FOR_CLASS_PawnPos if (ProgramVariables::GetTraceFlagForClass_PawnPos())
#define TRACE_FLAG_FOR_CLASS_ThreadIASimpleBoard if (ProgramVariables::GetTraceFlagForClass_ThreadIASimpleBoard())
#define TRACE_FLAG_FOR_CLASS_ConnectionManager if (ProgramVariables::GetTraceFlagForClass_ConnectionManager())
#define TRACE_FLAG_FOR_CLASS_Peers if (ProgramVariables::GetTraceFlagForClass_Peers())
#define TRACE_FLAG_FOR_CLASS_QueueTimer if (ProgramVariables::GetTraceFlagForClass_QueueTimer())
#define TRACE_FLAG_FOR_CLASS_QueueTimerList if (ProgramVariables::GetTraceFlagForClass_QueueTimerList())
#define TRACE_FLAG_FOR_CLASS_SharedMap if (ProgramVariables::GetTraceFlagForClass_SharedMap())
#define TRACE_FLAG_FOR_CLASS_SharedPtrList if (ProgramVariables::GetTraceFlagForClass_SharedPtrList())
#define TRACE_FLAG_FOR_CLASS_SharedPtrSet if (ProgramVariables::GetTraceFlagForClass_SharedPtrSet())
#define TRACE_FLAG_FOR_CLASS_Client if (ProgramVariables::GetTraceFlagForClass_Client())
#define TRACE_FLAG_FOR_CLASS_JobList if (ProgramVariables::GetTraceFlagForClass_JobList())
#define TRACE_FLAG_FOR_CLASS_Message if (ProgramVariables::GetTraceFlagForClass_Message())
#define TRACE_FLAG_FOR_CLASS_MessageCoder if (ProgramVariables::GetTraceFlagForClass_MessageCoder())
#define TRACE_FLAG_FOR_CLASS_Scheduler if (ProgramVariables::GetTraceFlagForClass_Scheduler())
#define TRACE_FLAG_FOR_CLASS_Worker if (ProgramVariables::GetTraceFlagForClass_Worker())
#define TRACE_FLAG_FOR_CLASS_ServerState if (ProgramVariables::GetTraceFlagForClass_ServerState())
#define TRACE_FLAG_FOR_CLASS_TCPConnection if (ProgramVariables::GetTraceFlagForClass_TCPConnection())
#define TRACE_FLAG_FOR_CLASS_TCPServer if (ProgramVariables::GetTraceFlagForClass_TCPServer())

#endif // PROGRAMVARIABLES_H
