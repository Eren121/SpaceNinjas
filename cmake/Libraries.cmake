include_directories("thirdparty/sigslot/include")
include_directories("thirdparty/box2d/include")
include_directories("thirdparty/CPP20Coroutines/include")
include_directories("thirdparty/lua")
include_directories("thirdparty/cereal/include")
link_directories("thirdparty/lua")
include_directories("thirdparty/sol2/include")
add_definitions(-DSOL_ALL_SAFETIES_ON=1)
add_definitions(-DSOL_USING_CXX_LUA=1)
link_directories("thirdparty/box2d/build/bin")
include_directories("thirdparty/freetype/include")
include_directories("thirdparty/spdlog/include")
link_directories("thirdparty/spdlog/build")

# Who knows why it doesnt work
# It probably because it works with system library but not project compiled one
#link_directories("thirdparty/freetype/build")

find_package(ImGUI REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory("${VENDOR}/entt")

add_subdirectory("${VENDOR}/lua-5.4.3")


#######################################
# CPP20Coroutines

include_directories("${VENDOR}/CPP20Coroutine/include")

#######################################
# Box2D

# To simplify custom user data for Box2D, directly binding to our class
add_definitions(-DB2_USER_SETTINGS)

set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(box2d_HOME "${VENDOR}/box2d")
add_subdirectory("${box2d_HOME}" "${PROJECT_BINARY_DIR}/box2d")

#######################################
# Freetype

add_subdirectory("${VENDOR}/freetype" "${PROJECT_BINARY_DIR}/freetype")
target_compile_definitions(freetype PUBLIC FT2_BUILD_LIBRARY)

#######################################
# FMOD

set(FMOD_HOME "${THIRD_PARTY_DIR}/fmod")

if(WIN32)
    set(FMOD_LIBS "${FMOD_HOME}/lib/x64")
    link_directories(${FMOD_LIBS})
    add_library(FMOD INTERFACE)
    target_link_libraries(FMOD INTERFACE fmodL_vc)
endif()

include_directories("${FMOD_HOME}/inc")