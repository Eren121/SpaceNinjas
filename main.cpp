#include "media/Window.hpp"
#include "Game.hpp"
#include <iostream>
#include "test/TestTransformable.hpp"
#include "test/TestTransparent.hpp"
#include "test/TestInput.hpp"
#include "utility/macro/unused.hpp"
#include <spdlog/sinks/stdout_sinks.h>

using std::cerr;
using std::cout;
using std::endl;

void register_loggers()
{
    static auto luaAPi{spdlog::stdout_logger_st("LuaAPI")};
}

/// @brief Main where all exceptions are caught
/// @details Better to never use abort(), but instead throwing errors to ensure cleanup of SDL.
void safe_main()
{
    register_loggers();
    
    SDL::init();
    SDL::init_image();

//    TestTransformable().run();
//    TestTransparent().run();
//    TestInput().run();
    
    Game game;
    game.show();
}

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    // Lazyfoo says with SDL, main should have argc and argv arguments to be cross-plateform:
    // https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php

    try
    {
        safe_main();
    }
    catch(const std::exception& e)
    {
        cerr << "Fatal error:" << endl;
        cerr << e.what() << endl;
    }
    catch(...)
    {
        cerr << "Fatal unkown error, aborting." << endl;
    }

    return 0;
}
