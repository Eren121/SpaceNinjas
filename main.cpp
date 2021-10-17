#include "Game.hpp"
#include "test/TestTransformable.hpp"
#include "test/TestTransparent.hpp"
#include "test/TestInput.hpp"
#include "utility/macro/unused.hpp"
#include "utility/logging.hpp"
#include <cstdlib>

namespace SpaceNinja
{

void unsafe_main()
{
    SDL::init();
    SDL::init_image();

//    TestTransformable().run();
//    TestTransparent().run();
//    TestInput().run();

    Game game;
    game.show();
}

/// @brief Main where all exceptions are caught
/// @details Better to never use abort(), but instead throwing errors to ensure cleanup of SDL.
void safe_main()
{
    Logger::pointer logger = Logger::getOrCreate("main");

    try
    {
        unsafe_main();
    }
    catch(const std::exception& e)
    {
        logger->error("Fatal error: {}", e.what());
    }
    catch(...)
    {
        logger->error("Fatal unkown error, aborting.");
    }
}

}

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    SpaceNinja::safe_main();

    return EXIT_SUCCESS;
}
