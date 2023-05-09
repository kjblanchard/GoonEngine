#pragma once
#include <string>

namespace goon
{
    class Texture
    {
    public:
        Texture(std::string &filename) : _filename(filename) {}
        static Texture *Create(std::string &filename);

        virtual void Bind() = 0;
        virtual void Load() = 0;

    protected:
        unsigned int id;
        std::string _filename;
    };
}