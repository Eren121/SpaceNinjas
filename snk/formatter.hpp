#pragma once

#include "fmt.hpp"
#include <string>
#include <concepts>

namespace snk
{
    /// @brief The type T should be compatible with to_string(T) and returns a type convertible to std::string.
    template<typename T>
    concept supports_to_string = requires(T t) {
        { to_string(t) } -> std::convertible_to<std::string>;
    };

    /// @brief Helper class to wrap formatters for simple use with to_string().
    template<supports_to_string T>
    struct formatter
    {
        constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
        {
            auto it = ctx.begin();

            if (it != ctx.end() && *it != '}') {
                throw fmt::format_error{"invalid format"};
            }

            return it;
        }

        template<typename FormatContext>
        auto format(const T &t, FormatContext &ctx) -> decltype(ctx.out())
        {
            return fmt::format_to(ctx.out(), "{}", to_string(t));
        }
    };
}