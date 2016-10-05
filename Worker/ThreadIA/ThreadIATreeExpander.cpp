#include "ThreadIATreeExpander.h"

template <unsigned long long MQueue, unsigned long long sQueue>
ThreadIATreeExpander<MQueue, sQueue>::ThreadIATreeExpander() :
    queueSize(sQueue),
    lastQueueElement(0),
    firstQueueElement(0),    
    doNotForgetQueueSize(sQueue/50),
    lastDoNotForgetQueueElement(0),
    numberOfElements(0)

{

}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::ExpandWithoutQueue(const unsigned int howManySteps, const unsigned int frequencyOfTransferData, const unsigned short numThread, std::atomic<int> *percentSteps, const KindOfSteps stepKind)
{
    Expand(howManySteps, frequencyOfTransferData, *mainBoardQueueLocal, numThread, percentSteps, stepKind);
}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::SetMainBoardQueue(ThreadIABoardQueue<MQueue> * mainBoardQueue)
{
    mainBoardQueueLocal = mainBoardQueue;
}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::Expand(const unsigned long long howManySteps, const unsigned int frequencyOfTransferData, ThreadIABoardQueue<MQueue> & mainBoardQueue, const unsigned short numThread, std::atomic<int> *percentSteps, const KindOfSteps stepKind)
{
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: EXPAND START";
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: void ThreadIATreeExpander<MQueue, sQueue>::Expand(unsigned int howManySteps, unsigned int frequencyOfTransferData, ThreadIABoardQueue<MQueue> &mainBoardQueue)";

    threadNumber = numThread;
    unsigned long long howManyStepsLocal = howManySteps;

    queue[0] = mainBoardQueue.PopFront(threadNumber);
    if (!queue[firstQueueElement].GetNullBoard()) numberOfElements++;

    unsigned long long step =0;
    unsigned long long totalStep = howManyStepsLocal;

    if (stepKind == KindOfSteps::Step)
    {
        step = 0;
    } else
    if (stepKind == KindOfSteps::Time)
    {
        step = Traces::GetMilisecondsSinceEpoch();
        howManyStepsLocal = step + howManyStepsLocal;
    }

    unsigned long long current;

    while (step < howManyStepsLocal)
    {
        if (queue[firstQueueElement].GetNullBoard()) break;

        for (current = firstQueueElement; current <= lastQueueElement; )
        {            

            //Expand current element
            if (queue[current].Black())
            {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: (treePointer->Black())";
              firstQueueElement = current+1;
              numberOfElements--;
              ExpandBlack(queue[current], step);                            
            } else
            {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: (treePointer->White())";
              firstQueueElement = current+1;
              numberOfElements--;
              ExpandWhite(queue[current], step);              
            };

            if (stepKind == KindOfSteps::Step)
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: step++";
                step++;
            } else
            if (stepKind == KindOfSteps::Time)
            {
                step = Traces::GetMilisecondsSinceEpoch();
            }

            //Finish job
            if (step >= howManyStepsLocal) break;

            //Check if time to transfer data 

            if ((lastQueueElement>=firstQueueElement)||(lastQueueElement>queueSize-100)||(lastDoNotForgetQueueElement>(queueSize/50)-100))
            {
                if ((frequencyOfTransferData<=lastQueueElement-firstQueueElement)||(lastQueueElement>queueSize-100)||(lastDoNotForgetQueueElement>(queueSize/50)-100))
                {

                         TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG:" << firstQueueElement;
                         TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG:" << lastQueueElement;
                        TransferBoards(mainBoardQueue);
                        queue[0] = mainBoardQueue.PopFront(threadNumber);
                        current = 0;

                        if (queue[0].GetNullBoard())
                        {
                            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Everyone is waiting! No jobs";
                            break;
                        };
                        continue;
                };
            };

            ++current;

            //Update status
            if (numThread == 1)
            {
                if (percentSteps)
                {
                    if (stepKind == KindOfSteps::Step)
                    {
                        if (step == 0)
                        {
                            *percentSteps = (step+1) / (howManyStepsLocal / 100);
                        } else
                        {
                            *percentSteps = step / (howManyStepsLocal / 100);
                        }
                    } else
                    if (stepKind == KindOfSteps::Time)
                    {
                        *percentSteps = (totalStep - (howManyStepsLocal - step)) * (100 / totalStep);
                    }
                };
            };

            if (mainBoardQueue.Size() > 3000000 - 1000)
            {
                break;
            }
        };

        if (queue[firstQueueElement].GetNullBoard())
        {
            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Everyone is waiting! No jobs";
            break;
        };

        if (mainBoardQueue.Size() > 3000000 - 1000)
        {
            break;
        }

        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: for (current = firstQueueElement; current <= lastQueueElement; ) END";

        //Finish job
        if (step >= howManyStepsLocal)
        {
            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: (step >= howManyStepsLocal) finihing job!";
            break;
        };

        //No jobs take more from global queue
        if (step < howManyStepsLocal)
        {
            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: No jobs. Taking fom global queue";

            TransferBoards(mainBoardQueue);
            queue[0] = mainBoardQueue.PopFront(threadNumber);

            if (queue[0].GetNullBoard())
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: No job in global queue. Finishing thread.";
                break;
            };

            lastQueueElement =0;
            firstQueueElement =0;
        }

    };

    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Number of temporary queue array " << lastQueueElement;
    TransferBoards(mainBoardQueue);

    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: EXPAND STOP";
    Traces::RemoveThreadID();
}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::TransferBoards(ThreadIABoardQueue<MQueue> &mainBoardQueue)
{
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: ThreadIATreeExpander<MQueue, sQueue>::TransferBoards(ThreadIABoardQueue<MQueue> &mainBoardQueue)";
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: numberOfElements=" << numberOfElements;

    if (numberOfElements>0)
    {
        if (lastQueueElement>=firstQueueElement)
        {
            if ((lastQueueElement-firstQueueElement)+1>0)
            {
                for (unsigned long long i=firstQueueElement;i<=lastQueueElement;i++)
                {
                     TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: mainBoardQueue.PushBack(queue[i])";
                    mainBoardQueue.PushBack(queue[i]);
                };

                firstQueueElement =0;
                lastQueueElement=0;
            };
        };
    };

    if (lastDoNotForgetQueueElement>0)
    {
        for (unsigned long long i=1;i<=lastDoNotForgetQueueElement;i++)
        {
            mainBoardQueue.PushBackDoNotForget(queue[i]);
        };
       lastDoNotForgetQueueElement = 0;
    };

    mainBoardQueue.NotifyRest();
}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::AddToMainQueue(const Board &board)
{
    if (++lastQueueElement > queueSize-1)
    {
        Traces() << "\n" << "ERROR: No free memory cells in main queue";
    } else
    {
        queue[lastQueueElement] = board;
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: AddToMainQueue numberOfElements=" << numberOfElements;
        numberOfElements= numberOfElements + 1;
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: AddToMainQueue numberOfElements=" << numberOfElements;
    };
}

template <unsigned long long MQueue, unsigned long long sQueue>
void ThreadIATreeExpander<MQueue, sQueue>::AddToDoNotForgetQueue(const Board & board)
{
    if (++lastDoNotForgetQueueElement > doNotForgetQueueSize-1)
    {
        Traces() << "\n" << "ERROR: No free memory cells in main do not forget queue";
    } else
    {
        doNotForgetQueue[lastDoNotForgetQueueElement] = board;
    };
}

template <unsigned long long MQueue, unsigned long long sQueue>
bool ThreadIATreeExpander<MQueue, sQueue>::ExpandWhite(const Board & board, const unsigned int stepNumber)
{
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: void IATreeExpander::ExpandWhite(IADecisionTree *treePointer)";

    //DEL Board board = board.GetBoard();
    unsigned short pawnNumber = board.GetNumberOfWhite();
    Board boardLocal = board;
    IAPossibleMoves possible;
    bool killFlag = false;

    if (boardLocal.GetPreviousMurder()<12)
    {
        if (boardLocal.GetNumberOfBlack() == 0)
        {
            AddToDoNotForgetQueue(boardLocal);

            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when all black was killed!";
            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
            return 0;
        };

        unsigned short i = boardLocal.GetPreviousMurder();
        //Left Bottom
       if (possible.CheckHitBottomLeftWhite(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomLeftWhite(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitBottomLeftWhite(i, tempNew);
           killFlag = true;

           tempNew.StartWhite();
           tempNew.SetPreviousMurder(i);
           if (!boardLocal.GetWhitePatchEnd()) tempNew.SetOrigin(tempNew);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //Right Bottom
       if (possible.CheckHitBottomRightWhite(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomRightWhite(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitBottomRightWhite(i, tempNew);
           killFlag = true;
           tempNew.StartWhite();
           tempNew.SetPreviousMurder(i);
           if (!boardLocal.GetWhitePatchEnd()) tempNew.SetOrigin(tempNew);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //For pons
       if (boardLocal.GetWhitePawnPons(i))
       {
           //Left Bottom
          if (possible.CheckHitTopLeftWhite(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomLeftWhite(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitTopLeftWhite(i, tempNew);
              killFlag = true;
              tempNew.StartWhite();
              tempNew.SetPreviousMurder(i);
              if (!boardLocal.GetWhitePatchEnd()) tempNew.SetOrigin(tempNew);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
          //Right Bottom
          if (possible.CheckHitTopRightWhite(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomRightWhite(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitTopRightWhite(i, tempNew);
              killFlag = true;
              tempNew.StartWhite();
              tempNew.SetPreviousMurder(i);
              if (!boardLocal.GetWhitePatchEnd()) tempNew.SetOrigin(tempNew);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
       };

       if (!killFlag)
       {           
           boardLocal.SetPreviousMurder(12);
           boardLocal.StartBlack();
           boardLocal.SetWhitePatchEnd(true);
           ExpandBlack(boardLocal);
       };

       return 0;
    };

    if (boardLocal.GetNumberOfWhite() == 0)
    {
        AddToDoNotForgetQueue(boardLocal);

        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when all white was killed!";
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
        return 0;
    };

    //Check possible kills
    for (unsigned short i=0;i<pawnNumber;i++)
    {
        //Left Bottom
       if (possible.CheckHitBottomLeftWhite(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomLeftWhite(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitBottomLeftWhite(i, tempNew);
           killFlag = true;
           tempNew.StartWhite();
           tempNew.SetPreviousMurder(i);           

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //Right Bottom
       if (possible.CheckHitBottomRightWhite(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomRightWhite(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitBottomRightWhite(i, tempNew);
           killFlag = true;
           tempNew.StartWhite();
           tempNew.SetPreviousMurder(i);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //For pons
       if (boardLocal.GetWhitePawnPons(i))
       {
           //Left Bottom
          if (possible.CheckHitTopLeftWhite(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomLeftWhite(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitTopLeftWhite(i, tempNew);
              killFlag = true;
              tempNew.StartWhite();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
          //Right Bottom
          if (possible.CheckHitTopRightWhite(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitBottomRightWhite(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitTopRightWhite(i, tempNew);
              killFlag = true;
              tempNew.StartWhite();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
       };
    };
    //If no kills check other steps
    if (!killFlag)
    {        
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG:  (!killFlag) == true";
        bool canImove = false;
        boardLocal.SetWhitePatchEnd(true);

        //Check possible puts
        for (unsigned short i=0;i<pawnNumber;i++)
        {
            //Bottom Left
            if (possible.CheckPutBottomLeftWhite(i, boardLocal))
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutBottomLeftWhite(" << i << ", board) == true";
                Board tempNew = boardLocal;
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                tempNew.PutWhiteBottomLeftPawn(i);
                tempNew.StartBlack();
                tempNew.SetPreviousMurder(12);

                AddToMainQueue(tempNew);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                canImove = true;
            };
            //Bottom Right
            if (possible.CheckPutBottomRightWhite(i, boardLocal))
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutBottomRightWhite(" << i << ", board) == true";
                Board tempNew = boardLocal;
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                tempNew.PutWhiteBottomRightPawn(i);
                tempNew.StartBlack();
                tempNew.SetPreviousMurder(12);

                AddToMainQueue(tempNew);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                canImove = true;
            };
            //For pons
            if (boardLocal.GetWhitePawnPons(i))
            {
                //Top Left
                if (possible.CheckPutTopLeftWhite(i, boardLocal))
                {
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutTopLeftWhite(" << i << ", board) == true";
                    Board tempNew = boardLocal;
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                    tempNew.PutWhiteTopLeftPawn(i);

                    //TO DO if (board.IsSimilarWhiteInPatch(treePointer,tempNew) == false)
                    //TO DO {
                        tempNew.StartBlack();
                        tempNew.SetPreviousMurder(12);

                        AddToMainQueue(tempNew);

                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                        canImove = true;
                    //TO DO } else
                    //TO DO {
                    //TO DO     TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was similar white board!";
                    //TO DO };
                };
                //Top Right
                if (possible.CheckPutTopRightWhite(i, boardLocal))
                {
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutTopRightWhite(" << i << ", board) == true";
                    Board tempNew = boardLocal;
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                    tempNew.PutWhiteTopRightPawn(i);

                    //TO DO if (board.IsSimilarWhiteInPatch(treePointer,tempNew) == false)
                    //TO DO {
                        tempNew.StartBlack();
                        tempNew.SetPreviousMurder(12);

                        AddToMainQueue(tempNew);

                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                        canImove = true;
                    //TO DO } else
                    //TO DO {
                    //TO DO     TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was similar white board!";
                    //TO DO };
                };
            };
        };
        if (!canImove)
        {
            if (stepNumber>0)
            {
                AddToDoNotForgetQueue(boardLocal);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when white player can not move!";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
            };
        };
    };

    return 0;
}

template <unsigned long long MQueue, unsigned long long sQueue>
bool ThreadIATreeExpander<MQueue, sQueue>::ExpandBlack(const Board & board, const unsigned int stepNumber)
{
    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: void IATreeExpander::ExpandBlack(IADecisionTree *treePointer)";

    //DEL Board board = board.GetBoard();
    unsigned short pawnNumber = board.GetNumberOfBlack();
    Board boardLocal = board;
    IAPossibleMoves possible;
    bool killFlag = false;

    if (boardLocal.GetPreviousMurder()<12)
    {
        if (boardLocal.GetNumberOfWhite() == 0)
        {
            AddToDoNotForgetQueue(boardLocal);

            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when all White was killed!";
            TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
            return 0;
        };

        unsigned short i = boardLocal.GetPreviousMurder();
        //Left Top
       if (possible.CheckHitTopLeftBlack(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopLeftBlack(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitTopLeftBlack(i, tempNew);
           killFlag = true;

           tempNew.StartBlack();
           tempNew.SetPreviousMurder(i);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //Right Top
       if (possible.CheckHitTopRightBlack(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopRightBlack(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitTopRightBlack(i, tempNew);
           killFlag = true;
           tempNew.StartBlack();
           tempNew.SetPreviousMurder(i);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //For pons
       if (boardLocal.GetBlackPawnPons(i))
       {
           //Left Top
          if (possible.CheckHitBottomLeftBlack(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopLeftBlack(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitBottomLeftBlack(i, tempNew);
              killFlag = true;
              tempNew.StartBlack();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
          //Right Top
          if (possible.CheckHitBottomRightBlack(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopRightBlack(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitBottomRightBlack(i, tempNew);
              killFlag = true;
              tempNew.StartBlack();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
       };

       if (!killFlag)
       {
           boardLocal.SetPreviousMurder(12);
           boardLocal.StartWhite();
           ExpandWhite(boardLocal);
       };

       return 0;
    };

    if (boardLocal.GetNumberOfBlack() == 0)
    {
        AddToDoNotForgetQueue(boardLocal);

        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when all Black was killed!";
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
        return 0;
    };

    //Check possible kills
    for (unsigned short i=0;i<pawnNumber;i++)
    {
        //Left Top
       if (possible.CheckHitTopLeftBlack(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopLeftBlack(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitTopLeftBlack(i, tempNew);
           killFlag = true;
           tempNew.StartBlack();
           tempNew.SetPreviousMurder(i);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //Right Top
       if (possible.CheckHitTopRightBlack(i, boardLocal))
       {
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopRightBlack(" << i << ", board) == true";
           Board tempNew = boardLocal;
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
           possible.KillHitTopRightBlack(i, tempNew);
           killFlag = true;
           tempNew.StartBlack();
           tempNew.SetPreviousMurder(i);

           AddToMainQueue(tempNew);

           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
           TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
       };
       //For pons
       if (boardLocal.GetBlackPawnPons(i))
       {
           //Left Top
          if (possible.CheckHitBottomLeftBlack(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopLeftBlack(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitBottomLeftBlack(i, tempNew);
              killFlag = true;
              tempNew.StartBlack();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
          //Right Top
          if (possible.CheckHitBottomRightBlack(i, boardLocal))
          {
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckHitTopRightBlack(" << i << ", board) == true";
              Board tempNew = boardLocal;
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
              possible.KillHitBottomRightBlack(i, tempNew);
              killFlag = true;
              tempNew.StartBlack();
              tempNew.SetPreviousMurder(i);

              AddToMainQueue(tempNew);

              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
              TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
          };
       };
    };
    //If no kills check other steps
    if (!killFlag)
    {
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG:  (!killFlag) == true";
        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG:  pawnNumber = " << pawnNumber;
        bool canImove = false;

        //Check possible puts
        for (unsigned short i=0;i<pawnNumber;i++)
        {
            //Top Left
            if (possible.CheckPutTopLeftBlack(i, boardLocal))
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutTopLeftBlack(" << i << ", board) == true";
                Board tempNew = boardLocal;
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                tempNew.PutBlackTopLeftPawn(i);
                tempNew.StartWhite();
                tempNew.SetPreviousMurder(12);

                AddToMainQueue(tempNew);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                canImove = true;
            }
            //Top Right
            if (possible.CheckPutTopRightBlack(i, boardLocal))
            {
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutTopRightBlack(" << i << ", board) == true";
                Board tempNew = boardLocal;
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                tempNew.PutBlackTopRightPawn(i);
                tempNew.StartWhite();
                tempNew.SetPreviousMurder(12);

                AddToMainQueue(tempNew);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                canImove = true;
            };
            //For pons
            if (boardLocal.GetBlackPawnPons(i))
            {
                //Bottom Left
                if (possible.CheckPutBottomLeftBlack(i, boardLocal))
                {
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutBottomLeftBlack(" << i << ", board) == true";
                    Board tempNew = boardLocal;
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                    tempNew.PutBlackBottomLeftPawn(i);

                    //TO DO if (board.IsSimilarBlackInPatch(treePointer,tempNew) == false)
                    //TO DO {
                        tempNew.StartWhite();
                        tempNew.SetPreviousMurder(12);

                        AddToMainQueue(tempNew);

                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                        canImove = true;
                    //TO DO } else
                    //TO DO {
                    //TO DO     TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was similar Black board!";
                    //TO DO };
                };
                //Bottom Right
                if (possible.CheckPutBottomRightBlack(i, boardLocal))
                {
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: possible.CheckPutBottomRightBlack(" << i << ", board) == true";
                    Board tempNew = boardLocal;
                    TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                    tempNew.PutBlackBottomRightPawn(i);

                    //TO DO if (board.IsSimilarBlackInPatch(treePointer,tempNew) == false)
                    //TO DO {
                        tempNew.StartWhite();
                        tempNew.SetPreviousMurder(12);

                        AddToMainQueue(tempNew);

                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: Result";
                        TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander tempNew.PrintDebug();
                        canImove = true;
                    //TO DO } else
                    //TO DO {
                    //TO DO     TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was similar Black board!";
                    //TO DO };
                };
            };
        };
        if (!canImove)
        {
            if (stepNumber>0)
            {
                AddToDoNotForgetQueue(boardLocal);

                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander Traces() << "\n" << "LOG: There was situation when Black player can not move!";
                TRACE_FLAG_FOR_CLASS_ThreadIATreeExpander boardLocal.PrintDebug();
            };
        };
    };

    return 0;
}

