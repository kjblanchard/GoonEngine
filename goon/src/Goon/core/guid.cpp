#include <random>
#include <Goon/core/guid.hpp>

namespace goon
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    Guid::Guid()
        : _guid(s_UniformDistribution(s_Engine))
    {
    }
    Guid::Guid(uint64_t guid)
    : _guid(guid)
    {
    }
}