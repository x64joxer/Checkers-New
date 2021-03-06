#ifndef THREADIAMOVE_H
#define THREADIAMOVE_H

#include <atomic>
#include "../Board/Board.h"
#include "ThreadIABoardQueue.h"
#include "ThreadIABoardQueue.cpp"
#include "ThreadIATreeExpander.cpp"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"
#include "../Counters/Counters.h"

template <unsigned long long QMain>
class ThreadIAMove
{
    public:
        ThreadIAMove();               

        void operator ()(Board * boardWsk,
                         std::atomic_bool * flag,
                         std::atomic_bool * stopFlag,
                         std::atomic<int> *percentSteps,
                         const unsigned short numberOfThreads,
                         const unsigned int refreshMainQueue,
                         const unsigned int numberOfStepsToDo,
                         const KindOfSteps stepKind,
                         const bool isFirstWorker,
                         std::atomic<bool> * canYouTakeBoardToReturnFast,
                         const unsigned int numberOfReturnFast,
                         std::atomic_bool * canNotMoveFlag);

        ThreadIABoardQueue<QMain> * GetThreadIABoardQueueWsk() { return &queue; }
   private:
        ThreadIABoardQueue<QMain> queue;
        void CreateFirstElements();
        void SetOriginToAll();

};

template  <unsigned long long QMain>
ThreadIAMove<QMain>::ThreadIAMove()
{

}

template  <unsigned long long QMain>
void ThreadIAMove<QMain>::operator ()(Board * boardWsk, std::atomic_bool * flag, std::atomic_bool * stopFlag, std::atomic<int> *percentSteps, const unsigned short numberOfThreads, const unsigned int refreshMainQueue, const unsigned int numberOfStepsToDo, const KindOfSteps stepKind, const bool isFirstWorker, std::atomic<bool> * canYouTakeBoardToReturnFast, const unsigned int numberOfReturnFast, std::atomic_bool * canNotMoveFlag)
{
    const unsigned short maxThreads = numberOfThreads + 1;
    std::thread iaThread[maxThreads];
    ThreadIATreeExpander<QMain,5000> expander[maxThreads];
    unsigned int numberOfSteps = numberOfStepsToDo / numberOfThreads;

    //TO_DO ProgramVariables::SetNumOfAnalyded(0);

    //TO_DO if (messageHandler) messageHandler->SetBoardQueue(&queue);

    if (stepKind == KindOfSteps::Time)
    {
        numberOfSteps = numberOfStepsToDo;
    };

    //Create first elements
    Board temp = *boardWsk;
    temp.StartWhite();
    temp.SetWhitePatchEnd(false);

    queue.PushBack(temp);
    CreateFirstElements();

    if (queue.Size() + queue.SizeDoNotForget() == 0)
    {
        *canNotMoveFlag = true;
        queue.NotifyRest();
    } else
    {
        if (queue.Size() >= numberOfReturnFast) *canYouTakeBoardToReturnFast = true;

        //When all black killed
        if (queue.First(0).GetNumberOfBlack() == 0)
        {
            *boardWsk = queue.First(0);
            *flag = true;
            queue.NotifyRest();
        } else
        //Can not move or all black killed
        if (queue.Size() == 0)
        {
            *flag = true;
            queue.NotifyRest();
        } else
        {
            //Set origin to all
            //TO DO Only when we are not second worker
             if (isFirstWorker) SetOriginToAll();

            //Start threads
            for (unsigned short i=1;i<=numberOfThreads;i++)
            {
                expander[i].SetMainBoardQueue(&queue);
                iaThread[i] = std::move(std::thread(&ThreadIATreeExpander<QMain,5000>::ExpandWithoutQueue,
                                                    &expander[i],
                                                    numberOfSteps,
                                                    refreshMainQueue,
                                                    i,
                                                    percentSteps,
                                                    stepKind,
                                                    stopFlag
                                                    ));

            };

            //TO_DEL Start sharing jobs
            //TO_DEL if (messageHandler) messageHandler->StartSharing(ProgramVariables::GetSecondsSinceEpoch());

            for (unsigned short i=1;i<=numberOfThreads;i++)
            {
                iaThread[i].join();
            };

            if (*stopFlag == false)
            {
                TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Waiting for all workers...";
                //TO_DEL {
                //TO_DEL    unsigned long long start = ProgramVariables::GetSecondsSinceEpoch();
                //TO_DEL    while (ProgramVariables::GetSecondsSinceEpoch() - start < ProgramVariables::GetMaxTimeWaitToWorkers())
                //TO_DEL    {
                //TO_DEL        if (WorkerAgent::GetBusyStateNumber() == 0) break;
                //TO_DEL    }
                //TO_DEL }

                //TO_DEL if (messageHandler) messageHandler->StopSharing();
                //TO_DO ProgramVariables::NotifyOne();

                Counters::AddToCounterNumberOfAnalysedBoard(queue.Size() + queue.SizeDoNotForget());

                TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces()<< "\n" << "LOG: -------------------------------------------------------------------";
                TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces()<< "\n" << "LOG: Total num of analysed elements: " << Counters::GetCounterNumberOfAnalysedBoard();
                TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces()<< "\n" << "LOG: -------------------------------------------------------------------";

                if (numberOfThreads <2)
                {
                    temp = queue.GetBestResult();
                } else
                {
                //NEW METHOD
                    unsigned long long minElements = 2000;
                    unsigned short numOfThreads = numberOfThreads;

                    if (queue.Size() / minElements >= queue.SizeDoNotForget() / minElements)
                    {
                        if (queue.Size() / minElements < numOfThreads) numOfThreads = queue.Size() / minElements;
                    } else
                    {
                        if (queue.SizeDoNotForget() / minElements < numOfThreads) numOfThreads = queue.SizeDoNotForget() / minElements;
                    };
                    if (numOfThreads == 0) numOfThreads = 1;

                    unsigned long long firstQueueElelemtsOnThread = queue.Size() / numberOfThreads;
                    unsigned long long secondQueueElelemtsOnThread = queue.SizeDoNotForget() / numberOfThreads;

                    if (firstQueueElelemtsOnThread == 0)
                    {
                        if (queue.Size()>0) firstQueueElelemtsOnThread = queue.Size()-1;
                    };
                    if (secondQueueElelemtsOnThread == 0) secondQueueElelemtsOnThread = queue.SizeDoNotForget();

                    unsigned long long start = queue.GetFirstNumber();
                    unsigned long long start2 = 0;
                    unsigned long long stop = firstQueueElelemtsOnThread;
                    unsigned long long stop2 = secondQueueElelemtsOnThread;
                    bool flag1 = true;
                    bool flag2 = true;
                    Board best[numOfThreads];

                    for (unsigned short i=1;i<=numOfThreads;i++)
                    {
                        TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Start sharing for thread " << i;
                        if (firstQueueElelemtsOnThread ==0) flag1 = false;
                        if (secondQueueElelemtsOnThread == 0) flag2 = false;
                        if (firstQueueElelemtsOnThread >= queue.GetFirstNumber()+ queue.Size()) firstQueueElelemtsOnThread = (queue.GetFirstNumber()+ queue.Size())-1;
                        if (secondQueueElelemtsOnThread >= queue.SizeDoNotForget()) secondQueueElelemtsOnThread = queue.SizeDoNotForget() -1;
                        stop = start + firstQueueElelemtsOnThread;
                        stop2 = start2 + secondQueueElelemtsOnThread;

                        iaThread[i] = std::move(std::thread(&ThreadIABoardQueue<QMain>::GetBestResultMultiThread,
                                                            &queue,
                                                            flag1,
                                                            start,
                                                            stop,
                                                            flag2,
                                                            start2,
                                                            stop2,
                                                            &best[i-1]
                                                            ));
                        start = stop;
                        start2 = stop2;
                    }

                    TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Waiting for all threads...";

                    for (unsigned short i=1;i<=numOfThreads;i++)
                    {
                        TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Waiting for " << i;
                        iaThread[i].join();
                        TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Thread " << i << " finished";
                    };

                    TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Workers finished";
                    temp = best[0];

                    for (unsigned short i=0;i<numOfThreads;i++)
                    {
                        if (temp.GetPercentageResult()>best[i].GetPercentageResult())
                        {
                            temp = best[i];
                        };
                    };
                };
                //END

                queue.Clear();
                *boardWsk = temp;

                TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Best board set:";
                TRACE_FLAG_FOR_CLASS_ThreadIAMove temp.PrintDebug();
                TRACE_FLAG_FOR_CLASS_ThreadIAMove boardWsk->PrintDebug();

                queue.GetConditionVariable()->notify_one();
                *flag = true;
            } else
            {
                *flag = true;
            }
        };
    }

   Traces::RemoveThreadID();
}

template  <unsigned long long QMain>
void ThreadIAMove<QMain>::CreateFirstElements()
{
    TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: void ThreadIAMove<QMain>::CreateFirstElements()";

    std::atomic_bool tmpFlag;
    tmpFlag = false;

    ThreadIATreeExpander<QMain,100> expander;    
    expander.Expand(1,100,queue,0, NULL, KindOfSteps::Step, &tmpFlag);

}


template  <unsigned long long QMain>
void ThreadIAMove<QMain>::SetOriginToAll()
{
    TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: void ThreadIAMove<QMain>::SetOriginToAll()";
    unsigned long long size = queue.Size();
    Board temp;

    if (size>0)
    {
        for (unsigned int i=0;i<size;i++)
        {
            temp = queue.PopFront(0);
            temp.SetOrigin(temp);

            TRACE_FLAG_FOR_CLASS_ThreadIAMove Traces() << "\n" << "LOG: Origin set";
            TRACE_FLAG_FOR_CLASS_ThreadIAMove temp.PrintDebug();
            queue.PushBack(temp);
        }
    };
}

#endif // THREADIAMOVE_H


