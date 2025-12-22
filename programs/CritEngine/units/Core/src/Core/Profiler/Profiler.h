#pragma once

#ifdef CE_DEBUG
    #define CE_PROFILE_MANUAL(name) Engine::Profile _profile##name(#name);
	#define CE_PROFILE_MANUAL_STOP(name) _profile##name.Stop();
    #define CE_PROFILE_FUNC(name) Engine::ScopedProfile _scopedprofile##name(#name);
#else
	#define CE_PROFILE_MANUAL(name)
	#define CE_PROFILE_MANUAL_STOP(name)
    #define CE_PROFILE_FUNC(name)
#endif

namespace Engine {

	class Profile {
	public:
		Profile(const std::string& name) : funcName(name), startTime(std::chrono::high_resolution_clock::now()) {}

		void ENGINE_API Stop();

	private:
		std::string funcName;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

	class ScopedProfile : public Profile {
	public:
		ScopedProfile(const std::string& name) : Profile(name) {}
		ENGINE_API ~ScopedProfile();
	};

}
