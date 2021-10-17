#pragma once

#include "ResourceCache.hpp"
#include <wrappers/gl/Texture.hpp>
#include <filesystem>

namespace Snow::media
{
class TextureCache : public ResourceCache<Texture>
{
public:
    /// @param root The root directory for all textures.
    explicit TextureCache(std::filesystem::path root);

    void loadResource(Texture &texture, const std::string &id) override;

private:
    std::filesystem::path m_root;
};
}