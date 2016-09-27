#ifndef THREADIATREEEXPANDER_H
#define THREADIATREEEXPANDER_H

#include "../Board/Board.h"
#include "../Traces/Traces.h"
#include "../IA/IAPossibleMoves.h"
#include "ThreadIABoardQueue.h"

enum KindOfSteps { Time, Step };

template <unsigned long long MQueue, unsigned long long sQueue>
class ThreadIATreeExpander
{
    public:
       ThreadIATreeExpander();       
       void Expand(const unsigned long long howManySteps,
                   const unsigned int frequencyOfTransferData,
                   ThreadIABoardQueue<MQueue> & mainBoardQueue,
                   const unsigned short numThread,
                   std::atomic<int> *percentSteps,
                   const KindOfSteps stepKind);

       void ExpandWithoutQueue(const unsigned int howManySteps, const unsigned int frequencyOfTransferData, const unsigned short numThread, std::atomic<int> *percentSteps, const KindOfSteps stepKind);
       void SetMainBoardQueue(ThreadIABoardQueue<MQueue> * mainBoardQueue);

    private:
       void TransferBoards(ThreadIABoardQueue<MQueue> & mainBoardQueue);
       inline void AddToMainQueue(const Board &board);
       inline void AddToDoNotForgetQueue(const Board & board);
       bool ExpandWhite(const Board & board, const unsigned int stepNumber = 1);
       bool ExpandBlack(const Board & board, const unsigned int stepNumber = 1);

       ThreadIABoardQueue<MQueue> * mainBoardQueueLocal;
       const unsigned long long queueSize;       
       unsigned long long lastQueueElement;
       unsigned long long firstQueueElement;
       unsigned long long numberOfElements;
       Board queue[sQueue];

       const unsigned long long doNotForgetQueueSize;
       unsigned long long lastDoNotForgetQueueElement;
       Board doNotForgetQueue[sQueue/50];

       unsigned short threadNumber;              
};

#endif // THREADIATREEEXPANDER_H
