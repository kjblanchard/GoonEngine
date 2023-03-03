#pragma once
#include <sound/sound.h>

namespace goon
{
    struct BgmComponent
    {
        BgmComponent(std::string soundFile, float loopBegin = 0.0f, float loopEnd = 0.0f)
            : SoundFile(soundFile), LoopBegin(loopBegin), LoopEnd(loopEnd)
        {
        }

        std::string SoundFile;
        float LoopBegin;
        float LoopEnd;
        Bgm* LoadedBgm = nullptr;
    };
}