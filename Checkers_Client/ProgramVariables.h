#ifndef PROGRAMVARIABLES_H
#define PROGRAMVARIABLES_H

#include <QString>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "Traces/Traces.h"

class ProgramVariables
{
    public:
        ProgramVariables();  

        static unsigned long long GetSecondsSinceEpoch();
        static unsigned long long GetMaxSecondsToEnd();
        static unsigned long long GetMaxTimeForIa();
        static unsigned long long GetMaxTimeWaitToWorkers();
        static unsigned long long GetMaxTimeWaitToServer();
        static unsigned int GetRecconectingTime();

        static QString GetServerIP();
        static int GetServerPort();

        static void SetNumOfAnalyded(const unsigned long long val) { numOfAnalysded = val; }
        static unsigned long long GetNumOfAnalyded() { return numOfAnalysded; }
        static void IncreaseNumOfAnalyded(const unsigned long long val) { numOfAnalysded+= val; }

        static const unsigned int K4 =  4 * 1024;

    private:                 
        static unsigned long long numOfAnalysded;        
};

#endif // PROGRAMVARIABLES_H
