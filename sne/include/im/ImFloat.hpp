#pragma once

#include "ImBase.hpp"
#include "ImBaseFormatFloat.hpp"
#include "ImBaseRange.hpp"

/// @brief Read/write float value inside a range, with a default value.
/// @remarks Guaranteed to have an unique ImGUI identifier per instance (based on class address).
class ImFloat
    : public ImBase<ImFloat, float>,
      public ImBaseFormatFloat<ImFloat>,
      public ImBaseRange<ImFloat, float>
{
protected:
    void showImpl() override;
};