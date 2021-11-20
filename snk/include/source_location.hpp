#pragma once

#include <ostream>
#include <version> // feature testing

#define SOURCE_LOCATION_DECL(arg) const snk::source_location& arg = snk::source_location::current()

/// @file
/// @details Wrapper around C++20 std::source_location.
/// @details
///     More easely maintenable in case of compiler-specific options and changes until full support.
///     If there is no std::source_location, provide an empty class.


// Use feature-test-macro: https://en.cppreference.com/w/cpp/utility/feature_test
#if __cpp_lib_source_location >= 201907L
    #include <source_location>
    namespace snk
    {
        using source_location = std::source_location;
    }
#else
    /// @brief Mocked class, returns empty data for all its methods.
    namespace snk
    {
        class source_location
        {
        public:
            static consteval source_location current() { return {}; }

            constexpr const char* file_name() const noexcept { return "<unknown>"; }
            constexpr std::uint_least32_t line() const noexcept { return 0; }
            constexpr std::uint_least32_t column() const noexcept { return 0; }
            constexpr const char* function_name() const noexcept { return "<unknown>"; }
        };
    }
#endif

/// @brief Overload to print a source_location.
/// @details The form is file.cpp(line:column) `function`
/// @remarks This function is non-standard.
std::ostream& operator<<(std::ostream& lhs, const snk::source_location& rhs);