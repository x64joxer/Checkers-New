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

ServerState & ServerState::operator=(const ServerState  & data)
{
    std::lock_guard<std::mutex> ls(mutex);
    Traces() << "\n" << "LOG: ServerState & ServerState::operator=(const ServerState  & data)";

    *this = data;
    return *this;
}

ServerState::~ServerState()
{

}
