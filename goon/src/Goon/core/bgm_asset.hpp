#pragma once
#include <Goon/core/asset.hpp>

class Bgm;
namespace goon
{
    class BgmAsset : public Asset
    {
    public:
        BgmAsset(std::string filename);
        virtual void Load() override;
        virtual void Destroy() override;
        Bgm *_bgm;
        // TODO remove this, just for testing.
        void Play();
        void Update();
    };
}