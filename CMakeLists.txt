cmake_minimum_required(VERSION 3.17)
project(SDL_OpenGL)

set(CMAKE_CXX_STANDARD 20)

### Compiler dependant support ########

# Coroutines
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
elseif(MSVC)
    # using MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /await:strict")
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
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
    thirdparty/imgui/backends/imgui_impl_sdl.cpp

    main.cpp
    wrappers/SDL.cpp
    wrappers/SDL.hpp
    wrappers/nostd/source_location.hpp
    wrappers/nostd/source_location.cpp
    wrappers/gl/Shader.cpp
    wrappers/gl/Shader.hpp

    media/Window.cpp
    media/Window.hpp
    media/ResourceCache.hpp

    utility/Exception.cpp
    utility/Exception.hpp
    utility/Str.cpp
    utility/Str.hpp Game.cpp Game.hpp utility/IO.cpp utility/IO.hpp wrappers/gl/GL.cpp wrappers/gl/GL.hpp wrappers/gl/Circle.cpp wrappers/gl/Circle.hpp
    utility/math.cpp
    utility/math.hpp wrappers/gl/Sprite.cpp wrappers/gl/Sprite.hpp wrappers/gl/VertexArray.cpp wrappers/gl/VertexArray.hpp utility/Rect.hpp wrappers/gl/Texture.cpp wrappers/gl/Texture.hpp wrappers/gl/RenderStates.hpp wrappers/gl/Line.cpp wrappers/gl/Line.hpp process/Process.cpp process/Process.hpp utility/Guard.hpp

    utility/binpacking/GuillotineBinPack.cpp
    utility/binpacking/GuillotineBinPack.h
    utility/binpacking/MaxRectsBinPack.cpp
    utility/binpacking/MaxRectsBinPack.h
    utility/binpacking/Rect.h
    utility/binpacking/Rect.cpp

    process/Action.cpp
    process/Action.hpp
    process/Process.cpp
    process/Process.hpp
    process/ProcessPool.cpp
    process/ProcessPool.hpp
    process/ProcessQueue.cpp
    process/ProcessQueue.hpp
    process/Wait.cpp
    process/Wait.hpp
    process/CoProcess.cpp
    process/CoProcess.hpp

    utility/time/Clock.cpp
    utility/time/Clock.hpp
    utility/time/Time.cpp
    utility/time/Time.hpp
    utility/list_iterator.hpp

    wrappers/box2d/box2d.hpp
    wrappers/box2d/World.cpp
    wrappers/box2d/World.hpp
    wrappers/box2d/DebugDraw.cpp
    wrappers/box2d/DebugDraw.hpp gameplay/Stage.cpp gameplay/Stage.hpp gameplay/lua/LuaAPI.cpp gameplay/lua/LuaAPI.hpp wrappers/lua/Coroutine.cpp wrappers/lua/Coroutine.hpp wrappers/lua/LuaException.cpp wrappers/lua/LuaException.hpp gameplay/StageWorld.cpp gameplay/StageWorld.hpp wrappers/gl/Drawable.hpp gameplay/Body.cpp gameplay/Body.hpp TextureCache.cpp TextureCache.hpp ViewControl.cpp ViewControl.hpp gameplay/PlayerControl.cpp gameplay/PlayerControl.hpp process/ProcessStack.cpp process/ProcessStack.hpp

        wrappers/gl/Transformable.cpp
    wrappers/gl/Transformable.hpp

    wrappers/freetype/Font.cpp
    wrappers/freetype/Font.hpp

    wrappers/freetype/Text.cpp wrappers/freetype/Text.hpp gameplay/UIStage.cpp gameplay/UIStage.hpp utility/time/FPSCounter.cpp utility/time/FPSCounter.hpp gameplay/CollisionManager.cpp gameplay/CollisionManager.hpp gameplay/PlayerShoot.cpp gameplay/PlayerShoot.hpp media/input/UnifiedInput.cpp media/input/UnifiedInput.hpp gameplay/PlayerProcess.cpp gameplay/PlayerProcess.hpp wrappers/freetype/FTException.cpp wrappers/freetype/FTException.hpp wrappers/freetype/Glyph.hpp test/TestTransformable.hpp test/TestTransformable.cpp wrappers/gl/Shape.cpp wrappers/gl/Shape.hpp wrappers/gl/Vertex.hpp wrappers/gl/Vertex.cpp utility/offset_of.hpp wrappers/gl/ConvexShape.cpp wrappers/gl/ConvexShape.hpp gameplay/VictoryMenu.cpp gameplay/VictoryMenu.hpp gameplay/Victory.hpp utility/macro/unused.hpp wrappers/lua/LuaGuard.cpp wrappers/lua/LuaGuard.hpp wrappers/lua/Lua.hpp wrappers/freetype/private/DynamicRectPacker.cpp wrappers/freetype/private/DynamicRectPacker.hpp wrappers/freetype/private/FontPacker.cpp wrappers/freetype/private/FontPacker.hpp utility/LazyResource.hpp test/TestTransparent.cpp test/TestTransparent.hpp utility/swapmove.hpp utility/macro/pp.hpp wrappers/freetype/private/raii/Library.cpp wrappers/freetype/private/raii/Library.hpp wrappers/freetype/private/Check.hpp wrappers/freetype/private/raii/Face.cpp wrappers/freetype/private/raii/Face.hpp wrappers/freetype/private/raii/Stroker.cpp wrappers/freetype/private/raii/Stroker.hpp wrappers/freetype/FontType.hpp wrappers/freetype/private/Face.cpp wrappers/freetype/private/Face.hpp wrappers/freetype/private/FT.cpp wrappers/freetype/private/FT.hpp wrappers/freetype/FontType.cpp wrappers/DebugWindow.cpp wrappers/DebugWindow.hpp ui/VerticalListMenu.cpp ui/VerticalListMenu.hpp ui/Input.cpp ui/Input.hpp media/input/Axis.cpp media/input/Axis.hpp test/TestInput.cpp test/TestInput.hpp test/Test.cpp test/Test.hpp media/input/InputListener.cpp media/input/InputListener.hpp media/input/BoolAction.cpp media/input/BoolAction.hpp media/input/Action.cpp media/input/Action.hpp media/input/Key.cpp media/input/Key.hpp media/input/AxisAction.cpp media/input/AxisAction.hpp media/input/VectorAction.cpp media/input/VectorAction.hpp GameControls.hpp GameControls.cpp media/scene/SceneNode.cpp media/scene/SceneNode.hpp utility/algorithm.hpp gameplay/lua/LuaStageCoroutine.cpp gameplay/lua/LuaStageCoroutine.hpp gameplay/lua/LuaEngine.cpp gameplay/lua/LuaEngine.hpp utility/time/Timer.cpp utility/time/Timer.hpp media/scene/SceneStack.cpp media/scene/SceneStack.hpp ui/MenuStage.cpp ui/MenuStage.hpp utility/GridPosition.cpp utility/GridPosition.hpp WindowDebugInfo.cpp WindowDebugInfo.hpp gameplay/Save.hpp utility/logging.cpp utility/string_litteral.hpp gameplay/Save.cpp)


add_executable(SDL_OpenGL ${SRC})

target_link_libraries(SDL_OpenGL GL SDL2 SDL2_image GLEW box2d lua dl freetype spdlog pthread)

message(STATUS "CXX Compiler is ${CMAKE_CXX_COMPILER}")