#ifndef SOUNDFX_H_
#define SOUNDFX_H_

#include <string>

#include "irrklang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include <iostream>

class SoundFXController
{
  public:

    void playClip(std::string soundFile)
    {
        if (m_engine != NULL)
            m_engine->play2D(soundFile.c_str(), false);
    }

    void abortClip()
    {
        if (m_engine != NULL)
            m_engine->stopAllSounds();
    }

    static SoundFXController& getInstance();

  private:
    SoundFXController()
    {
        m_engine = irrklang::createIrrKlangDevice();
        if (m_engine == NULL)
            std::cout << "Cannot create sound engine!  Game will be silent."
                      << std::endl;
    }

    ~SoundFXController()
    {
        if (m_engine != NULL)
            m_engine->drop();
    }

    SoundFXController(const SoundFXController&);
    SoundFXController& operator=(const SoundFXController&);

    irrklang::ISoundEngine* m_engine;
};

  // Meyers singleton pattern
inline SoundFXController& SoundFXController::getInstance()
{
    static SoundFXController instance;
    return instance;
}

inline SoundFXController& SoundFX()
{
    return SoundFXController::getInstance();
}

#endif // SOUNDFX_H_
