#pragma once

#include "Process.hpp"
#include <memory>
#include <stack>

/// @brief LIFO structure to store processes.
/// @details The stack equivalent to the queue version, ProcessQueue. The top process only is executed and removed
/// from the stack when it is finished.
/// @remarks It is useful to make a pausable Process: if the first process of the stack pushes a Wait process, then
/// it will resume only when the Wait process will finish.
class ProcessStack : public Process
{
public:
    void push(std::shared_ptr<Process> p);
    [[nodiscard]] bool empty() const;

protected:
    void update() override;

private:
    std::stack<std::shared_ptr<Process>> m_stack;
};

