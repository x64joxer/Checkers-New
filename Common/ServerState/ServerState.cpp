#include "ServerState.h"

ServerState::ServerState() : thinking(false)
{
    current =
    std::string("| |w| |w| |w| |w|") +
    std::string("|w| |w| |w| |w| |") +
    std::string("| |w| |w| |w| |w|") +
    std::string("| | | | | | | | |") +
    std::string("| | | | | | | | |") +
    std::string("|b| |b| |b| |b| |") +
    std::string("| |b| |b| |b| |b|") +
    std::string("|b| |b| |b| |b| |");

    mutex = new std::mutex();
}

ServerState::ServerState(const Board & tmpCurrent,
            const bool tmpThinking,
            const unsigned long long tmpStartTime,
            const unsigned long long tmpMaxTime,
            const unsigned long long tmpTimeToEnd) :
            current(tmpCurrent),
            thinking(tmpThinking),
            startTime(tmpStartTime),
            maxTime(tmpMaxTime),
            timeToEnd(tmpTimeToEnd)
{
    mutex = new std::mutex();
}

const ServerState & ServerState::operator=(const ServerState  & data)
{
    std::lock_guard<std::mutex> ls(*mutex);
    Traces() << "\n" << "LOG: const ServerState & ServerState::operator=(const ServerState  & data)";

    current = data.current;
    thinking = data.thinking;
    startTime = data.startTime;
    maxTime = data.maxTime;
    timeToEnd = data.timeToEnd;

    return *this;
}

ServerState::~ServerState()
{
    delete mutex;
}
