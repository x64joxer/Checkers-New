#include "Worker.h"

Worker::Worker()
{

}

Worker::Worker(const std::map<std::string, std::string> & data)
{
    SetState(Peers::STATE(atoi(data.at(MessageCoder::STATE).c_str())));
    SetMaxThread(atoi(data.at(MessageCoder::NUM_OF_THREAD).c_str()));
}

Worker::~Worker()
{
    TRACE_FLAG_FOR_CLASS_Worker Traces() << "\n" << "LOG: Worker::~Worker()";
}
