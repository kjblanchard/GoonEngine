#include <Goon/systems/SoundSystem.hpp>
#include <sound/sound.h>

void UpdateSoundBro()
{
    UpdateSound();
}

void SSPlayBgm(Bgm *bgm, float volume)
{
    if (LoadBgm(bgm, volume))
        PlayBgm(bgm);
}
void SSPauseBgm(Bgm *bgm)
{
    if (bgm->is_paused)
        UnPauseBgm(bgm);
    else
        PauseBgm(bgm);
}

void SSStopBgm(Bgm *bgm)
{
    StopBgm(bgm);
}