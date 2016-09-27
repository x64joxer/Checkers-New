#include "ThreadIABoardQueue.h"

template <unsigned long long size>
ThreadIABoardQueue<size>::ThreadIABoardQueue():
                          first(0),
                          last(0),
                          numberOfElements(0),
                          workersFlags(0),
                          doNotForgetnumberOfElements(0)
{
    condition_var = ProgramVariables::GetGlobalConditionVariable();
    queue = new Board[size];
    doNotForgetqueue = new Board[size];
}

template <unsigned long long size>
void ThreadIABoardQueue<size>::Clear()
{
    first = 0;
    last = 0;
    numberOfElements = 0;
    doNotForgetnumberOfElements =0;    
}

template <unsigned long long size>
Board ThreadIABoardQueue<size>::First(const bool remove)
{
    std::lock_guard<std::mutex> guard(mutex_guard);

    Board temp;

    if (Empty())
    {
        temp.SetNullBoard(true);
        return temp;
    }

    temp = queue[first];

    if (remove)
    {       
        TRACE01 Traces() << "\n" << "LOG: (numberOfElements>0)";

        if (numberOfElements>1)
        {
            first++;
            if (first==size)
            {
             first = 0;
            };
            numberOfElements--;

        } else
        {
            first = 0;
            last = 0;
            numberOfElements = 0;
        };

        TRACE01 Traces() << "\n" << "LOG: first " << first;
        TRACE01 Traces() << "\n" << "LOG: last " << last;
        TRACE01 Traces() << "\n" << "LOG: Number of cells " << numberOfElements;
    }

    return temp;
}

template <unsigned long long size>
Board ThreadIABoardQueue<size>::PopFront(const unsigned short num)
{    
    TRACE01 Traces() << "\n" << "LOG: Board ThreadIABoardQueue<size>::PopFront()";    
    {
        std::lock_guard<std::mutex> guard(mutex_guard);
        SetWorkerFlag(false, num);
    }

    std::unique_lock<std::mutex> guard(mutex_guard);
    condition_var->wait(guard,[this]
    {
        return (!Empty()) | (!workersFlags) ;}
    );

    if (num>0)
    {
            if (numberOfElements>0)
            {
                SetWorkerFlag(true, num);
            } else
            if (!workersFlags)
            {
                TRACE01 Traces() << "\n" << "LOG: No active workers! Finishing!";                
                Board temp_null;
                temp_null.SetNullBoard(true);
                return temp_null;
            };
    };

    unsigned long long temp = first;

    if (numberOfElements>0)
    {
        TRACE01 Traces() << "\n" << "LOG: (numberOfElements>0)";

        if (numberOfElements>1)
        {
            first++;
            if (first==size)
            {
             first = 0;
            };
            numberOfElements--;

        } else
        {
            first = 0;
            last = 0;
            numberOfElements = 0;
        };

        TRACE01 Traces() << "\n" << "LOG: first " << first;
        TRACE01 Traces() << "\n" << "LOG: last " << last;
        TRACE01 Traces() << "\n" << "LOG: Number of cells " << numberOfElements;

        TRACE01 queue[temp].PrintDebug();
        guard.unlock();
        return queue[temp];
    };

    TRACE01 Traces() << "\n" << "LOG: No elements to get!";
    Board temp_null;
    temp_null.SetNullBoard(true);    
    return temp_null;
}

template <unsigned long long size>
inline void ThreadIABoardQueue<size>::PushBack(const Board & board)
{
    std::lock_guard<std::mutex> guard(mutex_guard);
    TRACE01 Traces() << "\n" << "LOG: void ThreadIABoardQueue<size>::PushBack(Board board) Number of cells";

    if (numberOfElements == 0)
    {
       numberOfElements++;
       queue[last] = board;

       TRACE01 Traces() << "\n" << "LOG: first " << first;
       TRACE01 Traces() << "\n" << "LOG: last " << last;
       TRACE01 Traces() << "\n" << "LOG: Number of cells " << numberOfElements;
    } else
    if (numberOfElements<size)
    {
        TRACE01 Traces() << "\n" << "LOG: if (numberOfElements<=size)";
        if (last==size-1)
        {
            last = 0;
            queue[last] = board;
            numberOfElements++;
        } else
        {
            last++;
            queue[last] = board;
            numberOfElements++;
        };

        TRACE01 Traces() << "\n" << "LOG: first " << first;
        TRACE01 Traces() << "\n" << "LOG: last " << last;
        TRACE01 Traces() << "\n" << "LOG: Number of cells " << numberOfElements;
    } else
    {
        Traces() << "\n" << "ERROR: No more free cells!";
    };

    ProgramVariables::NotifyOne();
}

template <unsigned long long size>
inline void ThreadIABoardQueue<size>::PushBackDoNotForget(const Board & board)
{
    TRACE01 Traces() << "\n" << "LOG: inline void ThreadIABoardQueue<size>::PushBackDoNotForget(Board &board)";
    mutex_guard.lock();
    if (doNotForgetnumberOfElements < size)
    {
        doNotForgetqueue[doNotForgetnumberOfElements] = board;
        ++doNotForgetnumberOfElements;        
    }
    mutex_guard.unlock();
    ProgramVariables::NotifyOne();
}

template <unsigned long long size>
Board ThreadIABoardQueue<size>::GetBestResult() const
{
    double result = 0;    
    Board temp;

    qDebug() << "Number of eleents" << numberOfElements;
    qDebug() << "Number of do not forget eleents" << doNotForgetnumberOfElements;

    if (numberOfElements>0)
    {
        result = At(0).GetPercentageResult();
        temp = At(0);        

            for (unsigned long long i=0;i<numberOfElements;i++)
            {
                if (result>At(i).GetPercentageResult())
                {
                    result =  At(i).GetPercentageResult();
                    temp = At(i);
                };
            };
    }

    if (doNotForgetnumberOfElements>0)
    {
        if (result == 0)
        {
            result = doNotForgetqueue[0].GetPercentageResult();
            temp = doNotForgetqueue[0];
        };

        TRACE01 Traces() << "\n" << "LOG: Origin of doNotForgetqueue[0]";
        TRACE01 doNotForgetqueue[0].GetOrigin().PrintDebug();


        if (doNotForgetnumberOfElements>1)
        {
            for (unsigned long long i=1;i<doNotForgetnumberOfElements;i++)
            {
                if (result>doNotForgetqueue[i].GetPercentageResult())
                {
                    result =  doNotForgetqueue[i].GetPercentageResult();
                    temp = doNotForgetqueue[i];
                };
            };
        };
    }

    return temp;
}

template <unsigned long long size>
void ThreadIABoardQueue<size>::GetBestResultMultiThread(bool make, const unsigned long long start, const unsigned long long stop, bool make2, const unsigned long long start2, const unsigned long long stop2, Board *best) const
{
    TRACE01 Traces() << "\n" << "LOG: void ThreadIABoardQueue<size>::GetBestResult2";
    double result = 0;
    Board temp;

    TRACE01 Traces() << "\n" << "LOG: Number of eleents =" << numberOfElements;
    TRACE01 Traces() << "\n" << "LOG: Number of do not forget eleents =" << doNotForgetnumberOfElements;


    if (make)
    {
        TRACE01 Traces() << "\n" << "LOG: make = true";
        TRACE01 Traces() << "\n" << "LOG: start =" << start;
        TRACE01 Traces() << "\n" << "LOG: stop =" << stop;

        result = At(0).GetPercentageResult();
        temp = At(0);

        TRACE01 Traces() << "\n" << "LOG: Current best result";
        TRACE01 temp.PrintDebug();

        for (unsigned long long i = 0; i<numberOfElements; i++)
        {
            if (result>At(i).GetPercentageResult())
            {
                result =  At(i).GetPercentageResult();
                temp = At(i);
            };
        };
    };

    if (make2)
    {
        TRACE01 Traces() << "\n" << "LOG: make2 == true";
        TRACE01 Traces() << "\n" << "LOG: start2 =" << start2;
        TRACE01 Traces() << "\n" << "LOG: stop2 =" << stop2;

        if (!make)
        {
            TRACE01 Traces() << "\n" << "LOG: !make = false";
            result = doNotForgetqueue[start2].GetPercentageResult();
            temp = doNotForgetqueue[start2];            
            TRACE01 temp.PrintDebug();
        };

        TRACE01 Traces() << "\n" << "LOG: Searching best";

        for (unsigned long long i = start2; i<stop2; i++)
        {
            if (result>doNotForgetqueue[i].GetPercentageResult())
            {
                result =  doNotForgetqueue[i].GetPercentageResult();
                temp = doNotForgetqueue[i];
                TRACE01 temp.PrintDebug();
            };
        };
    };

    TRACE01 Traces() << "\n" << "LOG: End of searching";
    *best = temp;
}

template <unsigned long long size>
unsigned long long ThreadIABoardQueue<size>::Size() const
{
    TRACE01 Traces() << "\n" << "LOG: ThreadIABoardQueue<size>::Size()";

    std::lock_guard<std::mutex> guard(mutex_guard);
    return numberOfElements;
}

template <unsigned long long size>
unsigned long long ThreadIABoardQueue<size>::SizeDoNotForget() const
{
    std::lock_guard<std::mutex> guard(mutex_guard);
    return doNotForgetnumberOfElements;

}

template <unsigned long long size>
void ThreadIABoardQueue<size>::NotifyRest()
{
    ProgramVariables::NotifyOne();
}

template <unsigned long long size>
unsigned long long ThreadIABoardQueue<size>::GetFirstNumber() const
{
    return first;
}

template <unsigned long long size>
void ThreadIABoardQueue<size>::SetWorkerFlag(const bool flag, const unsigned short number)
{
    if (number > 0)
    {
        unsigned short val;

        if (!flag)
        {
            val = 65535;
            val-= pow(2, number);
            workersFlags = workersFlags & val;
        } else
        {
            val = pow(2, number);
            workersFlags = workersFlags | val;
        };
    };
}

template <unsigned long long size>
Board & ThreadIABoardQueue<size>::At(const unsigned long long number) const
{
    if (number > numberOfElements)
    {
        Traces() << "\n" << "LOG: Element not exist!";
    } else
    if (first + number < size - 1)
    {
        return queue[first + number];
    } else
    {
       return queue[number - ((size-1) - first)];
    }

    return queue[first];
}

template <unsigned long long size>
ThreadIABoardQueue<size>::~ThreadIABoardQueue()
{
    delete [] queue;
    delete [] doNotForgetqueue;
}
