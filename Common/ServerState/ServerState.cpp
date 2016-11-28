#include "ServerState.h"

ServerState::ServerState() : thinking(false),
                             lastServerError(ServerState::NO_SERVER_ERROR_TEXT),
                             whiteWins(false),
                             blackWins(false)
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
            const unsigned long long tmpTimeToEnd,
            const std::string & serverError) :
            current(tmpCurrent),
            thinking(tmpThinking),
            startTime(tmpStartTime),
            maxTime(tmpMaxTime),
            timeToEnd(tmpTimeToEnd),
            lastServerError(serverError)
{
    mutex = new std::mutex();
}

const ServerState & ServerState::operator=(const ServerState  & data)
{
    std::lock_guard<std::mutex> ls(*mutex);
    TRACE_FLAG_FOR_CLASS_ServerState Traces() << "\n" << "LOG: const ServerState & ServerState::operator=(const ServerState  & data)";

    current = data.current;
    thinking = data.thinking;
    startTime = data.startTime;
    maxTime = data.maxTime;
    timeToEnd = data.timeToEnd;
    lastServerError = data.lastServerError;

    return *this;
}

ServerState::~ServerState()
{
    delete mutex;
}

std::string ServerState::NO_SERVER_ERROR_TEXT = "NO_ERROR";
