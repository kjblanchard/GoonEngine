#include <Goon/core/asset_manager.hpp>
#include <Goon/core/asset.hpp>

namespace goon
{
    std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::_loadedAssets;
    void AssetManager::ClearAssetManager()
    {
        _loadedAssets.clear();
    }

}