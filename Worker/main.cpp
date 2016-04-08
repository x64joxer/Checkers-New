#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include "TCP/TCPSocket.h"

using namespace std;

int main()
{
    Traces::SetTraceFolder("trace");

    boost::asio::io_service io_service;    

    TCPSocket socket("192.168.0.7", "6000", io_service);

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    char *c = new char[100];


    while (true) {

        std::cin >> *c;

        std::strcpy(c, "Roman");

        socket.WriteMessage(c);

    }

    delete [] c;
    return 0;
}



