// Custom settings of Box2D for this project
// Base copied from b2_settings.h

/// @file Single file inside the directory to not pollute the include path.

#pragma once

#include <stdarg.h>
#include <stdint.h>

// Tunable Constants

/// You can use this to change the length scale used by your game.
/// For example for inches you could use 39.4.
#define b2_lengthUnitsPerMeter 1.0f

/// The maximum number of vertices on a convex polygon. You cannot increase
/// this too much because b2BlockAllocator has a maximum object size.
#define b2_maxPolygonVertices	8

// User data

/// You can define this to inject whatever data you want in b2Body

#include <memory>

// shared_ptr because copied from BodyDef (but not used)...

#include <functional>

namespace b2
{
    /// @details For performance reasons, no pointer check is done,
    /// construct and destruct should be implemented as non-null functions in user code.
    /// shared_ptr because box2D will copy it once with *Def classes.

    struct BodyUserData
    {
        static std::function<void(BodyUserData&)> construct;
        BodyUserData() { construct(*this); }
        std::shared_ptr<void> userData{nullptr};
    };

    struct FixtureUserData
    {
        static std::function<void(FixtureUserData&)> construct;
        FixtureUserData() { construct(*this); }
        std::shared_ptr<void> userData{nullptr};
    };
}

using b2BodyUserData = b2::BodyUserData;
using b2FixtureUserData = b2::FixtureUserData;

/// You can define this to inject whatever data you want in b2Joint
struct B2_API b2JointUserData
{
	b2JointUserData()
	{
		pointer = 0;
	}

	/// For legacy compatibility
	uintptr_t pointer;
};

// Memory Allocation

/// Default allocation functions
B2_API void* b2Alloc_Default(int32 size);
B2_API void b2Free_Default(void* mem);

/// Implement this function to use your own memory allocator.
inline void* b2Alloc(int32 size)
{
	return b2Alloc_Default(size);
}

/// If you implement b2Alloc, you should also implement this function.
inline void b2Free(void* mem)
{
	b2Free_Default(mem);
}

/// Default logging function
B2_API void b2Log_Default(const char* string, va_list args);

/// Implement this to use your own logging.
inline void b2Log(const char* string, ...)
{
	va_list args;
	va_start(args, string);
	b2Log_Default(string, args);
	va_end(args);
}
