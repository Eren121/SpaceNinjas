cmake_minimum_required(VERSION 3.17)
project(sn LANGUAGES CXX)

# SpaceNinja Game

set(SRC
    include/Fwd.hpp
    include/Game.hpp
    include/GameControls.hpp
    include/gameplay/components/Animation.hpp
    include/gameplay/components/CanAttack.hpp
    include/gameplay/components/Hostile.hpp
    include/gameplay/components/Living.hpp
    include/gameplay/components/LookAtTarget.hpp
    include/gameplay/components/Missile.hpp
    include/gameplay/components/Name.hpp
    include/gameplay/components/Turret.hpp
    include/stage/BodyType.hpp
    include/stage/CollisionManager.hpp
    include/stage/DataBody.hpp
    include/stage/DataFixture.hpp
    include/stage/lua/LuaAPI.hpp
    include/stage/lua/LuaEngine.hpp
    include/stage/lua/Thread.hpp
    include/stage/physics/BodyBuilder.hpp
    include/stage/player/PlayerControl.hpp
    include/stage/player/PlayerSceneNode.hpp
    include/stage/player/PlayerShoot.hpp
    include/stage/Save.hpp
    include/stage/Stage.hpp
    include/stage/StageSceneNode.hpp
    include/stage/StageWorld.hpp
    include/stage/Victory.hpp
    include/stage/VictoryMenu.hpp
    include/test/Test.hpp
    include/test/TestBox2D.hpp
    include/test/TestInput.hpp
    include/test/TestSpriteSheet.hpp
    include/test/TestTransformable.hpp
    include/test/TestTransparent.hpp
    include/ui/Input.hpp
    include/ui/MenuStage.hpp
    include/ui/StoryMessage.hpp
    include/ui/VerticalListMenu.hpp
    src/Game.cpp
    src/GameControls.cpp
    src/gameplay/components/CanAttack.cpp
    src/gameplay/components/Living.cpp
    src/gameplay/components/Name.cpp
    src/gameplay/components/Turret.cpp
    src/main.cpp
    src/stage/CollisionManager.cpp
    src/stage/DataBody.cpp
    src/stage/DataFixture.cpp
    src/stage/lua/LuaAPI.cpp
    src/stage/lua/LuaEngine.cpp
    src/stage/lua/Thread.cpp
    src/stage/physics/BodyBuilder.cpp
    src/stage/player/PlayerControl.cpp
    src/stage/player/PlayerSceneNode.cpp
    src/stage/player/PlayerShoot.cpp
    src/stage/Save.cpp
    src/stage/Stage.cpp
    src/stage/StageSceneNode.cpp
    src/stage/StageWorld.cpp
    src/stage/VictoryMenu.cpp
    src/test/Test.cpp
    src/test/TestBox2D.cpp
    src/test/TestInput.cpp
    src/test/TestSpriteSheet.cpp
    src/test/TestTransformable.cpp
    src/test/TestTransparent.cpp
    src/ui/Input.cpp
    src/ui/MenuStage.cpp
    src/ui/StoryMessage.cpp
    src/ui/VerticalListMenu.cpp)

add_executable(sn ${SRC})
target_include_directories(sn PUBLIC include)
target_link_libraries(sn PUBLIC snk::snk sne::sne)

# ========================================================================= EnTT

FetchContent_Declare(EnTT
        GIT_REPOSITORY
        https://github.com/skypjack/entt.git
        GIT_TAG dd6863f71da1b360ec09c25912617a3423f08149) # v3.8.1
FetchContent_MakeAvailable(EnTT)
target_link_libraries(sn PUBLIC EnTT)

# ========================================================================= cereal

set(SKIP_PERFORMANCE_COMPARISON ON)

FetchContent_Declare(cereal
        GIT_REPOSITORY
        https://github.com/USCiLab/cereal.git
        GIT_TAG e735cc8bc13cf98139e803dc2b5b6e4a712aa7aa) # v1.3.0
FetchContent_MakeAvailable(cereal)
target_link_libraries(sn PUBLIC cereal::cereal)

# =========================================================================

if(NOT WIN32)
    target_link_libraries(sn PUBLIC dl pthread)
endif()