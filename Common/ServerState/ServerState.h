#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include "../Board/Board.h"
#include "../Traces/Traces.h"

class ServerState
{
    public:
        ServerState();

        void SetBoard(const Board & board) { std::lock_guard<std::mutex> ls(mutex); current = board; }
        Board GetBoard() { std::lock_guard<std::mutex> ls(mutex); return current; }
        void SetThinking(const bool flag) { std::lock_guard<std::mutex> ls(mutex); thinking = flag; }
        bool IsThinking() { std::lock_guard<std::mutex> ls(mutex); return thinking; }
        void SetStartTime(const unsigned long long val) { std::lock_guard<std::mutex> ls(mutex); startTime = val; }
        void SetMaxTime(const unsigned long long val) { std::lock_guard<std::mutex> ls(mutex); maxTime = val;}
        void SetTimeToEnd(const unsigned long long val) { std::lock_guard<std::mutex> ls(mutex); timeToEnd = val; }
        unsigned long long GetStartTime() { std::lock_guard<std::mutex> ls(mutex); return startTime; }
        unsigned long long GetMaxTime() { std::lock_guard<std::mutex> ls(mutex); return maxTime; }
        unsigned long long GetTimeToEnd() { std::lock_guard<std::mutex> ls(mutex); return startTime; }

        ServerState & operator=(const ServerState  & data);

        ~ServerState();

    private:
        Board current;
        bool thinking;
        unsigned long long startTime;
        unsigned long long maxTime;
        unsigned long long timeToEnd;

        std::mutex mutex;

};

#endif // SERVERSTATE_H
