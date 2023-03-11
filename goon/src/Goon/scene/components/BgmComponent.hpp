#pragma once
#include <Goon/core/bgm_asset.hpp>
#include <Goon/core/asset_manager.hpp>
#include <sound/sound.h>

namespace goon
{
    struct BgmComponent
    {
        BgmComponent(std::string soundFile, float loopBegin = 0.0f, float loopEnd = 0.0f, bool bg = false, float volume = 1.0f)
            : SoundFile(soundFile), LoopBegin(loopBegin), LoopEnd(loopEnd), Ambient(bg), Volume(volume)
        {
            LoadedBgm = AssetManager::LoadAsset<BgmAsset, float, float, bool>(soundFile, LoopBegin, LoopEnd, Ambient);
            LoadedBgm.get()->Load();
        }

        void UpdateSoundFileName(std::string newfile)
        {
            SoundFile = newfile;
            LoadedBgm.get()->_bgm->bgm_name = newfile.c_str();
        }

        void UpdateLoopBegin(float newBegin)
        {
            LoopBegin = newBegin;
            LoadedBgm.get()->_bgm->loop_begin = newBegin;
        }
        void UpdateLoopEnd(float newEnd)
        {
            LoopEnd = newEnd;
            LoadedBgm.get()->_bgm->loop_end = newEnd;
        }

        std::string SoundFile;
        float LoopBegin;
        float LoopEnd;
        float Volume;
        bool Ambient;
        std::shared_ptr<BgmAsset> LoadedBgm = nullptr;
    };
}