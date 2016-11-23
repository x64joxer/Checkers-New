#include "JobList.h"

JobList::JobList()
{
    TRACE_FLAG_FOR_CLASS_JobList Traces() << "\n" << "LOG: JobList::JobList()";
}

void JobList::AddJob(const std::string & jobid, const std::string & workerip, const unsigned int workerport, const unsigned long long maxtime)
{
    TRACE_FLAG_FOR_CLASS_JobList Traces() << "\n" << "LOG: void JobList::AddJob(const std::string & jobid, const std::string & workerip, const unsigned int workerport, const unsigned long long maxtime)";

    jobMap.Insert(jobid, JobInfo(jobid, workerip, workerport, maxtime));
}

void JobList::Clear()
{
    TRACE_FLAG_FOR_CLASS_JobList Traces() << "\n" << "LOG: void JobList::Clear()";

    jobMap.Clear();
}

void JobList::GetJobList(SharedPtrList<std::string> & jobIdlist)
{
    TRACE_FLAG_FOR_CLASS_JobList Traces() << "\n" << "LOG: void JobList::GetJobList(SharedPtrList<std::string> & jobIdlist)";

    jobMap.GetAllKeys(jobIdlist);
}

void JobList::RemoveJob(const std::string & jobid)
{
    TRACE_FLAG_FOR_CLASS_JobList Traces() << "\n" << "LOG: void JobList::RemoveJob(const std::string & jobid)";

    jobMap.Erase(jobid);
}

bool JobList::IsJobId(std::string & jobid)
{
    try
    {
        jobMap.At(jobid);
    }
    catch(...)
    {
        return false;
    }

    return true;
}
