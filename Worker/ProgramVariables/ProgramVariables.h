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

};

#endif // PROGRAMVARIABLES_H
