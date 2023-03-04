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
        _bgm = LoadBgmFromLua(thing, _begin, _end);
    }
        void BgmAsset::Update()
        {
            UpdateSound();
        }
}