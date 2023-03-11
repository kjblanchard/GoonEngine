#include <gnpch.hpp>
#include <Goon/core/bgm_asset.hpp>
#include <sound/sound.h>

namespace goon
{
    static bool loaded_sound = false;
    BgmAsset::BgmAsset(std::string filename, float begin, float end, bool ambient)
        : Asset(filename), _begin(begin), _end(end), _ambient(ambient)
    {
    }

    BgmAsset::~BgmAsset()
    {
        puts("bgm destroyed, no other implementation.");
    }

    void BgmAsset::Load()
    {
        if (!loaded_sound)
        {
            InitializeSound();
            loaded_sound = true;
        }
        auto thing = &_filename[0];
        _bgm = NewBgm(thing);
        // Set values for bgm when we load.
        _bgm->loop_begin = _begin;
        _bgm->loop_end = _end;
        _bgm->is_ambient = (int)_ambient;
        _bgm->loops = -1;
        // float volume = _bgm->is_ambient ? 1.0 : 0.1;
        // Load and start playback here for now
        // LoadBgm(_bgm, volume);
        // PlayBgm(_bgm);
    }
}