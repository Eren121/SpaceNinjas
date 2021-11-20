#pragma once

#include "ImBase.hpp"

class ImBool : public ImBase<ImBool, bool>
{
protected:
    void showImpl() override;
};