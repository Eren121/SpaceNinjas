#include "process/Process.hpp"
#include <snk/guard.hpp>

namespace Snow::exe
{
    Process::Process(std::function<bool()> task)
        : m_task(task)
    {
    }

    bool Process::isRunning() const
    {
        return m_running;
    }

    void Process::stop()
    {
        m_running = false;
    }

    bool Process::isLocked() const
    {
        return m_locked;
    }

    bool Process::isFirstUpdate() const
    {
        return m_isFirstUpdate;
    }

    bool Process::run()
    {
        m_running = true;
        m_locked = true;

        WhenLeaveScope {
            m_locked = false;

            if(m_isFirstUpdate)
            {
                m_isFirstUpdate = false;
            }
        };

        update();

        return m_running;
    }

    void Process::update()
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

    task<> Process::operator co_await()
    {
        while((*this)())
        {
            co_await std::suspend_always{};
        }
    }
}