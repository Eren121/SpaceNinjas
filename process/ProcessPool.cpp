#include "ProcessPool.hpp"

void ProcessPool::update()
{
    for(auto it = m_processes.begin(); it != m_processes.end();)
    {
        auto& processPtr = *it;
        auto& process = *processPtr;

        bool isActive = process();
        if(!isActive)
        {
            it = m_processes.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if(m_processes.empty())
    {
        stop();
    }
}

void ProcessPool::add(std::shared_ptr<Process> p)
{
    m_processes.push_back(p);
}

void ProcessPool::remove(std::shared_ptr<Process> p)
{
    m_processes.remove(p);
}
