#ifndef COUNTERS_H
#define COUNTERS_H

#include <mutex>
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class Counters
{
    public:
        Counters();

        static void ClearCounterNumberOfAnalysedBoard();
        static void AddToCounterNumberOfAnalysedBoard(const unsigned long long tmpNumberOfAnalysedBoard);
        static const unsigned long long GetCounterNumberOfAnalysedBoard();

        static void ClearCounterNumberOfBestResultMessagesSend();
        static void AddToCounterNumberOfBestResultMessagesSend(const unsigned long long tmpnumberOfBestResultMessagesSend);
        static const unsigned long long GetCounterNumberOfBestResultMessagesSend();

    private:
        static std::mutex mutex;
        static unsigned long long numberOfAnalysedBoard;
        static unsigned long long numberOfBestResultMessagesSend;
};

#endif // COUNTERS_H
