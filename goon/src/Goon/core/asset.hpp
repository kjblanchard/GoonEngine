#pragma once

namespace goon
{
    class Asset
    {

    public:
        Asset(std::string filename)
            : _filename(filename)
        {
        }
        virtual ~Asset() = default;
        virtual void Load() = 0;
        virtual void* GetAsset() const = 0;

    protected:
        std::string _filename;
    };
}