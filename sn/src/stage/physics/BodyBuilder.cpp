#include "stage/physics/BodyBuilder.hpp"
#include "Game.hpp"
#include "stage/StageWorld.hpp"
#include "gameplay/components/Turret.hpp"
#include "gameplay/components/LookAtTarget.hpp"
#include "gameplay/components/Missile.hpp"
#include "gameplay/components/Hostile.hpp"
#include "gameplay/components/Living.hpp"
#include "gameplay/components/Name.hpp"
#include "gameplay/components/Animation.hpp"
#include <b2/b2.hpp>
#include <math.hpp>

namespace sn
{
    DataFixture &addBoxToBody(DataBody &data, const glm::vec2 &pos, float width,
                              const std::string &texPath)
    {
        StageWorld &world = data.getWorld();
        Game &game = world.getGame();

        const Texture *texture = &game.textures(texPath);
        const glm::vec2 textureSize = texture->getSize();
        const float ratio = textureSize.y / textureSize.x;

        Rect rect; // Coordinates of the box relative to the body
        rect.size = {width, width * ratio};
        rect.setOriginFromCenter(pos);

        return addBoxToBody(data, rect, &game.textures(texPath));
    }

    DataFixture &addBoxToBody(DataBody &data, const Rect& rect, const Texture *texture)
    {
        b2Body &body = data.getBody();
        StageWorld &world = data.getWorld();

        b2PolygonShape shape;
        shape.SetAsBox(glVec2(rect.size / 2.0f), b2::fromGLM(rect.getCenter()), 0.0f);

        b2FixtureDef def;
        def.density = 1.0f;
        def.shape = &shape;

        body.CreateFixture(&def);

        DataFixture &fixtureData{getData(*body.GetFixtureList())};

        // Add the drawable rectangle in center of the body with the same size as the rect
        if(texture)
        {
            fixtureData.sprite = std::make_unique<Sprite>();
            fixtureData.sprite->setOrigin(Sprite::Centered); // sprite 0.5 / 0.5 each side
            fixtureData.sprite->setScale(rect.size); // not / 2 because sprite 0.5/0.5 each side
            fixtureData.sprite->setTexture(texture);
        }

        return fixtureData;
    }

    DataBody &createEnemyBody(StageWorld &world, const glm::vec2 &pos)
    {
        // Each enemy has an unique ID, used for the name
        static int nextID = 0;

        b2BodyDef def;
        def.position = b2::fromGLM(pos);
        def.type = b2_dynamicBody;

        DataBody &data = world.createBody(def);
        Stage &stage = data.getWorld().getStage();

        float width = 2.0f;
        width *= 1.0f + math::randf() * 0.5f;
        addBoxToBody(data, {0.0f, 0.0f}, width, "enemy.png");

        data.type = BodyType::Enemy;
        //data.handle.emplace<Turret>();
        data.handle.emplace<Living>();
        data.handle.emplace<Hostile>();
        data.handle.emplace<Name>("Enemy");

        if (stage.hasPlayer())
        {
            const float angularSpeed{0.025f};
            //data.handle.emplace<LookAtTarget>(stage.getPlayer(), angularSpeed);
        }

        return data;
    }

    DataBody &createPlayerBody(StageWorld &world, const glm::vec2 &pos)
    {
        b2BodyDef def;
        def.position = b2::fromGLM(pos);
        def.type = b2_dynamicBody;

        DataBody &data = world.createBody(def);
        Stage &stage = data.getWorld().getStage();

        addBoxToBody(data, {0.0f, 0.0f}, 2.0f, "player.png");
        data.getBody().SetLinearDamping(0.8f);

        data.type = BodyType::Player;
        data.handle.emplace<Living>();
        data.handle.emplace<Name>("Player");

        return data;
    }

    DataBody &createMissileBody(StageWorld &world, const glm::vec2 &pos, const DataBody& thrower)
    {
        b2BodyDef def;
        def.position = b2::fromGLM(pos);
        def.type = b2_dynamicBody;

        DataBody &data = world.createBody(def);
        Stage &stage = data.getWorld().getStage();

        addBoxToBody(data, {0.0f, 0.0f}, 2.0f, "laser.png");

        data.type = BodyType::PlayerMissile;
        data.handle.emplace<Missile>(thrower);
        data.handle.emplace<Name>("Missile");

        for(b2Fixture& fixture : data.getBody())
        {
            fixture.SetSensor(true);
        }

        data.getBody().SetBullet(true);

        return data;
    }
}