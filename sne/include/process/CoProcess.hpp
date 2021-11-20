#pragma once

#include "Process.hpp"
#include <task.hpp>

namespace Snow::exe
{
/// A coroutine Process.
/// A coroutine Process is a coroutine that is marked as finished only when the coroutine ends,
/// and returns from the operator() at each suspension.
class CoProcess : public Process
{
public:
    /// @brief Creates a CoProcess from a coroutine.
    /// @remarks
    /// We don't provide a constructor CoProcess(task<> t), because it can make a lot of bugs.
    /// If the lambda contains capture and is local to the function, then the task<> will contain dangling pointer.
    /// std::function sotre the type and target without template which is convenient.
    /// If we constructor from task<>, BAD EXAMPLE:
    ///     std::shared_ptr<CoProcess> pr;
    ///     {
    ///         auto lambda = [this]() -> task<>() { ... };
    ///         pr = std::make_shared<CoProcess>(lambda());
    ///     }
    ///     (*pr)(); // Undefined behavior
    ///     // Here lambda is destroyed, but pr contains still a value. All capture will be dangling pointers (even captures by value!).
    /// GOOD EXAMPLE:
    ///     std::shared_ptr<CoProcess> pr;
    ///     {
    ///         auto lambda = [this]() -> task<>() { ... };
    ///         pr = std::make_shared<CoProcess>(lambda);
    ///     }
    ///     (*pr)(); // Ok. Lambda is copied. No bug, plus being cleaner to write.
    /// @param functor The task. Should be callable, with no argument.
    CoProcess(std::function<task<>()> functor);

    /// @brief Creates a CoProcess that does nothing, unless if a child class overrides the coroutine() method.
    CoProcess() = default;

protected:
    void update() final;

    /// @brief The main coroutine of the process.
    /// @details By default, if m_handle is not-null, executes it, otherwise returns immediately.
    virtual task<> coroutine();

private:
    std::function<task<>()> m_functor;
    task<> m_handle;
};
}