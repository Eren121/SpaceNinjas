#pragma once

#include "Process.hpp"
#include <time/Time.hpp>
#include <functional>

namespace Snow::exe
{
/// @brief A process that do nothing but last a specific amount of time.
/// @details The sleep can be stopped to finish immediately.
/// @remarks The time point can be customized, it's not obligatorily the real clock time.
class Wait : public Snow::exe::Process
{
public:
    /// @brief The time source type.
    /// @details The returned value should be the elapsed time.
    /// @remarks The time source can be variable in speed, or pause, but should never go in the past (newer
    /// calls should get higher or equals values).
    using TimeSource = std::function<Time()>;

    /// @brief By default, it use the real time clock.
    explicit Wait(Time amount, TimeSource source = Time::now);

    /// @brief Get the progress of the wait, representing the remaining time, in percent, in [0;1].
    /// 0 means the process just started, 1 mean the process will stop.
    float getProgress() const;

protected:
    void update() override;

private:
    TimeSource m_source;
    Time m_start;
    Time m_end;
};
}

