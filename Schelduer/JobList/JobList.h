#ifndef JOBLIST_H
#define JOBLIST_H

#include "../SafeQueue/SharedMap/SharedMap.h"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"
#include "JobInfo.h"

class JobList
{
    public:
        JobList();
        void AddJob(const std::string & jobid, const std::string & workerip, const unsigned int workerport, const unsigned long long maxtime);
        void Clear();
        void RemoveJob(const std::string & jobid);
        void GetJobList(SharedPtrList<std::string> & jobIdlist);
        bool IsJobId(std::string & jobid);

    private:
        SharedMap<std::string, JobInfo> jobMap;

};

#endif // JOBLIST_H
