#pragma once

// Wrapper around {fmt} library
// The reason to use a wrapper is because include path can change if spdlog use external or internal fmt.

#include <spdlog/fmt/fmt.h>