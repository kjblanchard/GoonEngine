#pragma once
#include <Goon/core/asset.hpp>

struct Bgm;
namespace goon
{
    class BgmAsset : public Asset
    {
    public:
        BgmAsset(std::string filename, float begin = 0, float end = 0, bool ambient = false);
        virtual ~BgmAsset() override;
        virtual void Load() override;
        virtual void *GetAsset() const override { return _bgm; }
        Bgm *_bgm;

        //TODO put this somewhere.
        void Update();

    private:
        float _begin;
        float _end;
        bool _ambient;
    };
}