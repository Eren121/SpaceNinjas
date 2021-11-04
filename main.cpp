#include "Game.hpp"
#include "test/TestTransformable.hpp"
#include "test/TestTransparent.hpp"
#include "test/TestInput.hpp"
#include <snk/version.hpp>
#include <snk/unused.hpp>
#include <snk/logging.hpp>
#include <snk/version.hpp>
#include <cstdlib>
#include <spdlog/fmt/fmt.h>
#include <entt/entt.hpp>
#include <version>

namespace SpaceNinja
{
    void print_libs_version()
    {
        Logger::pointer logger = Logger::getOrCreate("main");
        constexpr const char* format = "Library '{}': {}";

        {
            // 123,456 = v12.34.56

            snk::version version;
            version.major = FMT_VERSION / 10000;
            version.minor = FMT_VERSION % 10000 / 100;
            version.patch  = FMT_VERSION % 100;
            logger->info(format, "{fmt}", version);
        }
    }

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
            print_libs_version();
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