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

class Traces
{
    public:
        Traces();

        Traces& operator <<(const std::string);
        Traces& operator <<(const unsigned long long);

        static unsigned long long GetCurrentTime();
        static std::string GetCurrentDate();
        static unsigned long long GetCurrentDateLL();

        static void TurnOnTraces();
        static void TurnOffTraces();
        static bool GetTraceFlag();
        static void RemoveThreadID();
        static void SetTraceFolder(const std::string &dir);

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
};

#endif // TRACES_H
