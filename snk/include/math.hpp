#pragma once

/// @file Mathematical and random utilities that only depends to GLM.

/// @brief More portable mathematical constants
/// @remarks MSVC does not defines M_PI without some defines, and it can even not works if <cmath> was imported before
#define PI 3.14159265358979323846f
#define TAU (2.0f * PI)
#define SQRT2 1.41421356237309504880f

#include "formatter.hpp"
#include <cmath>
#include <algorithm>
#include <random>
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>

namespace glm
{
    template<typename T>
    void glm_check_inside_namespace(const T&) {}
}

/// @brief Check if a type belongs to the 'glm' library.
template<typename T>
concept glm_type = requires(T t)
{
    glm_check_inside_namespace(t);
};

/// @brief Make 'glm' library types supports fmt::format().
template<glm_type T>
struct fmt::formatter<T> : snk::formatter<T> {};

namespace math
{
    /// @brief Convert a signed normalized range [-1;1] to unsigned normalized range [0;1].
    auto normCoordsToUnsigned(const auto& vec)
    {
        using T = typename std::remove_reference_t<decltype(vec)>::value_type;
        return (vec + static_cast<T>(1)) / static_cast<T>(2);
    }

    using std::clamp;

    /// @brief Inverse error function
    /// @details https://stackovclow.com/questions/27229371/inverse-error-function-in-c
    inline float erfinv(float x)
    {
        float tt1, tt2, lnx, sgn;
        sgn = (x < 0) ? -1.0f : 1.0f;

        x = (1 - x)*(1 + x);        // x = 1 - x*x;
        lnx = std::log(x);

        tt1 = 2.0f/(PI*0.147f) + 0.5f * lnx;
        tt2 = 1.0f/(0.147f) * lnx;

        return(sgn*std::sqrt(-tt1 + std::sqrt(tt1*tt1 - tt2)));
    }

    /// @brief Returns a random number between min and max, evenly distributed.
    /// @details
    ///     It use C++11 randomness, so it should be trustable, but use it for non-deterministic needed stuff like
    ///     effects, graphics, audio, etc... The engine is statically allocated and global to all the program.
    /// @param min The minimum value (included).
    /// @param max The maximum value (excluded).
    inline float randf(float min = 0.0f, float max = 1.0f)
    {
        static std::random_device r;

        std::uniform_real_distribution<float> dist(min, max);
        return dist(r);
    }

    /// @brief Returns approximately the value given as an argument, but with a deviation in percentage.
    /// @param mean The mean value, which can be slightly altered by the percentage.
    /// @param deviation
    ///     A value between [0;+inf] which indicate how at maximum to change the value.
    ///     Often this value is in [0;1]. For example, 1 would mean at maximum double or halve the mean,
    ///     10 would mean return at most mean * 10 and at least mean / 10.
    ///     0 returns always mean.
    /// @returns
    ///     The returned value is from a normal distribution probability, with the input as the mean.
    ///     The returned value is never higher than mean * (1 + percent) or lower than mean * (1 - percent).
    ///     Individually and discretly, there will be an overrepresentation of bounds because the values are clamped.
    inline auto approx(auto mean, float deviation)
    {
        float percent = deviation;
        // This value is to compute the standard deviation.
        // We consider the probability to be between [mean*(1-percent);mean*(1+percent)] to be @p probability %.
        // Values not in this interval are clamped to the limit.
        // You should care that this value is high enough, otherwise bounds will be overrepresented in the samples.
        static const float probability = 0.999f;

        // Logic: If we want the range [-a;a] to be 99%:
        // how to compute the standard deviation ? (this is independant from mean)
        // P(-a < X < a) = P(X < a) - P(X < -a)
        //               = erf(a / sigma / sqrt2) = 0.99
        // So sigma = a / (sqrt2 * erfinv(0.99)) = a / denom
        // The denominator is a constant, we can cache it
        static const float denom = SQRT2 * erfinv(probability);

        auto a = percent;
        auto stddev = a / denom;

        static std::random_device d;
        std::normal_distribution<float> dist(0.0f, stddev);

        // Actual percentage computed to deviate from the original value
        float randomPercent = clamp(dist(d), -percent, percent);

        return (1.0f + randomPercent) * mean;
    }


    /// @brief Get the sign of a number
    /// @returns
    /// -1 if val < 0,
    /// 0 if val == 0
    /// 1 if val > 0
    template <typename T> T sgn(T val)
    {
        // https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
        // But here we rather returns a T to avoid implicit conversion warnings when assigns to float for example
        return (T(0) < val) - (val < T(0));
    }

    /// @brief Linear interpolation.
    /// @details The pushSceneStage value is @p x, the end value is @y, and the progression is @p a.
    /// @param x The starting value.
    /// @param y The ending value.
    /// @param a The progression between x and y, should be between 0.0 and 1.0 (clamped if it's not in the interval).
    /// @remarks Same as <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mix.xhtml">GLSL mix()</a>.
    /// @remarks Same as lerp but supports any type that has arithmetic operators.
    template<typename T>
    T mix(T x, T y, float a)
    {
        a = clamp(a, 0.0f, 1.0f);
        return x * (1 - a) + y * a;
    }

    /// @brief Randomize a little bit an input for a more organic feeling.
    /// @details baseValue The value without deviation
    /// @param deviation A percent of deviation in [0...1].
    /// @return
    /// A random value baseValue deviated by a maximum of deviation or -deviation percentage.
    /// The probability of each value is uniform.
    template<typename T>
    auto deviate(const T& baseValue, float deviation)
    {
        return randf(1.0f - deviation, 1.0f + deviation) * baseValue;
    }

    /// @brief Generate a vector from the trigonometric circle.
    /// @param angle The angle of the vector from the unit circle (angle with vector {1, 0}).
    /// @param radius The size of the vector to create. By default, it create an unit vector, so the vector from the unit circle.
    /// @return The vector {cos(angle) * radius, sin(angle) * radius}
    inline glm::vec2 angle2vec(float angle, float radius = 1.0f)
    {
        return {std::cos(angle) * radius, std::sin(angle) * radius};
    }

    /// @brief Opposite of angle2vec, get the angle of the vector.
    /// @return The angle of the vector, in radian, in the interval [-pi;pi] which equals atan2(v.y, v.x).
    /// @details
    ///     The length of the vector does not matters, unless the vector is zero.
    ///     In this case, the result is undefined.
    inline float vec2angle(const glm::vec2& v)
    {
        return std::atan2(v.y, v.x); // in interval [-pi;-pi]
    }

    /// @brief Normalize an angle in interval [-pi;-pi]
    inline float normalize(float angle)
    {
        angle = fmodf(angle, TAU); // restrict in range [0;2pi]

        if(angle > PI)
        {
            angle = angle - TAU; // restrict in range [-pi;-pi]
        }

        return angle;
    }

    /// @param angle Unnormalized angle (can take any float value).
    // @brief Normalize an angle in interval [0;2pi]
    /// @remarks _u stands for unsigned because [0;2pi] is unsigned and [-pi;pi] is signed.
    inline float normalize_u(float angle)
    {
        // Same angle restrained in interval [-2pi;2pi]
        angle = fmod(angle, 2.0f * PI);

        // If negative, make positive
        if(angle < 0)
        {
            angle += 2.0f * PI;
        }

        return angle;
    }

    /// @brief Get the angular distance between two points
    /// @returns The signed, angular distance as the smallest angle from @p from to @p to.
    inline float angularDist(float from, float to)
    {
        float d = std::fmod(to - from, TAU); // if one is -pi and the other is pi, angle is zero
        float magnitude = std::abs(d);
        
        if(magnitude > PI)
        {
            // If angle is larger than PI, go in the opposite direction because the path is shorter
            // Also "negate" the angle (tau-magnitude) because we go in the opposite direction


            d = -sgn(d) * (TAU - magnitude);
        }

        return d;
    }

    /// @brief Returns the angle between two angle
    /// @returns The angle signed, between origin and target
    /// @details Returns always the smallest angle in one of the two directions.
    inline float angleBetween(float origin, float target)
    {
        float ret{target - origin};

        if(std::abs(ret) > PI)
        {
            ret = -(2 * PI - ret);
        }

        return ret;
    }

    /// @brief Linear mix two angles, returns in [-pi;-pi]
    /// @param a in [0;1]
    inline float angularMix(float x, float y, float a)
    {
        float dist = angularDist(x, y);
        return normalize(x + mix(0.0f, dist, a));
    }
    
    
    /// @brief Positive modulo
    /// @param max Exclusive upper bound.
    template<typename T>
    T positive_mod(T value, T max)
    {
        value %= max;
        
        if(value < 0)
        {
            value = max + value;
        }
        
        return value;
    }
}