#pragma once

#include <SDL.h>
#include <sigslot/signal.hpp>
#include "UnifiedInput.hpp"

/// @brief Represents a concrete action in the game: Jump, Shoot, etc...
/// @remarks Inherit observer because children class will use it
class Action : public sigslot::observer
{
public:
    /// @brief Range for analog inputs. Buttons can only be either 0 or 1.
    enum {
        Min = -32768,
        Max = 32767
    };
    
    ~Action() override = default;

    /// @returns true if the action is currently pressed
    bool isPressed() const;
    
    bool isJustPressed() const;
    bool isJustReleased() const;
    
    /// @returns A value, depending of the type:
    ///     For bool, returns 0 or 1
    ///     For axis, returns in [-1;1]
    ///     For triggers, returns in [0;1].
    float asFloat() const;
    
    /// @remarks You should set an input before binding any value
    void setInput(UnifiedInput *input);
    
protected:
    void update(int newValue, long frame);
    
    /// @name
    /// @brief Values for the current frame and the previous frame, should be set by the child class.
    /// @remarks All values should be in range [Min;Max]
    /// @remarks Zero represent the default value.
    /// @remarks If the action is a boolean, the dead zone should be zero.
    /// @remarks If the action is an axis, its value can't be negative.
    /// @remarks The dead zone make all value with a certain distance (either positive or negative) from zero show as zero.
    /// @{
private:
    int m_currentValue{0};
    int m_previousValue{0};
    long m_lastFrameUpdate{0};
    
protected:
    int m_deadZone{0};
    UnifiedInput *m_input{nullptr};
    /// @}
    
    static int applyDeadZone(int value, float deadZone);
    static float asFloat(int intValue);
    static int asInt(float floatValue);
};

