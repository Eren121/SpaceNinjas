#include "ui/StoryMessage.hpp"
#include <process/Wait.hpp>
#include <iostream>

namespace sn::ui
{
    StoryMessage::StoryMessage(Stage &stage)
        : m_stage(stage)
    {
    }

    task<> StoryMessage::coroutine()
    {
        std::cout << m_message << std::endl;

        std::cout << "duration = " << m_duration.asMilliseconds() << "ms" << std::endl;
        co_await Snow::exe::Wait{m_duration};
    }
}
