#pragma once

namespace goon
{
    enum class ActionTypes
    {
        Default,
        ValueChange,
        GameObjectCreation,
        GameObjectDeletion,

    };


    template<class T>
    struct Action
    {
        Action(T previous, T newVal, T& ref)
        : PreviousValue(previous), NewValue(newVal), ValueRef(ref)
        {
        }
        T PreviousValue;
        T NewValue;
        T& ValueRef;
        ActionTypes ActionType;

    };
}