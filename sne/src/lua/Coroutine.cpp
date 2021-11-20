#include "lua/Coroutine.hpp"
#include "lua/LuaException.hpp"
#include "lua/LuaGuard.hpp"
#include <cassert>
#include <str.hpp>
#include <sol/sol.hpp>

using namespace lua_utils;

Coroutine::Coroutine(lua_State *L, int nargs)
    : m_nargs(nargs)
{
    // https://www.lua.org/pil/27.3.1.html
    // To prevent GC, why use the registry
    // The key is the address this
    // REGISTRY[this] = newThread

    {
        LuaGuard guard(L);

        // create thread + push it
        m_thread = lua_newthread(L);
        if(!m_thread)
        {
            lua_pop(L, 1); // pop light user data
            throw LuaException(L, "Failed to allocate Lua thread");
        }

        addToRegistry(true);
        lua_pop(L, 1);
    }

    // the function + arguments should be on top of the stack
    lua_xmove(L, m_thread, 1 + m_nargs); // lua_xmove() pop n values of the first argument to push to the second argument

    m_resumable = true;
}

Coroutine::Coroutine(Coroutine&& rhs) noexcept
{
    swap(*this, rhs);
}

Coroutine& Coroutine::operator=(Coroutine&& rhs) noexcept
{
    swap(*this, rhs);
    return *this;
}

Coroutine::~Coroutine()
{
    if(m_thread)
    {
        close();
    }
}

void Coroutine::close()
{
    assert(m_thread);

    // The coroutine will be close by the GC, by removing the reference

    // Clear the registry to allow GC
    addToRegistry(false);

    m_thread = nullptr;
    m_resumable = false;
    m_nargs = 0;
}

bool Coroutine::operator()(int *nres)
{
    assert(m_thread);

    // No resume parameters except the first time to give the arguments of the function

    int nresLocal; // Number of values the Lua coroutine yield
    const int code = lua_resume(m_thread, nullptr, m_nargs, &nresLocal);

    // Only use args for the first argument
    m_nargs = 0;

    if(code == LUA_YIELD || code == LUA_OK)
    {
        if(nres)
        {
            // The caller will pop the values

            *nres = nresLocal;
        }
        else
        {
            // If the caller does not want to get the returned values,
            // clear the stack from the yielded values

            if(nresLocal > 0)
            {
                lua_pop(m_thread,  nresLocal);
            }
        }
    }
    else
    {
        if(nres) *nres = 0;
        // Error, set the yield count to zero
    }

    switch(code)
    {
        case LUA_YIELD:
            // The coroutine yielded, continue
            break;

        case LUA_OK:
            // The coroutine finished without errors
            close();
            break;

        default:
            // Error
            std::string error = lua_tostring(m_thread, -1);
            lua_pop(m_thread, 1);


            LuaException e{m_thread, error};

            close();

            // create the error, close then throw
            throw e;
    }

    return m_resumable;
}

Coroutine::operator bool() const
{
    return m_resumable;
}

lua_State *Coroutine::getThread() const
{
    return m_thread;
}

void lua_utils::swap(Coroutine &a, Coroutine &b)
{
    // We also need to swap the registry key, so we remove and then re-add to update the keys
    
    std::swap(a.m_thread, b.m_thread);
    std::swap(a.m_resumable, b.m_resumable);
    std::swap(a.m_nargs, b.m_nargs);

    // dont call addToRegistry(false) because it co garbage-collect if it is the last value.
    // Overwrite only, which is sufficient.
    
    a.addToRegistry(true);
    b.addToRegistry(true);
}

void Coroutine::addToRegistry(bool present)
{
    if(m_thread)
    {
        LuaGuard guard(m_thread);
        lua_pushlightuserdata(m_thread, this);

        if(present)
        {
            lua_pushthread(m_thread);
        }
        else
        {
            lua_pushnil(m_thread);
        }

        lua_settable(m_thread, LUA_REGISTRYINDEX);
    }
}
