#pragma once

#include "Process.hpp"
#include <list>
#include <memory>

namespace Snow::exe
{
/// @brief Contains multiple process.
/// @details
/// Unlike a ProcessQueue, a ProcessPool contains multiple processes that will execute concurrently.
/// The ProcessPool stops when there is no more process running. The processes are destroyed when they get finished.
class ProcessPool : public Snow::exe::Process
{
public:
    /// @brief Add a Process to the ProcessPool.
    /// @param p The process to add.
    void add(std::shared_ptr<Process> p);

    /// @brief Remove a Process from the ProcessPool
    /// @param p The process to remove.
    void remove(std::shared_ptr<Process> p);

protected:
    void update() override;

protected:
    /// @remarks Stored as a std::list, so we can add while iterating it (in update()).
    std::list<std::shared_ptr<Process>> m_processes;
};
}
