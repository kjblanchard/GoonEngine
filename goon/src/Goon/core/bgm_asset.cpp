#include <gnpch.hpp>
#include <Goon/core/bgm_asset.hpp>
#include <sound/sound.h>

namespace goon
{
    BgmAsset::BgmAsset(std::string filename, float begin, float end)
        : Asset(filename), _begin(begin), _end(end)
    {
    }

    BgmAsset::~BgmAsset()
    {
        puts("bgm destroyed, no other implementation.");
    }

    void BgmAsset::Load()
    {
        InitializeSound();
        auto thing = &_filename[0];
        _bgm = LoadBgm(thing);
        UpdateBgmLoopTimes(_bgm, _begin, _end);
        PlayBgm(_bgm, 1.0f);
    }

    void BgmAsset::Update()
    {
        UpdateSound();
    }
}