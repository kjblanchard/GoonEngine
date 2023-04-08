#pragma once

namespace goon
{
    struct InactiveComponent
    {
        InactiveComponent() = default;
        InactiveComponent(const InactiveComponent&) = default;
        InactiveComponent(InactiveComponent&&) = default;
    };
}