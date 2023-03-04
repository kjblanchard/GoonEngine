#pragma once
#include <Goon/core/asset.hpp>

class Bgm;
namespace goon
{
    class BgmAsset : public Asset
    {
    public:
        BgmAsset(std::string filename, float begin = 0, float end = 0);
        virtual ~BgmAsset() override;
        virtual void Load() override;
        virtual void *GetAsset() const override { return _bgm; }
        Bgm *_bgm;

//TODO put this somewhere.
        void Update();

    private:
        float _begin;
        float _end;
    };
}