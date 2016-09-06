#include "Traces.h"

Traces::Traces()
{

}

/*Traces& Traces::operator <<(const std::string data)
{
    StringToFile(data);
}*/

void Traces::CreateTraceFolder()
{        
    std::string mkdirCommand = std::string("mkdir -p ") + traceFolder.c_str();    
    system(mkdirCommand.c_str());

    std::string removeOldLogsCommand = std::string("rm -f ") + traceFolder.c_str() + std::string("/*");
    system(removeOldLogsCommand.c_str());
}

void Traces::SetTraceFolder(const std::string &dir)
{
    traceFolder = dir;
    CreateTraceFolder();
}

void Traces::SetTraceFileNameMethod(const Traces::TraceFileNameMode mode)
{
    fileNameModeForTrace = mode;
}

unsigned long long Traces::GetThreadId()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    uint64_t id = std::stoull(ss.str());
    return id;
}

bool Traces::IsOnTheList()
{

    if (theardsId.empty()) return false;

    if (theardsId.find(GetThreadId()) != theardsId.map::end())
    {
        return true;
    };
    return false;
}

std::string Traces::CreateNewThreadText()
{
    std::string result;

    if (theardsId.empty())
    {
      result = GenerateText(1);
      theardsId[GetThreadId()] = result;
      idText.insert(result);
    } else
    {
        result = FindFirstFreeId();
        theardsId[GetThreadId()] = result;
        idText.insert(result);
    };

    return result;
}

std::string Traces::GenerateText(int i)
{
    return "thread_" + std::to_string(i);
}

std::string Traces::FindFirstFreeId()
{
    int id = 1;
    auto search = idText.find(GenerateText(id));

    while(search != idText.end())
    {        
        id++;
        search = idText.find(GenerateText(id));
    };
    return GenerateText(id);
}

std::string Traces::GetThreadText()
{
    if (fileNameModeForTrace == TraceFileNameMode::PrefixThreadAndNumberOfthread)
    {
        if (IsOnTheList())
        {
           return theardsId[GetThreadId()];
        };
    } else
    if (fileNameModeForTrace == TraceFileNameMode::OnlyThreadID)
    {
        std::stringstream ss;
        ss << GetThreadId();
        return ss.str();
    }

    return CreateNewThreadText();
}

Traces& Traces::operator <<(std::string data)
{        
        std::lock_guard<std::mutex> guard(mutex);
        StringToFile(data);
}

Traces& Traces::operator <<(const unsigned long long data)
{    
        std::lock_guard<std::mutex> guard(mutex);
        std::string number;
        std::stringstream strstream;
        strstream << data;
        strstream >> number;
        StringToFile(number);        
}

unsigned long long Traces::GetCurrentTime()
{
    if (!timeFlag)
    {
        start = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
        timeFlag = true;
    } else
    {
        stop = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
        timeFlag = false;       
        return stop - start;
    };    

    return 0;
}

std::string Traces::GetCurrentDate()
{
    std::string result;
    unsigned long long milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    unsigned long long counter = 1000000000;

    while (counter != 1)
    {
        result += std::to_string(milliseconds_since_epoch / counter) + ",";
        milliseconds_since_epoch = milliseconds_since_epoch % counter;
        counter = counter / 1000;
    };
    result += std::to_string(milliseconds_since_epoch);
    return result;
}


unsigned long long Traces::GetCurrentDateLL()
{
    unsigned long long milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

    return milliseconds_since_epoch;
}

void Traces::StringToFile(std::string log)
{    
    static std::ofstream logFile;
    logFile.open (traceFolder + "/" + GetThreadText().c_str(),std::ofstream::in | std::ofstream::app);

    if (log == "\n") log += GetThreadText() + "[" + GetCurrentDate() + "]";

    logFile << log;
    logFile.close();
}

void Traces::TurnOnTraces()
{
    traceOn = true;
}

void Traces::TurnOffTraces()
{
    traceOn = false;
}

bool Traces::GetTraceFlag()
{
    return traceOn;
}

void Traces::RemoveThreadID()
{
    std::lock_guard<std::mutex> guard(mutex);
    if (theardsId.find(GetThreadId()) != theardsId.map::end())
    {
        idText.erase(theardsId[GetThreadId()]);
        theardsId.erase(theardsId.find(GetThreadId()));
    };
}

std::string Traces::patchAndNameFile ="";
bool Traces::traceOn = false;
std::mutex Traces::mutex;
std::map<unsigned long long,std::string> Traces::theardsId;
std::set<std::string> Traces::idText;
bool Traces::timeFlag = false;
unsigned long long Traces::start =0;
unsigned long long Traces::stop =0;
std::string Traces::traceFolder = "traces";
Traces::TraceFileNameMode Traces::fileNameModeForTrace = Traces::TraceFileNameMode::OnlyThreadID;
