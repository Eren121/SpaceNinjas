#include "media/TextureCache.hpp"
#include <utility>

namespace Snow::media
{

TextureCache::TextureCache(std::filesystem::path root)
        : m_root(std::move(root))
{
}

void TextureCache::loadResource(Texture &texture, const std::string &id)
{
    auto path = m_root / id;
    texture.load(path);
    texture.setFilter(Texture::Nearest);
}

}