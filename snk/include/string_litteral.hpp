#pragma once

#include <algorithm>
#include <cstddef>

namespace snk
{
    /// @brief Permit to use litteral strings in template like X<"ABC">.
    /// @details https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
    template<std::size_t n>
    struct string_literal
    {
        constexpr string_literal(const char (&str)[n]) // NOLINT(google-explicit-constructor)
        {
            std::copy_n(str, n, value);
        }

        char value[n] {};
    };
}