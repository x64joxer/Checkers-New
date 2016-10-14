#include "Counters.h"

Counters::Counters()
{

}

void Counters::ClearCounterNumberOfAnalysedBoard()
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: void Counters::ClearCounterNumberOfAnalysedBoard()";

    std::lock_guard<std::mutex> ls(mutex);
    numberOfAnalysedBoard = 0;
}

void Counters::AddToCounterNumberOfAnalysedBoard(const unsigned long long tmpNumberOfAnalysedBoard)
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: void Counters::AddToCounterNumberOfAnalysedBoard(const unsigned long long tmpNumberOfAnalysedBoard)";

    std::lock_guard<std::mutex> ls(mutex);
    numberOfAnalysedBoard = numberOfAnalysedBoard + tmpNumberOfAnalysedBoard;
}

const unsigned long long Counters::GetCounterNumberOfAnalysedBoard()
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: unsigned long long Counters::GetCounterNumberOfAnalysedBoard()";

    std::lock_guard<std::mutex> ls(mutex);
    return numberOfAnalysedBoard;
}

void Counters::ClearCounterNumberOfBestResultMessagesSend()
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: void Counters::ClearCounterNumberOfBestResultMessagesSend()";

    std::lock_guard<std::mutex> ls(mutex);
    numberOfBestResultMessagesSend = 0;
}

void Counters::AddToCounterNumberOfBestResultMessagesSend(const unsigned long long tmpNumberOfBestResultMessagesSend)
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: void Counters::AddToCounterNumberOfBestResultMessagesSend(const unsigned long long tmpNumberOfBestResultMessagesSend)";

    std::lock_guard<std::mutex> ls(mutex);
    numberOfBestResultMessagesSend = numberOfBestResultMessagesSend + tmpNumberOfBestResultMessagesSend;
}

const unsigned long long Counters::GetCounterNumberOfBestResultMessagesSend()
{
    TRACE_FLAG_FOR_CLASS_Counters Traces() << "\n" << "LOG: unsigned long long Counters::GetCounterNumberOfBestResultMessagesSend()";

    std::lock_guard<std::mutex> ls(mutex);
    return numberOfBestResultMessagesSend;
}

std::mutex Counters::mutex;
unsigned long long Counters::numberOfAnalysedBoard = 0;
unsigned long long Counters::numberOfBestResultMessagesSend = 0;
