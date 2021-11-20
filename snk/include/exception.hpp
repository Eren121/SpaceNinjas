#pragma once

#include "source_location.hpp"
#include <stdexcept>

namespace snk {
    /// @brief Exception class but with source file, number and function name information.
    class exception : public virtual std::exception
    {
    public:
        exception(const std::string& msg = "", const snk::source_location& location = snk::source_location::current());
        [[nodiscard]] const snk::source_location& getLocation() const;
        const char *what() const noexcept override;

    private:
        std::string m_message;
        snk::source_location m_location;
    };
}

