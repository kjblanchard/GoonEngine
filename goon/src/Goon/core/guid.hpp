#pragma once

namespace goon
{
    class Guid
    {
    public:
        Guid();
        Guid(uint64_t guid);
        Guid(const Guid &) = default;
        operator uint64_t() const { return _guid; }

    private:
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