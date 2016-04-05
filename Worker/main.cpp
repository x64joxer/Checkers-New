#include <iostream>
#include "TCP/TCPSocket.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    TCPSocket socket("127.0.0.1", "6000");


    while (true) { }
    return 0;
}



