#include "TestBox2D.hpp"

namespace SpaceNinja::test
{
    TestBox2D::TestBox2D()
    {
        m_world.SetDebugDraw(&m_debugDraw);

        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position = {0.0f, 0.0f};

        b2Body& body = m_world.createBody(def);

        b2CircleShape shape;
        shape.m_p = {5.0f, 0.0f};
        shape.m_radius = 2.0f;

        b2Filter filter;
        filter.groupIndex = 1;

        b2FixtureDef fDef;
        fDef.shape = &shape;
        fDef.density = 1.0f;
        //body.CreateFixture(&fDef)->SetFilterData(filter);

        //body.SetLinearVelocity({-0.1, 0});

        b1 = &body;


        {
            b2BodyDef def;
            def.type = b2_dynamicBody;
            def.position = {-1.5f, 0.0f};

            b2Body& body = m_world.createBody(def);

            b2CircleShape shape;
            shape.m_p = {0.0f, 0.0f};
            shape.m_radius = 2.0f;

            b2FixtureDef fDef;
            fDef.shape = &shape;
            fDef.density = 1.0f;
            body.CreateFixture(&fDef)->SetFilterData(filter);
        }
    }

    void TestBox2D::draw()
    {
        RenderStates states;
        states.shader = &m_shader;

        const float radius{10.0f};
        states.view =  glm::ortho(-radius, radius, -radius, radius);

        getLogger().info("awak={}, enabled={}", b1->IsAwake(), b1->IsEnabled());

        m_debugDraw.states = states;

        m_world.step();
        m_world.DebugDraw();
    }
}