#include <iostream>
#include "TCP/TCPSocket.h"

using namespace std;

void resolve_handler(
    const boost::system::error_code& ec,
    tcp::resolver::iterator i)
{
    cout << "Hello World!!!!!!!!!" << endl;
}

int main()
{
    cout << "Hello World!" << endl;

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "6000");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    resolver.async_resolve(query, resolve_handler);
    TCPSocket socket(io_service, iterator);


    while (true) {}
    return 0;
}



