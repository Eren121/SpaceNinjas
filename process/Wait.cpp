#include "Wait.hpp"
#include <iostream>

Wait::Wait(Time amount, TimeSource source)
    : m_source(std::move(source)),
      m_start(m_source()), m_end(m_start + amount) // source moved, care to not use source but m_source
{
}

void Wait::update()
{
    if(m_source() >= m_end)
    {
        stop();
    }
}

float Wait::getProgress() const
{
    return std::clamp((m_source() - m_start) / (m_end - m_start), 0.0f, 1.0f);
}
