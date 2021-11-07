#include "Living.hpp"
#include "Animation.hpp"
#include "stage/StageWorld.hpp"
#include "Game.hpp"

namespace SpaceNinja
{
    void Living::damage(float amount, DataBody& data)
    {
        hitPoints -= amount;
        if (hitPoints <= 0.0f)
        {
            // Spawns an animation of destruction
            StageWorld& world{data.getWorld()};
            Stage& stage{world.getStage()};
            Game& game{stage.getGame()};

            auto& reg = *data.handle.registry();
            auto animEntity = reg.create();
            auto& anim = reg.emplace<Animation>(animEntity);

            anim.sheet = &game.sheets("explosion-1.json");
            anim.startTime = stage.getWorld().getTime();
            anim.endTime = anim.startTime + anim.sheet->getTotalDuration();

            // Make the explosion as big as the entity
            const float radius = 2.0f;
            anim.sprite.setScale({radius, radius});
            anim.sprite.setOrigin(Sprite::Centered);

            anim.sprite.setPosition(data.getPosition());

            data.handle.destroy();
        }
    }
}