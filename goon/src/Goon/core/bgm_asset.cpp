#include <gnpch.hpp>
#include <Goon/core/bgm_asset.hpp>
#include <sound/sound.h>

namespace goon
{
    BgmAsset::BgmAsset(std::string filename)
        : Asset(filename)
    {
    }
    void BgmAsset::Load()
    {
        InitializeSound();
        auto thing = &_filename[0];
        _bgm = LoadBgmFromLua(thing, 0, 30);
    }
    void BgmAsset::Destroy()
    {
    }
    void BgmAsset::Play()
    {
        PlayBgm(_bgm, 1.0);
    }
    void BgmAsset::Update()
    {
        UpdateSound();
    }
}