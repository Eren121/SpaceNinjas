cmake_minimum_required(VERSION 3.17)

# Swizz Knife but with typo
# Utilities in a std-like style.

set(SRC
        filesystem.hpp
        exception.cpp
        exception.hpp
    GridPosition.cpp
    GridPosition.hpp
        guard.hpp
        io.cpp
        io.hpp
        lazy_resource.hpp
    Rect.hpp
        str.cpp
        str.hpp
    algorithm.hpp
    binpacking/GuillotineBinPack.cpp
    binpacking/GuillotineBinPack.h
    binpacking/MaxRectsBinPack.cpp
    binpacking/MaxRectsBinPack.h
    binpacking/Rect.cpp
    binpacking/Rect.h
    list_iterator.hpp
    logging.cpp
    unused.hpp
    math.cpp
    math.hpp
    offset_of.hpp
    string_litteral.hpp
    swapmove.hpp
    time/Clock.cpp
    time/Clock.hpp
    time/FPSCounter.cpp
    time/FPSCounter.hpp
    time/Time.cpp
    time/Time.hpp
    time/Timer.cpp
    time/Timer.hpp
    fmt.hpp
    version.hpp
    version.cpp
    source_location.cpp
    source_location.hpp
)

add_library(snk ${SRC})
add_library(snk::snk ALIAS snk)
