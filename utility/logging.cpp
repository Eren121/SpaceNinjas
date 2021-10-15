#include "logging.hpp"
#include <spdlog/sinks/stdout_sinks.h>

std::unordered_map<std::string, Logger::pointer> Logger::m_registeredLoggers;

Logger::pointer Logger::getOrCreate(const std::string& name)
{
    const auto it = m_registeredLoggers.find(name);
    if(it != m_registeredLoggers.end())
    {
        return it->second;
    }
    else
    {
        auto ret = create(name);
        m_registeredLoggers[name] = ret;
        return ret;
    }
}

Logger::pointer Logger::create(const std::string& name)
{
    auto ret = spdlog::stdout_logger_st(name);
    ret->set_level(spdlog::level::trace);
    return ret;
}


