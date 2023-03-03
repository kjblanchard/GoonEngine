#pragma once
#include <unordered_map>
#include <memory>

namespace goon
{
    class Asset;
    class BgmAsset;
    class AssetManager
    {
    public:
        std::shared_ptr<BgmAsset> LoadBgm(std::string filename, float loopStart, float loopEnd);

    private:
        std::unordered_map<std::string, std::shared_ptr<Asset>> _loadedAssets;
    };
}