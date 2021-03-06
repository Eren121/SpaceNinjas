#pragma once

#include "iterate_body.hpp"
#include <math.hpp>
#include <box2d/box2d.h>
#include <glm/glm.hpp>

/// @brief box2d snk functions
namespace b2
{
    namespace
    {
        /// @brief Conversion between different libraries types.
        /// @{
        inline glm::vec2 toGLM(const b2Vec2 &v) {
            return {v.x, v.y};
        }

        glm::vec4 toGLM(const b2Color &c) {
            return {c.r, c.g, c.b, c.a};
        }

        b2Vec2 fromGLM(const glm::vec2 &v) {
            return {v.x, v.y};
        }

        /// @}

        /// @name
        /// @brief Box2D provide only SetTransform for both position and angle, but often we need to change only one
        /// @details Also useful for Lua wrappers.
        /// @{

        void setPosition(b2Body &body, const glm::vec2 &newPos) { body.SetTransform(fromGLM(newPos), body.GetAngle()); }
        glm::vec2 getPosition(const b2Body &body) { return toGLM(body.GetPosition()); }

        void setAngle(b2Body &body, float newAngle) { body.SetTransform(body.GetPosition(), newAngle); }
        float getAngle(const b2Body& body) { return body.GetAngle(); }

        void setVelocity(b2Body& body, const glm::vec2& vel) { body.SetLinearVelocity(fromGLM(vel)); }
        glm::vec2 getVelocity(const b2Body& body) { return toGLM(body.GetLinearVelocity()); }

        glm::vec2 getDirection(const b2Body& body) { return math::angle2vec(getAngle(body)); }

        /// @brief Set the velocity and update the angle to the direction.
        void setVelocityWithAngle(b2Body& body, const glm::vec2& vel)
        {
            setVelocity(body, vel);
            setAngle(body, math::vec2angle(vel));
        }

        /// @brief Set the angle of a body to look at a target
        void lookAt(b2Body& body, b2Body& target)
        {
            setAngle(body, math::vec2angle(getPosition(target) - getPosition(body)));
        }
    }
}