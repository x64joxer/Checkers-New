#ifndef THREADIAMOVE_H
#define THREADIAMOVE_H

#include <atomic>
#include "../Board/Board.h"
#include "ThreadIABoardQueue.h"
#include "ThreadIABoardQueue.cpp"
#include "ThreadIATreeExpander.cpp"
#include "../Traces/Traces.h"

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

template  <unsigned long long QMain>
ThreadIAMove<QMain>::ThreadIAMove()
{

}

template  <unsigned long long QMain>
void ThreadIAMove<QMain>::operator ()(Board * boardWsk, std::atomic_bool * flag, std::atomic<int> *percentSteps, const unsigned short numberOfThreads, const unsigned int refreshMainQueue, const unsigned int numberOfStepsToDo, const KindOfSteps stepKind)
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

    //When all black killed
    if (queue.First(0).GetNumberOfBlack() == 0)
    {
        *boardWsk = queue.First(0);
        *flag = true;
    } else
    //Can not move or all black killed
    if (queue.Size() == 0)
    {
        *flag = true;
    } else
    {
        //Set origin to all
        //TO DO Origin shoul be set by server!! if (!ProgramVariables::IsWorker()) SetOriginToAll();

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
                                                stepKind
                                                ));

        };

        //TO_DEL Start sharing jobs
        //TO_DEL if (messageHandler) messageHandler->StartSharing(ProgramVariables::GetSecondsSinceEpoch());

        for (unsigned short i=1;i<=numberOfThreads;i++)
        {
            iaThread[i].join();
        };

        TRACE01 Traces() << "\n" << "LOG: Waiting for all workers...";
        //TO_DEL {
        //TO_DEL    unsigned long long start = ProgramVariables::GetSecondsSinceEpoch();
        //TO_DEL    while (ProgramVariables::GetSecondsSinceEpoch() - start < ProgramVariables::GetMaxTimeWaitToWorkers())
        //TO_DEL    {
        //TO_DEL        if (WorkerAgent::GetBusyStateNumber() == 0) break;
        //TO_DEL    }
        //TO_DEL }

        //TO_DEL if (messageHandler) messageHandler->StopSharing();
        //TO_DO ProgramVariables::NotifyOne();

        //TO_DEL ProgramVariables::IncreaseNumOfAnalyded(queue.Size() + queue.SizeDoNotForget());

        //TO_DO Traces()<< "\n" << "LOG: -------------------------------------------------------------------";
        //TO_DO Traces()<< "\n" << "LOG: Total num of analysed elements: " << ProgramVariables::GetNumOfAnalyded();
        //TO_DO Traces()<< "\n" << "LOG: -------------------------------------------------------------------";

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
                TRACE01 Traces() << "\n" << "LOG: Start sharing for thread " << i;
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

            TRACE01 Traces() << "\n" << "LOG: Waiting for all threads...";

            for (unsigned short i=1;i<=numOfThreads;i++)
            {
                TRACE01 Traces() << "\n" << "LOG: Waiting for " << i;
                iaThread[i].join();
                TRACE01 Traces() << "\n" << "LOG: Thread " << i << " finished";
            };

            TRACE01 Traces() << "\n" << "LOG: Workers finished";
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
        *boardWsk = temp.GetOrigin();

        TRACE01 Traces() << "\n" << "LOG: Best board set:";
        temp.PrintDebug();
        boardWsk->PrintDebug();

        *flag = true;
    };

   Traces::RemoveThreadID();
}

template  <unsigned long long QMain>
void ThreadIAMove<QMain>::CreateFirstElements()
{
    TRACE01 Traces() << "\n" << "LOG: void ThreadIAMove<QMain>::CreateFirstElements()";
    ThreadIATreeExpander<QMain,100> expander;
    expander.Expand(1,100,queue,0, NULL, KindOfSteps::Step);
}


#include "ThreadIAMove_Impl.h"

#endif // THREADIAMOVE_H


