#pragma once

#include <string>

enum class Victory
{
    Running = -1,
    Loss = 0,
    Win = 1,
};

namespace
{
    std::string to_string(Victory v)
    {
        switch (v)
        {
            case Victory::Running:
                return "Running";
            
            case Victory::Loss:
                return "Loss";
            
            case Victory::Win:
                return "Win";
            
            default:
                return "???";
        }
    }
}