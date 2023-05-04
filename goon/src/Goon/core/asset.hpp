#pragma once

namespace goon
{
    class Asset
    {

    public:
        /**
         * @brief Construct a new Asset object
         *
         * @param filename The file to load this from.
         */
        Asset(std::string filename)
            : _filename(filename)
        {
        }
        virtual ~Asset() = default;
        virtual void Load() = 0;
        /**
         * @brief Get the Asset object
         *
         * @return void* The asset
         */
        virtual void *GetAsset() const = 0;

        /**
         * @brief Get the Asset As object, this has not been tested.
         *
         * @tparam T The type that this should be casted to (unsafe)
         * @return T* Casted to a T pointer.
         */
        template <typename T>
        T *GetAssetAsUnsafe()
        {
            return (T *)GetAsset();
        }

    protected:
        /**
         * @brief Filename
         *
         */
        std::string _filename;
    };
}