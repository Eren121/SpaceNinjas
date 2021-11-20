#pragma once

#include <sstream>

/// @brief Helper class to implement for types that need a format string for floating point numbers.
template<typename CRTP>
class ImBaseFormatFloat
{
public:
    CRTP& setPrecision(int precision_)
    {
        precision = precision_;
        return static_cast<CRTP&>(*this);
    }

    CRTP& setSuffix(const std::string& suffix_)
    {
        suffix = suffix_;
        return static_cast<CRTP&>(*this);
    }

    int precision = 5; ///< Count of digits to print after the comma for the format
    std::string suffix; ///< Suffix to add to the format string

protected:
    std::string buildFormat() const
    {
        std::string format = "%.";
        format += std::to_string(precision);
        format += "f";

        if(!suffix.empty()) {
            format += " ";
            format += suffix;
        }

        return format;
    }
};