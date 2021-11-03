#pragma once

#include "utility/logging.hpp"
#include <fmod.hpp>
#include <filesystem>

namespace Snow::media
{
class AudioEngine
{
    LOGGABLE("AudioEngine")

public:
    AudioEngine();
    ~AudioEngine();

    /// @brief Need to be called once per frame
    void update();

    /// @brief Play the sound from the file.
    /// @remarks The sound is stored and loaded only once for further calls.
    void playSound(const std::filesystem::path &path);

private:
    void FMOD_Check(FMOD_RESULT res);

    FMOD::System *m_system{nullptr};
};
}