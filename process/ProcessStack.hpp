#pragma once

#include "Process.hpp"
#include <memory>
#include <stack>

namespace Snow::exe
{
/// @brief LIFO structure to store processes.
/// @details The stack equivalent to the queue version, ProcessQueue. The top process only is executed and removed
/// from the stack when it is finished.
/// @remarks It is useful to make a pausable Process: if the first process of the stack pushes a Wait process, then
/// it will resume only when the Wait process will finish.
class ProcessStack : public Snow::exe::Process
{
public:
    void push(shared_ptr<Process> p);
    [[nodiscard]] bool empty() const;

protected:
    void update() override;

private:
    stack<shared_ptr<Process>> m_stack;
};
}

