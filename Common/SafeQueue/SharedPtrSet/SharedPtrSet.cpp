#include "SharedPtrSet.h"

template<typename wskType>
SharedPtrSet<wskType>::SharedPtrSet()
{
    Traces() << "\n" << "LOG: SharedPtrSet<wskType>::SharedPtrSet()";
    condition_var = new std::condition_variable();
}

template<typename wskType>
std::condition_variable * SharedPtrSet<wskType>::GetCondVar()
{
    Traces() << "\n" << "LOG: std::condition_variable * SharedPtrSet<wskType>::GetCondVar()";
    return condition_var;
}

template<typename wskType>
std::shared_ptr<wskType> SharedPtrSet<wskType>::PopFront()
{
    Traces() << "\n" << "LOG: std::shared_ptr<wskType> SharedPtrSet<wskType>::PopFront()";

    std::lock_guard<std::mutex> ls(mutex);

    if (setList.empty())
    {
        return nullptr;
    }

    auto it = setList.begin();
    setList.erase(it);
    return it;
}

template<typename wskType>
void SharedPtrSet<wskType>::PushBack(std::shared_ptr<wskType> wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrSet<wskType>::PushBack(std::shared_ptr<wskType> wsk)";

    std::lock_guard<std::mutex> ls(mutex);
    setList.insert(wsk);
    condition_var->notify_one();
}

template<typename wskType>
SharedPtrSet<wskType>::~SharedPtrSet()
{
    Traces() << "\n" << "LOG: SharedPtrSet<wskType>::~SharedPtrSet()";
    delete condition_var;
}

template<typename wskType>
void SharedPtrSet<wskType>::SetCondVar(std::condition_variable * wsk)
{
    Traces() << "\n" << "LOG: void SharedPtrSet<wskType>::SetCondVar(std::condition_variable * wsk)";
    condition_var = wsk;
}
