#pragma once

#include "stage/StageSceneNode.hpp"
#include "wrappers/lua/Coroutine.hpp"
#include "process/Process.hpp"
#include "utility/logging.hpp"
#include <memory>

namespace SpaceNinja::script
{
    /// @brief An instance of a lua coroutine that can:
    ///   - pause and wait next world step, by yielding nothing
    ///   - run another Process and wait its completion, by yielding a std::shared_ptr<Process>.
    /// @remarks If a Process is yielded, this Process will run once per world step, like the coroutine.
    class Thread : public Snow::exe::Process
    {
        LOGGABLE("ProcessCoroutine")

    public:
        /// @remarks See lua_utils::Coroutine::Coroutine(), this constructor has the same prerequisites for the Lua stack.
        explicit Thread(lua_State *L, int nargs = 0);

    protected:
        void update() override;

    private:
        /// @brief Wrapped coroutine.
        lua_utils::Coroutine m_coro;

        /// @brief If one, yielded Process to run before resuming the coroutine.
        std::shared_ptr<Snow::exe::Process> m_yielded;
    };
}

