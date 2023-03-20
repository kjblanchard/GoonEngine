#pragma once

#include <Goon/core/guid.hpp>

namespace goon
{
    /**
     * @brief Component that contains a guid, this is used to identify objects in the game, and can be serialized / deserialized in a scene yml file.
     *
     */
    struct IdComponent
    {
        IdComponent() = default;
        IdComponent(const IdComponent &) = default;
        IdComponent(IdComponent &&) = default;
        Guid Guid;
    };
}