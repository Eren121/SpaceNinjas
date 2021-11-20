#pragma once

#include "ImBase.hpp"
#include "ImBaseFormatFloat.hpp"
#include "ImBaseRange.hpp"
#include <glm/vec2.hpp>

class ImFloat2
    : public ImBase<ImFloat2, glm::vec2>,
      public ImBaseFormatFloat<ImFloat2>,
      public ImBaseRange<ImFloat2, float>
{
protected:
    void showImpl() override;
};