#include "Process.hpp"
#include <utility/Guard.hpp>

Snow::exe::Process::Process(function<bool()> task)
    : m_running(true), m_locked(false), m_task(task)
{
}

bool Snow::exe::Process::isRunning() const
{
    return m_running;
}

void Snow::exe::Process::stop()
{
    m_running = false;
}

bool Snow::exe::Process::isLocked() const
{
    return m_locked;
}

bool Snow::exe::Process::operator()()
{
    m_running = true;
    m_locked = true;

    WhenLeaveScope {
        m_locked = false;
    };

    update();

    return m_running;
}

void Snow::exe::Process::update()
{
    if(m_task)
    {
        // Execute the task if it exists

        if(!m_task())
        {
            // Stop when the task stops
            stop();
        }
    }
}

task<> Snow::exe::Process::operator co_await()
{
    while((*this)())
    {
        co_await std::suspend_always{};
    }
}