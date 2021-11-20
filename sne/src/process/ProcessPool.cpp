#include "process/ProcessPool.hpp"

void Snow::exe::ProcessPool::update()
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

void Snow::exe::ProcessPool::add(std::shared_ptr<Process> p)
{
    m_processes.push_back(p);
}

void Snow::exe::ProcessPool::remove(std::shared_ptr<Process> p)
{
    m_processes.remove(p);
}