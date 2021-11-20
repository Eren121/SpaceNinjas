cmake_minimum_required(VERSION 3.17)
project(snk LANGUAGES CXX)

# Swizz Knife but with typo
# Utilities in a std-like style.

set(SRC
    include/algorithm.hpp
    include/binpacking/GuillotineBinPack.h
    include/binpacking/MaxRectsBinPack.h
    include/binpacking/Rect.h
    include/exception.hpp
    include/filesystem.hpp
    include/fmt.hpp
    include/formatter.hpp
    include/GridPosition.hpp
    include/guard.hpp
    include/io.hpp
    include/json.hpp
    include/lazy_resource.hpp
    include/list_iterator.hpp
    include/logging.hpp
    include/math.hpp
    include/offset_of.hpp
    include/Rect.hpp
    include/source_location.hpp
    include/str.hpp
    include/string_litteral.hpp
    include/swapmove.hpp
    include/time/Clock.hpp
    include/time/FPSCounter.hpp
    include/time/Time.hpp
    include/time/Timer.hpp
    include/unused.hpp
    include/version.hpp
    src/binpacking/GuillotineBinPack.cpp
    src/binpacking/MaxRectsBinPack.cpp
    src/binpacking/Rect.cpp
    src/exception.cpp
    src/GridPosition.cpp
    src/io.cpp
    src/logging.cpp
    src/math.cpp
    src/source_location.cpp
    src/str.cpp
    src/time/Clock.cpp
    src/time/FPSCounter.cpp
    src/time/Time.cpp
    src/time/Timer.cpp
    src/version.cpp)

add_library(snk ${SRC})
target_include_directories(snk PUBLIC include)
add_library(snk::snk ALIAS snk)

# ========================================================================= GLM

FetchContent_Declare(GLM GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(GLM)
target_link_libraries(snk PUBLIC glm::glm)

# ========================================================================= JSON

FetchContent_Declare(JSON GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.10.4)
FetchContent_MakeAvailable(JSON)
target_link_libraries(snk PUBLIC nlohmann_json::nlohmann_json)

# ========================================================================= spdlog

FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.9.2)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(snk PUBLIC spdlog::spdlog)
