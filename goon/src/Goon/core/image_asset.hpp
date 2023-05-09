#pragma once
#include <Goon/core/asset.hpp>

namespace goon
{
    class ImageAsset : public Asset
    {
    public:
        ImageAsset(std::string filename);
        virtual ~ImageAsset() override;
        virtual void Load() override;
        virtual void *GetAsset() const override { return _data; }
        unsigned char *GetLoadedData() { return _data; }
        int GetWidth() const { return _width; }
        int GetHeight() const { return _height; }
        int GetChannels() const { return _bytesPerPixel; }

    private:
        unsigned char *_data;
        int _width;
        int _height;
        int _bytesPerPixel;
    };
}