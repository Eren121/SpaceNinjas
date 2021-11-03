#pragma once

// Wrapper around {fmt} library
// The reason to use a wrapper is because include path can change if spdlog use external or internal fmt.

#include <spdlog/fmt/fmt.h>

namespace Snow
{
    template<typename T>
    struct to_string_formatter
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

/// @brief Make a type supports format with to_string
#define SNOW_DEFINE_USER_FMT(Concept) \
    template<Concept T> \
    struct fmt::formatter<T> : Snow::to_string_formatter<T> {}