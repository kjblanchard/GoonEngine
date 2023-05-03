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
        /**
         * @brief Clears all assets in the asset manager.
         */
        static void ClearAssetManager();
        /**
         * @brief Loads an asset into the Asset manager
         *
         * @tparam T The type of asset that should be loaded
         * @tparam Args The additional argument types that are needed for this asset type, most assets only need string
         * @param filename The filename of the asset that should be loaded
         * @param args The additional arguments that will be passed into the asset constructor.
         * @return std::shared_ptr<T> A loaded ref for the Type specified.
         */
        template <typename T, typename... Args>
        static std::shared_ptr<T> LoadAsset(std::string filename, Args ...args)
        {
            static_assert(!std::is_base_of<T, Asset>::value, "Can only load assets with this, please pass proper value.");
            auto iterator = _loadedAssets.find(filename);
            if (iterator == _loadedAssets.end())
            {
                auto thing = std::make_shared<T>(filename, std::forward<Args>(args)...);
                _loadedAssets[filename] = thing;
                return thing;
            }
            else
                return std::dynamic_pointer_cast<T>(iterator->second);
        }

    private:
        /**
         * @brief All of the loaded assets, so that we don't have to reload them when another guy asks for it.
         *
         */
        static std::unordered_map<std::string, std::shared_ptr<Asset>> _loadedAssets;
    };
}