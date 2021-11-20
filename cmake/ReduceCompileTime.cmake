# Precompiled headers (for C++ only to avoid some errors)
# target_precompile_headers(SDL_OpenGL PRIVATE
#    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp>")


# Unity builds
# set_target_properties(SDL_OpenGL PROPERTIES UNITY_BUILD ON)
# set_target_properties(SDL_OpenGL PROPERTIES UNITY_BUILD_MODE BATCH UNITY_BUILD_BATCH_SIZE 10)
