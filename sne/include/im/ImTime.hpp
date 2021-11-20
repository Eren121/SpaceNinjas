#pragma once

#include "ImBase.hpp"
#include "ImBaseFormatFloat.hpp"
#include "ImBaseRange.hpp"
#include <time/Time.hpp>

class ImTime :
    public ImBase<ImTime, Time>,
    public ImBaseRange<ImTime, Time>,
    public ImBaseFormatFloat<ImTime>
{
public:
    enum Unit {
        Second,
        Millisecond
    };

    ImTime()
    {
        resetSuffix();
    }

    /// @param resetSuffix_ If true, the format suffix will be reset to have a suffix to reflect the chosen unit.
    ImTime& setUnit(Unit unit_, bool resetSuffix_ = true)
    {
        unit = unit_;
        if(resetSuffix_)
        {
            resetSuffix();
        }

        return *this;
    }

    Unit unit = Second; ///< In which unit display the value

protected:
    void showImpl() override;

private:
    /// @brief Reset the format with a suffix reflecting the unit
    void resetSuffix()
    {
        suffix = findSuffix(unit);
    }

    /// @brief Get the multiplier of time in the unit. The reference is the second, having a multiplier of 1.
    static float findMultiplier(Unit unit)
    {
        switch(unit)
        {
            case Second: return 1.0f;
            case Millisecond: return 1000.0f;
            default: return 1.0f;
        }
    }

    static const char* findSuffix(Unit unit)
    {
        switch(unit)
        {
            case Second: return "s";
            case Millisecond: return "ms";
            default: return "?";
        }
    }
};