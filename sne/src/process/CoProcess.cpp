#include "process/CoProcess.hpp"
#include <cassert>
#include <utility>

using namespace std;

Snow::exe::CoProcess::CoProcess(function<task<>()> functor)
    : m_functor(move(functor)), m_handle()
{
    // Having the functor is very convenient to restart the process!
    // Plus, even if we don't use it by not restarting, we still need it to not have a dangling pointer.
}

void Snow::exe::CoProcess::update()
{
    if(!m_handle.is_resumable())
    {
        // No task where given to constructor, we will use coroutine() one.
        // WARNING: if the process was stopped, then the coroutine is recreated and will run once again.
        // This will **work** with constructor argument because we gave the functor generating the coroutine as argument.
        m_handle = coroutine();
    }

    assert(m_handle.is_resumable());
    if(!m_handle())
    {
        stop();
    }
}

task<> Snow::exe::CoProcess::coroutine()
{
    if(m_functor) // NOOP if null functor or the child class does not override this function (returns immediately, no wait)
    {
        co_await m_functor();
    }
}