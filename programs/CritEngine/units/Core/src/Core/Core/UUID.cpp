#include "UUID.h"

#include <random>
#include <unordered_map>

namespace Engine {

	static std::random_device randomDevice;
	static std::mt19937_64 mersenne = std::mt19937_64(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	UUID::UUID()
		: uuid(uniformDistribution(mersenne)) {}

	UUID::UUID(uint64_t uuid)
		: uuid(uuid) {}

}