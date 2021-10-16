#pragma once

/// @dir
/// @brief Wrapper around C++20 components.
/// @details More easely maintenable in case of compiler-specific options and changes until full support.

#include <ostream>
#include <version> // feature testing


#define SOURCE_LOCATION_DECL(arg) const nostd::source_location& arg = nostd::source_location::current()

namespace nostd
{
#if __cpp_lib_source_location > 201907L
#   include <experimental/source_location>
    using source_location = std::experimental::source_location;
#else
    // Mocked class
    class source_location
    {
    public:
        static consteval source_location current() { return {}; }

        constexpr const char* file_name() const noexcept { return "<unknown>"; }
        constexpr std::uint_least32_t line() const noexcept { return 0; }
        constexpr std::uint_least32_t column() const noexcept { return 0; }
        constexpr const char* function_name() const noexcept { return "<unknown>"; }
    };

#endif
}

/// @brief Overload to print a source_location.
/// @details The form is file.cpp(line:column) `function`
/// @remarks This function is non-standard.
std::ostream& operator<<(std::ostream& lhs, const nostd::source_location& rhs);