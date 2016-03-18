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
}

const ServerState & ServerState::operator=(const ServerState  & data)
{
    std::lock_guard<std::mutex> ls(*mutex);
    Traces() << "\n" << "LOG: ServerState & ServerState::operator=(const ServerState  & data)";

    current = data.current;
    thinking = data.thinking;
    startTime = data.startTime;
    maxTime = data.maxTime;
    timeToEnd = data.timeToEnd;

    return *this;
}

ServerState::~ServerState()
{

}
