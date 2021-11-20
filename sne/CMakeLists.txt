cmake_minimum_required(VERSION 3.17)
project(sne LANGUAGES C CXX)

# SpaceNinja Engine

set(SRC
    include/b2/b2.hpp
    include/b2/DebugDraw.hpp
        include/b2/iterate_body.hpp
    include/b2/user_settings/b2_user_settings.h
    include/b2/World.hpp
    include/ft/Font.hpp
    include/ft/FontType.hpp
    include/ft/FTException.hpp
    include/ft/Glyph.hpp
    include/ft/private/Check.hpp
    include/ft/private/DynamicRectPacker.hpp
    include/ft/private/Face.hpp
    include/ft/private/FontPacker.hpp
    include/ft/private/FT.hpp
    include/ft/private/raii/Face.hpp
    include/ft/private/raii/Library.hpp
    include/ft/private/raii/Stroker.hpp
    include/ft/Text.hpp
    include/gl/Circle.hpp
    include/gl/ConvexShape.hpp
    include/gl/Drawable.hpp
    include/gl/GL.hpp
    include/gl/Line.hpp
        include/gl/RenderStates.hpp
    include/gl/Shader.hpp
    include/gl/Shape.hpp
    include/gl/Sprite.hpp
    include/gl/Texture.hpp
    include/gl/Transformable.hpp
    include/gl/Vertex.hpp
    include/gl/VertexArray.hpp
        include/im/ImGUI.hpp
        include/im/ImBase.hpp
        include/im/ImBaseFormatFloat.hpp
        include/im/ImBaseRange.hpp
        include/im/ImBool.hpp
        include/im/ImFloat.hpp
        include/im/ImFloat2.hpp
        include/im/ImInt.hpp
        include/im/ImTime.hpp
    include/lua/Coroutine.hpp
    include/lua/Lua.hpp
    include/lua/LuaException.hpp
    include/lua/LuaGuard.hpp
    include/media/ase/Frame.hpp
    include/media/ase/SheetCache.hpp
    include/media/ase/SpriteSheet.hpp
    include/media/AudioEngine.hpp
    include/media/DebugWindow.hpp
    include/media/input/Action.hpp
    include/media/input/Axis.hpp
    include/media/input/AxisAction.hpp
    include/media/input/BoolAction.hpp
    include/media/input/InputListener.hpp
    include/media/input/Key.hpp
    include/media/input/UnifiedInput.hpp
    include/media/input/VectorAction.hpp
    include/media/ResourceCache.hpp
    include/media/scene/SceneNode.hpp
    include/media/scene/SceneStack.hpp
    include/media/SDL.hpp
    include/media/TextureCache.hpp
    include/media/Window.hpp
    include/media/WindowDebugInfo.hpp
    include/process/Action.hpp
    include/process/CoProcess.hpp
    include/process/Process.hpp
    include/process/ProcessPool.hpp
    include/process/ProcessQueue.hpp
    include/process/ProcessStack.hpp
    include/process/Wait.hpp
    src/b2/DebugDraw.cpp
    src/b2/user_settings/b2_user_settings.h
    src/b2/World.cpp
    src/ft/Font.cpp
    src/ft/FontType.cpp
    src/ft/FTException.cpp
    src/ft/private/DynamicRectPacker.cpp
    src/ft/private/Face.cpp
    src/ft/private/FontPacker.cpp
    src/ft/private/FT.cpp
    src/ft/private/raii/Face.cpp
    src/ft/private/raii/Library.cpp
    src/ft/private/raii/Stroker.cpp
    src/ft/Text.cpp
    src/gl/Circle.cpp
        ../vendor/glew/src/glew.c
        ../vendor/glew/include/GL/eglew.h
        ../vendor/glew/include/GL/glew.h
        ../vendor/glew/include/GL/wglew.h
        ../vendor/glew/include/GL/glxew.h
    src/gl/ConvexShape.cpp
    src/gl/GL.cpp
    src/gl/Line.cpp
    src/gl/Shader.cpp
    src/gl/Shape.cpp
    src/gl/Sprite.cpp
    src/gl/Texture.cpp
    src/gl/Transformable.cpp
    src/gl/Vertex.cpp
    src/gl/VertexArray.cpp
    src/im/ImBool.cpp
    src/im/ImFloat.cpp
    src/im/ImFloat2.cpp
    src/im/ImInt.cpp
    src/im/ImTime.cpp
    src/lua/Coroutine.cpp
    src/lua/LuaException.cpp
    src/lua/LuaGuard.cpp
    src/media/ase/SheetCache.cpp
    src/media/ase/SpriteSheet.cpp
    src/media/AudioEngine.cpp
    src/media/DebugWindow.cpp
    src/media/input/Action.cpp
    src/media/input/Axis.cpp
    src/media/input/AxisAction.cpp
    src/media/input/BoolAction.cpp
    src/media/input/InputListener.cpp
    src/media/input/Key.cpp
    src/media/input/UnifiedInput.cpp
    src/media/input/VectorAction.cpp
    src/media/scene/SceneNode.cpp
    src/media/scene/SceneStack.cpp
    src/media/SDL.cpp
    src/media/TextureCache.cpp
    src/media/Window.cpp
    src/media/WindowDebugInfo.cpp
    src/process/Action.cpp
    src/process/CoProcess.cpp
    src/process/Process.cpp
    src/process/ProcessPool.cpp
    src/process/ProcessQueue.cpp
    src/process/ProcessStack.cpp
    src/process/Wait.cpp)

add_library(sne ${SRC})
target_include_directories(sne PUBLIC include)
add_library(sne::sne ALIAS sne)

# ========================================================================= OpenGL

find_package(OpenGL REQUIRED)
target_link_libraries(sne PUBLIC OpenGL::GL)

#
target_link_libraries(sne PUBLIC
    snk
    ImGUI
    Lua
    box2d
    freetype
    FMOD)

# ========================================================================= SDL2


find_package(SDL REQUIRED) # Use custom FindXXX.cmake

target_link_libraries(sne PUBLIC ${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS}) # Include globally because ImGUI needs it (for sub-projects)
target_include_directories(sne PUBLIC ${SDL2_INCLUDE_DIRS}) # But also for the library for sn (a higher-level project)
# target_link_libraries(sne PUBLIC SDL2_image)


# ========================================================================= Box2D

# Box2D user config
# Do not use target_include_directories because Box2D subproject need to be aware of this include
include_directories(include/b2/user_settings)
target_include_directories(sne PUBLIC include/b2/user_settings)

# To simplify custom user data for Box2D, directly binding to our class
add_definitions(-DB2_USER_SETTINGS)
target_compile_definitions(sne PUBLIC B2_USER_SETTINGS)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)

FetchContent_Declare(Box2D
    GIT_REPOSITORY
    https://github.com/erincatto/box2d.git
    GIT_TAG 9ebbbcd960ad424e03e5de6e66a40764c16f51bc) # v2.4.1
FetchContent_MakeAvailable(Box2D)
target_link_libraries(sne PUBLIC box2d)

# ========================================================================= Sigslot

FetchContent_Declare(Sigslot
        GIT_REPOSITORY
        https://github.com/palacaze/sigslot.git
        GIT_TAG e41c6e62edfa67b6402b17a627dca698a14f47a8) # v1.2.1
FetchContent_MakeAvailable(Sigslot)
target_link_libraries(sne PUBLIC Pal::Sigslot)

# ========================================================================= CPP20Coroutines

FetchContent_Declare(CPP20Coroutines
        GIT_REPOSITORY
        https://github.com/Eren121/CPP20Coroutines.git
        GIT_TAG 44c76f91fed21cb702ef0867b4e799f274bc5455)
FetchContent_MakeAvailable(CPP20Coroutines)
target_link_libraries(sne PUBLIC CPP20Coroutines)

# ========================================================================= Freetype

FetchContent_Declare(Freetype
        GIT_REPOSITORY
        https://github.com/freetype/freetype.git
        GIT_TAG 801cd842e27c85cb1d5000f6397f382ffe295daa) # v2.11.0
FetchContent_MakeAvailable(Freetype)
target_link_libraries(sne PUBLIC freetype)

# ========================================================================= ImGUI

find_package(ImGUI REQUIRED) # Use custom FindXXX.cmake
target_link_libraries(sne PUBLIC ImGUI)

# ========================================================================= Lua

find_package(Lua REQUIRED) # Use custom FindXXX.cmake
target_link_libraries(sne PUBLIC Lua)

# ========================================================================= sol2

add_definitions(-DSOL_ALL_SAFETIES_ON=1)

FetchContent_Declare(sol2
        GIT_REPOSITORY
        https://github.com/ThePhD/sol2.git
        GIT_TAG 288f4f4e73f379cca56f8eb7840a449adac0acb9) # v3.2.2
FetchContent_MakeAvailable(sol2)
target_link_libraries(sne PUBLIC sol2::sol2)

# ========================================================================= FMOD

find_package(FMOD REQUIRED) # Use custom FindXXX.cmake
target_link_libraries(sne PUBLIC FMOD)

# ========================================================================= GLEW

find_package(GLEW REQUIRED) # Use custom FindXXX.cmake
target_link_libraries(sne PUBLIC GLEW)