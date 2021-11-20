#include "media/ase/SheetCache.hpp"

namespace Snow
{
    SheetCache::SheetCache(fs::path root)
            : m_root{root}
    {
    }

    void SheetCache::loadResource(SpriteSheet &resource, const std::string &id)
    {
        resource = SpriteSheet{m_root / id};
    }
}