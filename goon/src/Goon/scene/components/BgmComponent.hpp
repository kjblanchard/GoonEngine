#pragma once
#include <Goon/core/bgm_asset.hpp>
#include <Goon/core/asset_manager.hpp>

namespace goon
{
    struct BgmComponent
    {
        BgmComponent(std::string soundFile, float loopBegin = 0.0f, float loopEnd = 0.0f)
            : SoundFile(soundFile), LoopBegin(loopBegin), LoopEnd(loopEnd)
        {
            LoadedBgm = AssetManager::LoadAsset<BgmAsset, float, float>(soundFile, LoopBegin, LoopEnd);
            LoadedBgm.get()->Load();
        }

        std::string SoundFile;
        float LoopBegin;
        float LoopEnd;
        std::shared_ptr<BgmAsset> LoadedBgm = nullptr;
    };
}