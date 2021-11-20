#pragma once

#include <string>
#include <entt/entity/handle.hpp>

namespace sn
{
    struct Name
    {
        Name() : Name("Entity") {}
        Name(const std::string& name) : name(name + std::to_string(nextID++)) {}

        std::string name;

        /// @brief Get the name from the Name component of an entity.
        /// @remarks If the entity does not have a Name component, it is automatically created in this function.
        static const std::string& get(entt::handle& handle)
        {
            return handle.get_or_emplace<Name>().name;
        }

    private:
        static int nextID;
    };
}