#pragma once

namespace goon
{
    /**
     * @brief Class is used as random guid for objects, so that we can save them to a scene file and reload as ENTT is not random
     *
     */
    class Guid
    {
    public:
        Guid();
        /**
         * @brief Construct a new Guid object
         *
         * @param guid The guid we should assign to this, this is used when loading from a file
         */
        Guid(uint64_t guid);
        Guid(const Guid &) = default;
        operator uint64_t() const { return _guid; }

    private:
        /**
         * @brief Guid
         *
         */
        uint64_t _guid;
    };
}

template <>
struct std::hash<goon::Guid>
{
    std::size_t operator()(const goon::Guid &guid) const
    {
        return (uint64_t)guid;
    }
};