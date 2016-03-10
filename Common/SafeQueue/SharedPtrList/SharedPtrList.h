#ifndef SHAREDPTRLIST_H
#define SHAREDPTRLIST_H

#include <mutex>
#include <boost/shared_ptr.hpp>
#include <list>
#include <condition_variable>
#include "../../Traces/Traces.h"

template<typename wskType>
class SharedPtrList
{
	public:
        SharedPtrList();

        std::condition_variable *GetCondVar();
        bool Empty();
        void SetCondVar(std::condition_variable * wsk);

        wskType PopFront();
        void PushBack(wskType wsk);

        ~SharedPtrList();
	private:       
        std::mutex mutex;
        std::list<wskType> setList;
        std::condition_variable *condition_var;
        std::condition_variable *org_condition_var;
};

template<typename wskType>
bool SharedPtrList<wskType>::Empty()
{
    Traces() << "\n" << "LOG: bool SharedPtrList<wskType>::Empty()";
    return setList.empty();
}

template<typename wskType>
SharedPtrList<wskType>::SharedPtrList()
{
    Traces() << "\n" << "LOG: SharedPtrList<wskType>::SharedPtrList()";
    condition_var = new std::condition_variable();
    org_condition_var = condition_var;
}

template<typename wskType>
std::condition_variable * SharedPtrList<wskType>::GetCondVar()
{
    Traces() << "\n" << "LOG: std::condition_variable * SharedPtrList<wskType>::GetCondVar()";
    return condition_var;
}

template<typename wskType>
wskType SharedPtrList<wskType>::PopFront()
{
    Traces() << "\n" << "LOG: std::shared_ptr<wskType> SharedPtrList<wskType>::PopFront()";

    std::lock_guard<std::mutex> ls(mutex);

    if (setList.empty())
    {        
        throw std::string("Empty");
    }

    return setList.pop_front();
}

template<typename wskType>
void SharedPtrList<wskType>::PushBack(wskType wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrList<wskType>::PushBack(std::shared_ptr<wskType> wsk)";

    std::lock_guard<std::mutex> ls(mutex);
    setList.push_back(wsk);
    condition_var->notify_one();

    Traces() << "\n" << "LOG: Number of element in list " << setList.size();
}

template<typename wskType>
SharedPtrList<wskType>::~SharedPtrList()
{
    Traces() << "\n" << "LOG: SharedPtrList<wskType>::~SharedPtrList()";
    delete org_condition_var;
}

template<typename wskType>
void SharedPtrList<wskType>::SetCondVar(std::condition_variable * wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrList<wskType>::SetCondVar(std::condition_variable * wsk)";
    condition_var = wsk;
}

#endif 
