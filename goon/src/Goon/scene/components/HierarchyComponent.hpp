#pragma once
#include <gnpch.hpp>
#include <entt/entity/registry.hpp>

namespace goon
{
    struct HierarchyComponent
    {
        entt::entity FirstChild, PreviousChild, NextChild, Parent;
        // ctors
        HierarchyComponent()
        {
            FirstChild = PreviousChild = NextChild = Parent = entt::null;
        }
        HierarchyComponent(const HierarchyComponent &) = default;
        HierarchyComponent(HierarchyComponent &) = default;
        HierarchyComponent(HierarchyComponent &&) = default;
        // Copy assignment
        HierarchyComponent& operator= ( const HierarchyComponent & ) = default;
    };

}