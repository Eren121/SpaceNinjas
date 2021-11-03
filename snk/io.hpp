#pragma once

#include "exception.hpp"
#include <string>
#include <filesystem>

namespace snk
{
    /// @brief Exception thrown if there is an error with IO functions.
    class io_exception : public exception
    {
    public:
        using exception::exception;
    };

    class file_not_found_exception : public io_exception
    {
    public:
        file_not_found_exception(const std::filesystem::path& path, SOURCE_LOCATION_DECL(location));
    };


    /// @brief Read all the content of a file into a string.
    std::string read_all(const std::filesystem::path& path);
}