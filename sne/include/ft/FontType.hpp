#pragma once

#include <memory>
#include <filesystem>

namespace FT
{
    namespace RAII
    {
        class Library;
    }

    class Face;
}

struct FontType
{
    int characterSize{32};
    bool outline{false};

    /// @remarks Permit to use the class as map key.
    friend auto operator<=>(const FontType& lhs, const FontType& rhs) = default;

    /// @brief Face factory from type.
    std::unique_ptr<FT::Face> makeFace(const FT::RAII::Library& library, std::filesystem::path path);
};