#ifndef THREADIABOARDQUEUE_H
#define THREADIABOARDQUEUE_H

#include <cmath>
#include <condition_variable>
#include <mutex>
#include "../Board/Board.h"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

template <unsigned long long size>
class ThreadIABoardQueue
{
    public:
        ThreadIABoardQueue();
        ~ThreadIABoardQueue();
        Board PopFront(const unsigned short num);
        inline void PushBack(const Board & board);
        inline void PushBackDoNotForget(const Board & board);
        Board GetBestResult() const;
        void GetBestResultMultiThread(const bool make, const unsigned long long start, const unsigned long long stop, bool make2, const unsigned long long start2, const unsigned long long stop2, Board *best) const;
        void Clear();
        Board First(const bool remove);      
        unsigned long long GetFirstNumber() const;
        void NotifyRest();
        unsigned long long Size() const;
        unsigned long long SizeDoNotForget() const;
        Board & At(const unsigned long long number) const;
        void SetConditionVariable(std::condition_variable *cond) { condition_var = cond;  }
        std::condition_variable * GetConditionVariable() { return condition_var;  }

    private:
        bool Empty() const { if (numberOfElements==0) { return true;}; return false;  }
        void SetWorkerFlag(const bool flag, const unsigned short number);

        Board *queue;        
        unsigned long long first;
        unsigned long long last;
        unsigned long long numberOfElements;

        Board *doNotForgetqueue;
        unsigned long long doNotForgetnumberOfElements;

        mutable std::mutex mutex_guard;
        std::condition_variable *condition_var;
        unsigned short workersFlags;
};

#endif // THREADIABOARDQUEUE_H
