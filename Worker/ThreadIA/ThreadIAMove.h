#ifndef THREADIAMOVE_H
#define THREADIAMOVE_H

#include <atomic>
#include "../Board/Board.h"
#include "ThreadIABoardQueue.h"
#include "ThreadIABoardQueue.cpp"
#include "ThreadIATreeExpander.cpp"

template <unsigned long long QMain>
class ThreadIAMove
{
    public:
        ThreadIAMove();       

        void operator ()(Board * boardWsk,
                         std::atomic_bool * flag,
                         std::atomic<int> *percentSteps,
                         const unsigned short numberOfThreads,
                         const unsigned int refreshMainQueue,
                         const unsigned int numberOfStepsToDo,
                         const KindOfSteps stepKind);

   private:
        ThreadIABoardQueue<QMain> queue;
        void CreateFirstElements();        

};


#include "ThreadIAMove_Impl.h"

#endif // THREADIAMOVE_H


