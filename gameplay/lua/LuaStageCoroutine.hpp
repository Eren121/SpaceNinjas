#pragma once

#include "process/Process.hpp"
#include "wrappers/lua/Coroutine.hpp"
#include <memory>

/// @brief An instance of a lua coroutine that can:
///   - pause and wait next world step, by yielding nothing
///   - run another Process and wait its completion, by yielding a std::shared_ptr<Process>.
/// @remarks If a Process is yielded, this Process will run once per world step, like the coroutine.
class LuaStageCoroutine : public Process
{
public:
    /// @remarks Obeys to the same rules as lua_utils::Coroutine constructor.
    explicit LuaStageCoroutine(lua_State *L, int nargs = 0);

protected:
    void update() override;

private:
    /// @brief Wrapped coroutine.
    lua_utils::Coroutine m_coro;
    
    /// @brief If one, yielded Process to run before resuming the coroutine.
    std::shared_ptr<Process> m_yielded;
};

