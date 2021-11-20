if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
elseif(MSVC)
    message(STATUS "MSVC Compiler detected")
    message(STATUS "MSVC version: ${MSVC_VERSION}")
    # using MSVC
    add_compile_options("/await:strict")

    # fix msvc warning about exceptions
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

    # syntax only
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zs")

else()
    # Assume using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()