#include "Axis.hpp"
#include <map>

namespace Snow::in
{

// Map each char to a weight
const auto &getWeightMap()
{
    static const std::map<SDL_Scancode Axis::*, glm::ivec2> weights{};
    ///TODO
    return weights;
}

glm::ivec2 Axis::poll() const
{
    glm::ivec2 ret{0};

    // Get the keyboard from a one allocated by SDL (this does not allocate memory)
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);

    for (const auto&[field, vec] : getWeightMap()) {
        SDL_Scancode scancode = this->*field;
        SDL_Keycode code = SDL_GetKeyFromScancode(scancode);

        if (code != SDLK_UNKNOWN) {
            if (keyboard[code]) {
                ret += vec;
            }
        }
    }

    return ret;
}

glm::ivec2 Axis::poll(const SDL_Event &event) const
{
    glm::ivec2 ret{0};

    if (event.type == SDL_KEYDOWN) {
        SDL_Scancode scancode = event.key.keysym.scancode;

        for (const auto&[field, vec] : getWeightMap()) {
            if (this->*field == scancode) {
                ret = vec;
                break;
            }
        }
    }

    return ret;
}

}