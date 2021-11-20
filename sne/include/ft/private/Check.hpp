#pragma once

#include "../FTException.hpp"

/// @brief Check a Freetype call by checking there is no error.
#define FT_Check(call) do { int error = (call); if(error) { throw FTException(error, #call " failed"); } } while(0)