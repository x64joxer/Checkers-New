#include "QueueTimerList.h"

QueueTimerList::QueueTimerList()
{

}

void QueueTimerList::InsertIntoList(TCPConnection_ptr socket, QueueTimer_ptr timer)
{
    Traces() << "\n" << "LOG: void QueueTimerList::InsertIntoList(TCPConnection_ptr socket, QueueTimer_ptr timer)";

    list.Insert(socket, timer);
}

void QueueTimerList::RemoveFromList(TCPConnection_ptr socket)
{
    Traces() << "\n" << "LOG: void QueueTimerList::RemoveFromList(TCPConnection_ptr socket)";

    try
    {
        QueueTimer_ptr tmpTimer = list.At(socket);
        tmpTimer->Stop();
        list.Erase(socket);
    }
    catch(...)
    {
        Traces() << "\n" << "LOG: No such timer!";
    }
}

QueueTimerList::~QueueTimerList()
{

}
