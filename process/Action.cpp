#include "Action.hpp"
#include "Process.hpp"

void Snow::exe::Action::update()
{
    action();
    Process::stop();
}