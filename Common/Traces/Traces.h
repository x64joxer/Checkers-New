#ifndef TRACES_H
#define TRACES_H

#include <ostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>

#define TRACE01 if (true)

class Traces
{
    public:
        Traces();

        enum class TraceFileNameMode { OnlyThreadID, PrefixThreadAndNumberOfthread };

        Traces& operator <<(const std::string);
        Traces& operator <<(const unsigned long long);

        static unsigned long long GetCurrentTime();
        static unsigned long long GetSecondsSinceEpoch();
        static std::string GetCurrentDate();
        static unsigned long long GetCurrentDateLL();

        static void TurnOnTraces();
        static void TurnOffTraces();
        static bool GetTraceFlag();
        static void RemoveThreadID();
        static void SetTraceFolder(const std::string &dir);
        static void SetTraceFileNameMethod(const Traces::TraceFileNameMode mode);

    private:
        static std::string patchAndNameFile;
        inline void StringToFile(std::string log);
        static bool traceOn;
        static std::mutex mutex;
        static std::map<unsigned long long,std::string> theardsId;
        static std::set<std::string> idText;

        static void CreateTraceFolder();
        static unsigned long long GetThreadId();
        bool IsOnTheList();
        std::string GetThreadText();
        std::string CreateNewThreadText();        
        std::string FindFirstFreeId();
        static std::string GenerateText(int);
        static bool timeFlag;
        static unsigned long long start,stop;
        static std::string traceFolder;        
        static TraceFileNameMode fileNameModeForTrace;
};

#endif // TRACES_H
