#include "Worker.h"

Worker::Worker()
{
    Traces() << "\n" << "LOG: Worker::Worker()";

    messageQueue = new SharedPtrList<Message>;
}

void Worker::Start()
{
    Traces() << "\n" << "LOG: void Worker::Start()";

    socketToServer.SetMessageQueue(messageQueue);
    socketToServer.Connect("192.168.0.7", "6000");
}

Worker::~Worker()
{
    Traces() << "\n" << "LOG: Worker::~Worker()";
    delete messageQueue;
}
