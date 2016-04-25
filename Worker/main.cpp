#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include "QueueTimer/QueueTimer.h"
#include "Worker/Worker.h"

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");
    /*
    //TCPSocket socket;
    //TCPSocket socket2;
    QueueTimer timer;
    SharedPtrList<Message> queue;
    Message message;
    timer.SetTime(20000);
    timer.SetQueue(&queue);
    timer.SetMessageToSend(message);
    timer.SetTime(500);
    timer.Start();
    timer.Stop();


    timer.Start();
    //timer.SetTime(2000);
    //timer.Start();

    //socket.Connect("192.168.0.7", "6000");
    //socket2.Connect("192.168.0.7", "6000");;


    char *c = new char[4048];
    char *c2 = new char[100];

    while (true) {

        std::cin >> *c;

        std::strcpy(c, "Message1");
        //std::strcpy(c2, "Message2");

        //socket.WriteMessage(c);
        //socket2.WriteMessage(c2);

    }

    delete [] c;
    delete [] c2; */

    return 0;
}



