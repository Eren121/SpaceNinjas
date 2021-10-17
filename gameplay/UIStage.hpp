#pragma once

#include "media/scene/SceneNode.hpp"
#include <wrappers/freetype/Text.hpp>
#include <utility/time/Clock.hpp>
#include <utility/time/FPSCounter.hpp>
#include "Fwd.hpp"

/// @remarks Inherits sigslot::observer to ensure all connections are destroyed when the object is destroyed.
class UIStage : public SceneNode, public sigslot::observer
{
public:
    explicit UIStage(SpaceNinja::Game& game, Stage& stage);
    
protected:
    bool updateNode() override;
    void drawNode(RenderStates states) const override;
    void debugNode() override;
    
private:
    SpaceNinja::Game& m_game;
    Stage& m_stage;

    /// @brief Store the total count of frames drew, incremented at each frame.
    /// @remarks It is updated before rendering, so if this number is shown to the screen, the printed number
    /// will include the currently frame shown to the user.
    int m_frame{0};
    
    /// @brief Store the average world tick per seconds.
    /// @remarks Equals to the window fps if there is one world step per frame.
    /// @remarks This is a temporary variable updated each given time.
    int m_fps{0};
    
    /// @brief Interval between two fps update
    /// @details Default is 400ms.
    Time m_fpsUpdateInterval;
    
    /// @brief Measure the elapsed time since the first frame
    Clock m_sinceFirstFrame;

    FPSCounter m_fpsCounter;
};