#pragma once

#include <exception.hpp>

/// @brief Exception to throw for FreeType errors.
class FTException : public snk::exception
{
public:
    explicit FTException(int ft_error, const std::string& msg = "", const snk::source_location& loc = snk::source_location::current());
};
