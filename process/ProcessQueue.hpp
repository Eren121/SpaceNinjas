#pragma once

#include "Process.hpp"
#include <memory>
#include <queue>

/// @brief A queue of Processes.
/// @details
/// This process contains multiple processes that will execute in order. The next process is only executed when
/// the previous process is finished.
/// The process queue ends when the last process of the queue finishes.
class ProcessQueue : public Process
{
public:
    void push(std::shared_ptr<Process> p);
    [[nodiscard]] bool empty() const;

protected:
    void update() override;

private:
    std::queue<std::shared_ptr<Process>> m_queue;
};

