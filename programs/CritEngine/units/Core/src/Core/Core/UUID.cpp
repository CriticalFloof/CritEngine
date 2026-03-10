#include "UUID.h"

#include <random>
#include <unordered_map>

namespace Engine
{
    static std::random_device g_random_device;
    static auto g_mersenne = std::mt19937_64(g_random_device());
    static std::uniform_int_distribution<uint64_t> g_uniform_distribution;

    UUID::UUID()
        : m_uuid(g_uniform_distribution(g_mersenne))
    {
    }

    UUID::UUID(uint64_t uuid)
        : m_uuid(uuid)
    {
    }
}
