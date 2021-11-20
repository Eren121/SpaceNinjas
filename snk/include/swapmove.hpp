#pragma once

#include <utility>

/// @brief If a class is trivially movable by swapping, and not copiable, add this to the function body.
/// @details This defines the copy/move constructors/assignations. Add this macro in public interface.
/// @param Class the enclosing class.
/// @param varargs (fields) All the fields that should be swapped, i.e.: &Class::myInt, &Class::myDouble
#define SWAP_MOVABLE(Class, ...) \
    /* Forbids copy */ \
    explicit Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete; \
    /* Allow move by swapping */ \
    Class(Class&& rhs) noexcept { *this = std::move(rhs); } \
    Class& operator=(Class&& rhs) noexcept { using std::swap; swap(*this, rhs); return *this; } \
    /* Define swap by sapping all the fields */ \
    friend void swap(Class& lhs, Class& rhs) { using std::swap; move_swap<Class, __VA_ARGS__>(lhs, rhs); }


/// @brief Generic implementation of swap the SWAP_MOVABLE macro uses.
/// @tparam Class the class that should be swapped.
/// @tparam fields All the fields of the class to use for swapping.
template<class Class, auto... fields>
void move_swap(Class& lhs, Class& rhs)
{
    using std::swap;
    (swap(lhs.*fields, rhs.*fields), ...);
}