cmake_minimum_required(VERSION 3.17)
project(SDL_OpenGL LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")

### Compiler dependant support ########

# Coroutines
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
elseif(MSVC)
    message(STATUS "MSVC Compiler detected")

    # using MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /await:strict")

    # fix msvc warning about exceptions
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    # Assume using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()


#######################################


include_directories(.)
include_directories("thirdparty/sigslot/include")
include_directories("thirdparty/box2d/include")
include_directories("thirdparty/CPP20Coroutines/include")
include_directories("thirdparty/lua")
include_directories("thirdparty/cereal/include")
link_directories("thirdparty/lua")
include_directories("thirdparty/sol2/include")
add_definitions(-DSOL_ALL_SAFETIES_ON=1)
link_directories("thirdparty/box2d/build/bin")
include_directories("thirdparty/freetype/include")
include_directories("thirdparty/spdlog/include")
link_directories("thirdparty/spdlog/build")

# Go out find why it doesnt work
# It probably because it works with system library but not project compiled one
#link_directories("thirdparty/freetype/build")

include_directories("thirdparty/imgui/")
include_directories("thirdparty/imgui/backends")
set(SRC
    OpenGL.hpp
    Game.cpp
    Game.hpp
    GameControls.cpp
    GameControls.hpp
        media/TextureCache.cpp
        media/TextureCache.hpp
        media/WindowDebugInfo.cpp
        media/WindowDebugInfo.hpp
    gameplay/Body.cpp
    gameplay/Body.hpp
    gameplay/CollisionManager.cpp
    gameplay/CollisionManager.hpp
    gameplay/PlayerControl.cpp
    gameplay/PlayerControl.hpp
    gameplay/PlayerProcess.cpp
    gameplay/PlayerProcess.hpp
    gameplay/PlayerShoot.cpp
    gameplay/PlayerShoot.hpp
    gameplay/Save.cpp
    gameplay/Save.hpp
    gameplay/Stage.cpp
    gameplay/Stage.hpp
    gameplay/StageWorld.cpp
    gameplay/StageWorld.hpp
    gameplay/UIStage.cpp
    gameplay/UIStage.hpp
    gameplay/Victory.hpp
    gameplay/VictoryMenu.cpp
    gameplay/VictoryMenu.hpp
    gameplay/lua/LuaAPI.cpp
    gameplay/lua/LuaAPI.hpp
    gameplay/lua/LuaEngine.cpp
    gameplay/lua/LuaEngine.hpp
    gameplay/lua/LuaStageCoroutine.cpp
    gameplay/lua/LuaStageCoroutine.hpp
    main.cpp
    media/ResourceCache.hpp
    media/Window.cpp
    media/Window.hpp
    media/input/Action.cpp
    media/input/Action.hpp
    media/input/Axis.cpp
    media/input/Axis.hpp
    media/input/AxisAction.cpp
    media/input/AxisAction.hpp
    media/input/BoolAction.cpp
    media/input/BoolAction.hpp
    media/input/InputListener.cpp
    media/input/InputListener.hpp
    media/input/Key.cpp
    media/input/Key.hpp
    media/input/UnifiedInput.cpp
    media/input/UnifiedInput.hpp
    media/input/VectorAction.cpp
    media/input/VectorAction.hpp
    media/scene/SceneNode.cpp
    media/scene/SceneNode.hpp
    media/scene/SceneStack.cpp
    media/scene/SceneStack.hpp
    process/Action.cpp
    process/Action.hpp
    process/CoProcess.cpp
    process/CoProcess.hpp
    process/Process.cpp
    process/Process.cpp
    process/Process.hpp
    process/Process.hpp
    process/ProcessPool.cpp
    process/ProcessPool.hpp
    process/ProcessQueue.cpp
    process/ProcessQueue.hpp
    process/ProcessStack.cpp
    process/ProcessStack.hpp
    process/Wait.cpp
    process/Wait.hpp
    test/Test.cpp
    test/Test.hpp
    test/TestInput.cpp
    test/TestInput.hpp
    test/TestTransformable.cpp
    test/TestTransformable.hpp
    test/TestTransparent.cpp
    test/TestTransparent.hpp
    ui/Input.cpp
    ui/Input.hpp
    ui/MenuStage.cpp
    ui/MenuStage.hpp
    ui/VerticalListMenu.cpp
    ui/VerticalListMenu.hpp
    utility/Exception.cpp
    utility/Exception.hpp
    utility/GridPosition.cpp
    utility/GridPosition.hpp
    utility/Guard.hpp
    utility/IO.cpp
    utility/IO.hpp
    utility/LazyResource.hpp
    utility/Rect.hpp
    utility/Str.cpp
    utility/Str.hpp
    utility/algorithm.hpp
    utility/binpacking/GuillotineBinPack.cpp
    utility/binpacking/GuillotineBinPack.h
    utility/binpacking/MaxRectsBinPack.cpp
    utility/binpacking/MaxRectsBinPack.h
    utility/binpacking/Rect.cpp
    utility/binpacking/Rect.h
    utility/list_iterator.hpp
    utility/logging.cpp
    utility/macro/pp.hpp
    utility/macro/unused.hpp
    utility/math.cpp
    utility/math.hpp
    utility/offset_of.hpp
    utility/string_litteral.hpp
    utility/swapmove.hpp
    utility/time/Clock.cpp
    utility/time/Clock.hpp
    utility/time/FPSCounter.cpp
    utility/time/FPSCounter.hpp
    utility/time/Time.cpp
    utility/time/Time.hpp
    utility/time/Timer.cpp
    utility/time/Timer.hpp
    wrappers/DebugWindow.cpp
    wrappers/DebugWindow.hpp
    wrappers/SDL.cpp
    wrappers/SDL.hpp
    wrappers/box2d/DebugDraw.cpp
    wrappers/box2d/DebugDraw.hpp
    wrappers/box2d/World.cpp
    wrappers/box2d/World.hpp
    wrappers/box2d/box2d.hpp
    wrappers/freetype/FTException.cpp
    wrappers/freetype/FTException.hpp
    wrappers/freetype/Font.cpp
    wrappers/freetype/Font.hpp
    wrappers/freetype/FontType.cpp
    wrappers/freetype/FontType.hpp
    wrappers/freetype/Glyph.hpp
    wrappers/freetype/Text.cpp
    wrappers/freetype/Text.hpp
    wrappers/freetype/private/Check.hpp
    wrappers/freetype/private/DynamicRectPacker.cpp
    wrappers/freetype/private/DynamicRectPacker.hpp
    wrappers/freetype/private/FT.cpp
    wrappers/freetype/private/FT.hpp
    wrappers/freetype/private/Face.cpp
    wrappers/freetype/private/Face.hpp
    wrappers/freetype/private/FontPacker.cpp
    wrappers/freetype/private/FontPacker.hpp
    wrappers/freetype/private/raii/Face.cpp
    wrappers/freetype/private/raii/Face.hpp
    wrappers/freetype/private/raii/Library.cpp
    wrappers/freetype/private/raii/Library.hpp
    wrappers/freetype/private/raii/Stroker.cpp
    wrappers/freetype/private/raii/Stroker.hpp
    wrappers/gl/Circle.cpp
    wrappers/gl/Circle.hpp
    wrappers/gl/ConvexShape.cpp
    wrappers/gl/ConvexShape.hpp
    wrappers/gl/Drawable.hpp
    wrappers/gl/GL.cpp
    wrappers/gl/GL.hpp
    wrappers/gl/Line.cpp
    wrappers/gl/Line.hpp
    wrappers/gl/RenderStates.hpp
    wrappers/gl/Shader.cpp
    wrappers/gl/Shader.hpp
    wrappers/gl/Shape.cpp
    wrappers/gl/Shape.hpp
    wrappers/gl/Sprite.cpp
    wrappers/gl/Sprite.hpp
    wrappers/gl/Texture.cpp
    wrappers/gl/Texture.hpp
    wrappers/gl/Transformable.cpp
    wrappers/gl/Transformable.hpp
    wrappers/gl/Vertex.cpp
    wrappers/gl/Vertex.hpp
    wrappers/gl/VertexArray.cpp
    wrappers/gl/VertexArray.hpp
    wrappers/lua/Coroutine.cpp
    wrappers/lua/Coroutine.hpp
    wrappers/lua/Lua.hpp
    wrappers/lua/LuaException.cpp
    wrappers/lua/LuaException.hpp
    wrappers/lua/LuaGuard.cpp
    wrappers/lua/LuaGuard.hpp
    wrappers/nostd/source_location.cpp
    wrappers/nostd/source_location.hpp Fwd.hpp wrappers/box2d/Fwd.hpp)

#######################################
# ImGUI

set(IMGUI_HOME "${THIRD_PARTY_DIR}/imgui")
set(IMGUI_SRC
    ${IMGUI_HOME}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_HOME}/backends/imgui_impl_sdl.cpp
    ${IMGUI_HOME}/imgui.cpp
    ${IMGUI_HOME}/imgui_demo.cpp
    ${IMGUI_HOME}/imgui_draw.cpp
    ${IMGUI_HOME}/imgui_tables.cpp
    ${IMGUI_HOME}/imgui_widgets.cpp)

add_library(ImGUI STATIC ${IMGUI_SRC})

#######################################
# GLEW
# Add sources in project

set(GLEW_HOME "thirdparty/glew")
set(GLEW_INCLUDE_DIR "${GLEW_HOME}/include")
set(GLEW_SOURCES "${GLEW_HOME}/src/glew.c")
include_directories(${GLEW_INCLUDE_DIR})
set(SRC ${SRC} ${GLEW_SOURCES})
add_definitions(-DGLEW_STATIC) # needed for executable

#######################################
# Lua
# Add sources in project

add_definitions(-DMAKE_LIB) # Do not compile lua interpreter
set(LUA_HOME "${THIRD_PARTY_DIR}/lua/")
set(SRC ${SRC} "${LUA_HOME}/onelua.c" media/AudioEngine.cpp media/AudioEngine.hpp)
include_directories("${LUA_HOME}")

#######################################

add_executable(SDL_OpenGL ${SRC})

#####################
#######################################
# SDL2
# Set the variable -DSDL2_DIR on windows to the SDL devel directory
# More information on https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/

if(WIN32)
    set(SDL2_DIR "${THIRD_PARTY_DIR}/windows/SDL2-2.0.16")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(SDL_OpenGL ${SDL2_LIBRARIES})

#######################################
# SDL2_image
# You must unzip the SDL2_image zip in the same directory as ${SDL2_DIR}

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_DIRS})
target_link_libraries(SDL_OpenGL ${SDL2_IMAGE_LIBRARIES})

#######################################
# GLM

set(GLM_INCLUDE_DIR "${THIRD_PARTY_DIR}/glm")
include_directories("${GLM_INCLUDE_DIR}")

#######################################
# CPP20Coroutines

include_directories("${THIRD_PARTY_DIR}/CPP20Coroutine/include")

#######################################
# Box2D

# To simplify custom user data for Box2D, directly binding to our class
add_definitions(-DB2_USER_SETTINGS)

set(box2d_HOME "${THIRD_PARTY_DIR}/box2d")
add_subdirectory("${box2d_HOME}" "${PROJECT_BINARY_DIR}/box2d")
include_directories("${box2d_HOME}/include")
target_link_libraries(SDL_OpenGL box2d)

#######################################
# Freetype

add_definitions(-DFT2_BUILD_LIBRARY)
add_subdirectory("${THIRD_PARTY_DIR}/freetype" "${PROJECT_BINARY_DIR}/freetype")
include_directories("${THIRD_PARTY_DIR}/freetype/include")
target_link_libraries(SDL_OpenGL freetype)

#######################################
# FMOD

set(FMOD_HOME "${THIRD_PARTY_DIR}/fmod")

if(WIN32)
    set(FMOD_LIBS "${FMOD_HOME}/lib/x64")
    target_link_directories(SDL_OpenGL PRIVATE ${FMOD_LIBS})
    target_link_libraries(SDL_OpenGL fmodL_vc)
endif()

include_directories("${FMOD_HOME}/inc")

#######################################
# Optimize compilation times

# Precompiled headers (for C++ only to avoid some errors)
target_precompile_headers(SDL_OpenGL PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp>")


# Unity builds
set_target_properties(SDL_OpenGL PROPERTIES UNITY_BUILD ON)
set_target_properties(SDL_OpenGL PROPERTIES UNITY_BUILD_MODE BATCH UNITY_BUILD_BATCH_SIZE 50)

#######################################

find_package(OpenGL REQUIRED)
target_link_libraries(SDL_OpenGL ${OPENGL_LIBRARIES} ImGUI)

if(NOT WIN32)
    target_link_libraries(SDL_OpenGL dl pthread)
endif()

message(STATUS "CXX Compiler is ${CMAKE_CXX_COMPILER}")