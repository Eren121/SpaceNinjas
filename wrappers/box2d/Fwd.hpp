#pragma once

// Forward declare some box 2D types
// Useful in b2_user_settings.h where you don't want to include your Body class because this would make
// a recursive include

class b2World;
class b2Body;
class b2BodyDef;

namespace b2
{
    class World;
}