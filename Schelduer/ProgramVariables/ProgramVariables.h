#ifndef PROGRAMVARIABLES_H
#define PROGRAMVARIABLES_H

#include <string>

class ProgramVariables
{
    public:
        ProgramVariables();

        static unsigned int GetMaxThredForMessageQueue();
        static std::string GetPortForScheduler();
        static unsigned int GetTimeReserveToSendBestResultToClient();
        static unsigned int GetTimeToSendJobsToFreeWorkers();
        static unsigned int GetMaxTimeoutForMessageResponse();

};

#endif // PROGRAMVARIABLES_H
