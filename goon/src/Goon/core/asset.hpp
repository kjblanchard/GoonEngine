#pragma once

namespace goon
{
    class Asset
    {

    protected:
        std::string _filename;

    public:
        Asset(std::string filename)
            : _filename(filename)
        {
        }
        virtual void Load() = 0;
        virtual void Destroy() = 0;
    };
}