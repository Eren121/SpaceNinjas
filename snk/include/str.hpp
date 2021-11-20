#pragma once

#include <sstream>

namespace snk
{
    /// @brief Tag class for snk::str. Ensure the class is not used anywhere else.
    static inline constexpr struct end_t {} end;

    /// @brief Facilitate the use of simple formated inline strings.
    /// @remarks For more complicated strings, use {fmt}.
    /// @example
    ///     std::string a = "1 is a number";
    ///     std::string b = str{} << 1 << " is a number".
    ///     // a == b
    class str
    {
    public:
        /// @brief Convert the str to a std::string. Same as str().
        /// @remarks Non-explicit to allow automatic conversions.
        operator std::string() const; // NOLINT(google-explicit-constructor)

    public:
        /// @brief Overload of operator<<() to automatically convert to str to a std::string.
        /// @details
        ///     It is because it is much simpler to use this than wrap the string around parenthesis and call str().
        ///     In some cases, implicit conversion doesn't work, for example giving a str as an argument taking a type that
        ///     has an std::string constructor (C++ doesn't make indirect conversions).
        /// @example
        ///     <code>
        ///         std::string a = (str{} << 1 << " hi").str(); // Possible, but tedious
        ///         std::string b = str{} << 1 << " hi" << str::end; // Much cleaer and simpler, no need for parenthesis.
        ///         // a == b
        ///     </code>
        [[nodiscard]] std::string operator<<(const end_t&) const;

        /// @brief Append any value to the string.
        template<typename T>
            requires (!std::is_same_v<T, end_t>)
        str& operator<<(const T& str)
        {
            m_stream << str;
            return *this;
        }

    private:
        std::ostringstream m_stream;
    };
}

namespace
{
    template<typename... T>
    std::string cat(T&&... t)
    {
        return (snk::str{} << ... << std::forward<T>(t));
    }
}