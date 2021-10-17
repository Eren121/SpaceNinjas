#include "LuaStageCoroutine.hpp"
#include "wrappers/lua/LuaException.hpp"
#include "process/Process.hpp"
#include <sol/state_view.hpp>

LuaStageCoroutine::LuaStageCoroutine(lua_State *L, int nargs)
    : m_coro(L, nargs)
{
}

void LuaStageCoroutine::update()
{
    if(m_yielded)
    {
        if(!(*m_yielded)())
        {
            m_yielded = {};
            // Yielded process is finished
        }
    }
    else if(m_coro)
    {
        int nres{0}; // Number of results = number of yielded values
        
        if(!m_coro(&nres))
        {
            if(nres != 0)
            {
                throw lua_utils::LuaException("Can't return value from LuaStageCoroutine");
            }
            
            // The process stopped
            stop();
        }
        else
        {
            if (nres == 1)
            {
                // Yielded, only yield of type std::shared_ptr<Process> is allowed
                // We should also clear the stack
            
                // Use sol2 conveniently to convert the object
                sol::state_view view(m_coro.getThread());
            
                {
                    // Scope to be sure there is no more use because we will pop it
                    // It will also throw an error if the yielded value has not the allowed type
                
                    sol::object yieldedObj(view, -1);
                    m_yielded = std::move(yieldedObj.as<std::shared_ptr<Process>>());
                }
            
                lua_pop(view, 1); // Pop the yielded return process
            }
            else if (nres != 0)
            {
                throw lua_utils::LuaException("Should yield only one or zero values");
            }
        }
    }
}