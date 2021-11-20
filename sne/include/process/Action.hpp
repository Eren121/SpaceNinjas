#pragma once

#include "Process.hpp"

namespace Snow::exe
{
/// @brief An Action is a Process that executes only once.
class Action : public Process
{
public:
    void update() override;

protected:
    virtual void action() = 0;
};
}
namespace sn::exe
{
}