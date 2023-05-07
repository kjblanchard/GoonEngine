#pragma once
#include <Goon/core/asset.hpp>

struct Bgm;
namespace goon
{
    class BgmAsset : public Asset
    {
    public:
        /**
         * @brief Construct a new Bgm Asset object
         *
         * @param filename The file to load this from
         * @param begin Where to start from on a loop
         * @param end Where to end and go back to begin
         * @param ambient If this should be played on the background sound channel or not.
         */
        BgmAsset(std::string filename, float begin = 0, float end = 0, bool ambient = false);
        virtual ~BgmAsset() override;
        virtual void Load() override;
        virtual void *GetAsset() const override { return _bgm; }
        Bgm *_bgm;

    private:
        float _begin;
        float _end;
        bool _ambient;
    };
}