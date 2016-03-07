#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <mutex>
#include <boost/shared_ptr.hpp>
#include <set>
#include <condition_variable>
#include "../../Traces/Traces.h"

template<typename wskType>
class SharedPtrSet
{
	public:
        SharedPtrSet();

        std::condition_variable *GetCondVar();
        void SetCondVar(std::condition_variable * wsk);

        std::shared_ptr<wskType> PopFront();
        void PushBack(std::shared_ptr<wskType> wsk);

        ~SharedPtrSet();
	private:       
        std::mutex mutex;
        std::set<std::shared_ptr<wskType> > setList;
        std::condition_variable *condition_var;
};

#endif 
