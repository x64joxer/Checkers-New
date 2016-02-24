#include "ProgramVariables.h"

ProgramVariables::ProgramVariables()
{

}

unsigned short ProgramVariables::InitMaxThreads()
{
    maxNumbeOfThreads = std::thread::hardware_concurrency();

    if (maxNumbeOfThreads == 0)
    {
        maxNumbeOfThreads = 1;
    };    

    return maxNumbeOfThreads;
}

unsigned short ProgramVariables::GetMaxNumberOfThreads()
{
    return maxNumbeOfThreads;
}

void ProgramVariables::SetNumberOfThreads(const unsigned short num)
{
    numbeOfThreads = num;
}

unsigned short ProgramVariables::GetNumberOfThreads()
{
    return numbeOfThreads;
}


bool ProgramVariables::IsWorker()
{
    return isWorker;
}

void ProgramVariables::SetWorker(const bool flag)
{
    isWorker = flag;
}

unsigned long long ProgramVariables::GetSecondsSinceEpoch()
{
    std::time_t result = std::time(nullptr);

    return result;
}

unsigned long long ProgramVariables::GetMaxTimeWaitToWorkers()
{
    return 5;
}

unsigned long long ProgramVariables::GetMaxTimeWaitToServer()
{
    return 3000;
}

QString ProgramVariables::GetServerIP()
{
    return "192.168.0.6";
}

int ProgramVariables::GetServerPort()
{
   return 6000;
}

std::condition_variable * ProgramVariables::GetGlobalConditionVariable()
{
    return &condition_var;
}

std::condition_variable * ProgramVariables::GetGlobalConditionVariableNetwork()
{
    return &condition_var_network;
}

void ProgramVariables::NotifyOne()
{
    condition_var.notify_one();
    condition_var_network.notify_one();
}

unsigned int ProgramVariables::GetRecconectingTime()
{
    return 5000;
}

unsigned short ProgramVariables::GetMaxNumberOfReattempt()
{
    return 2;
}

std::string ProgramVariables::CreateMessageId()
{
    std::lock_guard<std::mutex> guard(mutex_guard);
    return std::to_string(++messageId);
}

void ProgramVariables::SetMaxWorkers(const unsigned int val)
{
     std::lock_guard<std::mutex> guard(mutex_guard_for_max_workers);
     maxWorkers = val;
}

bool ProgramVariables::IsMaxWorkersReached()
{
    std::lock_guard<std::mutex> guard(mutex_guard_for_max_workers);
    if (maxWorkers == 0) return false;
    if (usedWorkers < maxWorkers) return false;

    return true;
}

void ProgramVariables::IncreaseUsedWorkers()
{
    std::lock_guard<std::mutex> guard(mutex_guard_for_max_workers);
    if (usedWorkers < maxWorkers) usedWorkers++;
}

void ProgramVariables::DecreaseUsedWorkers()
{
    std::lock_guard<std::mutex> guard(mutex_guard_for_max_workers);
    if (usedWorkers == 0)
    {
        Traces() << "\n" << "ERROR: usedWorkers wariable already 0! Can not decrease!";
    } else
    {
        usedWorkers--;
    }
}

unsigned long long ProgramVariables::GetMaxSecondsToEnd()
{
    return 3;
}

unsigned long long ProgramVariables::GetMaxTimeForIa()
{
    return 5;
}

bool ProgramVariables::isWorker = false;
unsigned short ProgramVariables::maxNumbeOfThreads = ProgramVariables::InitMaxThreads();
unsigned short ProgramVariables::numbeOfThreads = ProgramVariables::maxNumbeOfThreads;
unsigned long long ProgramVariables::messageId = 0;
std::mutex ProgramVariables::mutex_guard;
std::condition_variable ProgramVariables::condition_var;
std::condition_variable ProgramVariables::condition_var_network;
unsigned long long ProgramVariables::numOfAnalysded = 0;
unsigned int ProgramVariables::maxWorkers = 0;
unsigned int ProgramVariables::usedWorkers = 0;
std::mutex ProgramVariables::mutex_guard_for_max_workers;
