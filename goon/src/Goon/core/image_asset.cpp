#include <Goon/core/image_asset.hpp>
#include <Goon/core/Log.hpp>
#include <stb_image.h>

namespace goon
{
    ImageAsset::ImageAsset(std::string filename)
        : Asset(filename), _width(0), _height(0), _bytesPerPixel(0)
    {
    }

    ImageAsset::~ImageAsset()
    {
        stbi_image_free(_data);
        GN_CORE_TRACE("Asset {} is being destroyed", _filename);
    }

    void ImageAsset::Load()
    {
        _data = stbi_load(_filename.c_str(), &_width, &_height, &_bytesPerPixel, 0);
        if (_data)
            GN_CORE_TRACE("Asset {} is loaded", _filename);
        else
            GN_CORE_ERROR("Could not load asset {}", _filename);
    }

}