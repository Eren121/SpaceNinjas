#pragma once

#include "Fwd.hpp"
#include <process/CoProcess.hpp>
#include <time/Time.hpp>

namespace sn::ui
{
    /// @brief Message that appears to the player in Stage.
    class StoryMessage : public Snow::exe::CoProcess
    {
    public:
        explicit StoryMessage(Stage &stage);

        /// @brief Setters are effective before the coroutine is executed.
        /// @{
        void setMessage(const std::string& msg) { m_message = msg; }
        void setDuration(Time duration) { m_duration = duration; }
        /// @}

    protected:
        task<> coroutine() override;

    private:
        Stage& m_stage;
        std::string m_message;
        Time m_duration;
    };
}