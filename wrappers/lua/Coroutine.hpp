#pragma once

#include "wrappers/lua/Lua.hpp"
#include <sol/forward.hpp>

namespace lua_utils
{
    /// @brief Lua Coroutine wrapper into a Process.
    /// @details The coroutine is protected from garbage collection. It will only be deleted when the destructor
    /// will be called.
    /// The Process stops when the coroutine ends.
    class Coroutine
    {
    public:
        /// @brief Creates an invalid coroutine
        explicit Coroutine() = default;

        Coroutine(Coroutine&&) noexcept;
        Coroutine& operator=(Coroutine&&) noexcept;

        Coroutine(const Coroutine&) = delete;
        Coroutine& operator=(const Coroutine&) = delete;

        /// @brief Create a coroutine from a generator function and a lua State.
        /// @param nargs The count of arguments to give to the coroutine. You must push yourself the function, then
        /// the arguments to L, the arguments will be moved to the new thread. The function and the arguments will be
        /// cleaned from the stack.
        /// @details The coroutine will pushSceneStage immediately.
        Coroutine(lua_State *L, int nargs = 0);

        /// @brief The destructor.
        /// @details It will stop the coroutine if there is one.
        ~Coroutine();

        /// @brief Resumes the coroutine.
        /// @details The coroutine should be resumable. The arguments should be pushed to the stack. They are pop by
        /// this function.
        /// @param nres If null, then the any yielded value will be ignored and cleared from the stack.
        ///             If not null, then @p nres values will be pushed to the stack. These values are the yielded
        ///             values. The count of value will be adjusted from the waited count, that is @p nres values will
        ///             be always pushed to the stack, popping exceeding values and pushing nil if needed.
        /// @returns true If the coroutine is still resumable after the call.
        ///          false If the coroutine is ended.
        bool operator()(int *nres = nullptr);

        /// @brief Check if the Coroutine is resumable.
        operator bool() const;

        /// @brief Get the Lua thread representing this coroutine.
        /// @details Use this to retrieve the yield values.
        lua_State* getThread() const;

        void close();

        friend void swap(Coroutine& a, Coroutine& b);

    private:
        lua_State *m_thread {nullptr};
        bool m_resumable {false};
        int m_nargs {0};

        /// @brief Set the registry value of key "this" to avoid beging garbage-collected.
        /// @remarks If m_thread == null, then do nothing.
        /// @param present If true, REGISTRY[this] = thread
        ///                If false, REGISTRY[this] = nil
        void addToRegistry(bool present);
    };

    void swap(Coroutine& a, Coroutine& b);
}
