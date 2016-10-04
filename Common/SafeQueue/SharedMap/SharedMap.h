#pragma once
#ifndef SHAREMAP_H
#define SHAREDMAP_H

#include <mutex>
#include <boost/shared_ptr.hpp>
#include <map>
#include <stdexcept>
#include <condition_variable>
#include "../../Traces/Traces.h"
#include "../SharedPtrSet/SharedPtrList.h"

template<typename data1, typename data2>
class SharedMap
{
	public:
        SharedMap();

        data2 At(data1 &val);
        bool Empty();
        void Erase(data1 key);
        std::condition_variable *GetCondVar();
        int Insert(data1 key, data2 val);
        void SetCondVar(std::condition_variable * wsk);
        void GetAllKeys(SharedPtrList<data1> &return_list);

        ~SharedMap();
	private:       
        std::mutex mutex;
        std::map<data1, data2> mapList;
        std::condition_variable *condition_var;
        std::condition_variable *org_condition_var;
};

template<typename data1, typename data2>
data2 SharedMap<data1, data2>::At(data1 &val)
{
    std::lock_guard<std::mutex> ls(mutex);

    Traces() << "\n" << "LOG: data2 SharedMap<data1, data2>::At(data1 &val)";

    try
    {
        return mapList.at(val);
    }
    catch (...)
    {
        throw;
    }

}

template<typename data1, typename data2>
void SharedMap<data1, data2>::Erase(data1 key)
{
    std::lock_guard<std::mutex> ls(mutex);
    Traces() << "\n" << "LOG: void SharedMap<data1, data2>::Erase(data1 key)";
    mapList.erase(key);
}

template<typename data1, typename data2>
bool SharedMap<data1, data2>::Empty()
{
    Traces() << "\n" << "LOG: bool SharedMap<wskType>::Empty()";
    return mapList.empty();
}

template<typename data1, typename data2>
SharedMap<data1, data2>::SharedMap()
{
    Traces() << "\n" << "LOG: SharedMap<wskType>::SharedMap()";
    condition_var = new std::condition_variable();
    org_condition_var = condition_var;
}

template<typename data1, typename data2>
std::condition_variable * SharedMap<data1, data2>::GetCondVar()
{
    Traces() << "\n" << "LOG: std::condition_variable * SharedMap<wskType>::GetCondVar()";
    return condition_var;
}

template<typename data1, typename data2>
SharedMap<data1, data2>::~SharedMap()
{
    Traces() << "\n" << "LOG: SharedMap<data1, data2>::~SharedMap()";
    delete org_condition_var;
}

template<typename data1, typename data2>
int SharedMap<data1, data2>::Insert(data1 key, data2 val)
{
     std::lock_guard<std::mutex> ls(mutex);

     Traces() << "\n" << "LOG: int SharedMap<data1, data2>::Insert(data1 key, data2 val)";

     if (mapList.find(key) == mapList.end())
     {
        mapList[key] = val;
        return false;
     }

     return true;
}

template<typename data1, typename data2>
void SharedMap<data1, data2>::SetCondVar(std::condition_variable * wsk)
{
    Traces() << "\n" << "LOG: void SharedMap<data1, data2>::SetCondVar(std::condition_variable * wsk)";
    condition_var = wsk;
}

template<typename data1, typename data2>
void SharedMap<data1, data2>::GetAllKeys(SharedPtrList<data1> &return_list)
{
    Traces() << "\n" << "LOG: void SharedMap<data1, data2>::GetAllKeys(std::list &return_list)";

    std::lock_guard<std::mutex> ls(mutex);

    for(auto it = mapList.cbegin(); it != mapList.cend(); ++it)
    {
        return_list.PushBack(it->first);
    }

}

#endif 
