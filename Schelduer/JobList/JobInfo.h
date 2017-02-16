#ifndef JOBINFO_H
#define JOBINFO_H

#include <string>

class JobInfo
{
    public:
        JobInfo();
        JobInfo(const std::string & jobid, const std::string & workerip, const unsigned int workerport, const unsigned long long maxtime);

        std::string GetJobId() { return jobId; }
        std::string GetWorkerIp() { return workerIp; }
        unsigned int GetWorkerPort() { return workerPort; }
        unsigned long long GetMaxTime() { return maxTime; }

    private:
        std::string jobId;
        std::string workerIp;
        unsigned int workerPort;
        unsigned long long maxTime;
};

#endif // JOBINFO_H
