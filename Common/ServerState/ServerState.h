#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include "../Board/Board.h"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class ServerState
{
    public:
        ServerState();
        ServerState(const Board & tmpCurrent,
                    const bool tmpThinking,
                    const unsigned long long tmpStartTime,
                    const unsigned long long tmpMaxTime,
                    const unsigned long long tmpTimeToEnd,
                    const std::string & serverError);

        void SetBoard(const Board & board) { std::lock_guard<std::mutex> ls(*mutex); current = board; }
        Board GetBoard() const { std::lock_guard<std::mutex> ls(*mutex); return current; }
        void SetPreviousBoard(const Board & board) { std::lock_guard<std::mutex> ls(*mutex); previousBoard = board; }
        Board GetPrviousBoard() const { std::lock_guard<std::mutex> ls(*mutex); return previousBoard; }
        void SetThinking(const bool flag) { std::lock_guard<std::mutex> ls(*mutex); thinking = flag; }
        bool IsThinking() const { std::lock_guard<std::mutex> ls(*mutex); return thinking; }
        void SetStartTime(const unsigned long long val) { std::lock_guard<std::mutex> ls(*mutex); startTime = val; }
        void SetMaxTime(const unsigned long long val) { std::lock_guard<std::mutex> ls(*mutex); maxTime = val;}
        void SetMaxTimeForWorkers(const unsigned long long val) { std::lock_guard<std::mutex> ls(*mutex); maxTimeForWorkers = val;}
        void SetTimeToEnd(const unsigned long long val) { std::lock_guard<std::mutex> ls(*mutex); timeToEnd = val; }
        unsigned long long GetStartTime() const { std::lock_guard<std::mutex> ls(*mutex); return startTime; }
        unsigned long long GetMaxTime() const { std::lock_guard<std::mutex> ls(*mutex); return maxTime; }
        unsigned long long GetMaxTimeForWorkers() const { std::lock_guard<std::mutex> ls(*mutex); return maxTimeForWorkers; }
        unsigned long long GetTimeToEnd() const { std::lock_guard<std::mutex> ls(*mutex); return startTime; }
        void SetlastServerError(const std::string & error) { lastServerError = error; }
        std::string GetLastServerError() const { return lastServerError; }

        const ServerState & operator=(const ServerState  & data);

        ~ServerState();

        static std::string NO_SERVER_ERROR_TEXT;

    private:
        Board current;
        Board previousBoard;
        bool thinking;
        unsigned long long startTime;
        unsigned long long maxTime;
        unsigned long long maxTimeForWorkers;
        unsigned long long timeToEnd;
        std::string lastServerError;

        std::mutex *mutex;        

};

#endif // SERVERSTATE_H
