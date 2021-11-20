#include "Game.hpp"
#include "test/TestTransformable.hpp"
#include "test/TestTransparent.hpp"
#include "test/TestInput.hpp"
#include <version.hpp>
#include <unused.hpp>
#include <logging.hpp>
#include <version.hpp>
#include <entt/entt.hpp>
#include <media/ase/SpriteSheet.hpp>
#include <cstdlib>
#include <version>

namespace sn
{
    void print_libs_version()
    {
        auto& logger = *Logger::getOrCreate("main");

        logger.info("Working directory: {}", fs::current_path());

        constexpr const char* format = "Library '{}': {}";

        {
            // 123,456 = v12.34.56

            snk::version version;
            version.major = FMT_VERSION / 10000;
            version.minor = FMT_VERSION % 10000 / 100;
            version.patch  = FMT_VERSION % 100;
            logger.info(format, "{fmt}", version);
        }
    }

    void unsafe_main()
    {
        print_libs_version();
        SDL::init();
        SDL::init_image();

        {
            test::Test::runTests();
        }
        {
            Game game;
            game.show();
        }
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

    sn::safe_main();

    return EXIT_SUCCESS;
}