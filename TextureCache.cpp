#include "TextureCache.hpp"
#include <utility>

TextureCache::TextureCache(std::filesystem::path root)
    : ResourceCache(static_cast<ResourceLoader&>(*this)),
      m_root(std::move(root))
{
}

void TextureCache::loadResource(Texture& texture, const std::string& id)
{
    auto path = m_root / id;
    texture.load(path);
}
