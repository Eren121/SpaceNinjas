#include "gameplay/components/Living.hpp"
#include "gameplay/components/Animation.hpp"
#include "stage/StageWorld.hpp"
#include "Game.hpp"

namespace sn
{
    void Living::damage(float amount, DataBody& data)
    {
        if(data.handle.all_of<Invincible>())
        {
            return;
        }

        // Stop if there is no damage output
        if(amount <= 0)
        {
            return;
        }

        // Apply armor
        amount -= armor;

        // Always 1 damage minimum
        if(amount <= 0.0f)
        {
            amount = 1.0f;
        }

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

            data.destroy();
        }
    }
}