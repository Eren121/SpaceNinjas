#pragma once

#include <string>
#include <functional>
#include <task.hpp>

namespace Snow::exe
{
/// @brief Tool for managing the animation / game logic synchronization.
/// @remarks Implementation of https://gamedev.stackexchange.com/a/26887.
class Process
{
public:
    using Task = std::function<bool()>;

    /// @brief
    ///     By default, Creates a process that will only execute infinitely, and will do nothing,
    ///     unless called from a derived class that overrides update().
    /// @param task By default the update() method will execute this task, if it is not null. It is convenient
    /// to create task from lambdas.
    Process(Task task = nullptr);
    virtual ~Process() = default;

    /// @brief Executes the process.
    /// @returns
    ///     true, If the process is not finished and should continue running by a future operator() call.
    ///     false, if the process is finished.
    /// @remarks A stopped process can still re-executes, but you must be sure it is supported by the type of process.
    bool operator()();

    /// @brief Checks is the process is not finished and should continue running.
    /// @remarks Same as the return value of operator().
    [[nodiscard]] bool isRunning() const;

    /// @brief Stop the process.
    /// @details
    ///     Can be used internally by the process after stopping normally
    ///     as well as externally to force the process to stop.
    void stop();

    /// @brief Check if the Process is currently inside an update
    [[nodiscard]] bool isLocked() const;

    /// @brief Wait until the Process completion.
    task<> operator co_await();

protected:
    /// @brief Update the process, called by the operator() wrapper.
    /// @details If not overrided, this method does nothing and never stops.
    virtual void update();

private:
    bool m_running;

    /// @brief Only true during a call of operator()().
    /// @details
    ///     Some operations may be impossible during an update, like adding to a container while iterating it,
    ///     so we might need to check it.
    bool m_locked;

    Task m_task;
};
}