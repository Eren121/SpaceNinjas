cmake_minimum_required(VERSION 3.17)
project(SpaceNinja LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)

set(VENDOR "${CMAKE_SOURCE_DIR}/vendor")

include(FetchContent)
include(EnableCoroutines)

add_subdirectory(snk)
add_subdirectory(sne)
add_subdirectory(sn)