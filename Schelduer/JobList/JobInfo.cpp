#include "JobInfo.h"

JobInfo::JobInfo()
{

}

JobInfo::JobInfo(const std::string & jobid, const std::string & workerip, const unsigned int workerport, const unsigned long long maxtime)
                : jobId(jobid),
                  workerIp(workerip),
                  workerPort(workerport),
                  maxTime(maxtime)
{

}
