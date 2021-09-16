#pragma once

/// @file Simple preprocessing library.
/// @remarks Macros that need expansion wraps another macro with a same name but with a suffix number,
/// i.e. the macro TEST() will call TEST2() if defined.

/// @brief Get the first argument, ignoring all remaining arguments.
#define PP_FIRST(first, ...) first

/// @brief Get the arguments without the first argument.
#define PP_RFIRST(first, ...) __VA_ARGS__
