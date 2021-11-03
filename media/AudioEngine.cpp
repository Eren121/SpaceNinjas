#include "AudioEngine.hpp"
#include <snk/str.hpp>
#include <fmod_errors.h>
#include <stdexcept>

void Snow::media::AudioEngine::FMOD_Check(FMOD_RESULT res)
{
    if(res != FMOD_OK)
    {
        getLogger().error("FMOD error: {}", FMOD_ErrorString(res));
    }
}

Snow::media::AudioEngine::AudioEngine()
{
    FMOD_Check(FMOD::System_Create(&m_system));
    FMOD_Check(m_system->init(32, FMOD_INIT_NORMAL, nullptr));
}

Snow::media::AudioEngine::~AudioEngine()
{
    if(m_system)
    {
        FMOD_Check(m_system->close());
        FMOD_Check(m_system->release());
        m_system = nullptr;
    }
}

void Snow::media::AudioEngine::update()
{
    FMOD_Check(m_system->update());
}

void Snow::media::AudioEngine::playSound(const std::filesystem::path &path)
{
    if(!std::filesystem::exists(path)) throw std::runtime_error{snk::str{} << path << " does not exist"};

    const std::string name = path.string();

    getLogger().info("Playing sound {}", path);

    FMOD_MODE mode = FMOD_DEFAULT;

    FMOD::Sound *sound{nullptr};
    FMOD_Check(m_system->createSound(name.c_str(), mode, nullptr, &sound));
    FMOD_Check(sound->setMode(FMOD_LOOP_OFF));
    FMOD_Check(m_system->playSound(sound, nullptr, false, nullptr));
}
