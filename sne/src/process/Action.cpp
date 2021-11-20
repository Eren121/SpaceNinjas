#include "process/Action.hpp"

void Snow::exe::Action::update()
{
    action();
    Process::stop();
}