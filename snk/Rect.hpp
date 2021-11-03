#pragma once

#include "Str.hpp"
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>

/// @brief Represents a Rect with the origin at bottom left (lower coordinates is origin).
template<typename T>
struct tRect
{
    using vec = glm::tvec2<T>;

    explicit tRect(const vec& size = {0, 0})
        : size(size)
    {
    }

    static tRect fromCorners(const vec& lower, const vec& upper)
    {
        tRect ret;
        ret.origin = lower;
        ret.size = upper - lower;

        return ret;
    }

    vec size { static_cast<T>(0) };
    vec origin { static_cast<T>(0) };

    vec getCenter() const
    {
        return origin + size / static_cast<T>(2);
    }

    /// @brief Shift the origin, the size is preserved.
    void setOriginFromCenter(const glm::vec2 center)
    {
        origin = center - size / static_cast<T>(2);
    }

    T left() const
    {
        return origin.x;
    }

    T right() const
    {
        return origin.x + size.x;
    }

    T top() const
    {
        return origin.y + size.y;
    }

    T bottom() const
    {
        return origin.y;
    }

    vec bottomLeft() const
    {
        return origin;
    }

    vec bottomRight() const
    {
        return {origin.x + size.x, origin.y};
    }

    vec topLeft() const
    {
        return {origin.x, origin.y + size.y};
    }

    vec topRight() const
    {
        return origin + size;
    }

    void translate(const glm::vec2& amount)
    {
        origin += amount;
    }

    vec radius() const
    {
        return size / 2.0f;
    }

    /// @brief Multiply the coordinates of all the points.
    tRect operator*(T factor) const
    {
        return {size * factor, origin * factor};
    }

    friend std::string to_string(const tRect& r)
    {
        return Str{} << "Rect(origin=" << to_string(r.origin) << ", size=" << to_string(r.size) << ")";
    }
};

using Rect = tRect<float>;
using Recti = tRect<int>;