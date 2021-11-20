#pragma once

#include "string_litteral.hpp"
#include <spdlog/logger.h>
#include <spdlog/fmt/ostr.h> // To provide {}-format support for types overloading operator<<()
#include <memory>
#include <unordered_map>

#define LOGGABLE(name) protected: static auto& getLogger() { static auto ret{Logger::getOrCreate(name)}; return *ret; } private:

/// @brief Wrapper around 'spdlog' library..
/// @details Automatically creates and get loggers.
class Logger
{
public:
    using pointer = std::shared_ptr<spdlog::logger>;
    
    /// @brief Get a logger instance.
    /// @details The logger is automatically created and automatically deleted at the end of the process.
    static pointer getOrCreate(const std::string& name);
    
    /// @brief Creates a logger, without registering it
    static pointer create(const std::string& name);
    
private:
    static std::unordered_map<std::string, pointer> m_registeredLoggers;
};

template<snk::string_literal name>
class Loggable
{
public:
    Loggable() :
        m_logger(Logger::getOrCreate(name.value))
    {
    }
    
    virtual ~Loggable() = default;
    
protected:
    spdlog::logger& getLogger() const
    {
        return *m_logger;
    }
    
private:
    Logger::pointer m_logger;
};