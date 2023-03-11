#pragma once
#include <Goon/core/bgm_asset.hpp>
#include <Goon/core/asset_manager.hpp>

namespace goon
{
    struct BgmComponent
    {
        BgmComponent(std::string soundFile, float loopBegin = 0.0f, float loopEnd = 0.0f, bool bg = false)
            : SoundFile(soundFile), LoopBegin(loopBegin), LoopEnd(loopEnd), background(bg)
        {
            LoadedBgm = AssetManager::LoadAsset<BgmAsset, float, float, bool>(soundFile, LoopBegin, LoopEnd, background);
            LoadedBgm.get()->Load();
        }

        std::string SoundFile;
        float LoopBegin;
        float LoopEnd;
        bool background;
        std::shared_ptr<BgmAsset> LoadedBgm = nullptr;
    };
}