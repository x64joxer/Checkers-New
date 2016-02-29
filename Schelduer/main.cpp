#include <iostream>
#include <boost/filesystem/operations.hpp>

#include "Scheduler.h"

using namespace std;

int main()
{

    Scheduler workScheduler;

    cout << "Main";

    workScheduler.Init();

    return 0;
}

