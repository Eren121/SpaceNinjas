#include "ft/FontType.hpp"
#include "ft/private/Face.hpp"
#include "ft/private/raii/Library.hpp"

std::unique_ptr<FT::Face> FontType::makeFace(const FT::RAII::Library& library, std::filesystem::path path)
{
    if(outline)
    {
        return std::make_unique<FT::FaceOutline>(library, path, characterSize);
    }
    else
    {
        return std::make_unique<FT::FaceSolid>(library, path, characterSize);
    }
}
