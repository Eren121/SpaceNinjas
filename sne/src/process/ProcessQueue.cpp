#include "process/ProcessQueue.hpp"

void Snow::exe::ProcessQueue::update()
{
    // If a process finishes, pass immediately to the next process
    while(!m_queue.empty())
    {
        auto& p = *m_queue.front();

        if (!p())
        {
            m_queue.pop();
        }
        else
        {
            // The process is not finished and will continue on the next step
            break;
        }
    }

    if(m_queue.empty())
    {
        stop();
    }
}

void Snow::exe::ProcessQueue::push(std::shared_ptr<Process> p)
{
    m_queue.push(std::move(p));
}

bool Snow::exe::ProcessQueue::empty() const
{
    return m_queue.empty();
}
