#include "ProcessStack.hpp"


void ProcessStack::update()
{
    // If a process finishes, pass immediately to the next process
    while(!m_stack.empty())
    {
        auto& p = *m_stack.top();

        if (!p())
        {
            m_stack.pop();
        }
        else
        {
            // The process is not finished and will continue on the next step
            break;
        }
    }

    if(m_stack.empty())
    {
        stop();
    }
}

void ProcessStack::push(std::shared_ptr<Process> p)
{
    m_stack.push(std::move(p));
}

bool ProcessStack::empty() const
{
    return m_stack.empty();
}
