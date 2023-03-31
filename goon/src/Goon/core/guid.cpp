#include <random>
#include <Goon/core/guid.hpp>

namespace goon
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    /**
     * @brief Construct a new Guid:: Guid object, with a random guid, this is used when creating new objects in the scene
     *
     */
    Guid::Guid()
        : _guid(s_UniformDistribution(s_Engine))
    {
    }
    /**
     * @brief Construct a new Guid:: Guid object with a specific guid, for loading from a scene.
     *
     * @param guid The guid that the object should have.
     */
    Guid::Guid(uint64_t guid)
        : _guid(guid)
    {
    }
}