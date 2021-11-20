#pragma once

#include "ImBase.hpp"
#include "ImBaseFormatFloat.hpp"
#include "ImBaseRange.hpp"

class ImInt
    : public ImBase<ImInt, int>,
      public ImBaseRange<ImInt, int>
{
protected:
    void showImpl() override;
};