#ifndef SHAREDPTRSET_H
#define SHAREDPTRSET_H

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

        bool Empty();
        std::condition_variable *GetCondVar();
        void SetCondVar(std::condition_variable * wsk);

        wskType PopFront();
        void PushBack(wskType wsk);

        ~SharedPtrSet();
	private:       
        std::mutex mutex;
        std::set<wskType> setList;
        std::condition_variable *condition_var;
        std::condition_variable *org_condition_var;
};

template<typename wskType>
bool SharedPtrSet<wskType>::Empty()
{
    Traces() << "\n" << "LOG: bool SharedPtrSet<wskType>::Empty()";
    return setList.empty();
}

template<typename wskType>
SharedPtrSet<wskType>::SharedPtrSet()
{
    Traces() << "\n" << "LOG: SharedPtrSet<wskType>::SharedPtrSet()";
    condition_var = new std::condition_variable();
    org_condition_var = condition_var;
}

template<typename wskType>
std::condition_variable * SharedPtrSet<wskType>::GetCondVar()
{
    Traces() << "\n" << "LOG: std::condition_variable * SharedPtrSet<wskType>::GetCondVar()";
    return condition_var;
}

template<typename wskType>
wskType SharedPtrSet<wskType>::PopFront()
{
    Traces() << "\n" << "LOG: std::shared_ptr<wskType> SharedPtrSet<wskType>::PopFront()";

    std::lock_guard<std::mutex> ls(mutex);

    if (setList.empty())
    {
        throw std::string("Empty");
    }

    auto it = setList.begin();
    setList.erase(it);
    return it;
}

template<typename wskType>
void SharedPtrSet<wskType>::PushBack(wskType wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrSet<wskType>::PushBack(std::shared_ptr<wskType> wsk)";

    std::lock_guard<std::mutex> ls(mutex);
    setList.insert(wsk);
    condition_var->notify_one();

    Traces() << "\n" << "LOG: Number of element in list " << setList.size();
}

template<typename wskType>
SharedPtrSet<wskType>::~SharedPtrSet()
{
    Traces() << "\n" << "LOG: SharedPtrSet<wskType>::~SharedPtrSet()";
    delete org_condition_var;
}

template<typename wskType>
void SharedPtrSet<wskType>::SetCondVar(std::condition_variable * wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrSet<wskType>::SetCondVar(std::condition_variable * wsk)";
    condition_var = wsk;
}

#endif 
