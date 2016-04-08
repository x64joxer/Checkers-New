#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include "TCP/TCPSocket.h"

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");

    TCPSocket socket("192.168.0.7", "6000");
    TCPSocket socket2("192.168.0.7", "6000");

    char *c = new char[100];
    char *c2 = new char[100];

    while (true) {

        std::cin >> *c;

        std::strcpy(c, "Roman");
        std::strcpy(c2, "Mirek");

        socket.WriteMessage(c);
        socket2.WriteMessage(c2);

    }

    delete [] c;
    delete [] c2;

    return 0;
}



