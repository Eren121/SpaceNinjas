#pragma once

#include "SpriteSheet.hpp"
#include "media/ResourceCache.hpp"

namespace Snow
{
    class SheetCache : public media::ResourceCache<SpriteSheet>
    {
    public:
        explicit SheetCache(fs::path root);

        void loadResource(SpriteSheet &resource, const std::string &id) override;

    private:
        fs::path m_root;
    };
}