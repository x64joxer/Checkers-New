#ifndef QUEUETIMERLIST_H
#define QUEUETIMERLIST_H

#include <memory>
#include "../SafeQueue/SharedMap/SharedMap.h"
#include "../TCP/TCPConnection.h"
#include "../Traces/Traces.h"
#include "QueueTimer.h"
#include "../ProgramVariables/ProgramVariables.h"

typedef std::shared_ptr<QueueTimer> QueueTimer_ptr;

class QueueTimerList
{
    public:
        QueueTimerList();
        void InsertIntoList(TCPConnection_ptr socket, QueueTimer_ptr timer);
        void RemoveFromList(TCPConnection_ptr socket);
        ~QueueTimerList();

    private:
        SharedMap<TCPConnection_ptr, QueueTimer_ptr> list;
};

#endif // QUEUETIMERLIST_H
