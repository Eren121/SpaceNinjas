#pragma once

#include <algorithm>
#include <cstddef>

/// Permit to use litteral strings in template like X<"ABC">.
template<std::size_t n>
struct StringLiteral
{
    constexpr StringLiteral(const char (&str)[n])
    {
        std::copy_n(str, n, value);
    }
    
    char value[n];
};