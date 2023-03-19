#pragma once
#include <gnpch.hpp>
#include <yaml-cpp/yaml.h>

namespace goon
{
    struct TagComponent
    {
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(TagComponent &&) = default;
        TagComponent(std::string &tag)
            : Tag(tag)
        {
        }
        operator std::string()
        {
            return Tag;
        }
        operator const char *()
        {
            return Tag.c_str();
        }
    };

}