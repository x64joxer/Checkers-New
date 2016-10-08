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

    private:
        static std::mutex mutex;
        static unsigned long long numberOfAnalysedBoard;
};

#endif // COUNTERS_H
